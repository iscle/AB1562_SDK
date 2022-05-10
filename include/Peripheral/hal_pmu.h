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
#include "log.h"

#ifndef __HAL_PMU_H__
#define __HAL_PMU_H__
#ifdef HAL_PMU_MODULE_ENABLED

<<<<<<< HEAD
#define PMIC_SLAVE_ADDR            0x6B
#define INVALID_INTERRUPT_VALUE    0xFF
#define PMU_PRESS_PK_TIME 300000  //300ms
//#define HAL_PMU_DEBUG_ENABLE
#define PMU_VCORE_CP     0xA20A0204

=======
>>>>>>> db20e11 (second commit)
#define PMU_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(PMU,fmt,cnt,##arg)
#define PMU_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(PMU,fmt,cnt,##arg)
#define PMU_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(PMU,fmt,cnt,##arg)

<<<<<<< HEAD
=======
#define PMIC_SLAVE_ADDR            0x6B
#define PMU_1WIRE_BAT_THRD         3200

>>>>>>> db20e11 (second commit)
typedef enum {
    PMU_NO_FTK = 0xFF,
    PMU_ECO1 = 1,
    PMU_ECO2 = 2,
    PMU_ECO3 = 3,
} pmu_adie_version_t;

typedef enum{
    PMU_LOCK=0,
    PMU_UNLOCK,
    PMU_TEMP
}pmu_lock_parameter_t;

typedef enum {
<<<<<<< HEAD
    PMU_NONE          = 0,                 /**< NONE Trigger */
    PMU_EDGE_RISING   = 1,                 /**< edge and rising trigger */
    PMU_EDGE_FALLING  = 2,                 /**< edge and falling trigger */
    PMU_EDGE_FALLING_AND_RISING = 3        /**< edge and falling or rising trigger */
} pmu_int_trigger_mode_t;

typedef enum {
=======
>>>>>>> db20e11 (second commit)
    PMU_STATUS_INVALID_PARAMETER  = -1,     /**< pmu error invalid parameter */
    PMU_STATUS_ERROR              = 0,     /**< pmu undefined error */
    PMU_STATUS_SUCCESS            = 1       /**< pmu function ok */
}pmu_status_t;

typedef enum {
    PMU_ERROR   = 0,
    PMU_OK      = 1,
    PMU_UNUSED  = 2,
} pmu_operate_status_t;

typedef enum {
    PMU_OFF    = 0,
    PMU_ON   = 1,
} pmu_power_operate_t;

typedef enum {
<<<<<<< HEAD
    PMU_SW_MODE,
    PMU_HW_MODE,
} pmu_control_mode_t;

typedef enum {
=======
>>>>>>> db20e11 (second commit)
    PMU_PWRKEY_IRQ_RISE_FLAG,
    PMU_PWRKEY_IRQ_RISE_EN,
    PMU_PWRKEY_IRQ_FALL_FLAG,
    PMU_PWRKEY_IRQ_FALL_EN,
    PMU_REGEN_IRQ_TIME_SEL,
    PMU_CHG_PON_FLAG = 6,
    PMU_REGEN_PON_FLAG,
    PMU_RTC_ALARM_FLAG,
    PMU_CHG_ALARM_FLAG,
    PMU_REGEN_ALARM_FLAG,
    PMU_RTC_MODE_FLAG,
    PMU_CAP_LPSD_FLAG,
    PMU_REGEN_LPSD_FLAG,
    PMU_SYS_RST_FLAG,
    PMU_WD_RST_FLAG,
} pmu_pwrkey_int_mask_t;

typedef enum {
<<<<<<< HEAD
    PMU_DURATION_5S,
    PMU_DURATION_8S,
    PMU_DURATION_11S,
    PMU_DURATION_25S,
} pmu_pwrkey_time_t;

typedef enum {
=======
>>>>>>> db20e11 (second commit)
    PMU_PWR_KEY_DEB_10MS,
    PMU_PWR_KEY_DEB_20MS,
    PMU_PWR_KEY_DEB_30MS,
    PMU_PWR_KEY_DEB_40MS,
} pmu_pwrkey_deb_time_t;

typedef enum {
    PMU_PWR_KEY_IRQ_FALL_EN,
    PMU_PWR_KEY_IRQ_RISE_EN,
} pmu_pwrkey_irq_sel_t;

typedef enum {
    CAP_LPSD,
    REGEN_LPSD,
    SYS_RST,
    WD_RST,
} pmu_lpsd_rst_sel_t;

typedef enum {
    PMU_PWROFF,
    PMU_RTC,
    PMU_SLEEP,
    PMU_NORMAL,
} pmu_power_stage_t;

typedef union union_pmu_power_on_reason
{
<<<<<<< HEAD
    uint16_t value;
=======
    uint8_t value;
>>>>>>> db20e11 (second commit)
    struct stru_pmu_power_on_reason
    {
        bool off_norm_chg_pon        : 1;
        bool off_norm_regen_pon      : 1;
        bool rtc_norm_rtc_alarm      : 1;
        bool rtc_norm_chg_alarm      : 1;
        bool rtc_norm_regen_alarm    : 1;
        bool reserved5               : 1;
        bool reserved6               : 1;
        bool reserved7               : 1;
<<<<<<< HEAD
        bool reserved8               : 1;
        bool reserved9               : 1;
        bool reserved10              : 1;
        bool reserved11              : 1;
        bool reserved12              : 1;
        bool reserved13              : 1;
        bool reserved14              : 1;
        bool reserved15              : 1;
=======
>>>>>>> db20e11 (second commit)
    } field;
} PMU_POWER_ON_REASON_UNION;

typedef union union_pmu_power_off_reason
{
<<<<<<< HEAD
    uint16_t value;
=======
    uint8_t value;
>>>>>>> db20e11 (second commit)
    struct stru_pmu_power_off_reason
    {
        bool norm_rtc_rtc_mode       : 1;
        bool norm_rtc_cap_lpsd       : 1;
        bool norm_rtc_regen_lpsd     : 1;
        bool norm_excep_sys_rst      : 1;
        bool norm_excep_wd_rst       : 1;
        bool reserved5               : 1;
        bool reserved6               : 1;
        bool reserved7               : 1;
<<<<<<< HEAD
        bool reserved8               : 1;
        bool reserved9               : 1;
        bool reserved10              : 1;
        bool reserved11              : 1;
        bool reserved12              : 1;
        bool reserved13              : 1;
        bool reserved14              : 1;
        bool reserved15              : 1;
=======
>>>>>>> db20e11 (second commit)
    } field;

} PMU_POWER_OFF_REASON_UNION;

typedef enum {
    PMU_PWR_OFF,
    PMU_PWR_ON_SLEEP,
    PMU_PWR_ON_NORM,
    PMU_PWR_ON,
    PMU_PWR_ERROR,
} pmu_power_state_t;

typedef enum {
    PMIC_PERIOD_0P5_MS,
    PMIC_PERIOD_1P0_MS,
    PMIC_PERIOD_1P5_MS,
    PMIC_PERIOD_2P0_MS,
} pmu_vrf_period_t;

typedef enum {
    RESERVED_0,
    PMIC_PMIC_UNIT_10MV,
    PMIC_PMIC_UNIT_20MV,
    PMIC_PMIC_UNIT_30MV,
    RESERVED_4,
    RESERVED_5,
    PMIC_PMIC_UNIT_60MV
} pmu_vrf_unit_t;

typedef enum {
    PMIC_PWR_OFF,
    PMIC_PWR_0P8_V,
    PMIC_PWR_0P9_V,
} pmu_voltage_t;

typedef enum {
    PMIC_VCORE_0P8_V,
<<<<<<< HEAD
    PMIC_VCORE_0P9_V,
    PMIC_VCORE_0P8_V_CUSTOM,
    PMIC_VCORE_0P9_V_CUSTOM,
=======
>>>>>>> db20e11 (second commit)
} pmu_power_vcore_voltage_t;

typedef enum {
    PMU_BUCK_VCORE,
    PMU_BUCK_VIO18,
    PMU_BUCK_VRF,
    PMU_LDO_VLDO33,
    PMU_LDO_VDIG18,
} pmu_power_domain_t;

typedef enum {
    RG_INT_PWRKEY_FALL,                    //PMU_CON3,     index :0
    RG_INT_PWRKEY_RISE,                    //PMU_CON3,     index :2
    RG_INT_CHG_IN,                         //CHARGER_CON1, index :0
    RG_INT_CHG_OUT,                        //CHARGER_CON1, index :2
    RG_INT_CHG_COMPL,                      //CHARGER_CON1, index :4
    RG_INT_CHG_RECHG,                      //CHARGER_CON1, index :6
    RG_INT_ADC_SW_TRIG,                    //ADC_CON3,     index :1
    RG_INT_ADC_HW_TRIG,                    //ADC_CON3,     index :3
    PMU_INT_MAX,
} pmu_interrupt_index_t;

typedef enum {
    PMU_CHG_IN_INT_FLAG,
    PMU_CHG_IN_INT_EN,
    PMU_CHG_OUT_INT_FLAG,
    PMU_CHG_OUT_INT_EN,
    PMU_CHG_COMPLETE_INT_FLAG,
    PMU_CHG_COMPLETE_INT_EN,
    PMU_CHG_RECHG_INT_FLAG,
    PMU_CHG_RECHG_INT_EN,
} pmu_chg_int_mask_t;

typedef enum {
    PMU_CHG_OUT_DEB_0MS,
    PMU_CHG_OUT_DEB_10MS,
    PMU_CHG_OUT_DEB_20MS,
    PMU_CHG_OUT_DEB_40MS,
    PMU_CHG_IN_DEB_0MS = 0,
    PMU_CHG_IN_DEB_16MS,
    PMU_CHG_IN_DEB_40MS,
    PMU_CHG_IN_DEB_128MS,
} pmu_chg_deb_time_t;

typedef enum {
    PMU_CHG_OUT,
    PMU_CHG_IN,
    PMU_CHG_COMPL,
    PMU_CHG_RECHG,
} pmu_chg_irq_sel_t;

typedef enum {
    PMU_ADC_ONE_SHOT_START,
    PMU_ADC_SW_TRIG_FLAG,
    PMU_ADC_SW_IRQ_EN,
    PMU_ADC_HW_TRIG_FLAG,
} pmu_adc_int_mask_t;

enum
{
    PMU_NOT_INIT,
    PMU_INIT,
};

typedef void (*pmu_callback_t)(void *user_data);

typedef struct
{
    void (*pmu_callback)(void *user_data);
    void *user_data;
    bool init_status;
	bool isMask;
} pmu_function_t;

typedef struct
{
    pmu_callback_t callback1; //press callback
    void *user_data1;
    pmu_callback_t callback2; //release callback
    void *user_data2;
} pmu_pwrkey_config_t;

typedef enum {
    PMU_CHG_FORCE_OFF_DISABLE,
    PMU_CHG_FORCE_OFF_ENABLE,
} pmu_chg_force_t;

typedef struct
{
    uint8_t  enable;
    uint32_t timer_s;
}PACKED rtc_alarm_config_t;

<<<<<<< HEAD
/*
 * [4]   STS_RBOOT  :Power on for cold reset
 * [3]   STS_SPAR   :reserved
 * [2]   STS_CHRIN  :Power on for charger insertion
 * [1]   STS_RTCA   :Power on for RTC alarm
 * [0]   STS_PWRKEY :Power on for PWREKY press
 *
 */
extern pmu_function_t pmu_function_table[PMU_INT_MAX];
extern rtc_alarm_config_t rtc_config;
extern uint8_t Adie_Version;
extern PMU_POWER_ON_REASON_UNION pmu_power_on_reason;
extern PMU_POWER_OFF_REASON_UNION pmu_power_off_reason;
void pmu_get_adie_version(void);
pmu_power_vcore_voltage_t pmu_vcore_lock_control(pmu_power_stage_t mode,pmu_power_vcore_voltage_t vol,pmu_lock_parameter_t lock);
void pmu_control_power(pmu_power_domain_t pmu_pdm, pmu_power_operate_t operate);
pmu_status_t pmu_control_enable_interrupt(pmu_interrupt_index_t int_channel, int isEnable);
pmu_status_t pmu_control_mask_interrupt(pmu_interrupt_index_t int_channel, int isEnable);
pmu_status_t pmu_control_clear_interrupt(pmu_interrupt_index_t int_channel);
pmu_status_t  pmu_control_clear_interrupt(pmu_interrupt_index_t int_channel);
int pmu_get_raw_status_interrupt(pmu_interrupt_index_t int_channel);
int pmu_get_status_interrupt(pmu_interrupt_index_t int_channel);
void pmu_charger_interrupt_handler(void);
pmu_status_t pmu_deregister_callback(pmu_interrupt_index_t pmu_int_ch);
pmu_status_t pmu_register_callback(pmu_interrupt_index_t pmu_int_ch, pmu_callback_t callback, void *user_data);
//bool pmu_is_charger_exist_init(void);
pmu_status_t pmu_control_mask(pmu_interrupt_index_t pmu_int_ch, bool isMaskInterrupt);
void pmu_control_clear_all_charger_interrupt(void);
pmu_operate_status_t pmu_set_register_value_2byte_init(void);
pmu_operate_status_t pmu_set_register_value_2byte(unsigned short int address, unsigned short int mask, unsigned short int shift, unsigned short int value);
pmu_operate_status_t pmu_force_set_register_value_2byte(unsigned short int address, unsigned short int value);
uint32_t pmu_get_register_value_2byte(unsigned short int address, unsigned short int mask, unsigned short int shift);
void pmu_read_rg(unsigned short int address);
pmu_operate_status_t pmu_vcore_voltage_sel(pmu_power_stage_t mode ,pmu_power_vcore_voltage_t vol);
pmu_power_vcore_voltage_t pmu_get_vcore_voltage(void);
pmu_operate_status_t pmu_vaud18_voltage_sel(uint8_t vol);
uint16_t pmu_get_power_on_reason(void);
uint16_t pmu_get_power_off_reason(void);
void pmic_i2c_deinit(void);
void pmic_i2c_init(void);
void pmu_on_mode_switch(pmu_power_domain_t domain, pmu_control_mode_t mode);
void pmu_lp_mode(pmu_power_domain_t domain, pmu_control_mode_t mode);

pmu_operate_status_t pmu_chg_deb_time(pmu_chg_deb_time_t out_deb_time, pmu_chg_deb_time_t in_deb_time);

void pmu_init(void);
void pmu_eint_init();
void pmu_eint_clean_proc(void);
void pmu_eoc_ctrl (pmu_power_operate_t oper, uint8_t en);

void pmu_vcroe_voltage_turing(int symbol,int num );
void pmu_vio18_voltage_turing(int symbol,int num );
void pmu_vaud18_voltage_turing(int symbol,int num );

uint8_t pmu_get_lock_status(void);
int pmu_get_lock_index(void);
void pmu_set_usb_input_status(void);
uint8_t pmu_get_usb_input_status(void);
void pmu_rtc_eoc_ctrl (void);
void pmu_power_off_sequence(pmu_power_stage_t stage);
void pmu_srclken_control_mode(pmu_power_operate_t mode);
void pmu_power_enable(pmu_power_domain_t pmu_pdm, pmu_power_operate_t operate);
void pmu_voltage_selet(pmu_power_stage_t mode,pmu_power_domain_t domain,uint32_t vol);
/*PWRKEY long pressed reset enable & disable*/
pmu_operate_status_t pmu_pwrkey_enable(pmu_power_operate_t oper);
pmu_power_state_t pmu_get_power_status(pmu_power_domain_t pmu_pdm);
pmu_operate_status_t pmu_chg_force_sel(pmu_chg_force_t oper);

pmu_operate_status_t pmu_pwrkey_deb_sel(pmu_pwrkey_deb_time_t tmr);
pmu_operate_status_t pmu_pwrkey_irq_sel(pmu_pwrkey_irq_sel_t type, pmu_power_operate_t oper);

/*Long pressed time to issue reset
 * 2'b00: 5 sec
 * 2'b01: 8 sec
 * 2'b10: 11 sec
 * 2'b11: 25 sec
 * */
pmu_operate_status_t pmu_pwrkey_duration_time(pmu_pwrkey_time_t tmr);

/*Re power-on scenario slection
 * 2'b00: debounce pwrkey
 * 2'b01: after release pwrkey
 * 2'b10: after release pwrkey and press pwrkey again
*/
pmu_operate_status_t pmu_lpsd_rst_init(pmu_lpsd_rst_sel_t sel, pmu_power_operate_t oper);

=======
typedef enum {
    PMU_CC_JEITA_NORM,
    PMU_CC_JEITA_WARM,
    PMU_CC_JEITA_COOL,
    PMU_CC_BAT_REPORT,
    PMU_CC_MAX,
} pmu_chg_cc_state_t;

typedef struct
{
    uint16_t state;
    uint16_t norm_cc1_rsel;
    uint16_t warm_cc1_rsel;
    uint16_t cool_cc1_rsel;
    uint16_t vbat_cc1_rsel;
    uint16_t norm_cc2_rsel;
    uint16_t warm_cc2_rsel;
    uint16_t cool_cc2_rsel;
    uint16_t vbat_cc2_rsel;
} pmu_chg_cc_rsel_t;

typedef enum {
    PMU_CHG_CC1,
    PMU_CHG_CC2,
} pmu_cc_type_t;

enum {
    PMU_CASE_TYPE_NONE,
    PMU_CASE_TYPE_GPIOXY,
    PMU_CASE_TYPE_SMART,
    PMU_CASE_TYPE_SMART_V2,
    PMU_CASE_TYPE_ONE_WIRE,
};

extern pmu_function_t pmu_function_table[PMU_INT_MAX];
extern rtc_alarm_config_t rtc_config;
extern uint8_t pmu_version;
extern PMU_POWER_ON_REASON_UNION pmu_power_on_reason;
extern PMU_POWER_OFF_REASON_UNION pmu_power_off_reason;
extern pmu_chg_cc_rsel_t pmu_chg_cc_config;

void pmu_get_adie_version(void);
uint8_t pmu_get_power_on_reason(void);
uint8_t pmu_get_power_off_reason(void);
void pmu_power_off_sequence(pmu_power_stage_t stage);

void pmu_init(void);
void pmu_eint_clean_proc(void);

uint32_t pmu_get_register_value(unsigned short int address, unsigned short int mask, unsigned short int shift);
pmu_operate_status_t pmu_set_register_value(unsigned short int address, unsigned short int mask, unsigned short int shift, unsigned short int value);
pmu_operate_status_t pmu_force_set_register_value(unsigned short int address, unsigned short int value);

pmu_status_t pmu_register_callback(pmu_interrupt_index_t pmu_int_ch, pmu_callback_t callback, void *user_data);
pmu_status_t pmu_deregister_callback(pmu_interrupt_index_t pmu_int_ch);

void pmu_eoc_ctrl (pmu_power_operate_t oper, uint8_t en);
pmu_operate_status_t pmu_cc_rsel_config(pmu_cc_type_t type, pmu_chg_cc_state_t state);
pmu_operate_status_t pmu_chg_force_sel(pmu_chg_force_t oper);
pmu_operate_status_t pmu_chg_deb_time(pmu_chg_deb_time_t out_deb_time, pmu_chg_deb_time_t in_deb_time);

pmu_operate_status_t pmu_pwrkey_deb_sel(pmu_pwrkey_deb_time_t tmr);
pmu_operate_status_t pmu_pwrkey_irq_sel(pmu_pwrkey_irq_sel_t type, pmu_power_operate_t oper);
pmu_operate_status_t pmu_lpsd_rst_init(pmu_lpsd_rst_sel_t sel, pmu_power_operate_t oper);

pmu_power_state_t pmu_get_power_status(pmu_power_domain_t pmu_pdm);
void pmu_voltage_selet(pmu_power_stage_t mode,pmu_power_domain_t domain,uint32_t vol);
pmu_operate_status_t pmu_vcore_voltage_sel(pmu_power_stage_t mode ,pmu_power_vcore_voltage_t vol);
>>>>>>> db20e11 (second commit)

#endif /* HAL_PMU_MODULE_ENABLED */
#endif //__HAL_PMU_H__
