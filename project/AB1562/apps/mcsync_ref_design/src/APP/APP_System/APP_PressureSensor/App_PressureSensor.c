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


#include "bt.h"
#include "App_ResourceManager.h"
#include "App_PowerOn.h"
#include "App_System.h"
#include "App_Avrcp.h"
#include "App_A2dp.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_Nvram_Type.h"
#include "App_LinkKey.h"
#include "App_ChargeBattery.h"
#include "App_Battery.h"
#include "App_Media.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "Pairing_NvkeyHandler.h"
#include "App_ReConnection.h"
#include "App_Connection_NVKeyHandler.h"
#include "BtMCSync.h"
#include "App_MCSync.h"
#include "App_SmartChgCase.h"

#include "App_MCSync_System.h"

#include "App_Mcsync_Reconnection.h"
#include "App_InEarDetection.h"
#include "App_AEKey.h"

#include "App_Customer.h"
#include "System_Nvkey.h"

#include "App_PressureSensor.h"
#include "app_system_pressure_sensor_nvstruc.h"

#include "message_system.h"

<<<<<<< HEAD
#ifdef PRESSURE_SENSOR
=======
#ifdef AIR_CAPTOUCH_PRESSURE_SENSOR_ENABLE
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Structure
**************************************************************************************************/

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_Pressure_Sensor_IndHandler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static PRESSURE_SENSOR_DATA_STRU *gpAppPressureSensorConfig;
static const HandlerData gAppPressureSensorHandle = { app_Pressure_Sensor_IndHandler };

APP_PRESSURE_SENSOR_STRU gAppPressureSensorCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U32 app_Pressure_Sensor_IndHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	switch(handler_id)
	{
		case 0:
			APP_Media_PushMediaEvent(MEDIA_EVT_FORCE_TOUCH_RELEASE);
			break;
		case 1:
			APP_Media_PushMediaEvent(MEDIA_EVT_FORCE_TOUCH_PRESS);
			break;
	}
	UNUSED(handler); UNUSED(id); UNUSED(msg);
	return 0;
}
/**************************************************************************************************
*Public Functions
**************************************************************************************************/
void APP_Pressure_Sensor_Init(void)
{
	if(NVKEY_GetPayloadLength(NVKEYID_APP_FORCE_TOUCH_SETTING) == sizeof(PRESSURE_SENSOR_DATA_STRU))
	{
		gpAppPressureSensorConfig = (PRESSURE_SENSOR_DATA_STRU* )NVKEY_GetPayloadFlashAddress(NVKEYID_APP_FORCE_TOUCH_SETTING);
	}

	if(gpAppPressureSensorConfig)
	{
		DBG_LOG_APP_SYSTEM( "[APP Pressure Sensor] nvkey init success, en:%d ", 1, gpAppPressureSensorConfig->isEnableForceTouch);

		if(gpAppPressureSensorConfig->isEnableForceTouch)
		{
			gAppPressureSensorCtl.isPressureSensorEnable = TRUE;
			MSG_MessageForceTouchHandler((Handler)&gAppPressureSensorHandle);
		}
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[APP Pressure Sensor] nvkey init fail ", 0);
	}
}
#endif

