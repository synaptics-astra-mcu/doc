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
 * \file	spwm_sample_app.c
 *
 * \brief	SPWM Driver sample application
 */

/*******************************************************************************
*                              Include files
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "spwm_sample_app.h"
#include <math.h>

/*******************************************************************************
*                              Macro definitions
*******************************************************************************/
#define SPWM_PROFILE_START(spwmProfilingVar)  TickType_t spwmProfilingVar = xTaskGetTickCount()
#define SPWM_PROFILE_END(spwmProfilingVar, api) \
    do { \
        TickType_t __end = xTaskGetTickCount(); \
        LOG_INFO(LOG_MOD_SPWM, "Time for %s = %lu msec\r\n", api, (__end - spwmProfilingVar)); \
    } while(0)

/*******************************************************************************
*                            Type Definitions
*******************************************************************************/
/* Runtime context per SPWM group */
typedef struct {
    SemaphoreHandle_t sem;
    volatile uint32_t irq_mask;
} spwm_group_ctx_t;

/* LFSR config for PWM Pseudo-Random mode */
spwm_pwmpr_cfg_t pr_cfg = {
    .lfsr_seed = 0x5997,
    .lfsr_tap  = 0x1D00
};

/*******************************************************************************
*                              Variable definitions
*******************************************************************************/
static spwm_group_ctx_t spwm_group_ctx[SPWM_MAX_GROUPS] = { 0 };

static volatile uint32_t spwm_timer_last_counter = 0;
static volatile uint32_t spwm_timer_irq_mask    = 0;

/* Example groups array for user_ctx: first entry = count, then group ids */
static uint8_t active_groups_arr[SPWM_MAX_GROUPS];

/*******************************************************************************
*                              Function Definitions
*******************************************************************************/
/**
 * \brief Ensure semaphore exists for a SPWM group.
 *
 * Creates a binary semaphore for the specified group if it does not
 * already exist and resets the stored interrupt mask.
 *
 * \param group_id SPWM group index.
 *
 * \return 0 on success, -1 on error.
 */
static inline int spwm_group_ensure_sem(uint8_t group_id)
{
    if (group_id >= SPWM_MAX_GROUPS)
    {
        LOG_ERROR(LOG_MOD_SPWM, "Invalid group id\r\n");
        return -1;
    }

    if (spwm_group_ctx[group_id].sem == NULL) {
        spwm_group_ctx[group_id].sem = xSemaphoreCreateBinary();
        if (spwm_group_ctx[group_id].sem == NULL) {
            LOG_ERROR(LOG_MOD_SPWM, "Failed to create semaphore for group %u\r\n", group_id);
            return -1;
        }
    }
    spwm_group_ctx[group_id].irq_mask = 0;
    return 0;
}

/**
 * \brief Unified SPWM interrupt callback.
 *
 * Handles interrupts for one or more SPWM groups. The user context
 * provides a list of active group IDs. For each group, the callback
 * reads and clears the interrupt status and signals the group semaphore.
 *
 * \param id         SPWM instance.
 * \param irq_mask   Combined NVIC interrupt mask (unused).
 * \param user_ctx   Pointer to an array: [count, group1, group2, ...].
 */
static void spwm_irq_callback(spwm_instance_en id, uint32_t irq_mask,
                             void *user_ctx)
{
    LOG_INFO(LOG_MOD_SPWM, "[ISR] Callback triggered!\r\n");
    (void)irq_mask;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    uint8_t *groups = (uint8_t *)user_ctx;
    uint8_t num_groups = groups[0];

    for (uint8_t i = 1; i <= num_groups; i++)
    {
        uint8_t group_id = groups[i];
        uint32_t st = 0;

        /* Get effective interrupt status for this group */
        if (spwm_get_interrupt_status(id, group_id, &st) != SPWM_OK) {
            continue;
        }

        if (st == 0) {
            continue;
        }

        /* Record & log */
        spwm_group_ctx[group_id].irq_mask |= st;

        if (st & (1U << SPWM_INTR_CC0_MATCH)) {
            LOG_INFO(LOG_MOD_SPWM, "[ISR] Group %u: CC0 match\r\n", group_id);
        }

        if (st & (1U << SPWM_INTR_CC1_MATCH)) {
            LOG_INFO(LOG_MOD_SPWM, "[ISR] Group %u: CC1 match\r\n", group_id);
        }

        if (st & (1U << SPWM_INTR_TC)) {
            LOG_INFO(LOG_MOD_SPWM, "[ISR] Group %u: TC\r\n", group_id);
        }

        spwm_disable(id, group_id);
        /* Clear ONLY the bits that actually fired */
        spwm_clear_interrupt(id, group_id, st);

        /* Wake that group’s semaphore */
        SemaphoreHandle_t sem = spwm_group_ctx[group_id].sem;
        if (sem != NULL)
        {
            if (xPortIsInsideInterrupt())
            {
                xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            } else {
                xSemaphoreGive(sem);
            }
        }
    }
}

/**
 * \brief Descriptor completion ISR callback.
 *
 * Invoked when a descriptor sequence finishes. Disables the
 * corresponding SPWM group and signals the waiting task.
 *
 * \param id      SPWM instance.
 * \param desc_id Descriptor index that completed.
 * \param param   User context containing SPWM group ID.
 */
static void spwm_desc_done_isr(spwm_instance_en id, uint8_t desc_id, void *param)
{
    uint8_t group_id = (uint8_t)(uintptr_t)param;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    LOG_INFO(LOG_MOD_SPWM, "Descriptor %u completed\r\n", desc_id);

    /* Disable PWM counter */
    spwm_disable(id, group_id);

    /* Release FreeRTOS semaphore */
    SemaphoreHandle_t sem = spwm_group_ctx[group_id].sem;
    if (sem != NULL)
    {
        if (xPortIsInsideInterrupt())
        {
            xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        } else {
            xSemaphoreGive(sem);
        }
    }
}

/**
 * \brief Configure and enable a SPWM clock divider.
 *
 * This API disables the divider (recommended), programs the integer and
 * fractional divider values, and enables it again.
 *
 * \param id          SPWM instance
 * \param div_id      Divider index
 * \param int_div     Integer divider
 * \param frac_div    Fractional divider
 *
 * \return 0 on success, -1 on error
 */
int32_t spwm_sample_app_configure_clock(spwm_instance_en id, uint8_t div_id, uint16_t int_div,
                                        uint8_t frac_div)
{
    LOG_INFO(LOG_MOD_SPWM, "CLKDIV[%u] Setup: int_div=%u frac_div=%u\r\n",
             div_id, int_div, frac_div);

    if (spwm_clkdiv_enable(id, div_id, false, 0) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "CLKDIV disable failed\r\n");
        return -1;
    }

    if (spwm_clkdiv_config(id, div_id, int_div, frac_div) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "CLKDIV config failed\r\n");
        return -1;
    }

    if (spwm_clkdiv_enable(id, div_id, true, 0) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "CLKDIV enable failed\r\n");
        return -1;
    }

    return 0;
}

/**
 * \brief Configure SPWM interrupts for a group.
 *
 * Registers a single instance-level ISR callback and enables the
 * specified interrupt sources for the given SPWM group.
 *
 * \param id       SPWM instance.
 * \param group_id SPWM group index.
 * \param intr     Group-local interrupt mask (SPWM_INTR_* bits).
 * \param cb       ISR callback function.
 * \param ctx      User context pointer passed to the ISR callback.
 *
 * \return 0 on success, -1 on error.
 */
int32_t spwm_sample_app_configure_interrupts(spwm_instance_en id, uint8_t group_id,
                                             spwm_intr_en intr, spwm_isr_cb_t cb, void *ctx)
{
    LOG_INFO(LOG_MOD_SPWM, "INTERRUPT Setup: group=%u intr=%u\r\n", group_id, intr);

    /* Register single instance-level callback (user_ctx will be used by ISR) */
    if (spwm_register_callback(id, cb, ctx) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_register_callback failed\r\n");
        return -1;
    }

    spwm_disable_all_interrupts(id, group_id);

    /* Clear any pending interrupts for that group */
    if (spwm_clear_all_interrupts(id, group_id) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Interrupt clear failed for group %u\r\n", group_id);
        return -1;
    }

    /* Enable requested interrupt mask for that group */
    if (spwm_enable_interrupt(id, group_id, intr) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Interrupt enable failed for group %u\r\n", group_id);
        return -1;
    }

    return 0;
}

/**
 * \brief Prepare SPWM TIMER-mode configuration.
 *
 * Initializes the configuration structure for basic up-counting
 * timer operation with CC0 compare and auto-reload enabled.
 *
 * \param cfg    Pointer to SPWM configuration structure.
 */
void spwm_sample_app_prepare_timer_cfg(spwm_config_t *cfg)
{
    if (!cfg) {
        return;
    }

    *cfg = (spwm_config_t){
        .mode        = SPWM_MODE_TIMER,
        .clkdiv_sel  = 0,
        .count_mode  = SPWM_COUNT_UP,
        .prescale    = SPWM_PRESCALE_DIV1,
        .one_shot    = false,
        .period      = TIMER_PERIOD,
        .cc0         = TIMER_CC0,
        .cc0_buff    = TIMER_CC0,
        .cfg.timer = {
            .auto_reload_cc0 = true,
            .auto_reload_cc1 = false,
        },
    };
}

/**
 * \brief Prepare SPWM CAPTURE-mode configuration.
 *
 * Initializes the configuration structure for free-running
 * counter operation with capture enabled.
 *
 * \param cfg Pointer to SPWM configuration structure.
 */
void spwm_sample_app_prepare_capture_cfg(spwm_config_t *cfg)
{
    if (!cfg) {
        return;
    }

    *cfg = (spwm_config_t){
        .mode        = SPWM_MODE_CAPTURE,
        .clkdiv_sel  = 0,
        .count_mode  = SPWM_COUNT_UP,
        .prescale    = SPWM_PRESCALE_DIV1,
        .one_shot    = false,
        .period      = CAPTURE_PERIOD,
    };
}

/**
 * \brief       Prepares SPWM PWM-mode configuration.
 *
 * \details     Initializes the SPWM configuration structure for PWM mode
 *              operation using predefined period and duty-cycle parameters.
 *
 * \param cfg   Pointer to SPWM configuration structure.
 */
void spwm_sample_app_prepare_pwm_cfg(spwm_config_t *cfg)
{
    if (!cfg) {
        return;
    }

    *cfg = (spwm_config_t){
        .mode        = SPWM_MODE_PWM,
        .prescale    = SPWM_PRESCALE_DIV1,
        .count_mode  = SPWM_COUNT_UP,
        .period      = PWM_PERIOD,
        .cc0         = PWM_CC0,
        .period_buff = PWM_PERIOD_BUFF,
        .cc0_buff    = PWM_CC0_BUFF,
        .cfg.pwm = {
            .auto_reload_cc0      = true,
            .auto_reload_line_sel = true,
            .auto_reload_period   = true,
            .cc0_match_up_en      = true,
            .cc0_match_down_en    = false,
            .overflow_mode        = SPWM_CTRL_SET,
            .cc0_match_mode       = SPWM_CTRL_CLEAR,
            .underflow_mode       = SPWM_CTRL_NO_CHANGE,
            .disable_mode         = SPWM_DISABLE_LOW,
        },
    };
}

/**
 * \brief       Prepares SPWM PWM dead-time configuration.
 *
 * \details     Initializes the SPWM configuration structure for PWM mode with
 *              dead-time insertion using predefined timing parameters.
 *
 * \param cfg   Pointer to SPWM configuration structure.
 */
void spwm_sample_app_prepare_pwmdt_cfg(spwm_config_t *cfg)
{
    if (!cfg) {
        return;
    }

    *cfg = (spwm_config_t){
        .mode        = SPWM_MODE_PWM_DT,
        .prescale    = SPWM_PRESCALE_DIV1,
        .count_mode  = SPWM_COUNT_UP,
        .period      = PWM_PERIOD,
        .cc0         = PWM_CC0,
        .period_buff = PWM_PERIOD_BUFF,
        .cc0_buff    = PWM_CC0_BUFF,
        .cfg.pwmdt = {
            .pwm = {
                .cc0_match_up_en      = true,
                .cc0_match_down_en    = false,
                .overflow_mode        = SPWM_CTRL_SET,
                .underflow_mode       = SPWM_CTRL_NO_CHANGE,
                .cc0_match_mode       = SPWM_CTRL_CLEAR,
                .disable_mode         = SPWM_DISABLE_LOW,
            },
            .deadtime       = DEADTIME,
            .compl_deadtime = COMPL_DEADTIME,
        },
    };
}

/**
 * \brief Configure SPWM group for Pseudo-Random (PWM-PR) operation.
 *
 * Uses LFSR-derived values for period and tap to generate
 * pseudo-random PWM output.
 *
 * \param cfg        Pointer to SPWM configuration structure.
 * \param duty       Duty cycle as a fractional value (e.g., 0.30f).
 */
void spwm_sample_app_prepare_pwmpr_cfg(spwm_config_t *cfg, const spwm_pwmpr_cfg_t *pr_cfg)
{
    if (!cfg || !pr_cfg) {
        return;
    }
    /* Calculate cc0 and cc0_buff */
    uint32_t cc0_value = (uint32_t)(PWMPR_DUTY_CYCLE * PWM_DUTY_SCALE_FACTOR);

    *cfg = (spwm_config_t) {
        .mode        = SPWM_MODE_PWM_PR,
        .prescale    = SPWM_PRESCALE_DIV1,
        .one_shot    = false,
        .count_mode  = SPWM_COUNT_UPDOWN2,
        .period      = pr_cfg->lfsr_seed,
        .counter     = pr_cfg->lfsr_seed,
        .period_buff = pr_cfg->lfsr_tap,
        .cc0         = cc0_value,
        .cc0_buff    = cc0_value,
        .cfg.pwmpr = {
            .pwm = {
                .cc0_match_up_en   = true,
                .cc0_match_down_en = true,
                .underflow_mode    = SPWM_CTRL_NO_CHANGE,
                .overflow_mode     = SPWM_CTRL_SET,
                .cc0_match_mode    = SPWM_CTRL_CLEAR,
                .disable_mode      = SPWM_DISABLE_LOW,
            },
        },
    };
}

/**
 * \brief                 Configures SPWM for descriptor-based PWM ramp operation
 *
 * \param cfg             Pointer to SPWM configuration structure.
 * \param group_id        SPWM group index.
 * \param initial_period  Initial buffered period value.
 * \param initial_cc      Initial buffered duty-cycle value.
 *
 * \return                0 on success, negative value on error.
 */
int32_t spwm_sample_app_pwm_ramp_cfg(spwm_config_t *cfg, uint8_t group_id,uint16_t initial_period,
                                     uint16_t initial_cc)
{
    if (!cfg) {
        LOG_ERROR(LOG_MOD_SPWM, "NULL config pointer\r\n");
        return -1;
    }

    memset(cfg, 0, sizeof(spwm_config_t));

    cfg->mode       = SPWM_MODE_PWM;
    cfg->clkdiv_sel = 0;
    cfg->count_mode = SPWM_COUNT_UP;
    cfg->prescale   = SPWM_PRESCALE_DIV1;
    cfg->one_shot   = 0;

    /* Initial start with short period */
    cfg->period      = 10;
    cfg->period_buff = initial_period;
    /* Use CC1 for duty cycle, CC0 disabled */
    cfg->cc0 = 0xFFFFFFFF;                /* CC0 disabled */
    cfg->cc1 = 2;                         /* CC1 */
    cfg->cc0_buff = 0xFFFFFFFF;           /* CC0  disabled*/
    cfg->cc1_buff = initial_cc;           /* Configured duty value → CC1_BUFF */

    LOG_INFO(LOG_MOD_SPWM,
             "Initial config: ACTIVE=[Period=%u CC1=%u] BUFFER=[Period=%u CC1=%u]\r\n",
             cfg->period, cfg->cc1, cfg->period_buff, cfg->cc1_buff);

    /* Left-aligned PWM mode */
    cfg->cfg.pwm.overflow_mode  = SPWM_CTRL_SET;
    cfg->cfg.pwm.cc0_match_mode = SPWM_CTRL_NO_CHANGE;
    cfg->cfg.pwm.underflow_mode = SPWM_CTRL_NO_CHANGE;
    cfg->cfg.pwm.cc1_match_mode = SPWM_CTRL_CLEAR;
    cfg->cfg.pwm.cc0_match_up_en = true;
    cfg->cfg.pwm.cc1_match_up_en = true;

    /* To set Auto-reload configuration */
    cfg->cfg.pwm.auto_reload_cc0      = true;
    cfg->cfg.pwm.auto_reload_cc1      = true;
    cfg->cfg.pwm.auto_reload_period   = true;
    cfg->cfg.pwm.auto_reload_line_sel = true;

    /* Output set low initially and PWM is enabled in buffer */
    cfg->cfg.pwm.line_sel = SPWM_REG_FORMAT_LINE_SEL(SPWM_LINE_SRC_L, SPWM_LINE_SRC_H);
    cfg->cfg.pwm.line_sel_buff = SPWM_REG_FORMAT_LINE_SEL(SPWM_LINE_SRC_PWM, SPWM_LINE_SRC_INV);
    cfg->cfg.pwm.disable_mode = SPWM_DISABLE_LOW;

    /* Semaphore on TC - BOTH channels like DIAG */
    cfg->sema[0] = SPWM_TR_OUT_TC;
    cfg->sema[1] = SPWM_TR_OUT_TC;

    /* Trigger configuration */
    for (int i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        cfg->trig_in[i].src = SPWM_TRIN_0;
        cfg->trig_in[i].edge = SPWM_TR_NO_EDGE_DET;
        cfg->trig_in[i].invert = false;
    }

    cfg->trig_in[SPWM_TRIG_COUNT].src = SPWM_TRIN_1;
    cfg->trig_in[SPWM_TRIG_START].src = SPWM_TRIN_0;

    /* CAPTURE0 self-loop */
    cfg->trig_in[SPWM_TRIG_CAPTURE0].src = SPWM_TRIN_TROUT0_0 + (group_id * 2);
    cfg->trig_in[SPWM_TRIG_CAPTURE0].edge = SPWM_TR_NO_EDGE_DET;
    cfg->trig_in[SPWM_TRIG_CAPTURE0].invert = false;

    /* Trigger output on TC to feed CAPTURE0 */
    cfg->trig_out[0] = SPWM_TR_OUT_TC;
    cfg->trig_out[1] = SPWM_TR_OUT_DISABLED;

    return 0;
}


/**
 * \brief Demonstrate SPWM TIMER mode as an event-driven counter using CC0 interrupt.
 *
 * Configures a SPWM group in TIMER mode. The counter increments based on external
 * trigger input edges (TRINx) and is started by a configured trigger event.
 *
 * This mode does NOT operate as a free-running timer. Counting progresses only
 * when the configured trigger conditions are met. CC0 compare interrupt is used
 * to signal completion.
 *
 * \param group_id SPWM group index to be used for the demo.
 */
void spwm_sample_app_timer(uint8_t group_id)
{
    spwm_config_t      cfg;
    spwm_status_en     st;
    SemaphoreHandle_t  sem = NULL;
    uint32_t           final_count = 0;
    int                ret;

    LOG_INFO(LOG_MOD_SPWM, "SPWM Timer Mode Starts (group %u)\r\n", group_id);

    /* Validate group_id */
    if (group_id >= SPWM_CONFIG_GROUP_INSTANCE_NUM) {
        LOG_ERROR(LOG_MOD_SPWM, "Invalid group_id %u (max %u)\r\n",
                  group_id, SPWM_CONFIG_GROUP_INSTANCE_NUM - 1U);
        return;
    }

    /* Disable before configuration */
    st = spwm_disable(SPWM_INSTANCE, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", group_id, st);
        return;
    }

    /* Create / ensure semaphore */
    ret = spwm_group_ensure_sem(group_id);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Semaphore create failed for group %u\r\n", group_id);
        return;
    }

    sem = spwm_group_ctx[group_id].sem;
    if (sem == NULL) {
        LOG_ERROR(LOG_MOD_SPWM, "Semaphore NULL for group %u\r\n", group_id);
        return;
    }

    /* Clock and divider */
    ret = spwm_sample_app_configure_clock(SPWM_INSTANCE, 0, 1, 0);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Clock configure failed (%d)\r\n", st);
        return;
    }

    st = spwm_set_clkdiv(SPWM_INSTANCE, group_id, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Clkdiv select failed for group %u (%d)\r\n", group_id, st);
        return;
    }

    /* Prepare configuration */
    memset(&cfg, 0, sizeof(cfg));

    /* To populate the configs into the strucuture */
    spwm_sample_app_prepare_timer_cfg(&cfg);

    for (uint32_t i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        cfg.trig_in[i].src    = SPWM_TRIN_0;
        cfg.trig_in[i].edge   = SPWM_TR_NO_EDGE_DET;
        cfg.trig_in[i].invert = false;
    }

    cfg.trig_in[SPWM_TRIG_COUNT].src = SPWM_TRIN_1;
    cfg.trig_in[SPWM_TRIG_START].src  = SPWM_TRIN_1;
    cfg.trig_in[SPWM_TRIG_START].edge = SPWM_TR_RISING_EDGE;

    cfg.trig_out[0] = SPWM_TR_OUT_CC0_MATCH;
    cfg.trig_out[1] = SPWM_TR_OUT_DISABLED;

    /* Apply configuration */
    st = spwm_configure(SPWM_INSTANCE, group_id, &cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Configure failed for group %u (%d)\r\n", group_id, st);
        return;
    }

    /* ISR setup */
    active_groups_arr[0] = 1;
    active_groups_arr[1] = group_id;

    ret = spwm_sample_app_configure_interrupts(SPWM_INSTANCE, group_id, SPWM_INTR_CC0_MATCH,
                                    spwm_irq_callback, (void *)active_groups_arr);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Interrupt config failed for group %u\r\n", group_id);
        return;
    }

    /* Enable */
    st = spwm_enable(SPWM_INSTANCE, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Enable failed for group %u (%d)\r\n", group_id, st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "Timer running, waiting for CC0 interrupt...\r\n");

    /* Wait */
    if (xSemaphoreTake(sem, pdMS_TO_TICKS(10000)) == pdTRUE) {

        st = spwm_get_counter(SPWM_INSTANCE, group_id, &final_count);
        if (st == SPWM_OK) {
            LOG_INFO(LOG_MOD_SPWM, "CC0 interrupt received, counter=%u\r\n", final_count);
        } else {
            LOG_ERROR(LOG_MOD_SPWM, "Failed to read counter (%d)\r\n", st);
        }
    } else {
        LOG_ERROR(LOG_MOD_SPWM, "Timeout waiting for CC0 interrupt\r\n");
    }

    /* Cleanup */
    st = spwm_disable(SPWM_INSTANCE, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", group_id, st);
        return;
    }

    active_groups_arr[0] = 0;
    active_groups_arr[1] = 0;

    LOG_INFO(LOG_MOD_SPWM, "SPWM-Timer Sample App Completed (group %u)!\r\n", group_id);
}

/**
 * \brief Demonstrate SPWM TIMER to CAPTURE operation.
 *
 * Configures one SPWM group as a timer generating CC0 events and a second
 * group in capture mode to measure those events via trigger routing.
 *
 * \param timer_grp   SPWM group used as timer source.
 * \param capture_grp SPWM group used for capture.
 *
 * \return 0 on success, -1 on failure.
 */
void spwm_sample_app_capture(uint8_t timer_grp, uint8_t capture_grp)
{
    spwm_config_t      timer_cfg;
    spwm_config_t      cap_cfg;
    spwm_status_en     st;
    SemaphoreHandle_t  sem;
    uint32_t           cap_cc0 = 0;
    uint32_t           cap_cc1 = 0;
    uint32_t           trout;
    int                ret;

    /* Validate group IDs */
    if ((timer_grp >= SPWM_CONFIG_GROUP_INSTANCE_NUM) ||
        (capture_grp >= SPWM_CONFIG_GROUP_INSTANCE_NUM)) {

        LOG_ERROR(LOG_MOD_SPWM,
                  "Invalid group id (timer=%u, capture=%u)\r\n",
                  timer_grp, capture_grp);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "SPWM Timer + Capture App Starts\r\n");

    /* Ensure both groups are stopped */
    st = spwm_disable(SPWM_INSTANCE, timer_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", timer_grp, st);
        return;
    }
    st = spwm_disable(SPWM_INSTANCE, capture_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", capture_grp, st);
        return;
    }

    /* Semaphore for timer interrupt */
    if (spwm_group_ensure_sem(timer_grp) != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Semaphore create failed for timer group\r\n");
        return;
    }

    sem = spwm_group_ctx[timer_grp].sem;

    /* Clock and divider */
    ret = spwm_sample_app_configure_clock(SPWM_INSTANCE, 0, 1, 0);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Clock configure failed (%d)\r\n", st);
        return;
    }

    st = spwm_set_clkdiv(SPWM_INSTANCE, timer_grp, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Clkdiv select failed for group %u (%d)\r\n", timer_grp, st);
        return;
    }
    st = spwm_set_clkdiv(SPWM_INSTANCE, capture_grp, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Clkdiv select failed for group %u (%d)\r\n", capture_grp, st);
        return;
    }

    /* To populate the configs into the strucuture */
    memset(&timer_cfg, 0, sizeof(timer_cfg));
    spwm_sample_app_prepare_timer_cfg(&timer_cfg);

    for (uint32_t i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        timer_cfg.trig_in[i].src    = SPWM_TRIN_0;
        timer_cfg.trig_in[i].edge   = SPWM_TR_NO_EDGE_DET;
        timer_cfg.trig_in[i].invert = false;
    }

    timer_cfg.trig_in[SPWM_TRIG_COUNT].src    = SPWM_TRIN_1;
    timer_cfg.trig_in[SPWM_TRIG_COUNT].edge   = SPWM_TR_NO_EDGE_DET;
    timer_cfg.trig_in[SPWM_TRIG_COUNT].invert = false;

    timer_cfg.trig_in[SPWM_TRIG_START].src      = SPWM_TRIN_1;
    timer_cfg.trig_in[SPWM_TRIG_START].edge     = SPWM_TR_RISING_EDGE;
    timer_cfg.trig_in[SPWM_TRIG_START].invert   = false;

    timer_cfg.trig_out[0] = SPWM_TR_OUT_CC0_MATCH;
    timer_cfg.trig_out[1] = SPWM_TR_OUT_DISABLED;

    /* To set the Timer configuration*/
    st = spwm_configure(SPWM_INSTANCE, timer_grp, &timer_cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Timer configure failed (%d)\r\n", st);
        return;
    }

    /* Monitor timer group */
    active_groups_arr[0] = 1;
    active_groups_arr[1] = timer_grp;

    ret = spwm_sample_app_configure_interrupts(SPWM_INSTANCE, timer_grp, SPWM_INTR_CC0_MATCH,
                                   spwm_irq_callback, active_groups_arr);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Timer interrupt config failed\r\n");
        return;
    }

    /* To populate the configs into the strucuture */
    memset(&cap_cfg, 0, sizeof(cap_cfg));
    spwm_sample_app_prepare_capture_cfg(&cap_cfg);

    cap_cfg.trig_in[SPWM_TRIG_COUNT].src = SPWM_TRIN_1;
    cap_cfg.trig_in[SPWM_TRIG_COUNT].edge = SPWM_TR_NO_EDGE_DET;

    cap_cfg.trig_in[SPWM_TRIG_START].src  = SPWM_TRIN_1;
    cap_cfg.trig_in[SPWM_TRIG_START].edge = SPWM_TR_RISING_EDGE;

    trout = SPWM_TRIN_TROUT0_0 + (timer_grp * 2);

    cap_cfg.trig_in[SPWM_TRIG_CAPTURE0].src = (spwm_trigger_src_en)trout;
    cap_cfg.trig_in[SPWM_TRIG_CAPTURE0].edge = SPWM_TR_RISING_EDGE;
    cap_cfg.trig_in[SPWM_TRIG_CAPTURE0].invert = false;

    cap_cfg.trig_in[SPWM_TRIG_CAPTURE1].src = (spwm_trigger_src_en)trout;
    cap_cfg.trig_in[SPWM_TRIG_CAPTURE1].edge = SPWM_TR_RISING_EDGE;
    cap_cfg.trig_in[SPWM_TRIG_CAPTURE1].invert = false;

    /* To set the Capture configuration */
    st = spwm_configure(SPWM_INSTANCE, capture_grp, &cap_cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Capture configure failed (%d)\r\n", st);
        return;
    }

    /* Enable capture first, then timer */
    st = spwm_enable(SPWM_INSTANCE, capture_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Enable failed for group %u (%d)\r\n", capture_grp, st);
        return;
    }
    st = spwm_enable(SPWM_INSTANCE, timer_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Enable failed for group %u (%d)\r\n", timer_grp, st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "Waiting for timer CC0 interrupt...\r\n");

    if (xSemaphoreTake(sem, pdMS_TO_TICKS(TIMER_DEMO_TIMEOUT_MS)) != pdTRUE) {
        LOG_ERROR(LOG_MOD_SPWM, "Timeout waiting for timer interrupt\r\n");
        return;
    }

    st = spwm_get_cc0(SPWM_INSTANCE, capture_grp, &cap_cc0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_get_cc0 failed\r\n");
        return;
    }
    st = spwm_get_cc1(SPWM_INSTANCE, capture_grp, &cap_cc1);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_get_cc1 failed\r\n");
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "Capture results: CC0=%u CC1=%u\r\n", cap_cc0, cap_cc1);

    if ((cap_cc0 == 0) && (cap_cc1 == 0)) {
        LOG_ERROR(LOG_MOD_SPWM, "Capture did not occur\r\n");
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "Capture successful\r\n");

    /* Cleanup */
    st = spwm_disable(SPWM_INSTANCE, timer_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", timer_grp, st);
        return;
    }
    st = spwm_disable(SPWM_INSTANCE, capture_grp);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Disable failed for group %u (%d)\r\n", capture_grp, st);
        return;
    }

    active_groups_arr[0] = 0U;
    active_groups_arr[1] = 0U;

    LOG_INFO(LOG_MOD_SPWM, "SPWM TIMER + CAPTURE Sample App Completed!\r\n");

    return;
}

/**
 * \brief Demonstrate SPWM PWM mode operation.
 *
 * Configures a SPWM group in PWM mode, sets up duty cycle and period,
 * enables output routing, and starts PWM generation.
 *
 * \param id       SPWM instance.
 * \param group_id SPWM group index.
 */
void spwm_sample_app_pwm(spwm_instance_en id, uint8_t group_id)
{
    spwm_status_en st;
    spwm_config_t  cfg;
    int            ret;

    /* Validate group_id */
    if (group_id >= SPWM_CONFIG_GROUP_INSTANCE_NUM) {
        LOG_ERROR(LOG_MOD_SPWM, "Invalid group_id %u\r\n", group_id);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "SPWM PWM-Demo Starts\r\n");

    /* Ensure group is disabled before configuration */
    st = spwm_disable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_disable failed (%d)\r\n", st);
        return;
    }

    /* Configure SPWM clock */
    ret = spwm_sample_app_configure_clock(id, 0, 1, 0);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_sample_app_configure_clock failed (%d)\r\n", st);
        return;
    }

    /* Select clock divider for this group */
    st = spwm_set_clkdiv(id, group_id, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_set_clkdiv failed (%d)\r\n", st);
        return;
    }

    memset(&cfg, 0, sizeof(cfg));
    spwm_sample_app_prepare_pwm_cfg(&cfg);

    /* Disable all trigger inputs */
    for (uint32_t i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        cfg.trig_in[i].src    = SPWM_TRIN_0;
        cfg.trig_in[i].edge   = SPWM_TR_NO_EDGE_DET;
        cfg.trig_in[i].invert = false;
    }

    /* Free-running counter and immediate start */
    cfg.trig_in[SPWM_TRIG_START].src = SPWM_TRIN_1;
    cfg.trig_in[SPWM_TRIG_COUNT].src = SPWM_TRIN_1;

    /* Trigger output routing */
    cfg.trig_out[0] = SPWM_TR_OUT_TC;
    cfg.trig_out[1] = SPWM_TR_OUT_DISABLED;

    /* PWM-specific configuration */
    spwm_pwm_cfg_t *pwm = &cfg.cfg.pwm;

    pwm->line_sel = spwm_make_line_sel(SPWM_LINE_SRC_PWM, SPWM_LINE_SRC_INV);
    pwm->line_sel_buff = pwm->line_sel;

    /* Apply SPWM configuration */
    st = spwm_configure(id, group_id, &cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_configure failed (%d)\r\n", st);
        return;
    }

    /* Select PWM output pin */
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_0, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_2, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }

    /* Enable PWM */
    st = spwm_enable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_enable failed (%d)\r\n", st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "SPWM Group %u Enabled\r\n", group_id);

    st = spwm_trigger(id, group_id, SPWM_TRIG_CAPTURE0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_trigger failed (%d)\r\n", st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "PWM started: Period=%u CC0=%u\r\n", PWM_PERIOD_BUFF, PWM_CC0_BUFF);

    LOG_INFO(LOG_MOD_SPWM,
        "Duty = %.1f%%\r\n", ((float)PWM_CC0_BUFF / (float)PWM_PERIOD_BUFF) * 100.0f);
    LOG_INFO(LOG_MOD_SPWM, "PWM Sample App Completed Successfully! \r\n" );
}

/**
 * \brief Demonstrate SPWM PWM with dead-time (PWM-DT) mode.
 *
 * Configures a SPWM group in PWM-DT mode with complementary outputs
 * and programmable rising and falling edge dead times.
 *
 * \param id       SPWM instance.
 * \param group_id SPWM group index.
 */
void spwm_sample_app_pwm_deadtime(spwm_instance_en id, uint8_t group_id)
{
    spwm_status_en st;
    spwm_config_t  cfg;
    int            ret;

    /* Validate group_id */
    if (group_id >= SPWM_CONFIG_GROUP_INSTANCE_NUM) {
        LOG_ERROR(LOG_MOD_SPWM, "Invalid group_id %u\r\n", group_id);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "PWM-DT App Starts\r\n");

    /* Ensure group is disabled before configuration */
    st = spwm_disable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_disable failed (%d)\r\n", st);
        return;
    }

    /* Configure SPWM clock */
    ret = spwm_sample_app_configure_clock(id, 0, 1, 0);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_sample_app_configure_clock failed (%d)\r\n", st);
        return;
    }

    st = spwm_set_clkdiv(id, group_id, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_set_clkdiv failed (%d)\r\n", st);
        return;
    }

    memset(&cfg, 0, sizeof(cfg));
    spwm_sample_app_prepare_pwmdt_cfg(&cfg);

    /* Disable triggers for simple test */
    for (uint32_t i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        cfg.trig_in[i].src    = SPWM_TRIN_0;
        cfg.trig_in[i].edge   = SPWM_TR_NO_EDGE_DET;
        cfg.trig_in[i].invert = false;
    }

    cfg.trig_in[SPWM_TRIG_START].src = SPWM_TRIN_1;
    cfg.trig_in[SPWM_TRIG_COUNT].src = SPWM_TRIN_1;

    cfg.trig_out[0] = SPWM_TR_OUT_TC;
    cfg.trig_out[1] = SPWM_TR_OUT_DISABLED;

    /* PWM-DT specific configuration */
    spwm_pwmdt_cfg_t *pwm_dt = &cfg.cfg.pwmdt;

    pwm_dt->pwm.line_sel = spwm_make_line_sel(SPWM_LINE_SRC_PWM, SPWM_LINE_SRC_INV);
    pwm_dt->pwm.line_sel_buff = pwm_dt->pwm.line_sel;

    /* Apply configuration */
    st = spwm_configure(id, group_id, &cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_configure failed (%d)\r\n", st);
        return;
    }

    /* Route PWM and complementary outputs */
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_0, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_1, true);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }

    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_2, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_3, true);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }

    /* Enable PWM-DT */
    st = spwm_enable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_enable failed (%d)\r\n", st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "PWM-Deadtime Sample App Completed!\r\n");
}

/**
 * \brief PWM Pseudo-Random (PWM-PR) mode demo.
 *
 * Configures and runs the SPWM group in pseudo-random PWM mode
 * using LFSR-based period variation and a fixed duty cycle.
 *
 * \param id       SPWM instance.
 * \param group_id SPWM group to be used for PWM-PR operation.
 *
 * \return 0 on success, -1 on failure.
 */
void spwm_sample_app_pwm_pseudo_random(spwm_instance_en id, uint8_t group_id)
{
    spwm_status_en st;
    spwm_config_t cfg;
    uint32_t trout_idx = group_id * 2;
    uint32_t ret;

    /* Validate group_id */
    if (group_id >= SPWM_CONFIG_GROUP_INSTANCE_NUM) {
        LOG_ERROR(LOG_MOD_SPWM, "Invalid group_id %u\r\n", group_id);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "SPWM PWM Pseudo Random Mode Test Starts\n");

    /* Disable before configuring */
    st = spwm_disable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_disable failed (%d)\r\n", st);
        return;
    }
    /* Setup clock divider for SPWM */
    ret = spwm_sample_app_configure_clock(id, 0, 1, 0);
    if (ret != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Clock configure failed (%d)\r\n", st);
        return;
    }

    /* Select clock divider 0 for this group */
    st = spwm_set_clkdiv(id, group_id, 0);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_clkdiv_enable failed (%d)\r\n", st);
        return;
    }

    memset(&cfg, 0, sizeof(cfg));
    spwm_sample_app_prepare_pwmpr_cfg(&cfg, &pr_cfg);

    /* 32bit counters */
    if (group_id >= 3) {
        cfg.period      <<= COUNTER_32BIT_SHIFT;
        cfg.counter     <<= COUNTER_32BIT_SHIFT;
        cfg.period_buff <<= COUNTER_32BIT_SHIFT;
        cfg.cc0         <<= COUNTER_32BIT_SHIFT;
    }

    /* Disable all input triggers */
    for (int i = 0; i < SPWM_TRIG_IN_COUNT; i++) {
        cfg.trig_in[i].src    = SPWM_TRIN_0;
        cfg.trig_in[i].edge   = SPWM_TR_NO_EDGE_DET;
        cfg.trig_in[i].invert = false;
    }

    cfg.trig_in[SPWM_TRIG_START].src  = SPWM_TRIN_1;
    cfg.trig_in[SPWM_TRIG_RELOAD].src = SPWM_TRIN_1;

    cfg.trig_in[SPWM_TRIG_RELOAD].src    = (spwm_trigger_src_en)(SPWM_TRIN_TROUT0_0 + trout_idx);
    cfg.trig_in[SPWM_TRIG_RELOAD].edge   = SPWM_TR_RISING_EDGE;
    cfg.trig_in[SPWM_TRIG_RELOAD].invert = false;

    cfg.trig_out[0] = SPWM_TR_OUT_CC0_MATCH;
    cfg.trig_out[1] = SPWM_TR_OUT_DISABLED;

    spwm_pwm_cfg_t *pwm_pr = &cfg.cfg.pwmpr.pwm;

    pwm_pr->line_sel       = spwm_make_line_sel(SPWM_LINE_SRC_PWM, SPWM_LINE_SRC_INV);
    pwm_pr->line_sel_buff  = pwm_pr->line_sel;

    st = spwm_configure(id, group_id, &cfg);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_configure failed (%d)\r\n", st);
        return;
    }

    active_groups_arr[0] = 1;          /* Num groups */
    active_groups_arr[1] = group_id;   /* group to monitor */
    /* Register single callback for this instance; pass active_groups_arr as user_ctx */
    if (spwm_sample_app_configure_interrupts(id, group_id, (SPWM_INTR_CC0_MATCH),
                                  spwm_irq_callback, (void *)active_groups_arr) != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "PWM PR: configure_interrupts failed for group %u\n", group_id);
        return;
    }

    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_0, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }
    st = spwm_output_select(id, group_id, SPWM_PWM_OUT_2, false);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed (%d)\r\n", st);
        return;
    }

    /* Enable PWM  */
    st = spwm_enable(id, group_id);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_enable failed (%d)\r\n", st);
        return;
    }

    st = spwm_trigger(id, group_id, SPWM_TRIG_START);
    if (st != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_trigger failed (%d)\r\n", st);
        return;
    }

    LOG_INFO(LOG_MOD_SPWM, "PWM Pseudo Random Mode Sample App Completed!\r\n");
}

/**
 * \brief                Executes a descriptor-based SPWM PWM ramp test.
 *
 * \param group_id       SPWM group index.
 * \param desc_instance  Descriptor instance identifier.
 *
 * \return               0 on success, negative value on failure.
 */
#if SPWM_PWM_MODE_DESC
uint32_t spwm_sample_app_pwm_ramp_test(uint8_t group_id, uint8_t desc_instance)
{
    spwm_pwm_step_t seq[RAMP_MAX_STEPS];
    uint32_t steps = 0;
    uint32_t target_steps = 6;
    uint32_t total_duration_us = SPWM_APP_BURST_US;  /* Required Burst time */
    uint32_t total_cycles = 0;
    uint32_t status = 1;

    for (uint32_t i = 0; i < target_steps && steps < RAMP_MAX_STEPS; i++)
    {
        float progress = (float)i / (float)(target_steps - 1);
        float freq = SPWM_APP_START_F + (SPWM_APP_END_F - SPWM_APP_START_F) * progress;

        /* Explicitly truncate to ensure consistent behavior */
        uint32_t period = (uint32_t)truncf(SPWM_APP_CLK_HZ / freq);
        if (period > 0) {
            period -= 1;
        }
        if (period < 2) {
            period = 2;
        }
        if (period > 65535) {
            period = 65535;
        }
        /* Calculate CC1 for the configured duty cycle */
        uint32_t period_count = period + 1;
        float cc1_float = (float)period_count * (SPWM_APP_DUTY_PCT * 0.01);  /* Duty Cycle */
        uint32_t cc1 = (uint32_t)truncf(cc1_float);  /* Truncation */

        if (cc1 == 0) {
            cc1 = 1;
        }
        if (cc1 >= period_count) {
            cc1 = period_count - 1;
        }

        /* Calculate cycles for this step to achieve even time distribution */
        float step_duration_us    = total_duration_us / (float)target_steps;
        float freq_actual         = SPWM_APP_CLK_HZ / (float)period_count;
        float cycles_float        = step_duration_us * freq_actual / 1e6f;
        uint32_t cycles_this_step = (uint32_t)truncf(cycles_float);

        if (cycles_this_step < 1) {
            cycles_this_step = 1;
        }
        seq[steps].period = (uint16_t)period;
        seq[steps].cc1    = (uint16_t)cc1;
        seq[steps].cycles = cycles_this_step;

        total_cycles += cycles_this_step;

        float duty_actual = (cc1 * 100.0f) / (float)period_count;

        LOG_INFO(LOG_MOD_SPWM,
                "Step %u: Period=%u CC1=%u Cycles=%u (f=%.0f Hz, duty=%.2f%%, ~%.1fµs)\r\n",
                 steps, period, cc1, cycles_this_step, freq_actual, duty_actual, step_duration_us);

        steps++;
    }
    if (steps == 0u) {
        LOG_ERROR(LOG_MOD_SPWM, "PWM ramp: steps == 0\r\n");
        return -1;
    }

    /* Calculate total descriptors:
       - First step: 3 descriptors (CC1 + PERIOD + LINE_SEL) × cycles
       - Remaining steps: (CLEAR + CC1+PERIOD) × cycles per step
       - Final: LINE_SEL disable
    */
    uint32_t total_descriptors = seq[0].cycles * 3;  /* First step with LINE_SEL enable */
    for (uint32_t i = 2; i < steps; i++) {
        total_descriptors += seq[i].cycles * 3;     /* CLEAR + CFG(2-count) */
    }
    total_descriptors += 1;  /* Final LINE_SEL disable */

    LOG_INFO(LOG_MOD_SPWM, "PWM ramp: %u steps, %u total cycles, ~%u µs burst, %u descriptors\r\n",
             steps, total_cycles, total_duration_us, total_descriptors);

    /* Disable and clean state */
    spwm_disable(SPWM_INSTANCE, group_id);

    if (spwm_group_ensure_sem(group_id) != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Semaphore creation failed\r\n");
        return -1;
    }

    xSemaphoreTake(spwm_group_ctx[group_id].sem, 0);

    /* Configure clock */
    spwm_sample_app_configure_clock(SPWM_INSTANCE, 0, 1, 0);
    if (spwm_set_clkdiv(SPWM_INSTANCE, group_id, 0) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Clock config failed\r\n");
        return -1;
    }

    /* To populate the configs into the strucuture */
    spwm_config_t cfg;
    if (spwm_sample_app_pwm_ramp_cfg(&cfg, group_id, seq[0].period, seq[0].cc1) != 0) {
        LOG_ERROR(LOG_MOD_SPWM, "Config population failed\r\n");
        return -1;
    }

    /* To Set the PWM-Ramp Configuration */
    if (spwm_configure(SPWM_INSTANCE, group_id, &cfg) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_configure failed\r\n");
        return -1;
    }

    /* Select PWM output pin */
    if (spwm_output_select(SPWM_INSTANCE, group_id, SPWM_PWM_OUT_0, false) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed \r\n");
        return -1;
    }
    if (spwm_output_select(SPWM_INSTANCE, group_id, SPWM_PWM_OUT_2, false) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_output_select failed \r\n");
        return -1;
    }

    /* Register Descriptor Callback */
    if (spwm_register_desc_callback(SPWM_INSTANCE, spwm_desc_done_isr,
                                    (void *)(uintptr_t)group_id) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "Failed to register descriptor callback\r\n");
        return -1;
    }

    /* Memory Allocation for Descriptor */
    uint16_t dmem_handle;
    spwm_dsc_status_en st = spwm_dsc_mem_alloc(DESC_REQUIRED_MEM, &dmem_handle);
    if (st != SPWM_DSC_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_dsc_mem_alloc failed: %d\r\n", st);
        return -1;
    }

    spwm_dsc_chain_t *chain = spwm_dsc_chain_create();
    if (!chain) {
        LOG_ERROR(LOG_MOD_SPWM, "Descriptor chain alloc failed\r\n");
        spwm_dsc_mem_free(dmem_handle);
        return -1;
    }

    st = spwm_dsc_chain_init_for_handle(SPWM_INSTANCE, chain, dmem_handle);
    if (st != SPWM_DSC_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "spwm_dsc_chain_init failed: %d\r\n", st);
        spwm_dsc_chain_destroy(chain);
        spwm_dsc_mem_free(dmem_handle);
        return -1;
    }

    uint32_t sem_mask = (1u << group_id);

    LOG_INFO(LOG_MOD_SPWM, "Building descriptor chain for configured burst time\r\n");

    /* Enable output on first cycle only */
    for (uint32_t cycle = 0; cycle < seq[0].cycles; cycle++)
    {
        /* CLEAR for cycles after the first */
        if (cycle > 0) {
            st = spwm_dsc_chain_add_syn(chain, SPWM_DSC_SYN_CLEAR, 0, sem_mask, false);
            if (st != SPWM_DSC_OK) {
                status = 0;
                LOG_ERROR(LOG_MOD_SPWM, "Sync Descriptor Failed with the status:%d\r\n", st);
                break;
            }
        }

        /* Load only CC1 + PERIOD (2 values) */
        uint32_t vals[2];
        vals[0] = (uint32_t)seq[0].cc1;      /* CC1 */
        vals[1] = (uint32_t)seq[0].period;   /* Period */
        st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_CC1_BUFF, vals, 2, false);
        if (st != SPWM_DSC_OK) {
            status = 0;
            LOG_ERROR(LOG_MOD_SPWM, "CFG Descriptor(Period) Failed with the status:%d\r\n", st);
            break;
        }
        /* Load LINE_SEL only on FIRST cycle to enable output */
        if (cycle == 0) {
            uint32_t line_sel_val = SPWM_REG_FORMAT_LINE_SEL(SPWM_LINE_SRC_PWM, SPWM_LINE_SRC_INV);
            st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_LINE_SEL_BUFF,
                                       &line_sel_val, 1, false);
            if (st != SPWM_DSC_OK) {
                status = 0;
                LOG_ERROR(LOG_MOD_SPWM,
                          "CFG Descriptor(Line Sel) Failed with the status:%d\r\n", st);
                break;
            }
        }
    }

    /* CLEAR + LOAD CC1+PERIOD only */
    for (uint32_t i = 1; i < steps; i++)
    {
        for (uint32_t cycle = 0; cycle < seq[i].cycles; cycle++)
        {
            st = spwm_dsc_chain_add_syn(chain, SPWM_DSC_SYN_CLEAR, 0, sem_mask, false);
            if (st != SPWM_DSC_OK) {
                status = 0;
                LOG_ERROR(LOG_MOD_SPWM, "Sync Descriptor Failed with the status:%d\r\n", st);
                break;
            }
            uint32_t vals[2];
            vals[0] = (uint32_t)seq[i].cc1;      /* CC1 */
            vals[1] = (uint32_t)seq[i].period;   /* Period */
            st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_CC1_BUFF, vals, 2, false);
            if (st != SPWM_DSC_OK) {
                status = 0;
                LOG_ERROR(LOG_MOD_SPWM, "CFG Descriptor Failed with the status:%d\r\n", st);
                break;
            }
        }
    }

    /* (N-1) Pre-stop: Disable output */
    st = spwm_dsc_chain_add_syn(chain, SPWM_DSC_SYN_CLEAR, 0, sem_mask, false);
    if (st != SPWM_DSC_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "Sync Descriptor Failed with the status:%d\r\n", st);
    }
    uint32_t vals_prestop[2];
    vals_prestop[0] = 0xFFFFFFFF;  /* Period_buff */
    /* Line_sel disable */
    vals_prestop[1] = SPWM_REG_FORMAT_LINE_SEL(SPWM_LINE_SRC_L, SPWM_LINE_SRC_H);
    st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_PERIOD_BUFF, vals_prestop, 2, false);
    if (st != SPWM_DSC_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "CFG Descriptor Failed with the status:%d\r\n", st);
    }
    /* (N) Final stop: Halt counter */
    st = spwm_dsc_chain_add_syn(chain, SPWM_DSC_SYN_CLEAR, 0, sem_mask, false);

    if (st != SPWM_DSC_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "SYNC Descriptor Failed with the status:%d\r\n", st);
    }

    /* Write CTRL register to disable counter (clear bit 31) */
    uint32_t ctrl_disable = ~SPWM_CTRL_DISABLE_MASK;
    st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_CTRL, &ctrl_disable, 1, false);
    if (st != SPWM_DSC_OK) {
        return -1;
    }

    uint32_t vals_stop[2];
    vals_stop[0] = (1u << SPWM_TRIG_STOP);  /* TR_CMD[STOP] */
    vals_stop[1] = 0;                       /* INTR_STA  */
    st = spwm_dsc_chain_add_cfg(chain, group_id, SPWM_DSC_REG_TR_CMD, vals_stop, 2, true);
    if (st != SPWM_DSC_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "CFG Descriptor Failed with the status:%d\r\n", st);
    }

    LOG_INFO(LOG_MOD_SPWM, "Descriptor chain built with %u descriptors\r\n", total_descriptors);

    /* Clear semaphore before starting */
    spwm_set_sema(SPWM_INSTANCE, group_id, 0, 0);
    spwm_set_sema(SPWM_INSTANCE, group_id, 1, 0);

    /* Enable counter */
    if (spwm_enable(SPWM_INSTANCE, group_id) != SPWM_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "SPWM Enable (spwm_enable) is failed\r\n");
    }

    /* Trigger START */
    spwm_trigger(SPWM_INSTANCE, group_id, SPWM_TRIG_START);

    /* Start descriptor engine */
    st = spwm_dsc_chain_start(SPWM_INSTANCE, chain, desc_instance, true);
    if (st != SPWM_DSC_OK) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM,
                 "Failed to start the chain Descriptor (spwm_dsc_chain_start): %d\r\n", st);
    }

    /* Wait for completion */
    if (xSemaphoreTake(spwm_group_ctx[group_id].sem, pdMS_TO_TICKS(TIMEOUT)) != pdTRUE) {
        status = 0;
        LOG_ERROR(LOG_MOD_SPWM, "Timeout waiting for Completion\r\n");
    }
    if (status == 1) {
        LOG_INFO(LOG_MOD_SPWM, "PWM-Ramp using Descriptor completed successfully!\r\n");
    }
    /* Disable the SPWM block */
    spwm_disable(SPWM_INSTANCE, group_id);

    /* Destroy Descriptor chain and free-up memory */
    spwm_dsc_chain_destroy(chain);
    spwm_dsc_mem_free(dmem_handle);

    return 0;
}
#endif

/**
 * \brief       Starts the SPWM sample application
 *
 * \details     This function initializes the SPWM driver and runs the configured
 *              SPWM demo mode based on build-time settings.
 *
 * \return      None
 */
void spwm_sample_app_main(void)
{
    LOG_INFO(LOG_MOD_SPWM, "\r\n SPWM Sample App Starts \r\n");
    if (spwm_init(SPWM_INSTANCE) != SPWM_OK) {
        LOG_ERROR(LOG_MOD_SPWM, "SPWM init failed\r\n");
        vTaskDelete(NULL);
    }

#if SPWM_TIMER_MODE
    spwm_sample_app_timer(GROUP_ID_0);
#endif

#if SPWM_CAPTURE_MODE
    spwm_sample_app_capture(GROUP_ID_0, GROUP_ID_1);
#endif

#if SPWM_PWM_MODE
    spwm_sample_app_pwm(SPWM_INSTANCE, GROUP_ID_0);
#endif

#if SPWM_PWM_DEADTIME_MODE
    spwm_sample_app_pwm_deadtime(SPWM_INSTANCE, GROUP_ID_0);
#endif

#if SPWM_PWM_PSEUDO_RANDOM_MODE
    spwm_sample_app_pwm_pseudo_random(SPWM_INSTANCE, GROUP_ID_0);
#endif

#if SPWM_PWM_MODE_DESC
    spwm_sample_app_pwm_ramp_test(GROUP_ID_0, SPWM_DEMO_DESC_ID);
#endif
    vTaskDelete(NULL);
}
