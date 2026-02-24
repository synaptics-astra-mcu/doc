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


#ifndef __INFERENCE_BASIC_FLASH_SAMPLE_APP_H__
#define __INFERENCE_BASIC_FLASH_SAMPLE_APP_H__

#include "syna_device.h"

// Dump all outputs of the inference
//#define INFER_DUMP_OUTPUT_ON_TERMINAL

#if CHIP == SR100_B0
    // 32byte BootCommand + 96byte Header + 512byte Signature 
    #define INFER_FLASH_HEADER_SIZE                 (0x280)

    // Model offsets relative to XSPI base address
    #define INFER_FLASH_MODEL_A_OFFSET              (0x1C)
    #define INFER_NVM_A_OFFSET_IN_FLASH             (0x94)

    //NVM A address in flash: {0x3C00_0000 + 0x94} 
    #define INFER_NVM_A_OFFSET                      *(uint32_t*)(MEMMAP_XSPI_XIP_BASE_ADDRESS  + INFER_NVM_A_OFFSET_IN_FLASH)

    // Model A address in flash: 0x3C00_0000 + {0x3C00_0000 + 0x4A000  + 0x18} + 0x280
    #define INFER_FLASH_MODEL_A_LOCATION            (MEMMAP_XSPI_XIP_BASE_ADDRESS \
                                                        + *(uint32_t*)(MEMMAP_XSPI_XIP_BASE_ADDRESS + INFER_NVM_A_OFFSET + INFER_FLASH_MODEL_A_OFFSET) \
                                                        + INFER_FLASH_HEADER_SIZE)


    // #define INFER_FLASH_MODEL_B_OFFSET              (0xA0)    

    // Model B address in flash: 0x3C00_0000 {0x3C00_0000 + 0xA0} + 0x280
    //#define INFER_FLASH_MODEL_B_LOCATION            (MEMMAP_XSPI_XIP_BASE_ADDRESS \
                                                        + *(uint32_t*)(MEMMAP_XSPI_XIP_BASE_ADDRESS + INFER_FLASH_MODEL_B_OFFSET) \
                                                        + INFER_FLASH_HEADER_SIZE)


#else
    #error "Chip not supported !!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void single_model_flash_sample_app(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __INFERENCE_BASIC_FLASH_SAMPLE_APP_H__ */
