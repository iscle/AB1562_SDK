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

#include "hal_captouch.h"
#include "hal_captouch_internal.h"
<<<<<<< HEAD
=======
#include "hal_rtc.h"
>>>>>>> db20e11 (second commit)
#include "hal_rtc_internal.h"
#ifdef HAL_CAPTOUCH_MODULE_ENABLED
#include "interrupt.h"
#include "log.h"
#include "string.h"
#include "hal_nvic.h"
#include "hal_gpt.h"
#include "hal_pmu.h"
#include "drv_charger.h"

<<<<<<< HEAD
bool Is_feature_enable;

extern uint8_t captouch_gpio_mapping[8];
=======
log_create_module(Captouch, PRINT_LEVEL_INFO);

bool Is_feature_enable;

extern uint8_t captouch_gpio_mapping[8];
extern uint32_t tune_delay_us;
>>>>>>> db20e11 (second commit)
extern NORMAL_SETTING_STRU *normal_setting;
extern LPM_SETTING_STRU *lpm_setting;
extern SDWU_SETTING_STRU *sdwu_setting;

#ifdef  HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
extern AUTOSUSPEND_SETTING_STRU *autosuspend_setting;
extern captouch_autosuspend_timeout_context_t autosuspend_timeout_context;
#endif

#ifdef HAL_CAPTOUCH_FORCE_TOUCH_ENABLED
extern captouch_forcetouch_context_t captouch_forcetouch_context;
extern TimerHandle_t force_touch_timer;
#endif

//extern FORCETOUCH_SETTING_STRU forcetouch_setting;
extern GPIO_CFG0_REGISTER_T *gpio_cfg;
extern captouch_ear_detect_t captouch_ear_detect;

extern TimerHandle_t captouch_earcheck_timer;
extern TimerHandle_t captouch_earcheck_stop_baseK_timer;
<<<<<<< HEAD
extern TimerHandle_t captouch_debounce_timer_ch0;
extern TimerHandle_t captouch_debounce_timer_ch1;
extern TimerHandle_t captouch_debounce_timer_ch2;
extern TimerHandle_t captouch_ctrl_manual_timer;
=======
#if 0
extern TimerHandle_t captouch_ctrl_manual_timer;
#endif
>>>>>>> db20e11 (second commit)

hal_captouch_status_t hal_captouch_set_avg(hal_captouch_channel_t channel,uint8_t mavg_val, uint8_t avg_val)
{
    if (mavg_val > CAPTOUCH_MAVG_MAX) return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    if (avg_val > CAPTOUCH_AVG_MAX) return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

<<<<<<< HEAD
    captouch_set_mavg(channel,mavg_val);
    captouch_set_avg(channel,avg_val);
=======
    captouch_channel_sense_control(1<<channel, captouch_disable);
    captouch_set_mavg(channel,mavg_val);
    captouch_set_avg(channel,avg_val);
    captouch_channel_sense_control(1<<channel, captouch_enable);
>>>>>>> db20e11 (second commit)

    return HAL_CAPTOUCH_STATUS_OK;
}

<<<<<<< HEAD
=======
static void hal_captouch_init_tune(uint8_t channel_bit_map, bool coarse_tune_en, bool capcon_state, bool isChargerIn)
{
    uint8_t tune_ch_bit_map;
    uint32_t i;

    tune_ch_bit_map = channel_bit_map;
    //if(!((channel_bit_map & (1<<HAL_CAPTOUCH_CHANNEL_0)) && (!captouch_get_channel_trigger(0) || isChargerIn || !capcon_state)))
    if((channel_bit_map & (1<<HAL_CAPTOUCH_CHANNEL_0)) && captouch_get_channel_trigger(0) && capcon_state)
    {
        tune_ch_bit_map = tune_ch_bit_map & (~(1<<HAL_CAPTOUCH_CHANNEL_0) & 0x7);
    }
    if(tune_ch_bit_map == 0) return;//no channel need tune

    captouch_set_autok_suspend(tune_ch_bit_map, false);
    for(i=0;i<3;i++)
    {
        if (tune_ch_bit_map & (1<<i))
            hal_captouch_set_avg(i,8,normal_setting->touchAvg[i]);
    }
    captouch_get_tune_delay_time(32, 8);
    hal_gpt_delay_us(tune_delay_us); //wait fine tune
    for(i=0;i<3;i++)
    {
        if(tune_ch_bit_map & (1<<i))
            captouch_find_baseline(i);
    }
    for(i=0;i<3;i++)
    {
        if (coarse_tune_en && (tune_ch_bit_map & (1<<i)) && (isChargerIn || !capcon_state))
        {
            hal_captouch_tune_control(i,HAL_CAPTOUCH_TUNE_HW_AUTO);
        }
        if (tune_ch_bit_map & (1<<i))
            hal_captouch_set_avg(i, normal_setting->touchMavg[i], normal_setting->touchAvg[i]);
    }
    captouch_set_autok_suspend(tune_ch_bit_map, true);
}

static void hal_captouch_lpwusd_setting(bool autosuspend_timeout_en)
{
    uint32_t i;

#ifdef HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
    // auto_suspend time out function enable
    if(autosuspend_timeout_en)
    {
        autosuspend_timeout_context.ch_bitmap = autosuspend_setting->ch_bitmap & (~autosuspend_setting->lpsd_ch_bitmap) & (~autosuspend_setting->lpwu_ch_bitmap);
        for(i =0; i<3;i++)
        {
            if(autosuspend_setting->time[i] != 0)
                autosuspend_timeout_context.time[i]= autosuspend_setting->time[i];
        }
    }
    //lpsd and lpwu setting
    if(captouch_sdwusetting_from_NVKEY())
    {
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN))
        {
            if(autosuspend_timeout_en)
            {
                if(autosuspend_setting->lpsd_ch_bitmap)
                    captouch_longpress_channel_select_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, autosuspend_setting->lpsd_ch_bitmap, sdwu_setting->sdtime*32000 );
            }
            else
            {
                captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, sdwu_setting->sdtime*32000);
            }
            pmu_lpsd_rst_init(CAP_LPSD, PMU_ON);
            if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN_VBUS))
                hal_captouch_set_lpsd_chk_vbus(true);
        }
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        {
            captouch_longpress_int_control(true);
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_WAKEUP, sdwu_setting->wutime*32000);
        }
    }
#else
    if(captouch_sdwusetting_from_NVKEY())
    {
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN))
        {
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, sdwu_setting->sdtime*32000);
            pmu_lpsd_rst_init(CAP_LPSD, PMU_ON);
            if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN_VBUS))
                hal_captouch_set_lpsd_chk_vbus(true);
        }
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        {
            captouch_longpress_int_control(true);
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_WAKEUP, sdwu_setting->wutime*32000);
        }
    }
#endif
}

>>>>>>> db20e11 (second commit)
hal_captouch_status_t hal_captouch_init(void)
{
    uint8_t bitmap;
    uint32_t i;
    hal_captouch_status_t ret;
<<<<<<< HEAD
    bool capcon_state, lpwu_flag, isChargerIn;
    S8 finecap;
=======
    bool capcon_state, lpwu_flag = false, isChargerIn;
    //S8 finecap;
>>>>>>> db20e11 (second commit)
#ifdef HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
    bool autosuspend_timeout_en;
#endif

    Is_feature_enable = captouch_is_feature_enable();
    if(!Is_feature_enable) return HAL_CAPTOUCH_STATUS_ERROR;

    if(captouch_context.has_initilized == true) return HAL_CAPTOUCH_STATUS_INITIALIZED;

    captouch_check_tune_data_nvkey();
    captouch_normalsetting_from_NVKEY();

    if(normal_setting == NULL) return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    memset(&captouch_context, 0, sizeof(captouch_context_t));
#ifdef HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
    memset(&autosuspend_timeout_context, 0, sizeof(autosuspend_timeout_context));
#endif
    captouch_context.ear_detect = 0xff;

    capcon_state = rtc_internal_captouch_get_capcon_state();
<<<<<<< HEAD
    lpwu_flag = captouch_get_lpwu_int_flag();
    isChargerIn = DRV_CHARGER_IsPlugInState();
#ifdef  HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
    autosuspend_timeout_en = captouch_autosuspend_timeout_enable();
    captouch_init_parameter_print(capcon_state, lpwu_flag, isChargerIn, normal_setting->eardetect, autosuspend_timeout_en);
#else
    captouch_init_parameter_print(capcon_state, lpwu_flag, isChargerIn, normal_setting->eardetect, 0);
=======
    isChargerIn = DRV_CHARGER_IsPlugInState();
    lpwu_flag = captouch_get_lpwu_int_flag();
    //if(capcon_state && (hal_rtc_get_power_on_reason() == RTC_POWERED_BY_CAPTOUCH) && (sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP)))
	    //lpwu_flag = true;

#ifdef  HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
    autosuspend_timeout_en = captouch_autosuspend_timeout_enable();
    HAL_CAPTOUCH_LOG_PRINT("captouch init capcon_state:%d, lpwu_flag:%d, isChargerIn:%d, ear_detect_ch:%d, autosuspend_timeout_en:%d", 5,
                                                                                                            capcon_state, lpwu_flag, isChargerIn, normal_setting->eardetect, autosuspend_timeout_en);
#else
    HAL_CAPTOUCH_LOG_PRINT("captouch init capcon_state:%d, lpwu_flag:%d, isChargerIn:%d, ear_detect_ch:%d, autosuspend_timeout_en:%d", 5,
                                                                                                            capcon_state, lpwu_flag, isChargerIn, normal_setting->eardetect, 0);
>>>>>>> db20e11 (second commit)
#endif
    //open captouch annalog clock
    ret = hal_captouch_power_on_init(captouch_enable);
    if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

    captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_disable);

    //set threshold and coarse_cap
    for(i = 0; i < 3; i++)
    {
        if(normal_setting->captouchChannelBitMap & (1<<i))
        {
            ret = hal_captouch_set_threshold(i, normal_setting->highThre[i],normal_setting->lowThre[i]);
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

            ret = hal_captouch_set_coarse_cap(i,captouch_coarsecap_from_nvkey(i));
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

            ret = hal_captouch_set_avg(i,normal_setting->touchMavg[i],normal_setting->touchAvg[i]);
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

            if(i == 1) gpio_cfg->GPIO_G.CLR = (1<<4);

            if(i == 2) gpio_cfg->GPIO_G.CLR = (1<<5);
        }
    }

<<<<<<< HEAD
    if(!(DRV_CHARGER_IsPlugInState() || !capcon_state)&& !normal_setting->eardetect)
    {
        for(int i = 0; i < 3; i++)
        {
            if(normal_setting->captouchChannelBitMap & (1<<i))
            {
                finecap = captouch_fine_cap_from_nvkey(i);
                ret = hal_captouch_set_fine_cap(i, finecap);
                if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

                captouch_set_control_manual(i, true);
            }
        }
        captouch_ctrl_manual_timer = xTimerCreate("captouch_ctrl_man", pdMS_TO_TICKS(100), pdFALSE, NULL, captouch_findbase_timer_handle);
    }
=======
#if 0
    if(capcon_state && lpwu_flag && normal_setting->eardetect == 0x01)
    {
        finecap = captouch_fine_cap_from_nvkey(0);
        ret = hal_captouch_set_fine_cap(0, finecap);
        if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;
        captouch_set_control_manual(0, true);
        captouch_ctrl_manual_timer = xTimerCreate("captouch_ctrl_man", pdMS_TO_TICKS(100), pdFALSE, NULL, captouch_findbase_timer_handle);

        HAL_CAPTOUCH_LOG_PRINT("captouch init enable fine cap manual mode", 0);
    }
#endif
>>>>>>> db20e11 (second commit)
    captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_enable);
    for(i = 0; i < 3; i++)
    {
        if(normal_setting->eardetect & (1<<i))
        {
            captouch_context.ear_detect = i;
            captouch_ear_detect.mavg_num = 1<<(normal_setting->cfineBaseAvgNum);
            captouch_ear_detect.baseKFrozeTime = normal_setting->baseKFrozeTime;
            bitmap = normal_setting->captouchChannelBitMap & (~normal_setting->eardetect);
            captouch_channel_int_control(bitmap, captouch_enable);
        }
    }
    if(captouch_context.ear_detect == 0xFF) captouch_channel_int_control(normal_setting->captouchChannelBitMap, captouch_enable);

    captouch_set_dynamic_threshold(false, true, 0x28, 0);

#ifdef HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
<<<<<<< HEAD
     // auto_suspend time out function enable
    autosuspend_timeout_en = captouch_autosuspend_timeout_enable();
    if(autosuspend_timeout_en)
    {
        autosuspend_timeout_context.ch_bitmap = autosuspend_setting->ch_bitmap & (~autosuspend_setting->lpsd_ch_bitmap) & (~autosuspend_setting->lpwu_ch_bitmap);
        for(int i =0; i<3;i++)
        {
            if(autosuspend_setting->time[i] != 0)
                autosuspend_timeout_context.time[i]= autosuspend_setting->time[i];
        }
    }
        //lpsd and lpwu setting
    if(captouch_sdwusetting_from_NVKEY())
    {
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN))
        {
            if(autosuspend_timeout_en)
            {
                if(autosuspend_setting->lpsd_ch_bitmap)
                    captouch_longpress_channel_select_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, autosuspend_setting->lpsd_ch_bitmap, sdwu_setting->sdtime*32000 );
            }
            else
            {
                captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, sdwu_setting->sdtime*32000);
            }
            pmu_lpsd_rst_init(CAP_LPSD, PMU_ON);
            if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN_VBUS))
                hal_captouch_set_lpsd_chk_vbus(true);
        }
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        {
            captouch_longpress_int_control(true);
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_WAKEUP, sdwu_setting->wutime*32000);
        }
    }
#else
    if(captouch_sdwusetting_from_NVKEY())
    {
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN))
        {
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_SHUTDOWN, sdwu_setting->sdtime*32000);
            pmu_lpsd_rst_init(CAP_LPSD, PMU_ON);
            if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_SHUTDOWN_VBUS))
                hal_captouch_set_lpsd_chk_vbus(true);
        }
        if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        {
            captouch_longpress_int_control(true);
            captouch_longpress_channel_control(HAL_CAPTOUCH_LONGPRESS_WAKEUP, sdwu_setting->wutime*32000);
        }
    }
#endif
    captouch_clk_control(true);
    hal_gpt_delay_ms(10);

    if(DRV_CHARGER_IsPlugInState() || !capcon_state)
    {
        captouch_set_autok_suspend(normal_setting->captouchChannelBitMap, false);
        for(i = 0; i < 3; i++)
        {
            if (normal_setting->captouchChannelBitMap & (1<<i))
            {
                captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_disable);
                hal_captouch_set_avg(i, 5, normal_setting->touchAvg[i]);
                captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_enable);
                captouch_find_baseline(i);
                if(normal_setting->swtune_en)
                    hal_captouch_tune_control(i, HAL_CAPTOUCH_TUNE_HW_AUTO);
            }
        }
    }

    //Create SW debounce timer for touch key channel
    for(i = 0; i < 3; i++)
    {
        if(normal_setting->captouchChannelBitMap & (1<<i))
        {
            if(captouch_context.ear_detect != i)
            {
                switch(i)
                {
                    case HAL_CAPTOUCH_CHANNEL_0:
                        captouch_debounce_timer_ch0 = xTimerCreate("captouchdeb_ch0", pdMS_TO_TICKS(normal_setting->swDebounceTime), pdFALSE, NULL, captouch_key_press_event_handler);
                        break;
                    case HAL_CAPTOUCH_CHANNEL_1:
                        captouch_debounce_timer_ch1 = xTimerCreate("captouchdeb_ch1", pdMS_TO_TICKS(normal_setting->swDebounceTime), pdFALSE, NULL, captouch_key_press_event_handler);
                        break;
                    case HAL_CAPTOUCH_CHANNEL_2:
                        captouch_debounce_timer_ch2 = xTimerCreate("captouchdeb_ch2", pdMS_TO_TICKS(normal_setting->swDebounceTime), pdFALSE, NULL, captouch_key_press_event_handler);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    //register nvic irq handler and enable irq

    for(i = 0; i < 3; i++)
    {
        hal_captouch_set_avg(i, normal_setting->touchMavg[i], normal_setting->touchAvg[i]);
    }

    if((!DRV_CHARGER_IsPlugInState() && capcon_state ) && (!lpwu_flag))
    {
        for (int i =0;i<3 ; i++)
        {
            if (normal_setting->captouchChannelBitMap & (1<<i))
            {
                captouch_set_control_manual(i, false);
                hal_gpt_delay_ms(10);
                captouch_channel_sense_control(1<<i, captouch_disable);
                hal_captouch_set_avg(i, 5, normal_setting->touchAvg[0]);
                captouch_channel_sense_control(1<<i, captouch_enable);
                captouch_find_baseline(i);
                captouch_channel_sense_control(1<<i, captouch_disable);
                hal_captouch_set_avg(i, normal_setting->touchMavg[i], normal_setting->touchAvg[i]);
                captouch_channel_sense_control(1<<i, captouch_enable);
            }
        }
    }
=======
    hal_captouch_lpwusd_setting(autosuspend_timeout_en);
#else
    hal_captouch_lpwusd_setting(false);
#endif

    //Begin Fine/Coarse tune
    HAL_CAPTOUCH_LOG_PRINT("captouch fine/coarse cal process channel coarse cap:%x, %x, %x", 3, captouch_coarsecap_from_nvkey(0), captouch_coarsecap_from_nvkey(1), captouch_coarsecap_from_nvkey(2));
    captouch_clk_control(true);
    hal_gpt_delay_ms(10);
    hal_captouch_init_tune(normal_setting->captouchChannelBitMap, normal_setting->swtune_en, capcon_state, isChargerIn);
#if 0
    if(captouch_is_control_manual(0) && !captouch_get_channel_trigger(0))
    {
        captouch_set_control_manual(0, false);
        hal_gpt_delay_ms(10);
        hal_captouch_set_avg(0, 5, normal_setting->touchAvg[0]);
        captouch_find_baseline(0);
        hal_captouch_set_avg(0, normal_setting->touchMavg[0], normal_setting->touchAvg[0]);
        HAL_CAPTOUCH_LOG_PRINT("captouch init disable fine cap manual mode", 0);
    }
#endif
>>>>>>> db20e11 (second commit)

    //auto suspend
    captouch_set_autok_suspend(normal_setting->captouchChannelBitMap, true);
    captouch_set_autok_suspend(normal_setting->eardetect, false);
    hal_gpt_delay_us(200);
    //captouch_set_autok_Nsuspend(captouch_context.used_channel_map,true)

#ifdef  HAL_CAPTOUCH_FORCE_TOUCH_ENABLED
    captouch_force_init();
#endif
<<<<<<< HEAD
=======
    //register nvic irq handler and enable irq
>>>>>>> db20e11 (second commit)
    captouch_context.has_initilized = true;
    captouch_register_nvic_callback();
    captouch_int_control(true);

<<<<<<< HEAD

    if(lpwu_flag)
=======
    if(lpwu_flag && captouch_get_channel_trigger(0))
>>>>>>> db20e11 (second commit)
    {
        captouch_send_press_msg();
    }
    //Create SW mode timer for in ear channel
    if(normal_setting->eardetect != 0)
    {
        captouch_context.fine_cap_base[captouch_context.ear_detect] = captouch_earcheck_from_nvkey(captouch_context.ear_detect);
        captouch_earcheck_timer = xTimerCreate("CaptouchEarCheck", pdMS_TO_TICKS(500), pdTRUE, NULL, captouch_sw_ear_check);
        captouch_start_timer();
        if(captouch_ear_detect.baseKFrozeTime != 0)
        {
            captouch_earcheck_stop_baseK_timer = xTimerCreate ("CaptouchEarBasekStop", pdMS_TO_TICKS(captouch_ear_detect.baseKFrozeTime*1000),\
                                                                                       pdFALSE,\
                                                                                       NULL,\
                                                                                       captouch_stop_earcheck_basek_timeout);
        }
    }
    return HAL_CAPTOUCH_STATUS_OK;
}
hal_captouch_status_t hal_captouch_power_on_init(bool is_en)
{
    hal_captouch_status_t ret;

    rtc_internal_set_ldo08_boost(true);
    ret = captouch_rtc_clk_control(is_en);
    captouch_analog_init();
    rtc_internal_set_ldo08_boost(false);

    return ret;
}

void hal_captouch_coarse_cap_tune(hal_captouch_coarse_cal_t* coarse_cal_result)
{
    uint8_t i;

<<<<<<< HEAD
    captouch_int_control(false);
=======
    captouch_get_tune_delay_time(32, 8);
    captouch_int_control(false);
    if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        captouch_longpress_int_control(false);

>>>>>>> db20e11 (second commit)
    for(i = 0; i < 3; i++)
    {
        if(normal_setting->captouchChannelBitMap & (1<<i))
        {
            captouch_set_autok_suspend((1<<i), false);
<<<<<<< HEAD
            hal_captouch_set_avg(i, 5, normal_setting->touchAvg[i]);
            captouch_find_baseline(i);
            coarse_cal_result->coarse_cal[i].result = hal_captouch_tune_control(i, HAL_CAPTOUCH_TUNE_HW_AUTO);
            hal_captouch_set_avg(i, normal_setting->touchMavg[i], normal_setting->touchAvg[i]);
            captouch_set_autok_suspend((1<<i), true);
            coarse_cal_result->coarse_cal[i].coarse_value= captouch_get_coarse_cap(i);
=======
            hal_captouch_set_avg(i, 8, normal_setting->touchAvg[i]);
            hal_gpt_delay_us(tune_delay_us); //wait fine tune
            captouch_find_baseline(i);
            coarse_cal_result->coarse_cal[i].result = hal_captouch_tune_control(i, HAL_CAPTOUCH_TUNE_HW_AUTO);
            coarse_cal_result->coarse_cal[i].coarse_value= captouch_get_coarse_cap(i);
            hal_captouch_set_avg(i, normal_setting->touchMavg[i], normal_setting->touchAvg[i]);
>>>>>>> db20e11 (second commit)
        }
        else
        {
            coarse_cal_result->coarse_cal[i].result = 0xFF;
            coarse_cal_result->coarse_cal[i].coarse_value = 0xFF;
        }
    }
<<<<<<< HEAD
    captouch_int_control(true);
=======
    captouch_set_autok_suspend(normal_setting->captouchChannelBitMap, true);
    hal_gpt_delay_us(200);
    captouch_int_control(true);
    if(sdwu_setting->sdwu_en & (1<<HAL_CAPTOUCH_LONGPRESS_WAKEUP))
        captouch_longpress_int_control(true);
>>>>>>> db20e11 (second commit)
}

hal_captouch_status_t hal_captouch_channel_enable(hal_captouch_channel_t channel)
{
    if(channel>= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    //touch channel enable&aio en
    captouch_channel_sense_control(1<<channel, captouch_enable);

    //enable high and low threhold wakeup&int interrupt
    captouch_channel_int_control(1<<channel, captouch_enable);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_channel_disable(hal_captouch_channel_t channel)
{
    if(channel>= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    //disable chanel sensing
    captouch_channel_sense_control(channel, captouch_disable);

    //mask hign and low threhold wakeup&int interrupt
    captouch_channel_int_control(1<<channel, captouch_disable);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_get_event(hal_captouch_event_t *event)
{
    if(event == NULL)
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    if(captouch_get_buffer_data_size() <= 0)
        return HAL_CAPTOUCH_STATUS_NO_EVENT;

    captouch_pop_one_event_from_buffer(event);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_set_threshold(hal_captouch_channel_t channel, S16 high_thr, S16 low_thr)
{
    if(channel >= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_CHANNEL_ERROR;

    if((high_thr > 255) || (high_thr < (-256)) || (low_thr > 255) || (low_thr < (-256)))
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    captouch_set_threshold(channel, high_thr, low_thr);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_set_Nthreshold(hal_captouch_channel_t channel,S16 high_thr, S16 low_thr)
{
    if(channel >= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_CHANNEL_ERROR;

    if ((high_thr > 255) || (high_thr < (-256)) || (low_thr > 255) || (low_thr< (-256)))
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    captouch_set_nthreshold(channel, high_thr, low_thr);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_set_fine_cap(hal_captouch_channel_t channel,S8 fine)
{
    if(channel >= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_CHANNEL_ERROR;

    if((fine > 63) || (fine < (-64)))
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    captouch_set_fine_cap(channel, fine);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_set_coarse_cap(hal_captouch_channel_t channel, uint32_t coarse)
{
    if(channel >= HAL_CAPTOUCH_CHANNEL_MAX)
        return HAL_CAPTOUCH_STATUS_CHANNEL_ERROR;

    if(coarse > CAPTOUCH_COARSE_CAP_MAX)
        return HAL_CAPTOUCH_STATUS_INVALID_PARAMETER;

    captouch_set_coarse_cap(channel, (uint8_t)coarse);

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_tune_control(hal_captouch_channel_t channel,hal_captouch_tune_type_t tune_type)
{
    hal_captouch_status_t ret;
    uint32_t count;
    UNUSED(tune_type);

    count = 0;
    while(1)
    {
        ++count;
        ret = captouch_hw_auto_tune(channel);
        if(ret) break;
        if(count >= 3) break;
    }
    return ret;
}

hal_captouch_status_t hal_captouch_lowpower_control(hal_captouch_lowpower_type_t lowpower_type)
{
    if(!Is_feature_enable)
        return HAL_CAPTOUCH_STATUS_ERROR;

    if(captouch_context.has_initilized != true)
        return HAL_CAPTOUCH_STATUS_UNINITIALIZED;

    captouch->TOUCH_CON0.CON0 = 0;

    if(HAL_CAPTOUCH_MODE_NORMAL == lowpower_type)
    {
        captouch_rtc_lpm_control(HAL_CAPTOUCH_MODE_NORMAL);
        captouch->TOUCH_ANACFG0.ANACFG0 = (1<<16) | 0x2e;
        captouch_set_clk(HAL_CAPTOUCH_MODE_NORMAL, HAL_CAPTOUCH_CLK_32K);
        captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_enable);
        captouch_int_control(true);
    }
    else if (HAL_CAPTOUCH_MODE_LOWPOWER == lowpower_type)
    {
        captouch_rtc_lpm_control(HAL_CAPTOUCH_MODE_LOWPOWER);
        captouch->TOUCH_ANACFG0.ANACFG0 = (1<<16) | 0x3f;
        captouch_set_clk(HAL_CAPTOUCH_MODE_NORMAL, HAL_CAPTOUCH_CLK_4K);
<<<<<<< HEAD
        captouch_channel_sense_control(1<<HAL_CAPTOUCH_CHANNEL_0, captouch_disable);
        hal_captouch_set_avg(HAL_CAPTOUCH_CHANNEL_0, 5, 5);
        captouch_channel_sense_control(1<<HAL_CAPTOUCH_CHANNEL_0, captouch_enable);
        captouch_clk_control(true);
        captouch_int_control(true);
        hal_gpt_delay_ms(200);
        captouch_channel_sense_control(1<<HAL_CAPTOUCH_CHANNEL_0, captouch_disable);
        hal_captouch_set_avg(HAL_CAPTOUCH_CHANNEL_0, normal_setting->touchMavg[0] - 3, normal_setting->touchAvg[0]);
        captouch_channel_sense_control(1<<HAL_CAPTOUCH_CHANNEL_0, captouch_enable);
=======
        hal_captouch_set_avg(HAL_CAPTOUCH_CHANNEL_0, 5, 5);
        captouch_clk_control(true);
        captouch_int_control(true);
        hal_gpt_delay_ms(200);
        hal_captouch_set_avg(HAL_CAPTOUCH_CHANNEL_0, normal_setting->touchMavg[0] - 3, normal_setting->touchAvg[0]);
>>>>>>> db20e11 (second commit)
        captouch_set_autok_suspend(0x1, true);
    }

    return HAL_CAPTOUCH_STATUS_OK;
}

hal_captouch_status_t hal_captouch_deinit(void)
{
    captouch_analog_deinit();
    captouch_rtc_clk_control(captouch_disable);

    //disable channel interrupt
    captouch_channel_int_control(normal_setting->captouchChannelBitMap, captouch_disable);

    //disable channel calibration
    captouch_channel_sense_control(normal_setting->captouchChannelBitMap, captouch_disable);

    //close the captouch clock
    captouch_clk_control(captouch_disable);

    //disable the captouch inerrupt
    captouch_int_control(captouch_disable);

    captouch_context.has_initilized = false;

    memset(&captouch_context, 0, sizeof(captouch_context_t));

    return HAL_CAPTOUCH_STATUS_OK;
}

void hal_captouch_disable_sw_tune(void)
{
    captouch_normsetting_to_nvkey(SWTUNE_EN_ID, 0, 0x00);
}

void hal_captouch_set_lpsd_chk_vbus(bool is_en)
{
    captouch_set_lpsd_chk_vbus(is_en);
}
void hal_captouch_set_vbus_flag(bool is_chgin)
{
    captouch_set_vbus_flag(is_chgin);
}

#ifdef captouch_dvt
extern GPIO_CFG0_REGISTER_T *gpio_cfg;
hal_captouch_status_t hal_captouch_dvt_init(void)
{
    uint32_t i, time1, time2, duration, event, state, temp;
    hal_captouch_status_t ret;
    hal_captouch_config_t config;
    hal_captouch_channel_t channel;

    config.channel_bit_map = 0x7;
    config.mavg_r[0] = 10;
    config.mavg_r[1] = 9;
    config.mavg_r[2] = 8;
    config.avg_s[0] = 5;
    config.avg_s[1] = 6;
    config.avg_s[2] = 4;
    config.coarse_cap[0] = 2;
    config.coarse_cap[1] = 3;
    config.coarse_cap[2] = 3;
    config.high_thr[0] = 100;
    config.high_thr[1] = 110;
    config.high_thr[2] = 120;
    config.low_thr[0] = 40;
    config.low_thr[1] = 50;
    config.low_thr[2] = 60;

    temp = *(volatile uint32_t*)0xa20c0030;
    printf("---hal_captouch_hqa_init c0030=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa2080078;
    printf("hal_captouch_hqa_init rtc gpio0=0x%x\r\n", temp);
    gpio_cfg->GPIO_G.CLR = 0x30;

    if(captouch_context.has_initilized == true)
        return HAL_CAPTOUCH_STATUS_INITIALIZED;

    memset(&captouch_context, 0, sizeof(captouch_context_t));

    //get ept channel bit map
    captouch_context.used_channel_map = config.channel_bit_map;
    //captouch_context.captouch_callback.callback  = config.callback.callback;
    //captouch_context.captouch_callback.user_data = config.callback.user_data;

    //open captouch annalog clock
    ret = captouch_rtc_clk_control(captouch_enable);
    if (ret != HAL_CAPTOUCH_STATUS_OK) return ret;

    captouch_analog_init();

    //set threshold and coarse_cap
    for(i = 0; i < 3; i++)
    {
        if(captouch_context.used_channel_map & (1<<i))
        {
            ret = hal_captouch_set_threshold(i, config.high_thr[i], config.low_thr[i]);
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

            ret = hal_captouch_set_coarse_cap(i, config.coarse_cap[i]);
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;

            ret = hal_captouch_set_avg(i, config.mavg_r[i], config.avg_s[i]);
            if(ret != HAL_CAPTOUCH_STATUS_OK) return ret;
        }
    }

    captouch_channel_sense_control(captouch_context.used_channel_map, captouch_enable);
    captouch_channel_int_control(captouch_context.used_channel_map, captouch_enable);

    captouch_clk_control(true);

    //register nvic irq handler and enable irq
    captouch_register_nvic_callback();

    //auto suspend
    //captouch_set_autok_suspend(captouch_context.used_channel_map,true);

    captouch_int_control(true);
    hal_gpt_delay_ms(200);

    temp = *(volatile uint32_t*)0xa20f1000;
    printf("hal_captouch_hqa_init 0x00=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1004;
    printf("hal_captouch_hqa_init 0x04=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1008;
    printf("hal_captouch_hqa_init 0x08=0x%x\r\n", temp);
    temp =*(volatile uint32_t*)0xa20f100c;
    printf("hal_captouch_hqa_init 0x0c=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1010;
    printf("hal_captouch_hqa_init 0x10=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1014;
    printf("hal_captouch_hqa_init 0x14=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1018;
    printf("hal_captouch_hqa_init 0x18=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1030;
    printf("hal_captouch_hqa_init 0x30=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1034;
    printf("hal_captouch_hqa_init 0x34=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f1038;
    printf("hal_captouch_hqa_init 0x38=0x%x\r\n", temp);
    temp = *(volatile uint32_t*)0xa20f103c;
    printf("hal_captouch_hqa_init 0x3c=0x%x\r\n", temp);

      temp =*(volatile uint32_t*)0xa20f106c;
    printf("hal_captouch_hqa_init 0x6c=0x%x\r\n", temp);
      temp =*(volatile uint32_t*)0xa20f1070;
    printf("hal_captouch_hqa_init 0x70=0x%x\r\n", temp);
      temp =*(volatile uint32_t*)0xa20f1074;
    printf("hal_captouch_hqa_init 0x74=0x%x\r\n", temp);
    //hal_gpt_delay_ms(200);

    captouch_context.has_initilized = true;

    return HAL_CAPTOUCH_STATUS_OK;
}
#endif //captouch_dvt
#endif //HAL_CAPTOUCH_MODULE_ENABLED

