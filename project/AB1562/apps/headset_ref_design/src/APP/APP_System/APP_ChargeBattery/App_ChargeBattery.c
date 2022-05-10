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
#include "App_ChargeBattery.h"
#include "App_Media.h"
#include "App_PowerOff.h"
#include "App_ResourceManager.h"
#include "App_Nvram_Type.h"
#include "drv_battery.h"
#include "ChargerBattery_Nvkey.h"
#include "App_State.h"
#include "App_PowerOn.h"
#include "drv_charger.h"
#include "App_RaceCmd.h"
#include "drv_charger_case.h"
#include "PM_Interface.h"
#include "Battery.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "ChgCase.h"
#include "Charger.h"
#include "App_System.h"

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define CHARGE_COMPLETE_MEDIA_DELAY_TIME 1000 //ms

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_CHGBAT_TIMER_LOW_BAT_LED = 1,
	APP_CHGBAT_TIMER_LOW_BAT_RINGTONE,
	APP_CHGBAT_BATTERY_MONITOR_PERIOD,
	APP_CHGBAT_TIMER_DELAY_CHARGER_MEDIA_EVT,
	APP_CHGBAT_TIMER_CHECK_CHARGING,
	APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT,
	APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT,
};

enum
{
	CHG_CASE_NOT_IN_DSLEEP,
	CHG_CASE_ENTERING_DSLEEP,
	CHG_CASE_IN_DSLEEP,
};

enum
{
	CHG_CASE_NOT_POWERING_OFF,
	CHG_CASE_POWERING_OFF,
	CHG_CASE_NEED_TO_POWER_ON,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 batStatus;
	U8 currentLedDuration;
	U8 statePowerOff;
}APP_BAT_INFO_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_ChgBat_SetSwitchOffInLowBatTimer(void);//MMI_BAT_SetSwitchOffInLowBatTimer
static void app_ChgBat_PushBatChgMediaEvt(void);//MMI_CHGBAT_PushBatChgMediaEvt
static void app_ChgBat_SetDelayChgCplMediaEvt(void);
static void app_ChgBat_ReleaseDelayChgCplMediaEvt(void);
static void app_Battery_ReadInformationConfig(void);
static void app_ChgBat_SetTimeOutDectionTimer(void);//MMI_CHG_SetTimeOutDectionTimer
static void app_ChgBat_ReleaseSwitchOffInLowBatTimer(void);//MMI_BAT_ReleaseSwitchOffInLowBatTimer
static void app_ChgBat_ReleaseTimeOutDectionTimer(void);//MMI_CHG_ReleaseTimeOutDectionTimer
static U32 app_ChgBat_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static U8 app_ChgBat_GetTimerValue(U8 timer);
static U8 app_Battery_GetBatInHundredPercent(void);
static U16 app_Battery_GetBatInThousandPercent(void);
//static BOOL app_ChgBat_IsBelowInitPercent(void);
//static BOOL app_ChgBat_IsBelowShutdownPercent(void);
//static BOOL app_ChgBat_IsBelowLowPercent(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static APP_CHGBAT_INFO_STRU *pChgBatCtl;
static APP_BAT_INFO_STRU gAppBatInfo;
static HandlerData gAppChgBatTimerHandle = { app_ChgBat_TimerHandler };

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

static void app_ChgBat_PushBatChgMediaEvt(void)
{
	// Here the timer is use to prevent the condition that user plug in and out the charge rapidly.
	// we set timer to prevent the media events are sent a lot continuously.
	FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_DELAY_CHARGER_MEDIA_EVT, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_DELAY_CHG_MEDIA_EVT)*10);
}

static void app_ChgBat_SetDelayChgCplMediaEvt(void)
{
	// If charging in when the battery has been already full, LED have to turn on a while then turn off.
	FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT, NULL, 0, CHARGE_COMPLETE_MEDIA_DELAY_TIME);
}

static void app_ChgBat_ReleaseDelayChgCplMediaEvt(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT, 0);
}

static void app_Battery_ReadInformationConfig(void)
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

static void app_ChgBat_SetTimeOutDectionTimer(void)
{
	if(app_ChgBat_GetTimerValue(TIMER_ID_CHECK_CHARGING_TIME))
	{
		FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_CHECK_CHARGING_TIME)*1000*60*30); //unit 0.5 hours
	}
}

static void app_ChgBat_ReleaseSwitchOffInLowBatTimer(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT, 0);
}

static void app_ChgBat_ReleaseTimeOutDectionTimer(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, 0);
}

static void app_ChgBat_ChargerOutHandler(void)
{
	//Charger out now
	if(gAppBatInfo.batStatus >= APP_BAT_CHARGER_IN)
	{
		app_ChgBat_ReleaseDelayChgCplMediaEvt();
		//DRIVER_TEMP_SetChargeNormal();
		app_ChgBat_ReleaseTimeOutDectionTimer();
		APP_ChgBat_UpdateBatStatus(APP_BAT_WELL);
		app_ChgBat_PushBatChgMediaEvt();

		if(APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
		{
			U8 PwrLv = 3;
			//DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] DRV_CHARGER_MSG_CHARGEROUT : CHG_GPIO_INPUT_STATUS(0x%x) : [%d]", 1 , ChgCase_GPIOXY_GetInputStatus());
			if(ChgCase_GPIOXY_GetInputStatus() == 1)//GPIO_Y = 1
			{
				//not in charger case, set X==1
				ChgCase_GPIOXY_OutputHigh();
				DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] Charger out, CHG_GPIO_OUTPUT_HIGH", 0);
			}
			else//GPIO_Y = 0
			{
				//in charger case, enter deep sleep.
				ChgCase_GPIOXY_EnableMMITimer();
				DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] - Enter Deep Sleep !!! - ",0);
				APP_SendRaceCmd(RACE_SET_ADAPTIVE_POWER_LEVEL, &PwrLv, sizeof(U8));
				return;
			}
		}

		if(APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF) || !APP_PowerOff_IsPowerOn())
		{
			gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
			if(APP_ChgBat_IsFeatureSupported(APP_CHARGER_OUT_THEN_POWER_ON_FEAT) && !APP_Battery_IsBelowInitBat())
			{
				APP_PowerOn_Loader();
			}
			else if(PM_Conn_GetAgLinkState() == LINK_DISCONNECTED)
			{
				APP_PowerOff_SystemOff();
			}
		}
		else if((gAppBatInfo.statePowerOff == CHG_CASE_POWERING_OFF) && APP_ChgBat_IsFeatureSupported(APP_CHARGER_OUT_THEN_POWER_ON_FEAT) && !APP_Battery_IsBelowInitBat())
		{
			gAppBatInfo.statePowerOff = CHG_CASE_NEED_TO_POWER_ON;
		}
		else
		{
			gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
			APP_ChgBat_CheckCurrentBatLevel();
		}
	}
}

static U32 app_ChgBat_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	switch(id)
	{
		case APP_CHGBAT_TIMER_LOW_BAT_LED:
			if (APP_ChgBat_GetBatStatus() == APP_BAT_LOW)
			{
				if(pChgBatCtl->batteryPara.lowPercentLEDTimer == pChgBatCtl->batteryPara.lowPercentRingTimer)
				{
					APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);
				}
				APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW_LED);
				FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_LED, NULL, 0, (U32)(pChgBatCtl->batteryPara.lowPercentLEDTimer*ONE_SEC));
			}
			break;

		case APP_CHGBAT_TIMER_LOW_BAT_RINGTONE:
			if (APP_ChgBat_GetBatStatus() == APP_BAT_LOW)
			{
				if(pChgBatCtl->batteryPara.lowPercentLEDTimer != pChgBatCtl->batteryPara.lowPercentRingTimer)
				{
					APP_Media_PushMediaEvent(MEDIA_EVT_BAT_LOW_RING);
				}
				FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_RINGTONE, NULL, 0, (U32)(pChgBatCtl->batteryPara.lowPercentRingTimer*ONE_SEC));
			}
			break;

		case APP_CHGBAT_BATTERY_MONITOR_PERIOD:
			APP_ChgBat_SetBatterMonitorTimer();
			APP_ChgBat_CheckCurrentBatLevel();
			break;

		case APP_CHGBAT_TIMER_DELAY_CHARGER_MEDIA_EVT:
			switch(gAppBatInfo.batStatus)
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
			break;

		case APP_CHGBAT_TIMER_CHECK_CHARGING:
			APP_Media_PushMediaEvent(MEDIA_EVT_BAT_CHGTO);

			//turn off power-Let Hw gets into abnormal state
			//MCU will be shut down.
			APP_ChgCase_SetChargerOffEn();
			if(APP_ChgBat_IsFeatureSupported(APP_POWER_OFF_WHEN_CHARGING_TO_FEAT))
			{
				APP_PowerOff_SystemOff();
			}
			break;

		case APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT:
			APP_PowerOff_Loader(POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER);
			break;

		case DRV_CHARGER_MSG_CHARGERIN:
			if(gAppBatInfo.batStatus == APP_BAT_CHARGER_IN)
				return 0;

			app_ChgBat_ReleaseDelayChgCplMediaEvt();
			APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_IN);
			app_ChgBat_PushBatChgMediaEvt();
			app_ChgBat_SetTimeOutDectionTimer();
			app_ChgBat_ReleaseSwitchOffInLowBatTimer();


			if(!APP_PowerOff_IsPowerOn())
			{
				if(!APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF))
				{
					APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
				}
			}
			else if(APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
			{
				gAppBatInfo.statePowerOff = CHG_CASE_POWERING_OFF;
				APP_PowerOff_Loader(POWEROFF_REASON_CHARGING_CASE);
			}
			break;

		case DRV_CHARGER_MSG_CHARGEROUT:
			app_ChgBat_ChargerOutHandler();
			break;

		case APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT:
			if(gAppBatInfo.batStatus >= APP_BAT_CHARGER_IN)
			{
				APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGING_FULL);
				app_ChgBat_PushBatChgMediaEvt();
			}
			break;

		case DRV_CHARGER_MSG_CHARGERCPL:

			if(gAppBatInfo.batStatus == APP_BAT_CHARGING_FULL)
				return 0;

			app_ChgBat_SetDelayChgCplMediaEvt();
			if(APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
			{
				//DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] DRV_CHARGER_MSG_CHARGERCPL : input(0x%x), output(0x%x), isDoingRHO(%d) -> CHG_GPIO_OUTPUT_LOW", 1, 0);
				ChgCase_GPIOXY_OutputLow();
			}
			break;

		case DRV_CHARGER_MSG_RECHARGER:
			if((gAppBatInfo.batStatus >= APP_BAT_CHARGER_IN) && (gAppBatInfo.batStatus != APP_BAT_CHARGER_RECHARGE))
			{
				app_ChgBat_ReleaseDelayChgCplMediaEvt();
				APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_RECHARGE);
				app_ChgBat_PushBatChgMediaEvt();
			}
			break;

		case DRV_CHARGER_MSG_GPIO_Y:
			if(ext_id)//GOIO_Y == 1
			{
				//not in charger case, set X==1
				DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] DRV_CHARGER_MSG_GPIO_Y = 1, set x = 1, reset", 0);
				ChgCase_GPIOXY_OutputHigh();
				APP_SendRaceCmd(RACE_HARDWARE_RESET, NULL, 0);//HW reset
			}
			break;
	}
	return 0;
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

/*
static BOOL app_ChgBat_IsBelowInitPercent(void)
{
	return (app_ChgBat_GetBatInThousandPercent() < pChgBatCtl->percentThld.initPercent);
}

static BOOL app_ChgBat_IsBelowShutdownPercent(void)
{
	return (app_ChgBat_GetBatInThousandPercent() < pChgBatCtl->percentThld.shutdownPercent);
}

static BOOL app_ChgBat_IsBelowLowPercent(void)
{
	return (app_ChgBat_GetBatInThousandPercent() < pChgBatCtl->percentThld.lowPercent);
}
*/
/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_ChgBat_Init(void)
{
	app_Battery_ReadInformationConfig();

	ChgCase_Nvkey_ReadCaseInfo();

	switch(APP_ChgCase_GetCaseType())
	{
		case CASE_TYPE_NONE:
			/* fallthrough */
		case CASE_TYPE_GPIOXY:
			Charger_CaseInit(&gAppChgBatTimerHandle, NULL, NULL);
			break;
		default:
			break;
	}
}

void APP_ChgBat_UpdateBatStatus(U8 batStatus)
{
	if(gAppBatInfo.batStatus != batStatus)
	{
		DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] status: %s", 1, APP_BatStatusLogString[batStatus]);
		#ifdef AIRAPP_Profile
//		AirApp_ChgBatStatusUpdateEvent(batStatus);
		#endif
	}
	gAppBatInfo.batStatus = batStatus;
}

void APP_ChgBat_CheckCurrentBatLevel(void)
{
	U8 ledDurationPara;

	if(gAppBatInfo.batStatus >= APP_BAT_CHARGER_IN)
	{	//Already Charger-In do nothing now.
		if(!DRV_BAT_IsBelowLowBat())
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
		else if(DRV_BAT_IsBelowLowBat())
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
			app_ChgBat_PushBatChgMediaEvt();
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
	DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] Get Level Percent: %d", 1, batPercentage);

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

U8 APP_ChgBat_IsChargerIn(void)
{
	return Charger_IsChargerIn();
}

void APP_ChgBat_CheckBatStatus(void)
{
	if(APP_ChgBat_IsChargerIn())
	{
		//Charger in now.
		//issue1608
		if(APP_ChgBat_IsChargerComplete()) //charging complete now.
		{
			if(gAppBatInfo.batStatus != APP_BAT_CHARGING_FULL)
			{
				APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGING_FULL);
				app_ChgBat_PushBatChgMediaEvt();
			}
		}
		else //charging is not complete yet.
		{
			// the moment when charge is plug in
			if(gAppBatInfo.batStatus != APP_BAT_CHARGER_IN && gAppBatInfo.batStatus != APP_BAT_CHARGER_RECHARGE)
			{
				if(gAppBatInfo.batStatus == APP_BAT_CHARGING_FULL)
				{
					APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_RECHARGE);
				}
				else
				{
					APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_IN);
				}
				app_ChgBat_PushBatChgMediaEvt();
				app_ChgBat_SetTimeOutDectionTimer();
				app_ChgBat_ReleaseSwitchOffInLowBatTimer();
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
		app_ChgBat_ChargerOutHandler();
	}
}

BOOL APP_ChgBat_IsBelowLowLevel(void)
{
	return DRV_BAT_IsBelowLowBat();
	//return app_ChgBat_IsBelowLowPercent();
}

U8 APP_ChgBat_GetBatStatus(void)
{
	return gAppBatInfo.batStatus;
}

BOOL APP_Battery_IsBelowShutDownLevel(void)
{
	return Battery_IsBelowShutDownBat();
	//return app_ChgBat_IsBelowShutdownPercent();
}

BOOL APP_ChgBat_IsEventPowerOff(U16 opCode)
{
	switch(opCode)
	{
		case MEDIA_EVT_KEY_POWER_OFF:
		case MEDIA_EVT_POWER_OFF:
		case MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3:
			return TRUE;
		default:
			return FALSE;
	}
}

void APP_ChgBat_PowerOffHandler(void)
{
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_LED, 0);
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_LOW_BAT_RINGTONE, 0);
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_BATTERY_MONITOR_PERIOD, 0);
	FW_ReleaseTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_AUTO_SWITCH_OFF_IN_LOW_BAT, 0);
}

void APP_ChgBat_SetBatterMonitorTimer(void)
{
	if(app_ChgBat_GetTimerValue(TIMER_ID_BATTERY_MONITOR_PERIOD))
	{
		DBG_LOG_APP_SYSTEM( "[    APP][ChgBat] Monitor Hundred Percent:%d, Thousand Percent:%d", 2, app_Battery_GetBatInHundredPercent(), app_Battery_GetBatInThousandPercent());
		FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_BATTERY_MONITOR_PERIOD, NULL, 0, app_ChgBat_GetTimerValue(TIMER_ID_BATTERY_MONITOR_PERIOD)*ONE_SEC);
	}
}

void APP_ChgBat_ResetTimeOutDectionTimer(void)
{
	if(!FW_CheckTimer(&gAppChgBatTimerHandle, APP_CHGBAT_TIMER_CHECK_CHARGING, 0))
	{
		app_ChgBat_SetTimeOutDectionTimer();
	}
}

BOOL APP_ChgBat_IsTimerSet(U8 timerType)
{
	return MSG_MessageSearchAllEx(&gAppChgBatTimerHandle, timerType, 0);
}

U8 APP_ChgBat_GetPwrOnLowBatAction(void)
{
	return APP_ChgBat_IsChargerIn() ? pChgBatCtl->pwrOnLowPercentAct.pwrOnLowPercentChgIn : pChgBatCtl->pwrOnLowPercentAct.pwrOnLowPercent;
}

U16 APP_Battery_GetBatInThousandPercent(void)
{
	return app_Battery_GetBatInThousandPercent();
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

BOOL APP_ChgBat_IsFeatureSupported(U16 feature)
{
	return pChgBatCtl->supportedFeature & feature;
}

BOOL APP_ChgBat_IsPowerOnNeeded(void)
{
	return (gAppBatInfo.statePowerOff == CHG_CASE_NEED_TO_POWER_ON);
}

void APP_ChgBat_ClearPowerOffState(void)
{
	gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
}

BOOL APP_Battery_IsBelowInitBat(void)
{
	return Battery_IsBelowInitBat();
}

BOOL APP_ChgBat_IsChargerComplete(void)
{
	return Charger_IsCargerComplete();
}

void APP_ChgCase_SetChargerOffEn(void)
{
	Charger_SetChargerOffEn();
}

U8 APP_ChgCase_GetCaseType(void)
{
	return ChgCase_GetCaseType();
}

void APP_Battery_Init(void)
{
    /* In 1562 Check charger in in PowerOn */
}

