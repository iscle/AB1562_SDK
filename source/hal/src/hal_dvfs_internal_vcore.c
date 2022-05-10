/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
 #include "hal_dvfs_internal.h"

#ifdef HAL_DVFS_MODULE_ENABLED
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hal.h"
#include "hal_flash_sf.h"
#include "hal_gpt.h"
#include "hal_clock.h"
#include "hal_pmu.h"

#ifdef  HAL_DVFS_DEBUG_ENABLE
#define dvfs_debug(_message,...) log_hal_info("[DVFS] "_message, ##__VA_ARGS__)
#else
#define dvfs_debug(_message,...)
#endif

#define VCORE_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT)     pmu_lock_vcore_ab2568(PMU_NORMAL,PTR_NEXT->vcore,PMU_LOCK);\
                                                    pmu_lock_vcore_ab2568(PMU_NORMAL,PTR_CUR->vcore,PMU_UNLOCK)

#define VSRAM_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT)     if(PTR_NEXT->vsram < PTR_CUR->vsram){\
                                                        dvfs_vsram_set_delsel(PTR_NEXT->vsram);\
                                                        pmu_selec_vsram_voltage(PMU_NORMAL,PTR_NEXT->vsram);\
                                                    }else{\
                                                        pmu_selec_vsram_voltage(PMU_NORMAL,PTR_NEXT->vsram);\
                                                        dvfs_vsram_set_delsel(PTR_NEXT->vsram);\
                                                    }

#define PMIC_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT)      if(PTR_NEXT == &pmu_mapping_tbl_[DVFS_HIGH_SPEED_208M]){ \
                                                        VSRAM_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT);\
                                                        VCORE_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT);\
                                                    }else if(PTR_CUR == &pmu_mapping_tbl_[DVFS_HIGH_SPEED_208M]){\
                                                        VCORE_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT);\
                                                        VCORE_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT);\
                                                    }else{\
                                                        VCORE_DYNAMIC_SCALING(PTR_CUR,PTR_NEXT);\
                                                    }



typedef enum {
    DVFS_VCORE_MODE_LOW_SPEED = 26000,
    DVFS_VCORE_MODE_HELF_SPEED = 83000,
    DVFS_VCORE_MODE_FULL_SPEED = 104000,
    DVFS_VCORE_MODE_HIGH_SPEED = 208000
}dvfs_vcore_mode_t;
//------------[DVFS Variable Declaration]
static const uint32_t dvfs_vcore_voltage[DVFS_MAX_SPEED] =   { 0,700000,800000,900000 };



static const uint32_t dvfs_vcore_frequency[DVFS_MAX_SPEED] = { DVFS_VCORE_MODE_LOW_SPEED,
                                                               DVFS_VCORE_MODE_HELF_SPEED,
                                                               DVFS_VCORE_MODE_FULL_SPEED,
                                                               DVFS_VCORE_MODE_HIGH_SPEED };


#define __ISB()    __asm__ __volatile__(" isync             \n"                             " isync             \n")
#define __DSB()    __asm__ __volatile__(" dsync             \n"                             " isync             \n"); 
ATTR_TEXT_IN_TCM hal_dvfs_status_t dvfs_vcore_switch_frequency( unsigned int cur_opp, unsigned int next_opp)
{
    if(cur_opp == next_opp)
        return HAL_DVFS_STATUS_OK;
    uint32_t irq_mask;
    hal_dvfs_status_t result = HAL_DVFS_STATUS_OK;
    //dvfs_enter_privileged_level();
    hal_nvic_save_and_set_interrupt_mask(&irq_mask);
    hal_cache_flush_all_cache_lines();
    SFI_MaskAhbChannel(1);
    __ISB();__DSB();
    switch (next_opp) {
        case DVFS_HIGH_SPEED_208M:
            clock_mux_sel(CLK_SYS_SEL,3);
            clock_mux_sel(CLK_SFC_SEL,2);
            break;
        case DVFS_FULL_SPEED_104M:
            clock_mux_sel(CLK_SYS_SEL,4);
            clock_mux_sel(CLK_SFC_SEL,2);
            break;
        case DVFS_HALF_SPEED_83M:
            clock_mux_sel(CLK_SYS_SEL,5);
            clock_mux_sel(CLK_SFC_SEL,2);
            break;
        case DVFS_LOW_SPEED_26M:
            clock_mux_sel(CLK_SYS_SEL,0);
            clock_mux_sel(CLK_SFC_SEL,0);
            break;
        default:
            assert(0);
    }
    SFI_MaskAhbChannel(0);
    __ISB();__DSB();
    hal_nvic_restore_interrupt_mask(irq_mask);
    //dvfs_exit_privileged_level();
    return result;
}

ATTR_RWDATA_IN_TCM static dvfs_opp_t domain={
    //domain.name = __stringify("DVFS_VCORE");
    .opp_num = DVFS_MAX_SPEED, //max number about can choise voltage
    .module_num =1,// VCORE/DSP
    .cur_opp_index = DVFS_0P8V_VOLT_LV,
    .switch_voltage = NULL,//function about switch voltage
    .switch_frequency = dvfs_vcore_switch_frequency,// function about switch frequency
    .frequency = dvfs_vcore_frequency,//set all frequency
    .voltage = NULL,  // set all voltage
#ifdef MTK_SYSTEM_CLOCK_104M
    .basic_opp_index = DVFS_0P8V_VOLT_LV,
#elif defined(MTK_SYSTEM_CLOCK_208M)
    .basic_opp_index = DVFS_0P9V_VOLT_LV,
#elif defined(MTK_SYSTEM_CLOCK_83M)
    .basic_opp_index = DVFS_0P7V_VOLT_LV,
#elif defined(MTK_SYSTEM_CLOCK_26M)
    .basic_opp_index = DVFS_NONE_VOLT_LV,
#endif

};

dvfs_opp_t *dvfs_domain_init(){
    uint8_t clk_sys_clk_sel = clock_mux_cur_sel(CLK_SYS_SEL);
    switch(clk_sys_clk_sel){
    case DVFS_CM4_CLOCK_26M:
        domain.cur_opp_index = DVFS_NONE_VOLT_LV;
        break;
    case DVFS_CM4_CLOCK_52M:
        domain.cur_opp_index = DVFS_0P7V_VOLT_LV;
        break;
    case DVFS_CM4_CLOCK_104M:
        domain.cur_opp_index = DVFS_0P8V_VOLT_LV;
        break;
    case DVFS_CM4_CLOCK_208M:
        domain.cur_opp_index = DVFS_0P9V_VOLT_LV;
        break;
    default :
        return NULL;
    }
    return &domain;
}


#endif /* HAL_DVFS_MODULE_ENABLED */

