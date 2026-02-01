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
#include "timer.h"
#include "fw_tasks.h"
#include "wifi_sample_app.h"
#include "usb.h"
#include "syna_version.h"
#include "syna_system_init.h"

/*******************************************************************************
*                               Macro Definitons
*******************************************************************************/

#define DEFAULT_STACK_DEPTH                 (8 * configMINIMAL_STACK_SIZE)

/*******************************************************************************
*                          Variable declarations
*******************************************************************************/

static StackType_t  xStackAutonrun[DEFAULT_STACK_DEPTH];
static StaticTask_t xTaskBufferAutorun;

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

#define USB_DEVICE_TASK_PRIORITY                USB_DEFAULT_DEVICE_TASK_PRIORITY

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
*                          Function Macro Definitons
*******************************************************************************/

/**
 * \brief     initialize the USB instances and related system configurations:
 */
#define sys_usb_init() (usb_all_install())

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

    /* Init system timers. */
    if ((rc = timer_init()) != ARM_DRIVER_OK) {
        LOG_INFO(LOG_MOD_TIMER, "timer_init() failed! rc = %d\n", rc);
        error_occured = true;
    }

    /* Init USB peripheral. */
    rc = sys_usb_init();
    if ((rc != ARM_DRIVER_OK) && (rc != USB_ERROR_NOT_CONNECTED)) {
        LOG_ERROR(LOG_MOD_USB,"sys_usb_init() failed! rc = %d\n",rc);
        error_occured = true;
    }

    if(error_occured != true) {
        LOG_INFO(LOG_MOD_SYSTEM, "Application drivers initialization complete without errors.\n");
    }
}
/**
 * \brief Main application entry point.
 */
int main(void)
{
    /* Initialize system drivers */
    syna_system_init();

    /* Configure board peripherals. */
    board_init();

    /*all drivers init msg print only to console*/
    logger_init(CONFIG_LOGGER_DEFAULT_INTERFACE, LOGGER_BUFFER_DEFAULT_SIZE,
                LOGGER_ENABLE_ALL_MODULES, CONFIG_LOG_LVL_LOWEST_AVAILABLE, LOGGER_MODE_ASCII);

    /* Set final logger interface. */
    logger_set_interface(CONFIG_LOGGER_DEFAULT_INTERFACE);

    /* Initialize app specific drivers */
    app_driver_init();

    /* Create syna services task */
    syna_service_init();

    /* Print SDK version */
    sdk_version();

    /* Run wifi sample application. */
    wifi_sample_app();

    /* Schedules idle and user tasks */
    vTaskStartScheduler();

    /* Should never reach here, ensures control never returns */
    assert(0);

    return 0;
}
