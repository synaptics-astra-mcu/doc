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
 * \file	clock_sample_app.c
 *
 * \brief	Clock Driver sample application
 */

/*******************************************************************************
*                              Include files
*******************************************************************************/

#include "wifi_sample_app.h"

/*******************************************************************************
*                   Integer and enum variables                   
*******************************************************************************/
#define SHELL_STACK_DEPTH						(300 * 4)
#define SHELL_DEMO_STACK_DEPTH					(300 * 4)

#define SHELL_TASK_PRIORITY						(tskIDLE_PRIORITY + 1)
#define SHELL_DEMO_TASK_PRIORITY				(tskIDLE_PRIORITY + 4)

StackType_t  xStackShell[SHELL_STACK_DEPTH];
StaticTask_t xTaskBufferShell;
StackType_t  xStackShell_demo[SHELL_STACK_DEPTH];
StaticTask_t xTaskBufferShell_demo;

uint32_t oob_int_cnt = 0;

uint32_t wlan_get_oob_cnt()
{
	return oob_int_cnt;
}

/*******************************************************************************
*                               Clock Sample App 
*******************************************************************************/

void wifi_sample_app(void)
{
    sdio_status_en ret;

    /* Initialize SDIO controller */
    ret = sdio_init(SDIO1);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO Init Failure: %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "  - Unable to continue with sample app\n");
        return; /* Early return on critical initialization failure */
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO Init Success\n");
    }

    shell_handle = xTaskCreateStatic(	vTaskShell,        	// Task function
										"Shell",           	// Task name (for debugging)
										SHELL_STACK_DEPTH,    	// Stack depth (in words)
										NULL,                 	// Task parameter
										SHELL_TASK_PRIORITY,  	// Task priority
										xStackShell,           // Stack buffer
										&xTaskBufferShell      // Task control block (TCB)
										);

    shell_demo_handle = xTaskCreateStatic(	vTaskShell_demo,        	// Task function
										"Shell_demo",           	// Task name (for debugging)
										SHELL_DEMO_STACK_DEPTH,    	// Stack depth (in words)
										NULL,                 	// Task parameter
										SHELL_DEMO_TASK_PRIORITY,  	// Task priority
										xStackShell_demo,           // Stack buffer
										&xTaskBufferShell_demo      // Task control block (TCB)
										);
}