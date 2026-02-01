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

#ifndef _SDIO_SAMPLE_APP_H_
#define _SDIO_SAMPLE_APP_H_

/*******************************************************************************
*                              Include files
*******************************************************************************/

#include "sdio.h"
#include "gpio.h"
#include "semphr.h"
#include "dev_common.h"

/*******************************************************************************
*                            Macro Definitions
*******************************************************************************/
#define SDIO_IO_OCR_C                 (0x80000000UL) 	/* The IO power up status (IORDY). */
#define SDIO_GLOBAL_BUFFER_SIZE       (512*2)  /* Global buffer size */

/* CMD53 constants. */
#define SDIO_SD_CMD53                 (53UL)
#define SDIO_CMD53_RWFLAG_POS         (31U)          /*The CMD53 RW Flag position. */
#define SDIO_CMD53_FUNCT_NUM_POS      (28U)          /*The CMD53 Function Number position. */
#define SDIO_CMD53_BLK_MODE_POS       (27U)          /*The CMD53 Block Mode position. */
#define SDIO_CMD53_OPCODE_POS         (26U)          /*The CMD53 Opcode position. */
#define SDIO_CMD53_REG_ADDR_POS       (9U)           /*The CMD53 Register Address position. */
#define SDIO_CMD53_DATA_MSK           (0xFFUL)       /*The CMD53 data mask. */

/*Data Configurations*/
#define SDIO_DATA_ADDRESS          0UL                  /* The address to write/read data on the
                                                         * card or eMMC. 
                                                         */
#define SDIO_DATA_NO_BLOCKS        1UL                  /* The number of blocks to write/read
                                                         * (Single block write/read). 
                                                         */
#define SDIO_DATA_AUTO_CMD         SDIO_AUTO_CMD_NONE   /* Selects which auto commands are used
                                                         * if any. 
                                                         */
#define SDIO_DATA_TIMEOUT          12UL                 /* The timeout value for the transfer. */
#define SDIO_DATA_EN_RELIABLE_DATA false                /* For EMMC cards enable reliable write. */
#define SDIO_DATA_EN_DMA           false                /* Enable DMA mode. */
#if CONFIG_BUILD_BOARD_SR110_RDK
#define SDIO_BASE_ADDRESS         MEMMAP_SDIO1_BASE_ADDRESS
#else
#define SDIO_BASE_ADDRESS         MEMMAP_SDIO0_BASE_ADDRESS
#endif

/*******************************************************************************
*                            Function Declarations
*******************************************************************************/

/*Creates the SDIO Sample App*/
void sdio_sample_app_create(void);

/*Starts the SDIO Sample App*/
void sdio_sample_app_start(void);

/*Main function of SDIO Sample App*/
void sdio_sample_app_main(void);

/*Create SDIO sample application task*/
void task_create(void);

#endif
