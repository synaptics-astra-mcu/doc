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

#ifndef I2C_EXP_APP_H
#define I2C_EXP_APP_H

/*******************************************************************************
*                          Include files
*******************************************************************************/

#include <stdint.h>
#include "io_expander.h"

/**
* \addtogroup group_i2c_exp_app
* \{
* Test Application for I2C Expander
* Demonstrates continuous GPIO toggling on expander GPIO pin 5
* between high (1) and low (0) states with configurable delay
*
*/

/*******************************************************************************
*                          Function Prototypes
*******************************************************************************/

/**
* \addtogroup group_i2c_exp_app_functions
* \{
* \brief Test functions for I2C Expander application
*/

/*******************************************************************************
* Function Name: i2c_exp_app_task
****************************************************************************//**
*
* \brief Main task that continuously toggles GPIO 5 of the I2C expander
*        between 0 and 1 states with a delay between transitions.
*
* \return void
*
*******************************************************************************/
void i2c_exp_app_task(void);

/** \} group_i2c_exp_app_functions */
/** \} group_i2c_exp_app */

#endif /* I2C_EXP_APP_H */
