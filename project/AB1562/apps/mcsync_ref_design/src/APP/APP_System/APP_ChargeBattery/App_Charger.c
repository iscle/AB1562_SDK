/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

#include "message.h" 
#include "chargercase_common.h"
#include "DrvCharger.h"
#include "DrvSmartCase.h"
#include "App_Media.h"
#include "App_ChargeBattery.h"
#include "App_Battery.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_charger_Handler(Handler handler, U16 id, void *msg, U32 ext_id);
 
/**************************************************************************************************
* Variable
**************************************************************************************************/

HandlerData gAppChargerHandle = { app_charger_Handler };

static U32 app_charger_Handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	switch(id)
	{
		case CHARGER_CHARGING_IND:
			APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_IN);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGRIN);
			break;
			
		case CHARGER_CHARGER_COMPLETE_IND:
			APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGING_FULL);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGCPL);
			break;
			
		case CHARGER_CHARGER_OUT_IND:
			APP_ChgBat_UpdateBatStatus((APP_Battery_IsBelowLowLevel())? APP_BAT_LOW : APP_BAT_WELL);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGROUT);
			break;
			
		default:
			break;
	}
	return 0;
}

