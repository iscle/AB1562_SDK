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

#define PMU_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(PMU,fmt,cnt,##arg)
#define PMU_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(PMU,fmt,cnt,##arg)
#define PMU_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(PMU,fmt,cnt,##arg)

#define PMIC_SLAVE_ADDR            0x6B
#define PMU_1WIRE_BAT_THRD         3200

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
    uint8_t value;
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
    } field;
} PMU_POWER_ON_REASON_UNION;

typedef union union_pmu_power_off_reason
{
    uint8_t value;
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

#endif /* HAL_PMU_MODULE_ENABLED */
#endif //__HAL_PMU_H__
