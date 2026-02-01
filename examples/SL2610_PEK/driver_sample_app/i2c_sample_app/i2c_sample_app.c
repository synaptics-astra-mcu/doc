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
 * \file	i2c_sample_app.c
 *
 * \brief	I2C Driver sample application
 */

#include <stdio.h>
#include <stdint.h>
#include "pinmux.h"
#include "logger.h"
#include "i2c_sample_app.h"

#define DEFAULT_STACK_DEPTH         (8 * configMINIMAL_STACK_SIZE)  /**< Default stack depth for sample task */
#define UC_MANAGER_TASK_PRIORITY    (tskIDLE_PRIORITY + 1)          /**< Task priority */
#define I2C_TARGET_ADDRESS          I2C_SLV_ADDR                    /**< I2C slave target address */
#define I2C_INST                    I2C0                           /**< Select I2C instance: I2C0, I2C1, I2C2 */

static StaticTask_t xTaskBufferi2c;
static StackType_t xStacki2c[DEFAULT_STACK_DEPTH];

static volatile uint32_t i2c_event_flag;   /**< Flag for event occurrence */
static volatile uint32_t i2c_event_type;   /**< Type of event triggered */

/**
 * Controller configuration used for sample app
 */
i2c_controller_config_t i2c_con_config = {
    .ten_bit_addr               = false,
    .speed                      = I2C_SPEED_STANDARD,
    .enable_bus_clear           = false,
    .clear_stuck_sda            = false,
    .scl_stuck_timeout_ms       = 1,
    .sda_stuck_timeout_ms       = 1
};

/** Buffer and config for blocking write */
uint8_t block_data[] = {0x02,0x03, 0x04, 0x05, 0x06, 0x00, 0x55, 0xAA};
i2c_blocking_config_t config = {
    .buf         = block_data,
    .size        = sizeof(block_data),
    .timeout     = 10,
    .send_stop   = false
};

uint8_t block_data1[] = {0x02};
i2c_blocking_config_t config1 = {
    .buf         = block_data1,
    .size        = sizeof(block_data1),
    .timeout     = 10,
    .send_stop   = true
};

/** Buffer and config for blocking read */
uint8_t read_buffer[1] = {};
i2c_blocking_config_t read_config = {
    .buf        = read_buffer,
    .size       = sizeof(read_buffer),
    .timeout    = 10,
    .send_stop  = true
};

/* Buffer and config for non-blocking write */

static uint8_t test_data[] = {
    0x02, 0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x29, 0x30, 0x31, 0x32
};

i2c_nonblocking_config_t xfer = {
    .buf       = test_data,
    .size      = sizeof(test_data),
    .send_stop = false
};


static uint8_t reg_addr[] = {0x02};

i2c_nonblocking_config_t xfer_set_reg = {
    .buf = reg_addr,
    .size = sizeof(reg_addr),
    .send_stop = true
};

/** Buffer and config for non-blocking read */
uint8_t read_data[1] = {};
i2c_nonblocking_config_t read_non_block = {
    .buf       = read_data,
    .size      = sizeof(read_data),
    .send_stop = true
};

/**
 * Callback invoked on I2C event
 */
static void i2c_event_cb(i2c_instance_en id, i2c_event_en event_mask, void* user_data)
{
    LOG_DEBUG(LOG_MOD_GENERIC, "I2C: event 0x%x raised\n", event_mask);
    i2c_event_type = event_mask;
    i2c_event_flag = 1;
}

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
 * Main test application to validate I2C APIs
 */
int32_t i2c_sample_test()
{
    LOG_INFO(LOG_MOD_GENERIC, "I2C Sample App Starts\n");
    i2c_status_en ret = I2C_OK;
    uint32_t address = 0;
    i2c_event_type = 0;

    configure_i2c_pins(I2C_INST);

    ret = i2c_init(I2C_INST);
    if (ret != I2C_OK) {
        LOG_ERROR(LOG_MOD_GENERIC, "Failed to init I2C\n");
        return ret;
    }

    i2c_disable(I2C_INST);

    /* Controller Configuration */
    LOG_INFO(LOG_MOD_GENERIC, "Configuring controller...\n");
    i2c_controller_set_config(I2C_INST, &i2c_con_config);
    i2c_controller_get_config(I2C_INST, &i2c_con_config);
    LOG_INFO(LOG_MOD_GENERIC,"ten_bit_addr: %d,enable_bus_clear:%d,clear_stuck_sda:%d,\
    scl_stuck_timeout_ms:%d,sda_stuck_timeout_ms:%d\n",i2c_con_config.ten_bit_addr,\
    i2c_con_config.enable_bus_clear,i2c_con_config.clear_stuck_sda,i2c_con_config.scl_stuck_timeout_ms,\
    i2c_con_config.sda_stuck_timeout_ms);

    /* Set target addresses */
    LOG_INFO(LOG_MOD_GENERIC, "Setting target addresses...\n");
    i2c_controller_set_target_address(I2C_INST, I2C_TARGET_ADDRESS );
    if (i2c_controller_get_target_address(I2C_INST, &address) == I2C_OK) {
    /* Print it in hexadecimal (most common for I2C addresses) */
        LOG_INFO(LOG_MOD_GENERIC, "Controller target address: 0x%02X\n", address);
    }
    else {
        LOG_ERROR(LOG_MOD_GENERIC, "Failed to get I2C target address\n");
    }

    /* Enable interfaces */
    i2c_enable(I2C_INST);

    LOG_INFO(LOG_MOD_GENERIC, "I2C controller write blocking started\n");
    ret = i2c_controller_write_blocking(I2C_INST, &config);
    if (ret == I2C_OK) {
      LOG_INFO(LOG_MOD_GENERIC, "Master write success with status code:%d\n", ret);
    }
    else {
        LOG_ERROR(LOG_MOD_GENERIC, "Master write failed with status code:%d\n", ret);
    }

    LOG_INFO(LOG_MOD_GENERIC, "I2C controller write blocking started\n");
    ret = i2c_controller_write_blocking(I2C_INST, &config1);
    if (ret == I2C_OK) {
        LOG_INFO(LOG_MOD_GENERIC, "Master write success with status code:%d\n", ret);
    }
    else {
        LOG_ERROR(LOG_MOD_GENERIC, "Master write failed with status code:%d\n", ret);
    }

    LOG_INFO(LOG_MOD_GENERIC, "I2C controller read blocking started\n");
    ret = i2c_controller_read_blocking(I2C_INST, &read_config);

    if (ret == I2C_OK) {
        LOG_INFO(LOG_MOD_GENERIC, "Blocking read success with status code: %d\n", ret);
        for (int i = 0; i < read_config.size; i++) {
           LOG_INFO(LOG_MOD_GENERIC, "Read byte [%d]: 0x%02X\n", i, read_config.buf[i]);
        }
    }
    else {
        LOG_ERROR(LOG_MOD_GENERIC, "Blocking read failed with status code: %d\n", ret);
    }
    i2c_disable(I2C_INST);
    i2c_enable(I2C_INST);
    i2c_register_callback(I2C_INST, i2c_event_cb, NULL);

    i2c_event_flag = 0;
    i2c_event_type = 0;
    ret = i2c_controller_write_non_blocking(I2C_INST, &xfer);
    while (!i2c_event_flag);  /* Wait until ISR sets flag */
    if (i2c_event_type == I2C_EVENT_ABORT) {
        LOG_ERROR(LOG_MOD_GENERIC, "Non-Blocking write failed due to NACK/invalid address\n");
    }
    else if (i2c_event_type == I2C_EVENT_WRITE_COMPLETE) {
        LOG_INFO(LOG_MOD_GENERIC, "Non-Blocking write success with event type: %d\n", i2c_event_type);
    }
    else {
        LOG_ERROR(LOG_MOD_GENERIC, "Non-Blocking write with event type: %d\n", i2c_event_type);
    }
    i2c_event_flag = 0;
    i2c_event_type = 0;
    ret = i2c_controller_write_non_blocking(I2C_INST, &xfer_set_reg);
    while (!i2c_event_flag);  /* Wait until ISR sets flag */
    if (i2c_event_type == I2C_EVENT_ABORT) {
        LOG_ERROR(LOG_MOD_GENERIC, "Non-Blocking write failed due to NACK/invalid address\n");
    }
    else if (i2c_event_type == I2C_EVENT_WRITE_COMPLETE) {
        LOG_INFO(LOG_MOD_GENERIC, "Non-Blocking write success with event type: %d\n", i2c_event_type);
    }
    else {
        LOG_INFO(LOG_MOD_GENERIC, "Non-Blocking write with event type: %d\n", i2c_event_type);
    }

    i2c_event_flag = 0;
    i2c_event_type = 0;
    ret = i2c_controller_read_non_blocking(I2C_INST, &read_non_block);
    while (!i2c_event_flag);  /* Wait until ISR sets flag */
    if (i2c_event_type == I2C_EVENT_ABORT) {
        LOG_ERROR(LOG_MOD_GENERIC, "Non-Blocking read failed due to NACK/invalid address\n");
    }
    else if (i2c_event_type == I2C_EVENT_READ_COMPLETE) {
        LOG_INFO(LOG_MOD_GENERIC, "Non-Blocking read success with event type: %d\n", i2c_event_type);
        for (int i = 0; i < read_non_block.size; i++) {
            LOG_INFO(LOG_MOD_GENERIC, "Read byte [%d]: 0x%02X\n", i, read_non_block.buf[i]);
        }
    }
    else {
        LOG_INFO(LOG_MOD_GENERIC, "Non-Blocking read with event type: %d\n", i2c_event_type);
    }

    if (i2c_event_type == I2C_EVENT_ERROR) {
        LOG_DEBUG(LOG_MOD_GENERIC, "%s: unexpected event = 0x%x\n", __FUNCTION__, i2c_event_type);
        return I2C_ERROR;
    }

    i2c_disable(I2C_INST);
    i2c_deinit(I2C_INST);

    LOG_INFO(LOG_MOD_GENERIC, "All I2C Driver APIs functionalities are verified successfully!\n");
    return 0;
}

/**
 * Task wrapper for I2C test
 */
void i2c_sample_task(void *pvParameters)
{
    i2c_sample_test();
    vTaskDelete(NULL);
}

/**
 * Application entry point to create I2C sample task
 */
void i2c_sample_app_task(void)
{
    xTaskCreateStatic(
        i2c_sample_task,
        "I2C_SAMPLE_APP",
        DEFAULT_STACK_DEPTH,
        NULL,
        UC_MANAGER_TASK_PRIORITY,
        xStacki2c,
        &xTaskBufferi2c
    );
}
