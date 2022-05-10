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

#ifndef __HAL_ADC_INTERNAL_H__
#define __HAL_ADC_INTERNAL_H__

#include "hal_platform.h"
#include "hal_adc.h"


#ifdef HAL_ADC_MODULE_ENABLED

/*!
     @name ADC average selection
     @{
 */
#define ADC_AVG_16          0
#define ADC_AVG_64          1
#define ADC_AVG_256         2
#define ADC_AVG_1024        4
#define ADC_AVG_2048        5
#define ADC_AVG_4096        6
#define ADC_AVG_8192        7

/*!
     @}
*/
/*!
     @name ADC enable delay
     @{
 */
#define ADC_DELAY_200T      0
#define ADC_DELAY_250T      1
#define ADC_DELAY_300T      2
#define ADC_DELAY_350T      3
/*!
     @}
*/
/*!
     @name ADC reset rising to start sample adc1 timing interval
     @{
 */
#define ADC_RESET_SAMPLE_INTVL_20T      0
#define ADC_RESET_SAMPLE_INTVL_30T      1
#define ADC_RESET_SAMPLE_INTVL_40T      2
#define ADC_RESET_SAMPLE_INTVL_50T      3
/*!
     @}
*/

void adc_ana_clk_enable(void);
void adc_ana_clk_disable(void);
void adc_dig_clk_enable(void);
void adc_dig_clk_disable(void);
void adc_ana_set_config(void);
void adc_dig_set_config(void);
void adc_set_channel(hal_adc_channel_t channel);
void adc_start(void);
void adc_channel_mask_enable(hal_adc_channel_t channel);
uint32_t adc_get_running_status(void);
uint32_t adc_read_adc_data(hal_adc_channel_t channel);


#endif /*HAL_ADC_MODULE_ENABLED*/
#endif /*__HAL_ADC_INTERNAL_H__*/

