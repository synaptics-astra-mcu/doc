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
 * \file	i2c_soc_config.h
 *
 * \brief	I2C Driver soc configuration
 */

#ifndef I2C_SOC_CONFIG_H
#define I2C_SOC_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "syna_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_SOC_SL2610 && CONFIG_BOARD_SL2610_PEK
    #define I2CM0_SCL_PIN        GPIO12
    #define I2CM0_SCL_PIN_FUNC   GPIO12__TW0_SCL
    #define I2CM0_SDA_PIN        GPIO13
    #define I2CM0_SDA_PIN_FUNC   GPIO13__TW0_SDA
    #define I2CM1_SCL_PIN        GPIO14
    #define I2CM1_SCL_PIN_FUNC   GPIO14__TW1_SCL
    #define I2CM1_SDA_PIN        GPIO15
    #define I2CM1_SDA_PIN_FUNC   GPIO15__TW1_SDA
    #define I2C_SLV_ADDR         0x43

#elif CONFIG_SOC_SL2610 && CONFIG_BOARD_SL2610_RDK
    #define I2CM0_SCL_PIN        GPIO12
    #define I2CM0_SCL_PIN_FUNC   GPIO12__TW0_SCL
    #define I2CM0_SDA_PIN        GPIO13
    #define I2CM0_SDA_PIN_FUNC   GPIO13__TW0_SDA
    #define I2CM1_SCL_PIN        GPIO14
    #define I2CM1_SCL_PIN_FUNC   GPIO14__TW1_SCL
    #define I2CM1_SDA_PIN        GPIO15
    #define I2CM1_SDA_PIN_FUNC   GPIO15__TW1_SDA
    #define I2C_SLV_ADDR         0x43

#elif CONFIG_SOC_SR110 && CONFIG_BOARD_SR110_EVB
    #define I2CM0_SCL_PIN        I2C0_MS_SCL
    #define I2CM0_SCL_PIN_FUNC   I2C0_MS_SCL__I2C0_MS_SCL
    #define I2CM0_SDA_PIN        I2C0_MS_SDA
    #define I2CM0_SDA_PIN_FUNC   I2C0_MS_SDA__I2C0_MS_SDA
    #define I2CM1_SCL_PIN        SPI_MSTR_MOSI
    #define I2CM1_SCL_PIN_FUNC   SPI_MSTR_MOSI__I2C1_MS_SCL
    #define I2CM1_SDA_PIN        SPI_MSTR_MISO
    #define I2CM1_SDA_PIN_FUNC   SPI_MSTR_MISO__I2C1_MS_SDA
    #define I2CS_SCL_PIN         I2C_SLV_SCL
    #define I2CS_SCL_PIN_FUNC    I2C_SLV_SCL__I2C_SLV_SCL
    #define I2CS_SDA_PIN         I2C_SLV_SDA
    #define I2CS_SDA_PIN_FUNC    I2C_SLV_SDA__I2C_SLV_SDA
    #define I2C_SLV_ADDR         0x20

#elif CONFIG_SOC_SR110 && CONFIG_BOARD_SR110_RDK
    #define I2CM0_SCL_PIN        I2C0_MS_SCL
    #define I2CM0_SCL_PIN_FUNC   I2C0_MS_SCL__I2C0_MS_SCL
    #define I2CM0_SDA_PIN        I2C0_MS_SDA
    #define I2CM0_SDA_PIN_FUNC   I2C0_MS_SDA__I2C0_MS_SDA
    #define I2CM1_SCL_PIN        SPI_MSTR_MOSI
    #define I2CM1_SCL_PIN_FUNC   SPI_MSTR_MOSI__I2C1_MS_SCL
    #define I2CM1_SDA_PIN        SPI_MSTR_MISO
    #define I2CM1_SDA_PIN_FUNC   SPI_MSTR_MISO__I2C1_MS_SDA
    #define I2CS_SCL_PIN         I2C_SLV_SCL
    #define I2CS_SCL_PIN_FUNC    I2C_SLV_SCL__I2C_SLV_SCL
    #define I2CS_SDA_PIN         I2C_SLV_SDA
    #define I2CS_SDA_PIN_FUNC    I2C_SLV_SDA__I2C_SLV_SDA
    #define I2C_SLV_ADDR         0x20

#else
    #error "Unsupported SoC configuration. Please define CONFIG_SOC_SL2610 or CONFIG_SOC_SR110"
#endif


#ifdef __cplusplus
}
#endif

#endif /* I2C_SOC_CONFIG_H */