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

#ifndef __EMMC_SAMPLE_APP_H__
#define __EMMC_SAMPLE_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Include files *********************************/

#include "emmc.h"
#include "emmchc.h"

/*************************** Macro declarations ****************************/

/*************************** Function declarations ****************************/

/*******************************************************************************
* Function Name: emmc_sample_app_main
****************************************************************************//**
*
* Entry point for the eMMC sample application.
*
* \details Demonstrates basic eMMC initialization, read, and write operations.
*          The application writes a fixed pattern to a buffer, writes it to
*          eMMC storage, reads it back, and verifies data integrity by comparing
*          the read buffer with the original write buffer. Status messages are
*          logged throughout the process to aid in debugging and validation.
*
* \return void
*
*******************************************************************************/

void emmc_sample_app_task(void);

#ifdef __cplusplus
}
#endif
#endif /* __EMMC_SAMPLE_APP_H__ */