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
 * @file process_bootsource.h
 *
 * @brief Process boot source header file
 */
#ifndef __PROCESS_BOOTSOURCE_H__
#define __PROCESS_BOOTSOURCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    SM_STATUS_OK = 0,
    SM_STATUS_BOOT_SRC_INVALID = -1,
    SM_STATUS_ERROR = -2,
} sm_status_en;

/**
 * @brief    Handle cold boot
 *
 * @return sm_status_en
 */
sm_status_en handle_cold_boot(void);

#ifdef __cplusplus
}
#endif
#endif /* __PROCESS_BOOTSOURCE_H__ */
