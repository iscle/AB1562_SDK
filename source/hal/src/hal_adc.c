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

#include "hal_adc.h"

#ifdef HAL_ADC_MODULE_ENABLED

#include "hal_clock.h"
//#include "hal_log.h"
#include "hal_nvic.h"
#include "hal_nvic_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include <stdio.h>

#ifdef HAL_ADC_CALIBRATION_ENABLE
#include "hal_flash_disk_internal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAL_SLEEP_MANAGER_ENABLED
static volatile  uint32_t s_macro_con2;
static volatile  uint32_t s_ana_en_con;

void adc_backup_all_register(void);
void adc_restore_all_register(void);
#endif

//todo:wait log driver ready
#define log_hal_msgid_error(_message,arg_cnt,...)

#define ADC_DEBUG_LOG
#ifdef ADC_DEBUG_LOG
#define ADC_DEBUG_INF(fmt,...) printf(fmt,##__VA_ARGS__)
#else
#define ADC_DEBUG_INF(fmt,...)
#endif

#define ADC_BUSY 1
#define ADC_IDLE 0

volatile uint8_t g_adc_status = ADC_IDLE;

#ifdef HAL_ADC_CALIBRATION_ENABLE
double adc_slope;
double X1, Y1;
double adc_a , adc_b;
#define Calibration_one  1
#define Calibration_two  0
uint8_t g_calibration_status = Calibration_two;
#endif

hal_adc_status_t hal_adc_init(void)
{
    uint32_t int_mask;

    /* In order to prevent race condition, interrupt should be disabled when query and update global variable which indicates the module status */
    hal_nvic_save_and_set_interrupt_mask(&int_mask);

    /* Check module status */
    if (g_adc_status == ADC_BUSY) {
        log_hal_msgid_error("\r\n [ADC] Module is busy!", 0);

        /* Restore the previous status of interrupt */
        hal_nvic_restore_interrupt_mask(int_mask);

        return HAL_ADC_STATUS_ERROR_BUSY;
    } else {
        /* Change status to busy */
        g_adc_status = ADC_BUSY;

        /* Restore the previous status of interrupt */
        hal_nvic_restore_interrupt_mask(int_mask);
    }

#ifdef HAL_SLEEP_MANAGER_ENABLED
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_AUXADC, (sleep_management_suspend_callback_t)adc_backup_all_register, NULL);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_AUXADC, (sleep_management_resume_callback_t)adc_restore_all_register, NULL);
#endif

    /* Enable VA28_LDO */
    ADC->MACRO_CON2 |= MACRO_CON2_RG_AUXADC_LDO_EN_MASK;

    /* Enable AUXADC */
    ADC->ANA_EN_CON = ANA_EN_CON_AUXADC_EN_MASK;

    /*Enable clock */
    if (HAL_CLOCK_STATUS_OK != hal_clock_enable(HAL_CLOCK_CG_AUXADC)) {
        log_hal_msgid_error("\r\n [ADC] Clock enable failed!", 0);
        return HAL_ADC_STATUS_ERROR;
    }

    /* Software reset ADC */
    ADC->AUXADC_CON3_UNION.AUXADC_CON3_CELLS.SOFT_RST = AUXADC_CON3_SOFT_RST_MASK;
    ADC->AUXADC_CON3_UNION.AUXADC_CON3_CELLS.SOFT_RST = 0;

    /* Reference voltage selection */
    *(uint32_t *)(0xA2070400) |= 0x100;

#ifdef HAL_ADC_CALIBRATION_ENABLE
    uint8_t otp_flag = 0,otp_flag2 = 0,otp_flag3=0;
    uint8_t otp_data[8] = {0};
    uint8_t otp_data_24[3]={0};
    uint16_t otp_data_GE=0,otp_data_OE=0;
    int8_t flash_ret = -1;

    /* if otp_flag == 0,read otp data from 156,else form 146 */
    if (0 == hal_flash_otp_read(176, &otp_flag, 1)) {
        if (0 == otp_flag) {
                  if(0 != hal_flash_otp_read(29,&otp_flag2,1)){
                      ADC_DEBUG_INF("flash_read_29_error");
                  }
                  if(otp_flag2 ==0xFF )
                  {   ADC_DEBUG_INF("otp29=oxff,flash_read_otp_154 value");
                      flash_ret = hal_flash_otp_read(154,otp_data,8);
                  }
                  else{
                      ADC_DEBUG_INF("flash_read_otp_24 value");
                       flash_ret= hal_flash_otp_read(24,otp_data_24,3);
                       otp_data_GE= otp_data_24[1];
                       otp_data_GE=(otp_data_GE<<8)|otp_data_24[2];
                       otp_data_GE&=0x3FF;
                       otp_data_OE = otp_data_24[0];
                       otp_data_OE = (otp_data_OE<<8)|otp_data_24[1];
                       otp_data_OE = (otp_data_OE>>2)&0X1FF;
                       ADC_DEBUG_INF("GE=%d,OE=%d\r\n",otp_data_GE,otp_data_OE);
                       adc_a = (double)(otp_data_GE-256)/4096.0;
                       adc_b = (double)(otp_data_OE-128)/4096.0;
                       ADC_DEBUG_INF("adc_a = %lf,adc_b = %lf\r\n", adc_a, adc_b);
                       g_calibration_status = Calibration_one;
                       return HAL_ADC_STATUS_OK;
                  }
        } else {
                if(0 != hal_flash_otp_read(156,&otp_flag3,1))
                {
                    ADC_DEBUG_INF("flash_read_156_error");
                }
                if(otp_flag3 == 0xFF)
                {    ADC_DEBUG_INF("flash_read_146 value");
                    flash_ret = hal_flash_otp_read(146, otp_data, 8);
                }else{
                    ADC_DEBUG_INF("flash_read_154 value");
                    flash_ret = hal_flash_otp_read(154, otp_data, 8);
                }
        }
        if (0 != flash_ret) {
            ADC_DEBUG_INF("read flash error\r\n");
            adc_slope = 0;
            return HAL_ADC_STATUS_OK;
        }

        adc_slope = (double)((1.0 * (otp_data[7] * 256 + otp_data[6]) - 1.0 * (otp_data[3] * 256 + otp_data[2]))
                             / (1.0 * (otp_data[5] * 256 + otp_data[4]) - 1.0 * (otp_data[1] * 256 + otp_data[0])));
        adc_slope = adc_slope * 1000;

        ADC_DEBUG_INF(" adc_slope(2925)= %lf\r\n", adc_slope);

        X1 = (otp_data[1] * 256 + otp_data[0]);
        Y1 = (otp_data[3] * 256 + otp_data[2]);

        /* X1 unit is mv */
        X1 = X1 / 1000;

        ADC_DEBUG_INF("x1 = %lf,y1 = %lf\r\n", X1, Y1);

    } else {
        ADC_DEBUG_INF("read flash error\r\n");
        adc_slope = 0;
    }

#endif
    return HAL_ADC_STATUS_OK;
}

hal_adc_status_t hal_adc_deinit(void)
{

    /* Disable clock: *(volatile uint32_t *)(0xA2030b10) |= 0x200000; */
    if (HAL_CLOCK_STATUS_OK != hal_clock_disable(HAL_CLOCK_CG_AUXADC)) {
        log_hal_msgid_error("\r\n [ADC] Clock disable failed!", 0);
        return HAL_ADC_STATUS_ERROR;
    }

    /* Disable AUXADC */
    ADC->ANA_EN_CON = 0;

    /* Disable VA28_LDO */
    ADC->MACRO_CON2 = 0;

    /* Change status to idle */
    g_adc_status = ADC_IDLE;

    return HAL_ADC_STATUS_OK;
}

hal_adc_status_t hal_adc_get_data_polling(hal_adc_channel_t channel, uint32_t *data)
{

    uint32_t times = 0;
    uint32_t temp_data = 0;

    /* Channel is invalid */
    if (channel >= HAL_ADC_CHANNEL_MAX) {
        log_hal_msgid_error("\r\n [ADC] Invalid channel: %d.", 1, channel);
        return HAL_ADC_STATUS_ERROR_CHANNEL;
    }

    /* Parameter check */
    if (data == NULL) {
        log_hal_msgid_error("\r\n [ADC] Invalid parameter.", 0);
        return HAL_ADC_STATUS_INVALID_PARAMETER;
    }

    /* changed by mtk60731 on 2020.2.10: from DE descrption,1562 adc gpio default as DGPIO,so,need to switch it to
      * AGPIO,otherwise adc will not work.
      */
    *((volatile uint32_t *)(0xA20C0038)) |= 1 << (6 - channel);

    for (times = 0; times < 8; times++) {
        /* Disable the corresponding region */
        ADC->AUXADC_CON1 = 0;
        ADC->AUXADC_CON1 = (1 << (uint16_t)channel);

        /* Wait until the module status is idle */
        while (ADC->AUXADC_CON3_UNION.AUXADC_CON3_CELLS.ADC_STAT & AUXADC_CON3_ADC_STA_MASK);

        /* Retrieve data for corresponding channel */
        switch (channel) {
            case HAL_ADC_CHANNEL_0:
                temp_data += ADC->AUXADC_DATA0;
                break;
            case HAL_ADC_CHANNEL_1:
                temp_data += ADC->AUXADC_DATA1;
                break;
            case HAL_ADC_CHANNEL_2:
                temp_data += ADC->AUXADC_DATA2;
                break;
            case HAL_ADC_CHANNEL_3:
                temp_data += ADC->AUXADC_DATA3;
                break;
            default:
                /* Should not run here */
                break;
        }
    }

    *data = (temp_data >> 3);

    /* Switch GPIO to digital mode after sample done */
    *((volatile uint32_t *)(0xA20C0034)) |= 1 << (6 - channel);;

    return HAL_ADC_STATUS_OK;
}

hal_adc_status_t hal_adc_get_average_data(hal_adc_channel_t channel, hal_adc_average_t average_num, uint32_t *data)
{
    /* Channel is invalid */
    if (channel >= HAL_ADC_CHANNEL_MAX) {
        log_hal_msgid_error("\r\n [ADC] Invalid channel: %d.", 1, channel);
        return HAL_ADC_STATUS_ERROR_CHANNEL;
    }

    /* Parameter check */
    if (data == NULL) {
        log_hal_msgid_error("\r\n [ADC] Invalid parameter.", 0);
        return HAL_ADC_STATUS_INVALID_PARAMETER;
    }

    if (average_num >= HAL_ADC_AVERAGE_MAX) {
        log_hal_msgid_error("\r\n [ADC] Invalid parameter.", 0);
        return HAL_ADC_STATUS_INVALID_PARAMETER;
    }

    /* changed by mtk60731 on 2020.2.10: from DE descrption,1562 adc gpio default as DGPIO,so,need to switch it to
      * AGPIO,otherwise adc will not work.
      */
    *((volatile uint32_t *)(0xA20C0038)) |= 1 << (6 - channel);

    /* Disable the corresponding channel */
    ADC->AUXADC_CON1 = 0;

    /* Enable auto average */
    ADC->AUXADC_CON4 |= (1 << 8);

    /* Set average number */
    ADC->AUXADC_AVG_NUM = average_num;

    /* Start auxadc */
    ADC->AUXADC_CON1 = (1 << (uint16_t)channel);

    /* Wait until the module status is ready */
    while (ADC->AUXADC_AVG_READY == 0);

    /* Retrieve data for corresponding channel */
    *data = ADC->AUXADC_AVG_DATA >> 3;

    /* Switch GPIO to digital mode after sample done */
    *((volatile uint32_t *)(0xA20C0034)) |= 1 << (6 - channel);;

    /* change by suchuan on 2020.02.20:from DE descrption,when data ready flag set */
    /* Disable auto average */
    //ADC->AUXADC_CON4 &= ~(1 << 8);
    return HAL_ADC_STATUS_OK;
}

#ifdef HAL_SLEEP_MANAGER_ENABLED
void adc_backup_all_register(void)
{
    s_macro_con2 = ADC->MACRO_CON2;
    s_ana_en_con = ADC->ANA_EN_CON;

}

void adc_restore_all_register(void)
{
    ADC->MACRO_CON2 = s_macro_con2;
    ADC->ANA_EN_CON = s_ana_en_con;
}
#endif

#ifdef HAL_ADC_CALIBRATION_ENABLE

hal_adc_status_t hal_adc_get_calibraton_data(uint32_t raw_data, uint32_t  *cal_data)
{
    int32_t temp_cal_data = 0;
    if (NULL == cal_data) {
        return HAL_ADC_STATUS_INVALID_PARAMETER;
    }

    if(g_calibration_status == Calibration_one){
        *cal_data = (((double)raw_data / 4096.0 - adc_b) * 1.4 / (1.0 + adc_a))*2925;
        return HAL_ADC_STATUS_OK;
    }

    if (adc_slope == 0) {
        *cal_data = raw_data;
        return HAL_ADC_STATUS_OK;
    }
    temp_cal_data = (((double)raw_data - Y1  + adc_slope * X1) / adc_slope) * 2925;
    if (0 > temp_cal_data) {
        *cal_data = -temp_cal_data;
    } else {
        *cal_data = temp_cal_data;
    }

    return HAL_ADC_STATUS_OK;
}

hal_adc_status_t hal_adc_get_calibraton_voltage(uint32_t raw_data, uint32_t *cal_voltage)
{
    if (NULL == cal_voltage) {
        return HAL_ADC_STATUS_INVALID_PARAMETER;
    }
    if(g_calibration_status == Calibration_one){
        *cal_voltage = ((double)raw_data / 4096 - adc_b) * 1.4 / (1.0 + adc_a)*1000;
        return HAL_ADC_STATUS_OK;
    }
    if (adc_slope == 0) {
        *cal_voltage = raw_data * 1400 / 4095;
        return HAL_ADC_STATUS_OK;
    }

    /* unit is mv */
    *cal_voltage = ((double)raw_data - Y1  + adc_slope * X1) / adc_slope * 1000;
    return HAL_ADC_STATUS_OK;
}

#endif /* AUXADC_CALIBRATION_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_MODULE_ENABLED */

