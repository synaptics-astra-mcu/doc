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

#include <stdio.h>
#include <stdint.h>
#include "logger.h"
#include "board.h"
#include "i2c_exp_app.h"
#include "io_expander.h"

#define DEFAULT_STACK_DEPTH         (8 * configMINIMAL_STACK_SIZE)  /**< Default stack depth for sample task */
#define I2C_EXP_TASK_PRIORITY       (tskIDLE_PRIORITY + 1)          /**< Task priority */
#define TOGGLE_DELAY_MS             1000                            /**< Delay between toggles in milliseconds */

static StaticTask_t xTaskBufferExpander;
static StackType_t xStackExpander[DEFAULT_STACK_DEPTH];

/**
 * \brief Task function that continuously toggles GPIO 5 of the I2C expander
 *
 * This function runs in a FreeRTOS task and toggles GPIO pin 5 between
 * high (1) and low (0) states with a configurable delay.
 *
 * \param pvParameters Task parameters (unused)
 */
static void i2c_exp_toggle_task(void *pvParameters)
{
    (void)pvParameters;  /* Suppress unused parameter warning */

    io_exp_configs fxl6408_exp_config =
    {
        .i2c_instance = board_get_i2c_instance(),
        .expander_type = FXL6408,
        .exp_slv_addr = 0x43,
        .gpio_id = 5,
        .value = 0,
        .port = 0
    };

    uint32_t gpio_state = 0;  /* Start with GPIO low */
    uint32_t fxl_result;

    LOG_INFO(LOG_MOD_GENERIC, "I2C Expander GPIO Toggle Task Started\n");
    LOG_INFO(LOG_MOD_GENERIC, "Controlling FXL6408 pin %d\n",
                fxl6408_exp_config.gpio_id);

    while (1)
    {
        /* Set GPIO value */
        fxl6408_exp_config.value = gpio_state;

        /* Set GPIO via expander */
        fxl_result = io_exp_gpio_out_set(&fxl6408_exp_config);

        if (fxl_result == IO_EXP_OK)
        {
            LOG_INFO(LOG_MOD_GENERIC,
                        "FXL6408 pin %d set to %d\n", fxl6408_exp_config.gpio_id, gpio_state);
        }
        else
        {
            LOG_ERROR(LOG_MOD_GENERIC, "FXL6408 pin %d failed to set to %d (error: %d)\n",
                        fxl6408_exp_config.gpio_id, gpio_state, fxl_result);
        }

        /* Toggle state for next iteration */
        gpio_state ^= 1;

        /* Wait for specified delay */
        vTaskDelay(pdMS_TO_TICKS(TOGGLE_DELAY_MS));
    }
}

/**
 * \brief Initialize and start the I2C expander GPIO toggle task
 */
void i2c_exp_app_task(void)
{
    xTaskCreateStatic(
        i2c_exp_toggle_task,           /* Task function */
        "I2C_EXP_TOGGLE",              /* Task name */
        DEFAULT_STACK_DEPTH,           /* Stack depth */
        NULL,                          /* Task parameters */
        I2C_EXP_TASK_PRIORITY,         /* Task priority */
        xStackExpander,                /* Stack buffer */
        &xTaskBufferExpander           /* Task control block */
    );
}
