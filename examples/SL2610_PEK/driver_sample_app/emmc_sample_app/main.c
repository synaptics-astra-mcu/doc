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

#include <stdio.h>
#include <assert.h>
#include "syna_system_init.h"
#include "assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "logger.h"
#include "secure_definition.h"
#include "syna_system_init.h"
#include "syna_version.h"
#include "emmc_sample_app.h"

//*************************** Macro declarations ****************************


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

int main(void)
{

    /* Initialize system */
    syna_system_init();

    /* Set secure attributes */
    set_secure_attributes();

    /* Configure board peripherals. */
    board_init();

    /* all drivers init msg print only to console */
    logger_init(CONFIG_LOGGER_DEFAULT_INTERFACE, LOGGER_BUFFER_DEFAULT_SIZE,
        LOGGER_ENABLE_ALL_MODULES, CONFIG_LOG_LVL_LOWEST_AVAILABLE, LOGGER_MODE_ASCII);

    /* Set final logger interface. */
    logger_set_interface(CONFIG_LOGGER_DEFAULT_INTERFACE);

    /* Print Hash */
    syna_hash();

    emmc_sample_app_task();

    vTaskStartScheduler();

    assert(0);

}
