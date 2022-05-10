/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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

#include "bt.h"
#include "ntc.h"
#include "App_Media.h"
#include "App_PowerOff.h"

#if NTC_DEVELOPMENT
/**************************************************************************************************
* static function prototype
**************************************************************************************************/
static U32 app_NTC_Handler(Handler handler, U16 id, void *msg, U32 handler_id);
static void app_NTC_LevelChangeChar(NTC_LEVEL_CHANGE_IND_T * msg);
static void app_NTC_LevelChangeUnchar(NTC_LEVEL_CHANGE_UNCHAR_IND_T * msg);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static HandlerData gAppNtcHandle = { app_NTC_Handler };

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_NTC_LevelChangeChar(NTC_LEVEL_CHANGE_IND_T * msg)
{
	switch(msg->level)
	{
		case LEVEL_RISK_LOW:
		case LEVEL_RISK_HIGH:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_RISKY_TEMP);
			break;
		case LEVEL_ABNORMAL_LOW:
		case LEVEL_ABNORMAL_HIGH:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_DISCOUNT_TEMP);
			break;
		//case LEVEL_NORMAL_LOW:
		case LEVEL_NORMAL:
		//case LEVEL_NORMAL_HIGH:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_SECURE_TEMP);
			break;
	}
}

static void app_NTC_LevelChangeUnchar(NTC_LEVEL_CHANGE_UNCHAR_IND_T * msg)
{
	switch(msg->level)
	{
		case LEVEL_UNCHAR_SECURE_LOW:
		case LEVEL_UNCHAR_SECURE_HIGH:
			if(APP_PowerOff_IsPowerOn())
			{
				APP_PowerOff_Loader(POWEROFF_REASON_TEMP_PROTECT);
			}
			else
			{
				APP_PowerOff_SystemOff();
			}
			break;
		case LEVEL_UNCHAR_NORMAL_LOW:
		case LEVEL_UNCHAR_NORMAL:
		case LEVEL_UNCHAR_NORMAL_HIGH:
			break;
	}
}

static U32 app_NTC_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
		case NTC_LEVEL_CHANGE_IND:
			app_NTC_LevelChangeChar((NTC_LEVEL_CHANGE_IND_T *)msg);
			break;
		case NTC_LEVEL_CHANGE_UNCHAR_IND:
			app_NTC_LevelChangeUnchar((NTC_LEVEL_CHANGE_UNCHAR_IND_T *)msg);
			break;
	}

	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_NTC_Init(void)
{
	NTCRegsiterHandler(&gAppNtcHandle);
}
#endif
