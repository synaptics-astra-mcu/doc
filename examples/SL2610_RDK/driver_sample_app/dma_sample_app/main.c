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

/*******************************************************************************
*                          Include files
*******************************************************************************/

#include <assert.h>
#include "logger.h"
#include "board.h"
#include "dma_sample_app.h"
#include "syna_version.h"
#include "syna_system_init.h"

/*******************************************************************************
*                               Macro Definitons
*******************************************************************************/

#define DEFAULT_STACK_DEPTH					(8 * configMINIMAL_STACK_SIZE)
#define UC_MANAGER_TASK_PRIORITY			(tskIDLE_PRIORITY + 1)

/*******************************************************************************
*                          Variable declarations
*******************************************************************************/

/* Stack and Task Control Block for DMA sample application */
StackType_t  dma_sample_app_stack[DEFAULT_STACK_DEPTH];
StaticTask_t dma_sample_app_taskdef;

#if REMOVE_SEMIHOSTING_USAGE
    __asm(".global __use_no_semihosting\n\t");
#endif

#if !defined(__ARMCC_VERSION)
    __attribute__((cmse_nonsecure_entry))
    void dummy_veneer(void) {
    }
#endif

#if DISABLE_SCATTER_LOADING
    void $sub$$__scatterload(void) {}
#endif

/*******************************************************************************
*                          Function Macro Definitons
*******************************************************************************/

/*******************************************************************************
*                          Function definitions
*******************************************************************************/

/**
 * \brief Main application entry point.
 */
int main(void)
{
    /* Initilaize System drivers*/
    syna_system_init();

    /* Configure board peripherals. */
    board_init();

    /*all drivers init msg print only to console*/
    logger_init(CONFIG_LOGGER_DEFAULT_INTERFACE, LOGGER_BUFFER_DEFAULT_SIZE,
                LOGGER_ENABLE_ALL_MODULES, CONFIG_LOG_LVL_LOWEST_AVAILABLE, LOGGER_MODE_ASCII);

    /* Set final logger interface. */
    logger_set_interface(CONFIG_LOGGER_DEFAULT_INTERFACE);

    /* Create syna services task */
    syna_hash();

    /* Print SDK version */
    sdk_version();

    /* Run DMA sample application. */
    xTaskCreateStatic((void*)dma_sample_app_main,       /* Task function */
                        "Dma Sample APP",               /* Task name (for debugging) */
                        DEFAULT_STACK_DEPTH,            /* Stack depth (in words) */
                        NULL,                           /* Task parameter */
                        UC_MANAGER_TASK_PRIORITY,       /* Task priority */
                        dma_sample_app_stack,           /* Stack buffer */
                        &dma_sample_app_taskdef         /* Task control block (TCB) */
                    );

    /* Schedules idle and user tasks */
    vTaskStartScheduler();

    /* Should never reach here, ensures control never returns */
    assert(0);

    return 0;
}