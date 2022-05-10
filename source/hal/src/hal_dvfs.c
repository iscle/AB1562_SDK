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
#include "hal.h"

#include "hal_clock.h"

#ifdef  HAL_DVFS_DEBUG_ENABLE
#define dvfs_debug(_message,...) log_hal_info("[DVFS] "_message, ##__VA_ARGS__)
#else
#define dvfs_debug(_message,...)
#endif

#define RESET_DVFS_LEVEL(index)      (DVFS_LOCK_STATUS ? domain->frequency[freq_index] : domain->frequency[domain->basic_opp_index])
#define DVFS_LOCK_STATUS             *((uint32_t*)(&frequency_Ctrl[0]))
static dvfs_opp_t *domain;
uint8_t frequency_Ctrl[DVFS_MAX_SPEED];

//ATTR_RWDATA_IN_CACHED_SYSRAM bool dvfs_switched_to_privileged;

int dvfs_query_frequency(uint32_t freq, const dvfs_opp_t *dvfs_domain, hal_dvfs_freq_relation_t relation){
    int opp;
    uint8_t upper_bound = DVFS_HIGH_SPEED_208M, lower_bound = DVFS_LOW_SPEED_26M;
    for (opp = DVFS_HIGH_SPEED_208M ; opp >= DVFS_LOW_SPEED_26M; opp--) {
        if (freq == dvfs_domain->frequency[opp]) {
            return opp;
        }else if(freq < dvfs_domain->frequency[opp]){
            upper_bound = opp;
        }else{
            lower_bound = opp;
            break;
        }
    }
    opp = relation == HAL_DVFS_FREQ_RELATION_H ? upper_bound : lower_bound;
    return opp; //DVFS_MODE_NONE;
}

#ifdef MTK_DVFS_LOCK_CTRL_ENABLED
hal_dvfs_status_t hal_dvfs_lock_control(dvfs_frequency_t freq,hal_dvfs_lock_parameter_t lock) {
    int freq_index = DVFS_FULL_SPEED_104M;
    hal_dvfs_status_t result = HAL_DVFS_STATUS_ERROR;
    if (lock == HAL_DVFS_LOCK ) {
        hal_dvfs_target_cpu_frequency(domain->frequency[freq],HAL_DVFS_FREQ_RELATION_H);
        frequency_Ctrl[freq]++;

    }else if( (lock == HAL_DVFS_UNLOCK &&  frequency_Ctrl[freq])){

        --frequency_Ctrl[freq];

        if(!frequency_Ctrl[freq]){
            for (freq_index = freq; freq_index > DVFS_LOW_SPEED_26M; freq_index--){
                if (frequency_Ctrl[freq_index]) { break; }
            }
            hal_dvfs_target_cpu_frequency(RESET_DVFS_LEVEL(freq_index),HAL_DVFS_FREQ_RELATION_H);
        }
    }
    result = HAL_DVFS_STATUS_OK;
    return result;
}
#endif
uint32_t hal_dvfs_get_cpu_frequency(void){
    if (!domain) {
        //log_hal_error("[%s] non-init\r\n", __FUNCTION__);
        return 0;
    }
    return domain->frequency[domain -> cur_opp_index];
}

hal_dvfs_status_t hal_dvfs_get_cpu_frequency_list(const uint32_t **list, uint32_t *list_num){
    if (!domain) {
        return HAL_DVFS_STATUS_UNINITIALIZED;
    }
    *list = domain->frequency;
    *list_num = domain->opp_num;

    return HAL_DVFS_STATUS_OK;
}

hal_dvfs_status_t dvfs_target_frequency(dvfs_opp_t *domain, uint32_t target_freq,hal_dvfs_freq_relation_t relation){
    unsigned int freq_index = DVFS_HIGH_SPEED_208M;
    unsigned int next_opp_index, old_opp_index;
    hal_dvfs_status_t ret = HAL_DVFS_STATUS_OK;
    next_opp_index = (unsigned int)dvfs_query_frequency(target_freq, domain, relation);

    old_opp_index = domain -> cur_opp_index;

    for (; freq_index > next_opp_index; freq_index--) {
        if (frequency_Ctrl[freq_index]) { break; }
    }

    next_opp_index = freq_index;
    if (next_opp_index < old_opp_index) {  //voltgae down
        if((ret = domain->switch_frequency( old_opp_index, next_opp_index))!= HAL_DVFS_STATUS_OK)
            goto DVFS_END;
        //domain->switch_voltage(domain, old_opp_index, next_opp_index);
    }else if (next_opp_index > old_opp_index) {  //voltgae up
        //domain->switch_voltage(domain, old_opp_index, next_opp_index);
        if((ret = domain->switch_frequency( old_opp_index, next_opp_index))!= HAL_DVFS_STATUS_OK){
            //domain ->switch_voltage(domain,next_opp_index,old_opp_index); // back to origin vcore voltage
            goto DVFS_END;
        }
    }
    domain->cur_opp_index = next_opp_index;
DVFS_END:
    return ret;
}

hal_dvfs_status_t hal_dvfs_target_cpu_frequency(uint32_t target_freq, hal_dvfs_freq_relation_t relation) {
    return dvfs_target_frequency(domain, target_freq, relation);
}

hal_dvfs_status_t hal_dvfs_init(void){
    if (!domain) {
        domain = dvfs_domain_init();
        if (!domain) {
            return HAL_DVFS_STATUS_ERROR;
        }
    }
    return HAL_DVFS_STATUS_OK;
}
void dvfs_debug_dump(void){}

#endif /* HAL_DVFS_MODULE_ENABLED */

