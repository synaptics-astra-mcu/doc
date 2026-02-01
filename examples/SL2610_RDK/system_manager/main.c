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
#include "dhl_lib.h"
#include "board.h"
#include "timer.h"
#include "fw_tasks.h"
#include "secure_definition.h"
#include "sys_mng.h"
#include "syna_version.h"
#include "syna_system_init.h"
#include "usb.h"
#include "timer.h"
#include "xspic.h"
//*************************** Variable declarations ****************************

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

#if !CONFIG_MODULE_USB_USE_INTERNAL_TASK_BUFFERS

#if CFG_TUSB_DEBUG
#define USB_DEV_STACK_SIZE                  USB_DEFAULT_DEVICE_STACK_SIZE_DBG
#else
#define USB_DEV_STACK_SIZE                  USB_DEFAULT_DEVICE_STACK_SIZE
#endif

#define USB_DEVICE_TASK_PRIORITY            USB_DEFAULT_DEVICE_TASK_PRIORITY

size_t  g_usb_event_loop_stack_size = USB_DEFAULT_EVENT_LOOP_STACK_SIZE;
size_t  g_usb_device_stack_size = USB_DEV_STACK_SIZE;
uint8_t g_usb_device_task_priority = USB_DEVICE_TASK_PRIORITY;
uint8_t g_usb_event_loop_task_priority = USB_DEFAULT_EVENT_LOOP_TASK_PRIORITY;

#if configSUPPORT_STATIC_ALLOCATION

StackType_t  g_usb_event_loop_stack[USB_DEFAULT_EVENT_LOOP_STACK_SIZE];
StaticTask_t g_usb_event_loop_taskdef;
StackType_t  g_usb_device_stack[USB_DEV_STACK_SIZE];
StaticTask_t g_usb_device_taskdef;

#endif

#endif /* !CONFIG_MODULE_USB_USE_INTERNAL_TASK_BUFFERS */

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

#if CONFIG_MODULE_TIMER_ENABLED
    if ((rc = timer_init()) != 0) {
        LOG_INFO(LOG_MOD_TIMER,"timer init failed\n");
        error_occured = true;
    }

    wic_interrupt_init();
#endif /* CONFIG_MODULE_TIMER_ENABLED */

    /* Initialize XSPI SOC  */
    if ((rc = xspi_soc_init()) != XSPI_DRIVER_OK) {
        LOG_ERROR(LOG_MOD_FLASH, "xspi_soc_init failed, sts=%d\n", rc);
        error_occured = true;
    }

    if(error_occured == true) {
        LOG_INFO(LOG_MOD_SYSTEM, "drivers initialization failed.\n");
    }
}

/**
 * \brief Main application entry point.
 */
int main(void)
{
    dmem_init();

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

    /* Set logger level to info. */
    logger_set_level(LOG_LVL_INFO);

    /* Initialize app specific drivers */
    app_driver_init();

    /* Create syna services task */
    syna_service_init();

    /* Print Hash */
    syna_hash();

    /* Print SDK version */
    sdk_version();

    /* Initialize system manager */
    sys_mng_task_create();

    /* Start FreeRTOS scheduler */
    vTaskStartScheduler();

    assert(0);

}
