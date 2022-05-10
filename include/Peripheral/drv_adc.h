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

#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_

#include "message.h"

/**
 * @addtogroup Peripheral
 * @{
 * This section introduces the Peripheral APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*! 
     
     @defgroup PAGE_API_DRIVER_ADC drv_adc.h
     @{
        @page PAGE_API_DRIVER_ADC drv_adc.h 
        Drv_adc.h is a driver for analog to digital converter.
        
*/

/******************************************************************************
 * Type Definitions
 ******************************************************************************/
/*!
	@brief ADC channel selection. Channel 0 ~ 31.
*/
typedef enum
{
    AIO_0,            	//ADC Channel_0
    AIO_1,            	//ADC Channe_1
    AIO_2,            	//ADC Channe_2
    AIO_3,            	//ADC Channe_3
    AIO_4,            	//ADC Channe_4
    AIO_5,   			//ADC Channe_5
    AIO_6,   			//ADC Channe_6
    AIO_7,   			//ADC Channe_7
    AIO_8,  			//ADC Channe_8
    AIO_9,   			//ADC Channe_9
    AIO_10,   			//ADC Channe_10
    AIO_11,   			//ADC Channe_11
    AIO_12,  			//ADC Channe_12
    AIO_13,   			//ADC Channe_13
    AIO_14,   			//ADC Channe_14
    AIO_15,   			//ADC Channe_15
    AIO_16,   			//ADC Channe_16
    AIO_17,   			//ADC Channe_17
    VCCIO,   			//ADC Channe_18
    AVSS,   			//ADC Channe_19
    AIO_28,   			//ADC Channe_20
    AIO_29,   			//ADC Channe_21
    AIO_30,   			//ADC Channe_22
    AIO_31,   			//ADC Channe_23
	ANC_L_DCOC,			//ADC Channe_24
    VCCDIG,				//ADC Channe_25
    ANC_R_DCOC,			//ADC Channe_26
    VBAT_SCALE_1DIV5,	//ADC Channe_27
    VBAT_SCALE_2DIV5,	//ADC Channe_28
    AIOADC_PWR,			//ADC Channe_29
    VCM,				//ADC Channe_30
    V_TEMP,				//ADC Channe_31
    RESERVED,
}ADC_SOURCE_SEL;


/******************************************************************************
 * Macro Definition
 ******************************************************************************/
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
#define MESSAGE_ADC_DATA MESSAGE_ADC_BASE

typedef struct _ADC_Data{
	U32 Channel;
	U32 ADC_Value;
}ADC_Data;

typedef struct _adc_handler_data
{
    HandlerData adc_handler;
} ADCHandlerData;



/*!
    @brief ADC calibration is a HW mechanism. It shoud be done before ADC value be read.
    @param None
    @return None.
*/
//void DRV_ADC_Calibration(void);
/*!
    @brief Get the ADC value.
    For convenience, this function will switch MUX internally. There's no need to call DRV_PinMux_AioSet first.        
    @param sel ADC channel selection. Channel 0~31.
    @return A 16 bit data.
*/

U16 DRV_ADC_Read(ADC_SOURCE_SEL sel);
/*!
    @}
*/

/*!
    @}
*/
#endif
