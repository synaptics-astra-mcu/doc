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
*/

/****************************** Include files *********************************/
#include "xspi_sample_app.h"
#include "logger.h"
#include "cache.h"
#include "flash.h"
#include "FreeRTOS.h"
#include <stdlib.h>

/**************************** Macro definitions *******************************/
#define FLASH_TEST_BUFFER_SIZE     0x200   /* 512 bytes */

/**************************** Globals *****************************************/
static uint32_t mismatch_count;

/**************************** Helper Functions ********************************/
static bool verify_buffers(const uint8_t *exp,
                           const uint8_t *act,
                           uint32_t len)
{
    mismatch_count = 0;

    for (uint32_t i = 0; i < len; i++) {
        if (exp[i] != act[i]) {
            if (mismatch_count < 5) {
                LOG_ERROR(LOG_MOD_FLASH,
                          "[FLASH] Data mismatch @%u: exp=0x%02X act=0x%02X\n",
                          i, exp[i], act[i]);
            }
            mismatch_count++;
        }
    }

    if (mismatch_count > 5) {
        LOG_ERROR(LOG_MOD_FLASH,
                  "[FLASH] Total mismatches: %u\n", mismatch_count);
    }

    return (mismatch_count == 0);
}

static bool verify_erase(const uint8_t *buf, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        if (buf[i] != 0xFF) {
            LOG_ERROR(LOG_MOD_FLASH,
                      "[FLASH] Erase verify failed @%u: 0x%02X\n",
                      i, buf[i]);
            return false;
        }
    }
    return true;
}

/**************************** Flash Tests *************************************/
static int32_t flash_test_init(void)
{
    int32_t rc = flash_init(0, 0, SPI_NOR);
    if (rc != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Init failed rc=%d\n", rc);
        return FLASH_TEST_RC_FAIL;
    }

    LOG_INFO(LOG_MOD_FLASH, "[FLASH] Init successful\n");
    return FLASH_TEST_RC_OK;
}

static int32_t flash_read_device_id(void)
{
    uint32_t jedec_id;
    int32_t rc = flash_read_jedec_id(0, 0, &jedec_id);

    if (rc != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] JEDEC read failed rc=%d\n", rc);
        return FLASH_TEST_RC_FAIL;
    }

    LOG_INFO(LOG_MOD_FLASH,
             "[FLASH] JEDEC ID: 0x%06X (MFG=0x%02X)\n",
             jedec_id, jedec_id & 0xFF);

    return FLASH_TEST_RC_OK;
}

static int32_t flash_test_throughput(uint8_t io_width, uint8_t mode)
{
    uint8_t *wr = NULL;
    uint8_t *rd = NULL;
    uint32_t addr = 0;
    TickType_t t_start, t_end;

    LOG_INFO(LOG_MOD_FLASH,
             "[FLASH] RW throughput test (IO=%d-bit, Mode=%s)\n",
             1 << io_width,
             mode == FLASH_SDR ? "SDR" : "DDR");

    wr = malloc(FLASH_TEST_BUFFER_SIZE);
    rd = malloc(FLASH_TEST_BUFFER_SIZE);

    if (!wr || !rd) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Buffer allocation failed\n");
        goto fail;
    }

    for (uint32_t i = 0; i < FLASH_TEST_BUFFER_SIZE; i++) {
        wr[i] = (uint8_t)i;
        rd[i] = 0;
    }

    if (flash_set_bus_mode(0, io_width, mode) != ARM_DRIVER_OK ||
        flash_configure_direct_mode(0) != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Bus configuration failed\n");
        goto fail;
    }

    if (flash_erase(0, addr, FLASH_TEST_BUFFER_SIZE) != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Erase failed\n");
        goto fail;
    }

    if (flash_read(0, addr, rd, FLASH_TEST_BUFFER_SIZE) != ARM_DRIVER_OK ||
        !verify_erase(rd, FLASH_TEST_BUFFER_SIZE)) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Erase verification failed\n");
        goto fail;
    }

    t_start = xTaskGetTickCount();
    if (flash_write(0, addr, wr, FLASH_TEST_BUFFER_SIZE) != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Write failed\n");
        goto fail;
    }
    t_end = xTaskGetTickCount();

    LOG_INFO(LOG_MOD_FLASH,
             "[FLASH] Write: %u bytes in %u ms\n",
             FLASH_TEST_BUFFER_SIZE,
             (t_end - t_start) * portTICK_PERIOD_MS);

    t_start = xTaskGetTickCount();
    if (flash_read(0, addr, rd, FLASH_TEST_BUFFER_SIZE) != ARM_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Read failed\n");
        goto fail;
    }
    t_end = xTaskGetTickCount();

    LOG_INFO(LOG_MOD_FLASH,
             "[FLASH] Read : %u bytes in %u ms\n",
             FLASH_TEST_BUFFER_SIZE,
             (t_end - t_start) * portTICK_PERIOD_MS);

    if (!verify_buffers(wr, rd, FLASH_TEST_BUFFER_SIZE)) {
        LOG_ERROR(LOG_MOD_FLASH, "[FLASH] Data verification failed\n");
        goto fail;
    }

    LOG_INFO(LOG_MOD_FLASH, "[FLASH] RW throughput test passed\n");

    free(wr);
    free(rd);
    return FLASH_TEST_RC_OK;

fail:
    free(wr);
    free(rd);
    return FLASH_TEST_RC_FAIL;
}

int32_t flash_test_rw_perf(void)
{
    int32_t ret_sts = FLASH_TEST_RC_OK;
    int32_t rc;

    LOG_INFO(LOG_MOD_FLASH, "Flash RW throughput test started\n");

    /* IO1 SDR */
    LOG_INFO(LOG_MOD_FLASH, "Testing throughput: IO=%d-bit, Mode=SDR\n", 1 << FLASH_IO1);
    rc = flash_test_throughput(FLASH_IO1, FLASH_SDR);
    if (rc != FLASH_TEST_RC_OK) {
        LOG_INFO(LOG_MOD_FLASH, "Throughput test FAILED: IO=%d-bit, Mode=SDR, rc=%d\n", 1 << FLASH_IO1, rc);
        ret_sts = rc;
    } else {
        LOG_INFO(LOG_MOD_FLASH, "Throughput test PASSED: IO=%d-bit, Mode=SDR\n", 1 << FLASH_IO1);
    }

    /* IO4 SDR */
    LOG_INFO(LOG_MOD_FLASH, "Testing throughput: IO=%d-bit, Mode=SDR\n", 1 << FLASH_IO4);
    rc = flash_test_throughput(FLASH_IO4, FLASH_SDR);
    if (rc != FLASH_TEST_RC_OK) {
        LOG_INFO(LOG_MOD_FLASH, "Throughput test FAILED: IO=%d-bit, Mode=SDR, rc=%d\n", 1 << FLASH_IO4, rc);
        ret_sts = rc;
    } else {
        LOG_INFO(LOG_MOD_FLASH, "Throughput test PASSED: IO=%d-bit, Mode=SDR\n", 1 << FLASH_IO4);
    }


    LOG_INFO(LOG_MOD_FLASH, "Flash RW throughput test completed, final status=%d\n", ret_sts);

    return ret_sts;
}


/**************************** Application Entry ********************************/
void xspi_sample_app_main(void)
{
    LOG_INFO(LOG_MOD_FLASH, "Starting XSPI Sample Application...\n");

    cache_disable(CACHE_TYPE_DATA);

    LOG_INFO(LOG_MOD_FLASH, "Starting Flash Initialization Test...\n");
    if (flash_test_init() == FLASH_TEST_RC_OK) {
        LOG_INFO(LOG_MOD_FLASH, "Flash Initialization Test Passed.\n");
    } else {
        LOG_ERROR(LOG_MOD_FLASH, "Flash Initialization Test Failed!\n");
    }

    LOG_INFO(LOG_MOD_FLASH, "Starting Read Device ID Test...\n");
    if (flash_read_device_id() == FLASH_TEST_RC_OK) {
        LOG_INFO(LOG_MOD_FLASH, "Read Device ID Test Passed.\n");
    } else {
        LOG_ERROR(LOG_MOD_FLASH, "Read Device ID Test Failed!\n");
    }

    LOG_INFO(LOG_MOD_FLASH, "Starting Read/Write Throughput Test...\n");
    if (flash_test_rw_perf() == FLASH_TEST_RC_OK) {
        LOG_INFO(LOG_MOD_FLASH, "Read/Write Throughput Test Passed.\n");
    } else {
        LOG_ERROR(LOG_MOD_FLASH, "Read/Write Throughput Test Failed!\n");
    }

    LOG_INFO(LOG_MOD_FLASH, "All Flash Tests Completed Successfully.\n");

    cache_enable(CACHE_TYPE_DATA);

    LOG_INFO(LOG_MOD_FLASH, "XSPI Sample Application Completed Successfully.\n");

    vTaskDelete(NULL);
}
