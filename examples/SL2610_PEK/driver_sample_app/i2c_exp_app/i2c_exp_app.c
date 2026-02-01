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
    io_exp_configs tca6416a_exp_config =
    {
        .i2c_instance = board_get_i2c_instance(),
        .expander_type = TCA6416A,
        .exp_slv_addr = 0x20,
        .gpio_id = 4,
        .value = 0,
        .port = 1
    };

    uint32_t gpio_state = 0;  /* Start with GPIO low */
    uint32_t fxl_result, tca_result;
    io_exp_configs *expanders[] = {&fxl6408_exp_config, &tca6416a_exp_config};
    const char *exp_names[] = {"FXL6408", "TCA6416A"};
    const uint8_t num_expanders = sizeof(expanders) / sizeof(expanders[0]);

    LOG_INFO(LOG_MOD_GENERIC, "I2C Expander GPIO Toggle Task Started\n");
    LOG_INFO(LOG_MOD_GENERIC, "Controlling %s pin %d and %s pin %d\n",
             exp_names[0], expanders[0]->gpio_id, exp_names[1], expanders[1]->gpio_id);

    while (1)
    {
        /* Set GPIO state for both expanders */
        for (uint8_t i = 0; i < num_expanders; i++) {
            expanders[i]->value = gpio_state;
        }

        /* Call expander GPIO set functions and track results separately */
        fxl_result = io_exp_gpio_out_set(expanders[0]);
        tca_result = io_exp_gpio_out_set(expanders[1]);

        /* Log results - success or failure for each expander */
        if (fxl_result == IO_EXP_OK && tca_result == IO_EXP_OK)
        {
            LOG_INFO(LOG_MOD_GENERIC, "Both expanders set to %d: %s pin %d, %s pin %d\n",
                     gpio_state, exp_names[0], expanders[0]->gpio_id, exp_names[1], expanders[1]->gpio_id);
        }
        else
        {
            if (fxl_result != IO_EXP_OK) {
                LOG_ERROR(LOG_MOD_GENERIC, "%s pin %d failed to set to %d (error: %d)\n",
                         exp_names[0], expanders[0]->gpio_id, gpio_state, fxl_result);
            }
            if (tca_result != IO_EXP_OK) {
                LOG_ERROR(LOG_MOD_GENERIC, "%s pin %d failed to set to %d (error: %d)\n",
                         exp_names[1], expanders[1]->gpio_id, gpio_state, tca_result);
            }
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
