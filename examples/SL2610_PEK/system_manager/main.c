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

//****************************** Include files *********************************

#include <assert.h>
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <assert.h>
#include "logger.h"
#include "board.h"
#include "secure_definition.h"
#include "sys_mng.h"
#include "syna_version.h"
#include "syna_system_init.h"

//*************************** Variable declarations ****************************

#if REMOVE_SEMIHOSTING_USAGE
    __asm(".global __use_no_semihosting\n\t");
#endif

#if !defined(__ARMCC_VERSION)
    __attribute__((cmse_nonsecure_entry))
    void dummy_veneer(void) {
    }
#endif

//**************************** Function definitions ****************************

#if DISABLE_SCATTER_LOADING
    void $sub$$__scatterload(void) {}
#endif

/*******************************************************************************
*                          Function definitions
*******************************************************************************/

/**
 * \brief Application specific driver initialization
 */
static void app_driver_init(void)
{
    int32_t rc = ARM_DRIVER_OK;
    bool error_occured = false;

    /* Init UART3 driver. */
    if ((rc = board_uart_init(UART3)) != UART_OK) {
        LOG_ERROR(LOG_MOD_UART, "board_uart_init(UART1) failed! rc = %d\n", rc);
        error_occured = true;
    }

    if(error_occured != true) {
        LOG_INFO(LOG_MOD_SYSTEM, "Application drivers initialization complete without errors.\n");
    }
}

int main(void)
{
    /* Initialize system */
    syna_system_init();

    /* Configure board peripherals. */
    board_init();

    /* Set secure attributes */
    set_secure_attributes();

    /* Initialize services */
    syna_service_init();

    /* all drivers init msg print only to console */
    logger_init(CONFIG_LOGGER_DEFAULT_INTERFACE, LOGGER_BUFFER_DEFAULT_SIZE,
        LOGGER_ENABLE_ALL_MODULES, CONFIG_LOG_LVL_LOWEST_AVAILABLE, LOGGER_MODE_ASCII);

    /* Set final logger interface. */
    logger_set_interface(CONFIG_LOGGER_DEFAULT_INTERFACE);

    /* Initialize app specific drivers */
    app_driver_init();

    /* Print Hash */
    syna_hash();

    /* Initialize system manager */
    sys_mng_task_create();

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    assert(0);

}