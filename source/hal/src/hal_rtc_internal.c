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
#include "hal_platform.h"
#ifdef HAL_RTC_MODULE_ENABLED
#include "hal_rtc_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hal.h"
#include "hal_eint.h"
#include "hal_gpt.h"
#include "hal_nvic.h"
#include "hal_clock.h"
#include "syslog.h"
#include "hal_captouch_internal.h"
#include "assert.h"
/***********************************************************************************************
 * Pre Processor Definitions
***********************************************************************************************/
<<<<<<< HEAD
#define     RTC_EOSC_CALI_FREQUENCY     30660
=======
#define     RTC_EOSC_CALI_FREQUENCY     32768
>>>>>>> db20e11 (second commit)

/*version rg*/
#define   RTC_CHIP_VERSION      *((uint32_t *)0xA2000008)
#define   RTC_ECO_VERSION       *((uint32_t *)0xA2000000)
#define   ECO_VER_E1            0xCA00
#define   ECO_VER_E2            0xCA01
#define   ECO_VER_E3            0xCA02

//#define   RTC_HW_NEED_ECO

/*register number*/
#define   RTC_SPARE_RG_NUM      6

/*Avaiable byte per register*/
#define   RTC_SPARE_RG_BYTE     2

/*Enable time check log*/
//#define RTC_DEBUG_TIME_CHECK_ENABLED

#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
#define     RTC_DEBUG_GET_1M_TICK( tick )   hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &tick)
#else
#define     RTC_DEBUG_GET_1M_TICK( tick )
#endif


log_create_module(RTC, PRINT_LEVEL_INFO);


/***********************************************************************************************
 * Private type Declarations
***********************************************************************************************/
static  volatile RTC_REGISTER_T             *rtc_register          = (RTC_REGISTER_T *)RTC_BASE;
static  volatile RTC_ABIST_FQMTR_REGISTER_T *rtc_fqmtr_register    = (RTC_ABIST_FQMTR_REGISTER_T *)RTC_ABIST_FQMTR_BASE;
static  const    rtc_period_type_mask_map_t  g_rtc_type_mask_map[] =
{
   { .type = HAL_RTC_TIME_NOTIFICATION_NONE,             .mask =0},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND,     .mask =3},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_MINUTE,     .mask =4},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_HOUR,       .mask =5},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_DAY,        .mask =6},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_MONTH,      .mask =8},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_YEAR,       .mask =9},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_2, .mask =2},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_4, .mask =1},
   { .type = HAL_RTC_TIME_NOTIFICATION_EVERY_SECOND_1_8, .mask =0},
};
static  volatile    bool                    g_cg_enabled = false;
static  volatile    uint32_t                g_wr_state   = 0;
static  volatile    uint32_t                g_ref_cnt    = 0;
/***********************************************************************************************
 * Function Declare
***********************************************************************************************/
inline void                 rtc_internal_reload(void);
uint32_t                    rtc_internal_measure_counter(uint32_t fixed_clock, uint32_t tested_clock, uint32_t window_setting);
extern hal_flash_status_t   hal_flash_otp_read(uint32_t start_address, uint8_t *buffer, uint32_t length);

/***********************************************************************************************
 * Private Function
***********************************************************************************************/

static  bool    rtc_local_is_eco_done()
{
#ifdef RTC_HW_NEED_ECO
    static  bool prt_done = false;
    uint32_t    chp_ver,eco_ver;
    bool        rslt = false;

    chp_ver = RTC_CHIP_VERSION;
    eco_ver = RTC_ECO_VERSION;
    if (chp_ver == 0x1562 && eco_ver >= ECO_VER_E3){
        rslt = true;
    } else if (chp_ver == 0x1568 && eco_ver >= ECO_VER_E2){
        rslt = true;
    }
    if(prt_done == false) {
        log_rtc_warn("chip version %x-> e%d, eco sta %d", 3, (chp_ver&0xff), ((eco_ver&0xff)+1), rslt);
        prt_done = true;
    }
    return rslt;
#else
    return false;
#endif
}

static  void   rtc_local_save_irq(uint32_t  *status)
{
#ifdef HAL_NVIC_MODULE_ENABLED
    if(status != NULL) {
        extern hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask_special(uint32_t *mask);
        hal_nvic_save_and_set_interrupt_mask_special(status);
    }
#endif
}

static  void   rtc_local_restore_irq(uint32_t  status)
{
#ifdef HAL_NVIC_MODULE_ENABLED
    extern hal_nvic_status_t hal_nvic_restore_interrupt_mask_special(uint32_t mask);
    hal_nvic_restore_interrupt_mask_special(status);
#endif
}

static void rtc_local_wait_done(void)
{
<<<<<<< HEAD
#define     RTC_TIMEOUT_WAIT_BUSY_US    500
=======
#define     RTC_TIMEOUT_WAIT_BUSY_US    5000
>>>>>>> db20e11 (second commit)

    uint32_t  count = 0;
    uint32_t  pre_tick = 0, cur_tick = 0, tick_gap = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &pre_tick);
    while (1) {
        if ((RTC_WRTGR_CBUSY_MASK & rtc_register->RTC_WRTGR) == 0) {
            break;
        }
        /* for check wait busy timeout */
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &cur_tick);
        hal_gpt_get_duration_count(pre_tick, cur_tick, &tick_gap);
        /* if timeout, then print log and update pre-tick */
        if(tick_gap > RTC_TIMEOUT_WAIT_BUSY_US){
<<<<<<< HEAD
            if(count++ == 3){
                log_rtc_info("[hal][rtc] local_wait_done: failed will assert\r\n", 0);
                assert(0);
=======
            if(count++ == 5){
                log_rtc_error("[hal][rtc] local_wait_done: failed ,sw will return\r\n", 0);
                break;
>>>>>>> db20e11 (second commit)
            } else {
                pre_tick = cur_tick;
                log_rtc_info("[hal][rtc] local_wait_done(%d): timeout=%dus, RTC_WRTGR = %x!\r\n", 3, count, tick_gap, rtc_register->RTC_WRTGR);
            }
        }
    }
}


static void rtc_local_trigger_wait()
{
    rtc_register->RTC_WRTGR |= RTC_WRTGR_WRTGR_MASK;
    rtc_local_wait_done();
}


static  void    rtc_local_config_wakeup_source(rtc_wakeup_source_t source, bool enable)
{
    uint32_t    key  = 0;
    uint32_t    mask = 0;
    switch(source)
    {
        case RTC_WAKEUP_SOURCE_ALARM:  key = KEY_BBPU_MASK_ALARM; break;
        case RTC_WAKEUP_SOURCE_TICK:   key = KEY_BBPU_MASK_TICK;  break;
        case RTC_WAKEUP_SOURCE_EINT0:  key = KEY_BBPU_MASK_EINT0;  break;
        case RTC_WAKEUP_SOURCE_EINT1:  key = KEY_BBPU_MASK_EINT1;  break;
        case RTC_WAKEUP_SOURCE_EINT2:  key = KEY_BBPU_MASK_EINT2; break;
        case RTC_WAKEUP_SOURCE_CAP:    key = KEY_BBPU_MASK_CAP;   break;
        //case RTC_WAKEUP_SOURCE_GALARM: key = KEY_BBPU_MASK_GALARM;break;
        default:    break;
    }
    mask = (1<<source);
    if( enable )
        rtc_register->RTC_BBPU = ((key<<8) | mask);
    else
        rtc_register->RTC_BBPU = (key<<8);
    rtc_local_trigger_wait();
}



static void    rtc_local_lowpower_detect()
{
    //rtc_internal_reload();
    /*enable lpd*/
    rtc_register->RTC_LPD_CON |= RTC_LPD_CON_EOSC32_LPDEN_MASK;
    rtc_local_trigger_wait();
    /*reset lpd*/
    rtc_register->RTC_LPD_CON |= (RTC_LPD_CON_EOSC32_LPDEN_MASK | RTC_LPD_CON_LPRST_MASK);
    rtc_local_trigger_wait();
    /* designer suggests delay at least 1 ms */
    hal_gpt_delay_us(1000);
    /*disable rst*/
    rtc_register->RTC_LPD_CON &= ~RTC_LPD_CON_LPRST_MASK;
    rtc_local_trigger_wait();
    rtc_register->RTC_LPD_CON |= RTC_LPD_CON_EOSC32_LPDEN_MASK;
    rtc_local_trigger_wait();

    if ((rtc_register->RTC_LPD_CON & RTC_LPD_CON_LPSTA_RAW_MASK) != 0) {
        log_rtc_error("rtc_lpd_init fail : RTC_LPD_CON = %x!\r\n", 1, (unsigned int) rtc_register->RTC_LPD_CON);
    }
}



static  void    rtc_local_set_osc32con(uint32_t reg_id, uint32_t val)
{
    volatile    uint32_t *reg_addr = NULL;
    //uint32_t    status;
    uint32_t    key_1[] = { RTC_OSC32CON0_MAGIC_KEY_1, \
                            RTC_OSC32CON1_MAGIC_KEY_1, \
                            RTC_OSC32CON2_MAGIC_KEY_1  \
                          };
    uint32_t    key_2[] = { RTC_OSC32CON0_MAGIC_KEY_2, \
                            RTC_OSC32CON1_MAGIC_KEY_2, \
                            RTC_OSC32CON2_MAGIC_KEY_2  \
                          };
    switch(reg_id) {
        case 0:  reg_addr = &rtc_register->RTC_OSC32CON0; break;
        case 1:  reg_addr = &rtc_register->RTC_OSC32CON1; break;
        case 2:  reg_addr = &rtc_register->RTC_OSC32CON2_UNION.VALUE; break;
        default:    {
            return;
        }
    }

    //rtc_local_save_irq(&status);
    *reg_addr = key_1[reg_id];
    rtc_local_wait_done();
    *reg_addr = key_2[reg_id];
    rtc_local_wait_done();
    *reg_addr = val;
    rtc_local_wait_done();
    //rtc_local_restore_irq(status);
}


static  uint32_t    rtc_local_get_osc32con(uint32_t reg_id)
{
    volatile    uint32_t *reg_addr = NULL;
    switch(reg_id) {
        case 0:  reg_addr = &rtc_register->RTC_OSC32CON0; break;
        case 1:  reg_addr = &rtc_register->RTC_OSC32CON1; break;
        case 2:  reg_addr = &rtc_register->RTC_OSC32CON2_UNION.VALUE; break;
        default:    {return 0;};
    }
    rtc_internal_reload();
    return *reg_addr;
}


/***********************************************************************************************
 * Public Function
***********************************************************************************************/
void    rtc_internal_reload()
{
    rtc_register->RTC_BBPU = KEY_BBPU_MASK_VAL_RELOAD<<RTC_BBPU_KEY_BBPU_OFFSET;
    rtc_local_trigger_wait();
}

void    rtc_internal_clear_wakeup_status()
{
    rtc_register->RTC_BBPU = (KEY_BBPU_MASK_CLR_WAKEUP<<8) | 0x2;
    rtc_local_trigger_wait();
}

uint32_t    rtc_internal_irq_status()
{
    return rtc_register->RTC_IRQ_STA;
}


<<<<<<< HEAD
=======
uint32_t    rtc_internal_power_reason(uint32_t irq_status)
{
    int ret = 0;
#ifdef RTC_CAPTOUCH_SUPPORTED
    if(irq_status == 0) {
        return RTC_POWERED_BY_CAPTOUCH;
    }
#endif
    if( (irq_status & RTC_IRQ_STA_ALARM_MASK)) {
        ret = RTC_POWERED_BY_ALARM;
    }else if( (irq_status & RTC_IRQ_STA_TICK_MASK ) ) {
        ret = RTC_POWERED_BY_TICK;
    } else if( (irq_status & RTC_IRQ_STA_EINT0_MASK)) {
        ret = RTC_POWERED_BY_EINT0;
    } else if( (irq_status & RTC_IRQ_STA_EINT1_MASK)) {
        ret = RTC_POWERED_BY_EINT1;
    } else if( (irq_status & RTC_IRQ_STA_EINT2_MASK)) {
        ret = RTC_POWERED_BY_EINT2;
    }
    return ret;
}


>>>>>>> db20e11 (second commit)
void    rtc_internal_print_power_reason(uint32_t irq_status)
{
#ifdef RTC_CAPTOUCH_SUPPORTED
    if(irq_status == 0) {
        log_rtc_error("rtc power by captouch!\r\n",0);
        return;
    }
#endif
    if( (irq_status & RTC_IRQ_STA_ALARM_MASK) && (rtc_register->RTC_BBPU & RTC_BBPU_ALARM_PWREN_MASK)) {
        log_rtc_error("rtc power by alarm!\r\n",0);
    }
    if( (irq_status & RTC_IRQ_STA_TICK_MASK ) && (rtc_register->RTC_BBPU & RTC_BBPU_TICK_PWREN_MASK)) {
        log_rtc_error("rtc power by tick!\r\n",0);
    }
    if( (irq_status & RTC_IRQ_STA_EINT0_MASK) && (rtc_register->RTC_BBPU & RTC_BBPU_EINT0_PWREN_MASK)) {
        log_rtc_error("rtc power by eint0!\r\n",0);
    }
    if( (irq_status & RTC_IRQ_STA_EINT1_MASK) && (rtc_register->RTC_BBPU & RTC_BBPU_EINT1_PWREN_MASK)) {
        log_rtc_error("rtc power by eint1!\r\n",0);
    }
    if( (irq_status & RTC_IRQ_STA_EINT2_MASK) && (rtc_register->RTC_BBPU & RTC_BBPU_EINT1_PWREN_MASK)) {
        log_rtc_error("rtc power by eint2!\r\n",0);
    }
}


void    rtc_internal_enable_setting()
{
<<<<<<< HEAD
=======
    rtc_register->RTC_OSC32CON2_UNION.VALUE = RTC_OSC32CON2_MAGIC_KEY_1;
    hal_gpt_delay_us(2);
    rtc_register->RTC_OSC32CON2_UNION.VALUE = RTC_OSC32CON2_MAGIC_KEY_2;
    hal_gpt_delay_us(2);
    rtc_register->RTC_OSC32CON2_UNION.BITS.SETTING_CG = 1;
    rtc_local_wait_done();
>>>>>>> db20e11 (second commit)
}





void    rtc_internal_set_setting_cg_by_user(bool force_ctrl, bool enable)
{
<<<<<<< HEAD
#define     RTC_STATE_IDLE      0
#define     RTC_STATE_KEY1      1
#define     RTC_STATE_KEY2      2
#define     RTC_STATE_RW        3
#ifdef    RTC_DEBUG_TIME_CHECK_ENABLED
    uint32_t        tick0,tick1,tick2,tick3;
#endif
    uint32_t                    status;

    rtc_local_save_irq(&status);
    if(enable != 0){
        g_ref_cnt++;
        if(g_cg_enabled == true && force_ctrl != true) {
            rtc_local_restore_irq(status);
            return;
        }
    } else {
        if(g_ref_cnt != 0) {
            g_ref_cnt--;
        }
        if( (g_ref_cnt != 0 || g_cg_enabled == false) && force_ctrl != true){
            rtc_local_restore_irq(status);
            return;
        }
    }
    rtc_local_restore_irq(status);

retry:
    /*avoid thread configure again*/
    rtc_local_wait_done(); //wait hw free
    if(enable == g_cg_enabled){
        return;
    }

    RTC_DEBUG_GET_1M_TICK(tick0);
    /*block start: state => write key 1 */
    rtc_local_save_irq(&status);
    if(g_wr_state == RTC_STATE_IDLE){
        rtc_register->RTC_OSC32CON2_UNION.VALUE = RTC_OSC32CON2_MAGIC_KEY_1;
        g_wr_state = RTC_STATE_KEY1;
     }
    rtc_local_restore_irq(status);
    rtc_local_wait_done();/*wait hw idle*/
    RTC_DEBUG_GET_1M_TICK(tick1);

    /*block start: state => write key 2 */
    rtc_local_save_irq(&status);
    if(g_wr_state == RTC_STATE_KEY1){

        rtc_register->RTC_OSC32CON2_UNION.VALUE = RTC_OSC32CON2_MAGIC_KEY_2;
        g_wr_state = RTC_STATE_KEY2;
    }
    rtc_local_restore_irq(status);
    rtc_local_wait_done();/*wait hw idle*/
    RTC_DEBUG_GET_1M_TICK(tick2);

    /*block start: state => write data */
    rtc_local_save_irq(&status);
    if(g_wr_state == RTC_STATE_KEY2){
        g_cg_enabled = (enable == 0)?0:1;
        rtc_register->RTC_OSC32CON2_UNION.BITS.SETTING_CG = g_cg_enabled;
        g_wr_state = RTC_STATE_IDLE;
        g_ref_cnt  = (enable == 0)?0:g_ref_cnt;
        rtc_local_restore_irq(status);
        /*wait hw idle*/
        rtc_local_wait_done();
    }else {
        g_wr_state = RTC_STATE_IDLE;
        rtc_local_restore_irq(status);
        goto retry;
    }
    /*block end: state => write data */
    RTC_DEBUG_GET_1M_TICK(tick3);
#ifdef    RTC_DEBUG_TIME_CHECK_ENABLED
    log_rtc_info("[hal][rtc] set OSC32CON0 time key1(%dus),key2(%dus),val(%dus)\r\n", 3, tick1-tick0,tick2-tick1,tick3-tick2);
#endif
=======
    return;
>>>>>>> db20e11 (second commit)
}

void    rtc_internal_set_timer_cg(bool  enable)
{
    uint32_t    value = 0;

    value =rtc_local_get_osc32con(0);
    if (enable) {
        value |= RTC_OSC32CON0_TIME_CG_EN_MASK;
        rtc_local_set_osc32con(0,value);
    } else {
        value &= (~RTC_OSC32CON0_TIME_CG_EN_MASK);
        rtc_local_set_osc32con(0,value);
    }
}

void    rtc_internal_set_cali_cg(bool  enable)
{
    (void)enable;
}


void    rtc_internal_unlock_protect(void)
{
    rtc_register->RTC_PROT = RTC_PROTECT1;
    rtc_local_trigger_wait();

    rtc_register->RTC_PROT = RTC_PROTECT2;
    rtc_local_trigger_wait();
}

void rtc_internal_set_32k_off()
{
    rtc_register->RTC_RESET_CON = 0x0;
}

bool    rtc_internal_enable_tick_notify(hal_rtc_time_notification_period_t type, bool enable)
{
    uint32_t     i = 0, temp = 0;
    uint32_t     mask = 0;

    if(enable == false || type == HAL_RTC_TIME_NOTIFICATION_NONE) {
        mask   = 0;
        enable = false;
    } else {
        temp = sizeof(g_rtc_type_mask_map)/sizeof(rtc_period_type_mask_map_t);
        for(i = 0;i < temp; i++){
            if(g_rtc_type_mask_map[i].type == type){
                mask = g_rtc_type_mask_map[i].mask;
                break;
            }
        }
        mask = (mask | (1<<RTC_CII_EN_TC_EN_OFFSET));
    }
    rtc_register->RTC_CII_EN = mask;
    rtc_local_trigger_wait();
    rtc_local_config_wakeup_source(RTC_WAKEUP_SOURCE_TICK, enable);
    return true;
}


void    rtc_internal_alarm_setting(const hal_rtc_time_t *time, bool enable,uint32_t mask)
{
    if(time != NULL) {
        rtc_register->RTC_AL0 = (time->rtc_sec<<RTC_AL0_AL_SECOND_OFFSET) | (time->rtc_min<<RTC_AL0_AL_MINUTE_OFFSET);
        rtc_register->RTC_AL1 = (time->rtc_hour<<RTC_AL1_AL_HOUR_OFFSET ) | (time->rtc_day<<RTC_AL1_AL_DOM_OFFSET);
        rtc_register->RTC_AL2 = (time->rtc_mon<<RTC_AL2_AL_MONTH_OFFSET ) | (time->rtc_week<<RTC_AL2_AL_DOW_OFFSET);
        rtc_register->RTC_AL3 = (time->rtc_year<<RTC_AL3_AL_YEAR_OFFSET );
        rtc_local_trigger_wait();
    }
    /*enable alarm irq, & unmask tick condition*/
    if(enable) {
        rtc_register->RTC_AL_MASK =  (1<<RTC_AL_MASK_AL_EN_OFFSET) | mask;
    } else {
        rtc_register->RTC_AL_MASK &= ~(1<<RTC_AL_MASK_AL_EN_OFFSET);
    }
    rtc_local_trigger_wait();
    rtc_local_config_wakeup_source(RTC_WAKEUP_SOURCE_ALARM, enable);
}

void    rtc_internal_alarm_get(hal_rtc_time_t *time, bool  *is_enable, uint32_t  *mask)
{
    uint16_t value_al3, value_al2, value_al1, value_al0;

    if(time != NULL) {
        value_al3 = rtc_register->RTC_AL3;
        value_al2 = rtc_register->RTC_AL2;
        value_al1 = rtc_register->RTC_AL1;
        value_al0 = rtc_register->RTC_AL0;
        time->rtc_year = (value_al3 & RTC_AL3_AL_YEAR_MASK)>>RTC_AL3_AL_YEAR_OFFSET;
        time->rtc_milli_sec = 0; // have no meaning, just init
        time->rtc_week = (value_al2 & RTC_AL2_AL_DOW_MASK)>>RTC_AL2_AL_DOW_OFFSET;
        time->rtc_mon  = (value_al2 & RTC_AL2_AL_MONTH_MASK)>>RTC_AL2_AL_MONTH_OFFSET;
        time->rtc_hour = (value_al1 & RTC_AL1_AL_HOUR_MASK)>>RTC_AL1_AL_HOUR_OFFSET;
        time->rtc_day  = (value_al1 & RTC_AL1_AL_DOM_MASK)>>RTC_AL1_AL_DOM_OFFSET;
        time->rtc_min  = (value_al0 & RTC_AL0_AL_MINUTE_MASK)>>RTC_AL0_AL_MINUTE_OFFSET;
        time->rtc_sec  = (value_al0 & RTC_AL0_AL_SECOND_MASK)>>RTC_AL0_AL_SECOND_OFFSET;
    }
    if(is_enable != NULL) {
        if(rtc_register->RTC_AL_MASK & ((1<<RTC_AL_MASK_AL_EN_OFFSET))) {
            *is_enable = true;
        } else {
            *is_enable = false;
        }
    }
    if(mask != NULL)
        *mask = rtc_register->RTC_AL_MASK & RTC_AL_MASK_AL_MASK_MASK;
}


void    rtc_internal_set_date_time(const hal_rtc_time_t *time)
{
    rtc_register->RTC_TC0 = (time->rtc_min << RTC_TC0_MINUTE_OFFSET) | (time->rtc_sec<<RTC_TC0_SECOND_OFFSET);
    rtc_register->RTC_TC1 = (time->rtc_day << RTC_TC1_DOM_OFFSET)    | (time->rtc_hour<<RTC_TC1_HOUR_OFFSET);
    // day-of-week range is 1~7, header file is 0~6
    rtc_register->RTC_TC2 = (time->rtc_mon << RTC_TC2_MONTH_OFFSET)  | ((time->rtc_week + 1)<<RTC_TC2_DOW_OFFSET);
    rtc_register->RTC_TC3 = time->rtc_year<<RTC_TC3_YEAR_OFFSET;
    rtc_register->RTC_INT_CNT = time->rtc_milli_sec;
    rtc_local_trigger_wait();
}


void    rtc_internal_get_date_time( hal_rtc_time_t *time)
{
    uint16_t    value_tc2  = 0;
    uint16_t    value_tc1  = 0;
    uint16_t    value_tc0  = 0;
    uint16_t    int_cnt    = 0;
    uint16_t    int_cnt_pre= 0;
    /* re-read time information if internal millisecond counter has carried */
    do {
        int_cnt_pre     = rtc_register->RTC_INT_CNT;
        time->rtc_year  = rtc_register->RTC_TC3;
        value_tc2       = rtc_register->RTC_TC2;
        value_tc1       = rtc_register->RTC_TC1;
        value_tc0       = rtc_register->RTC_TC0;
        int_cnt         = rtc_register->RTC_INT_CNT;
    } while (int_cnt < int_cnt_pre);
    time->rtc_week  = ((value_tc2 & RTC_TC2_DOW_MASK) - 1)>>RTC_TC2_DOW_OFFSET;
    time->rtc_mon   = (value_tc2  & RTC_TC2_MONTH_MASK)>>RTC_TC2_MONTH_OFFSET;
    time->rtc_hour  = (value_tc1  & RTC_TC1_HOUR_MASK)>>RTC_TC1_HOUR_OFFSET;
    time->rtc_day   = (value_tc1  & RTC_TC1_DOM_MASK)>>RTC_TC1_DOM_OFFSET;
    time->rtc_min   = (value_tc0  & RTC_TC0_MINUTE_MASK)>>RTC_TC0_MINUTE_OFFSET;
    time->rtc_sec   = (value_tc0  & RTC_TC0_SECOND_MASK)>>RTC_TC0_SECOND_OFFSET;
    time->rtc_milli_sec = int_cnt;
}


bool    rtc_internal_powerkey_is_valid()
{
  uint32_t mask = (RTC_WRTGR_PWR_STA_MASK | RTC_WRTGR_CLK_STA_MASK);
    return (rtc_register->RTC_WRTGR & mask) == mask? true:false;
}

bool    rtc_internal_is_powered_by_rtc()
{
  return  ((rtc_register->RTC_BBPU & RTC_BBPU_RTC_WAKEUP_MASK) != 0)? true:false;
}

void    rtc_internal_set_power_key()
{
    uint32_t   value;
    uint32_t   mask;

    mask = (RTC_WRTGR_PWR_STA_MASK | RTC_WRTGR_CLK_STA_MASK);
    /*disable lpd,->set pwrkey -> enable lpd */
    /* setting default OSCCON RG*/
    rtc_local_set_osc32con(0,0x101);
    //rtc_local_set_osc32con1(0xf07);
    rtc_local_set_osc32con(2,0x103);
    /*disable lpd*/
    rtc_register->RTC_LPD_CON = 0x100;
    rtc_local_trigger_wait();
    rtc_register->RTC_LPD_CON = 0x101;
    rtc_local_trigger_wait();

    /*set eosc step pwd and lpd*/
    value  = rtc_local_get_osc32con(1);
    value |= RTC_OSC32CON1_EOSC_STP_PWD_MASK;
    rtc_local_set_osc32con(1,value);
    rtc_local_lowpower_detect();

    /*reset clock status*/
    rtc_register->RTC_RESET_CON = 0x101;
    rtc_local_trigger_wait();
    rtc_register->RTC_RESET_CON = 0x201;
    rtc_local_trigger_wait();

    hal_gpt_delay_us(200);
    mask = (RTC_WRTGR_PWR_STA_MASK | RTC_WRTGR_CLK_STA_MASK);
    if ((rtc_register->RTC_WRTGR & mask) != mask) {
        log_rtc_error("rtc_set_power_key fail : rtc_wrtgr = %x!\r\n", 1,(unsigned int) rtc_register->RTC_WRTGR);
        //rtc_internal_dump(NULL);
    }
}


uint32_t    rtc_internal_measure_frequency(rtc_osc32k_mode_t mode)
{
    #define     RTC_XO_CK_FREQ      26000000
    uint32_t    frequency   = 0;
    uint32_t    fqmtr_data  = 0;

    fqmtr_data = rtc_internal_measure_counter(mode, RTC_FQMTR_TCKSEL_XO_CK, 99);
    if(fqmtr_data != 0) {
        frequency  = (uint32_t)AIR_RTC_FCXO_CLOCK * (99 + 1) / fqmtr_data;
    }

    return frequency;
}

uint32_t    rtc_local_abs(int  value)
{
    uint32_t    temp =value ;

    if(value < 0){
        temp = 0 - value;
    }
    return temp;
}


<<<<<<< HEAD

=======
>>>>>>> db20e11 (second commit)
uint32_t rtc_internal_get_eosc32_calibration()
{
    uint32_t    value          = 0;
    uint8_t     eosc_cali      = 0;
    int         low_xosccali   = 0x00;
    int         high_xosccali  = 0x1f;
    uint32_t    medium_xosccali= 0;
    uint32_t    low_frequency  = 0;
    uint32_t    high_frequency = 0;
    uint32_t    medium_frequency;

    value = rtc_local_get_osc32con(1);
    value &= ~(RTC_OSC32CON1_XOSC_CALI_MASK);
    value |= ((low_xosccali<<RTC_OSC32CON1_XOSC_CALI_OFFSET) | RTC_OSC32CON1_EOSC_STP_PWD_MASK);
    rtc_local_set_osc32con(1, value);

    high_frequency = rtc_internal_measure_frequency(RTC_OSC32K_EOSC_MODE);
    if (high_frequency <= RTC_EOSC_CALI_FREQUENCY) {
        log_rtc_error("high_frequency <= %d, frequency = %u, xosccali = %d", 3, RTC_EOSC_CALI_FREQUENCY, (unsigned int)high_frequency, (unsigned int)low_xosccali);
        goto error;
    }

    value &= ~RTC_OSC32CON1_XOSC_CALI_MASK;
    value |= (high_xosccali << RTC_OSC32CON1_XOSC_CALI_OFFSET);
    rtc_local_set_osc32con(1, value);
    low_frequency = rtc_internal_measure_frequency(RTC_OSC32K_EOSC_MODE);
    if (low_frequency >= RTC_EOSC_CALI_FREQUENCY) {
        log_rtc_error("low_frequency >= %d, frequency = %u, xosccali = %d\n", 3, RTC_EOSC_CALI_FREQUENCY, (unsigned int)low_frequency, (unsigned int)high_xosccali);
        goto error;
    }

    while (rtc_local_abs(high_xosccali - low_xosccali) > 1) {
        medium_xosccali = (low_xosccali + high_xosccali) / 2;
        value &= ~RTC_OSC32CON1_XOSC_CALI_MASK;
        value |= (medium_xosccali << RTC_OSC32CON1_XOSC_CALI_OFFSET);
        rtc_local_set_osc32con(1,value);
        medium_frequency = rtc_internal_measure_frequency(RTC_OSC32K_EOSC_MODE);
        log_rtc_info("eosc cali(%d) -> frequency(%d)", 2,(int) medium_xosccali,(int)medium_frequency);
        if (medium_frequency > RTC_EOSC_CALI_FREQUENCY) {
            low_xosccali = medium_xosccali;
            high_frequency = medium_frequency;
        } else if (medium_frequency < RTC_EOSC_CALI_FREQUENCY) {
            high_xosccali = medium_xosccali;
            low_frequency = medium_frequency;
        } else {
            log_rtc_info("rtc eosc cali value = %d", 1,(int) medium_xosccali);
            eosc_cali = medium_xosccali;
            goto error;
        }
    }

    /*get more accuratly value*/
    if ((RTC_EOSC_CALI_FREQUENCY - low_frequency) < (high_frequency - RTC_EOSC_CALI_FREQUENCY)) {
        medium_xosccali = (high_xosccali);
    } else {
        medium_xosccali = (low_xosccali);
    }

    value &= ~RTC_OSC32CON1_XOSC_CALI_MASK;
    value |= (medium_xosccali << RTC_OSC32CON1_XOSC_CALI_OFFSET);
    rtc_local_set_osc32con(1,value);
    eosc_cali = (medium_xosccali);
    log_rtc_warn("rtc eosc cali value = %d\r\n", 1, (unsigned int)medium_xosccali);

error:
    return eosc_cali;
}

void    rtc_internal_set_eosc32_calibration(uint32_t eosc_cali)
{
    uint32_t value;

    value = rtc_local_get_osc32con(1);
    value &= ~RTC_OSC32CON1_XOSC_CALI_MASK;
    value |= ((eosc_cali << RTC_OSC32CON1_XOSC_CALI_OFFSET) & RTC_OSC32CON1_XOSC_CALI_MASK);
    rtc_local_set_osc32con(1,value);
}



int    rtc_internal_calc_time_calibration(rtc_osc32k_mode_t mode)
{
    uint32_t    actual_freq = 0;
    int         cali        = 0;

    actual_freq = rtc_internal_measure_frequency(mode);
    /* Set RTC_CALI */
    cali = (32768 - actual_freq);
    log_rtc_warn("rtc time cali value %d(0x%x) = (32768 - %d)",3,cali, cali, (unsigned int)actual_freq);

    if (cali > 4095) {
        cali = 4095;
        log_rtc_error("rtc cali is too big\r\n", 0);
    } else if (cali < (-4096)) {
        cali = -4096;
        log_rtc_error("rtc cali is too small\r\n", 0);
    }
    return cali;
}

void    rtc_internal_set_time_calibration(rtc_time_cali_mode_t  mode, uint32_t  cali)
{
    if(mode == RTC_TIME_CALI_K_EOSC) {
        rtc_register->RTC_CALI = (cali & RTC_CALI_VALUE_MASK) | (1<<RTC_CALI_RW_SEL_OFFSET);
    } else {
        rtc_register->RTC_CALI = ((cali<<3) & RTC_CALI_VALUE_MASK);
    }
    rtc_local_trigger_wait();
}

void    rtc_internal_clear_pwr_status()
{
    rtc_register->RTC_RESET_CON = 0x101;
    rtc_local_trigger_wait();
}


uint32_t    rtc_internal_get_time_calibration()
{
    return (rtc_register->RTC_CALI & RTC_CALI_VALUE_MASK);
}


<<<<<<< HEAD
=======
bool    rtc_internal_check_clock_source(hal_rtc_osc32k_mode_t mode)
{
    uint32_t                temp = 0;
    hal_rtc_osc32k_mode_t   sys_mode;

    rtc_register->RTC_DEBUG_CON_0 = RTC_DEBUG_CON0_DBG_EN_MASK | (8<<RTC_DEBUG_CON0_DBG_SEL_OFFSET);
    hal_gpt_delay_us(2);
    temp = rtc_register->RTC_DEBUG_CON_1;
    temp = (temp>>8) & 0x3;
    switch(temp){
        case 0:
        case 2:     sys_mode = HAL_RTC_OSC32K_DCXO_MODE; break;
        case 1:     sys_mode = HAL_RTC_OSC32K_EOSC_MODE; break;
        case 3:     sys_mode = HAL_RTC_OSC32K_XOSC_MODE; break;
    }
    if(mode == sys_mode){
        return true;
    } else {
        return false;
    }
}


void    rtc_internal_debug_code()
{
    uint32_t temp = 0;

    /*output EOSC-32k to GPIO*/
    temp = 0;
    rtc_register->RTC_DEBUG_CON_0 = RTC_DEBUG_CON0_DBG_EN_MASK | (temp<<RTC_DEBUG_CON0_DBG_SEL_OFFSET);
    hal_gpt_delay_us(2);
    log_rtc_warn("rtc debug select:%d, out: 0x%x\r\n",2,  temp, rtc_register->RTC_DEBUG_CON_1);

    temp = 1;
    rtc_register->RTC_DEBUG_CON_0 = RTC_DEBUG_CON0_DBG_EN_MASK | (temp<<RTC_DEBUG_CON0_DBG_SEL_OFFSET);
    hal_gpt_delay_us(2);
    log_rtc_warn("rtc debug select:%d, out: 0x%x\r\n",2, temp, rtc_register->RTC_DEBUG_CON_1);

    temp = 8;
    rtc_register->RTC_DEBUG_CON_0 = RTC_DEBUG_CON0_DBG_EN_MASK | (temp<<RTC_DEBUG_CON0_DBG_SEL_OFFSET);
    hal_gpt_delay_us(2);
    log_rtc_warn("rtc debug select:%d, out: 0x%x\r\n",2, temp, rtc_register->RTC_DEBUG_CON_1);
}




>>>>>>> db20e11 (second commit)
void    rtc_internal_set_osc32_mode(rtc_osc32k_mode_t mode)
{
    uint32_t    temp = 0;

    switch(mode) {
        case RTC_OSC32K_DCXO_MODE:{
            /*setup eosc*/
            temp = rtc_local_get_osc32con(1);
            temp &= ~(RTC_OSC32CON1_AMPCRL_EN_MASK | RTC_OSC32CON1_AMP_GSEL_MASK);
            temp |= (RTC_OSC32CON1_EOSC_STP_PWD_MASK | RTC_OSC32CON1_EOSC_CHOP_EN_MASK | RTC_OSC32CON1_EOSC_VCT_EN_MASK);
            rtc_local_set_osc32con(1,temp);
            /*swtich clock*/
            temp  = rtc_local_get_osc32con(0);
            temp &= ~RTC_OSC32CON0_32K_SEL_MASK;
<<<<<<< HEAD
            temp |= 0x2;
            rtc_local_set_osc32con(0,temp);
            hal_gpt_delay_us(70);
=======
            //temp |= 0x0;
            rtc_local_set_osc32con(0,temp);
            hal_gpt_delay_us(160);
>>>>>>> db20e11 (second commit)
            /*power down XOSC*/
            temp =  rtc_local_get_osc32con(2);
            temp &= ~(RTC_OSC32CON2_EOSC_PWDB_MASK | RTC_OSC32CON2_XOSC_PWDB_MASK);
            temp |= RTC_OSC32CON2_EOSC_PWDB_MASK;
            rtc_local_set_osc32con(2,temp);
        }break;

        case RTC_OSC32K_EOSC_MODE: {
            /*setup eosc*/
            temp = rtc_local_get_osc32con(1);
            temp &= ~(RTC_OSC32CON1_AMPCRL_EN_MASK | RTC_OSC32CON1_AMP_GSEL_MASK);
            temp |= (RTC_OSC32CON1_EOSC_STP_PWD_MASK | RTC_OSC32CON1_EOSC_CHOP_EN_MASK | RTC_OSC32CON1_EOSC_VCT_EN_MASK);
            rtc_local_set_osc32con(1,temp);
            /*swtich clock*/
            temp  = rtc_local_get_osc32con(0);
            temp &= ~RTC_OSC32CON0_32K_SEL_MASK;
            temp |= 0x1;
            rtc_local_set_osc32con(0,temp);
<<<<<<< HEAD
            hal_gpt_delay_us(70);
=======
            hal_gpt_delay_us(160);
>>>>>>> db20e11 (second commit)
            /*power down XOSC*/
            temp =  rtc_local_get_osc32con(2);
            temp &= ~(RTC_OSC32CON2_EOSC_PWDB_MASK | RTC_OSC32CON2_XOSC_PWDB_MASK);
            temp |= RTC_OSC32CON2_EOSC_PWDB_MASK;
            rtc_local_set_osc32con(2,temp);
        }break;

        case RTC_OSC32K_XOSC_MODE: {
            /*setup xosc*/
            temp = rtc_local_get_osc32con(1);
            temp |= (RTC_OSC32CON1_AMPCRL_EN_MASK | RTC_OSC32CON1_AMP_GSEL_MASK | RTC_OSC32CON1_EOSC_STP_PWD_MASK);
            rtc_local_set_osc32con(1, temp);
            /*swtich clock*/
            temp  = rtc_local_get_osc32con(0);
            temp &= ~RTC_OSC32CON0_32K_SEL_MASK;
            temp |= 0x3;
            rtc_local_set_osc32con(0, temp);
<<<<<<< HEAD
            hal_gpt_delay_us(70);
=======
            hal_gpt_delay_us(160);
>>>>>>> db20e11 (second commit)
            /*power down eosc*/
            temp =  rtc_local_get_osc32con(2);
            temp &= ~(RTC_OSC32CON2_EOSC_PWDB_MASK | RTC_OSC32CON2_XOSC_PWDB_MASK);
            temp |=  (RTC_OSC32CON2_XOSC_PWDB_MASK);// | RTC_OSC32CON2_EOSC_PWDB_MASK);
            rtc_local_set_osc32con(2, temp);
        }break;
        default:    break;
    }
}

void    rtc_internal_set_onetime_cali(uint32_t cali)
{
    rtc_register->RTC_DIFF = cali;
    rtc_local_trigger_wait();
}

uint32_t    rtc_internal_get_onetime_cali()
{
    rtc_internal_reload();
    return rtc_register->RTC_DIFF;
}




void    rtc_internal_set_spar_rg(uint16_t value)
{
    rtc_register->RTC_SPAR_REG = value;
    rtc_local_trigger_wait();
}

uint16_t    rtc_internal_get_spar_rg()
{
    return rtc_register->RTC_SPAR_REG;
}



uint32_t    rtc_local_get_eintcon(uint8_t rg_id)
{
    uint32_t tmp0 = 0,tmp1 = 0;

    if( rtc_local_is_eco_done()) {
        if(rg_id == 0) {
            tmp1 = rtc_register->RTC_EINT_CON0;
        }else if (rg_id == 1){
            tmp1 = rtc_register->RTC_EINT_CON1;
        }
    }else {
        rtc_internal_reload();
        if(rg_id == 0) {
            tmp0 = rtc_register->RTC_EINT_CON0;
        }else if (rg_id == 1){
            tmp0 = rtc_register->RTC_EINT_CON1;
        }
        tmp1 = (tmp0&0xFF)<<8 | (tmp0&0xFF00)>>8;
    }
    return tmp1;
}


hal_rtc_status_t rtc_internal_eint_setting(hal_rtc_eint_config_t *eint_config)
{
    uint32_t        value = 0x0;
    volatile uint32_t *eint_rg = NULL;
    uint32_t       offset;
    uint32_t       eint_val = 0;
    uint8_t        wake_no;

    if (eint_config == NULL) {
        return HAL_RTC_STATUS_INVALID_PARAM;
    }
    switch (eint_config->rtc_gpio) {
        case HAL_RTC_GPIO_0:
            eint_rg = &(rtc_register->RTC_EINT_CON0);
            offset  = RTC_EINT_CON0_0_START_OFFSET;
            wake_no = RTC_WAKEUP_SOURCE_EINT0;
        break;
        case HAL_RTC_GPIO_1:
            eint_rg = &(rtc_register->RTC_EINT_CON0);
            offset  = RTC_EINT_CON0_1_START_OFFSET;
            wake_no = RTC_WAKEUP_SOURCE_EINT1;
        break;
        case HAL_RTC_GPIO_2:
            eint_rg = &(rtc_register->RTC_EINT_CON1);
            offset  = RTC_EINT_CON1_2_START_OFFSET;
            wake_no = RTC_WAKEUP_SOURCE_EINT2;
        break;
        default: {
            return HAL_RTC_STATUS_INVALID_PARAM;
        };
    }
    rtc_internal_reload();
    value = *eint_rg;
    if(rtc_local_is_eco_done()) {
        eint_val = value;
    } else {
        eint_val = (value&0xFF)<<8 | (value&0xFF00)>>8;
    }
    if(eint_config->is_enable_rtc_eint == true){
        uint32_t   tmp;

        tmp      = (0x3F<<offset);
        eint_val = eint_val & (~tmp);
        *eint_rg = eint_val;
        rtc_local_trigger_wait();
        value = RTC_EINT_SYN_EN_MASK;
        if (eint_config->is_falling_edge_active == true) {
            value |= RTC_EINT_INV_EN_MASK;
        }
        if (eint_config->is_enable_debounce == true) {
            value |= RTC_EINT_DEB_EN_MASK;
        }
        value   |= (RTC_EINT_EINT_EN_MASK | RTC_EINT_IRQ_EN_MASK);
        tmp      = eint_val;
        tmp     |=(value<<offset);
        *eint_rg = tmp;
        rtc_local_trigger_wait();
        rtc_local_config_wakeup_source(wake_no, true);
    } else {
        uint32_t tmp1 = 0;
        tmp1     = (0x3F<<offset);
        eint_val = eint_val&(~tmp1);
        *eint_rg = eint_val; /*disable eint*/
        rtc_local_trigger_wait();
    }
    return HAL_RTC_STATUS_OK;
}



#ifdef RTC_GPIO_SUPPORTED
void rtc_internal_gpio_init(hal_rtc_gpio_t  rtc_gpio, uint32_t value)
{
    switch(rtc_gpio)
    {
        case HAL_RTC_GPIO_0: rtc_register->RTC_GPIO0_CON = value; break;
        case HAL_RTC_GPIO_1: rtc_register->RTC_GPIO1_CON = value; break;
        case HAL_RTC_GPIO_2: rtc_register->RTC_GPIO2_CON = value; break;
        default:    break;
    }
    rtc_local_trigger_wait();
}


hal_rtc_status_t    rtc_internal_gpio_control(hal_rtc_gpio_t  rtc_gpio, bool is_output, bool *level)
{
    uint32_t             temp;
    uint32_t    rtc_out_offset[] = {RTC_GPIO_CON_0_OUT_OFFSET,    \
                                    RTC_GPIO_CON_1_OUT_OFFSET,    \
                                    RTC_GPIO_CON_2_OUT_OFFSET};
    uint32_t    rtc_in_offset[] =  {RTC_GPIO_CON_0_IN_OFFSET,     \
                                    RTC_GPIO_CON_1_IN_OFFSET,     \
                                    RTC_GPIO_CON_2_IN_OFFSET};
    if(rtc_gpio >= HAL_RTC_GPIO_MAX){
        return HAL_RTC_STATUS_OK;
    }
<<<<<<< HEAD
=======
    rtc_internal_reload();
>>>>>>> db20e11 (second commit)
    if(is_output == true) {
        if(*level == 0){
            temp = ~(1<<rtc_out_offset[rtc_gpio]);
            rtc_register->RTC_GPIO_CON &= temp;
        } else {
            temp = (1<<rtc_out_offset[rtc_gpio]);
            rtc_register->RTC_GPIO_CON |= temp;
        }
<<<<<<< HEAD
    } else {
        rtc_internal_reload();
=======
        rtc_local_trigger_wait();
    } else {
>>>>>>> db20e11 (second commit)
        temp = (1<<rtc_in_offset[rtc_gpio]);
        if(rtc_register->RTC_GPIO_CON & temp){
            *level = true;
        } else {
            *level = false;
        }
    }
    return HAL_RTC_STATUS_OK;
}

#endif


hal_rtc_status_t    rtc_internal_set_data(uint16_t idx, uint8_t data)
{
    uint8_t              sps_sz = 0;
    uint8_t              offset = 0;
    volatile uint32_t   *base_reg;
    uint32_t             temp = 0;
    hal_rtc_status_t     status = HAL_RTC_STATUS_ERROR;
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    uint32_t        tick0,tick1;
    RTC_DEBUG_GET_1M_TICK(tick0);
#endif
    sps_sz   = RTC_SPARE_RG_NUM * RTC_SPARE_RG_BYTE;
    if(idx < sps_sz) {
        //rtc_internal_reload();
        base_reg = &(rtc_register->RTC_SPAR0);
        offset   = idx/RTC_SPARE_RG_BYTE;
        idx      = idx%RTC_SPARE_RG_BYTE;
        idx      = (idx*8);
        temp     = base_reg[offset];
        temp    &= ~(0xFF<<idx);
        temp    |= (data<<idx);
        base_reg[offset] = temp;
        rtc_local_trigger_wait();
        status = HAL_RTC_STATUS_OK;
    } else {
        status = HAL_RTC_STATUS_ERROR;
    }
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    RTC_DEBUG_GET_1M_TICK(tick1);
    log_rtc_info("[hal][rtc] set retention rg spend %d us\r\n", 1, tick1-tick0);
#endif
    return status;
}


hal_rtc_status_t    rtc_internal_get_data(uint16_t idx, uint8_t *data)
{
    uint8_t              sps_sz = 0;
    uint8_t              offset = 0;
    volatile uint32_t   *base_reg;
    hal_rtc_status_t     status = HAL_RTC_STATUS_ERROR;

#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    uint32_t        tick0,tick1;
    RTC_DEBUG_GET_1M_TICK(tick0);
#endif

    sps_sz   = RTC_SPARE_RG_NUM * RTC_SPARE_RG_BYTE;
    if (idx < sps_sz) {
        base_reg = &(rtc_register->RTC_SPAR0);
        offset   = idx/RTC_SPARE_RG_BYTE;
        idx      = idx%RTC_SPARE_RG_BYTE;
        idx      = idx*8;
        *data    = base_reg[offset]>>(idx);
        status = HAL_RTC_STATUS_OK;
    }
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    RTC_DEBUG_GET_1M_TICK(tick1);
    log_rtc_info("[hal][rtc] get retention rg spend %d us\r\n", 1, tick1-tick0);
#endif
    return status;
}


hal_rtc_status_t    rtc_internal_set_retention_reg(uint32_t offset, uint8_t *buff, uint32_t size)
{
    uint8_t              i      = 0;
    uint8_t              sps_sz = 0;
    uint8_t              offs   = 0;
    uint32_t             temp   = 0;
    uint32_t             idx    = 0;
    volatile uint32_t   *base_reg;
    hal_rtc_status_t     status ;
    uint32_t             reg_mem[12];
    uint8_t              sta_reg_id;
    uint8_t              sto_reg_id;
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    uint32_t             tick0,tick1;
    RTC_DEBUG_GET_1M_TICK(tick0);
#endif
    sta_reg_id = (offset/RTC_SPARE_RG_BYTE);
    sto_reg_id = (offset + size)/RTC_SPARE_RG_BYTE;
    base_reg   = &(rtc_register->RTC_SPAR0);

    for(i=sta_reg_id; i < (sto_reg_id + 1);i++){
        reg_mem[i] = (uint16_t) base_reg[i];
    }

    sps_sz   = RTC_SPARE_RG_NUM * RTC_SPARE_RG_BYTE;
    for(i = 0;i < size; i++){
        idx = offset + i;
        if(idx < sps_sz) {
            offs     = idx/RTC_SPARE_RG_BYTE;
            idx      = idx%RTC_SPARE_RG_BYTE;
            idx      = (idx*8);
            temp     = reg_mem[offs];
            temp    &= ~(0xFF<<idx);
            temp    |= (buff[i]<<idx);
            reg_mem[offs] = temp;
            status = HAL_RTC_STATUS_OK;
        } else {
            status = HAL_RTC_STATUS_ERROR;
            break;
        }
    }
    for(i = sta_reg_id; i < (sto_reg_id + 1); i++){
       base_reg[i] = (uint32_t) reg_mem[i];
    }
    rtc_local_trigger_wait();
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    RTC_DEBUG_GET_1M_TICK(tick1);
    log_rtc_info("[hal][rtc] write retention rg spend %d us\r\n", 1, tick1-tick0);
#endif
    return status;
}


hal_rtc_status_t    rtc_internal_get_retention_reg(uint32_t offset, uint8_t *buff, uint32_t size)
{
    uint8_t              i      = 0;
    uint8_t              idx    = 0;
    uint8_t              sps_sz = 0;
    uint8_t              offs   = 0;
    volatile uint32_t   *base_reg;
    hal_rtc_status_t     status = HAL_RTC_STATUS_ERROR;
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    uint32_t        tick0,tick1;
    RTC_DEBUG_GET_1M_TICK(tick0);
#endif
    sps_sz   = RTC_SPARE_RG_NUM * RTC_SPARE_RG_BYTE;
    for(i = 0;i < size; i++){
        idx = offset + i;
        if (idx < sps_sz) {
            base_reg = &(rtc_register->RTC_SPAR0);
            offs     = idx/RTC_SPARE_RG_BYTE;
            idx      = idx%RTC_SPARE_RG_BYTE;
            idx      = idx*8;
            buff[i]  = base_reg[offs]>>(idx);
            status = HAL_RTC_STATUS_OK;
        }else{
            status = HAL_RTC_STATUS_ERROR;
            break;
        }
    }
#ifdef RTC_DEBUG_TIME_CHECK_ENABLED
    RTC_DEBUG_GET_1M_TICK(tick1);
    log_rtc_info("[hal][rtc] read retention rg spend %d us\r\n", 1, tick1-tick0);
#endif
    return status;
}



#ifdef RTC_SRAM_SUPPORTED
static  void   rtc_local_write_sram_con(uint8_t rg_id, uint32_t value)
{
    switch(rg_id){
        case 0: {
            rtc_register->RTC_SRAM_CON0 = RTC_SRAM_CON0_MAGIC_KEY_1;
            rtc_local_wait_done();
            rtc_register->RTC_SRAM_CON0 = RTC_SRAM_CON0_MAGIC_KEY_2;
            rtc_local_wait_done();
            rtc_register->RTC_SRAM_CON0 = value;
            rtc_local_wait_done();
        } break;
        case 1: {
            rtc_register->RTC_SRAM_CON1 = RTC_SRAM_CON1_MAGIC_KEY_1;
            rtc_local_wait_done();
            rtc_register->RTC_SRAM_CON1 = RTC_SRAM_CON1_MAGIC_KEY_2;
            rtc_local_wait_done();
            rtc_register->RTC_SRAM_CON1 = value;
            rtc_local_wait_done();
        } break;

        default:    break;
    }
}

void    rtc_internal_sram_setting_cg(bool   disable_ck, bool disable_do)
{
    uint32_t  temp = 0;

    rtc_internal_reload();
    temp = rtc_register->RTC_SRAM_CON0;
    if(disable_ck) {
        temp |= RTC_SRAM_CON0_IOS_EN_CKCS_MASK;
    } else {
        temp &= ~RTC_SRAM_CON0_IOS_EN_CKCS_MASK;
    }

    if(disable_do) {
        temp |= RTC_SRAM_CON0_IOS_EN_DO_MASK;
    } else {
        temp &= ~RTC_SRAM_CON0_IOS_EN_DO_MASK;
    }
    rtc_local_write_sram_con(0, temp);
}

void    rtc_internal_sram_setting(hal_rtc_sram_cell_t cell, hal_rtc_sram_mode_t mode)
{
    uint8_t   select_cell = (1<<cell);
    uint32_t  val_con0 = 0,val_con1 = 0;

    rtc_internal_reload();
    val_con1 = rtc_register->RTC_SRAM_CON1;
    val_con0 = rtc_register->RTC_SRAM_CON0;
    switch(mode){
        case HAL_RTC_SRAM_NORMAL_MODE:{
            val_con1 &= ~(select_cell<<RTC_SRAM_CON1_PD_OFFSET);
            val_con1 |=  (select_cell<<RTC_SRAM_CON1_SLEEP_OFFSET);
            rtc_local_write_sram_con(1, val_con1);
            hal_gpt_delay_us(1);
            val_con0 |=  (select_cell<<RTC_SRAM_CON0_ISO_INTB_OFFSET);
            rtc_local_write_sram_con(0, val_con0);
        }break;

        case HAL_RTC_SRAM_SLEEP_MODE:{
            hal_gpt_delay_us(1);
            val_con0 &= ~(select_cell<<RTC_SRAM_CON0_ISO_INTB_OFFSET);
            rtc_local_write_sram_con(0, val_con0);
            hal_gpt_delay_us(2);
            val_con1 &= ~(select_cell<<RTC_SRAM_CON1_SLEEP_OFFSET);
            val_con1 &= ~(select_cell<<RTC_SRAM_CON1_PD_OFFSET);
            rtc_local_write_sram_con(1, val_con1);
        }break;

        case HAL_RTC_SRAM_PD_MODE:{
            val_con0 |=  (select_cell<<RTC_SRAM_CON0_ISO_INTB_OFFSET);
            val_con1 |=  (select_cell<<RTC_SRAM_CON1_SLEEP_OFFSET);
            val_con1 |=  (select_cell<<RTC_SRAM_CON1_PD_OFFSET);
            rtc_local_write_sram_con(0, val_con0);
            rtc_local_write_sram_con(1, val_con1);
        }break;
    }
}
#endif

void    rtc_internal_init_register()
{
    rtc_local_set_osc32con(0, 0x701);
    rtc_local_set_osc32con(1, 0xf97);
    rtc_local_set_osc32con(2, 0x103);
    /* Clear BBPU */
    rtc_register->RTC_BBPU = KEY_BBPU_MASK_ALL<<8;
    rtc_register->RTC_IRQ_EN = 0x0;
    rtc_register->RTC_CII_EN = 0x0;
    rtc_register->RTC_AL_MASK = 0x0;
    rtc_register->RTC_TC0 = 0x0;
    rtc_register->RTC_TC1 = 0x100;
    rtc_register->RTC_TC2 = 0x101;
    rtc_register->RTC_TC3 = 0x0;
    rtc_register->RTC_AL0 = 0x000;
    rtc_register->RTC_AL1 = 0x100;
    rtc_register->RTC_AL2 = 0x101;
    rtc_register->RTC_AL3 = 0x0;
    rtc_register->RTC_LPD_CON   = 0x100;
    rtc_local_trigger_wait();

    rtc_register->RTC_SPAR0 = 0x0;
    rtc_register->RTC_SPAR1 = 0x0;
    rtc_register->RTC_SPAR2 = 0x0;
    rtc_register->RTC_SPAR3 = 0x0;
    rtc_register->RTC_SPAR4 = 0x0;
    rtc_register->RTC_SPAR5 = 0x0;
    //rtc_register->RTC_SPAR_REG = 0x0;
    rtc_register->RTC_DIFF  = 0x0;
    rtc_register->RTC_CALI  = 0x0;
#ifdef RTC_CAPTOUCH_SUPPORTED
    rtc_register->RTC_CAP_CON   = 0x0;
    rtc_register->RTC_RESET_CON = 1;
#endif
    rtc_register->RTC_EINT_CON0 = 0;
    rtc_register->RTC_EINT_CON1 = 0;

    rtc_local_trigger_wait();

#ifdef RTC_GPIO_SUPPORTED
{
    uint32_t temp;

    rtc_register->RTC_GPIO0_CON = 0x02; //rtc_gpio0 for captouch
    temp = (1<<RTC_GPIO_PD_OFFSET) | (1<<RTC_GPIO_G_OFFSET);
    rtc_register->RTC_GPIO1_CON = temp;
    rtc_register->RTC_GPIO2_CON = temp; //E1:input mode, pull down
    rtc_local_trigger_wait();
}
#endif
}


void        rtc_internal_set_ldocon(uint32_t reg_id, uint32_t val)
{
    volatile uint32_t *reg_addr = NULL;
    uint32_t key_1[] = { RTC_LDOCON0_MAGIC_KEY_1, \
                         RTC_LDOCON1_MAGIC_KEY_1, \
                         RTC_LDOCON2_MAGIC_KEY_1  \
                       };
    uint32_t key_2[] = { RTC_LDOCON0_MAGIC_KEY_2, \
                         RTC_LDOCON1_MAGIC_KEY_2, \
                         RTC_LDOCON2_MAGIC_KEY_2  \
                       };
    switch(reg_id){
        case 0: reg_addr = &rtc_register->RTC_LDOCON0;break;
        case 1: reg_addr = &rtc_register->RTC_LDOCON1;break;
        case 2: reg_addr = &rtc_register->RTC_LDOCON2;break;
        default:{
            return;
        }
    }
    *reg_addr = key_1[reg_id];
    rtc_local_wait_done();
    *reg_addr = key_2[reg_id];
    rtc_local_wait_done();
    *reg_addr = val;
    rtc_local_trigger_wait();
}

uint32_t    rtc_internal_get_ldocon(uint32_t reg_id)
{
    volatile uint32_t *reg_addr = NULL;

    rtc_internal_reload();
    switch(reg_id){
        case 0: reg_addr = &rtc_register->RTC_LDOCON0;break;
        case 1: reg_addr = &rtc_register->RTC_LDOCON1;break;
        case 2: {
            rtc_register->RTC_DEBUG_CON_0 = 0x104;
            hal_gpt_delay_us(1);
            return rtc_register->RTC_DEBUG_CON_1;
        }break;
        default:{
            return 0;
        }
    }
    return *reg_addr;
}


hal_rtc_status_t    rtc_internal_set_ldo_lowpower(bool is_lp)
{
    uint32_t    temp0  = 0;

    temp0 = rtc_internal_get_ldocon(0);
    if(is_lp){
        temp0 |= (RTC_LDOCON0_LP_BG_MASK | RTC_LDOCON0_LP_VERF_MASK);
    } else {
        temp0 &= ~(RTC_LDOCON0_LP_BG_MASK | RTC_LDOCON0_LP_VERF_MASK);
    }
    rtc_internal_set_ldocon(0, temp0);
    return HAL_RTC_STATUS_OK;
}



hal_rtc_status_t    rtc_internal_set_ldo08_boost(bool is_boost)
{
    uint8_t     ldo08v = 0x7;
    uint32_t    temp0  = 0;

    rtc_internal_set_setting_cg_by_user(false, true);
    temp0 = rtc_internal_get_ldocon(0);
    if(is_boost){
        temp0 |= (ldo08v<<RTC_LDOCON0_SEL_LDO08_OFFSET);
    } else {
#ifdef HAL_FLASH_MODULE_ENABLED
        hal_flash_status_t  status;
        status = hal_flash_otp_read(19, &ldo08v, 1);
        if(HAL_FLASH_STATUS_OK != status){
            ldo08v = 3;
        }
#endif
    }
    rtc_internal_set_ldocon(0, temp0);
    rtc_internal_set_setting_cg_by_user(false, false);
    return HAL_RTC_STATUS_OK;
}



#ifdef RTC_CAPTOUCH_SUPPORTED

static hal_rtc_status_t rtc_internal_unlock_captouch(void)
{
    rtc_register->RTC_CAP_CON = 0x05AD;
    rtc_local_trigger_wait();
    rtc_register->RTC_CAP_CON = 0x92E1;
    rtc_local_trigger_wait();

    return HAL_RTC_STATUS_OK;
}

hal_rtc_status_t rtc_internal_captouch_init(void)
{
    uint32_t temp;

    rtc_local_config_wakeup_source(RTC_WAKEUP_SOURCE_CAP,true);
    temp =  rtc_internal_get_ldocon(0);
    temp &= 0xdfff;
    temp |= RTC_LDOCON0_LDO09_MASK;
    rtc_internal_set_ldocon(0, temp);
    hal_gpt_delay_us(100);


    temp |= (RTC_LDOCON0_LDO09_MASK | RTC_LDOCON0_PWR_CPT_MASK);
    rtc_internal_set_ldocon(0, temp);
    hal_gpt_delay_us(5);

    rtc_internal_unlock_captouch();
<<<<<<< HEAD
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_ISO_EN_MASK);
=======
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK);
>>>>>>> db20e11 (second commit)
    rtc_local_trigger_wait();
    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_ISO_EN_MASK);
    rtc_local_trigger_wait();
    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_ISO_EN_MASK | RTC_CAP_CON_CLK_CG_MASK);
    rtc_local_trigger_wait();
    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_CLK_CG_MASK);
    rtc_local_trigger_wait();
    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_CLK_CG_MASK | RTC_CAP_CON_LSH_EN_MASK);
    rtc_local_trigger_wait();
    hal_gpt_delay_us(50);

    return HAL_RTC_STATUS_OK;
}

bool rtc_internal_captouch_get_capcon_state(void)
{
    uint8_t ret;

    rtc_internal_set_setting_cg_by_user(false, true);
<<<<<<< HEAD
    rtc_internal_unlock_captouch(); 
=======
    rtc_internal_unlock_captouch();
>>>>>>> db20e11 (second commit)
    rtc_internal_reload();
    ret = rtc_register->RTC_CAP_CON & 0x01;
    rtc_internal_set_setting_cg_by_user(false, false);
    return ret;
}


hal_rtc_status_t rtc_internal_captouch_deinit(void)
{
    uint32_t temp;

    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_CLK_CG_MASK);
    rtc_local_trigger_wait();

    hal_gpt_delay_us(50);

    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_ISO_EN_MASK  | RTC_CAP_CON_CLK_CG_MASK);
    rtc_local_trigger_wait();

    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_ISO_EN_MASK  | RTC_CAP_CON_CLK_CG_MASK);
    rtc_local_trigger_wait();

    rtc_internal_unlock_captouch();
    rtc_register->RTC_CAP_CON = (RTC_CAP_CON_RST_MASK | RTC_CAP_CON_ISO_EN_MASK);
    rtc_local_trigger_wait();

    temp = rtc_internal_get_ldocon(0);
    temp &= 0x9fe0;
    rtc_internal_set_ldocon(0, temp | RTC_LDOCON0_LDO09_MASK);

    return HAL_RTC_STATUS_OK;
}


hal_rtc_status_t rtc_internal_captouch_lowpower(hal_captouch_lowpower_type_t lowpower_type)
{
    uint32_t    temp0 = 0, temp1 = 0;

    rtc_internal_set_setting_cg_by_user(false, true);
    if(lowpower_type == HAL_CAPTOUCH_MODE_LOWPOWER) {
        temp0 = rtc_internal_get_ldocon(0);
        temp0 &= 0xfff0;
        temp0 |=  (RTC_LDOCON0_LP_BG_MASK);
        rtc_internal_set_ldocon(0, temp0);

        temp1 =  rtc_internal_get_ldocon(1);
        temp1 &= 0xfcff;
        temp1 |=  RTC_LDOCON1_ISEL_VREF_MASK;
        rtc_internal_set_ldocon(1, temp1);

        temp0 = rtc_internal_get_ldocon(0);
        temp0 &= 0xffef;
        temp0 |=  (RTC_LDOCON0_LP_VERF_MASK);
        rtc_internal_set_ldocon(0, temp0);
    } else {
        temp0 = rtc_internal_get_ldocon(0);
        temp0 &= ~(RTC_LDOCON0_LP_BG_MASK);
        rtc_internal_set_ldocon(0, temp0);

        temp1 =  rtc_internal_get_ldocon(1);
        temp1 &= ~(0x3<<RTC_LDOCON1_ISEL_VREF_OFFSET);
        rtc_internal_set_ldocon(1, temp1);

        temp0 &= ~(RTC_LDOCON0_LP_VERF_MASK);
        rtc_internal_set_ldocon(0, temp0);
    }
    rtc_internal_set_setting_cg_by_user(false, false);
    return HAL_RTC_STATUS_OK;
}
#endif

uint32_t rtc_internal_measure_counter(uint32_t fixed_clock, uint32_t tested_clock, uint32_t window_setting)
{
    uint32_t fqmtr_data = 0;

    /* 1) *PLL_ABIST_FQMTR_CON1__F_FQMTR_EN = 0x0*/
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 &= (~(1<<RTC_ABIST_ENABLE_OFFSET)); //disable en
    /* 2) *PLL_ABIST_FQMTR_CON1__F_FQMTR_RST = 0x1 */
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 |= (1<<RTC_ABIST_RESET_OFFSET);     //enable reset
    /* 3) *PLL_ABIST_FQMTR_CON1__F_FQMTR_RST = 0x0 */
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 &= ~(1<<RTC_ABIST_RESET_OFFSET);    //disable reset
    /* 4) *CKSYS_TST_SEL_1__F_TST_SEL_1 = REF_CLK << 8 + SRC_CLK */
    *RTC_CKSYS_TST_SEL_1_BASE = (fixed_clock<<8)|(tested_clock);
    /* 5) *PLL_ABIST_FQMTR_CON0__F_FQMTR_WINSET = 0x63 */
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON0  = (window_setting);
    /* 6) *PLL_ABIST_FQMTR_CON1__F_FQMTR_EN = 0x1 */
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 |= (1<<RTC_ABIST_ENABLE_OFFSET);   //enable en
    //log_rtc_info("FQMTR = %x",1,rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1);
    /* 7) Wait FQMTR start*/
    hal_gpt_delay_ms(5);
    while ((rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 & (1<<RTC_ABIST_STATUS_OFFSET)));
    /* 8) fqmtr_data =*PLL_ABIST_FQMTR_DATA__F_DATA;*/
    //log_rtc_info("FQMTR STA= %x,FQMTR DATA= %d", 2, rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1, rtc_fqmtr_register->RTC_ABIST_FQMTR_DATA);
    fqmtr_data = rtc_fqmtr_register->RTC_ABIST_FQMTR_DATA;
     /* 8) Disable FQMTR*/
    rtc_fqmtr_register->RTC_ABIST_FQMTR_CON1 &= (~(1<<RTC_ABIST_ENABLE_OFFSET)); //disable en
    return fqmtr_data;
}

void rtc_internal_output_32k_to_gpio()
{
//#ifdef RTC_DEBUG_MONITOR_ENABLE
    uint32_t    temp = 0;

    *((volatile uint32_t*)(0xA2010008)) = 0x12;

    /*output EOSC-32k to GPIO*/
    temp |= (0x1<<RTC_DEBUG_CON0_DBG_EN_OFFSET);
    temp |= (0x1<<RTC_DEBUG_CON0_GPIO_SEL_OFFSET);//RTC GPIO1 output mon_bit_2
    temp |= (0xd<<RTC_DEBUG_CON0_DBG_SEL_OFFSET); //select debug signal
    rtc_register->RTC_DEBUG_CON_0 = temp;

    hal_gpio_init(HAL_GPIO_4);
    hal_pinmux_set_function(HAL_GPIO_4, 11);


    /*output ldo power to RTC_GPIO_0*/
    rtc_internal_gpio_init(HAL_RTC_GPIO_0, 0x02);
    rtc_internal_set_ldocon(0, 0x4C60);
    rtc_internal_set_ldocon(2, 0x0020);
    //rtc_register->RTC_RESET_CON = 0x0;
//#endif
}


void    rtc_internal_dump(char *comment)
{
    (void)comment;

    rtc_internal_reload();

    log_rtc_info("------------ Start Dump ------------", 0);
    log_rtc_info("BBPU(%x), LPD_CON(%x),  WRTGR(%x),  RESETCON(%x)", 4,
                    (unsigned int)rtc_register->RTC_BBPU,(unsigned int)rtc_register->RTC_LPD_CON,
                    (unsigned int)rtc_register->RTC_WRTGR, rtc_register->RTC_RESET_CON
                 );
    log_rtc_info("OSC32CON0(%x),  CON32CON1(%x),  CON32CON2(%x),    CALI(%x)",4,
                    (unsigned int)rtc_register->RTC_OSC32CON0, (unsigned int)rtc_register->RTC_OSC32CON1,
                    (unsigned int)rtc_register->RTC_OSC32CON2_UNION.VALUE, (unsigned int)rtc_register->RTC_CALI
                 );
    log_rtc_info("EINT0(%x),  EINT1(%x),  BOND(%x),  DIFF(%x), CAP_CON(%x)", 5,
                    (unsigned int)rtc_local_get_eintcon(0),   (unsigned int)rtc_local_get_eintcon(1),
                    (unsigned int)0, (unsigned int)rtc_register->RTC_DIFF,
                    (unsigned int)rtc_register->RTC_CAP_CON
                 );
    log_rtc_info("GPIO0(%x), GPIO1(%x), GPIO2(%x), GPIO_CON(%x), CII_EN(%x)", 5,
                    (unsigned int)rtc_register->RTC_GPIO0_CON, (unsigned int)rtc_register->RTC_GPIO1_CON,
                    (unsigned int)rtc_register->RTC_GPIO2_CON, (unsigned int)rtc_register->RTC_GPIO_CON,
                    (unsigned int)rtc_register->RTC_CII_EN
                 );
    log_rtc_info("AL_MASK(%x), AL0(%x), AL1(%x),  AL2(%x), AL3(%x)", 5, (unsigned int)rtc_register->RTC_AL_MASK,
                   (unsigned int)rtc_register->RTC_AL0, (unsigned int)rtc_register->RTC_AL1,
                   (unsigned int)rtc_register->RTC_AL2, (unsigned int)rtc_register->RTC_AL3
                 );
    log_rtc_info("LDOCON0(%x),  LDOCON1(%x),  LDOCON2(%x), SPAR_REG(%x)", 4,
                   (unsigned int)rtc_register->RTC_LDOCON0, (unsigned int)rtc_register->RTC_LDOCON1,
                   (unsigned int)rtc_internal_get_ldocon(2),(unsigned int)rtc_register->RTC_SPAR_REG
                 );
    log_rtc_info("SPARE0(%x), SPARE1(%x), SPARE2(%x), SPARE3(%x),SPARE4(%x), SPARE5(%x)", 6,
                   (unsigned int)rtc_register->RTC_SPAR0, (unsigned int)rtc_register->RTC_SPAR1,
                   (unsigned int)rtc_register->RTC_SPAR2, (unsigned int)rtc_register->RTC_SPAR3,
                   (unsigned int)rtc_register->RTC_SPAR4, (unsigned int)rtc_register->RTC_SPAR5
                 );
    log_rtc_info("------------ End  Dump ------------", 0);
}
/***********************************************************************************************
 * Private using api
***********************************************************************************************/

<<<<<<< HEAD
=======

/*
****************************************************************************
* Description:
*       Below two function is for resolve captouch-32k is disturb by audio 812.5kHz.
*   For resolve this, using DCXO-32k when audio is enabled;other case using
    EOSC-32k for save power.
*       Because of bt using 32k clock, so before switch 32k clock source need siwtch
*   BT switch clock from 32k to 26M first, that is avoid BT disconnect.
*       The 32k clock may stop 10us when 32k clock switch proecess.
******************************************************************************
*/

#include "hal_clock_internal.h"

extern ATTR_TEXT_IN_IRAM hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask_special(uint32_t *mask);
extern ATTR_TEXT_IN_IRAM hal_nvic_status_t hal_nvic_restore_interrupt_mask_special(uint32_t mask);
extern void bt_switch_clock_to_26M(void);
extern void bt_switch_clock_to_32K(void);
extern void bt_wait_clock_ready(void);
extern bool bt_check_32k_clock(void);

void    rtc_internal_lp_switch_32k(hal_rtc_osc32k_mode_t mode)
{
    uint32_t temp    = 0;
    uint32_t timeout = 10;

    log_rtc_info("[hal][rtc] lp switch 32k(%d)", 1, mode);
    rtc_local_wait_done();
    while ( timeout-- ){
        switch(mode) {
            case HAL_RTC_OSC32K_DCXO_MODE: {
                temp = 0x100;
            }break;
            case HAL_RTC_OSC32K_EOSC_MODE: {
                temp = 0x101;
            }break;
            case HAL_RTC_OSC32K_XOSC_MODE: {
                temp = 0x103;
            }break;
        }
        rtc_register->RTC_OSC32CON0 = RTC_OSC32CON0_MAGIC_KEY_1;
        hal_gpt_delay_us(2);
        rtc_register->RTC_OSC32CON0 = RTC_OSC32CON0_MAGIC_KEY_2;
        hal_gpt_delay_us(2);
        rtc_register->RTC_OSC32CON0 = temp;
        hal_gpt_delay_us(2);
        if(rtc_internal_check_clock_source(mode)){
            break;
        } else {
            rtc_internal_debug_code();
        }
    }
    if(timeout == 0 && rtc_internal_check_clock_source(mode) == false){
       log_rtc_error("[hal][rtc] lp switch 32k failed", 0);
       assert(0);
    }
    hal_gpt_delay_us(361);
}



void audio_class_d_disable_handle()
{
    uint32_t temp_flag= 0;
    uint32_t irq_mask = 0;
    //return;
    hal_nvic_save_and_set_interrupt_mask_special(&irq_mask);
    if(bt_check_32k_clock()){
        bt_switch_clock_to_26M(); /* Only write registers. */
        temp_flag = 1;
    }
    hal_rtc_switch_32k_source(HAL_RTC_OSC32K_EOSC_MODE); /* switch to EOSC-32k */

    hal_gpt_delay_us(500); /* dummy wait for BT */
    DCXO_PCON_REG.DCXO32K_EN = 0x0; /* disable dcxo-32k */
    bt_wait_clock_ready(); /*about0~ 500us: clear reg and polling reg */
    if(temp_flag == 1){
        bt_switch_clock_to_32K();
        temp_flag = 0;
    }
    hal_nvic_restore_interrupt_mask_special(irq_mask);
}

void audio_class_d_enable_handle()
{
    uint32_t temp_flag= 0;
    uint32_t irq_mask = 0;

    hal_nvic_save_and_set_interrupt_mask_special(&irq_mask);
    if(bt_check_32k_clock()){
        bt_switch_clock_to_26M(); /* Only write registers.*/
        temp_flag = 1;
    }
    DCXO_PCON_REG.DCXO32K_EN = 0x1; /* enable dcxo-32k */
    hal_rtc_switch_32k_source(HAL_RTC_OSC32K_DCXO_MODE); /* switch to DCXO-32k */

    hal_gpt_delay_us(500);/* dummy wait for BT */
    bt_wait_clock_ready(); /* about0~ 500us:clear reg and polling reg */
    if(temp_flag == 1){
        bt_switch_clock_to_32K();
        temp_flag = 0;
    }
    hal_nvic_restore_interrupt_mask_special(irq_mask);
}



>>>>>>> db20e11 (second commit)
/*private macro, donnot modify*/
#define     HAL_RTC_CHK_MONTH_OFFSET    (22)
#define     HAL_RTC_CHK_DAY_OFFSET      (17)
#define     HAL_RTC_CHK_HOUR_OFFSET     (12)
#define     HAL_RTC_CHK_MINUTE_OFFSET   (6)
#define     HAL_RTC_CHK_SECOND_OFFSET   (0)
#define     HAL_RTC_CHK_MONTH_MASK      (0x0F)
#define     HAL_RTC_CHK_DAY_MASK        (0x1F)
#define     HAL_RTC_CHK_HOUR_MASK       (0x1F)
#define     HAL_RTC_CHK_MINUTE_MASK     (0x3F)
#define     HAL_RTC_CHK_SECOND_MASK     (0x3F)
#define     RTC_DAY_OF_FOUR_YEAR        (366+365+365+365)
#define     RTC_SECOND_OF_DAY           (24*3600)
#define     RTC_SECOND_OF_FOUR_YEAR     (RTC_DAY_OF_FOUR_YEAR*24*3600)
#define     RTC_SECOND_OF_LYEAR         (366*24*3600)
#define     RTC_SECOND_OF_YEAR          (365*24*3600)
#define     RTC_MONTH_MAX               12
static const uint32_t    g_rtc_year_days[]       = {366,365,365,365};
static const uint32_t    g_rtc_year_month_day[RTC_MONTH_MAX +1]  = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const uint32_t    g_rtc_lyear_month_day[RTC_MONTH_MAX+1] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint32_t rtc_internal_time_to_tick(uint8_t  year,uint8_t month, uint8_t day,uint8_t hour, uint8_t min, uint8_t second)
{
    uint32_t    year_x4   = 0, year_lf = 0;
    uint32_t    total_sec = 0, temp    = 0;
    uint32_t    i     = 0;

    year_x4 = year>>2;
    year_lf = year%4;
    temp = RTC_DAY_OF_FOUR_YEAR * year_x4;
    /*calc year second*/
    for(i=0;i < year_lf;i++){
        temp += g_rtc_year_days[i];
    }
    /*calc month second*/
#if 0
{
    uint32_t (*pdom)[RTC_MONTH_MAX+1] = (year_lf == 0)?(&g_rtc_lyear_month_day):(&g_rtc_year_month_day);
    for(i = 1;i < month; i++){
        temp += (*pdom)[i];
    }
}
#else
    if(year_lf == 0) {
        for(i = 1;i < month; i++){
            temp += g_rtc_lyear_month_day[i];
        }
    } else {
        for(i = 1;i < month; i++){
            temp += g_rtc_year_month_day[i];
        }
    }
#endif
    temp += (day-1);
    total_sec = temp*RTC_SECOND_OF_DAY + hour *3600 + min*60 + second;
    return total_sec;
}

bool rtc_internal_tick_to_time(uint32_t tick, hal_rtc_time_t *time)
{
    uint32_t    one_year_second  = 0;
    uint32_t    year_x4 = 0;
    uint32_t    year_lf = 0;
    uint32_t    i       = 0;
    uint32_t    uitmp   = 0;

    if(time == NULL){
       return false;
    }
    year_x4 = tick/RTC_SECOND_OF_FOUR_YEAR;
    tick   -= (year_x4 * RTC_SECOND_OF_FOUR_YEAR);
    one_year_second = RTC_SECOND_OF_LYEAR;
    /*calc year*/
    uitmp    = year_x4*4;
    for(i = 0; i < 4; i++){
        if(tick > one_year_second){
            tick -= one_year_second;
            one_year_second = RTC_SECOND_OF_YEAR;
            uitmp++;
            year_lf++;
        } else {
            break;
        }
    }
    time->rtc_year = uitmp;
    /*calc month*/
#if 0
{
    uint32_t (*pdom)[RTC_MONTH_MAX+1] = (year_lf == 0)?(&g_rtc_lyear_month_day):(&g_rtc_year_month_day);
    for(i=1;i<13;i++){
        uitmp =  (*pdom)[i] * RTC_SECOND_OF_DAY;
        if (tick > uitmp) {
            tick -= uitmp;
        } else {
            break;
        }
    }
}
#else
if(year_lf == 0) {
    for(i=1;i<13;i++){
        uitmp =  g_rtc_lyear_month_day[i] * RTC_SECOND_OF_DAY;
        if (tick > uitmp) {
            tick -= uitmp;
        } else {
            break;
        }
    }
}else {
    for(i=1;i<13;i++){
        uitmp =  g_rtc_year_month_day[i] * RTC_SECOND_OF_DAY;
        if (tick > uitmp) {
            tick -= uitmp;
        } else {
            break;
        }
    }
}
#endif
    time->rtc_mon = i;
    /*calc day*/
    uitmp = tick/RTC_SECOND_OF_DAY;
    tick -= (uitmp*RTC_SECOND_OF_DAY);
    uitmp++;
    time->rtc_day = uitmp;
    /*calc hour*/
    uitmp  = tick/3600;
    tick -= uitmp*3600;
    time->rtc_hour = uitmp;
    /*calc minute*/
    uitmp = tick/60;
    tick -= (uitmp*60);
    time->rtc_min = uitmp;
    /*calc second*/
    time->rtc_sec = tick;
    return true;
}

#endif /*HAL_RTC_MODULE_ENABLED*/

