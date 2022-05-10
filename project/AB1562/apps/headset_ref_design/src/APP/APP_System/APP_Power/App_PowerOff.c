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
#include "system.h"
#include "App_ResourceManager.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "App_System.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_Media.h"
#include "App_ChargeBattery.h"
#include "App_LinkKey.h"
#include "App_Pairing.h"
#include "Pairing_NvkeyHandler.h"
#include "App_Nvram_Type.h"
#include "AudioDSP_Registration.h"
#include "App_RaceCmd.h"
#include "app_nvram_drivervariation_nvstruc.h"
#include "App_ChargeBattery.h"
#include "Volume_NvkeyHandler.h"
#include "App_VolumeAiroThruProcess.h"
#include "AudioDSP_StreamManager.h"
#include "DSP_Control.h"
#include "App_PowerOn.h"
#include "App_A2dp.h"
#include "drv_anc.h"
#include "AirApp.h"
#include "App_Ble.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SYS_POWERED_OFF,
	SYS_POWERED_ON,
	SYS_POWERING_OFF,
	//APP_KEY_STATE_POWER_OFF_SYNC,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 powerOnOffStatus;
	U16 autoSwitchOffMask[FW_MAX_LINK_IDX];
}APP_POWER_CTL_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_PowerOff_TimerHandler(Handler t, U16 id, void *payload, U32 id_ext);
static void app_PowerOff_AutoSwitchOffTimeoutHandler(void);
static void app_PowerOff_SetPoweredOnOffStatus(U8 status);
static bool app_PowerOff_CheckAutoPowerOff(void);
static BOOL app_PowerOff_IsAllSwitchOffMaskNotSet(void);
static void app_PowerOff_SetAutoSwitchOffByLink(U16 bitMask, bool isBitAdded, BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData gPowerOffTimerHandlerData;
static APP_POWER_CTL_STRU gAppPowerCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U32 app_PowerOff_TimerHandler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(payload);
	UNUSED(handler);
	UNUSED(id_ext);

	switch(id)
	{
		case TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT:

			break;
		case TIMER_AUTO_SWITCH_OFF_TIMEOUT:
			app_PowerOff_AutoSwitchOffTimeoutHandler();
			break;
		case TIMER_SET_HW_POWER_OFF_TIMEOUT:
			DBG_LOG_APP_SYSTEM( "[APP_PWROFF] POWER_OFF_TIMEOUT!!!!!!!", 0);
			APP_PowerOff_Proce();
			break;
		case TIMER_SET_HW_RESET_TIMEOUT:
			APP_Reset();
			break;
		default:
			FW_Assert(FALSE);
			break;
	}

	return 0;
}

static void app_PowerOff_AutoSwitchOffTimeoutHandler(void)
{
	APP_PowerOff_Loader(POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER);
}

static void app_PowerOff_SetPoweredOnOffStatus(U8 status) //MMI_SetKeyState
{
	gAppPowerCtl.powerOnOffStatus = status;
	PM_SetPowerOff((status != SYS_POWERED_ON) ? TRUE : FALSE);
	PM_SetRejectConnectInd((status != SYS_POWERED_ON) ? TRUE : FALSE);
}

static bool app_PowerOff_CheckAutoPowerOff(void)
{
#ifdef ANC_FF_MMI_CTRL
	if(drv_anc_is_playing())
	{
		return FALSE;
	}
#endif//ANC_FF_MMI_CTRL
	return TRUE;
}

static BOOL app_PowerOff_IsAllSwitchOffMaskNotSet(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(gAppPowerCtl.autoSwitchOffMask[linkIndex] != 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

static void app_PowerOff_SetAutoSwitchOffByLink(U16 bitMask, bool isBitAdded, BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if((pBdAddr == APP_EOF) || (APP_GetLinkIndexByBdAddr(pBdAddr) == linkIndex))
		{
			if(isBitAdded)
			{
				gAppPowerCtl.autoSwitchOffMask[linkIndex] |= bitMask;
			}
			else
			{
				gAppPowerCtl.autoSwitchOffMask[linkIndex] &= ~bitMask;
			}
		}
	}
}


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_PowerOff_Init(void)
{
	gPowerOffTimerHandlerData.handler = app_PowerOff_TimerHandler;
}

void APP_PowerOff_Loader(U8 reason)
{
	if(!APP_PowerOff_IsPowerOn())
	{
		return;
	}

	DBG_LOG_APP_SYSTEM( "[APP_PWROFF] Power Off Loader. Reason:%d, FakeOffState:%d, IsPowerOn:%d", 3, reason, APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF), APP_PowerOff_IsPowerOn());

	switch(reason)
	{
		case POWEROFF_REASON_KEY:
			break;
		case POWEROFF_REASON_SHUT_DOWN_LOW_BAT:
			break;
		case POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER:
			break;
		case POWEROFF_REASON_PAIRING_MODE_TO:
			break;
		case POWEROFF_REASON_TEMP_PROTECT:
			break;
		case POWEROFF_REASON_CHARGING_CASE:
			break;

		default:

			break;
	}

	APP_A2dp_GameModeOff();
	APP_AudioDspClose();
	APP_AudioDeviceDeRegisterAllDevice();
	APP_State_PowerOffHandle();
	APP_VolumeNvkey_WriteDefaultSoundLevel(COMPONENT_AIROTHRU_SPK, APP_VolAiroThru_GetCurrentSoundLevel());
	app_PowerOff_SetPoweredOnOffStatus(SYS_POWERING_OFF);
	if(!AirApp_IsFOTASuccess())
	{
		APP_ReleaseDspSuspend();
		DBG_LOG_APP_SYSTEM( "[APP_PWROFF] DisConBLE!!!", 0);
		APP_Ble_DisableAdv(BLE_ADV_PRIMARY_MASK);
        #ifdef XIAOAI_DEVELOPMENT
        APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
        #endif
	}


//    #if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
//    MMI_LE_DisconnectPowerOff();
//    #endif

	APP_Media_PowerOffHandle(FALSE);
	APP_ChgBat_PowerOffHandler();
	APP_SetPowerOnStatus(FALSE);

	if(APP_Conn_PowerOffHandler())
	{
		APP_PowerOff_SetTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
	}
	else
	{
		APP_PowerOff_Proce();
	}
}

void APP_PowerOff_Proce(void)
{
	U8 linkIndex;

	APP_SetWatchDogReboot(FALSE);
	APP_State_CleanNestState(APP_EOF);

	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN)
	{//Charging battery Now
		APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
	}
	else
	{
		APP_State_AddTopState(APP_EOF, APP_OFF);
	}

	//Release current running timers.
	APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_TIMEOUT);//SYS_ReleaseTimer(&gAppCtl.mmiTimerInfo.sysTimer.pairModeTimer);
	APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
	APP_PowerOff_CancelTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
	APP_PowerOff_CancelTimer(TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT);
	APP_Pairing_CancelTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);//SYS_ReleaseTimer(&gAppCtl.mmiTimerInfo.sysTimer.connectableTimeoutTimer);

	APP_ReConn_ClearCtl();
	APP_LinkKey_ClearConnectedFlag();

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		gAppCtl.appInfo[linkIndex].subState = APP_NONE_SUBSTATE;
		gAppCtl.appInfo[linkIndex].stateEntryExit = 0;
	}
}

void APP_PowerOff_SetTimer(U8 timerType)
{
	switch(timerType)
	{
		case TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT:
			FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, APP_PowerNvkey_GetTimer(timerType)*ONE_SEC);
			break;
		case TIMER_AUTO_SWITCH_OFF_TIMEOUT:
			if(APP_PowerNvkey_GetTimer(timerType) && !APP_PowerOff_IsTimerSet(timerType))
			{
				FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, APP_PowerNvkey_GetTimer(timerType)*ONE_SEC);
			}
			break;
		case TIMER_SET_HW_POWER_OFF_TIMEOUT:
			if(app_PowerOff_CheckAutoPowerOff())
			{
				FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, APP_PowerNvkey_GetTimer(timerType)*ONE_SEC);
			}
			break;
		case TIMER_SET_HW_RESET_TIMEOUT:
			FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, APP_PowerNvkey_GetTimer(timerType)*ONE_SEC);
			break;
		default:
			FW_Assert(FALSE);
			break;
	}

	return;
}

void APP_PowerOff_CancelTimer(U8 timerType)
{
	MSG_MessageCancelAll(&gPowerOffTimerHandlerData, timerType);
	return;
}

bool APP_PowerOff_IsTimerSet(U8 timerType)
{
	return MSG_MessageSearchAllEx(&gPowerOffTimerHandlerData, timerType, 0);
}

void APP_SetPowerOnStatus(BOOL isOn)
{
	app_PowerOff_SetPoweredOnOffStatus((isOn)?SYS_POWERED_ON:SYS_POWERED_OFF);
	if(isOn)
		SYS_SET_POWER_STATE();
	else
		SYS_CLEAR_POWER_STATE();
}

U8 APP_PowerOff_IsPowerOn(void)
{
	return (gAppPowerCtl.powerOnOffStatus == SYS_POWERED_ON)?TRUE:FALSE;
}

void APP_PowerOff_SystemOff(void)
{
    DSP_AudioCommand_AudioOff();

	DBG_LOG_APP_SYSTEM( "[APP_PWROFF] System off, charger in:%d, power on:%d", 2, APP_ChgBat_IsChargerIn(), APP_PowerOff_IsPowerOn());

	if(APP_PowerOff_IsPowerOn())
	{
		return;
	}

	if(APP_POWER_KEY_TO_DEEP_SLEEP_FEAT)
	{
        APP_SendRaceCmd(RACE_SET_CONSTRAINED_POWER_LEVEL, NULL, 0);//deep sleep
		APP_State_ReleaseTimerSystemOff();
	}
	else
	{
		if(APP_ChgBat_IsChargerIn())
		{
			APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
		}
		else if(APP_ChgBat_IsPowerOnNeeded())
		{
			APP_ChgBat_ClearPowerOffState();
			APP_PowerOn_Loader();
		}
		else
		{
			//There are following reasons causing power off fail.
			//1. User press pio_0: mcu will shut down in 1xx us.
			//2. charger in: mcu will go into shut down. (CHG abnormal state)

			if(APP_RESET_INSTEAD_OF_POWEROFF_FOR_REGRESSION_TEST)
			{
				APP_SendRaceCmd(RACE_HARDWARE_RESET, NULL, 0);//reset //MMI_Reset();
			}
			else
			{
				//turn off power
				APP_SendRaceCmd(RACE_POWER_OFF, NULL, 0);//power off
			}
			APP_State_ReleaseTimerSystemOff();
		}
	}
}

void APP_PowerOff_SetAutoSwitchOffBit(U16 bitMask, bool isBitAdded, BD_ADDR_T *pBdAddr)
{
	if(isBitAdded == TRUE)
	{
		if(app_PowerOff_IsAllSwitchOffMaskNotSet())
		{
			APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
		}

		app_PowerOff_SetAutoSwitchOffByLink(bitMask, isBitAdded, pBdAddr);
	}
	else
	{
		app_PowerOff_SetAutoSwitchOffByLink(bitMask, isBitAdded, pBdAddr);

		if(app_PowerOff_IsAllSwitchOffMaskNotSet())
		{
			APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
		}
	}
}

void APP_Reset(void)
{
	APP_SetWatchDogReboot(FALSE);

	APP_LinkKey_ClearConnectedFlag();

	APP_SendRaceCmd(RACE_HARDWARE_RESET, NULL, 0);
}

void APP_Reset_Loader(void)
{
	APP_AudioDeviceDeRegisterAllDevice();
	APP_State_PowerOffHandle();

	app_PowerOff_SetPoweredOnOffStatus(SYS_POWERED_OFF);

//    #if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
//    MMI_LE_DisconnectPowerOff();
//    #endif

	APP_Media_PowerOffHandle(TRUE);

	APP_ChgBat_PowerOffHandler();

	if(APP_Conn_PowerOffHandler())
	{
		APP_PowerOff_SetTimer(TIMER_SET_HW_RESET_TIMEOUT);
	}
	else
	{
		APP_Reset();
	}
}

