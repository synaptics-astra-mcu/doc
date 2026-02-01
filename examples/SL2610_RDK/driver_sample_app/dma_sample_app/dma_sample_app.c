/*
* (C) Copyright 2025 Synaptics Incorporated. All rights reserved.
*
* This program is the proprietary software of Synaptics and/or its licensors,
* and may only be used, duplicated, modified or distributed under the authorized
* license from Synaptics.
* ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR
* COPYRIGHT LAW IS PROHIBITED.
*
* \file dma_sample_app.c
*
* \brief This sample application demonstrates the use of DMA
* driver to perform read and write operations.
*/

/*******************************************************************************
*                              Include files
*******************************************************************************/
#include "dma_sample_app.h"
#include "FreeRTOS.h"
#include "logger.h"
#include "dev_common.h"
#include "semphr.h"
#include "cache.h"

/*******************************************************************************
*                              Macro definitions
*******************************************************************************/

#define DMA_PROFILE_START(dmaProfilingVar)  TickType_t dmaProfilingVar = xTaskGetTickCount()
#define DMA_PROFILE_END(dmaProfilingVar, api) \
    do { \
        TickType_t __end = xTaskGetTickCount(); \
        printf("Time for %s = %lu msec\n", api, (__end - dmaProfilingVar)); \
    } while(0)
/*******************************************************************************
*                              Variable definitions
*******************************************************************************/
static SemaphoreHandle_t dma_done_semaphore = NULL;

/* Test data buffers */
static uint32_t dma_src_buf[DMA_TRANSFER_SIZE] SRAM_DATA_ATTRIBUTE = {0};
static uint32_t dma_dest_buf[DMA_TRANSFER_SIZE] SRAM_DATA_ATTRIBUTE = {0};

/* Alternative buffers for linked transfers */
static uint32_t dma_src_buf_2[DMA_TRANSFER_SIZE] SRAM_DATA_ATTRIBUTE = {0};
static uint32_t dma_dest_buf_2[DMA_TRANSFER_SIZE] SRAM_DATA_ATTRIBUTE = {0};

/* 2D transfer buffers */
static uint32_t dma_src_2d_buf[ROWS][COLS] SRAM_DATA_ATTRIBUTE = {0};
static uint32_t dma_dest_2d_buf[ROWS][COLS] SRAM_DATA_ATTRIBUTE = {0};

/* Buffers & descriptors (aligned for DMA) */
__attribute__((aligned(32))) static uint32_t descbuf1
                             [DMA_DESCRIPTOR_BIT_COUNT] SRAM_DATA_ATTRIBUTE;
__attribute__((aligned(32))) static uint32_t descbuf2
                             [DMA_DESCRIPTOR_BIT_COUNT] SRAM_DATA_ATTRIBUTE;
__attribute__((aligned(32))) static uint32_t descbuf3
                             [DMA_DESCRIPTOR_BIT_COUNT] SRAM_DATA_ATTRIBUTE;

static dma_descriptor_handle_t desc1 = descbuf1;
static dma_descriptor_handle_t desc2 = descbuf2;
static dma_descriptor_handle_t desc3 = descbuf3;


/*******************************************************************************
*                              Function definitions
*******************************************************************************/
/**
 * \brief DMA 1D transfer complete callback
 *
 * \details Called when a 1D DMA transfer completes. Can be used for signaling
 *          task completion or further processing.
 *
 * \param instance   DMA instance ID
 * \param channel    DMA channel index
 * \param event      DMA status flags (e.g., DONE, ERROR)
 * \param user_data  Optional user data
 */
static void dma_transfer_callback(dma_instance_en instance, uint32_t channel, uint32_t event,
                                  void *user_data)
{
    if (event & DMA_STATUS_DONE) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        /* Signal that transfer is complete */
        xSemaphoreGiveFromISR(dma_done_semaphore, &xHigherPriorityTaskWoken);
        /* Yield if a higher-priority task was unblocked */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
 * \brief Fill buffer with test pattern
 *
 * This function fills a buffer with a predetermined test pattern
 * for DMA transfer validation.
 *
 * \param buf Buffer to fill
 * \param count Number of 32-bit words in buffer
 */
static void fill_buffer_with_pattern(uint32_t *buf, size_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        /* Use a pattern that makes validation easy but isn't all zeros or ones */
        buf[i] = (i * 0x01010101) + 0xABCDEF00;
    }
}

/**
 * \brief Fill buffer with a specific value
 *
 * \param buf Buffer to fill
 * \param count Number of 32-bit words in buffer
 * \param value Value to fill the buffer with
 */
static void fill_buffer_with_value(uint32_t *buf, size_t count, uint32_t value)
{
    for (uint32_t i = 0; i < count; i++) {
        buf[i] = value;
    }
}

/**
 * \brief Fill 2D buffer with test pattern
 *
 * This function fills a 2D buffer with a predetermined test pattern
 * for DMA transfer validation. Each element in the buffer is assigned
 * a unique value based on its position, ensuring easy validation
 * while avoiding uniform patterns.
 *
 * \param buf 2D buffer to fill
 */
static void fill_buffer_with_pattern_2d(uint32_t buf[ROWS][COLS])
{
    uint32_t val = 0;
    for (uint32_t r = 0; r < ROWS; r++) {
        for (uint32_t c = 0; c < COLS; c++) {
            buf[r][c] = (val * 0x01010101) + 0xABCDEF00;
            val++;
        }
    }
}

/**
 * \brief Fill 2D buffer with a specific value
 *
 * This function fills a 2D buffer with a given value, overwriting any existing
 * data in the buffer.
 *
 * \param buf 2D buffer to fill
 * \param value Value to fill the buffer with
 */
static void fill_buffer_with_value_2d(uint32_t buf[ROWS][COLS], uint32_t value)
{
    for (uint32_t r = 0; r < ROWS; r++) {
        for (uint32_t c = 0; c < COLS; c++) {
            buf[r][c] = value;
        }
    }
}

/**
 * \brief Validate DMA transfer by comparing source and destination buffers
 *
 * This function compares source and destination buffers element by element
 * and reports any mismatches.
 *
 * \param src Source buffer
 * \param dst Destination buffer
 * \param count Number of 32-bit words to compare
 * \return true if buffers match, false otherwise
 */
static bool validate_buffers(const uint32_t *src, const uint32_t *dst, size_t count)
{
    bool valid = true;
    uint32_t mismatch_count = 0;
    for (uint32_t i = 0; i < count; i++) {
        if (src[i] != dst[i]) {
            if (mismatch_count < 5) {
                LOG_ERROR(LOG_MOD_DMA, "[DMA] Data mismatch at %d: src=0x%08x, dst=0x%08x\n",
                        i, src[i], dst[i]);
            }
            mismatch_count++;
            valid = false;
        }
    }

    if (mismatch_count > 5) {
        LOG_ERROR(LOG_MOD_DMA, "[DMA] %d total mismatches found\n", mismatch_count);
    }

    return valid;
}

/**
 * \brief Perform a single 1D DMA transfer
 *
 * \details Initializes DMA, allocates a channel, sets up a 1D memory-to-memory
 *          transfer, and starts it. Completion is handled via interrupt callback.
 *
 * \note Blocking example; waits for transfer completion.
 */
dma_status_en dma_single_transfers_1d_copy(void)
{
    /* Variable Declarations */
    dma_status_en rc = DMA_OK;
    uint32_t status = 0;
    uint32_t timeout = DMA_TIMEOUT_MS();
    uint32_t channel = 0;
    bool valid = false;

    /* Clear destination buffer */
    fill_buffer_with_value(dma_dest_buf, DMA_TRANSFER_SIZE, 0);

    /* Fill source buffer with test pattern */
    fill_buffer_with_pattern(dma_src_buf, DMA_TRANSFER_SIZE);

    /* Request DMA channel */
    dma_channel_attr_t attr = { .is_free = true, .security = DMA_SECURITY_SECURE };
    if ((rc = dma_channel_request(DMA_1, &channel, &attr)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Channel request failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Channel %u allocated successfully.\n", channel);

    /* Configure the DMA channel for 1D transfer */
    dma_channel_config_t config_1d = {
        .mode = DMA_TRANSFER_MODE_1D,
        .config_1d = {
            .x_type           = DMA_OP_CONTINUE,
            .src_addr         = (uint32_t)dma_src_buf,
            .dest_addr        = (uint32_t)dma_dest_buf,
            .transfer_size    = DMA_TRANSFER_SIZE,
            .unit_size        = DMA_UNIT_SIZE_4BYTES,
            .src_burst_size   = DMA_BURST_SIZE_4,
            .dest_burst_size  = DMA_BURST_SIZE_4,
            .src_x_increment  = 1,
            .dest_x_increment = 1,
            .fill_value       = 0,
            .template_config  = { 0 },
            .link_addr        = 0
        },
        .priority        = 0,
        .src_security    = DMA_SECURITY_SECURE,
        .dest_security   = DMA_SECURITY_SECURE,
        .src_privilege   = DMA_PRIVILEGE_PRIVILEGED,
        .dest_privilege  = DMA_PRIVILEGE_PRIVILEGED,
        .src_trigger     = { 0 },
        .dest_trigger    = { 0 },
        .auto_restart.infinite_restart = 0,
        .reg_reload_type = DMA_REG_RELOAD_DISABLED,
        .callback        = dma_transfer_callback,
        .user_data       = NULL
    };

    LOG_INFO(LOG_MOD_DMA, "[DMA] Preparing 1D transfer: %d elements from %p to %p\n",
             DMA_TRANSFER_SIZE, dma_src_buf, dma_dest_buf);

    /* Clean D-Cache for buffers */
    cache_clean_addr((uint32_t *)dma_src_buf, sizeof(dma_src_buf));
    cache_clean_addr((uint32_t *)dma_dest_buf, sizeof(dma_dest_buf));

    /* Prepare channel for transfer */
    if ((rc = dma_prepare_transfer(DMA_1, channel, &config_1d)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer preparation failed.\n");
        return rc;
    }

    /* Enable interrupts */
    if ((rc = dma_enable_int(DMA_1, channel, DMA_INT_DONE_MASK)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Enable interrupts failed.\n");
        return rc;
    }

    DMA_PROFILE_START(dma_start_time);

    /* Start transfer */
    if ((rc = dma_start_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer start failed.\n");
        return rc;
    }

    /* Pause transfer */
    if ((rc = dma_pause_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer pause failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer Paused.\n");

    /* Wait for transfer to pause */
    do {
        if ((rc = dma_get_status(DMA_1, channel, &status)) != DMA_OK) {
            LOG_ERROR(LOG_MOD_DMA, "Failed to get DMA status.\n");
            return rc;
        }
        if (DMA_IS_PAUSED(status)) {
            break;
        }
    } while (--timeout);

    if (timeout == 0) {
        LOG_ERROR(LOG_MOD_DMA, "Timeout waiting for DMA to pause.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    /* Resume transfer */
    if ((rc = dma_resume_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer resume failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer resumed.\n");

    /* Wait for transfer completion */
    if (xSemaphoreTake(dma_done_semaphore, pdMS_TO_TICKS(DMA_TIMEOUT_MS())) != pdTRUE) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer timed out.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    DMA_PROFILE_END(dma_start_time, "DMA 1D Transfer");

    /* Invalidate D-Cache for destination */
    cache_invalidate_addr(CACHE_TYPE_DATA, (uint32_t *)dma_dest_buf, DMA_TRANSFER_SIZE * sizeof(uint32_t));

    /* Validate transfer results */
    valid = validate_buffers(dma_src_buf, dma_dest_buf, DMA_TRANSFER_SIZE);
    if (valid) {
        LOG_INFO(LOG_MOD_DMA, "[DMA] 1D transfer successful: %d elements verified\n",
                 DMA_TRANSFER_SIZE);
        rc = DMA_OK;
    } else {
        LOG_ERROR(LOG_MOD_DMA, "[DMA] 1D transfer validation failed\n");
        rc = DMA_ERROR_TRANSFER_FAIL;
    }

    /* Stop transfer */
    if ((rc = dma_stop_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer stop failed.\n");
        return rc;
    }

    /* Free channel */
    if ((rc = dma_free_channel(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Channel free failed.\n");
        return rc;
    }

    return rc;
}

/**
 * \brief Perform a single 2D DMA transfer
 *
 * \details Initializes DMA, allocates a channel, configures it for a 2D
 *          matrix copy, and starts the transfer. Completion is handled
 *          via callback.
 *
 * \note ROWS and COLS must define valid matrix dimensions.
 */
dma_status_en dma_single_transfers_2d_copy(void)
{
    /* Variable Declarations */
    dma_status_en rc = DMA_OK;
    uint32_t status = 0;
    uint32_t timeout = DMA_TIMEOUT_MS();
    uint32_t channel;

    /* Clear destination buffer */
    fill_buffer_with_value_2d(dma_dest_2d_buf, 0);

    /* Fill source buffer with test pattern */
    fill_buffer_with_pattern_2d(dma_src_2d_buf);

    /* Request DMA channel */
    dma_channel_attr_t attr = { .is_free = true, .security = DMA_SECURITY_SECURE };
    if ((rc = dma_channel_request(DMA_1, &channel, &attr)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Channel request failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Channel %u allocated successfully.\n", channel);

    /* Clean D-Cache for source and destination buffers */
    cache_clean_addr((uint32_t *)dma_src_2d_buf, sizeof(dma_src_2d_buf));
    cache_clean_addr((uint32_t *)dma_dest_2d_buf, sizeof(dma_dest_2d_buf));

    /* Configure DMA channel for 2D transfer */
    dma_channel_config_t config_2d = {
        .mode = DMA_TRANSFER_MODE_2D,
        .config_2d = {
            .x_type           = DMA_OP_CONTINUE,
            .y_type           = DMA_OP_CONTINUE,
            .src_addr         = (uint32_t)dma_src_2d_buf,
            .dest_addr        = (uint32_t)dma_dest_2d_buf,
            .src_x_size       = COLS,
            .dest_x_size      = COLS,
            .src_y_size       = ROWS,
            .dest_y_size      = ROWS,
            .unit_size        = DMA_UNIT_SIZE_4BYTES,
            .src_burst_size   = DMA_BURST_SIZE_4,
            .dest_burst_size  = DMA_BURST_SIZE_4,
            .src_x_increment  = 1,
            .dest_x_increment = 1,
            .src_y_stride     = COLS,
            .dest_y_stride    = COLS,
            .fill_value       = 0,
            .link_addr        = 0
        },
        .priority           = 0,
        .src_security       = DMA_SECURITY_SECURE,
        .dest_security      = DMA_SECURITY_SECURE,
        .src_privilege      = DMA_PRIVILEGE_PRIVILEGED,
        .dest_privilege     = DMA_PRIVILEGE_PRIVILEGED,
        .src_trigger        = { 0 },
        .dest_trigger       = { 0 },
        .auto_restart.infinite_restart = 0,
        .reg_reload_type    = DMA_REG_RELOAD_DISABLED,
        .callback           = dma_transfer_callback,
        .user_data          = NULL
    };

    /* Prepare channel for transfer */
    if ((rc = dma_prepare_transfer(DMA_1, channel, &config_2d)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer preparation failed.\n");
        return rc;
    }

    /* Enable interrupts */
    if ((rc = dma_enable_int(DMA_1, channel, DMA_INT_DONE_MASK)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Enabling interrupts failed.\n");
        return rc;
    }

    DMA_PROFILE_START(dma_start_time);

    /* Start transfer */
    if ((rc = dma_start_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer start failed.\n");
        return rc;
    }

    /* Pause transfer */
    if ((rc = dma_pause_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer pause failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer Paused.\n");

    /* Wait for transfer to pause with timeout */
    do {
        if ((rc = dma_get_status(DMA_1, channel, &status)) != DMA_OK) {
            LOG_ERROR(LOG_MOD_DMA, "Failed to get DMA status.\n");
            return rc;
        }
        if (DMA_IS_PAUSED(status)) {
            break;
        }
    } while (--timeout);

    if (timeout == 0) {
        LOG_ERROR(LOG_MOD_DMA, "Timeout waiting for DMA to pause.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    /* Resume transfer */
    if ((rc = dma_resume_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer resume failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer resumed.\n");

    DMA_PROFILE_END(dma_start_time, "DMA 2D Transfer");

    /* Wait for completion */
    if (xSemaphoreTake(dma_done_semaphore, pdMS_TO_TICKS(DMA_TIMEOUT_MS())) != pdTRUE) {
        LOG_ERROR(LOG_MOD_DMA, "2D transfer timed out.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    /* Invalidate D-Cache for destination buffer */
    cache_invalidate_addr(CACHE_TYPE_DATA, (uint32_t *)dma_dest_2d_buf, sizeof(dma_dest_2d_buf));

    /* Validate */
    bool valid = validate_buffers((uint32_t *)dma_src_2d_buf,
                                  (uint32_t *)dma_dest_2d_buf,
                                  ROWS * COLS);

    if (valid) {
        LOG_INFO(LOG_MOD_DMA, "[DMA] 2D transfer successful: %dx%d elements verified\n",
                 ROWS, COLS);
        rc = DMA_OK;
    } else {
        LOG_ERROR(LOG_MOD_DMA, "[DMA] 2D transfer validation failed\n");
        rc = DMA_ERROR_TRANSFER_FAIL;
    }

    /* Stop transfer */
    if ((rc = dma_stop_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Stopping transfer failed.\n");
        return rc;
    }

    /* Free channel */
    if ((rc = dma_free_channel(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Freeing DMA channel failed.\n");
        return rc;
    }

    return rc;
}

/**
 * \brief Perform linked DMA transfers
 *
 * \details Initializes DMA, allocates a channel, creates three descriptors
 *          (1D → 2D → 1D), links them, and starts the chain. Completion is
 *          validated via callback.
 *
 * \note Descriptors must meet hardware alignment requirements.
 */
dma_status_en dma_linked_transfers(void)
{
    /* Variable Declarations */
    dma_status_en rc = DMA_OK;
    uint32_t channel;
    uint32_t status = 0;
    uint32_t timeout = DMA_TIMEOUT_MS();

    /* Fill buffers */
    fill_buffer_with_pattern(dma_src_buf, DMA_TRANSFER_SIZE);
    fill_buffer_with_value(dma_dest_buf, DMA_TRANSFER_SIZE, 0);
    fill_buffer_with_pattern(dma_src_buf_2, DMA_TRANSFER_SIZE);
    fill_buffer_with_value(dma_dest_buf_2, DMA_TRANSFER_SIZE, 0);

    /* Clean D-cache for all sources and destinations */
    cache_clean_addr((uint32_t *)dma_src_buf, sizeof(dma_src_buf));
    cache_clean_addr((uint32_t *)dma_src_buf_2, sizeof(dma_src_buf_2));
    cache_clean_addr((uint32_t *)dma_dest_buf, sizeof(dma_dest_buf));
    cache_clean_addr((uint32_t *)dma_dest_buf_2, sizeof(dma_dest_buf_2));

    /* Request DMA channel */
    dma_channel_attr_t attr = { .is_free = true, .security = DMA_SECURITY_SECURE };
    if ((rc = dma_channel_request(DMA_1, &channel, &attr)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Channel request failed.\n");
        return rc;
    }

    /* Configure descriptors */
    dma_channel_config_t config_1d = {
        .mode = DMA_TRANSFER_MODE_1D,
        .config_1d = {
            .x_type           = DMA_OP_CONTINUE,
            .src_addr         = (uint32_t)dma_src_buf,
            .dest_addr        = (uint32_t)dma_dest_buf,
            .transfer_size    = DMA_TRANSFER_SIZE,
            .unit_size        = DMA_UNIT_SIZE_4BYTES,
            .src_burst_size   = DMA_BURST_SIZE_4,
            .dest_burst_size  = DMA_BURST_SIZE_4,
            .src_x_increment  = 1,
            .dest_x_increment = 1,
            .fill_value       = 0,
            .template_config  = { 0 },
            .link_addr        = 0
        },
        .priority           = 0,
        .src_security       = DMA_SECURITY_SECURE,
        .dest_security      = DMA_SECURITY_SECURE,
        .src_privilege      = DMA_PRIVILEGE_PRIVILEGED,
        .dest_privilege     = DMA_PRIVILEGE_PRIVILEGED,
        .src_trigger        = { 0 },
        .dest_trigger       = { 0 },
        .auto_restart.infinite_restart = 0,
        .reg_reload_type    = DMA_REG_RELOAD_DISABLED,
        .callback           = NULL,
        .user_data          = NULL
    };

    dma_channel_config_t config_1d_2 = {
        .mode = DMA_TRANSFER_MODE_1D,
        .config_1d = {
            .x_type           = DMA_OP_CONTINUE,
            .src_addr         = (uint32_t)dma_src_buf_2,
            .dest_addr        = (uint32_t)dma_dest_buf_2,
            .transfer_size    = DMA_TRANSFER_SIZE,
            .unit_size        = DMA_UNIT_SIZE_4BYTES,
            .src_burst_size   = DMA_BURST_SIZE_4,
            .dest_burst_size  = DMA_BURST_SIZE_4,
            .src_x_increment  = 1,
            .dest_x_increment = 1,
            .fill_value       = 0,
            .template_config  = { 0 },
            .link_addr        = 0
        },
        .priority           = 0,
        .src_security       = DMA_SECURITY_SECURE,
        .dest_security      = DMA_SECURITY_SECURE,
        .src_privilege      = DMA_PRIVILEGE_PRIVILEGED,
        .dest_privilege     = DMA_PRIVILEGE_PRIVILEGED,
        .src_trigger        = { 0 },
        .dest_trigger       = { 0 },
        .auto_restart.infinite_restart = 0,
        .reg_reload_type    = DMA_REG_RELOAD_DISABLED,
        .callback           = dma_transfer_callback,
        .user_data          = NULL
    };

    /* Create descriptors */
    if ((rc = dma_create_descriptor(DMA_1, channel, &config_1d, &desc1)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Descriptor 1 creation failed.\n");
        return rc;
    }

    if ((rc = dma_create_descriptor(DMA_1, channel, &config_1d_2, &desc3)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Descriptor 3 creation failed.\n");
        return rc;
    }

    /* Link descriptors */
    if ((rc = dma_link_descriptors(DMA_1, desc1, desc3)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to link descriptor 1 -> 2.\n");
        return rc;
    }

    /* Enable interrupts */
    if ((rc = dma_enable_int(DMA_1, channel, DMA_INT_DONE_MASK)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to enable interrupts.\n");
        return rc;
    }

    DMA_PROFILE_START(dma_start_time);

    /* Start chain */
    if ((rc = dma_start_descriptor_chain(DMA_1, channel, &desc1)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to start descriptor chain.\n");
        return rc;
    }

    /* Pause transfer */
    if ((rc = dma_pause_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer pause failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer Paused.\n");

    /* Wait for pause with timeout */
    do {
        if ((rc = dma_get_status(DMA_1, channel, &status)) != DMA_OK) {
            LOG_ERROR(LOG_MOD_DMA, "Failed to get DMA status.\n");
            return rc;
        }
        if (DMA_IS_PAUSED(status)) {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    } while (--timeout);

    if (timeout == 0) {
        LOG_ERROR(LOG_MOD_DMA, "Timeout waiting for DMA to pause.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    /* Resume transfer */
    if ((rc = dma_resume_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer resume failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Transfer resumed.\n");

    /* Wait for completion */
    if (xSemaphoreTake(dma_done_semaphore, pdMS_TO_TICKS(DMA_TIMEOUT_MS())) != pdTRUE) {
        LOG_ERROR(LOG_MOD_DMA, "Linked transfer timed out.\n");
        return DMA_ERROR_TRANSFER_FAIL;
    }

    DMA_PROFILE_END(dma_start_time, "DMA Linked Transfer");

    /* Invalidate destination buffers */
    cache_invalidate_addr(CACHE_TYPE_DATA, (uint32_t *)dma_dest_buf, sizeof(dma_dest_buf));
    cache_invalidate_addr(CACHE_TYPE_DATA, (uint32_t *)dma_dest_buf_2, sizeof(dma_dest_buf_2));

    /* Validate all transfers */
    bool valid1 = validate_buffers(dma_src_buf, dma_dest_buf, DMA_TRANSFER_SIZE);
    bool valid3 = validate_buffers(dma_src_buf_2, dma_dest_buf_2, DMA_TRANSFER_SIZE);

    if (!valid1 || !valid3) {
        LOG_ERROR(LOG_MOD_DMA, "Linked transfers validation failed.\n");
        rc = DMA_ERROR_TRANSFER_FAIL;
    } else {
        LOG_INFO(LOG_MOD_DMA, "Linked transfers verified successfully.\n");
        rc = DMA_OK;
    }

    /* Stop/free */
    if ((rc = dma_stop_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Stopping transfer failed.\n");
        return rc;
    }
    if ((rc = dma_free_descriptor(DMA_1, desc1)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Freeing descriptor 1 failed.\n");
        return rc;
    }

    if ((rc = dma_free_descriptor(DMA_1, desc3)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Freeing descriptor 3 failed.\n");
        return rc;
    }
    if ((rc = dma_free_channel(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Freeing channel failed.\n");
        return rc;
    }

    return rc;
}

/**
 * \brief Perform DMA transfer using software triggers
 *
 * \details Initializes DMA, allocates a channel, sets up a 1D transfer, and
 *          triggers each element manually using software triggers in FLOW mode.
 *          Validates data after completion.
 *
 * \note Uses dma_sw_trig() inside a loop for each transfer step.
 */
dma_status_en dma_sw_trig_transfers(void)
{
    /* Variable Declarations */
    dma_status_en rc = DMA_OK;
    uint32_t channel;
    uint32_t status = 0;
    int i;

    /* Fill and clear buffers */
    fill_buffer_with_pattern(dma_src_buf, DMA_TRANSFER_SIZE);
    fill_buffer_with_value(dma_dest_buf, DMA_TRANSFER_SIZE, 0);

    /* Clean D-cache for source and destination */
    cache_clean_addr((uint32_t *)dma_src_buf, sizeof(dma_src_buf));
    cache_clean_addr((uint32_t *)dma_dest_buf, sizeof(dma_dest_buf));

    /* Request DMA channel */
    dma_channel_attr_t attr = { .is_free = true, .security = DMA_SECURITY_SECURE };
    if ((rc = dma_channel_request(DMA_1, &channel, &attr)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Channel request failed.\n");
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Channel %u allocated successfully.\n", channel);

    /* Configure channel for 1D transfer */
    dma_channel_config_t config_1d = {
        .mode = DMA_TRANSFER_MODE_1D,
        .config_1d = {
            .x_type           = DMA_OP_CONTINUE,
            .src_addr         = (uint32_t)dma_src_buf,
            .dest_addr        = (uint32_t)dma_dest_buf,
            .transfer_size    = DMA_TRANSFER_SIZE,
            .unit_size        = DMA_UNIT_SIZE_4BYTES,
            .src_burst_size   = DMA_BURST_SIZE_4,
            .dest_burst_size  = DMA_BURST_SIZE_4,
            .src_x_increment  = 1,
            .dest_x_increment = 1,
            .fill_value       = 0,
            .template_config  = { 0 },
            .link_addr        = 0
        },
        .priority           = 0,
        .src_security       = DMA_SECURITY_SECURE,
        .dest_security      = DMA_SECURITY_SECURE,
        .src_privilege      = DMA_PRIVILEGE_PRIVILEGED,
        .dest_privilege     = DMA_PRIVILEGE_PRIVILEGED,
        .src_trigger        = { 0 },
        .dest_trigger       = { 0 },
        .auto_restart.infinite_restart = 0,
        .reg_reload_type    = DMA_REG_RELOAD_DISABLED,
        .callback           = NULL,
        .user_data          = NULL
    };

    /* Configure SW trigger */
    dma_sw_trigger_config_t trig_cfg = {
        .trig_type  = DMA_TRIG_SW_REQ,
        .sw_trig_type = DMA_SW_TRIG_SINGLE,
        .mode       = DMA_TRIG_MODE_FLOW,
        .block_size = 0
    };

    /* Prepare channel for transfer */
    if ((rc = dma_prepare_transfer(DMA_1, channel, &config_1d)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer preparation failed.\n");
        dma_free_channel(DMA_1, channel);
        return rc;
    }

    /* Configure SW trigger */
    if ((rc = dma_sw_trig_config(DMA_1, channel, &trig_cfg)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "SW trigger configuration failed.\n");
        dma_free_channel(DMA_1, channel);
        return rc;
    }

    DMA_PROFILE_START(dma_start_time);

    /* Start transfer */
    if ((rc = dma_start_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Transfer start failed.\n");
        dma_free_channel(DMA_1, channel);
        return rc;
    }

    /* Manually trigger each element transfer */
    for (i = 0; i < DMA_TRANSFER_SIZE; i++) {
        if ((rc = dma_sw_trig(DMA_1, channel, DMA_TRIGGER_SOURCE)) != DMA_OK) {
            LOG_ERROR(LOG_MOD_DMA, "SW trigger failed at index %d.\n", i);
            dma_free_channel(DMA_1, channel);
            return rc;
        }
    }

    /* Get final transfer status */
    if ((rc = dma_get_status(DMA_1, channel, &status)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to get DMA status.\n");
        dma_free_channel(DMA_1, channel);
        return rc;
    }
    LOG_INFO(LOG_MOD_DMA, "Final status = 0x%x\n", status);

    DMA_PROFILE_END(dma_start_time, "DMA SW Triggered Transfer");

    if (DMA_IS_DONE(status)) {
        LOG_INFO(LOG_MOD_DMA, "Transfer completed. Verifying data...\n");

        /* Invalidate D-cache for destination buffer */
        cache_invalidate_addr(CACHE_TYPE_DATA, (uint32_t *)dma_dest_buf, sizeof(dma_dest_buf));

        if (validate_buffers(dma_src_buf, dma_dest_buf, DMA_TRANSFER_SIZE)) {
            LOG_INFO(LOG_MOD_DMA, "SW-triggered DMA transfer successful!\n");
            rc = DMA_OK;
        } else {
            LOG_ERROR(LOG_MOD_DMA, "SW-triggered DMA transfer failed: Data mismatch.\n");
            rc = DMA_ERROR_TRANSFER_FAIL;
        }
    } else {
        LOG_ERROR(LOG_MOD_DMA, "DMA transfer did not complete.\n");
        rc = DMA_ERROR_TRANSFER_FAIL;
    }

    /* Stop and free DMA channel */
    if ((rc = dma_stop_transfer(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to stop DMA transfer.\n");
    }
    if ((rc = dma_free_channel(DMA_1, channel)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to free DMA channel.\n");
    }

    return rc;
}

/**
 * \brief Executes all DMA transfer sample operations
 *
 * \details Runs the following sample operations in sequence:
 *          - 1D DMA transfer
 *          - 2D DMA transfer
 *          - Linked DMA transfer
 *          - Software triggered DMA transfer
 *
 *\param channel DMA channel to use.
 *
 * \return none
 */
void dma_all_transfers() {
    /* Variable Declarations */
    dma_status_en rc = DMA_OK;
    uint32_t err_count = 0;

    if ((rc = dma_single_transfers_1d_copy()) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "1D DMA transfer failed.\n");
        err_count++;
    }

    if ((rc = dma_linked_transfers()) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Linked DMA transfer failed.\n");
        err_count++;
    }

    if ((rc = dma_sw_trig_transfers()) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Software triggered DMA transfer failed.\n");
        err_count++;
    }

    if (err_count == 0) {
        LOG_INFO(LOG_MOD_DMA, "All DMA transfers successful.\n");
    }
}

/**
 * \brief Execute selected DMA sample application
 *
 * \details Runs DMA test cases based on compile-time feature macros
 *
 *
 * \return void
 */
void dma_sample_app_main(void)
{
    LOG_INFO(LOG_MOD_DMA, "Starting DMA Sample Application...\n");
    dma_status_en rc = DMA_OK;

    /* Create a semaphore */
    dma_done_semaphore = xSemaphoreCreateBinary();
    if (dma_done_semaphore == NULL) {
        LOG_ERROR(LOG_MOD_DMA, "Failed to create semaphore.\n");
    }

    /* Initialize DMA controller */
    if ((rc = dma_init(DMA_1)) != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Init failed.\n");
    }

#if DMA_TRANSFER_1D_COPY
    LOG_INFO(LOG_MOD_DMA, "Running 1D DMA Transfer Test...\n");
    rc = dma_single_transfers_1d_copy();
    if (rc != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "1D transfer test failed.\n");
    }
#endif

#if DMA_TRANSFER_LINKED
    LOG_INFO(LOG_MOD_DMA, "Running Linked DMA Transfer Test...\n");
    rc = dma_linked_transfers();
    if (rc != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "Linked transfer test failed.\n");
    }
#endif

#if DMA_TRANSFER_SW_TRIG
    LOG_INFO(LOG_MOD_DMA, "Running SW-Triggered DMA Transfer Test...\n");
    rc = dma_sw_trig_transfers();
    if (rc != DMA_OK) {
        LOG_ERROR(LOG_MOD_DMA, "SW-triggered transfer test failed.\n");
    }
#endif

#if DMA_TRANSFER_ALL
    LOG_INFO(LOG_MOD_DMA, "Running All DMA Transfer Tests...\n");
    dma_all_transfers();
#endif

    dma_done_semaphore = NULL;
    LOG_INFO(LOG_MOD_DMA, "DMA Sample Application Completed.\n");
    vTaskDelete(NULL);
}

