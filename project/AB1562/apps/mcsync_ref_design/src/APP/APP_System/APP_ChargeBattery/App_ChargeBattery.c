/* Copyright Statement:
 *
 * (C) 2017	 Airoha Technology Corp. All rights reserved.
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
#include "App_MCSync_RHO.h"
#include "App_MCSyncRho_NvkeyHandler.h"
#include "App_Battery.h"
#include "PM_Interface.h"
#include "Power_NvkeyHandler.h"

#include "ChargerBatteryCaseNvkeyHandler.h"
#include "ChgCase.h"
#include "Charger.h"
#include "BtMCSync.h"
#include "Battery.h"

#define DBG_LOG_APP_CHGBAT(_message, arg_cnt, ...)  LOG_MSGID_I(APP_CHGBAT, _message, arg_cnt, ##__VA_ARGS__)


log_create_module(APP_CHGBAT, PRINT_LEVEL_INFO);


/**************************************************************************************************
* Definition
**************************************************************************************************/

#define CHG_IN_DEBOUNCE_TIME 1000
#define CHG_OUT_DEBOUNCE_TIME 500


/**************************************************************************************************
* Enum
**************************************************************************************************/
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
	CHG_CASE_DEEP_SLEEP,
};

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_ChgBat_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static U32 app_ChgBat_DelayHandler(Handler handler, U16 id, void *msg, U32 ext_id);

/**************************************************************************************************
* Variable
**************************************************************************************************/
HandlerData gAppChgBatTimerHandle = { app_ChgBat_TimerHandler };
HandlerData gAppChgBatDelayHandle = { app_ChgBat_DelayHandler };

extern APP_BAT_INFO_STRU gAppBatInfo;
extern HandlerData gAppSccStateHandle;
extern HandlerData gAppSccRsvHandle;
extern HandlerData gAppChargerHandle;
extern APP_CHGBAT_INFO_STRU *pChgBatCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/

static void app_ChgBat_DrvEvtHandler(U16 id, U32 ext_id)
{
<<<<<<< HEAD
=======
        bool KeepFakeOff;
>>>>>>> db20e11 (second commit)
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
<<<<<<< HEAD
			#ifdef XIAOAI_DEVELOPMENT
=======
			#if defined(XIAOAI_DEVELOPMENT) || defined(AIR_GFP_ENABLE)
>>>>>>> db20e11 (second commit)
			if(BtAwsMce_IsDefaultRolePartner())
				BtMCSync_SendSyncBatteryLevel(Battery_GetLevelInPercent());
			#endif
			break;

		case APP_CHGBAT_TIMER_DELAY_CHARGER_MEDIA_EVT:
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
			if(APP_ChgBat_GetBatStatus() == APP_BAT_CHARGER_IN)
				break;

			APP_ChgBat_ReleaseDelayChgCplMediaEvt();
			APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_IN);
			APP_ChgBat_PushBatChgMediaEvt();
			APP_ChgBat_SetTimeOutDectionTimer();
			APP_ChgBat_ReleaseSwitchOffInLowBatTimer();

			DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] RHO for Charging Case: %d, %d, %d, %d", 4,
					 APP_PowerOff_IsPowerOn(), APP_ChgCase_GetCaseType(),
					 APP_MCSyncNvkey_IsRhoFeatureOn(APP_RHO_TRIGGER_FEATURE_CHARGECASE), APP_MCSync_Rho_IsAbleToStartRho());

			if(!APP_PowerOff_IsPowerOn())
			{
				if(!APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF))
				{
					APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
				}
			}
			else if(APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
			{
				if(APP_MCSyncNvkey_IsRhoFeatureOn(APP_RHO_TRIGGER_FEATURE_CHARGECASE) && APP_MCSync_Rho_IsAbleToStartRho())

				{
					APP_ChgBat_SetDoingRHO();
					APP_MCSync_Rho_Start();
				}
				else
				{
					//gAppBatInfo.statePowerOff = CHG_CASE_POWERING_OFF;
					//APP_PowerOff_Loader(POWEROFF_REASON_CHARGING_CASE);
				}
			}

			if(APP_ChgBat_IsFeatureSupported(APP_POWER_OFF_WHEN_CHARGING_IN_INTR))
			{
				gAppBatInfo.statePowerOff = CHG_CASE_POWERING_OFF;
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] Charger In, then power off", 0);
				APP_PowerOff_Loader(POWEROFF_REASON_KEY);
			}
			break;

		case DRV_CHARGER_MSG_CHARGEROUT:
			APP_ChgBat_ChargerOutHandler();
			break;
		case APP_CHGBAT_TIMER_DELAY_CHARGER_COMPLETE_MEDIA_EVT:
			if(APP_ChgBat_GetBatStatus() == APP_BAT_CHARGING_FULL)
			{
				APP_ChgBat_PushBatChgMediaEvt();
<<<<<<< HEAD
		   		if(APP_ChgCase_GetCaseType() == CASE_TYPE_NONE)
=======
				KeepFakeOff = APP_PowerOff_IsKeepFakeOff();
		   		if(APP_ChgCase_GetCaseType() == CASE_TYPE_NONE && !KeepFakeOff)
>>>>>>> db20e11 (second commit)
		   		{
		   			U8 PwrLv = 3;
					gAppBatInfo.statePowerOff = CHG_CASE_DEEP_SLEEP;
					DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] Enter Deep Sleep",0);
					APP_SendRaceCmd(RACE_SET_ADAPTIVE_POWER_LEVEL, &PwrLv, sizeof(U8));
<<<<<<< HEAD
		   		}				
=======
		   		}
				else
				{
					if(APP_ChgBat_IsChargerIn() || KeepFakeOff)
					{
						APP_ChgBat_ClearPowerOffState();
						DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] Keeping Fake Off for Tile",0);
						APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
					}
		   		}
>>>>>>> db20e11 (second commit)
			}
			break;
		case DRV_CHARGER_MSG_CHARGERCPL:
			if(APP_ChgBat_GetBatStatus() == APP_BAT_CHARGING_FULL)
				break;
			APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGING_FULL);
			APP_ChgBat_SetDelayChgCplMediaEvt();
			if((APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY)
			   && (!APP_ChgBat_IsRHOForChgCase())
			   )
			{
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] DRV_CHARGER_MSG_CHARGERCPL : input(0x%x), output(0x%x), isDoingRHO(%d) -> CHG_GPIO_OUTPUT_LOW", 1, APP_ChgBat_IsRHOForChgCase());
				ChgCase_GPIOXY_OutputLow();//X=>0
			}
			break;
		case DRV_CHARGER_MSG_RECHARGER:
			if(APP_ChgBat_GetBatStatus() != APP_BAT_CHARGER_RECHARGE)
			{
				APP_ChgBat_ReleaseDelayChgCplMediaEvt();
				APP_ChgBat_UpdateBatStatus(APP_BAT_CHARGER_RECHARGE);
				APP_ChgBat_PushBatChgMediaEvt();
			}
			break;

		case DRV_CHARGER_MSG_GPIO_Y:
			{
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] DRV_CHARGER_MSG_GPIO_Y ext_id:(%d) APP_ChgBat_GetPowerOffState():(%d)", 2, ext_id, APP_ChgBat_GetPowerOffState());
				if(ext_id)//GOIO_Y == 1
				{
					ChgCase_GPIOXY_OutputHigh();
					if(gAppBatInfo.statePowerOff == CHG_CASE_DEEP_SLEEP)
					{
						APP_SendRaceCmd(RACE_HARDWARE_RESET, NULL, 0);//HW reset
					}
					else
					{
						gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
						if(!APP_Battery_IsBelowInitBat())
						{
							if(!APP_PowerOff_IsPowerOn())
								APP_PowerOn_Loader();
						}
						else
						{
							APP_PowerOff_SystemOff();
						}
					}
				}
			}
			break;

		case APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID:
			DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID %d", 1, gAppBatInfo.statusDuringDelay);
			if(gAppBatInfo.statusDuringDelay)
			{
				if(gAppBatInfo.statusDuringDelay == DRV_CHARGER_MSG_CHARGERCPL)
				{
					app_ChgBat_DrvEvtHandler(DRV_CHARGER_MSG_CHARGERIN, ext_id);
					FW_SetTimer(&gAppChgBatDelayHandle, gAppBatInfo.statusDuringDelay, NULL, ext_id, CHG_IN_DEBOUNCE_TIME);
				}
				else
				{
					app_ChgBat_DrvEvtHandler(gAppBatInfo.statusDuringDelay, ext_id);
				}
				gAppBatInfo.statusDuringDelay = 0;
			}
			break;

		case APP_CHGBAT_CHARGER_OUT_DUMMY_DELAY_ID:
			{
				app_ChgBat_DrvEvtHandler(DRV_CHARGER_MSG_CHARGEROUT, ext_id);
			}
			break;

		case APP_CHGBAT_CHGOUT_SW_WORKAROUND_ID:
			break;

		default:
			break;
	}
}

static U32 app_ChgBat_DelayHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	app_ChgBat_DrvEvtHandler(id, ext_id);

	return 0;
}

static U32 app_ChgBat_TimerHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	U16 DuringChgInDummyDelay =  MSG_MessageSearchAllEx(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID, 0);

	DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  id:%x DuringDummyDelay:%d", 2, id, DuringChgInDummyDelay);
	if(DuringChgInDummyDelay && (id >= DRV_CHARGER_MSG_CHARGERIN) && (APP_ChgCase_GetCaseType() == CASE_TYPE_NONE))//normal charger case without External circuit(need debounce)
	{
		DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  DuringDummyDelay && (id >= DRV_CHARGER_MSG_CHARGERIN) && case_none (%d)", 1, id);
		gAppBatInfo.statusDuringDelay = id;
		FW_SetTimer(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID, msg, ext_id, CHG_IN_DEBOUNCE_TIME);
	}
	else if(DuringChgInDummyDelay && (APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY))//charger case with goio_xy
	{
		DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  DuringDummyDelay && case gpio_xy:%d", 1, id);
		FW_SetTimer(&gAppChgBatDelayHandle, id, msg, ext_id, CHG_IN_DEBOUNCE_TIME);
	}
	else
	{
		if(APP_ChgCase_GetCaseType() == CASE_TYPE_NONE)
		{
			U16 DuringChgOutDummyDelay =  MSG_MessageSearchAllEx(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_OUT_DUMMY_DELAY_ID, 0);
			if(DuringChgOutDummyDelay && (id == DRV_CHARGER_MSG_CHARGERIN))
			{
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  DuringChgOutDummyDelay Release Timer", 0);
				FW_ReleaseTimer(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_OUT_DUMMY_DELAY_ID, 0);
				return 0;
			}
			else
			{
				if(id == DRV_CHARGER_MSG_CHARGERIN)
				{
					FW_SetTimer(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID, msg, ext_id, CHG_IN_DEBOUNCE_TIME);
				}
				else if(id == DRV_CHARGER_MSG_CHARGEROUT)
				{
					FW_SetTimer(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_OUT_DUMMY_DELAY_ID, msg, ext_id, CHG_OUT_DEBOUNCE_TIME);
					return 0;
				}
			}
			DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  app_ChgBat_DrvEvtHandler([%x])", 1, id);
			app_ChgBat_DrvEvtHandler(id, ext_id);
		}
		else
		{
			if(id == DRV_CHARGER_MSG_CHARGERIN)
			{
				FW_SetTimer(&gAppChgBatDelayHandle, APP_CHGBAT_CHARGER_IN_DUMMY_DELAY_ID, msg, ext_id, CHG_IN_DEBOUNCE_TIME);
			}
			DBG_LOG_APP_CHGBAT( "[APP_CHGBAT]  app_ChgBat_DrvEvtHandler([%x])", 1, id);
			app_ChgBat_DrvEvtHandler(id, ext_id);
		}
	}

	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_ChgBat_Init(void)
{
	APP_Battery_ReadInformationConfig();

	ChgCase_Nvkey_ReadCaseInfo();

	switch(APP_ChgCase_GetCaseType())
	{
		case CASE_TYPE_SMART:
			/* fallthrough */
		case CASE_TYPE_SMART_V2:
			/* fallthrough */
		case CASE_TYPE_ONE_WIRE:	
			Charger_CaseInit(&gAppSccStateHandle, &gAppChargerHandle, &gAppSccRsvHandle);
			break;
		case CASE_TYPE_NONE:
			/* fallthrough */
		case CASE_TYPE_GPIOXY:	
			Charger_CaseInit(&gAppChgBatTimerHandle, NULL, NULL);
			break;
		default:
			break;
	}
}


U8 APP_ChgBat_IsChargerIn(void)
{
	return Charger_IsChargerIn();
}


BOOL APP_ChgBat_IsChargerComplete(void)
{
	return Charger_IsCargerComplete();
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

BOOL APP_ChgBat_IsTimerSet(U8 timerType)
{
	return MSG_MessageSearchAllEx(&gAppChgBatTimerHandle, timerType, 0);
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

BOOL APP_ChgBat_IsDSleepNeededAfterRHO(void)
{
	return (APP_ChgBat_GetBatStatus() == APP_BAT_CHARGING_FULL);
}

void APP_ChgBat_EnterDSleepAfterRHO(void)
{
	ChgCase_GPIOXY_OutputLow();
}

void APP_ChgBat_ChargerOutHandler(void)
{
	U8 isChgOutOn = (APP_ChgBat_IsFeatureSupported(APP_CHARGER_OUT_THEN_POWER_ON_FEAT))? TRUE: FALSE;
	//Charger out now
	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN)
	{
		U8 oldBatStatus = APP_ChgBat_GetBatStatus();
		APP_ChgBat_ReleaseDelayChgCplMediaEvt();
		//DRIVER_TEMP_SetChargeNormal();
		APP_ChgBat_ReleaseTimeOutDectionTimer();
		APP_ChgBat_UpdateBatStatus(APP_BAT_WELL);
		APP_ChgBat_PushBatChgMediaEvt();

		#if BOOMTECH_WORKAROUND
		if(APP_ChgBat_IsFeatureSupported(APP_CHARGER_OUT_THEN_POWER_ON_FEAT) && APP_ChgBat_IsFeatureSupported(APP_POWER_OFF_WHEN_CHARGING_IN_INTR))
			FW_SetTimer(&gAppChgBatTimerHandle, APP_CHGBAT_CHGOUT_SW_WORKAROUND_ID, NULL, 0, 2500);
		#endif

		if((APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY))
		{
			U8 PwrLv = 3;
			isChgOutOn = TRUE;
			//DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] DRV_CHARGER_MSG_CHARGEROUT : CHG_GPIO_INPUT_STATUS(0x%x) : [%d]", 2, APP_ChgBat_GetCaseInputGPIO(), CHG_GPIO_INPUT_STATUS(APP_ChgBat_GetCaseInputGPIO()));
			if(ChgCase_GPIOXY_GetInputStatus() == 1)//GPIO_Y = 1
			{
				//not in charger case, set X==1
				ChgCase_GPIOXY_OutputHigh();
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] Charger out, CHG_GPIO_OUTPUT_HIGH", 0);

			}
			else if(oldBatStatus == APP_BAT_CHARGING_FULL)//GPIO_Y = 0
			{
				//in charger case, enter deep sleep.
				ChgCase_GPIOXY_EnableMMITimer();
				gAppBatInfo.statePowerOff = CHG_CASE_DEEP_SLEEP;
				DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] - Enter Deep Sleep !!! - ",0);
				APP_SendRaceCmd(RACE_SET_ADAPTIVE_POWER_LEVEL, &PwrLv, sizeof(U8));
				return;
			}
		}

		DBG_LOG_APP_CHGBAT( "[APP_CHGBAT] CheckNestState[FAKEOFF]:(0x%d), APP_PowerOff_IsPowerOn:(0x%d), isChgOutOn:(0x%d), DRV_BAT_IsBelowInitBat:(0x%d)", 4, APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF), APP_PowerOff_IsPowerOn(), isChgOutOn, APP_Battery_IsBelowInitBat());

		if(APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF) || !APP_PowerOff_IsPowerOn())
		{
			gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
			if(isChgOutOn && !APP_Battery_IsBelowInitBat())
			{
				APP_PowerOn_Loader();
			}
			else if(PM_Conn_GetAgLinkState() == LINK_DISCONNECTED)
			{
				APP_PowerOff_SystemOff();
			}
		}
		else if((gAppBatInfo.statePowerOff == CHG_CASE_POWERING_OFF) && isChgOutOn && !APP_Battery_IsBelowInitBat())
		{
			if(APP_PowerOff_IsPowerOffAfterRHO())
			{
				APP_PowerOff_CancelTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
				APP_PowerOff_ClearPowerOffAfterRHO();
			}
			else
			{
				gAppBatInfo.statePowerOff = CHG_CASE_NEED_TO_POWER_ON;
			}
		}
		else
		{
			gAppBatInfo.statePowerOff = CHG_CASE_NOT_POWERING_OFF;
			APP_ChgBat_CheckCurrentBatLevel();
		}
	}
}


void APP_ChgCase_SetChargerOffEn(void)
{
	Charger_SetChargerOffEn();
}

U8 APP_ChgCase_GetCaseType(void)
{
	return ChgCase_GetCaseType();
}

BOOL APP_ChgCase_IsCaseSmartType(void)
{
	return ChgCase_Smart_IsCaseSmartType();
}

