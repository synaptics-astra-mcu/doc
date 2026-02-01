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

#ifndef __XSPI_SAMPLE_APP_H__
#define __XSPI_SAMPLE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Include files *********************************/

// #include "xspi_flash.h"
#include "hw_reg_macro.h"
/*************************** Macro declarations ****************************/

#define FLASH_TEST_RC_OK 0
#define FLASH_TEST_RC_FAIL -1
/*************************** Function declarations ****************************/

/*******************************************************************************
* Function Name: uart_sample_app_main
****************************************************************************//**
*
* Entry point for the UART sample application.
*
* \details Demonstrates UART transmission and reception in both blocking and
*          non-blocking modes across enabled UART instances. Each operation's
*          result is printed to the console for functional verification.
*
* \return void
*
*******************************************************************************/

void xspi_sample_app_main(void);

#ifdef __cplusplus
}
#endif
#endif /* __XSPI_SAMPLE_APP_H__ */