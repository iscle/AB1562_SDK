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

#ifndef __DRV_LED_H_
#define __DRV_LED_H_

#include <types.h>


/**
 * @addtogroup Peripheral
 * @{
 * This section introduces the Peripheral APIs including terms and acronyms,
 * supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.
 *
 */

/*! 
     
     @defgroup PAGE_API_DRIVER_LED drv_led.h
     @{
        @page PAGE_API_DRIVER_LED drv_led.h 
        Drv_led.h is an LED drvier which allow user to control LED activity.
        
*/

/******************************************************************************
 * Macros
 ******************************************************************************/
/*!
	LED instance definition.
*/

typedef enum
{

   led0,
   led1,
   led2,
   led3,
   led4,
   led5,   
}led_ins;

typedef enum
{
    current_limitation_1K = 0,
    current_limitation_666,
    current_limitation_333,
    current_limitation_30
} LED_Current_Limitation;

/******************************************************************************
 * Type Definitions
 ******************************************************************************/
#define LED_CURRENT_LIMIT   FALSE
#define DRV_LED_DVT_ENABLE  FALSE
#define DRV_LED_DEBUG_MODE_ENABLE  TRUE
#define DRV_LED_CALI_ENABLE	FALSE


/*!
    @brief Parameters for @ref Drv_LedSet function.
*/

typedef struct
{
    /*! only allow 1 ms ~64 ms */
    U8 unit_ms;
   	/*!the repeat times of T1 & T2 during one complete waveform. */
    U8 t1t2_repeat_cnt;
    /*!unit of t0 (initial wait period), unit base on unit_ms. */
    U8 t0_cnt;				    
    /*!unit of t1 (led on period), unit base on unit_ms */
    U8 t1_cnt;				    
    /*!unit of t2 (led off period), unit base on unit_ms. */
    U8 t2_cnt;
    /*!unit of t3 (final wait period), unit base on unit_ms */
    U8 t3_cnt;    			 
    /*! Repeat foever if set to 1. */
    U8 nostop : 1;			 
    /*! PWM enabe. 1: enable, 0 : disable. */
    U8 pwm_en : 1;			 
    /*! To invert the waveform, 1: invert, 0:none. */
    U8 inverse : 1;		 
    /*!reserved. */
    U8 reserved : 5;		 
    /*!led follow select. Determine which led to follow. */
    led_ins follow_idx;
    /*!led duration select. Determine duration select. */	
	U8 duration_sel;
    /*!led Duty increate step. Determine breathe select. */	
	U8 pwm_onstep;	
    /*!led Duty decreate step. Determine breathe select. */	
	U8 pwm_offstep;	
    /*!led Duty period. Determine breathe select period. */	
	U8 pwm_period;	
} PACKED drv_led_setting;

#if DRV_LED_CALI_ENABLE

typedef struct{
    uint32_t expected_expire_tick;
    uint32_t led_sw_cali_coefficient;
}drv_led_sw_cali_t;.

typedef struct{
    uint8_t con_res_sel;
    uint8_t con_invert;
    uint8_t con_repeat;
    uint8_t con_t1t2_repeat;
    uint8_t unit_xn;
    uint8_t unit_unit;
    uint8_t duration_t0;
    uint8_t duration_t1;
    uint8_t duration_t2;
    uint8_t duration_t3;
    uint8_t pwm_load;
    uint8_t pwm_period;
    uint8_t pwm_duration;
    uint8_t pwm_offstep;
    uint8_t pwm_onstep;
    uint8_t reset_no_repeat;
} isink_config_t;


typedef struct{
    drv_led_sw_cali_t   sw_cali;
    isink_config_t      isink_config;
    uint32_t            isink_idx;
    uint32_t            gpt_handle;
    uint32_t            dly_sta_tm;
    uint32_t            ext_loop_cycle;
    uint8_t             last_repeat;
}drv_isink_private_info_t;
#endif

/******************************************************************************
 * API
 ******************************************************************************/
 
/*!
    @brief The fucntion programs the corresponding LED unit for required operation like LED bliking.
    @param led_idx Index of LED.
    @param setting See structure of drv_led_setting.
    @return None.
*/
EXTERN void DRV_LED_Setting(led_ins led_idx, drv_led_setting *setting);
/*!
    @brief Starting LED activity which related to Drv_LedSet function.
    @param led_idx Index of LED.
    @return None.
*/
EXTERN void DRV_LED_Start(led_ins led_idx);
/*!
    @brief The activity of follow_idx follow the 1st led_idx.
    @param led_idx Index of LED.
    @param follow_idx Follow index of LED.
    @return None.
*/
EXTERN void DRV_LED_StartFollow(led_ins led_idx, led_ins follow_idx);
/*!
    @brief Stop LED activity.
    @param led_idx Index of LED.
    @return None.
*/
EXTERN void DRV_LED_Stop(led_ins led_idx);

/*!
    @brief Read LED enable status.
    @param None.
    @return LED Enable status.
*/
EXTERN U8 DRV_LED_Read_State(void);


/*!
    @brief Set multi_LED Enable.
    @param bitmap mapping to LED,f(led0) mapping to bit0, f(led1) mapping to bit1 ...etc. 
    @bitmap bit vale 0 => Stop, 1 => Start .
    @return None.
*/
EXTERN void DRV_LED_MultiLED_Start(U8 bitmap);


/*!
    @brief Clear multi_LED Enable.
    @param bitmap mapping to LED,f(led0) mapping to bit0, f(led1) mapping to bit1 ...etc. 
    @bitmap bit vale 0 => Stop, 1 => Start .
    @return None.
*/
EXTERN void DRV_LED_MultiLED_Stop(U8 bitmap);


/*!
    @brief set LED iomux mapping to gpio.
    @param ioIndex  mapping pio0~pio31. 
    @ledIndex  LED3 LED4 LED5 need set mapping pio.
    @return errorcode.
*/
EXTERN U32 DRV_LED_SetPIO(U8 ioIndex,U8 ledIndex);


EXTERN void DRV_LED_Init(void);

#if DRV_LED_DVT_ENABLE
BOOL DRV_LED_FOR_HQA2(void );
BOOL DRV_LED_ENABLE_ALL(void );
BOOL DRV_LED_ENABLE_TWO_LED_FOR_SMT(void );
BOOL DRV_LED_T1T2_Repeat(void );
BOOL DRV_LED_T1T2_Repeat_Inverse(void );
BOOL DRV_LED_T1T2_Repeat_PWM(void );
BOOL DRV_LED_T1T2_Repeat_RAMP(void );
BOOL DRV_LED_T1T2_Repeat_RAMP_Durating(void );
#endif

/*!
    @}
*/

/*!
    @}
*/
#endif
