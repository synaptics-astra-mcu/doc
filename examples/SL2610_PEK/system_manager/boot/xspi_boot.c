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

#include <stdint.h>
#include "syna_device.h"
#include "logger.h"
#include "image_parser.h"
#include "image_header.h"
#include "gpt_parser.h"
#include "seclib.h"
#include "flash.h"
#include "flashMemmap.h"

#define ENTRIES_BUF_LEN               (32)
#define MAX_BOOT_PARTITION_NUM        2

extern int __preload_buf_start;
extern int __img_buff_start;

static int32_t get_flash_partition_info(void);

typedef struct
{
    uint32_t Reserved_1;
    uint32_t SPK_Boot_Index_Partition;
    uint32_t MCU_Boot_Index_Partition;
    uint32_t Partition_Table_A_Offset;
    uint32_t Partition_Table_B_Offset;
    uint32_t Partition_table_AP_partA_offset;
    uint32_t Partition_table_AP_partB_offset;
    uint32_t Partition_enties;
} flash_attr_ext_st;

typedef struct {
    flash_attr_ext_st *p_flash_attr;
    uint32_t flash_attr_A_offset;
    uint32_t flash_attr_B_offset;
    Flash_Partition_Table *flash_layout[MAX_BOOT_PARTITION_NUM];
    uint8_t flash_attr_offset;
    uint32_t flash_magic_number;
    uint32_t curr_partition;
} flash_image_info_st;

static flash_image_info_st flash_image_info = {.flash_magic_number = 0x30414C54,
                    .flash_attr_A_offset = 0x0000,
                    .flash_attr_B_offset = 0x1000,
                    .flash_attr_offset = 0x70,
                    .curr_partition = 0, };


static uint32_t flash_direct_read(int32_t offset)
{
    if(offset < 0 || offset > (XSPI_XIP_END_ADDRESS - XSPI_XIP_BASE_ADDRESS))
        return 0;

    return (uint32_t)(XSPI_XIP_BASE_ADDRESS + offset);
}

static uint32_t get_partition_indicator( uint32_t indic_addr_offset)
{
    uint32_t *entry_prt = NULL;
    uint32_t indic_addr = 0;

    typedef struct _entry_table_t {
        uint16_t index;
        uint16_t valid;
    } entry_table_t;

    entry_table_t entry_buf[ ENTRIES_BUF_LEN ];

    for (int i = 0; i < (BOOT_PARTITION_ENTRIES_LEN / ENTRIES_BUF_LEN); i++) {

        memset( (uint8_t *)entry_buf, 0, sizeof(entry_buf) );

        indic_addr = indic_addr_offset + ( i * sizeof(entry_buf) );
        LOG_INFO(LOG_MOD_SYSTEM, "  indic \t 0x%x\n", indic_addr);
        entry_prt = flash_direct_read(indic_addr);
        memcpy(entry_buf, entry_prt, sizeof(entry_buf));

        for (int j = 0; j < ENTRIES_BUF_LEN; j++) {
            if (BOOT_PARTITION_ENTRY_VALID == entry_buf[j].valid) {
                switch (entry_buf[j].index) {
                case BOOT_PARTITION_A_MAGIC:
                    LOG_INFO(LOG_MOD_SYSTEM, "entry: %d partition: 0\n",j);
                    return 0; break;
                case BOOT_PARTITION_B_MAGIC:
                    LOG_INFO(LOG_MOD_SYSTEM, "entry: %d partition: 1\n",j);
                    return 1; break;
                default:
                    LOG_INFO(LOG_MOD_SYSTEM, "j: %d p: INVALID (0x%x), use 0 \n",j, entry_buf[j].index);
                    return 0; break;
                }
            }
        }
    }

    LOG_INFO(LOG_MOD_SYSTEM, "entry: x partition: NOTFOUND, use 0\n");
    return 0; /* default partition A */
}

static int32_t flash_update_partition_info(void)
{
    uint8_t *part_info = NULL;
    uint32_t *magic_number = NULL;
    flash_image_info_st *image_ctx = &flash_image_info;

    part_info = (uint8_t*)flash_direct_read(image_ctx->flash_attr_A_offset);
    magic_number = (uint32_t*)part_info;
    if(part_info && *magic_number == image_ctx->flash_magic_number) {
        //LOG_INFO(LOG_MOD_SYSTEM, "Flash attribute A offset: %X\n", image_ctx->flash_attr_A_offset);
        image_ctx->p_flash_attr = &part_info[image_ctx->flash_attr_offset];
        return 0;
    }

    part_info = (uint8_t*)flash_direct_read(image_ctx->flash_attr_B_offset);
    magic_number = (uint32_t*)part_info;
    if(part_info && *magic_number == image_ctx->flash_magic_number) {
        //LOG_INFO(LOG_MOD_SYSTEM, "Flash attribute B offset: %X\n", image_ctx->flash_attr_B_offset);
        image_ctx->p_flash_attr = &part_info[image_ctx->flash_attr_offset];
        return 0;
    }
    LOG_INFO(LOG_MOD_SYSTEM, "Flash magic number not found 0x%08X:0x%08X\n", magic_number, *magic_number);
    return -1;
}

static int32_t get_flash_partition_info(void)
{
    uint32_t partn_addr = 0;
    uint32_t partn_count = 0;
    flash_image_info_st *image_ctx = &flash_image_info;
    Flash_Partition_Table *pt = NULL;

    /* Get the partition_table/s */
    if (flash_update_partition_info() != 0) {
        LOG_INFO(LOG_MOD_SYSTEM, "Flash partition magic failed\n");
        return -1;
    }

    /* partition A MUST be available */
    if (0xFFFFFFFF != image_ctx->p_flash_attr->Partition_Table_A_Offset && image_ctx->p_flash_attr->Partition_Table_A_Offset != 0) {
        partn_addr =  image_ctx->p_flash_attr->Partition_Table_A_Offset;
        //LOG_INFO(LOG_MOD_SYSTEM, "  pat a \t 0x%x\n", partn_addr);
        image_ctx->flash_layout[0] = flash_direct_read(partn_addr);
        partn_count++;
    }

    /* Partition B, may or may not be there */
    if (0xFFFFFFFF != image_ctx->p_flash_attr->Partition_Table_B_Offset && image_ctx->p_flash_attr->Partition_Table_B_Offset != 0) {
        partn_addr =  image_ctx->p_flash_attr->Partition_Table_B_Offset;
        //LOG_INFO(LOG_MOD_SYSTEM, "  pat b \t 0x%x\n", partn_addr);
        image_ctx->flash_layout[1] = flash_direct_read(partn_addr);
        partn_count++;
    }

    /* Get the currently indicated partition only if there are more than 1 partitions */
    if (partn_count > 1) {
        image_ctx->curr_partition = get_partition_indicator(image_ctx->p_flash_attr->MCU_Boot_Index_Partition);
    } else {
        /* only 1 partition so current partition is 0*/
        image_ctx->curr_partition = 0;
    }
    return 0;
}

static uint32_t get_image_offset(uint32_t image_id)
{
    for (int i = 0; i < flash_image_info.p_flash_attr->Partition_enties; i++) {
        if (flash_image_info.flash_layout[flash_image_info.curr_partition]->pt[i].flash_type == image_id) {
            return flash_image_info.flash_layout[flash_image_info.curr_partition]->pt[i].offset;
        }
    }
    LOG_INFO(LOG_MOD_SYSTEM, "Invalid image id 0x%X\n", image_id);
    return 0;
}

/**
 * @fn void p_flash_load_bin(void)
 * @brief Command parser for Boot from Flash
 *
 */
static void p_flash_load_bin(uint32_t _flash_offset)
{
    imager_header_t img_hdr;
    uint32_t image_body_offset;
    uint32_t image_body_size;

    flash_read(FLASH_INSTANCE_0, _flash_offset, (uint8_t*)&img_hdr, sizeof(imager_header_t));
    LOG_INFO(LOG_MOD_SYSTEM, "_flash_offset %08X img_hdr.Length 0x%04X\n", _flash_offset, img_hdr.Length);
    _flash_offset += sizeof(imager_header_t);

    image_body_size = img_hdr.Length - sizeof(imager_header_t);
    image_body_offset = (uint32_t)_flash_offset;
    LOG_INFO(LOG_MOD_SYSTEM, "image_body_offset %08X dest.addr %08X size 0x%04X\n", image_body_offset, img_hdr.Destination_address, image_body_size);
    flash_read(FLASH_INSTANCE_0, image_body_offset, (uint8_t*)img_hdr.Destination_address, image_body_size); // Copy the image to the destination address
}

static void flash_direct_read_image(uint32_t offset, uint32_t image_id, uint8_t **pImage)
{
    imager_header_t *img_hdr;
    uint8_t *Image;

    if (offset == 0) {
        LOG_INFO(LOG_MOD_SYSTEM, "Invalid offset\n");
        return;
    }

    img_hdr = flash_direct_read(offset);
    if (img_hdr && img_hdr->Image_ID != image_id) {
        LOG_INFO(LOG_MOD_SYSTEM, "no valid image, ImageID:%d @ 0x%04X\n", img_hdr->Image_ID, offset);
        *pImage = NULL;
        return;
    }

    //LOG_INFO(LOG_MOD_SYSTEM, "ImagId:%x Length:%x\n", img_hdr->Image_ID, img_hdr->Length);
    Image = (uint8_t *)&__img_buff_start;
    memcpy(Image, flash_direct_read(offset), img_hdr->Length);
    *pImage = Image;
}

int32_t xspi_boot_flow(void)
{
    uint32_t rc, l;
    uint8_t *pImage;
    psa_status_t psa_status;

    LOG_INFO(LOG_MOD_SYSTEM, "XSPI: Boot A55\n");

    rc = flash_init(FLASH_INSTANCE_0, 0, SPI_NOR);
    if(rc) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to init XSPI device\n");
        return -1;
    }

    rc = flash_configure_direct_mode(FLASH_INSTANCE_0);
    if(rc) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to configure XSPI device\n");
        return -1;
    }

    if (get_flash_partition_info() != 0) {
        LOG_INFO(LOG_MOD_SYSTEM, "Flash partition magic failed\n");
        return -1;
    }

    if(get_image_offset(IMAGE_ID_UBOOT) == 0) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to load image offset: %X\n", get_image_offset(IMAGE_ID_UBOOT));
        return -1;
    }

    flash_direct_read_image(get_image_offset(IMAGE_ID_UBOOT), IMAGE_ID_UBOOT, &pImage);
    if(pImage == NULL) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to load image\n");
        return -1;
    }

    LOG_INFO(LOG_MOD_SYSTEM, "Loading %s...\n", BL_A_NAME);
    psa_status = sec_lib_verify_sw_image(IMAGE_ID_UBOOT, SSCHEME_SHA512_ECDSA_P521, (uint8_t*)pImage, (uint8_t*)&pImage[sizeof(uboot_image_t)]);
    if (psa_status) {
        LOG_ERROR(LOG_MOD_SYSTEM, "%s image verify failed psa_status: %d\n", BL_A_NAME, psa_status);
        //return psa_status;
    }

    if(get_image_offset(IMAGE_ID_OPTEE_CONTAINER_HEADER) == 0) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to load image offset: %X\n", get_image_offset(IMAGE_ID_OPTEE_CONTAINER_HEADER));
        return -1;
    }

    flash_direct_read_image(get_image_offset(IMAGE_ID_OPTEE_CONTAINER_HEADER), IMAGE_ID_OPTEE_CONTAINER_HEADER, &pImage);
    if(pImage == NULL) {
        LOG_INFO(LOG_MOD_SYSTEM, "Failed to load image\n");
        return -1;
    }

    LOG_INFO(LOG_MOD_SYSTEM, "Loading %s...\n", TZK_A_NAME);
    psa_status = sec_lib_verify_sw_image(IMAGE_ID_OPTEE_CONTAINER_HEADER, SSCHEME_SHA512_ECDSA_P521, (uint8_t*)pImage, (uint8_t*)&pImage[sizeof(image_info_header_t)]);
    if (psa_status) {
        LOG_ERROR(LOG_MOD_SYSTEM, "%s image verify failed psa_status: %d\n", TZK_A_NAME, psa_status);
        return psa_status;
    }

    return 0;
}