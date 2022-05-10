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

#ifndef _HAL_RTC_INTERNAL_H_
#define _HAL_RTC_INTERNAL_H_
#include "hal_platform.h"

#ifdef  HAL_RTC_MODULE_ENABLED
#ifdef __cplusplus
extern "C" {
#endif
#include "hal.h"
#include "syslog.h"
#include "hal_rtc.h"
#include "hal_captouch.h"
#include "hal_clock_platform.h"

#define RTC_DEBUG_ENABLE

/*chip supportt eint time?*/
#if defined(HAL_RTC_FEATURE_EINT) && defined(HAL_EINT_MODULE_ENABLED)
    #define     RTC_EINT_SUPPORTED
#endif

#ifdef HAL_NVIC_MODULE_ENABLED
    #define     RTC_NVIC_SUPPORTED
#endif

/*chip supportt gps time?*/
#ifdef HAL_RTC_FEATURE_GPS_TIME
    #define     RTC_GPS_SUPPORTED
#endif
/*chip supportt GPIO?*/
#ifdef HAL_RTC_FEATURE_GPIO
    #define     RTC_GPIO_SUPPORTED
#endif
/*chip supportt retention SRAM?*/
#ifdef HAL_RTC_FEATURE_RETENTION_SRAM
    #define     RTC_SRAM_SUPPORTED
#endif
/*chip is support captouch?*/
#ifdef HAL_CAPTOUCH_MODULE_ENABLED
    #define     RTC_CAPTOUCH_SUPPORTED
#endif

#ifdef MTK_HAL_EXT_32K_ENABLE
    #define     RTC_XOSC_SUPPORTED
#endif


#ifdef RTC_DEBUG_ENABLE
    #define log_rtc_info(_message, cnt, ...)    LOG_MSGID_I(RTC,_message,cnt, ##__VA_ARGS__)
    #define log_rtc_warn(_message, cnt, ...)    LOG_MSGID_W(RTC,_message,cnt, ##__VA_ARGS__)
    #define log_rtc_error(_message,cnt, ...)    LOG_MSGID_E(RTC,_message,cnt, ##__VA_ARGS__)
#else
    #define log_rtc_info(_message, cnt, ...)    printf(_message, ##__VA_ARGS__)
    #define log_rtc_warn(_message, cnt, ...)    printf(_message, ##__VA_ARGS__)
    #define log_rtc_error(_message, cnt, ...)   printf(_message, ##__VA_ARGS__)
#endif
/*Strap pin info RG*/
#define     RG_SYSTEM_INFO                      (0xA2010000)
#define     SYSTEM_INFO_RTC_CLOCK_TYPE_OFS      (4)
#define     SYSTEM_INFO_RTC_CLOCK_TYPE_MASK     (1<<SYSTEM_INFO_RTC_CLOCK_TYPE_OFS)
/*Measure 32k-clock*/
#define     AIR_RTC_FCXO_CLOCK                  (26000000)
#define     RTC_ABIST_FQMTR_BASE                (0xA2020400)
#define     RTC_CKSYS_TST_SEL_1_BASE            ((volatile uint32_t *)(0xA2020224))
#define     RTC_ABIST_RESET_OFFSET              (24)
#define     RTC_ABIST_ENABLE_OFFSET             (16)
#define     RTC_ABIST_STATUS_OFFSET             (7)
/*Referrence clock for measure 32k*/
#define     RTC_FQMTR_TCKSEL_XO_CK              (45)

#define     RTC_SPAR_RG_RESRV_OFFSET            (8)
#define     RTC_SPAR_RG_RESRV_MASK              (0xFF<<RTC_SPAR_RG_RESRV_OFFSET)
#define     RTC_SPAR_PWR_FLG_OFFSET             (7)
#define     RTC_SPAR_PWR_FLG_MASK               (1<<RTC_SPAR_PWR_FLG_OFFSET)
#define     RTC_SPAR_BROM_SKIP_OFFSET           (0)
#define     RTC_SPAR_BROM_SKIP_MASK             (0x1<<RTC_SPAR_BROM_SKIP_OFFSET)

/* frequency meter start */
typedef struct {
    __IO uint32_t RTC_ABIST_FQMTR_CON0;     /* Frequency Meter Control Register 0 */
    __IO uint32_t RTC_ABIST_FQMTR_CON1;     /* Frequency Meter Control Register 1 */
    __IO uint32_t RTC_ABIST_FQMTR_CON2;     /* Frequency Meter Control Register 2 */
    __IO uint32_t RTC_ABIST_FQMTR_DATA;     /* Frequency Meter Data */
} RTC_ABIST_FQMTR_REGISTER_T;

typedef enum {
    RTC_TIME_CALI_NORMAL = 0,
    RTC_TIME_CALI_K_EOSC = 1,
}rtc_time_cali_mode_t;

<<<<<<< HEAD
=======

typedef enum {
    RTC_POWERED_REASON_UNKNOW = 0,
    RTC_POWERED_BY_1ST = 1,
    RTC_POWERED_BY_ALARM = 2,
    RTC_POWERED_BY_TICK  = 3,
    RTC_POWERED_BY_EINT0 = 4,
    RTC_POWERED_BY_EINT1 = 5,
    RTC_POWERED_BY_EINT2 = 6,
    RTC_POWERED_BY_CAPTOUCH = 7,
    RTC_POWERED_BY_PWRKEY   = 8,
}hal_rtc_power_reason_t;


>>>>>>> db20e11 (second commit)
typedef enum {
    KEY_BBPU_MASK_ALL       = 0x43,
    KEY_BBPU_MASK_ALARM     = 0xA2, //bit0
    KEY_BBPU_MASK_CLR_WAKEUP= 0x39, //bit1
    KEY_BBPU_MASK_TICK      = 0xDA, //bit2
    KEY_BBPU_MASK_EINT0     = 0xC8, //bit3
    KEY_BBPU_MASK_EINT1     = 0x64, //bit4
    KEY_BBPU_MASK_EINT2     = 0xEE, //bit5
    KEY_BBPU_MASK_CAP       = 0x73, //bit6
    KEY_BBPU_MASK_VAL_RELOAD= 0x58,
}rtc_bbpu_bitmask_key_t;

typedef enum {
    RTC_WAKEUP_SOURCE_ALARM  =  0,
    RTC_WAKEUP_SOURCE_TICK   =  2,
    RTC_WAKEUP_SOURCE_EINT0  =  3,
    RTC_WAKEUP_SOURCE_EINT1  =  4,
    RTC_WAKEUP_SOURCE_EINT2  =  5,
    RTC_WAKEUP_SOURCE_CAP    =  6,
//RTC_WAKEUP_SOURCE_GALARM  = 4,
}rtc_wakeup_source_t;

typedef enum {
    RTC_IRQ_ENABLE_LOWPWR   = 0,
    RTC_IRQ_ENABLE_TICK     = 1,
    RTC_IRQ_ENABLE_ONESHOT  = 8,
}rtc_irq_enable_t;

typedef enum {
    RTC_TC_EN_1_8_SECOND    = 0,
    RTC_TC_EN_1_4_SECOND    = 1,
    RTC_TC_EN_1_2_SECOND    = 2,
    RTC_TC_EN_SECOND        = 3,
    RTC_TC_EN_MINUTE        = 4,
    RTC_TC_EN_HOUR          = 5,
    RTC_TC_EN_DAY           = 6,
    RTC_TC_EN_WEEK          = 7,
    RTC_TC_EN_MONTH         = 8,
    RTC_TC_EN_YEAR          = 9,
}rtc_tick_enable_t;

typedef enum {
    RTC_AL_MASK_SECOND      = 0,
    RTC_AL_MASK_MINUTE      = 1,
    RTC_AL_MASK_HOUR        = 2,
    RTC_AL_MASK_DAY         = 3,
    RTC_AL_MASK_WEEK        = 4,
    RTC_AL_MASK_MONTH       = 5,
    RTC_AL_MASK_YEAR        = 6,
    RTC_AL_MASK_MS          = 7,
}rtc_alarm_mask_t;


typedef enum {
    RTC_OSC32K_FCLK_MODE = 1,
    RTC_OSC32K_EOSC_MODE = 4,
    RTC_OSC32K_DCXO_MODE = 5,
    RTC_OSC32K_XOSC_MODE = 6,
}rtc_osc32k_mode_t;

typedef enum {
    RTC_BACK_FROM_RTC_MODE = 1,
    RTC_BACK_FROM_PWR_EXCEPTION,
}rtc_previou_mode_t;

<<<<<<< HEAD
=======
#define     RTC_STATE_INIT_BY_BL_FLG        0x1    /* flag for indicate whether rtc init by bl */
#define     RTC_STATE_POWER_LOST_FLG        0x2  /* flag for indicate whether rtc power lost */
#define     RTC_STATE_RTC_MODE_FLG          0x4  /* flag for indicate whether back from  rtc mode */
#define     RTC_STATE_RTC_WAKE_FLG          0x8  /* flag for indicate system wakeup by rtc event */



>>>>>>> db20e11 (second commit)

typedef struct {
    bool       used_xosc;
    bool       used_gpio;
<<<<<<< HEAD
    bool       flag_power_st;//1:off; 0:on
    bool       flag_rtc_mode;//1:rtc mode, 0: others
    bool       hw_init_st; //0:init;1:not init
    uint8_t    sram_mode;
=======
    uint8_t    sram_mode;
    uint16_t   op_state;
>>>>>>> db20e11 (second commit)
    uint16_t   irq_status;
    int        cali_eosc; /*only for eosc adjust*/
    int        cali_time;  /*for all osc32 adjust*/
    uint32_t   sram_mask;

    hal_rtc_time_callback_t   tick_callback;
    void                     *tick_userdata;
    hal_rtc_alarm_callback_t  alarm_callback;
    void                     *alarm_userdata;
    //hal_rtc_alarm_callback_t  galarm_callback;
    //void                     *galarm_userdata;
    hal_rtc_eint_callback_t   eint_callback[HAL_RTC_GPIO_MAX];
    void                     *eint_userdata[HAL_RTC_GPIO_MAX];
}rtc_private_parameter_t;

typedef struct {
    hal_rtc_time_notification_period_t type;
    uint8_t                            mask;
}rtc_period_type_mask_map_t;

void                rtc_internal_print_power_reason(uint32_t irq_status);
void                rtc_internal_reload(void);
void                rtc_internal_clear_wakeup_status(void);
//void                rtc_internal_set_setting_cg(bool  enable);
void                rtc_internal_set_setting_cg_by_user(bool force_ctrl, bool enable);
void                rtc_internal_set_timing_cg(bool  enable);
void                rtc_internal_unlock_protect(void);
bool                rtc_internal_enable_tick_notify(hal_rtc_time_notification_period_t type, bool enable);
void                rtc_internal_alarm_setting(const hal_rtc_time_t *time, bool enable,uint32_t mask);
void                rtc_internal_alarm_get(hal_rtc_time_t *time, bool  *is_enable, uint32_t  *mask);
void                rtc_internal_set_date_time(const hal_rtc_time_t *time);
void                rtc_internal_get_date_time( hal_rtc_time_t *time);
bool                rtc_internal_powerkey_is_valid(void);
bool                rtc_internal_is_powered_by_rtc(void);
void                rtc_internal_set_power_key(void);
void                rtc_internal_set_eosc32_calibration(uint32_t eosc_cali);
int                 rtc_internal_calc_time_calibration(rtc_osc32k_mode_t mode);
void                rtc_internal_set_time_calibration(rtc_time_cali_mode_t  mode, uint32_t  cali);
void                rtc_internal_clear_initb(void);
void                rtc_internal_dump(char*);
void                rtc_internal_set_osc32_mode(rtc_osc32k_mode_t mode);
void                rtc_internal_set_onetime_cali(uint32_t cali);
void                rtc_internal_init_register(void);
void                rtc_internal_enable_setting(void);
void                rtc_internal_set_spar_rg(uint16_t value);
void                rtc_internal_output_32k_to_gpio();
void                rtc_internal_clear_pwr_status();
uint16_t            rtc_internal_get_spar_rg(void);
uint32_t            rtc_internal_measure_frequency(rtc_osc32k_mode_t mode);
uint32_t            rtc_internal_irq_status(void);
uint32_t            rtc_internal_get_eosc32_calibration(void);
uint32_t            rtc_internal_get_time_calibration(void);
uint32_t            rtc_internal_get_onetime_cali(void);
hal_rtc_status_t    rtc_internal_eint_setting(hal_rtc_eint_config_t *eint_config);
hal_rtc_status_t    rtc_internal_set_data(uint16_t idx, uint8_t data);
hal_rtc_status_t    rtc_internal_get_data(uint16_t idx, uint8_t *data);
hal_rtc_status_t    rtc_internal_set_ldocon0(uint32_t value);
hal_rtc_status_t    rtc_internal_set_ldocon1(uint32_t value);
hal_rtc_status_t    rtc_internal_set_ldocon2(uint32_t value);
<<<<<<< HEAD
=======
uint32_t            rtc_internal_power_reason(uint32_t irq_status);


>>>>>>> db20e11 (second commit)

hal_rtc_status_t    rtc_internal_set_ldo08_boost(bool is_boost);
hal_rtc_status_t    rtc_internal_set_ldo_lowpower(bool is_lp);


hal_rtc_status_t    rtc_internal_set_retention_reg(uint32_t offset, uint8_t *buff, uint32_t size);
hal_rtc_status_t    rtc_internal_get_retention_reg(uint32_t offset, uint8_t *buff, uint32_t size);

#ifdef RTC_GPIO_SUPPORTED
void                rtc_internal_gpio_init(hal_rtc_gpio_t  rtc_gpio, uint32_t value);
hal_rtc_status_t    rtc_internal_gpio_control(hal_rtc_gpio_t  rtc_gpio, bool is_output, bool *level);
#endif

#ifdef RTC_CAPTOUCH_SUPPORTED
hal_rtc_status_t    rtc_internal_captouch_init(void);
hal_rtc_status_t    rtc_internal_captouch_deinit(void);
hal_rtc_status_t    rtc_internal_captouch_lowpower(hal_captouch_lowpower_type_t lowpower_type);
bool rtc_internal_captouch_get_capcon_state(void);

#endif

#ifdef RTC_SRAM_SUPPORTED
void                rtc_internal_sram_setting_cg(bool   disable_ck, bool disable_do);
void                rtc_internal_sram_setting(hal_rtc_sram_cell_t cell, hal_rtc_sram_mode_t mode);
void                rtc_internal_sram_setting(hal_rtc_sram_cell_t cell, hal_rtc_sram_mode_t mode);
#endif

<<<<<<< HEAD
=======
void                rtc_internal_lp_switch_32k(hal_rtc_osc32k_mode_t mode);
void                rtc_internal_lp_cg_setting(hal_rtc_osc32k_mode_t mode);

hal_rtc_power_reason_t  hal_rtc_get_power_on_reason();


>>>>>>> db20e11 (second commit)
/*internal using*/
uint32_t            rtc_internal_time_to_tick(uint8_t  year,uint8_t month, uint8_t day,uint8_t hour, uint8_t min, uint8_t second);
bool                rtc_internal_tick_to_time(uint32_t tick, hal_rtc_time_t *time);
#ifdef __cplusplus
}
#endif

#endif /*End HAL_RTC_MODULE_ENABLED*/
#endif /*End _HAL_RTC_INTERNAL_H_*/
