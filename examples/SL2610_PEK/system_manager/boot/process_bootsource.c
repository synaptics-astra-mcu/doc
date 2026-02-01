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

#include "syna_device.h"
#include "logger.h"
#include "process_bootsource.h"

sm_status_en handle_cold_boot(void)
{
    sm_status_en rc = SM_STATUS_OK;
    uint32_t boot_src;

    /* Check boot source */
    boot_src = check_boot_src();
    boot_src = boot_src & 0x3;

    if (boot_src == 0x1) {
        rc = xspi_boot_flow();
    } else if (boot_src == 0x2) {
        xspi_soc_init();
        rc = emmc_boot_flow();
    } else {
        LOG_ERROR(LOG_MOD_SYSTEM, "Invalid boot_src: 0x%x\n", boot_src);
        return -1;
    }

    return rc;
}
