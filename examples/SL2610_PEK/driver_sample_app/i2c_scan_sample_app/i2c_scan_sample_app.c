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
 * \file	i2c_scan_sample_app.c
 *
 * \brief	I2C Bus Scanner sample application - Dedicated I2C device detection utility
 */

#include <stdio.h>
#include <stdint.h>
#include "pinmux.h"
#include "logger.h"
#include "i2c_scan_sample_app.h"

#define DEFAULT_STACK_DEPTH         (8 * configMINIMAL_STACK_SIZE)  /**< Default stack depth for sample task */
#define UC_MANAGER_TASK_PRIORITY    (tskIDLE_PRIORITY + 1)          /**< Task priority */
#define I2C_INST                    I2C0                           /**< Select I2C instance: I2C0, I2C1 */

static StaticTask_t xTaskBufferi2c;
static StackType_t xStacki2c[DEFAULT_STACK_DEPTH];

/**
 * Controller configuration used for I2C scan
 */
i2c_controller_config_t i2c_scan_config = {
    .ten_bit_addr               = false,
    .speed                      = I2C_SPEED_STANDARD,
    .enable_bus_clear           = false,
    .clear_stuck_sda            = false,
    .scl_stuck_timeout_ms       = 1,
    .sda_stuck_timeout_ms       = 1
};


/** Maximum number of 7-bit I2C addresses */
#define I2C_MAX_7BIT_ADDRESSES  128
#define I2C_SCAN_START_ADDR     0x00
#define I2C_SCAN_END_ADDR       0x7F
#define I2C_SCAN_TIMEOUT_MS     10

/**
 * Configure pinmux for selected I2C instance
 */
 static void configure_i2c_pins(i2c_instance_en instance)
 {
     switch (instance) {
         case I2C0:
             pinmux_set_pin_function(I2CM0_SCL_PIN, I2CM0_SCL_PIN_FUNC);
             pinmux_set_pin_function(I2CM0_SDA_PIN, I2CM0_SDA_PIN_FUNC);
             break;
         case I2C1:
             pinmux_set_pin_function(I2CM1_SCL_PIN, I2CM1_SCL_PIN_FUNC);
             pinmux_set_pin_function(I2CM1_SDA_PIN, I2CM1_SDA_PIN_FUNC);
             break;
 #if CONFIG_MODULE_I2C_S_ENABLED
         case I2C2:
             pinmux_set_pin_function(I2CS_SCL_PIN, I2CS_SCL_PIN_FUNC);
             pinmux_set_pin_function(I2CS_SDA_PIN, I2CS_SDA_PIN_FUNC);
             break;
 #endif
         default:
             LOG_ERROR(LOG_MOD_GENERIC, "Invalid I2C instance\n");
             break;
     }
 }

/**
 * \brief I2C Bus Scanner API - Scans all valid 7-bit addresses to detect connected devices
 *
 * \param instance       I2C instance to use for scanning (I2C0, I2C1)
 * \param scan_results   Pointer to array to store scan results
 * \param max_results    Maximum number of results to store
 * \param detected_count Pointer to store number of detected devices
 *
 * \return I2C_OK on successful scan, error code otherwise
 *
 * \details This function scans I2C addresses from 0x08 to 0x77 (excluding reserved addresses)
 *          and attempts to communicate with each address to detect connected devices.
 *          Uses a simple write operation to probe each address.
 */
i2c_status_en i2c_scan_bus(i2c_instance_en instance, uint32_t max_results)
{
    i2c_status_en status = I2C_OK;
    uint32_t devices_found = 0;
    uint8_t probe_data = 0x00;  /* Dummy data for probing */

    /* Validate input parameters */
    if (max_results == 0) {
        LOG_ERROR(LOG_MOD_SYSTEM, "[I2C_SCAN] Invalid parameters\n");
        return I2C_ERROR_PARAMETER;
    }

    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] Starting I2C bus scan on instance %d\n", instance);
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] Scanning addresses 0x%02X to 0x%02X\n",
             I2C_SCAN_START_ADDR, I2C_SCAN_END_ADDR);

    /* Scan all valid 7-bit addresses */
    for (uint8_t addr = I2C_SCAN_START_ADDR; addr <= I2C_SCAN_END_ADDR && devices_found < max_results; addr++) {

        i2c_disable(instance);

        /* Set target address for this probe */
        i2c_controller_set_target_address(instance, addr);

        i2c_enable(instance);

        /* Configure blocking write for address probe */
        i2c_blocking_config_t probe_config = {
            .buf = &probe_data,
            .size = sizeof(probe_data),  /* Zero-length write for address probe */
            .timeout = I2C_SCAN_TIMEOUT_MS,
            .send_stop = true
        };

        /* Attempt to write to the address */
        status = i2c_controller_write_blocking(instance, &probe_config);

        /* Count and log detected devices */
        if (status == I2C_OK) {
            devices_found++;
            LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] Device found at address 0x%02X\n", addr);
        }

        /* Small delay between probes to avoid bus congestion */
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] Scan complete. Found %lu devices\n", devices_found);

    return I2C_OK;
}


/**
 * \brief Comprehensive I2C bus scan test
 *
 * \param instance I2C instance to scan
 *
 * \return I2C_OK on successful scan, error code otherwise
 */
i2c_status_en i2c_bus_scan_test(i2c_instance_en instance)
{
    uint32_t detected_count = 0;
    i2c_status_en status;

    LOG_INFO(LOG_MOD_SYSTEM, "\n[I2C_SCAN] === Starting I2C Bus Scan Test ===\n");
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] Instance: %s\n",
             (instance == I2C0) ? "I2C0" :
             (instance == I2C1) ? "I2C1" : "I2C2");

    /* Perform the scan */
    status = i2c_scan_bus(instance, I2C_MAX_7BIT_ADDRESSES);

    if (status != I2C_OK) {
        LOG_ERROR(LOG_MOD_SYSTEM, "[I2C_SCAN] Bus scan failed with error: %d\n", status);
        return status;
    }

    return I2C_OK;
}

/**
 * \brief Main I2C scan test application
 *
 * \return Test result status
 */
int32_t i2c_scan_sample_test()
{
    i2c_status_en status;

    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] ========================================\n");
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C Bus Scanner Sample Application\n");
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] ========================================\n");

    /* Configure I2C pins for the selected instance */
    configure_i2c_pins(I2C_INST);
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C pins configured for instance %d\n", I2C_INST);

    /* Initialize I2C controller */
    status = i2c_init(I2C_INST);
    if (status != I2C_OK) {
        LOG_ERROR(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C controller initialization failed: %d\n", status);
        return -1;
    }
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C controller initialized successfully\n");

    i2c_disable(I2C_INST);

    /* Controller Configuration */
    LOG_INFO(LOG_MOD_GENERIC, "Configuring controller...\n");
    i2c_controller_set_config(I2C_INST, &i2c_scan_config);
    i2c_controller_get_config(I2C_INST, &i2c_scan_config);
    LOG_INFO(LOG_MOD_GENERIC,"ten_bit_addr: %d,enable_bus_clear:%d,clear_stuck_sda:%d,\
    scl_stuck_timeout_ms:%d,sda_stuck_timeout_ms:%d\n",i2c_scan_config.ten_bit_addr,\
    i2c_scan_config.enable_bus_clear,i2c_scan_config.clear_stuck_sda,i2c_scan_config.scl_stuck_timeout_ms,\
    i2c_scan_config.sda_stuck_timeout_ms);

    /* Enable interfaces */
    i2c_enable(I2C_INST);

    /* Enable I2C instance */
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C instance enabled\n");

    /* Perform I2C bus scan */
    status = i2c_bus_scan_test(I2C_INST);

    if (status == I2C_OK) {
        LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C scan test completed successfully\n");
    } else {
        LOG_ERROR(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C scan test failed with error: %d\n", status);
    }

    /* Disable I2C instance */
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C instance disabled\n");

    /* Deinitialize I2C controller */
    i2c_deinit(I2C_INST);
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C controller deinitialized\n");

    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] ========================================\n");
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] I2C Bus Scanner Application Complete\n");
    LOG_INFO(LOG_MOD_SYSTEM, "[I2C_SCAN] ========================================\n\n");

    return (status == I2C_OK) ? 0 : -1;
}

/**
 * \brief Task wrapper for I2C scan test
 *
 * \param pvParameters FreeRTOS task parameters (unused)
 */
void i2c_scan_sample_task(void *pvParameters)
{
    (void)pvParameters;

    /* Run the I2C scan test */
    i2c_scan_sample_test();

    /* Task completed, delete itself */
    vTaskDelete(NULL);
}

/**
 * \brief Application entry point to create I2C scan sample task
 */
void i2c_scan_sample_app_task(void)
{
    /* Create I2C scan task */
    xTaskCreateStatic(i2c_scan_sample_task,
                      "I2C_SCAN_TASK",
                      DEFAULT_STACK_DEPTH,
                      NULL,
                      UC_MANAGER_TASK_PRIORITY,
                      xStacki2c,
                      &xTaskBufferi2c);
}
