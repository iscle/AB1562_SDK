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
#include "App_Pairing.h"
#include "Pairing_NvkeyHandler.h"
#include "App_EventOpcode.h"
#include "App_ResourceManager.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_Hfp.h"
#include "App_Media.h"
#include "App_LinkKey.h"
#include "App_Nvram_Type.h"
#include "App_VolumeManage.h"
#include "App_LedControl.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "App_ChargeBattery.h"
#include "APP_HfpScoHandler.h"
#include "APP_AudioDriver.h"
#include "app_hfpnvkey_nvstruc.h"
#include "App_PowerOn.h"
#include "App_Connection_NVKeyHandler.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "Volume_NvkeyHandler.h"
#include "System_Nvkey.h"
#include "APP_AirApp.h"
#include "PM_Interface.h"
#include "bt_controller.h"
#include "AudioDSP_Registration.h"
#include "AudioDSP_Vp.h"
#include "AudioDSP_StreamManager.h"
#include "App_System.h"
#include "afe_ctl.h"
#include "drv_rt.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "App_VolumeSetting.h"


#define DBG_LOG_APP_STATE(_message, arg_cnt, ...)  LOG_MSGID_I(APP_STATE, _message, arg_cnt, ##__VA_ARGS__)

log_create_module(APP_STATE, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Define
**************************************************************************************************/
#define MAX_STATE_NESTING_LEVEL (FW_MAX_LINK_IDX*7)
#define IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL	(!APP_PairNvkey_GetTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT))
#define INTERRUPT_REASON_MMI_TIMER	1

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static BOOL app_State_IsDutMode(void);
static void app_State_UnloadState(BD_ADDR_T *pBdAddr, U8 state);
static void app_State_LoadState(BD_ADDR_T *pBdAddr, U8 state);

static void app_State_ConnectDiscoverableEntry(BD_ADDR_T *pBdAddr);
static void app_State_ConnectDiscoverableHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_State_ConnectDiscoverableExit(BD_ADDR_T *pBdAddr);
static void app_State_ConnectableEntry(BD_ADDR_T *pBdAddr);
static void app_State_ConnectableHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);

static void app_State_OffEntry(BD_ADDR_T *pBdAddr);
static void app_State_OffHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_State_FakeonEntry(BD_ADDR_T *pBdAddr);
static void app_State_FakeonHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_State_FakeoffEntry(BD_ADDR_T *pBdAddr);
static void app_State_FakeoffHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_State_DetachEntry(BD_ADDR_T *pBdAddr);
static void app_State_DetachHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_State_DetachExit(BD_ADDR_T *pBdAddr);

static void app_State_AddTopStateByLink(BD_ADDR_T *pBdAddr, U8 state);
static void app_State_AddTopStateAllLink(U8 state);
static bool app_State_InsertStateByLink(BD_ADDR_T *pBdAddr, U8 state, U8 beforeState);
static bool app_State_InsertStateAllLink(U8 state, U8 beforeState);
static bool app_State_ReplaceStateByLink(BD_ADDR_T *pBdAddr, U8 newState, U8 oldState);
static bool app_State_ReplaceStateAllLink(U8 newState, U8 oldState);
static void app_State_CleanNestStateByLink(BD_ADDR_T *pBdAddr);
static void app_State_CleanNestStateAllLink(void);
static bool app_State_RmvStateByLink(BD_ADDR_T *pBdAddr, U8 state);
static bool app_State_RmvStateAllLink(U8 state);

static APP_STATE_STRU *app_State_AllocateAppState(void);
static void app_State_FreeAppState(APP_STATE_STRU *pAppState);
static U8 app_State_ReadStateType(U8 state);
static U8 app_State_GetTopStateByLink(BD_ADDR_T *pBdAddr);
static U8 app_State_GetTopStateInDefaultLink(void);
static void app_State_CallStateSwitchVP(BD_ADDR_T *pBdAddr);

static void app_State_VoicePromptLangSelectEntry(BD_ADDR_T *pBdAddr);
static void app_State_VoicePromptLangSelectExit(BD_ADDR_T *pBdAddr);

static U32 app_State_VpSelHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static const HandlerData gAppStateVpSelHandle = { app_State_VpSelHandler };


static U32 app_State_TimerHandler(Handler t, U16 id, void *payload, U32 id_ext);

static void app_State_AutoSwitchOffBitHandler(U8 state, bool isBitAdded, BD_ADDR_T *pBdAddr);
static void app_State_PairingTimeoutHandler(void);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateOffHandler = //MMI_StateOffHandler
{
	app_State_OffEntry, app_State_OffHandler, NULL, //APP_OFF
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateConnectDiscoverableHandler = //MMI_StateConnectDiscoverableHandler
{
	app_State_ConnectDiscoverableEntry, app_State_ConnectDiscoverableHandler, app_State_ConnectDiscoverableExit, //APP_CONDISCABLE
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateConnectableHandler = //MMI_StateConnectableHandler
{
	app_State_ConnectableEntry, app_State_ConnectableHandler, NULL, //APP_CONNECTABLE
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateFakeonHandler = //MMI_StateFakeonHandler
{
	app_State_FakeonEntry, app_State_FakeonHandler, NULL, //APP_FAKEON
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateFakeoffHandler = //MMI_StateFakeoffHandler
{
	app_State_FakeoffEntry, app_State_FakeoffHandler, NULL, //APP_FAKEOFF
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateDetachHandler = //MMI_StateDetachHandler
{
	app_State_DetachEntry, app_State_DetachHandler, app_State_DetachExit, //APP_DETACHING_LINK
};

static APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateVoicePromptLangSelectHandler = //MMI_StateVoicePromptLangSelectHandler
{
	app_State_VoicePromptLangSelectEntry, NULL, app_State_VoicePromptLangSelectExit, //MMI_VOICE_PROMPT_LANG_SELECT
};

extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPConnectedHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPImgHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPOggHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaImgHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaOggHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateHFPCaMultyHandler;
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateTestModeHandler;
#ifdef LINEIN_ENABLE
extern APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateLineInHandler;
#endif

const APP_STATE_HANDLER_SET_TYPE_STRU const *gAppStateHandlerSet[APP_TOTAL_STATE_NO] =//MMI_StateHandlerSet
{
	&gAppStateOffHandler, //APP_OFF
	&gAppStateConnectDiscoverableHandler, //APP_CONDISCABLE
	&gAppStateConnectableHandler, //APP_CONNECTABLE
	&gAppStateHFPConnectedHandler, //APP_CONNECTED
	&gAppStateHFPImgHandler, //APP_HFP_INCOMMING
	&gAppStateHFPOggHandler, //APP_HFP_OUTGOING
	&gAppStateHFPCaHandler, //APP_HFP_CALLACTIVE
	&gAppStateHFPCaHandler, //APP_HFP_CALLACTIVE_WITHOUT_SCO
	&gAppStateHFPCaImgHandler, //APP_HFP_CAIMG,
	&gAppStateHFPCaOggHandler, //APP_HFP_CAOGG,
	&gAppStateHFPCaMultyHandler, //APP_HFP_CAMULTY,
	&gAppStateFakeonHandler, //APP_FAKEON
	&gAppStateFakeoffHandler, //APP_FAKEOFF
	&gAppStateDetachHandler, //APP_DETACHING_LINK
	&gAppStateTestModeHandler, //APP_TEST_MODE
	NULL, //APP_FM
	#ifdef LINEIN_ENABLE
	&gAppStateLineInHandler, //APP_LINE_IN
	#else
	NULL, //APP_LINE_IN
	#endif
	NULL, //APP_UPDATING
	NULL, //APP_UPDATED_OK
	NULL, //APP_UPDATED_FAIL
	&gAppStateVoicePromptLangSelectHandler, //APP_VOICE_PROMPT_LANG_SELECT

	NULL, //APP_TWS_PAIRING
	NULL, //APP_INQUIRY
	NULL, //APP_MP_STATE
	NULL, //APP_MP3
};

APP_STATE_STRU gAppSitPool[MAX_STATE_NESTING_LEVEL];//mmiSitPool
APP_STATE_STRU *pFreeAppSitList;//freeMmiLlSitPtr //free link list of sit

static const HandlerData gAppStateTimerHandlerData = { app_State_TimerHandler };

static U8 gPoolFreeState = MAX_STATE_NESTING_LEVEL;

/**************************************************************************************************
* Static Functions (Timer)
**************************************************************************************************/

static U32 app_State_TimerHandler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(payload);
	UNUSED(handler);
	UNUSED(id_ext);

	switch(id)
	{
		case WATCHDOG_REBOOT_NO_MEDIA_TIMER:
			/*empty block, to prevent coverity warning*/
			break;
		case PREVENT_AUTO_POWER_ON_TIMER:
			if (key_map_gpio_key_pressed())
			{
				APP_State_SetTimer(PREVENT_AUTO_POWER_ON_TIMER, System_ReadNvkeyAutoPowerOnTimer() * MILLI_SEC_100);
			}
			else if(System_ReadNvkeyAutoPowerOnTimer())  // Not Auto Power On
			{
				APP_PowerOff_SystemOff();   //MMI should power off immediately
			}
			else
			{
				DBG_LOG_APP_STATE( "[APP_STATE] PREVENT_AUTO_POWER_ON_TIMER, boot reason:(%d), ForaReconn(%d)", 2, SYS_BOOT_REASON_STATUS(), APP_AirApp_FOTA_Reconnect());
				#if FPGA
				APP_PowerOn_Loader();
				#else
				if ((SYS_BOOT_REASON_STATUS() == BOOT_CODE_REGEN_RESET)
					#ifndef BLUETOOTH_SPEAKER
					&& !APP_ChgBat_IsChargerIn()
					#endif
					)
				{
					APP_PowerOn_Loader();
				}
				else
				{
					APP_PowerOff_SystemOff();
				}
				#endif
			}
			break;
		case DETACH_LINK_TIMER:
			{
				APP_State_SendMsgToHandler(NULL, APP_DETACH_LINK_TIMER_TIMEOUT, APP_DETACH_LINK_TIMER_TIMEOUT, TRUE);
			}
			break;

		case APP_READY_SYSTEM_OFF_TIMER:
			{
				APP_State_SendMsgToHandler(NULL, APP_PWROFF_CMD, APP_PWROFF_CMD, TRUE);
			}
			break;

		case RECONN_FAIL_ENTER_DISC_TIMER:
			{
				APP_System_Discoverable();
			}

			break;
		default:

			break;
	}

	return 0;
}

static void app_State_AutoSwitchOffBitHandler(U8 state, bool isBitAdded, BD_ADDR_T *pBdAddr)
{
	U16 bitMask;

	switch(state)
	{
		case APP_CONDISCABLE:
			bitMask = AUTO_SWITCH_OFF_BIT_CONDISCABLE;
			break;
		case APP_OFF:
			bitMask = AUTO_SWITCH_OFF_BIT_OFF;
			break;
		case APP_TEST_MODE:
			bitMask = AUTO_SWITCH_OFF_BIT_TESTMODE;
			break;
		case APP_FAKEOFF:
			bitMask = AUTO_SWITCH_OFF_BIT_FAKEOFF;
			break;
		case APP_FM:
			bitMask = AUTO_SWITCH_OFF_BIT_FM;
			break;
		case APP_MP3:
			bitMask = AUTO_SWITCH_OFF_BIT_MP3;
			break;
		case APP_LINE_IN:
			bitMask = AUTO_SWITCH_OFF_BIT_LINEIN;
			break;
		case APP_CONNECTED:
			bitMask = AUTO_SWITCH_OFF_BIT_CONNECTED;
			break;
		default:
			return;
	}

	APP_PowerOff_SetAutoSwitchOffBit(bitMask, isBitAdded, pBdAddr);
}

static void app_State_PairingTimeoutHandler(void)
{
	if(APP_Pairing_IsTimerSet(TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT) || APP_Pairing_IsTimerSet(TIMER_PAIRING_MODE_TIMEOUT))
	{
		APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT);
		APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_TIMEOUT);
	}
	else if(!APP_System_IsMpTestMode() && !APP_AirApp_IsBLEConnected())
	{
		APP_Pairing_PairModeTimeoutHandler();
	}
}

/**************************************************************************************************
* Static Functions (State)
**************************************************************************************************/
static BOOL app_State_IsDutMode(void)
{
	NVKEY_ENALBE_DUT_STRU dutMode;

	if(NVKEY_ReadFullKey(NVKEYID_APP_ENABLE_DUT, &dutMode, sizeof(dutMode)))
	{
		return dutMode.EnableDUT;
	}

	return FALSE;
}

static void app_State_UnloadState(BD_ADDR_T *pBdAddr, U8 state)
{
	if (gAppStateHandlerSet[state] && gAppStateHandlerSet[state]->ExitAction != (void*)NULL)
		gAppStateHandlerSet[state]->ExitAction(pBdAddr);
}

static void app_State_LoadState(BD_ADDR_T *pBdAddr, U8 state)
{
	U8 topState;

	DBG_LOG_APP_STATE( "[APP_STATE] load state :%s", 1, APP_StateLogString[state]);

	topState = APP_State_GetTopState(pBdAddr);

	if(gAppStateHandlerSet[state] && gAppStateHandlerSet[state]->InitAction != (void*)NULL)
	{
		gAppStateHandlerSet[state]->InitAction(pBdAddr);
	}

	if(state == topState)
	{
		if((state == APP_CONNECTED) && (CURRENT_ACTIVE_LINK_CNT == APP_Conn_GetSupportDeviceNo()))
		{
			#ifdef TWS_SETTINGS
			if(CURRENT_ACTIVE_LINK_CNT == 2)
			{
				if(APP_EOF == APP_GetNextNormalLink(pBdAddr) || !APP_State_CheckLinkInCallRelatedState(APP_GetNextNormalLink(pBdAddr)))
				{
					APP_LED_SetBgPattern(APP_LED_GetMaxLinkIndex());
				}
				else
				{
					APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(APP_State_GetTopState(APP_GetNextNormalLink(pBdAddr))));
				}
			}
			else
			{
				APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(state));
			}
			#else
			if(APP_EOF == APP_GetNextNormalLink(pBdAddr) || !APP_State_CheckLinkInCallRelatedState(APP_GetNextNormalLink(pBdAddr)))
			{
				APP_LED_SetBgPattern(APP_LED_GetMaxLinkIndex());
			}
			else
			{
				APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(APP_State_GetTopState(APP_GetNextNormalLink(pBdAddr))));
			}
			#endif
		}
		else if(APP_Conn_CheckProfileOnAllLinks() && (state == APP_CONDISCABLE || state == APP_CONNECTABLE))
		{
			return;
		}
		else
		{
			#ifdef TWS_SETTINGS
			if((state == APP_CONNECTED) && (APP_Conn_GetSupportDeviceNo() > 1))
			{
				APP_LED_SetBgPattern(LED_RED_FAST_FLASH_TWICE);
			}
			else
			{
				APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(state));
			}
			#else
			#if (defined AB1520S) || (defined AB1520U) || (defined AB1525S)
			if(!(((state == APP_FAKEON) || (state == APP_FAKEOFF)))// && APP_ChgBat_IsChargerIn()))
			#endif
			{
				APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(state));
			}
			#endif
		}
	}
	else //insert state or replace state
	{
		if(APP_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT)
		{
			if((state == APP_CONDISCABLE) && (CURRENT_ACTIVE_LINK_CNT == 1))
			{
				APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(state));
			}
		}
	}
}

static void app_State_ConnectDiscoverableEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	if(!APP_State_CheckTimer(WATCHDOG_REBOOT_NO_MEDIA_TIMER))
	{
		if(!CURRENT_ACTIVE_LINK_CNT
			&& !APP_IsAudioDeviceRegistered(NULL, AUDIO_DEVICE_FM)
			&& !APP_IsAudioDeviceRegistered(NULL, AUDIO_DEVICE_LINE_IN)
		   )
		{
			APP_Media_PushMediaEvent(MEDIA_EVT_ENTER_DISCOVERABLE);
		}
	}
}

static void app_State_ConnectDiscoverableHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch (pMsg->eventCode)
	{
		case APP_CHECK_POWERON_TIMER:
			DBG_LOG_APP_STATE( "[APP_Reconn] ------ Reconnect Check: ConnectDiscoverableHandler!! -----", 0);
			APP_ReConn_Reconnect(RECONNECT_CHECK);
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_ConnectDiscoverableExit(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	app_State_PairingTimeoutHandler();

	APP_Media_PushMediaEvent(MEDIA_EVT_EXIT_DISCOVERABLE);
	gAppCtl.miscMask2 |= APP_EXIT_DISCOVERABLE_STATE;
}

static void app_State_ConnectableEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	if(!(gAppCtl.miscMask2 & APP_EXIT_DISCOVERABLE_STATE))
	{
		if(APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT)||(APP_DIS_PDL_LESS_THAN > APP_LinkKey_GetNormalPDLNumber()))
		{
			APP_Pairing_EnterDiscoverable();
		}
	}
	gAppCtl.miscMask2 &= ~APP_EXIT_DISCOVERABLE_STATE;
}

static void app_State_ConnectableHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch (pMsg->eventCode)
	{
		case APP_CHECK_POWERON_TIMER:
			DBG_LOG_APP_STATE( "[APP_Reconn] ------ Reconnect Check: ConnectableHandler!! -----", 0);
			APP_ReConn_Reconnect(RECONNECT_CHECK);
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_OffEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//Power off procedure will be activated while ringtone is finished
	APP_Media_PushMediaEvent(MEDIA_EVT_OFF_STATE);
	//Release timers
//	MMI_TIMER_ReleaseAllLinksConnectionTimer();

	APP_State_DiscoverabilityHandle(CMD_SYSTEM_OFF);
	APP_State_ConnectabilityHandle(CMD_SYSTEM_OFF);
}

static void app_State_OffHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case APP_FAKE_MEDIA_EVT:
			if(pMsg->msgOpcode == MEDIA_EVT_OFF_STATE)
			{
				APP_AudioDriver_SendRingtoneStopCmd();
				APP_AudioDriver_SendVoicepromptStopCmd();
				APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, (U32)0);
			}
			break;

		case APP_PWROFF_CMD:
            if(!key_map_gpio_regenkey_switched())
            {
                if(APP_LED_IsFGLedOn())
                {
                    APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, HALF_SEC);
                }
                else
                {
                    APP_PowerOff_SystemOff();
                }
            }
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_FakeonEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

static void app_State_FastPowerOn(BOOL isWatchDogBoot)
{
	if(isWatchDogBoot)
		APP_SetWatchDogReboot(TRUE);

	if((isWatchDogBoot && APP_DO_NOT_RING_WHEN_WATCHDOG_TO_PWON_FEAT) || APP_AirApp_FOTA_Reconnect())
		APP_State_SetTimer(WATCHDOG_REBOOT_NO_MEDIA_TIMER, 2*ONE_SEC);

	APP_PowerOn_Loader();

	APP_Reconn_SetDelayTimer(RECONNECT_WATCHDOG, (APP_AirApp_FOTA_Reconnect() ? 100 : App_ReConnNvKey_GetWatchDogReconnectTime() * HALF_SEC));
}

static void app_State_FakeonHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
//	U8 isWakeUpPressed;
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case APP_CHECK_POWERON_TIMER:
			{
				DBG_LOG_APP_STATE( "[APP_STATE] Fake On Power On1:%d, %d, %d, %d", 4, APP_ChgCase_GetCaseType(), SYS_BOOT_REASON_STATUS(), GET_PMU_INT_STATUS(), SYS_CHECK_POWER_STATE());
				DBG_LOG_APP_STATE( "[APP_STATE] Fake On Power On2:%d, %d, %d", 3, APP_AirApp_FOTA_Reconnect(), APP_PowerOff_IsPowerOn(), APP_UKEY_SWITCH_ONOFF_TYPE_FEAT);
				if(APP_PowerOff_IsPowerOn() || APP_UKEY_SWITCH_ONOFF_TYPE_FEAT)
					break;

				if(APP_FIX_ESD_FEAT && (SYS_BOOT_REASON_STATUS() == BOOT_CODE_UNKNOWN_REASON) && (!APP_ChgBat_IsChargerIn()))
				{
					gAppCtl.isEsdPowerOn = TRUE;
					app_State_FastPowerOn(TRUE);
                    APP_Reconn_SetDelayTimer(ESD_RECONNECT_CHECK, 5000);
					break;
				}

				if((APP_ChgCase_GetCaseType() == CASE_TYPE_NONE) && (SYS_BOOT_REASON_STATUS() == BOOT_CODE_FROM_DEEP_SLEEP))
				{
					app_State_FastPowerOn(FALSE);
					break;
				}

				if((APP_ChgCase_GetCaseType() == CASE_TYPE_GPIOXY) && (SYS_BOOT_REASON_STATUS() == BOOT_CODE_FROM_DEEP_SLEEP))
				{
					app_State_FastPowerOn(FALSE);
					break;
				}

				if((SYS_BOOT_REASON_STATUS() == BOOT_CODE_WATCHDOG_RESET) || APP_AirApp_FOTA_Reconnect())
				{
                    gAppCtl.isEsdPowerOn = TRUE;
					app_State_FastPowerOn(TRUE);
					break;
				}

				if(SYS_CHECK_POWER_STATE())
				{
					app_State_FastPowerOn(FALSE);
					break;
				}
			}
			break;
		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_FakeoffEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	
	APP_State_DiscoverabilityHandle(CMD_SYSTEM_OFF);
	APP_State_ConnectabilityHandle(CMD_SYSTEM_OFF);
	
	APP_State_CancelTimer(PREVENT_AUTO_POWER_ON_TIMER);

//	//Close BLE ADV
//	#ifdef MMI_LE_ENABLE
//	MMI_LE_DisableBleLink();
//	#endif


	APP_Media_PushMediaEvent(MEDIA_EVT_OFF_STATE);

/*
	//Release timers
	MMI_TIMER_ReleaseAllLinksConnectionTimer();
*/

	//Clear low battery power on flag
	APP_SetNoConnectivityPwOnLowBat(FALSE);
	APP_SetKeyInvalidPwOnLowBat(FALSE);

	//Clear	discoverable flag
	gAppCtl.miscMask2 &= ~(APP_EXIT_DISCOVERABLE_STATE);
}

static void app_State_FakeoffHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case APP_FAKE_MEDIA_EVT:
			if(pMsg->msgOpcode == MEDIA_EVT_OFF_STATE)
			{
				APP_AudioDriver_SendRingtoneStopCmd();
				APP_AudioDriver_SendVoicepromptStopCmd();
				APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, 5*ONE_SEC);
			}
			break;

		case APP_CHECK_POWERON_TIMER:
			if(APP_PowerOff_IsPowerOn() || APP_UKEY_SWITCH_ONOFF_TYPE_FEAT)
				break;

			if(SYS_CHECK_POWER_STATE())
			{
				app_State_FastPowerOn(FALSE);
				break;
			}

			if((SYS_BOOT_REASON_STATUS() == BOOT_CODE_WATCHDOG_RESET) || APP_AirApp_FOTA_Reconnect())
			{
				app_State_FastPowerOn(TRUE);
				break;
			}
			break;

		case APP_PWROFF_CMD:
			APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, 5 * ONE_SEC);
			APP_ChgBat_CheckBatStatus();
			break;
		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_DetachEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_State_DiscoverabilityHandle(CMD_SYSTEM_OFF);
	APP_State_ConnectabilityHandle(CMD_SYSTEM_OFF);
	
	APP_State_SetTimer(DETACH_LINK_TIMER, 10 * ONE_SEC);
}

static void app_State_DetachHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	gAppCtl.msgHandled = PROCD_RELEASE;

	switch(pMsg->eventCode)
	{
		case APP_DETACH_LINK_TIMER_TIMEOUT:
			if (CURRENT_ACTIVE_LINK_CNT)
			{
				if (APP_ChgBat_IsChargerIn())
				{
					APP_Reset();
				}
				else
				{
					APP_PowerOff_Proce();
				}
			}
			break;
		case APP_LINK_DETACHED_EVT:
			if (!CURRENT_ACTIVE_LINK_CNT)
			{
				APP_PowerOff_Proce();
			}
			break;
		case APP_FAKE_MEDIA_EVT:
			if(pMsg->msgOpcode == MEDIA_EVT_OFF_STATE)
			{
				APP_AudioDriver_SendRingtoneStopCmd();
				APP_AudioDriver_SendVoicepromptStopCmd();
				APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, 5 * ONE_SEC);
			}
			break;
		case APP_PWROFF_CMD:
			APP_State_SetTimer(APP_READY_SYSTEM_OFF_TIMER, 5 * ONE_SEC);
			break;

		default:
			gAppCtl.msgHandled = NOTPROCD;
			break;
	}
}

static void app_State_DetachExit(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_State_CancelTimer(DETACH_LINK_TIMER);
}

static void app_State_AddTopStateByLink(BD_ADDR_T *pBdAddr, U8 state)
{
	APP_STATE_STRU *pNewState;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo == NULL)
	{
		return;
	}

	//Get memory from pool
	pNewState = app_State_AllocateAppState();

	pNewState->pNextState = pLinkInfo->pNestState;
	pNewState->nestState = state;

	gPoolFreeState -= 1;
	//DBG_LOG_APP_STATE( "[APP_STATE]Add Top State:%s, Free:%d", 2, APP_StateLogString[state], gPoolFreeState);

	pLinkInfo->pNestState = pNewState;

	DBG_LOG_APP_STATE( "[APP_STATE] Add State: %s, bdAddr:0x%x%x", 3, APP_StateLogString[state], FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));

	return;
}

static void app_State_AddTopStateAllLink(U8 state)
{
	APP_STATE_STRU *pNewState;
	U8 linkIndex;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		//Get memory from pool
		pNewState = app_State_AllocateAppState();

		pNewState->pNextState = (APP_STATE_STRU *)gAppCtl.appInfo[linkIndex].pNestState;
		pNewState->nestState = state;

		gPoolFreeState -= 1;
		//DBG_LOG_APP_STATE( "[APP_STATE]Add Top State:%s, Free:%d", APP_StateLogString[state], gPoolFreeState);

		gAppCtl.appInfo[linkIndex].pNestState = pNewState;
	}

	DBG_LOG_APP_STATE( "[APP_STATE] Add State: %s, bdAddr:0x%x%x", 3, APP_StateLogString[state], 0,0);

	return;
}

static bool app_State_InsertStateByLink(BD_ADDR_T *pBdAddr, U8 state, U8 beforeState)
{
	APP_INFO_STRU* pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	APP_STATE_STRU *pCurrentState, *pPreviousState;
	APP_STATE_STRU *pNewState;

	if(pLinkInfo == NULL)
	{
		return FALSE;
	}

	pPreviousState = pCurrentState = pLinkInfo->pNestState;
	while(pCurrentState != (APP_STATE_STRU *)NULL)
	{
		if(pCurrentState->nestState == beforeState)
		{
			break;
		}
		pPreviousState = pCurrentState;
		pCurrentState = pCurrentState->pNextState;
	}

	//before state not found
	if (pCurrentState == (APP_STATE_STRU *)NULL)
	{
		return FALSE;
	}

	//Get memory from pool
	pNewState = app_State_AllocateAppState();

	pNewState->pNextState = pCurrentState;
	pNewState->nestState = state;

	gPoolFreeState -= 1;
	//DBG_LOG_APP_STATE( "[APP_STATE]Insert State:%s, Free:%d", APP_StateLogString[state], gPoolFreeState);

	if (pPreviousState == pCurrentState) //the before state is the top state
	{
		pLinkInfo->pNestState = pNewState;
	}
	else
	{
		pPreviousState->pNextState = pNewState;
	}

	DBG_LOG_APP_STATE( "[APP_STATE] Insert State before: %s, state: %s, bdAddr:0x%x%x", 4, APP_StateLogString[beforeState], APP_StateLogString[state], FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));

	return TRUE;
}

static bool app_State_InsertStateAllLink(U8 state, U8 beforeState)
{
	APP_STATE_STRU *pCurrentState, *pPreviousState;
	APP_STATE_STRU *pNewState;
	U8 linkIndex;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pPreviousState = pCurrentState = gAppCtl.appInfo[linkIndex].pNestState;
		while(pCurrentState != (APP_STATE_STRU *)NULL)
		{
			if(pCurrentState->nestState == beforeState)
				break;
			pPreviousState = pCurrentState;
			pCurrentState = pCurrentState->pNextState;
		}

		//before state not found
		if (pCurrentState == (APP_STATE_STRU *)NULL)
		{
			return FALSE;
		}

		//Get memory from pool
		pNewState = app_State_AllocateAppState();

		pNewState->pNextState = pCurrentState;
		pNewState->nestState = state;

		gPoolFreeState -= 1;
		//DBG_LOG_APP_STATE( "[APP_STATE]Insert State:%s, Free:%d", APP_StateLogString[state], gPoolFreeState);

		if (pPreviousState == pCurrentState) //the before state is the top state
		{
			gAppCtl.appInfo[linkIndex].pNestState = pNewState;
		}
		else
		{
			pPreviousState->pNextState = pNewState;
		}
	}

	DBG_LOG_APP_STATE( "[APP_STATE] Insert State before: %s, state: %s, bdAddr:0x%x%x", 4, APP_StateLogString[beforeState],APP_StateLogString[state], 0,0);

	return TRUE;
}

static bool app_State_ReplaceStateByLink(BD_ADDR_T *pBdAddr, U8 newState, U8 oldState)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo == NULL)
	{
		return FALSE;
	}

	APP_STATE_STRU *pCurrentAppSitList = pLinkInfo->pNestState;
	bool isReplaced = FALSE;

	//Traversal the state list to find out specific state.
	while(pCurrentAppSitList != (APP_STATE_STRU *)NULL)
	{
		if (pCurrentAppSitList->nestState == oldState)
		{
			pCurrentAppSitList->nestState = newState;
			DBG_LOG_APP_STATE( "[APP_STATE] Replace State old: %s, new: %s, bdAddr:0x%x%x", 4, APP_StateLogString[oldState], APP_StateLogString[newState], FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));
			isReplaced = TRUE;
		}
		pCurrentAppSitList = pCurrentAppSitList->pNextState;
	}
	return isReplaced;
}

static bool app_State_ReplaceStateAllLink(U8 newState, U8 oldState)
{
	U8 linkIndex;
	bool isReplaced;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		APP_STATE_STRU *pCurrentAppSitList = gAppCtl.appInfo[linkIndex].pNestState;
		isReplaced = FALSE;

		//Traversal the state list to find out specific state.
		while(pCurrentAppSitList != (APP_STATE_STRU *)NULL)
		{
			if (pCurrentAppSitList->nestState == oldState)
			{
				DBG_LOG_APP_STATE( "[APP_STATE] Replace State old: %s, new: %s, bdAddr:0x%x%x", 4, APP_StateLogString[oldState], APP_StateLogString[newState], 0, 0);
				pCurrentAppSitList->nestState = newState;
				isReplaced = TRUE;
			}
			pCurrentAppSitList = pCurrentAppSitList->pNextState;
		}

		if(isReplaced == FALSE)
		{
			return FALSE;
		}
	}

	DBG_LOG_APP_STATE( "[APP_STATE] Replace State old: %s, new: %s, bdAddr:0x%x%x", 4, APP_StateLogString[oldState], APP_StateLogString[newState], 0, 0);

	return TRUE;
}

static void app_State_CleanNestStateByLink(BD_ADDR_T *pBdAddr)
{
	APP_STATE_STRU *pNest;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo == NULL)
	{
		return;
	}

	pNest = pLinkInfo->pNestState;

	while(pNest && pNest->nestState != APP_FAKEON)
	{
		switch (app_State_ReadStateType(pNest->nestState))
		{
			case TYPE_ONE_LINK:
				APP_State_RmvState(pBdAddr, pNest->nestState);
				pNest = pLinkInfo->pNestState;
				break;
			case TYPE_ALL_LINK:
				pNest = pNest->pNextState;
				break;
		}
	}
	DBG_LOG_APP_STATE( "[APP_STATE] CleanNestState bdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));
}

static void app_State_CleanNestStateAllLink(void)
{
	U8 linkIndex;
	APP_STATE_STRU *pNest;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		pNest = gAppCtl.appInfo[linkIndex].pNestState;
		while(pNest && pNest->nestState != APP_FAKEON)
		{
			APP_State_RmvState(pBdAddr, pNest->nestState);
			pNest = gAppCtl.appInfo[linkIndex].pNestState->pNextState;
		}
	}
	DBG_LOG_APP_STATE( "[APP_STATE] CleanNestState bdAddr:0x%x%x", 2, 0,0);
}

static bool app_State_RmvStateByLink(BD_ADDR_T *pBdAddr, U8 state)
{
	APP_STATE_STRU *pCurrentState, *pPreviousState;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	bool isRemoved = FALSE;

	if(pLinkInfo == NULL)
	{
		return FALSE;
	}

	pPreviousState = pCurrentState = pLinkInfo->pNestState;
	while(pCurrentState != (APP_STATE_STRU *)NULL)
	{
		if (pCurrentState->nestState == state)
		{
			if (pPreviousState == pCurrentState) //the removed state is the top state
			{
				pLinkInfo->pNestState = pCurrentState->pNextState;
				pPreviousState = pCurrentState->pNextState;
			}
			else
			{
				pPreviousState->pNextState = pCurrentState->pNextState;
			}

			app_State_FreeAppState(pCurrentState);
			pCurrentState = pPreviousState;

			gPoolFreeState += 1;
			DBG_LOG_APP_STATE( "[APP_STATE] Rmv state: %s, bdAddr:0x%x%x", 3, APP_StateLogString[state], FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));
			isRemoved = TRUE;
		}
		else
		{
			pPreviousState = pCurrentState;
			pCurrentState = pCurrentState->pNextState;
		}
	}

	return isRemoved;
}

static bool app_State_RmvStateAllLink(U8 state)
{
	APP_STATE_STRU *pCurrentState, *pPreviousState;
	U8 linkIndex;
	bool isRemoved;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pPreviousState = pCurrentState = gAppCtl.appInfo[linkIndex].pNestState;
		isRemoved = FALSE;
		while(pCurrentState != (APP_STATE_STRU *)NULL)
		{
			if (pCurrentState->nestState == state)
			{
				if (pPreviousState == pCurrentState) //the removed state is the top state
				{
					gAppCtl.appInfo[linkIndex].pNestState = pCurrentState->pNextState;
					pPreviousState = pCurrentState->pNextState;
				}
				else
				{
					pPreviousState->pNextState = pCurrentState->pNextState;
				}

				app_State_FreeAppState(pCurrentState);
				pCurrentState = pPreviousState;

				gPoolFreeState += 1;
				DBG_LOG_APP_STATE( "[APP_STATE] Rmv state: %s, bdAddr:0x%x%x", 3, APP_StateLogString[state], 0,0);
				isRemoved = TRUE;
			}
			else
			{
				pPreviousState = pCurrentState;
				pCurrentState = pCurrentState->pNextState;
			}
		}

		if(isRemoved == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

static APP_STATE_STRU *app_State_AllocateAppState(void)
{
	APP_STATE_STRU *pAppState;

    if(pFreeAppSitList == (APP_STATE_STRU *)NULL)
    {
        FW_Assert(FALSE);
        return (APP_STATE_STRU *)NULL;
    }

	pAppState = pFreeAppSitList;
	pFreeAppSitList = pAppState->pNextState;
	return pAppState;
}

static void app_State_FreeAppState(APP_STATE_STRU *pAppState)
{
	pAppState->pNextState = pFreeAppSitList;
	pFreeAppSitList = pAppState;
}

static U8 app_State_ReadStateType(U8 state)
{
	switch(state)
	{
		case APP_DETACHING_LINK:
		case APP_CONNECTABLE:
		case APP_CONDISCABLE:
		case APP_FAKEON:
		case APP_FAKEOFF:
		case APP_FM:
		case APP_LINE_IN:
		case APP_MP3:
		case APP_OFF:
		case APP_VOICE_PROMPT_LANG_SELECT:
		case APP_AWS_PAIRING:
		case APP_TEST_MODE:
		case APP_INQUIRY:
		case APP_MP_STATE:
		case APP_UPDATING:
		case APP_UPDATED_FAIL:
			return TYPE_ALL_LINK;
		default:
			return TYPE_ONE_LINK;
	}
}

static U8 app_State_GetTopStateByLink(BD_ADDR_T *pBdAddr)
{
	U8 topState = APP_TOTAL_STATE_NO;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo != (APP_INFO_STRU *)NULL && pLinkInfo->pNestState != (APP_STATE_STRU *)NULL)
	{
	 	topState = pLinkInfo->pNestState->nestState;
	}

	FW_Assert(topState < APP_TOTAL_STATE_NO);

 	return topState;
}

static U8 app_State_GetTopStateInDefaultLink(void)
{
	FW_Assert(gAppCtl.appInfo[DEFAULT_LINK_INDEX].pNestState != NULL);

	return gAppCtl.appInfo[DEFAULT_LINK_INDEX].pNestState->nestState;
}

static void app_State_CallStateSwitchVP(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo == NULL)
	{
		return;
	}

	if(pLinkInfo->stateEntryExit & APP_EXIT_OUTGOING_STATE)
	{
		if(APP_GetHfpIsRejectByKey(pBdAddr))
		{
			APP_Media_PushMediaEvent(MEDIA_EVT_KEY_CNLOUTGOING);
		}
		else
		{
			APP_Media_PushMediaEvent(MEDIA_EVT_AG_CNLOUTGOING);
		}
	}
}

static U32 app_State_VpSelHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);

	switch(id)
	{
		case TIMER_ID_VP_SEL:
			gAppCtl.vpRuntime.vpSelectIndex++;
			if(gAppCtl.vpRuntime.vpSelectIndex >= APP_VpGetLangCnt())
			{
				gAppCtl.vpRuntime.vpSelectIndex = 0;
				gAppCtl.vpRuntime.vpSelectRound++;
			}

			if(!gAppCtl.vpRuntime.vpLangIsCfm)
			{
				if(gAppCtl.vpRuntime.vpSelectRound < APP_VpGetSelectRound())
				{
					APP_VpSetLanguage(gAppCtl.vpRuntime.vpSelectIndex);
					APP_Media_PushMediaEvent(MEDIA_EVT_VOICE_PROMPT_LANG_SELECT);
					FW_SetTimer((Handler)&gAppStateVpSelHandle, TIMER_ID_VP_SEL, NULL, 0, APP_VpGetSelectTime() * ONE_SEC);
				}
				else
				{
					APP_VpSetLanguage(gAppCtl.vpRuntime.vpSelectOriginalIndex);
					APP_State_RmvState(APP_EOF, APP_VOICE_PROMPT_LANG_SELECT);
				}
			}
			break;
	}

	return 0;
}

static void app_State_VoicePromptLangSelectEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);
	APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	gAppCtl.vpRuntime.vpLangIsCfm = FALSE;
	gAppCtl.vpRuntime.vpSelectIndex = 0;
	gAppCtl.vpRuntime.vpSelectRound = 0;
	gAppCtl.vpRuntime.vpSelectOriginalIndex = APP_VpGetCurrentLangId();

	APP_VpSetLanguage(gAppCtl.vpRuntime.vpSelectIndex);
	APP_Media_PushMediaEvent(MEDIA_EVT_VOICE_PROMPT_LANG_SELECT);
	FW_SetTimer((Handler)&gAppStateVpSelHandle, TIMER_ID_VP_SEL, NULL, 0, APP_VpGetSelectTime() * ONE_SEC);
}

static void app_State_VoicePromptLangSelectExit(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(gAppCtl.vpRuntime.vpLangIsCfm)
		APP_Media_PushMediaEvent(MEDIA_EVT_VOICE_PROMPT_LANG_CONFIRM);

	APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
}

/**************************************************************************************************
* Internal Public Functions
**************************************************************************************************/
BOOL APP_State_CheckTimer(U8 timerType)
{
	return FW_CheckTimer((Handler)&gAppStateTimerHandlerData, timerType, 0);
}

void APP_State_CancelTimer(U8 timerType)
{
	MSG_MessageCancelFirst((Handler)&gAppStateTimerHandlerData, timerType);
}

void APP_State_SetTimer(U8 timerType, U32 time)
{
	FW_SetTimer((Handler)&gAppStateTimerHandlerData, timerType, NULL, 0, time);
}

void APP_State_Init(void)
{
	U8 index;
	//Initialize the free App state memory block and add basic state : FAKE ON
	for (index = 1; index < MAX_STATE_NESTING_LEVEL; index++)
	{
		gAppSitPool[index].pNextState = (APP_STATE_STRU*)(&gAppSitPool[index - 1]);
	}

	pFreeAppSitList = &gAppSitPool[MAX_STATE_NESTING_LEVEL - 1];
	APP_State_AddTopState(APP_EOF, APP_FAKEON);
}

void APP_State_SetAutoPowerOnTimer(void)
{
	APP_State_SetTimer(PREVENT_AUTO_POWER_ON_TIMER, System_ReadNvkeyAutoPowerOnTimer() * MILLI_SEC_100);
}

void APP_State_PassThruMsg(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	APP_STATE_STRU *pCurrentState;
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(pBdAddr == NULL || FW_CmpBdAddr(pBdAddr, &gAppCtl.appInfo[linkIndex].bdAddr))
			pCurrentState = gAppCtl.appInfo[linkIndex].pNestState;
		else
			pCurrentState = NULL;

		gAppCtl.msgHandled = NOTPROCD;
		while(pCurrentState != (APP_STATE_STRU *)NULL)
		{
			if(gAppStateHandlerSet[pCurrentState->nestState] && gAppStateHandlerSet[pCurrentState->nestState]->MsgHandler)
			{
				DBG_LOG_APP_STATE( "[APP_STATE] Pass Thru Msg: %s, Opcode:0x%x, evtcode:0x%x, linkIndex:%d", 4, APP_StateLogString[pCurrentState->nestState], pMsg->msgOpcode, pMsg->eventCode, linkIndex);
				gAppStateHandlerSet[pCurrentState->nestState]->MsgHandler(&gAppCtl.appInfo[linkIndex].bdAddr, pMsg);
			}

			if (gAppCtl.msgHandled != NOTPROCD)
				break;

			pCurrentState = pCurrentState->pNextState;
		}

		if(pBdAddr != NULL && pCurrentState != NULL)
			break;
	}
}

U8 APP_State_GetTopState(BD_ADDR_T *pBdAddr)
{
	U8 state;
	if(APP_IsSetAppLink(pBdAddr))
	{
		state = app_State_GetTopStateByLink(pBdAddr);
	}
	else
	{
		state = app_State_GetTopStateInDefaultLink();
	}
	DBG_LOG_APP_STATE( "[APP_STATE] GetTopState: BdAddr: 0x%x%x, State:%s", 3, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE), APP_StateLogString[state]);
	return state;
}

void APP_State_AddTopState(BD_ADDR_T *pBdAddr, U8 state)
{
	FW_Assert(state < APP_TOTAL_STATE_NO);

	if (APP_State_CheckNestStateByLink(pBdAddr, state))
	{
		return;
	}

	if(((state == APP_HFP_INCOMMING) && APP_State_CheckNestStateByLink(pBdAddr, APP_HFP_OUTGOING))||
	   ((state == APP_HFP_OUTGOING) && APP_State_CheckNestStateByLink(pBdAddr, APP_HFP_INCOMMING))  )
	{
		return;
	}

	app_State_AutoSwitchOffBitHandler(state, TRUE, pBdAddr);

	switch(app_State_ReadStateType(state))
	{
		case TYPE_ONE_LINK:
			app_State_AddTopStateByLink(pBdAddr, state);
//			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 *)NULL, linkIndex);
			{
				app_State_LoadState(pBdAddr, state);
			}
			break;
		case TYPE_ALL_LINK:
			app_State_AddTopStateAllLink(state);
//			MMI_HCI_UartSendUnSolicitedIndication(MMI_UART_IND_STATE_CHANGE, (U8 *)NULL, APP_EOF);
			{
				app_State_LoadState(APP_EOF, state);
			}
			break;
	}
}

void APP_State_InsertState(BD_ADDR_T *pBdAddr, U8 state, U8 beforeState)
{
	FW_Assert(state < APP_TOTAL_STATE_NO);

	if(APP_State_CheckNestStateByLink(pBdAddr, state))
	{
		if(state == APP_CONDISCABLE
		   && CURRENT_ACTIVE_LINK_CNT < APP_Conn_GetSupportDeviceNo()
		   && APP_CONDISCABLE != APP_State_GetTopState(APP_EOF))
		{
			app_State_LoadState(APP_EOF, state);
		}
		return;
	}
	else
	{
		app_State_AutoSwitchOffBitHandler(state, TRUE, pBdAddr);

		switch(app_State_ReadStateType(state))
		{
			case TYPE_ONE_LINK:
				if(app_State_InsertStateByLink(pBdAddr, state, beforeState))
				{
					app_State_LoadState(pBdAddr, state);
				}
				break;
			case TYPE_ALL_LINK:
				FW_Assert(app_State_ReadStateType(beforeState) == TYPE_ALL_LINK);
				if(app_State_InsertStateAllLink(state, beforeState))
				{
					app_State_LoadState(APP_EOF, state);
				}
				break;
		}
	}
}

void APP_State_RmvStateAllForce()
{
	U8 linkIndex;
	APP_STATE_STRU *pCurrentState, *pNextState;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pCurrentState = gAppCtl.appInfo[linkIndex].pNestState;

		while(pCurrentState != (APP_STATE_STRU *)NULL)
		{
			pNextState = pCurrentState->pNextState;
			app_State_FreeAppState(pCurrentState);
			gPoolFreeState += 1;
			pCurrentState = pNextState;
		}

		gAppCtl.appInfo[linkIndex].pNestState = NULL;
	}
}

void APP_State_RmvState(BD_ADDR_T *pBdAddr, U8 state)
{
	BOOL isTopState;

	isTopState = (APP_State_GetTopState(pBdAddr) == state) ? TRUE : FALSE;
	switch(app_State_ReadStateType(state))
	{
		case TYPE_ONE_LINK:
			if(!app_State_RmvStateByLink(pBdAddr, state))
			{
				return;
			}
			break;
		case TYPE_ALL_LINK:
			if (!app_State_RmvStateAllLink(state))
			{
				return;
			}
			pBdAddr = APP_EOF;
			break;
	}


	app_State_UnloadState(pBdAddr, state);

	if(state == APP_DETACHING_LINK)
	{
		return;
	}

	app_State_AutoSwitchOffBitHandler(state, FALSE, pBdAddr);

	state = APP_State_GetTopState(pBdAddr);

	if (state != APP_FAKEON && state != APP_LINE_IN && state != APP_FM && state != APP_MP3 && isTopState)
	{
		BD_ADDR_T *pServiceBdAddr;

		if(pBdAddr)
		{
			app_State_LoadState(pBdAddr, state);
		}
		else if((pServiceBdAddr = APP_GetServiceBdAddr()))
		{
			app_State_LoadState(pServiceBdAddr, state);
		}
		else
		{
			app_State_LoadState(APP_EOF, state);
		}
	}
	else
	{
		APP_LED_SetBgPattern(APP_LED_GetDefaultIndex(state));
	}
}

void APP_State_ReplaceStateWithEntry(BD_ADDR_T *pBdAddr, U8 newState, U8 oldState, bool withEntry)
{
	switch (app_State_ReadStateType(newState))
	{
		case TYPE_ONE_LINK:
			if (!app_State_ReplaceStateByLink(pBdAddr, newState, oldState))
			{
				return;
			}
			break;
		case TYPE_ALL_LINK:
			if (!app_State_ReplaceStateAllLink(newState, oldState))
			{
				return;
			}
			pBdAddr = APP_EOF;
			break;
	}

	app_State_UnloadState(pBdAddr, oldState);

	if(withEntry)
	{
		app_State_LoadState(pBdAddr, newState);
	}
}

void APP_State_CleanNestState(BD_ADDR_T *pBdAddr)
{
	if (pBdAddr == APP_EOF)
	{
		app_State_CleanNestStateAllLink();
	}
	else
	{
		app_State_CleanNestStateByLink(pBdAddr);
	}
}

bool APP_State_CheckNestStateByLink(BD_ADDR_T *pBdAddr, U8 state)
{
	APP_INFO_STRU *pLinkInfo;
	APP_STATE_STRU *pNestState;

	if(pBdAddr == APP_EOF)
	{
		pLinkInfo = APP_GetDefaultAppLink();
	}
	else
	{
		pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	}

	if(pLinkInfo)
	{
		pNestState = pLinkInfo->pNestState;
		while(pNestState && pNestState->nestState != APP_FAKEON)
		{
			if(pNestState->nestState == state)
			{
				return TRUE;
			}
			else
			{
				pNestState = pNestState->pNextState;
			}
		}
	}
	return FALSE;
}

bool APP_State_CheckLinkConnected(BD_ADDR_T *pBdAddr)
{
	return APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED);
}


bool APP_State_CheckLinkInCallRelatedState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if (	pLinkInfo != NULL
		&& 	pLinkInfo->pNestState->nestState >= APP_HFP_INCOMMING
		&& 	pLinkInfo->pNestState->nestState <= APP_HFP_CAMULTY)
	{
		return TRUE;
	}
	return FALSE;
}

BD_ADDR_T * APP_State_CheckOtherLinksInCallRelatedState(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;
	BD_ADDR_T *pOtherBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pOtherBdAddr && !FW_CmpBdAddr(pBdAddr, pOtherBdAddr))
		{
			if(APP_State_CheckLinkInCallRelatedState(pOtherBdAddr))
			{
				return pOtherBdAddr;
			}
		}
	}
	return NULL;
}

bool APP_State_AllLinksInCallRelatedState(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr && APP_State_CheckLinkInCallRelatedState(pBdAddr))
		{
			return TRUE;
		}
	}
	return FALSE;
}

<<<<<<< HEAD
=======
bool APP_State_IsAnyLinkInSpecifiedState(U8 state)
{
	U8 linkIndex;
	BD_ADDR_T *pBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if((pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex)))
		{
			if(state == APP_State_GetTopState(pBdAddr))
				return TRUE;
		}
	}

	return FALSE;
}

>>>>>>> db20e11 (second commit)
bool APP_State_CheckLinkInCallActiveState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(		pLinkInfo != NULL
	   && 	pLinkInfo->pNestState->nestState >= APP_HFP_CALLACTIVE
	   && 	pLinkInfo->pNestState->nestState <= APP_HFP_CAMULTY)
	{
		return TRUE;
	}
	return FALSE;
}

bool APP_State_CheckOtherLinksInCallActiveState(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;
	BD_ADDR_T *pOtherBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pOtherBdAddr && !FW_CmpBdAddr(pBdAddr, pOtherBdAddr))
		{
			if(APP_State_CheckLinkInCallActiveState(pOtherBdAddr))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool APP_State_AllLinksInCallActiveState(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr && APP_State_CheckLinkInCallActiveState(pBdAddr))
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool APP_State_CheckLinkInCallNotifyState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(		pLinkInfo != NULL
		&& 	pLinkInfo->pNestState->nestState >= APP_HFP_INCOMMING
		&& 	pLinkInfo->pNestState->nestState <= APP_HFP_OUTGOING)
	{
		return TRUE;
	}
	return FALSE;
}

bool APP_State_CheckLinkInOutGoingState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo != NULL && pLinkInfo->pNestState->nestState == APP_HFP_OUTGOING)
	{
		return TRUE;
	}
	return FALSE;
}

bool APP_State_CheckOtherLinksInOutGoingState(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;
	BD_ADDR_T *pOtherBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pOtherBdAddr && !FW_CmpBdAddr(pBdAddr, pOtherBdAddr))
		{
			if(APP_State_CheckLinkInOutGoingState(pOtherBdAddr))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

bool APP_State_CheckLinkInIncomingState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo != NULL && pLinkInfo->pNestState->nestState == APP_HFP_INCOMMING)
	{
		return TRUE;
	}
	return FALSE;
}

BD_ADDR_T * APP_State_CheckOtherLinksInIncomingState(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;
	BD_ADDR_T *pOtherBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pOtherBdAddr && !FW_CmpBdAddr(pBdAddr, pOtherBdAddr))
		{
			if(APP_State_CheckLinkInIncomingState(pOtherBdAddr))
				return pOtherBdAddr;
		}
	}
	return (BD_ADDR_T *)NULL;
}

bool APP_State_CheckLinkInImgAndCaImgState(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	if( pLinkInfo != (APP_INFO_STRU *)NULL
		&&(pLinkInfo->pNestState->nestState == APP_HFP_INCOMMING || pLinkInfo->pNestState->nestState == APP_HFP_CAIMG))
	{
		return TRUE;
	}
	return FALSE;
}

bool APP_State_CheckInFMState(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(gAppCtl.appInfo[linkIndex].pNestState->nestState == APP_FM)
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool APP_State_CheckInLineInState(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(gAppCtl.appInfo[linkIndex].pNestState->nestState == APP_LINE_IN)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL APP_State_IsInSystemAudioState(void)
{
	if(APP_State_CheckInFMState())
	{
		return TRUE;
	}

	if(APP_State_CheckInLineInState())
	{
		return TRUE;
	}

	if(APP_State_CheckInMP3State())
	{
		return TRUE;
	}

	return FALSE;
}

bool APP_State_CheckInMP3State(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(gAppCtl.appInfo[linkIndex].pNestState->nestState == APP_MP3)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void APP_State_ConnectabilityHandle(U8 action)
{
	// MMI1 interrupt should be set when page scan is off. Otherwise system will fall asleep.
	// init(MMI1) --> connectable (scan) --> connected(MMI1)
	DBG_LOG_APP_STATE( "[APP_STATE] Connect ability action:%d", 1, action);

	switch(action)
	{
		case CMD_SYSTEM_INIT:
//			MMI_MMI1Handle(TRUE);
			break;
		case CMD_SECURITY_MODE_COMPLETE:
			APP_Pairing_ConnectableHandler(CONNECTABLE_MODE);
			break;
		case CMD_SET_CONNECTABLE:
			if(!APP_PowerOff_IsPowerOn())
			{
				return;
			}

			if(CURRENT_ACTIVE_LINK_CNT < APP_Conn_GetSupportDeviceNo() && !APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
			{
				APP_Pairing_ConnectableHandler(CONNECTABLE_MODE);
				if(APP_PairNvkey_GetTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT))
				{
					APP_Pairing_SetTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);
				}
			}
			break;
		case CMD_SET_RESUME_CONNECTABLE:
			if(!APP_PowerOff_IsPowerOn() || APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
			{
				return;
			}
//			if((IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL && CURRENT_ACTIVE_LINK_CNT < APP_Conn_GetSupportDeviceNo) ||
//				(!IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL && gAppCtl.mmiTimerInfo.sysTimer.connectableTimeoutTimer))
			{
				APP_Pairing_ConnectableHandler(CONNECTABLE_MODE);
			}
			break;
		case CMD_SET_NON_CONNECTABLE:
			if(APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
			{
				return;
			}
//			if(IS_ALWAYS_CONNECTEBLE_UNTIL_LINK_FULL)
			{
//				return;
			}
			/* fallthrough */
		case CMD_SET_NON_CONNECTABLE_ANYWAY:
			if(!APP_PowerOff_IsPowerOn())
			{
				APP_Pairing_CancelTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);
				return;
			}
			/* fallthrough */
		case CMD_SYSTEM_OFF:
			APP_Pairing_ConnectableHandler(NON_CONNECTABLE_MODE);
			APP_Pairing_CancelTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);
			break;
	}
}

void APP_State_DiscoverabilityHandle(U8 action)
{
	DBG_LOG_APP_STATE( "[APP_STATE] Discover ability action:%d", 1, action);

	switch(action)
	{
		case CMD_SPECIAL_LIMITED_DISCOVERABLE:
			if(!APP_PowerOff_IsPowerOn())
			{
				return;
			}
			APP_Pairing_DiscoverableHandler(SPECIAL_LIMITED_DISCOVERABLE_MODE |LIMITED_DISCOVERABLE_MODE);
			break;
		case CMD_LIMITED_DISCOVERABLE:
			if(!APP_PowerOff_IsPowerOn())
			{
				return;
			}
			APP_Pairing_DiscoverableHandler(LIMITED_DISCOVERABLE_MODE);
			break;

		case CMD_SET_NORMAL_DISCOVERABLE:
			if(!APP_PowerOff_IsPowerOn())
			{
				return;
			}

			APP_Pairing_DiscoverableHandler(GENERAL_DISCOVERABLE_MODE);
			APP_Pairing_SetPairModeTimer();

			if(app_State_IsDutMode())
			{
				PM_GAP_EnableDutMode();
			}
			break;

		case CMD_SET_NON_DISCOVERABLE:
			if(APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT))
			{
				return;
			}
            /* fallthrough */
		case CMD_SET_NON_DISCOVERABLE_ANYWAY:
			if(!APP_PowerOff_IsPowerOn())
			{
				APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_TIMEOUT);
				return;
			}
            /* fallthrough */
		case CMD_SYSTEM_OFF:
			APP_Pairing_DiscoverableHandler(NON_DISCOVERABLE_MODE);
			break;

		case CMD_SET_RESUME_DISCOVERABLE:
			if((APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT) && APP_Conn_GetSupportDeviceNo() > CURRENT_ACTIVE_LINK_CNT) ||
				(!APP_PairNvkey_IsFeatureOn(APP_ALWAYS_ENA_DISCOVER_MODE_FEAT) && APP_Pairing_IsTimerSet(TIMER_PAIRING_MODE_TIMEOUT)))
			{
				APP_Pairing_DiscoverableHandler(GENERAL_DISCOVERABLE_MODE);
			}
			break;
	}
}

U8 APP_State_CountConnectedDev(void)
{
	U8 linkIndex, count = 0;
	BD_ADDR_T * pBdAddr;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr && APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED))
		{
			count++;
		}
	}
	return count;
}

void APP_State_ConnectedEntry(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		if(pLinkInfo->stateEntryExit & APP_EXIT_CALLACTIVE_STATE)
		{
			pLinkInfo->stateEntryExit |= ~(APP_EXIT_IMGCALL_STATE|APP_EXIT_OUTGOING_STATE);
		}
	}
	else
	{
		return;
	}

	//MMI_DRV_ScoModeControl(linkIndex, TRUE, FALSE);
	if((pLinkInfo->stateEntryExit &(APP_EXIT_OUTGOING_STATE|APP_EXIT_CALLACTIVE_STATE)))
	{
		if(APP_IsScoActiveConnect(pLinkInfo))
		{
			APP_HfpSco_DisconnectRequest(pBdAddr);
		}
	}
	if((pLinkInfo->stateEntryExit &(APP_EXIT_OUTGOING_STATE | APP_EXIT_CALLACTIVE_STATE | APP_EXIT_IMGCALL_STATE)))
	{
		if(APP_Hfp_IsFeatureOn(APP_SUPPORT_MULTI_HFP_FEAT))
		{
			BD_ADDR_T *pOtherBdAddr;
			if((pOtherBdAddr = APP_State_CheckOtherLinksInCallRelatedState(pBdAddr)) != (BD_ADDR_T *)NULL)
			{
				APP_VolManage_SetSpkMuteByLink(pOtherBdAddr, SET_MUTE_OFF);
			}
		}
	}

	app_State_CallStateSwitchVP(pBdAddr);

	if(pLinkInfo->stateEntryExit & (APP_EXIT_IMGCALL_STATE|APP_EXIT_OUTGOING_STATE|APP_EXIT_CALLACTIVE_STATE))
	{
		pLinkInfo->stateEntryExit &= ~(APP_EXIT_IMGCALL_STATE|APP_EXIT_OUTGOING_STATE|APP_EXIT_CALLACTIVE_STATE);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		if(APP_Hfp_IsHspConnected(pBdAddr))
		{
			APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_SCO_HSP);
			APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO_HSP);
		}
		else
#endif            
		{
			APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_SCO);
			APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
			APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO);
			APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_MUSIC_CALL_NOTIFY);
		}
	}
	APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);

	APP_ChgBat_ResetTimeOutDectionTimer();

	if(pLinkInfo->subState < APP_VDIAL_CONNECTING_HFP || pLinkInfo->subState > APP_RDIAL_STARTING)
	{
		pLinkInfo->subState = APP_SBY_NONE_SUBSTATE;
	}

	//Set Volume as history
	if(pBdAddr && FW_CmpBdAddr(&gAppCtl.serviceBdAddr, pBdAddr) && !APP_State_IsInSystemAudioState())
	{
		APP_VolManage_SpkVolumeRemoteHandler(pBdAddr, APP_VolManage_GetCurrentScoSoundLevel(pBdAddr), APP_HfpSco_IsSupportCodecWideBand(pBdAddr) ? COMPONENT_SCO_WB : COMPONENT_SCO_NB);
	}
	APP_SetHfpIsRejectByKey(pBdAddr, FALSE);

	APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);
	APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_CALL_STATE);
}

void APP_State_ConnectedExit(BD_ADDR_T *pBdAddr)
{
//	U8 keyState = APP_GetKeyState();
	UNUSED(pBdAddr);

	APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_CALL_STATE);

	// Not detach after audio transfer
	if(APP_PowerOff_IsPowerOn())
	{
		#ifdef HFP_Profile
		if(!APP_HFP_CheckTimer(pBdAddr, TIMER_ID_AUDIO_TRANSFER_DETACH))
		#endif
		{
			if(APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.miscMask & APP_LINK_LOSS)
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_LINK_LOSS);
			}
			else
			{
				APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
			}
		}

		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_CALL_STATE);
	}

	#ifdef LINEIN_ENABLE
	if(APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && APP_GetCurrentAudioDevice() == AUDIO_DEVICE_LINE_IN)
	{
		APP_AudioDevSetResumeTimer(2 * ONE_SEC);
	}
	#endif
}

void APP_State_PowerOffHandle(void)
{
	if(APP_State_CheckInFMState())
	{
		APP_State_RmvState(APP_EOF, APP_FM);
	}
	if(APP_State_CheckInLineInState())
	{
		APP_State_RmvState(APP_EOF, APP_LINE_IN);
	}
	if(APP_State_CheckInMP3State())
	{
		APP_State_RmvState(APP_EOF, APP_MP3);
	}

	APP_State_DiscoverabilityHandle(CMD_SYSTEM_OFF);
	APP_State_ConnectabilityHandle(CMD_SYSTEM_OFF);
}

U8 APP_State_GetConnectedCount(void)
{
	U8 linkIndex;
	U8 count = 0;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr && APP_State_CheckLinkConnected(pBdAddr))
		{
			count++;
		}
	}

	return count;
}

void APP_State_ReleaseTimerSystemOff(void)
{
	APP_State_CancelTimer(APP_READY_SYSTEM_OFF_TIMER);
}

void APP_State_SendMsgToHandler(BD_ADDR_T *pBdAddr, U16 msgCode, U16 evtCode, BOOL isSetMsgHabdled)
{
	APP_MSG_TYPE_STRU *pMsg;

	if((pMsg = APP_GetAppCmdMsgMemory(msgCode)) != (APP_MSG_TYPE_STRU *)NULL)
	{
		if(evtCode != APP_NONE_EVT)
			pMsg->eventCode = evtCode;
		if(isSetMsgHabdled)
			gAppCtl.msgHandled = NOTPROCD;
		APP_State_PassThruMsg(pBdAddr, pMsg);
		FW_FreeMemory (pMsg);
	}
}

