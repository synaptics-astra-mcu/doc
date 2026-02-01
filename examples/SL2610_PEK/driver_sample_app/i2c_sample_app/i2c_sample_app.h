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

 #ifndef I2C_SAMPLE_APP_H
 #define I2C_SAMPLE_APP_H
 
/*******************************************************************************
*                          Include files
*******************************************************************************/
 
 #include <stdint.h>
 #include "i2c.h"
 #include "i2c_soc_config.h"

/**
* \addtogroup group_i2c_driver_sample_app
* \{
* Test Application for I2C peripheral
* Demonstrates UART transmission and reception in both blocking and
* non-blocking modes across enabled I2C instances. Each operation's
* result is printed to the console for functional verification
*
*/
/*******************************************************************************
*                          Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_i2c_driver_test_function
* \{
* \brief Test functions for I2C driver
*/

/*******************************************************************************
* Function Name: i2c_sample_app_task
******************************************************************************
*
* \brief Validates all I2C APIs and its functionalities.
*
* \return void
*
*******************************************************************************/

void i2c_sample_app_task(void);

/** \} group_i2c_driver_test_function */
/** \} group_i2c_driver_sample_app */


#endif /* I2C_SAMPLE_APP_H */

