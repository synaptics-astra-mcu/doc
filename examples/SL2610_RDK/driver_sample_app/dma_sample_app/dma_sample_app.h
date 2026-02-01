/*
* (C) Copyright 2025 Synaptics Incorporated. All rights reserved.
*
* This program is the proprietary software of Synaptics and/or its licensors,
* and may only be used, duplicated, modified or distributed under the authorized
* license from Synaptics.
* ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR
* COPYRIGHT LAW IS PROHIBITED.
*
* \file dma_sample_app.h
*
* \brief This sample application demonstrates the use of DMAs
* driver to perform read and write operations.
*/

#ifndef _DMA_SAMPLE_APP_H_
#define _DMA_SAMPLE_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*                              Include files
*******************************************************************************/
#include "syna_device.h"
#include "dma.h"
/*******************************************************************************
*                              Macro definitions
*******************************************************************************/
// #define DMA_TRANSFER_SIZE           65536
#define DMA_TRANSFER_SIZE           64

#define DMA_TIMEOUT_MS() ((DMA_TRANSFER_SIZE) * 100)

#define ROWS                        8
#define COLS                        8

#define DMA_TRANSFER_1D_COPY        0   /* Enable 1D DMA transfer test */
#define DMA_TRANSFER_LINKED         0   /* Enable linked descriptor test */
#define DMA_TRANSFER_SW_TRIG        0   /* Enable SW-triggered transfer test */
#define DMA_TRANSFER_ALL            1   /* Enable All transfers at once */

/* Compile-time check: Only one test should be enabled */
#if (DMA_TRANSFER_1D_COPY  + DMA_TRANSFER_LINKED + \
    DMA_TRANSFER_SW_TRIG  + DMA_TRANSFER_ALL) != 1
#error "Exactly one DMA transfer test must be enabled (set to 1)."
#endif

/*******************************************************************************
*                              Function declarations
*******************************************************************************/

/*******************************************************************************
* Function Name: dma_sample_app_start
********************************************************************************
*
* Initializes and starts the DMA sample application. This function sets up
* the necessary resources and prepares the application for execution.
*
* \return void
*
*******************************************************************************/
void dma_sample_app_start(void);

/*******************************************************************************
* Function Name: dma_sample_app_main
********************************************************************************
*
* The main function of the DMA sample application. This function contains
* the core logic and runs the sample operations using the DMA interface.
*
* \return void
*
*******************************************************************************/
void dma_sample_app_main(void);

/*******************************************************************************
* Function Name: dma_single_transfers_1d_copy
********************************************************************************
*
* Executes a single 1D DMA transfer.
*
* This function initializes the DMA controller, allocates a channel, and
* performs a one-dimensional transfer from `dma_src_buf` to `dma_dest_buf`.
* After transfer completion, data integrity is validated.
*
* \return dma_status_en
*
*******************************************************************************/
dma_status_en dma_single_transfers_1d_copy();

/*******************************************************************************
* Function Name: dma_single_transfers_2d_copy
********************************************************************************
*
* Executes a single 2D DMA transfer.
*
* This function initializes the DMA controller, allocates a channel, and
* performs a two-dimensional transfer from `dma_src_2d_buf` to
* `dma_dest_2d_buf`. After transfer completion, data integrity is validated.
*
* \return dma_status_en
*
*******************************************************************************/
dma_status_en dma_single_transfers_2d_copy();

/*******************************************************************************
* Function Name: dma_linked_transfers
********************************************************************************
*
* Executes a linked DMA transfer chain.
*
* This function initializes the DMA controller, allocates a channel, and
* prepares multiple descriptors for 1D and 2D transfers. The descriptors
* are linked together to form a chain that executes sequentially.
* After completion, all buffers are validated for correctness.
*
* \return dma_status_en
*
*******************************************************************************/
dma_status_en dma_linked_transfers();

/*******************************************************************************
* Function Name: dma_sw_trig_transfers
********************************************************************************
*
* Executes a DMA transfer using software triggers.
*
* This function initializes the DMA controller, allocates a channel, and
* configures a 1D transfer in FLOW mode with software triggers. Each element
* transfer is triggered manually in a loop. At the end, data integrity is
* verified.
*
* \return dma_status_en
*
*******************************************************************************/
dma_status_en dma_sw_trig_transfers();

/*******************************************************************************
* Function Name: dma_all_transfers
********************************************************************************
*
* Performs all transfer modes.
*
* Executes a multi-mode DMA transfer sequence.
*
* This function sets up and initiates a sequence of DMA operations that
* includes 1D, 2D, and linked transfers. Each transfer mode is executed
* in succession to demonstrate the flexibility of the DMA controller.
* Data integrity is checked after each transfer.
*
* \param channel DMA channel to use.
*
* \return none
*
*******************************************************************************/
void dma_all_transfers();

#ifdef __cplusplus
}
#endif

#endif  /* _DMA_SAMPLE_APP_H_ */
