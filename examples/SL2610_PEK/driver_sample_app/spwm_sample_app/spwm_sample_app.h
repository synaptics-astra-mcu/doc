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

#ifndef SPWM_SAMPLE_APP_H
#define SPWM_SAMPLE_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
*                              Include files
*******************************************************************************/
#include <stdint.h>
#include "spwm.h"
#include <syna_device.h>
#include "FreeRTOS.h"
#include "semphr.h"

/*******************************************************************************
*                              Macro definitions
*******************************************************************************/

/* SPWM SUPPORTED OPERATING MODES */
#define SPWM_TIMER_MODE                     0
#define SPWM_CAPTURE_MODE                   0
#define SPWM_PWM_MODE                       0
#define SPWM_PWM_DEADTIME_MODE              0
#define SPWM_PWM_PSEUDO_RANDOM_MODE         0
#define SPWM_PWM_MODE_DESC                  0

#define SPWM_APP_CLK_HZ                     100000000UL
#define RAMP_MAX_STEPS                      12u
#define DESC_REQUIRED_MEM                   680

/* DESCRIPTOR MODE CONFIGURATION */
#if SPWM_PWM_MODE_DESC
    /* Demo selection - set ONE to 1, rest to 0 */
    #define SPWM_DEMO_440_480KHZ            0
    #define SPWM_DEMO_800_840KHZ            0
    #define SPWM_DEMO_100_400KHZ            0
    #define SPWM_DEMO_200_800KHZ            0

    /* Case 1: 440-480 kHz, 80 µs, 15% duty (existing) */
    #if SPWM_DEMO_440_480KHZ
        #define SPWM_APP_START_F            440000u
        #define SPWM_APP_END_F              480000u
        #define SPWM_APP_DUTY_PCT           15u
        #define SPWM_APP_BURST_US           80u
    /* Case 2: 800-840 kHz, 30 µs, 25% duty */
    #elif SPWM_DEMO_800_840KHZ
        #define SPWM_APP_START_F            800000u
        #define SPWM_APP_END_F              840000u
        #define SPWM_APP_DUTY_PCT           25u
        #define SPWM_APP_BURST_US           30u
    /* Case 3: 100-400 kHz, 90 µs, 20% duty */
    #elif SPWM_DEMO_100_400KHZ
        #define SPWM_APP_START_F            100000u
        #define SPWM_APP_END_F              400000u
        #define SPWM_APP_DUTY_PCT           20u
        #define SPWM_APP_BURST_US           90u
    /* Case 4: 100-400 kHz, 90 µs, 20% duty */
    #elif SPWM_DEMO_200_800KHZ
        #define SPWM_APP_START_F            200000u
        #define SPWM_APP_END_F              800000u
        #define SPWM_APP_DUTY_PCT           25u
        #define SPWM_APP_BURST_US           80u
    #endif
#endif

/* SPWM PWM: Deadtime, Pseudo Random, Timer, Capture Parameters */
#define PWM_PERIOD                          10000
#define PWM_CC0                             4000
#define PWM_PERIOD_BUFF                     5000
#define PWM_CC0_BUFF                        1000
#define DEADTIME                            2000
#define COMPL_DEADTIME                      3000
#define CAPTURE_GRP_PERIOD                  0xFFFFFFFF
#define TIMER_PERIOD                        10000
#define TIMER_CC0                           1000
#define TIMER_DEMO_TIMEOUT_MS               3000
#define CAPTURE_PERIOD                      60000
#define PWMPR_DUTY_CYCLE                    0.3f
#define COUNTER_32BIT_SHIFT                 16
#define PWM_DUTY_SCALE_FACTOR               0x8000U

/* Instances for Groups: Timer, Capture and Descriptor */
#define GROUP_ID_0                          0
#define GROUP_ID_1                          1

#define SPWM_INSTANCE                       SPWM0
#define TIMER_GROUP                         GROUP_ID_0
#define CAPTURE_GROUP                       GROUP_ID_1

#define SPWM_MAX_GROUPS                     12
#define SPWM_DEMO_DESC_ID                   0  /* Can be configured as 1, 2 */

#define TIMEOUT                             1000

/*******************************************************************************
*                              Type definitions
*******************************************************************************/
typedef struct {
    uint32_t period;
    uint32_t cc1;
    uint32_t cycles;
} spwm_pwm_step_t;

/*******************************************************************************
*                              Function declarations
*******************************************************************************/

/*******************************************************************************
* Function Name: spwm_sample_app_configure_clock
********************************************************************************
*
* Configures the SPWM clock divider.
*
* \param id         SPWM instance identifier.
* \param div_id     Clock divider index.
* \param int_div    Integer divider value.
* \param frac_div   Fractional divider value.
*
* \return Status of the operation.
*
*******************************************************************************/
int32_t spwm_sample_app_configure_clock(spwm_instance_en id, uint8_t div_id,
                                        uint16_t int_div, uint8_t frac_div);

/*******************************************************************************
* Function Name: spwm_sample_app_configure_interrupts
********************************************************************************
*
* Configures SPWM interrupts for a given group.
*
* \param id         SPWM instance identifier.
* \param group_id   SPWM group index.
* \param intr       Interrupt mask.
* \param cb         Interrupt callback function.
* \param ctx        User context pointer.
*
* \return Status of the operation.
*
*******************************************************************************/
int32_t spwm_sample_app_configure_interrupts(spwm_instance_en id, uint8_t group_id,
                                             spwm_intr_en intr, spwm_isr_cb_t cb, void *ctx);

/*******************************************************************************
* Function Name: spwm_sample_app_prepare_timer_cfg
********************************************************************************
*
* Prepares SPWM timer-mode configuration.
*
* \param cfg Pointer to SPWM configuration structure.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_prepare_timer_cfg(spwm_config_t *cfg);

/*******************************************************************************
* Function Name: spwm_sample_app_prepare_capture_cfg
********************************************************************************
*
* Prepares SPWM capture-mode configuration.
*
* \param cfg Pointer to SPWM configuration structure.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_prepare_capture_cfg(spwm_config_t *cfg);

/*******************************************************************************
* Function Name: spwm_sample_app_prepare_pwm_cfg
********************************************************************************
*
* Prepares SPWM PWM-mode configuration.
*
* \param cfg Pointer to SPWM configuration structure.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_prepare_pwm_cfg(spwm_config_t *cfg);

/*******************************************************************************
* Function Name: spwm_sample_app_prepare_pwmdt_cfg
********************************************************************************
*
* Prepares SPWM PWM Dead-Time (PWM-DT) mode configuration.
*
* \param cfg Pointer to SPWM configuration structure.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_prepare_pwmdt_cfg(spwm_config_t *cfg);

/*******************************************************************************
* Function Name: spwm_sample_app_prepare_pwmpr_cfg
********************************************************************************
*
* Prepares SPWM PWM pseudo-random mode configuration.
*
* \param cfg      Pointer to sPWM configuration structure.
* \param pr_cfg   Pointer to PWM pseudo-random configuration parameters.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_prepare_pwmpr_cfg(spwm_config_t *cfg, const spwm_pwmpr_cfg_t *pr_cfg);

/*******************************************************************************
* Function Name: spwm_sample_app_pwm_ramp_cfg
********************************************************************************
*
* Configures SPWM for PWM ramp operation.
*
* \param cfg             Pointer to SPWM configuration structure.
* \param group_id        SPWM group index.
* \param initial_period  Initial buffered period value.
* \param initial_cc      Initial buffered duty-cycle value.
*
* \return 0 on success, negative value on error.
*
*******************************************************************************/
int32_t spwm_sample_app_pwm_ramp_cfg(spwm_config_t *cfg, uint8_t group_id, uint16_t initial_period,
                                     uint16_t initial_cc);

/*******************************************************************************
* Function Name: spwm_sample_app_timer
********************************************************************************
*
* Runs the SPWM timer mode demonstration.
*
* \param group_id SPWM group index.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_timer(uint8_t group_id);

/*******************************************************************************
* Function Name: spwm_sample_app_capture
********************************************************************************
*
* Runs the SPWM capture mode demonstration.
*
* \param timer_group   Timer group index.
* \param capture_group Capture group index.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_capture(uint8_t timer_group, uint8_t capture_group);

/*******************************************************************************
* Function Name: spwm_sample_app_pwm
********************************************************************************
*
* Runs the SPWM PWM mode demonstration.
*
* \param id       SPWM instance identifier.
* \param group_id SPWM group index.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_pwm(spwm_instance_en id, uint8_t group_id);

/*******************************************************************************
* Function Name: spwm_sample_app_pwm_deadtime
********************************************************************************
*
* Runs the SPWM PWM Dead-Time mode demonstration.
*
* \param id       SPWM instance identifier.
* \param group_id SPWM group index.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_pwm_deadtime(spwm_instance_en id, uint8_t group_id);

/*******************************************************************************
* Function Name: spwm_sample_app_pwm_pseudo_random
********************************************************************************
*
* Runs the SPWM PWM pseudo-random mode demonstration.
*
* \param id       SPWM instance identifier.
* \param group_id SPWM group index.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_pwm_pseudo_random(spwm_instance_en id, uint8_t group_id);

/*******************************************************************************
* Function Name: spwm_sample_app_pwm_ramp_test
********************************************************************************
*
* Executes a PWM frequency ramp test using SPWM.
*
* \param group_id       SPWM group index.
* \param desc_instance  Descriptor instance identifier.
*
* \return 0 on success, negative value on failure.
*
*******************************************************************************/
uint32_t spwm_sample_app_pwm_ramp_test(uint8_t group_id, uint8_t desc_instance);

/*******************************************************************************
* Function Name: spwm_sample_app_main
********************************************************************************
*
* Main entry function for the SPWM sample application.
*
* \return void
*
*******************************************************************************/
void spwm_sample_app_main(void);

#ifdef __cplusplus
}
#endif

#endif /* SPWM_SAMPLE_APP_H */