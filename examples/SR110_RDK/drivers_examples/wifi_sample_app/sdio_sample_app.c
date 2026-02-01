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
*                              Include files
*******************************************************************************/

#include "sdio_sample_app.h"

/*******************************************************************************
*                              Macro definitions
*******************************************************************************/

#define CACHE_TYPE_DATA            1
#define UC_MANAGER_TASK_PRIORITY   (tskIDLE_PRIORITY + 1)

#define SDIO_PROFILE_START(sdioProfilingVar)  TickType_t sdioProfilingVar = xTaskGetTickCount()
#define SDIO_PROFILE_END(sdioProfilingVar, api) \
    do { \
        TickType_t __end = xTaskGetTickCount(); \
        LOG_INFO(LOG_MOD_SDIO, "Time for %s = %lu msec\n", api, (__end - sdioProfilingVar)); \
    } while(0)


/*******************************************************************************
*                              Variable Definitions
*******************************************************************************/

/*Do not place the TX and RX buffers in DTCM region*/
static uint8_t rxBuff[SDIO_BLOCK_SIZE] SRAM_DATA_ATTRIBUTE; /* Data to read. */
static uint8_t txBuff[SDIO_BLOCK_SIZE] SRAM_DATA_ATTRIBUTE; /* Data to write. */
static int match_count;

/* Buffer for IO device block transfer */
static uint8_t global_tx_Buffer[SDIO_GLOBAL_BUFFER_SIZE] __attribute__((section(".nonCached")));
static uint8_t global_rx_Buffer[SDIO_GLOBAL_BUFFER_SIZE] __attribute__((section(".nonCached")));

/* unified buffer for IO device block transfer */
uint8_t g_sdiobuff[1024 * 8 + 256] __attribute__((section(".nonCached")));

static SemaphoreHandle_t sdio_completion_sem;  /* For ISR signaling */
static SemaphoreHandle_t sdio_mutex;           /* For mutual exclusion */

/* CMD53 interrupt state tracking */
static volatile uint32_t cmd53_initial_required_interrupts = 0;  /* Required interrupt mask */
static volatile uint32_t cmd53_initial_received_interrupts = 0;  /* Received interrupt mask */
static volatile bool cmd53_initial_operation_active = false; /* Flag to indicate operation in progress */

/* CMD53 xfer interrupt state tracking */
static volatile uint32_t cmd53_xfer_complete_active = false;
static volatile uint32_t cmd53_xfer_complete_received_interrupts = 0;
static volatile uint32_t cmd53_xfer_complete_required_interrupts = 0;
/*******************************************************************************
*                              Function definitions
*******************************************************************************/

/**
 * \brief Release the SDIO semaphore lock from ISR.
 * \param sem The semaphore instance.
 * \param xHigherPriorityTaskWoken High priority task woken
 * \return BaseType_t pdTRUE if the semaphore was given successfully,
 *         pdFALSE if there was an error or the semaphore was NULL
 */
__STATIC_INLINE BaseType_t sdio_unlock_from_isr_test(SemaphoreHandle_t sem,
                        BaseType_t *xHigherPriorityTaskWoken)
{
    if (!sem || !xHigherPriorityTaskWoken) {
        return pdFALSE; /* Return failure if parameters are invalid */
    }

    return xSemaphoreGiveFromISR(sem, xHigherPriorityTaskWoken);
}

/**
 * \brief Acquire the SDIO semaphore lock.
 * \param sem The semaphore instance.
 * \return sdio_status_en SDIO_OK on success, appropriate error code otherwise
 */
__STATIC_INLINE sdio_status_en sdio_lock_test(SemaphoreHandle_t sem)
{
    if (!sem) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! sdio_lock_test received NULL semaphore\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    BaseType_t ret = xSemaphoreTake(sem, portMAX_DELAY);
    if (ret != pdPASS) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! sdio lock fail = %d\n", ret);
        return SDIO_ERROR_TIMEOUT;
    }

    return SDIO_OK;
}

/**
 * \brief Release the SDIO semaphore lock.
 * \param sem The semaphore instance.
 * \return sdio_status_en SDIO_OK on success, appropriate error code otherwise
 */
__STATIC_INLINE sdio_status_en sdio_unlock_test(SemaphoreHandle_t sem)
{
    if (!sem) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! sdio_unlock_test received NULL semaphore\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    BaseType_t ret = xSemaphoreGive(sem);
    if (ret != pdPASS) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! sdio_unlock_test fail = %d\n", ret);
        return SDIO_ERROR_GENERAL;
    }

    return SDIO_OK;
}

/**
 * \brief	Interrupt service routine (ISR) for SDIO module 1
 * \return	none
 */
ISR_CODE void PERIF_SDIO1_Handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t handled;

    handled = sdio_isr(SDIO1);
    if (sdio_completion_sem != NULL) {
        /* Check if CMD53 operation is active */
        if (cmd53_initial_operation_active) {
            /* Accumulate received interrupts for CMD53 multi-interrupt completion */
            cmd53_initial_received_interrupts |= handled;

            /* Check if all required CMD53 interrupts have been received */
            if ((cmd53_initial_received_interrupts &
                    cmd53_initial_required_interrupts) == cmd53_initial_required_interrupts) {
                BaseType_t semResult = sdio_unlock_from_isr_test(sdio_completion_sem,
                                                                    &xHigherPriorityTaskWoken);
                if (semResult == pdTRUE) {
                    /* Reset CMD53 operation state */
                    cmd53_initial_operation_active = false;
                    cmd53_initial_required_interrupts = 0;
                    cmd53_initial_received_interrupts = 0;
                }
            }
        } else if (cmd53_xfer_complete_active) {
            cmd53_xfer_complete_received_interrupts |= handled;

            /* Check if the required CMD53 xfer interrupt has been received */
            if ((cmd53_xfer_complete_received_interrupts &
                 cmd53_xfer_complete_required_interrupts) ==
                cmd53_xfer_complete_required_interrupts) {
                BaseType_t semResult = sdio_unlock_from_isr_test(sdio_completion_sem,
                                                                    &xHigherPriorityTaskWoken);
                if (semResult == pdTRUE) {
                    /* Reset CMD53 xfer operation state */
                    cmd53_xfer_complete_active = false;
                    cmd53_xfer_complete_received_interrupts = 0;
                    cmd53_xfer_complete_required_interrupts = 0;
                }
            }
        } else {
            BaseType_t semResult = sdio_unlock_from_isr_test(sdio_completion_sem,
                                                                &xHigherPriorityTaskWoken);
            if (semResult == pdTRUE) {
                sdio_set_normal_interrupt_mask(SDIO1, 0);
            }
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * \brief Enables interrupts for a particular SDIO module.
 * \param id SDIO Instance ID.
 * \return \ref sdio_status_en
 */
__STATIC_INLINE sdio_status_en sys_enable_irq_test(sdio_instance_en id)
{
    /* Validate the SDIO instance ID */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Enable the appropriate IRQ based on instance ID */
    if (id == SDIO0) {
        NVIC_EnableIRQ(PERIF_SDIO0_IRQn);
    } else if (id == SDIO1) {
        NVIC_EnableIRQ(PERIF_SDIO1_IRQn);
    } else {
        /* This should never happen if the above validation is correct,
         * but included for safety */
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    return SDIO_OK;
}

/**
 * \brief Initializes the SDIO host controller for IO operations.
 *  After this function is called, the IO device must be initialized.
 * \param id SDIO Instance ID
 * \param config The pointer to the card configuration structure.
 * \return \ref sdio_status_en
 */
sdio_status_en sdio_host_init_test(sdio_instance_en id, sdio_config_t *config)
{
    sdio_status_en ret;

    /* Validate parameters */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Create binary semaphore for interrupt completion */
    sdio_completion_sem = xSemaphoreCreateBinary();
    if (sdio_completion_sem == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Binary semaphore creation failed\n");
        return SDIO_ERROR_GENERAL;
    }

    /* Mutex creation */
    sdio_mutex = xSemaphoreCreateMutex();
    if(sdio_mutex == NULL) {
        LOG_ERROR(LOG_MOD_INFERENCE, "Mutex creation failed\n");
        return SDIO_ERROR_GENERAL;
    }

    /* Perform SW reset on all controller components */
    ret = sdio_software_reset(id, SDIO_RESET_ALL);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Software reset failed with error %d\n", ret);
        /* Clean up resources */
        vSemaphoreDelete(sdio_completion_sem);
        sdio_completion_sem = NULL;
        return ret;
    }

    /* Set initial clock rate for identification mode (400 kHz) */
    ret = sdio_change_clock(id, SDIO_CLK_400K);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Clock configuration failed with error %d\n", ret);
        /* Clean up resources */
        vSemaphoreDelete(sdio_completion_sem);
        sdio_completion_sem = NULL;
        return ret;
    }

    /* Configure host voltage */
    ret = sdio_set_host_voltage(id);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Host voltage configuration failed with error %d\n", ret);
        /* Clean up resources */
        vSemaphoreDelete(sdio_completion_sem);
        sdio_completion_sem = NULL;
        return ret;
    }

    /* Configure GPIO pins for card power */
    gpio_pin_write(GPIO_PORT1, GPIO_43, 1);
    gpio_pin_write(GPIO_PORT0, GPIO_31, 1);

    /* Enable normal interrupts */
    sdio_set_normal_interrupt_enable(id, SDIO_NORMAL_INT_STAT_EN_R_CMD_COMPLETE_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_XFER_COMPLETE_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_BUF_WR_READY_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_BUF_RD_READY_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_CARD_INSERTION_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_CARD_INTERRUPT_STAT_EN_Msk |
                                          SDIO_NORMAL_INT_STAT_EN_R_CARD_REMOVAL_STAT_EN_Msk);

    /* Enable error interrupts */
    sdio_set_error_interrupt_enable(id, SDIO_ERROR_INT_STAT_EN_R_CMD_TOUT_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_CMD_CRC_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_CMD_END_BIT_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_CMD_IDX_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_DATA_TOUT_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_DATA_CRC_ERR_STAT_EN_Msk |
                                        SDIO_ERROR_INT_STAT_EN_R_DATA_END_BIT_ERR_STAT_EN_Msk);

    return SDIO_OK;
}

/**
 * \brief Initializes the IO device.
 * \param id SDIO Instance ID
 */
/**
 * \brief Initializes the SDIO IO device with comprehensive error handling
 * \param id SDIO Instance ID
 * \return \ref sdio_status_en
 */
sdio_status_en sdio_device_init_test(sdio_instance_en id)
{
    sdio_cmd_config_t cmd;
    sdio_status_en ret;
    uint32_t argument = 0, ocr = 0, resp = 0;
    uint32_t idx = 0;

    /* FUNCTIONAL FIX: Add parameter validation */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Enable IRQ */
    ret = sys_enable_irq_test(id);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to enable SDIO IRQ: %d\n", ret);
        return ret;
    }

    /* Set default block size and count */
    sdio_set_block_size_count(id);

    /* Send CMD0 (GO_IDLE_STATE) */
    ret = sdio_ops_go_idle(id);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to execute GO_IDLE_STATE command: %d\n", ret);
        return ret;
    }

    /* Send CMD52 to reset the card (Write 0x08 to CCCR reg 6) */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 1, 0, 0, 0x06, 0x08);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to reset card with CMD52: %d\n", ret);
        return ret;
    }

    /* Reset the command and data lines */
    sdio_software_reset(id, SDIO_RESET_CMD_LINE);
    sdio_software_reset(id, SDIO_RESET_DATALINE);
    sdio_clear_error_interrupt_status(id, 1);

    /* Send CMD5 with argument 0 to get OCR */
    ret = sdio_ops_sdio_send_op_cond(id, &ocr, 0);
    if (ret != SDIO_OK) {
        return ret;
    }

    /* FUNCTIONAL FIX: Add timeout protection for Loop with CMD5 */
    ocr = 0x000100; /* Start with a valid voltage window */
    idx = 0;
    do {
        ret = sdio_ops_sdio_send_op_cond(id, &resp, ocr);
        if (ret != SDIO_OK) {
            return ret;
        }
        /* Check the C bit (IORDY) in the R4 response */
        if (idx++ > 200) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! Timeout waiting for card ready (CMD5)\n");
            return SDIO_ERROR_TIMEOUT;
        }
    } while (!(resp & SDIO_IO_OCR_C));

    /* Send CMD3 to get RCA */
    ret = sdio_fetch_rca(id);
    if (ret != SDIO_OK) {
        return ret;
    }

    /* Send CMD7 to select the card */
    ret = sdio_ops_select_card(id);
    if (ret != SDIO_OK) {
        return ret;
    }

    /* Enable Function 1 (Write 0x02 to CCCR reg 2) */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 1, 0, 0, 0x02, 0x02);
    if (ret != SDIO_OK) {
        return ret;
    }

    /* Wait until Function 1 is ready (Poll CCCR reg 3) */
    idx = 0;
    do {
        ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x03, 0x00);
        if (ret != SDIO_OK) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to poll Function 1 readiness: %d\n", ret);
            return ret;
        }

        ret = sdio_get_response(id, &resp, false);
        if (ret != SDIO_OK) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for Function 1 readiness check: %d\n", ret);
            return ret;
        }

        /* Implement timeout with informative messages */
        if (idx++ > 200) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! Timeout waiting for Function 1 to become ready\n");
            return SDIO_ERROR_TIMEOUT;
        }

        /* Add delay between polling attempts */
        if (!(resp & 0x02) && (idx % 20 == 0)) {
            LOG_INFO(LOG_MOD_SDIO, "Still waiting for Function 1 to become ready (attempt %d)...\n", idx);
        }
    } while (!(resp & 0x02));

    /* Get capabilities (Read CCCR reg 0x08) */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x08, 0x00);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to read card capabilities: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for capabilities check: %d\n", ret);
        return ret;
    }

    /* Validate capabilities are compatible with our requirements */
    if (!(resp & 0x02)) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Card does not support 4-bit mode\n");
        return SDIO_ERROR_NOT_SUPPORTED;
    }

    /* Get bus speed capabilities */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x13, 0x00);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to read bus speed register: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for bus speed check: %d\n", ret);
        return ret;
    }

    /* Reading the CCCR 0x14, UHS-I support */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x14, 0x00);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to read UHS-I support register: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for UHS-I support check: %d\n", ret);
        return ret;
    }

    /* Reading the CCCR 0x15, driver strength */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x15, 0x00);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to read driver strength register: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for driver strength check: %d\n", ret);
        return ret;
    }

    /* Log driver strength for debugging */
    LOG_INFO(LOG_MOD_SDIO, "Driver Strength Register: 0x%08X\n", resp);

    /* Enable Interrupts for 3 functions */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 1, 0, 0, 0x4, 0x03);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to enable interrupts: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for interrupt enable operation: %d\n", ret);
        return ret;
    }

    /* Set 4-bit bus width (Write 0x02 to CCCR reg 7) */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 1, 0, 0, 0x07, 0x02);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to set card bus width to 4-bit mode: %d\n", ret);
        return ret;
    }

    /* Verify bus width setting */
    ret = sdio_ops_send_io_rw_direct_cmd(id, 0, 0, 0, 0x07, 0x00);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to verify bus width setting: %d\n", ret);
        return ret;
    }

    ret = sdio_get_response(id, &resp, false);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to get response for bus width verification: %d\n", ret);
        return ret;
    }

    if ((resp & 0x03) != 0x02) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Bus width verification failed. Expected 0x02, got 0x%02X\n", resp & 0x03);
        return SDIO_ERROR_GENERAL;
    }

    /* Set host controller bus width */
    ret = sdio_set_host_bus_width(id, SDIO_BUS_WIDTH_4_BIT);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to set host bus width: %d\n", ret);
        return ret;
    }

    /* Change clock to a higher speed */
    ret = sdio_change_clock(id, SDIO_CLK_25M);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Failed to change clock speed: %d\n", ret);
        return ret;
    }

    LOG_INFO(LOG_MOD_SDIO, "SDIO device initialized successfully on instance %d\n", id);
    return SDIO_OK;
}

/**
 * \brief Sends CMD52 using interrupts.
 * \param id SDIO Instance ID.
 * \param rwFlag Read/Write flag. Set to 1 for write, 0 for read.
 * \param functionNumber SDIO function number to access (0-7).
 * \param rawFlag Read After Write flag. Used when performing write with readback verification.
 * \param registerAddress The register address to access (0-0x1FFFF).
 * \param data Data to write or dummy value for read (0-0xFF).
 * \return \ref sdio_status_en
 */
sdio_status_en sdio_sd_cmd52_test(sdio_instance_en id, uint32_t rwFlag, uint32_t functionNumber,
    uint32_t rawFlag, uint32_t registerAddress, uint32_t *data)
{
    sdio_status_en ret;

    /* Validate parameters */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Validate SDIO function number (0-7) */
    if (functionNumber > 7) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid function number %d (valid range: 0-7)\n",
                    functionNumber);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Validate R/W flag (0 or 1) */
    if (rwFlag > 1) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid R/W flag %d (valid values: 0-1)\n", rwFlag);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Validate RAW flag (0 or 1) */
    if (rawFlag > 1) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid RAW flag %d (valid values: 0-1)\n", rawFlag);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Validate register address (17-bit address space) */
    if (registerAddress > 0x1FFFF) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid register address 0x%x (max: 0x1FFFF)\n",
                    registerAddress);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* FUNCTIONAL FIX: Add NULL check for data pointer */
    if (data == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! NULL data pointer\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }
 
    /* FUNCTIONAL FIX: Correct dereferencing - was: if (*data > 0xFF) */
    if (rwFlag && (*data > 0xFF)) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid data value 0x%x (max: 0xFF)\n", *data);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Enable command complete interrupt with error checking */
    sdio_set_normal_interrupt_mask(id, SDIO_NORMAL_INT_SIGNAL_EN_R_CMD_COMPLETE_SIGNAL_EN_Msk);

    /* Send the command with comprehensive error reporting */
    ret = sdio_ops_send_io_rw_direct_cmd(id, rwFlag, functionNumber, rawFlag, registerAddress,
                                            *data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! CMD52 failed with error code %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "Failed CMD52 parameters: rwFlag=%d, func=%d, reg=0x%05X\n",
                    rwFlag, functionNumber, registerAddress);
    }

    /* FUNCTIONAL IMPROVEMENT: Handle RAW flag response */
    if (rwFlag == 0 || rawFlag == 1) {
        ret = sdio_get_response(id, data, false);
        if (ret != SDIO_OK) {
            LOG_ERROR(LOG_MOD_SDIO,
                      "Error! Failed to get response for interrupt enable operation: %d\n", ret);
        }
    }

    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "2nd CMD52 failed!\n");
    }

    return ret;
}

/**
 * \brief Initializes data transfer and sends an SDIO command.
 * \param id   SDIO instance ID.
 * \param cmd  Pointer to the SDIO command configuration structure.
 * \param data Pointer to the SDIO data configuration structure.
 * \return \ref sdio_status_en Status of the operation.
 */
sdio_status_en sdio_send_cmd_and_data(sdio_instance_en id, sdio_cmd_config_t *cmd, sdio_data_config_t *data)
{
    sdio_status_en ret;

    /* FUNCTIONAL FIX: Add parameter validation */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }
 
    if (cmd == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! NULL command pointer\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }
 
    if (data == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! NULL data pointer\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    ret = sdio_init_data_transfer(id, data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "sdio_init_data_transfer failed!\n");
        return ret;
    }

    __NVIC_DisableIRQ(PERIF_SDIO1_IRQn);
    ret = sdio_send_command(id, cmd);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "sdio_send_command failed!\n");
        return ret;
    }
    NVIC_EnableIRQ(PERIF_SDIO1_IRQn);

    /* FUNCTIONAL FIX: Validate semaphore before taking it */
    if (sdio_completion_sem == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Completion semaphore is NULL\n");
        return SDIO_ERROR_GENERAL;
    }

    if (xSemaphoreTake(sdio_completion_sem, 20000) != pdPASS) {
        LOG_ERROR(LOG_MOD_SDIO, "Timeout waiting for CMD53 completion!\n");
        /* FUNCTIONAL FIX: Atomic reset of interrupt state tracking */
        taskENTER_CRITICAL();
        cmd53_initial_operation_active = false;
        cmd53_initial_received_interrupts = 0;
        cmd53_initial_required_interrupts = 0;
        taskEXIT_CRITICAL();
        return SDIO_ERROR_GENERAL;
    }

    return ret;
}

/**
 * \brief Writes data to the SDIO device without using DMA.
 * This function initiates a non-DMA data transmission using the SDIO interface.
 * \param id   SDIO instance ID.
 * \param data Pointer to the SDIO data configuration structure.
 * \return \ref sdio_status_en Status of the operation.
 */
sdio_status_en sdio_write_data_non_dma(sdio_instance_en id, sdio_data_config_t* data )
{
    sdio_status_en ret;

    ret = sdio_cmd_tx_data(id, data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Issue with sdio_cmd_tx_data!\n");
    }

    return ret;
}

/**
 * \brief Reads data from the SDIO device without using DMA.
 * This function initiates a non-DMA data reception using the SDIO interface.
 * \param id   SDIO instance ID.
 * \param data Pointer to the SDIO data configuration structure.
 * \return \ref sdio_status_en Status of the operation.
 */
sdio_status_en sdio_read_data_non_dma(sdio_instance_en id, sdio_data_config_t *data )
{
    sdio_status_en ret;

    ret = sdio_cmd_rx_data(id, data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Issue with sdio_cmd_rx_data!\n");
    }

    return ret;
}

/**
 * \brief Sends CMD53 (Extended I/O read/write) using interrupts.
 * \param id SDIO instance ID.
 * \param read Read/Write flag. Set to 0 for read, 1 for write.
 * \param funcNum SDIO function number to access (0-7).
 * \param block_mode Block mode flag. Set to 1 for block mode, 0 for byte mode.
 * \param opCode Operation code. Set to 0 for fixed address, 1 for incrementing address.
 * \param regAddr Register address to access (0-0x1FFFF).
 * \param byteCnt Number of bytes or blocks to transfer.
 * \param LocalBuffer Pointer to the data buffer for read/write.
 * \return \ref sdio_status_en
 */
sdio_status_en sdio_sd_cmd53_test(sdio_instance_en id, uint32_t read, uint32_t funcNum, uint32_t block_mode,
    uint32_t opCode, uint32_t regAddr, uint32_t byteCnt, unsigned char *LocalBuffer)
{
    sdio_status_en ret;
    sdio_dma_type_en dmaType = SDIO_DMA_SDMA;

    /* Validate parameters */
    if (id >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", id);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (read > 1) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid read flag %d (valid values: 0-1)\n", read);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (funcNum > 7) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid function number %d (valid range: 0-7)\n", funcNum);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (block_mode > 1) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid block mode %d (valid values: 0-1)\n", block_mode);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (opCode > 1) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid opcode %d (valid values: 0-1)\n", opCode);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (regAddr > 0x1FFFF) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid register address 0x%x (max: 0x1FFFF)\n", regAddr);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (LocalBuffer == NULL) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! NULL data buffer provided\n");
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    if (byteCnt > SDIO_DMA_TXR_BLOCK_SIZE) {
        dmaType = SDIO_DMA_ADMA2;
    }

    ret = sdio_set_dma_type(SDIO1, dmaType);

    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO Set DMA Type Failure: %d\n", ret);
        return ret;
    }

    /* if write, copy from local buffer to noncached buf */
    if (read)
        memcpy(g_sdiobuff, LocalBuffer, byteCnt);
        
    sdio_data_config_t data;

    data.blockSize = (block_mode == 1) ? ((funcNum == 2) ? 256 : 64) : byteCnt;
    data.numberOfBlock = (block_mode == 1) ? (byteCnt / data.blockSize) : 1;
    data.byteCnt = byteCnt;

    /* Construct CMD53 */
    sdio_cmd_config_t cmd = {
        .commandIndex                 = SDIO_SD_CMD53,
        .commandArgument              = (((read        ? 1UL : 0UL) << SDIO_CMD53_RWFLAG_POS) |
                                        ((funcNum    & 0x7UL)    << SDIO_CMD53_FUNCT_NUM_POS) |
                                        ((block_mode  ? 1UL : 0UL) << SDIO_CMD53_BLK_MODE_POS) |
                                        ((opCode     ? 1UL : 0UL) << SDIO_CMD53_OPCODE_POS) |
                                        ((regAddr     & 0x1FFFFUL) << SDIO_CMD53_REG_ADDR_POS) |
                                        ((((block_mode == 1) ? (byteCnt / data.blockSize) : byteCnt)       & 0x1FFUL))),
        .enableAutoResponseErrorCheck = false,
        .respType                     = SDIO_RESPONSE_LEN_48,
        .enableCrcCheck               = true,
        .enableIdxCheck               = true,
        .dataPresent                  = true
    };

    data.autoCommand = SDIO_AUTO_CMD_NONE;
    data.enableDma = true;
    data.data = (uint32_t*)g_sdiobuff;
    data.read = (read == 0) ? true : false;
    data.enReliableWrite = false;

    taskENTER_CRITICAL();
    /* Set up CMD53 interrupt state tracking */
    cmd53_initial_operation_active = true;
    cmd53_initial_received_interrupts = 0;

    /* Define required interrupts for CMD53 completion */
    if (!data.enableDma) {
        cmd53_initial_required_interrupts =
        SDIO_NORMAL_INT_SIGNAL_EN_R_CMD_COMPLETE_SIGNAL_EN_Msk;

        if (read) {
            cmd53_initial_required_interrupts |= SDIO_NORMAL_INT_SIGNAL_EN_R_BUF_WR_READY_SIGNAL_EN_Msk;
        } else {
            cmd53_initial_required_interrupts |= SDIO_NORMAL_INT_SIGNAL_EN_R_BUF_RD_READY_SIGNAL_EN_Msk;
        }
    } else {
        cmd53_initial_required_interrupts =
        SDIO_NORMAL_INT_SIGNAL_EN_R_CMD_COMPLETE_SIGNAL_EN_Msk |
        SDIO_NORMAL_INT_SIGNAL_EN_R_XFER_COMPLETE_SIGNAL_EN_Msk;
    }
    taskEXIT_CRITICAL();

    /* Enable the required interrupts */
    sdio_set_normal_interrupt_mask(id, cmd53_initial_required_interrupts|SDIO_NORMAL_INT_SIGNAL_EN_R_XFER_COMPLETE_SIGNAL_EN_Msk);

    /* Send the command */
    ret = sdio_send_cmd_and_data(id, &cmd, &data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! CMD53 command failed with error code %d\n", ret);
        /* FUNCTIONAL FIX: Clear state on failure */
        taskENTER_CRITICAL();
        cmd53_initial_operation_active = false;
        taskEXIT_CRITICAL();
        return ret;
    }


    /* if read, copy from noncached buf to local buf */
    if (!read)
        memcpy(LocalBuffer, g_sdiobuff, byteCnt);

    if (!data.enableDma) {
        cmd53_xfer_complete_active = true;
        cmd53_xfer_complete_required_interrupts = SDIO_NORMAL_INT_SIGNAL_EN_R_XFER_COMPLETE_SIGNAL_EN_Msk;

        /* Transfer data word by word when not using DMA */
        if (!data.read) {
            /* Write data to the card */
            sdio_status_en ret_buf = sdio_write_data_non_dma(id, &data);
            if (ret_buf != SDIO_OK) {
                LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO write data failed %d\n", ret_buf);
                /* FUNCTIONAL FIX: Clear state on failure */
                taskENTER_CRITICAL();
                cmd53_initial_operation_active = false;
                taskEXIT_CRITICAL();
                return ret_buf;
            }
        }

        /* Perform read operation if needed */
        if (data.read) {
            /* Read data from the card */
            sdio_status_en ret_buf = sdio_read_data_non_dma(id, &data);
            if (ret_buf != SDIO_OK) {
                LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO read data failed %d\n", ret_buf);
                /* FUNCTIONAL FIX: Clear state on failure */
                taskENTER_CRITICAL();
                cmd53_initial_operation_active = false;
                taskEXIT_CRITICAL();
                return ret_buf;
            }
        }

        if(xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO, "Timeout waiting for CMD53 XFER completion!\n");
            /* Reset xfer interrupt state tracking */
            cmd53_xfer_complete_active = false;
            cmd53_xfer_complete_received_interrupts = 0;
            cmd53_xfer_complete_required_interrupts = 0;
            return SDIO_ERROR_GENERAL;
        }
    }
    /* Disable interrupts after completion */
    sdio_set_normal_interrupt_mask(id, 0);

    return SDIO_OK;
}

/**
 * \brief Checks if the data written matches the data read.
 * \param tx Pointer to the transmitted data buffer.
 * \param rx Pointer to the received data buffer.
 * \return None.
 */
static void sdio_cmd53_write_read_check_test(uint8_t *tx, uint8_t *rx , uint32_t size) {
    for (int i = 0; i < size; ++i) {
        if (tx[i] != rx[i]) {
            LOG_ERROR(LOG_MOD_SDIO, "Mismatch found at index: %d %02X %02X\n", i, tx[i], rx[i]);
            return;
        }
    }
    LOG_INFO(LOG_MOD_SDIO, "All values matched!\n");
}

/**
 * \brief Check SDIO transfer result and handle errors appropriately
 * \param ret The return code from the SDIO operation to check
 * \param dma_enabled Whether DMA was enabled for the transfer
 * \param is_read Whether this was a read operation (true) or write operation (false)
 * \param instance The SDIO instance used for the transfer
 * \return sdio_status_en SDIO_OK on success, or the appropriate error code
 */
sdio_status_en transfer_check(sdio_status_en ret, bool dma_enabled, bool is_read, sdio_instance_en instance)
{
    /* Validate parameters */
    if (instance >= SDIO_COUNT) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid SDIO instance ID %d\n", instance);
        return SDIO_ERROR_INVALID_PARAMETER;
    }

    /* Check transfer status */
    if ((ret == SDIO_OK) && dma_enabled) {
        uint32_t timeout_counter = 0;
        uint32_t max_timeout = 1000000; /* Appropriate timeout value for the platform */

        /* Wait for transfer completion with timeout */
        while ((SDIO_XFER_COMPLETE != (sdio_get_normal_interrupt_status(instance) &
                                        SDIO_XFER_COMPLETE))) {
            /* Implement a timeout to avoid infinite loop */
            if (++timeout_counter >= max_timeout) {
                LOG_ERROR(LOG_MOD_SDIO, "Error! DMA transfer timeout\n");
                return SDIO_ERROR_TIMEOUT;
            }
        }

        /* Clear the data-transaction complete event */
        sdio_clear_normal_interrupt_status(instance, SDIO_XFER_COMPLETE);
        return SDIO_OK;
    } else if (ret != SDIO_OK) {
        /* Provide more descriptive error messages based on error code */
        const char* operation = is_read ? "Read" : "Write";

        switch (ret) {
            case SDIO_ERROR_TIMEOUT:
                LOG_ERROR(LOG_MOD_SDIO, "%s Error: Operation timed out\n", operation);
                break;
            case SDIO_ERROR_CRC:
                LOG_ERROR(LOG_MOD_SDIO, "%s Error: CRC check failed\n", operation);
                break;
            case SDIO_ERROR_DATA_END_BIT:
                LOG_ERROR(LOG_MOD_SDIO, "%s Error: Data end bit error\n", operation);
                break;
            case SDIO_ERROR_INVALID_PARAMETER:
                LOG_ERROR(LOG_MOD_SDIO, "%s Error: Invalid parameter\n", operation);
                break;
            default:
                LOG_ERROR(LOG_MOD_SDIO, "%s Error: Code %d\n", operation, ret);
                break;
        }

        return ret;
    }

    return SDIO_OK;
}

static void cm52_read_write_and_check()
{
    uint32_t function_number = 0x1;
    uint32_t raw_flag = 0x0;
    uint32_t reg_addr = 0x0001000b;
    uint32_t data = 25;
    uint32_t mismatch_counter = 0;
    uint32_t old_data = 0;
    uint32_t rw_flag = 0x1;
    sdio_status_en ret = 0;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    rw_flag = 0x0;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &old_data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    if ((uint8_t)old_data != data) {
            LOG_INFO(LOG_MOD_SDIO,"DATA MISMATCHED!!\n");
    } else {
            LOG_INFO(LOG_MOD_SDIO,"DATA MATCHED!!\n");
    }

}

void sdio_sample_app_create()
{
#if CONFIG_BOARD_SR110_EVB
    /* Initialize SDIO host for SD card */
    sdio_status_en ret;

    LOG_INFO(LOG_MOD_SDIO, "Starting SDIO sample application for EVB board...\n");
    LOG_INFO(LOG_MOD_SDIO, "Initializing SDIO controller on instance %d...\n", SDIO0);

    /* Populate configuration structure */
    const sdio_config_t sd_host_config = {
        .dmaType = CONFIG_SDIO_HOST_DMA_TYPE,
    };

    /* Initialize SDIO controller with enhanced error handling */
    ret = sdio_init(SDIO0);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO controller initialization failed: %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "  - Unable to continue with sample app\n");
        return; /* Early return on critical initialization failure */
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO controller initialized successfully\n");
    }

    /* Configure SDIO controller with enhanced error handling */
    LOG_INFO(LOG_MOD_SDIO, "Configuring SDIO controller parameters...\n");
    ret = sdio_config(SDIO0, &sd_host_config);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO controller configuration failed: %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "  - Unable to continue with sample app\n");
        return; /* Early return on critical configuration failure */
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO controller configured successfully\n");
    }

    sdio_card_type_en cardType;
    uint32_t rca;
    sdio_card_capacity_en cardCapacity;

    /* Populate configuration structure */
    sdio_sd_card_config_t sdCardConfig = {
#ifdef CONFIG_SDIO_CARD__LOW_VOLTAGE_SIGNALING
        .lowVoltageSignaling = true,
#endif
        .busWidth = CONFIG_SDIO_CARD_BUS_WIDTH,
        .cardType = &cardType,
        .rca = &rca,
        .cardCapacity = &cardCapacity,
        .clockSpeed = CONFIG_SDIO_CLOCK,
        .busSpeed = CONFIG_SDIO_SDR50_SPEED,
    };

    /* Initialize the SD card with comprehensive error handling */
    LOG_INFO(LOG_MOD_SDIO, "Initializing SD card with speed %d and bus width %d...\n",
                sdCardConfig.clockSpeed, sdCardConfig.busWidth);

    ret = sdio_sd_card_init(SDIO0, &sdCardConfig);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SD card initialization failed: %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "  - Check if card is inserted properly\n");
        LOG_ERROR(LOG_MOD_SDIO, "  - Check if card is compatible with requested settings\n");
        /* Continue despite card init failure - some features might still work */
        LOG_INFO(LOG_MOD_SDIO, "SDIO initialization completed with errors\n");
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SD card initialized successfully\n");

        /* Log card information for debugging */
        LOG_INFO(LOG_MOD_SDIO, "Card information:\n");
        LOG_INFO(LOG_MOD_SDIO, "  - Card type: %d\n", *sdCardConfig.cardType);
        LOG_INFO(LOG_MOD_SDIO, "  - Card RCA: 0x%04X\n", *sdCardConfig.rca);
        LOG_INFO(LOG_MOD_SDIO, "  - Card capacity: %d\n", *sdCardConfig.cardCapacity);

        LOG_INFO(LOG_MOD_SDIO, "SDIO initialization completed successfully\n");
    }

#elif CONFIG_BOARD_SR110_RDK
    /* Initialize SDIO host for IO device */
    sdio_status_en ret;

    /* Populate configuration structure */
    sdio_config_t sd_host_config = {
        .dmaType = CONFIG_SDIO_HOST_DMA_TYPE,
    };

    /* Initialize SDIO controller */
    ret = sdio_init(SDIO1);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO Init Failure: %d\n", ret);
        LOG_ERROR(LOG_MOD_SDIO, "  - Unable to continue with sample app\n");
        return; /* Early return on critical initialization failure */
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO Init Success\n");
    }

    /* Initialize SDIO host with improved error handling */
    ret = sdio_host_init_test(SDIO1, &sd_host_config);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO Host Init Failure: %d\n", ret);
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO Host Init Success\n");
    }

    sdio_card_type_en cardType;
    uint32_t rca;
    sdio_card_capacity_en cardCapacity;

    /* Populate configuration structure */
    sdio_sd_card_config_t sdCardConfig = {
#ifdef CONFIG_SDIO_CARD__LOW_VOLTAGE_SIGNALING
        .lowVoltageSignaling = true,
#endif
        .busWidth = CONFIG_SDIO_CARD_BUS_WIDTH,
        .cardType = &cardType,
        .rca = &rca,
        .cardCapacity = &cardCapacity,
        .clockSpeed = CONFIG_SDIO_CLOCK,
        .busSpeed = CONFIG_SDIO_SDR50_SPEED,
    };

    /* Initialize the IO Device with proper error handling */
    ret = sdio_device_init_test(SDIO1);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! SDIO IO Device Init Failure: %d\n", ret);
    } else {
        LOG_INFO(LOG_MOD_SDIO, "SDIO IO Device Init Success\n");
    }
    LOG_INFO(LOG_MOD_SDIO, "SDIO Initialization Complete\n");
#endif
}

void sdio_sample_app_main()
{
#if CONFIG_BOARD_SR110_EVB
    sdio_write_read_config_t data;
    sdio_status_en ret;

    data.address         = SDIO_DATA_ADDRESS;
    data.numberOfBlocks  = SDIO_DATA_NO_BLOCKS;
    data.autoCommand     = SDIO_DATA_AUTO_CMD;
    data.dataTimeout     = SDIO_DATA_TIMEOUT;
    data.enReliableWrite = SDIO_DATA_EN_RELIABLE_DATA;
    data.enableDma       = SDIO_DATA_EN_DMA;

    /* Clear any pending interrupts */
    sdio_clear_normal_interrupt_status(SDIO0, SDIO_XFER_COMPLETE);

    /* Populate data buffers */
    int32_t i;
    for (i = 0; i < SDIO_BLOCK_SIZE; i++) {
        rxBuff[i] = 0x00;
        txBuff[i] = (uint8_t)(rand());
    }

    /* Perform data transfer operations using CMD53 */
    LOG_INFO(LOG_MOD_SDIO, "Starting SD card data transfer tests\n");

    /*force TX buffer into memory*/
    cache_clean_addr((uint32_t*)&txBuff[0], SDIO_BLOCK_SIZE);

    data.data = (uint32_t*)txBuff;          /* The pointer to data to write. */
    SDIO_PROFILE_START(sdioWrite);
    ret = sdio_sd_card_write(SDIO0,&data);          /* Write data to the card. */
    SDIO_PROFILE_END(sdioWrite, "sdio_sd_card_write");
    transfer_check(ret, data.enableDma, false, SDIO0);

    data.data = (uint32_t*)rxBuff;          /* The pointer to data to read. */
    SDIO_PROFILE_START(sdioRead);
    ret = sdio_sd_card_read(SDIO0, &data);          /* Read data from the card. */
    SDIO_PROFILE_END(sdioRead, "sdio_sd_card_read");
    transfer_check(ret, data.enableDma, true, SDIO0);

    /*force read buffer from memory*/
    if(data.enableDma == false) {
        cache_clean_addr((uint32_t*)&rxBuff[0], SDIO_BLOCK_SIZE);
    }
    cache_invalidate_addr(CACHE_TYPE_DATA, (void *)rxBuff, SDIO_BLOCK_SIZE);

    /* Verify data integrity with comprehensive error reporting */
    bool data_match = true;
    uint32_t mismatch_count = 0;
    uint32_t first_mismatch_index = 0;

    /* Validate buffer pointers before verification */
    if (!rxBuff || !txBuff) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! Invalid buffer pointers for data verification\n");
        return;
    }

    LOG_INFO(LOG_MOD_SDIO, "Verifying %d bytes of data integrity...\n", SDIO_BLOCK_SIZE);

    /* Compare data with detailed mismatch tracking */
    for (i = 0; i < SDIO_BLOCK_SIZE; i++) {
        if (rxBuff[i] != txBuff[i]) {
            if (mismatch_count == 0) {
                first_mismatch_index = i;
            }
            mismatch_count++;

            /* Log first few mismatches in detail */
            if (mismatch_count <= 5) {
                LOG_ERROR(LOG_MOD_SDIO, "Data mismatch at offset %d: TX=0x%02x, RX=0x%02x\n",
                            i, txBuff[i], rxBuff[i]);
            }
            /* Don't break early - count all mismatches for better diagnostics */
        }
    }

    /* Report verification results with detailed statistics */
    if (mismatch_count == 0) {
        LOG_INFO(LOG_MOD_SDIO, "Data verification successful - all %d bytes match\n", SDIO_BLOCK_SIZE);
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "Data verification FAILED: %d mismatches detected (%.1f%% error rate)\n",
                 mismatch_count, (float)mismatch_count * 100.0f / SDIO_BLOCK_SIZE);
        LOG_ERROR(LOG_MOD_SDIO, "First mismatch at offset %d (0x%04X)\n", first_mismatch_index, first_mismatch_index);

        /* Provide a hex dump of the first few bytes around the first mismatch for debugging */
        uint32_t start_dump = (first_mismatch_index > 4) ? (first_mismatch_index - 4) : 0;
        uint32_t end_dump = (first_mismatch_index + 4 < SDIO_BLOCK_SIZE) ? (first_mismatch_index + 4) : (SDIO_BLOCK_SIZE - 1);

        LOG_ERROR(LOG_MOD_SDIO, "TX Buffer dump around mismatch:\n");
        for (i = start_dump; i <= end_dump; i++) {
            LOG_ERROR(LOG_MOD_SDIO, "0x%02X ", txBuff[i]);
        }
        LOG_ERROR(LOG_MOD_SDIO, "\n");

        LOG_ERROR(LOG_MOD_SDIO, "RX Buffer dump around mismatch:\n");
        for (i = start_dump; i <= end_dump; i++) {
            LOG_ERROR(LOG_MOD_SDIO, "0x%02X ", rxBuff[i]);
        }
        LOG_ERROR(LOG_MOD_SDIO, "\n");

        /* Return error status to indicate data verification failure */
        LOG_ERROR(LOG_MOD_SDIO, "SDIO Data Integrity Compromised.\n");
    }
#elif CONFIG_BOARD_SR110_RDK
    /*Byte and block mode transfer*/

    uint32_t funcNum = 1;           /* function number to access */
    uint32_t block_mode = 0;        /* block mode flag */
    uint32_t opCode = 1;            /* operation code */
    uint32_t regAddr = 0x18000000;
    uint32_t reg2 = 0x170000;
    uint32_t byteCnt = 4;           /* no of bytes/blocks to transfer */
    uint32_t read = 0;              /* 0 - read, 1 - write */

    /* Memset for buffers */
    memset(global_tx_Buffer, 0, sizeof(global_tx_Buffer));

    /* Chip ID Read - Start */

    uint32_t rw_flag = 0x1;
    uint32_t function_number = 0x1;
    uint32_t raw_flag = 0x0;
    uint32_t reg_addr = 0x0001000c;
    uint32_t data = (regAddr >> 24) & 0xff;

    sdio_lock_test(sdio_mutex);
    sdio_status_en ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr,
                                            &data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "1st CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    rw_flag = 0x1;
    function_number = 0x1;
    raw_flag = 0x0;
    reg_addr = 0x0001000b;
    data = (regAddr >> 16) & 0xff;

    sdio_lock_test(sdio_mutex);

    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "2nd CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    reg_addr = 0x0;

    sdio_lock_test(sdio_mutex);
    sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt, global_tx_Buffer);
    if (SDIO_OK != ret) {
        LOG_ERROR(LOG_MOD_SDIO, "1st CMD53 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    LOG_INFO(LOG_MOD_SDIO,"Read Chip ID : 0x%02x %02x %02x %02x\n",
        global_tx_Buffer[0], global_tx_Buffer[1], global_tx_Buffer[2], global_tx_Buffer[3]);

    /* Chip ID Read - Complete */

    /* CMD52 Read/Write verification - Start */
    function_number = 0x1;
    raw_flag = 0x0;
    reg_addr = 0x0001000b;
    data = 0;
    uint32_t mismatch_counter = 0;
    uint32_t old_data = 0;

    LOG_INFO(LOG_MOD_SDIO, "CMD52 Read/Write verification: start\n");
    for (data = 0; data <= 255; data++) {
        rw_flag = 0x1;
        /* Execute CMD52 Write */
        sdio_lock_test(sdio_mutex);
        ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr,
                                                &data);
        if (ret != SDIO_OK) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! CMD52 Write failed with error: %d\n", ret);
        } else {
            if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
                LOG_ERROR(LOG_MOD_SDIO, "Error! Timeout waiting for CMD52 Write completion\n");
                ret = SDIO_ERROR_TIMEOUT;
            }
        }
        sdio_unlock_test(sdio_mutex);

        /* Execute CMD52 Read */
        uint8_t rw_flag = 0x0;
        old_data = 0;
        sdio_lock_test(sdio_mutex);
        ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr,
                                 &old_data);
        if (ret != SDIO_OK) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! CMD52 Read failed with error: %d\n", ret);
        } else {
            if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
                LOG_ERROR(LOG_MOD_SDIO, "Error! Timeout waiting for CMD52 Read completion\n");
                ret = SDIO_ERROR_TIMEOUT;
            }
        }
        sdio_unlock_test(sdio_mutex);

        /* Compare values */
        if ((uint8_t)old_data != data) {
            LOG_ERROR(LOG_MOD_SDIO,
                      "Mismatch! Written: 0x%02X, Read: 0x%02X\n", data, (uint8_t)old_data);
            mismatch_counter++;
        }
        vTaskDelay(10);
    }

    /* Summary */
    if (mismatch_counter > 0) {
        LOG_ERROR(LOG_MOD_SDIO,
                  "CMD52 Verification completed with %u mismatches\n", mismatch_counter);
    } else {
        LOG_INFO(LOG_MOD_SDIO, "CMD52 Verification successful! All values matched\n");
    }
    LOG_INFO(LOG_MOD_SDIO, "CMD52 Read/Write verification: completed\n");

    /* CMD52 Read/Write verification - Complete */

    /* Single Block Transfer Test - Start */
    rw_flag = 0x1;
    function_number = 0x0;
    raw_flag = 0x0;
    reg_addr = 0x110;
    data = 64;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "3rd CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    rw_flag = 0x1;
    function_number = 0x1;
    raw_flag = 0x0;
    reg_addr = 0x0001000c;
    data = (reg2 >> 24) & 0xff;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &data);
    if (SDIO_OK == ret) {
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO,"Timeout waiting for CMD52 completion!\n");
        }
    } else {
        LOG_ERROR(LOG_MOD_SDIO, "4th CMD52 failed!\n");
    }
    sdio_unlock_test(sdio_mutex);

    rw_flag = 0x1;
    function_number = 0x1;
    raw_flag = 0x0;
    reg_addr = 0x0001000b;
    data = (reg2 >> 16) & 0xff;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd52_test(SDIO1, rw_flag, function_number, raw_flag, reg_addr, &data);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 5th CMD52 failed with error: %d\n", ret);
    } else {
        /* Wait for completion with timeout */
        if (xSemaphoreTake(sdio_completion_sem, 5000) != pdPASS) {
            LOG_ERROR(LOG_MOD_SDIO, "Error! Timeout waiting for CMD52 completion\n");
            ret = SDIO_ERROR_TIMEOUT;
        }
    }
    sdio_unlock_test(sdio_mutex);

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed!\n");


    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

    /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed!\n");

    /* Check matches */
    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);

    /* Single Block Transfer Test - Complete */

    /* Successive block transfers - Start */

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed!\n");


    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

    /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed!\n");

    /* Check matches */
    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);

/***********************************Sequence test**************************************/
    cm52_read_write_and_check();

/*********************************** 4 byte R/W **************************************/

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 0;
    reg_addr = 0x0;
    byteCnt = 4;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed!\n");


    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

    /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 0;
    reg_addr = 0x0;
    byteCnt = 4;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed!\n");

    /* Check matches */
    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);
/*********************************** 4 byte R/W **************************************/
    cm52_read_write_and_check();
/*********************************** 64 byte R/W **************************************/

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed!\n");


    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

    /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 64;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed!\n");

    /* Check matches */
    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);

/*********************************** 64 byte R/W **************************************/
    cm52_read_write_and_check();
/*********************************** 4 byte R **************************************/
    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

 /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 0;
    reg_addr = 0x0;
    byteCnt = 4;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed!\n");
/*********************************** 4 byte R **************************************/
/*********************************** 32 byte R **************************************/
    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

 /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 0;
    reg_addr = 0x0;
    byteCnt = 32;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);

    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed1!\n");
/*********************************** 32 byte R **************************************/
/*********************************** 512 byte R **************************************/
    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

 /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 512;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed 512 1!\n");
/*********************************** 512 byte R **************************************/
/*********************************** 4 byte W **************************************/

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 0;
    reg_addr = 0x0;
    byteCnt = 4;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed!\n");
/*********************************** 4 byte W **************************************/
/*********************************** 512 byte W **************************************/

    /* Execute CMD53 Block Write */
    read = 1;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 512;

    /* Filling the buffer with values */
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1 + 4);
    }

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                             global_tx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "CMD53 Write failed with error: %d\n", ret);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Write Completed 512 + 4!\n");
/*********************************** 512 byte W **************************************/
/*********************************** 512 byte R **************************************/
    LOG_INFO(LOG_MOD_SDIO,"Setting the buffer to 0\n");
    memset(global_rx_Buffer, 0, sizeof(global_rx_Buffer));

    vTaskDelayUs(1000);

    /* Execute CMD53 Block Read */
    read = 0;
    block_mode = 1;
    reg_addr = 0x0;
    byteCnt = 512;

    sdio_lock_test(sdio_mutex);
    ret = sdio_sd_cmd53_test(SDIO1, read, funcNum, block_mode, opCode, reg_addr, byteCnt,
                            global_rx_Buffer);
    if (ret != SDIO_OK) {
        LOG_ERROR(LOG_MOD_SDIO, "Error! 2nd CMD53 failed with error: %d\n", ret);
    }
    for (int i = 0; i < sizeof(global_tx_Buffer); i++) {
        global_tx_Buffer[i] = (uint8_t)((i % 0xFF) + 1 + 4);
    }
    sdio_unlock_test(sdio_mutex);
    LOG_INFO(LOG_MOD_SDIO,"Block Read Completed 512 2 + 4!\n");

    /* Check matches */
    sdio_cmd53_write_read_check_test(global_tx_Buffer, global_rx_Buffer, byteCnt);
/*********************************** 512 byte R **************************************/
    cm52_read_write_and_check();
/*********************************** End of Sequence **************************************/
    return;

#endif
    LOG_INFO(LOG_MOD_SDIO,"SDIO Sample App Completed! \n");
    vTaskDelete(NULL);
}


/**
* \brief   Create SDIO sample application task
*/
void task_create()
{
    sdio_sample_app_create();
    sdio_sample_app_main();
}