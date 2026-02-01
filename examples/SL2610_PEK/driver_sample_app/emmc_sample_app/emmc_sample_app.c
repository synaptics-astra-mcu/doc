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

/****************************** Include files *********************************/

#include "emmc_sample_app.h"
#include "logger.h"

/**************************** Macro definitions *******************************/

#define SOCREG_REGION(addr)         ((addr-0xF0000000 + 0xD0000000))
#define DDR_REGION_MCU2SOC(addr)    (addr - 0xB0000000)

#define SAMPLE_APP_STACK_DEPTH     2048
/*************************** Variable Definitions *****************************/

StackType_t  xStackSampleApp[SAMPLE_APP_STACK_DEPTH];
StaticTask_t xTaskBufferSampleApp;

#define BUF_LENGTH 512

/**
 * \brief  EMMC Sample Application
 *
 * \details Demonstrates basic eMMC initialization, read, and write operations.
 *          The application writes a fixed pattern to a buffer, writes it to
 *          eMMC storage, reads it back, and verifies data integrity by comparing
 *          the read buffer with the original write buffer. Status messages are
 *          logged throughout the process to aid in debugging and validation.
 *
 *
 * \return      None
 */

void emmc_sample_app_main(void)
{
    emmc_error_code_t ret;
    int i;
    uint8_t *rd_buff = (uint8_t*)0xB1A00000;
    uint8_t *wr_buff = (uint8_t*)0xB1B00000;

    LOG_INFO(LOG_MOD_EMMC, "Starting EMMC Sample App! \n");

    for (i = 0; i < BUF_LENGTH; i++)
    {
        wr_buff[i] = 0xAB;
    }
    ret = emmc_init(EMMC_INSTANCE_0);
    if (ret != NO_ERROR) {
        LOG_INFO(LOG_MOD_EMMC, "emmc init failed errcode:%d \n", ret);
        return;
    }

    ret = emmc_read_bytes(EMMC_INSTANCE_0, 0, BUF_LENGTH, (uint8_t*)DDR_REGION_MCU2SOC((uintptr_t)rd_buff));
    if (ret != NO_ERROR) {
        LOG_INFO(LOG_MOD_EMMC, "emmc write failed errcode:%d \n", ret);
    }

    ret = emmc_write_bytes(EMMC_INSTANCE_0, 0, BUF_LENGTH, (uint8_t*)DDR_REGION_MCU2SOC((uintptr_t)wr_buff));
    if (ret != NO_ERROR) {
        LOG_INFO(LOG_MOD_EMMC, "emmc write failed errcode:%d \n", ret);
    }

    ret = emmc_read_bytes(EMMC_INSTANCE_0, 0, BUF_LENGTH, (uint8_t*)DDR_REGION_MCU2SOC((uintptr_t)rd_buff));
    if (ret != NO_ERROR) {
        LOG_INFO(LOG_MOD_EMMC, "emmc write failed errcode:%d \n", ret);
    }

    for (i = 0; i < BUF_LENGTH; i++)
    {
        if(rd_buff[i] != wr_buff[i])
        {
            LOG_ERROR(LOG_MOD_EMMC, "EMMC Read/Write failed\n");
            break;
        }
    }
    if (i == BUF_LENGTH) {
        LOG_INFO(LOG_MOD_EMMC, "EMMC Read/Write successfull \n");
    }
    LOG_INFO(LOG_MOD_UART, "EMMC Sample App Completed!\n");

    while (1)
    {
        ;
    }

    return;
}

/**
 * Task wrapper for eMMC test
 */
void emmc_sample_task(void *pvParameters)
{
    emmc_sample_app_main();
    vTaskDelete(NULL);
}

/**
 * Application entry point to create eMMC sample task
 */
void emmc_sample_app_task(void)
{
    xTaskCreateStatic(
        emmc_sample_task,
        "EMMC_SAMPLE_APP",
        SAMPLE_APP_STACK_DEPTH,
        NULL,
        tskIDLE_PRIORITY + 1,
        xStackSampleApp,
        &xTaskBufferSampleApp
    );
}
