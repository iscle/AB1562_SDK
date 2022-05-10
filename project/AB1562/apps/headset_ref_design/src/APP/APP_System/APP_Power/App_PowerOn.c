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
#include "App_Media.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "Pairing_NvkeyHandler.h"
#include "App_ReConnection.h"
#include "App_Connection_NVKeyHandler.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#endif
#ifdef AIRAPP_Profile
#include "APP_AirApp.h"
#endif
#include "AudioDSP_AncPassThru.h"
#include "System_Nvkey.h"
#include "key.h"
#include "App_Ble.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
    BOOL enterDiscoverable;
}APP_POWERON_CTL_STRU;

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_POWERON_CTL_STRU gPowerOnCtl;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static BOOL app_PowerOn_SetActionWhenPowerOnInLowBat(U8 actionValue);
static BOOL app_PowerOn_IsEnterDiscoverable(void);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_PowerOn_SetActionWhenPowerOnInLowBat(U8 actionValue)
{
	if(actionValue == ACTION_POWER_ON_PROHIBITED)
	{
		return TRUE;
	}

	if(actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_UNTIL_BAT_OK || actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK)
	{
		APP_SetNoConnectivityPwOnLowBat(TRUE);
	}

	if(actionValue == ACTION_POWER_ON_NO_VALID_KEYS_UNTIL_BAT_OK|| actionValue == ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK)
	{
		APP_SetKeyInvalidPwOnLowBat(TRUE);
	}

	return FALSE;
}

static BOOL app_PowerOn_IsEnterDiscoverable(void)
{
	return gPowerOnCtl.enterDiscoverable;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_PowerOn_Loader(void)
{
	DBG_LOG_APP_SYSTEM( "[APP_PWRON] APP_SYSTEM_PowerOnHandler, power on:%d", 1, APP_PowerOff_IsPowerOn());

	if(APP_PowerOff_IsPowerOn())
	{
		return;
	}

	if(APP_ChgBat_IsBelowLowLevel())
	{
		if(app_PowerOn_SetActionWhenPowerOnInLowBat(APP_ChgBat_GetPwrOnLowBatAction()))
		{
			return;
		}
	}

#if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
	#ifndef MMI_LE_ENABLE_ADV   //if MMI_LE_ENABLE_ADV not define, enable LE adv after power on
/*
	if(pMpParameterF->leBDAddr.type == LE_PUBLIC_DEVICE_ADDR)
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_ADV_PARAM_SEND_CMD);
    }
    else
    {
        MMI_LE_SetState(MMI_LE_ROM_CONFIG, MMI_LE_ROM_SET_RANDOM_ADDRESS_SEND_CMD);
    }
*/
	#endif
#endif

	APP_SetPowerOnStatus(TRUE);

	APP_State_CancelTimer(PREVENT_AUTO_POWER_ON_TIMER);
	APP_PowerOff_CancelTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
	APP_State_ReleaseTimerSystemOff();
	APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);

	if(APP_PowerNvkey_GetTimer(TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT))
	{
		APP_PowerOff_SetTimer(TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT);
	}

	if(APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF) || APP_State_CheckNestStateByLink(APP_EOF, APP_OFF))
	{
		APP_State_RmvState(APP_EOF, APP_FAKEOFF);
		APP_State_RmvState(APP_EOF, APP_OFF);
		if(APP_FAKEON != APP_State_GetTopState(APP_EOF))
			APP_State_AddTopState(APP_EOF, APP_FAKEON);
		APP_State_ReleaseTimerSystemOff();
	}


	APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);

//	MMI_GAP_WritePairingMode();
//	MMI_GAP_WriteSecurityMode();
//	MMI_HCI_SendWriteSimplePairingDebugMode(MMI_DEBUG_PRIVATE_KEY_FEAT);

    APP_ChgBat_CheckBatStatus();
    if((APP_ChgBat_GetBatStatus() == APP_BAT_CHARGER_IN && APP_ChgBat_IsFeatureSupported(APP_POWER_OFF_WHEN_CHARGING_IN_INTR)) || APP_Battery_IsBelowInitBat())
    {
        DBG_LOG_APP_SYSTEM( "[APP_PWRON] Charger In Power Off or init low bat", 0);
        //if(app_ChgBat_IsBelowInitPercent())
        {
            APP_PowerOff_SystemOff();
            return;
        }
    }

	//Not watch dog reset
	if(!APP_State_CheckTimer(WATCHDOG_REBOOT_NO_MEDIA_TIMER))
	{
		//Play startup media
		switch(APP_ChgBat_GetLevelInQuarter())
		{
			case IND_BATTERY_LOW:
			case IND_BATTERY_INTERVAL_0:
				APP_Media_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT1);
				break;
			case IND_BATTERY_INTERVAL_1:
				APP_Media_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT2);
				break;
			case IND_BATTERY_INTERVAL_2:
				APP_Media_PushMediaEvent(MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT3);
				break;
			case IND_BATTERY_INTERVAL_3:
				APP_Media_PushMediaEvent(MEDIA_EVT_POWER_ON);
				break;
			default:
				FW_Assert(FALSE);
				break;
		}
		APP_LinkKey_ClearConnectedFlag();
	}

	//Added by Eric Chou, 2011.1.13
	//New Feature - "MMI_INIT_BAT_CHECK_ON_POWER_UP_FEAT"
	//Send media event of current battery level indication on power up.
	if(APP_ChgBat_IsFeatureSupported(APP_INIT_BAT_CHECK_ON_POWER_UP_FEAT))
	{
		APP_ChgBat_CheckCurrentBatLevel();
	}

	APP_ChgBat_SetBatterMonitorTimer();

//	DRIVER_ResetLineInKeyTimer();

	APP_State_ConnectabilityHandle(CMD_SECURITY_MODE_COMPLETE);
	#if FPGA
	APP_PowerOn_SetEnterDiscoverable(TRUE);
	#elif BRINGUP_1562
	APP_PowerOn_SetEnterDiscoverable(TRUE);
	#endif
	if(APP_System_IsMpTestMode() || APP_PairNvkey_IsFeatureOn(APP_ENTER_DISCOVER_AFTER_PWR_ON_FEAT) || app_PowerOn_IsEnterDiscoverable())
	{
		APP_PowerOn_SetEnterDiscoverable(FALSE);
		if(APP_System_IsMpTestMode() ||
			(!(APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT && APP_State_CheckInLineInState()) &&
			!(APP_NO_DISCAVERABLE_IN_FM_FEAT && APP_State_CheckInFMState())))
		{
			APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
		}
	}
	else
	{
		APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
	}

	#if !FPGA
	if(APP_AUTO_RECONN_AFTER_PWR_ON_FEAT)
	#endif
	{
		APP_ReConn_Reconnect(RECONNECT_PWR_ON);
	}

	APP_Ble_StartGattService();
    
    APP_Ble_EnableAdv(APP_Ble_GetAdvMask());

	//#ifdef OLED_ENABLE
	//OLED_DisplayOn(OLED_SHOW_START);
	//#endif

	AudioDsp_ANCPassThruOnByNVKeySettings();
	if(System_ReadNvkeyReconnFailDiscTimer())
		APP_State_SetTimer(RECONN_FAIL_ENTER_DISC_TIMER, System_ReadNvkeyReconnFailDiscTimer() * MILLI_SEC_100);

#ifdef LINEIN_ENABLE
    DBG_LOG_APP_SYSTEM("[APP_PWRON] Detecing for line in toggle pin", 0);
    key_reset_toggle_timer();
#endif
	if(APP_AirApp_FOTA_Reconnect())
		APP_Reconn_SetDelayTimer(RECONNECT_WATCHDOG, 100);
}

void APP_PowerOn_SetEnterDiscoverable(BOOL isOn)
{
	gPowerOnCtl.enterDiscoverable = isOn;
}
