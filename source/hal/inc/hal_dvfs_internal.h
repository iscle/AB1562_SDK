/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
 *
 * This software/firmware and related documentation ("Airoha Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
 * Without the prior written permission of Airoha and/or its licensors,
 * any reproduction, modification, use or disclosure of Airoha Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
 * if you have agreed to and been bound by the applicable license agreement with
 * Airoha ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of Airoha Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
 */

#ifndef __HAL_DVFS_INTERNAL_H__
#define __HAL_DVFS_INTERNAL_H__

#include "hal_platform.h"

#ifdef HAL_DVFS_MODULE_ENABLED
#include "hal.h"
#include "memory_attribute.h"
#include "hal_pmu.h"
#include "hal_clock_internal.h"
//#define HAL_DVFS_DEBUG_ENABLE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define DVFS_MODE_NONE (-1)
#define DVFS_NULL_HANDLE 0
#define SRAM_DELSEL_ADDR                            ((volatile uint32_t *)0xA2010500)
#define SRAM_DELSEL_REG                             ((sram_del_sel*)SRAM_DELSEL_ADDR)
#define DVFS_LV_TO_SRAM_DELSEL_IDX(LV)       (LV-1)

typedef struct dvfs_lock_t {
    const char *name;
    unsigned int count;
    uint8_t lock_index;
    uint32_t dvfs_module_index;
    int count_0P7;
    int count_0P8;
    int count_0P9;
} dvfs_lock_t;

typedef enum {
    DVFS_NONE_VOLT_LV = 0,
    DVFS_0P7V_VOLT_LV,
    DVFS_0P8V_VOLT_LV,
    DVFS_0P9V_VOLT_LV,
    DVFS_CURRENT_SPEED,
    DVFS_ERROR_SPEED,
} dvfs_voltage_mode_t;

typedef struct {
    __IO uint32_t SRAM_RF_DELSEL;
    __IO uint32_t SRAM_HDE_DELSEL;
    __IO uint32_t SRAM_UHDE_DELSEL;
}sram_del_sel;


typedef struct {
    const char *name; //domain name
    dvfs_voltage_mode_t cur_opp_index; //now frequency index from table ;dvfs_voltage_mode_t
    uint32_t opp_num; //  208M 104M 52M 1.3 1.1 0.9 0.7
    unsigned int module_num; // how much frequency  about VCORE /DSP
    const uint32_t *voltage; //how much voltage can use 1.3 1.1 0.9
    const uint32_t *frequency; //how muchfrequency  208000, 104000, 52000
    void (*switch_voltage)(unsigned int , unsigned int );
    hal_dvfs_status_t (*switch_frequency)( unsigned int , unsigned int );
    dvfs_voltage_mode_t basic_opp_index;
} dvfs_opp_t;

dvfs_opp_t *dvfs_domain_init(void);
extern bool dvfs_switched_to_privileged;
int dvfs_query_frequency(uint32_t freq, const dvfs_opp_t *dvfs_domain, hal_dvfs_freq_relation_t relation);

#endif /* HAL_DVFS_MODULE_ENABLED */

#endif /* __HAL_DVFS_INTERNAL_H__ */
