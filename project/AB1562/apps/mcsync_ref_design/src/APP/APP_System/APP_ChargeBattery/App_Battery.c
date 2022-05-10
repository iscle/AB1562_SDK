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

#include "bt.h"
#include "App_Battery.h"
#include "App_ChargeBattery.h"
#include "App_Media.h"
#include "App_PowerOff.h"
#include "drv_battery.h"
#include "App_State.h"
#include "App_Nvram_Type.h"
#include "Battery.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "drv_charger.h"
#include "App_Customer_Func.h"


#define DBG_LOG_APP_BAT(_message, arg_cnt, ...)  LOG_MSGID_I(APP_BAT, _message, arg_cnt, ##__VA_ARGS__)


log_create_module(APP_BAT, PRINT_LEVEL_INFO);


/**************************************************************************************************
* Definition
**************************************************************************************************/
#define CHARGE_COMPLETE_MEDIA_DELAY_TIME 1000 //ms

/**************************************************************************************************
* Enum
**************************************************************************************************/



/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_ChgBat_SetSwitchOffInLowBatTimer(void);
static U8 app_ChgBat_GetTimerValue(U8 timer);
static U8 app_Battery_GetBatInHundredPercent(void);
static U16 app_Battery_GetBatInThousandPercent(void);


/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_CHGBAT_INFO_STRU *pChgBatCtl;
APP_BAT_INFO_STRU gAppBatInfo;
extern HandlerData gAppChgBatTimerHandle;
extern HandlerData gAppChgBatDelayHandle;
/**************************************************************************************************
* Static Functions
**************************************************************************************************/
void APP_ClearActionOfPowerOnInLowBat(void)
{
	if(APP_IsNoConnectivityPwOnLowBat())
	{
		APP_SetNoConnectivityPwOnLowBat(FALSE);
		APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
		APP_State_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
	}
	if(APP_IsKeyInvalidPwOnLowBat())
	{
		APP_SetKeyInvalidPwOnLowBat(FALSE);
//		DRIVER_ResetLineInKeyTimer();
	}
}

static void app_ChgBat_SetSwitchOffInLowBatTimer(void)
{
	if(app_ChgBat_GetTimerValue(TIMER_ID_AUTO_SWITCH_OFF_IN_LOW_BAT) && !FW_CheckTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT, 0))
	{
		FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_AUTO_SWITCH_OFF_IN_LOW_BAT)*ONE_MIN);
	}
}
static U8 app_ChgBat_GetTimerValue(U8 timer)
{
	switch(timer)
	{
		case TIMER_ID_AUTO_SWITCH_OFF_IN_LOW_BAT:
			return pChgBatCtl->timer.autoSwitchOffInLowBatTimer;
		case TIMER_ID_DELAY_CHG_MEDIA_EVT:
			return pChgBatCtl->timer.delayChgMediaEvtTimer;
		case TIMER_ID_CHECK_CHARGING_TIME:
			return pChgBatCtl->timer.checkChargingTimer;
		case TIMER_ID_BATTERY_MONITOR_PERIOD:
			return pChgBatCtl->timer.batteryMonitoringPeriod;
		default:
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

static U8 app_Battery_GetBatInHundredPercent(void)
{
	return Battery_GetLevelInPercent();
}

static U16 app_Battery_GetBatInThousandPercent(void)
{
	return Battery_GetThousandPercent();
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_ChgBat_UpdateBatStatus(U8 batStatus)
{
	if(APP_ChgBat_GetBatStatus() != batStatus)
	{
		DBG_LOG_APP_BAT( "[APP_BAT] status: %s", 1, APP_BatStatusLogString[batStatus]);
		#ifdef AIRAPP_Profile
//		AirApp_ChgBatStatusUpdateEvent(batStatus);
		#endif
		
		APP_Customer_UpdateBatStatus_Adv(batStatus);
	}
	APP_ChgBat_SetBatStatus(batStatus);
}

void APP_ChgBat_PushBatChgMediaEvt(void)
{
	switch(APP_ChgBat_GetBatStatus())
	{
		case APP_BAT_LOW:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW);
			break;

		case APP_BAT_WELL:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGROUT);
			break;

		case APP_BAT_CHARGING_FULL:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGCPL);
			break;

		case APP_BAT_CHARGER_RECHARGE:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_RECHG);
			break;

		case APP_BAT_CHARGER_IN:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGRIN);
			break;
	}

}

void APP_ChgBat_SetDelayChgCplMediaEvt(void)
{
	// If charging in when the battery has been already full, LED have to turn on a while then turn off.
	FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT, NULL, 0, CHARGE_COMPLETE_MEDIA_DELAY_TIME);
}

void APP_ChgBat_ReleaseDelayChgCplMediaEvt(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT, 0);
}

void APP_ChgBat_SetTimeOutDectionTimer(void)
{
	if(app_ChgBat_GetTimerValue(TIMER_ID_CHECK_CHARGING_TIME))
	{
		FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_CHECK_CHARGING_TIME)*1000*60*30); //unit 0.5 hours
	}
}

void APP_ChgBat_SetBatterMonitorTimer(void)
{
	if(app_ChgBat_GetTimerValue(TIMER_ID_BATTERY_MONITOR_PERIOD))
	{
		DBG_LOG_APP_BAT( "[APP_BAT] Monitor Hundred Percent:%d, Thousand Percent:%d", 2, app_Battery_GetBatInHundredPercent(), app_Battery_GetBatInThousandPercent());
		FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_BATTERY_MONITOR_PERIOD, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_BATTERY_MONITOR_PERIOD)*ONE_SEC);
	}
}

void APP_ChgBat_CheckCurrentBatLevel(void)
{
	U8 ledDurationPara;

	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN)
	{	//Already Charger-In do nothing now.
		if(!APP_Battery_IsBelowLowLevel())
		//if(!app_ChgBat_IsBelowLowPercent())
		{
			APP_ClearActionOfPowerOnInLowBat();
		}
	}
	else
	{
		if(APP_Battery_IsBelowShutDownLevel())
		//if(app_ChgBat_IsBelowShutdownPercent())
		{
			APP_ChgBat_UpdateBatStatus(APP_BAT_LOW);
			APP_PowerOff_Loader(POWEROFF_REASON_SHUT_DOWN_LOW_BAT);
		}
		else if(APP_Battery_IsBelowLowLevel())
		//else if(app_ChgBat_IsBelowLowPercent())
		{
			//Under Low battery threshold
			APP_ChgBat_UpdateBatStatus(APP_BAT_LOW);

			if(!FW_CheckTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_LED, 0))
			{
				FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_LED, NULL, 0, (U32)(pChgBatCtl->batteryPara.lowPercentLEDTimer*ONE_SEC));
				if(pChgBatCtl->batteryPara.lowPercentLEDTimer != pChgBatCtl->batteryPara.lowPercentRingTimer)
				{
					FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_RINGTONE, NULL, 0, (U32)(pChgBatCtl->batteryPara.lowPercentRingTimer*ONE_SEC));
				}
				APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW_LED);
				APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);
			}

			app_ChgBat_SetSwitchOffInLowBatTimer();
			APP_ChgBat_PushBatChgMediaEvt();
		}
		else
		{
			APP_ChgBat_UpdateBatStatus(APP_BAT_WELL);
			APP_ClearActionOfPowerOnInLowBat();
		}
	}

	switch(APP_ChgBat_GetLevelInQuarter())
	{
		case IND_BATTERY_LOW:
		case IND_BATTERY_INTERVAL_0:
			ledDurationPara = pChgBatCtl->batteryPara.ledDuration.level0;
			break;
		case IND_BATTERY_INTERVAL_1:
			ledDurationPara = pChgBatCtl->batteryPara.ledDuration.level1;
			break;
		case IND_BATTERY_INTERVAL_2:
			ledDurationPara = pChgBatCtl->batteryPara.ledDuration.level2;
			break;
		case IND_BATTERY_INTERVAL_3:
			ledDurationPara = pChgBatCtl->batteryPara.ledDuration.level3;
			break;
		default:
			return;
	}

	if(gAppBatInfo.currentLedDuration != ledDurationPara)
	{
		gAppBatInfo.currentLedDuration = ledDurationPara;
//		DRIVER_Cmd_LED_UpdateDuration(ledDurationPara);
	}
}

U8 APP_ChgBat_GetLevelInQuarter(void)
{
	U16 batPercentage;

	if(APP_ChgBat_GetBatStatus() == APP_BAT_LOW)
	{
		return IND_BATTERY_LOW;
	}

	batPercentage = app_Battery_GetBatInThousandPercent();
	DBG_LOG_APP_BAT( "[APP_BAT] Get Level Percent: %d", 1, batPercentage);

	if(batPercentage < pChgBatCtl->batteryPara.checkPoint.checkPoint1)
	{
		return IND_BATTERY_INTERVAL_0;
	}

	if(batPercentage < pChgBatCtl->batteryPara.checkPoint.checkPoint2)
	{
		return IND_BATTERY_INTERVAL_1;
	}

	if(batPercentage < pChgBatCtl->batteryPara.checkPoint.checkPoint3)
	{
		return IND_BATTERY_INTERVAL_2;
	}

	return IND_BATTERY_INTERVAL_3;
}

U8 APP_ChgBat_GetPwrOnLowBatAction(void)
{
	return APP_ChgBat_IsChargerIn() ? pChgBatCtl->pwrOnLowPercentAct.pwrOnLowPercentChgIn : pChgBatCtl->pwrOnLowPercentAct.pwrOnLowPercent;
}

U16 APP_Battery_GetBatInThousandPercent(void)
{
	return app_Battery_GetBatInThousandPercent();
}

void APP_ChgBat_ReleaseSwitchOffInLowBatTimer(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT, 0);
}

void APP_ChgBat_ReleaseTimeOutDectionTimer(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, 0);
}

void APP_ChgBat_CheckBatStatus(void)
{
	if(APP_ChgBat_IsChargerIn())
	{
		//Charger in now.
		//issue1608
		if(APP_ChgBat_IsChargerComplete()) //charging complete now.
		{
			if(APP_ChgBat_GetBatStatus() != APP_BAT_CHARGING_FULL)
			{
				if(APP_ChgCase_GetCaseType() == CASE_TYPE_NONE)
				{
					if(!MSG_MessageSearchAllEx(&gAppChgBatDelayHandle, DRV_CHARGER_MSG_CHARGERCPL, 0))
					{
						DBG_LOG_APP_BAT( "[APP_BAT] not dummy delay, cpl handler !!!", 0);
						FW_SetTimer(&gAppChgBatDelayHandle, DRV_CHARGER_MSG_CHARGERCPL, NULL, 0, 0);
					}
					DBG_LOG_APP_BAT( "[APP_BAT] dummy delay, cpl", 0);
				}
				else
				{
					APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGING_FULL);
					APP_ChgBat_PushBatChgMediaEvt();
				}
			}
		}
		else //charging is not complete yet.
		{
			// the moment when charge is plug in
			if(APP_ChgBat_GetBatStatus() != APP_BAT_CHARGER_IN && APP_ChgBat_GetBatStatus() != APP_BAT_CHARGER_RECHARGE)
			{
				if(APP_ChgBat_GetBatStatus() == APP_BAT_CHARGING_FULL)
				{
					APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_RECHARGE);
				}
				else
				{
					APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_IN);
				}
				APP_ChgBat_PushBatChgMediaEvt();
				APP_ChgBat_SetTimeOutDectionTimer();
				APP_ChgBat_ReleaseSwitchOffInLowBatTimer();
			}
		}

		if(!APP_PowerOff_IsPowerOn())
		{
			if(!APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF))
			{
				APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
			}
		}
	}
	else
	{
		APP_ChgBat_ChargerOutHandler();
	}
}

BOOL APP_Battery_IsBelowInitBat(void)
{
	return Battery_IsBelowInitBat();
}

BOOL APP_Battery_IsBelowLowLevel(void)
{
	return Battery_IsBelowLowBat();
	//return app_ChgBat_IsBelowLowPercent();
}

U8 APP_ChgBat_GetBatStatus(void)
{
	return gAppBatInfo.batStatus;
}

void APP_ChgBat_SetBatStatus(U8 batStatus)
{
	gAppBatInfo.batStatus = batStatus;
}


BOOL APP_Battery_IsBelowShutDownLevel(void)
{
	return Battery_IsBelowShutDownBat();
	//return app_ChgBat_IsBelowShutdownPercent();
}

void APP_ChgBat_ReportBatteryStatus(void)
{
	switch(APP_ChgBat_GetLevelInQuarter())
	{
		case IND_BATTERY_LOW:
		case IND_BATTERY_INTERVAL_0:
			APP_Media_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_0);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_1:
			APP_Media_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_1);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_2:
			APP_Media_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_2);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		case IND_BATTERY_INTERVAL_3:
			APP_Media_PushMediaEvent(MEDIA_EVT_BATTERY_INTERVAL_3);
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_OK);
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
}

void APP_ChgBat_ResetTimeOutDectionTimer(void)
{
	if(!FW_CheckTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, 0))
	{
		APP_ChgBat_SetTimeOutDectionTimer();
	}
}

void APP_ChgBat_SetDoingRHO(void)
{
	gAppBatInfo.isDoingRHO = TRUE;
}

void APP_ChgBat_SetRHOFinished(void)
{
	gAppBatInfo.isDoingRHO = FALSE;
}

BOOL APP_ChgBat_IsRHOForChgCase(void)
{
	return gAppBatInfo.isDoingRHO;
}


U8 APP_ChgBat_GetPowerOffState(void)
{
	return gAppBatInfo.statePowerOff;
}

void APP_ChgBat_SetPowerOffState(U8 statePowerOff)
{
	gAppBatInfo.statePowerOff = statePowerOff;
}

void APP_Battery_ReadInformationConfig(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_BAT_CHG_INFORMATION);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_BAT_CHG_INFORMATION);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pChgBatCtl = (APP_CHGBAT_INFO_STRU *)pAddr;
	}
}

void APP_Battery_Init(void)
{
    /* In 1562 Check charger in in PowerOn */
}

<<<<<<< HEAD
=======
U8 APP_Battery_GetSmartCaseBatLevel(void)
{
	return gAppBatInfo.smartCaseBatLevel;
}

void APP_Battery_SetSmartCaseBatLevel(U8 batLevel)
{
	
	if(APP_ChgCase_GetCaseType() == CASE_TYPE_ONE_WIRE)
	{
		gAppBatInfo.smartCaseBatLevel = (batLevel * 100) / 31;
	}
	else
	{
		gAppBatInfo.smartCaseBatLevel = batLevel;
	}
}

>>>>>>> db20e11 (second commit)
