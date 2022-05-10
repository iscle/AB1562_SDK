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
 
 
#ifndef _LED_STATE_BG_H_
#define _LED_STATE_BG_H_

#include "App_State.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	LED_BLUE_ALWAYS_ON,					//0
	LED_BLUE_SLOW_FLASH_SHORT_PERIOD,
	LED_BLUE_FAST_FLASH_LONG_PERIOD,
	LED_BLUE_FAST_FLASH_SHORT_PERIOD,
	LED_BLUE_SLOW_FLASH_LONG_PERIOD,
	LED_RED_FAST_FLASH_TWICE,			//5
	LED_BOTH_OFF,
	LED_BOTH_ON,
	LED_ALT_BLUE_THEN_RED,
	LED_BOTH_CONSTANTLY,
	LED_RED_ALWAYS_ON,					//10
	LED_NUM_OF_TYPE
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * @KeyID 0xF205
 * 
 */
 
typedef struct
{
	//The significant bit represents user defined or not.
	U8 entryLedDisplayIndex;
}LedSettingType;

/*NvkeyDefine NVKEYID_APP_STATE_BG_LED_SETTING*/
typedef struct
{
	LedSettingType defaultLedSetting[APP_TOTAL_STATE_NO];		  /**< @Value      @Desc  Led index for each state */ 
	LedSettingType maxLinkLedSetting;							  /**< @Value      @Desc  Led index for max link */
}LED_BG_SETTING_STRU;

/*
LED_BG_SETTING_STRU LED_BG = 
{
	{	//defaultLedSetting
		LED_BOTH_OFF,	 	//APP_OFF
		LED_ALT_BLUE_THEN_RED,				//APP_CONDISCABLE
		LED_ALT_BLUE_THEN_RED,				//APP_CONNECTABLE
		LED_BLUE_FAST_FLASH_LONG_PERIOD,	 	//APP_HFP_CONNECTED
		LED_BLUE_FAST_FLASH_SHORT_PERIOD,  	//INCOMING CALL
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,	 	//OUTGOING
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,		//CALLACTIVE
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,		//CALLACTIVE_WITHOUT_SCO
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,		//APP_HFP_CAIMG
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,	 	//APP_HFP_CAOGG
		LED_BLUE_SLOW_FLASH_LONG_PERIOD,	 	//APP_HFP_CAMULTY
		LED_BOTH_OFF,	 						//FAKE_ON
		LED_BOTH_OFF,	 						//FAKE_OFF
		LED_BOTH_OFF,	 						//DETACH_LINK
		LED_BOTH_CONSTANTLY, 					//TEST MODE
		LED_RED_FAST_FLASH_TWICE, 			//APP_FM,
		LED_RED_FAST_FLASH_TWICE, 			//APP_LINE_IN,
		LED_BOTH_OFF, 						//APP_UPDATING
		LED_BOTH_ON, 							//APP_UPDATED_OK
		LED_ALT_BLUE_THEN_RED, 				//APP_UPDATED_FAIL
		LED_ALT_BLUE_THEN_RED,				//APP_VOICE_PROMPT_LANG_SELECT
		LED_BLUE_FAST_FLASH_SHORT_PERIOD, 	//APP_AWS_PAIRING
		LED_BOTH_OFF,  						//APP_INQUIRY
		LED_BOTH_OFF,  						//APP_MP_STATE
		LED_BOTH_OFF,  						//APP_STATE_RESERVED1
		LED_BOTH_OFF,  						//APP_STATE_RESERVED2
		LED_BOTH_OFF, 						//APP_STATE_RESERVED3
		LED_BOTH_OFF, 						//APP_STATE_RESERVED4
		LED_BOTH_OFF, 						//APP_STATE_RESERVED5
		LED_BOTH_OFF, 						//APP_STATE_RESERVED6
		LED_BOTH_OFF	 						//APP_STATE_RESERVED7
	},
	
	{
		LED_BLUE_FAST_FLASH_LONG_PERIOD			//maxLinkLedSetting
	}
};
*/

#endif
