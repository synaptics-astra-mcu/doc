/**
* SPDX-License-Identifier: Apache-2.0
*
* Copyright 2025 Synaptics Incorporated
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**
 * @file    process_bootsource.c
 *
 * @brief    Process boot source, load images from eMMC or XSPI based on boot source
 */

//****************************** Include files *********************************

#include <stdint.h>
#include "logger.h"
#include "emmc.h"
#include "image_parser.h"
#include "image_header.h"
#include "gpt_parser.h"
#include "seclib.h"

extern int __preload_buf_start;
extern int __img_buff_start;

int32_t read_tzk_image_container(const char *pt_name, image_info_header_t *img_hdr)
{
    uint8_t *Image;
    int32_t rc;

    rc = read_image_by_ptname(pt_name, sizeof(image_info_header_t), img_hdr);
    if (rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"EMMC read hdr failed to read %d bytes ret:%d\n", img_hdr->length, rc);
        return -1;
    }

    if( img_hdr->image_type != IMAGE_ID_OPTEE_CONTAINER_HEADER ||
        img_hdr->image_format_version != 1) {
        LOG_ERROR(LOG_MOD_SYSTEM,"No valid image from eMMC device ImageType:%d\n", img_hdr->image_type);
        return -1;
    }

    return 0;
}

int32_t flash_load_image(const char *pt_name, uint8_t **pImage)
{
    imager_header_t *img_hdr = &__img_buff_start;
    uint8_t *Image;
    int32_t rc;

    rc = read_image_by_ptname(pt_name, sizeof(imager_header_t), img_hdr);
    if (rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"EMMC read hdr failed to read %d bytes ret:%d\n", img_hdr->Length, rc);
        *pImage = NULL;
        return -1;
    }

    Image = (uint8_t *)&__img_buff_start;
    rc = read_image_by_ptname(pt_name, img_hdr->Length, (void*)Image);
    if (rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"EMMC read failed to read %d bytes ret:%d\n", img_hdr->Length, rc);
        *pImage = NULL;
        return -1;
    }

    *pImage = Image;
    return 0;
}

int32_t load_image(const char *pLoadImage, uint32_t ImageId, uint32_t image_offset)
{
    uint8_t *pImage = NULL;
    int32_t rc = 0;
    psa_status_t psa_status;

    rc = flash_load_image(pLoadImage, &pImage);
    if (!rc) {
        LOG_INFO(LOG_MOD_SYSTEM, "Loading %s\n", pLoadImage);
        psa_status = sec_lib_verify_sw_image(ImageId, SSCHEME_SHA512_ECDSA_P521, (uint8_t*)pImage, (uint8_t*)&pImage[image_offset]);
        if (psa_status) {
            LOG_ERROR(LOG_MOD_SYSTEM, "%s image verify failed psa_status: %d\n", pLoadImage, psa_status);
            return psa_status;
        }
    }
    return rc;
}

int32_t emmc_boot_flow(void)
{
    int32_t rc, partition = 0;
    image_info_header_t *img_hdr = &__img_buff_start;

    LOG_INFO(LOG_MOD_SYSTEM, "eMMC: Boot A55\n");

    //SCB_EnableDCache();
    rc = emmc_init(EMMC_INSTANCE_0);
    if(rc) {
        LOG_ERROR(LOG_MOD_SYSTEM, "Failed to init eMMC device, rc: %d\n", rc);
        return -1;
    }

    rc = emmc_switch_part(EMMC_INSTANCE_0, partition);
    if(rc) {
        LOG_ERROR(LOG_MOD_SYSTEM, "Failed to switch eMMC partition %d, rc: %d\n", partition, rc);
        return -1;
    }

    //buffer size must larger than 0x6800
    //the gpt buff address should be 64byte align for DMA transfer
    rc = get_partition_info((void *)&__preload_buf_start);
    if(rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"get gpt error!, unable to jump to SM!\n");
        return -1;
    } else {
        //dump_gpt_table();
    }

    rc = load_image(BL_A_NAME, IMAGE_ID_UBOOT, sizeof(uboot_image_t));
    if (rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"failed to load %s image rc %d\n", BL_A_NAME, rc);
        return -1;
    }

    rc = load_image(TZK_A_NAME, IMAGE_ID_OPTEE_CONTAINER_HEADER, sizeof(image_info_header_t));
    if (rc) {
        LOG_ERROR(LOG_MOD_SYSTEM,"failed to load %s image rc %d\n", TZK_A_NAME, rc);
        return -1;
    }

    /* Allow ACPU to handle EMMC interrupt by disabling it */
    emmc_intr_enable(EMMC_INSTANCE_0, false);
    return 0;
}
