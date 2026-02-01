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

/******************************************************************************
* \file i2c_scan_sample_app.h
* \version 1.00
*
* I2C Bus Scanner sample application header file
*
********************************************************************************
* \copyright
* Copyright 2025 Synaptics Incorporated. All rights reserved.
*
* This program is the proprietary software of Synaptics and/or its licensors,
* and may only be used, duplicated, modified or distributed under the
* authorized license from Synaptics.
* ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR
* COPYRIGHT LAW IS PROHIBITED.
*******************************************************************************/

#ifndef I2C_SCAN_SAMPLE_APP_H
#define I2C_SCAN_SAMPLE_APP_H

/*******************************************************************************
*                          Include files
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "i2c_soc_config.h"

/**
* \addtogroup group_i2c_scan_driver_sample_app
* \{
* I2C Bus Scanner Sample Application
* Dedicated utility for scanning I2C bus to detect connected devices.
* Provides comprehensive scanning functionality with formatted output
* and device detection capabilities across all valid 7-bit I2C addresses.
*
*/

/*******************************************************************************
*                          Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_i2c_scan_driver_test_function
* \{
* \brief Test functions for I2C bus scanner
*/

/*******************************************************************************
* Function Name: i2c_scan_sample_app_task
********************************************************************************
* Summary:
*  Application entry point to create I2C scan sample task.
*  This function creates a FreeRTOS task that will perform I2C bus scanning
*  to detect connected devices.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void i2c_scan_sample_app_task(void);

/*******************************************************************************
* Function Name: i2c_scan_bus
********************************************************************************
* Summary:
*  I2C Bus Scanner API - Scans all valid 7-bit addresses to detect connected devices.
*  This function systematically probes I2C addresses from 0x08 to 0x77
*  (excluding reserved addresses) to identify connected devices.
*
* Parameters:
*  instance       - I2C instance to use for scanning (I2C0, I2C1)
*  scan_results   - Pointer to array to store scan results
*  max_results    - Maximum number of results to store
*  detected_count - Pointer to store number of detected devices
*
* Return:
*  i2c_status_en - I2C_OK on successful scan, error code otherwise
*
*******************************************************************************/
i2c_status_en i2c_scan_bus(i2c_instance_en instance, uint32_t max_results);

/*******************************************************************************
* Function Name: i2c_bus_scan_test
********************************************************************************
* Summary:
*  Comprehensive I2C bus scan test function.
*  Performs a complete scan of the I2C bus and displays formatted results
*  showing all detected devices with their addresses.
*
* Parameters:
*  instance - I2C instance to scan (I2C0, I2C1, I2C2)
*
* Return:
*  i2c_status_en - I2C_OK on successful scan, error code otherwise
*
*******************************************************************************/
i2c_status_en i2c_bus_scan_test(i2c_instance_en instance);

/*******************************************************************************
* Function Name: i2c_scan_sample_test
********************************************************************************
* Summary:
*  Main I2C scan test application function.
*  Initializes I2C controller, performs bus scan, and handles cleanup.
*  Provides complete application flow for I2C device detection.
*
* Parameters:
*  None
*
* Return:
*  int32_t - 0 on success, -1 on failure
*
*******************************************************************************/
int32_t i2c_scan_sample_test(void);

/** \} group_i2c_scan_driver_test_function */
/** \} group_i2c_scan_driver_sample_app */

#endif /* I2C_SCAN_SAMPLE_APP_H */
