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
#include "App_Battery.h"
#include "Volume_NvkeyHandler.h"
#include "App_VolumeAiroThruProcess.h"
#include "AudioDSP_StreamManager.h"
#include "DSP_Control.h"
#include "App_PowerOn.h"
#include "PM_Connection.h"
#include "App_SmartChgCase.h"
#include "App_Mcsync_Reconnection.h"
#include "App_A2dp.h"
#include "drv_anc.h"

#ifdef MODULE_LOG_FEATURE
#include "BtModule_log.h"
#endif
#include "App_Customer_Func.h"
#include "AirApp.h"
#include "APP_AirApp.h"
#include "App_Customer.h"
#include "DrvSmartCase.h"
#include "chargercase_common.h"
#include "BtMCSync.h"
#include "App_RhoHandler.h"
#include "App_MCSync_Message.h"
#include "App_MCSync_RHO.h"
#include "App_MCSyncRho_NvkeyHandler.h"
#include "App_RhoDominator.h"
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h" 
#endif
#include "App_Ble.h"

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_Gfp.h"
#endif

extern uint8_t captouch_wakeup_enable(VOID);

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_PowerOff_TimerHandler(Handler t, U16 id, void *payload, U32 id_ext);
static void app_PowerOff_AutoSwitchOffTimeoutHandler(void);
static void app_PowerOff_SetPoweredOnOffStatus(U8 status);
static bool app_PowerOff_CheckAutoPowerOff(void);
static BOOL app_PowerOff_IsAllSwitchOffMaskNotSet(void);
static void app_PowerOff_SetAutoSwitchOffByLink(U16 bitMask, bool isBitAdded, BD_ADDR_T *pBdAddr);
static void app_PowerOff_AwsIdleTimeoutHandler(void);
static BOOL app_PowerOff_ReasonCheckOK(U8 reason);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData gPowerOffTimerHandlerData;
APP_POWER_CTL_STRU gAppPowerCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U32 app_PowerOff_TimerHandler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	UNUSED(payload);
	UNUSED(handler);
	UNUSED(id_ext);
    //logPrint(LOG_OS, PRINT_LEVEL_INFO, "[HALT DBG:11 %d]", 1, id);

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
		case TIMER_AWS_IDLE_POWEROFF:
			if(BtAwsMce_IsDefaultRoleAgent())
			{
				DBG_LOG_APP_SYSTEM( "[APP_MCSync][Agent] AWS Idle Timer Time out, PowerOff!!", 0);
				app_PowerOff_AwsIdleTimeoutHandler();
			}
			break;
		case TIMER_AUTO_SWITCH_OFF_SYNC_TIMEOUT:
			if(APP_PowerOff_IsPowerOn())
				APP_PowerOff_Loader(POWEROFF_REASON_SYNC);
			break;
        #ifdef XIAOAI_DEVELOPMENT
        case TIMER_XIAOAI_RESET:
            APP_Reset_Loader();		
            break;
        #endif
		case TIMER_XIAOAI_POWER_OFF:
			if(APP_PowerOff_IsPowerOn())
				APP_PowerOff_Loader(POWEROFF_REASON_XIAOAI);				
			break;
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case TIMER_SET_FOLLOWER_POWER_OFF_PKT_TIMEOUT:
			if(gAppPowerCtl.followerIFPKTRetryCount >= 3)
			{
				DBG_LOG_APP_SYSTEM( "[APP_MCSync][Share] Follower Power off PKT send over 5 times!!", 0);
				FW_Assert(FALSE);
			}
			else
			{
				DBG_LOG_APP_SYSTEM( "[APP_MCSync][Share] Follower Power off PKT send again!!", 0);
				APP_MCSYNC_SendSyncPowerOff(POWEROFF_REASON_SYNC, TRUE, FALSE);
				APP_PowerOff_SetTimer(TIMER_SET_FOLLOWER_POWER_OFF_PKT_TIMEOUT);
				gAppPowerCtl.followerIFPKTRetryCount++;
			}
			break;
		#endif
			
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


static void app_PowerOff_AwsIdleTimeoutHandler(void)
{
	APP_PowerOff_Loader(POWEROFF_REASON_AWS_IDLE);
}

static BOOL app_PowerOff_ReasonCheckOK(U8 reason)
{
	return ((reason != POWEROFF_REASON_CHARGING_CASE) && ((reason != POWEROFF_REASON_KEY && reason != POWEROFF_REASON_SYNC) || !BtMCSync_IsFeatureSupport(AWSMCE_SYNC_PWR_OFF_KEY_FEAT)));
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
	U8 isReset = (APP_PowerOff_IsPowerOn()) ? FALSE : TRUE;

	DBG_LOG_APP_SYSTEM( "[APP_PWROFF] Power Off Loader. Reason:%d, FakeOffState:%d, IsPowerOn:%d", 3,
		reason, APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF), APP_PowerOff_IsPowerOn());

	APP_State_CancelTimer(RECONN_FAIL_ENTER_DISC_TIMER);
	

	#ifdef MODULE_LOG_FEATURE
		BtModuleLog_SendLogHandler(MODULE_LOG_MODULE_APP_POWER, 
			MODULE_LOG_MODULE_APP_POWER_SUB_MODULE_POWER_OFF, reason);
	#endif

	if(APP_MCSync_RHO_IsReceiveRHOPkt() && BtAwsMce_IsDefaultRolePartner())
	{
		gAppPowerCtl.powerOff.isNeedToPowerOff = TRUE;
		gAppPowerCtl.powerOff.reason = reason;
		return;
	}
	switch(reason)
	{
		case POWEROFF_REASON_KEY:
			if(BtMCSync_IsFeatureSupport(AWSMCE_SYNC_PWR_OFF_KEY_FEAT))
			{
<<<<<<< HEAD
				#ifdef MCSYNC_SHARE_MODE
=======
				#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
				DBG_LOG_APP_SYSTEM( "[APP_PWROFF] Power Off Loader, share mode:%d, role:%d", 2, 
					BtMCSync_GetShareMode(), BtAwsMce_GetDefaultRole());

				if(MCSYNC_SHARE_MODE_FOLLOWER_ENABLE == BtMCSync_GetShareMode())
				{
					APP_MCSYNC_SendSyncPowerOff(POWEROFF_REASON_SYNC, TRUE, TRUE);
					APP_PowerOff_SetTimer(TIMER_SET_FOLLOWER_POWER_OFF_PKT_TIMEOUT);
					gAppPowerCtl.followerIFPKTRetryCount = 1;
				}
				else if(MCSYNC_SHARE_MODE_NORMAL_ENABLE == BtMCSync_GetShareMode()
					&& BtAwsMce_IsDefaultRolePartner())		
				{
					APP_MCSYNC_SendSyncPowerOff(POWEROFF_REASON_SYNC, FALSE, FALSE);
					APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_SYNC_TIMEOUT);
				}
				else
				#endif
				{
					APP_MCSYNC_SendSyncPowerOff(POWEROFF_REASON_SYNC, FALSE, FALSE);
					APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_SYNC_TIMEOUT);
				}
				return;
			}
			break;
		case POWEROFF_REASON_SHUT_DOWN_LOW_BAT:
			break;
		case POWEROFF_REASON_AUTO_SWITCH_OFF_TIMER:
			break;
		case POWEROFF_REASON_PAIRING_MODE_TO:
			break;
		case POWEROFF_REASON_TEMP_PROTECT:
			break;
		case POWEROFF_REASON_AWS_IDLE:
			APP_MCSYNC_SendSyncPowerOff(POWEROFF_REASON_SYNC, FALSE, FALSE);
			APP_PowerOff_SetTimer(TIMER_AUTO_SWITCH_OFF_SYNC_TIMEOUT);
			return;
		case POWEROFF_REASON_AWS_RHO_FINISHED:
			break;
		case POWEROFF_REASON_CHARGING_CASE:
			break;
		case POWEROFF_REASON_SMARTCASE_POWEROFF:
			break;
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case POWEROFF_REASON_SYNC:
			if(MCSYNC_SHARE_MODE_FOLLOWER_ENABLE == BtMCSync_GetShareMode())
			{
				APP_PowerOff_CancelTimer(TIMER_SET_FOLLOWER_POWER_OFF_PKT_TIMEOUT);
				gAppPowerCtl.followerIFPKTRetryCount = 0;
			}
			break;		
		#endif
		default:

			break;
	}

	if(reason == POWEROFF_REASON_SMARTCASE_POWEROFF)
	{
		if(APP_State_CheckNestStateByLink(APP_EOF, APP_FAKEOFF))
		{
			APP_PowerOff_Proce();
			return;
		}
		else
		{
			DBG_LOG_APP_SYSTEM( "[APP_PWROFF] not fakeoff , poweroff...", 0);
		}
	}
	else
	{
		if(!APP_PowerOff_IsPowerOn())
			return;
	}

	DBG_LOG_APP_SYSTEM( "[APP_MCSync] RHO for Power Off :%d, %d, %d, %d", 4,
	         gAppPowerCtl.powerOffAfterRHO, APP_MCSyncNvkey_IsRhoFeatureOn(APP_RHO_TRIGGER_FEATURE_POWEROFF),
			 APP_MCSync_Rho_IsAbleToStartRho(), app_PowerOff_ReasonCheckOK(reason));

	if(!gAppPowerCtl.powerOffAfterRHO && APP_MCSyncNvkey_IsRhoFeatureOn(APP_RHO_TRIGGER_FEATURE_POWEROFF)
		&& APP_MCSync_Rho_IsAbleToStartRho() && app_PowerOff_ReasonCheckOK(reason) && APP_SmtChgCse_IsAbleToRHO())
	{
		if(APP_MCSync_Rho_Start())
		{
			gAppPowerCtl.powerOffAfterRHO = TRUE;
			APP_PowerOff_SetTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
			return;
		}
	}

	APP_Customer_ResetPassThruPara();
	APP_Customer_PowerOff_Handler();
	

	APP_Mcsync_ClearAllNotReconnMask();
	APP_Mcsync_ClearLinkLossBdAddr();
	APP_System_RoleModeLeave(FALSE, TRUE);

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_MCSync_Share_ExitMode();
#endif
	APP_A2dp_GameModeOff();
	gAppPowerCtl.powerOffAfterRHO = FALSE;
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
<<<<<<< HEAD
		#ifdef PROFILE_GFP_ENABLE
        APP_Gfp_DisableAdv();
        #endif
		APP_Ble_Disconnect(NULL);		
	}

    #ifdef PROFILE_GFP_ENABLE
        APP_Gfp_DisableAdv();
=======
		APP_Ble_Disconnect(NULL);		
	}

    #ifdef AIR_GFP_ENABLE
    APP_Ble_DisableAdv(BLE_ADV_GFP_MASK);
>>>>>>> db20e11 (second commit)
    #endif

//    #if defined(LE_SUPPORTED_HOST_FLASH) && defined(MMI_LE_ENABLE)
//    MMI_LE_DisconnectPowerOff();
//    #endif

	APP_Media_PowerOffHandle(isReset);
	APP_ChgBat_PowerOffHandler();
	APP_SetPowerOnStatus(FALSE);
	if(APP_Conn_CheckTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING) != 0)
	{
		APP_Conn_SetTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING, TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING_DELAY);
	}
	
	DBG_LOG_APP_SYSTEM( "[APP_PWROFF] IsDefaultRolePartner(%d), SYNC_PWR_OFF_KEY_TO_AGENT_FEAT(%d)", 2,
			BtAwsMce_IsDefaultRolePartner(), BtMCSync_IsFeatureSupport(AWSMCE_SYNC_PWR_OFF_KEY_FEAT));

	if((!BtAwsMce_IsDefaultRolePartner() || !BtMCSync_IsFeatureSupport(AWSMCE_SYNC_PWR_OFF_KEY_FEAT)) &&
		APP_Conn_PowerOffHandler())
	{
        //logPrint(LOG_OS, PRINT_LEVEL_INFO, DBG_HANG_01_String, 0);
		APP_PowerOff_SetTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
	}
	else
	{
        //logPrint(LOG_OS, PRINT_LEVEL_INFO, "[HALT DBG:2]", 0);
		APP_PowerOff_Proce();
	}
}

<<<<<<< HEAD
=======
void APP_PowerOff_KeepFakeOff(bool enable)
{
    gAppPowerCtl.powerOff.KeepFakeOff = enable;
}

bool APP_PowerOff_IsKeepFakeOff(void)
{
    return gAppPowerCtl.powerOff.KeepFakeOff;
}

static bool app_PowerOff_CheckKeepFakeOff(void)
{
    bool ret = FALSE;
    U8 reason = gAppPowerCtl.powerOff.reason;

    if (APP_PowerOff_IsKeepFakeOff())
    {
        if (reason != POWEROFF_REASON_SHUT_DOWN_LOW_BAT && reason != POWEROFF_REASON_TEMP_PROTECT && reason != POWEROFF_REASON_PAIRING_MODE_TO)
        {        
            DBG_LOG_APP_SYSTEM("[APP_PWROFF] ChargerIn:%d, SmartCase:%d, chargerFull:%d", 3, APP_ChgBat_IsChargerIn(), APP_ChgCase_IsCaseSmartType(), APP_ChgBat_IsChargerComplete());
            if (APP_ChgBat_IsChargerIn())
            {
                if (APP_ChgCase_IsCaseSmartType() && APP_ChgBat_IsChargerComplete())
                {
                    ret = TRUE;//for SystemOff():keep fake off when smart charger full.
                }
            }
            else
            {
                ret = TRUE;//keep fake off when charger out if not the case low bat or temp protect or pairing mode TO.
            }
        }
    }
    DBG_LOG_APP_SYSTEM("[APP_PWROFF] ret:%d, KeepFakeOff:%d, reason:%d, batStatus:%d", 4, ret, APP_PowerOff_IsKeepFakeOff(), reason, APP_ChgBat_GetBatStatus());

    return ret;
}

>>>>>>> db20e11 (second commit)
void APP_PowerOff_Proce(void)
{
	U8 linkIndex;
    //logPrint(LOG_OS, PRINT_LEVEL_INFO, "[HALT DBG:3]", 0);

	if(APP_PowerOff_IsPowerOn())
	{
		if(APP_State_CheckNestStateByLink(APP_EOF, APP_DETACHING_LINK))
		{
			APP_State_RmvState(APP_EOF, APP_DETACHING_LINK);
		}
		return;
	}
	
	APP_SetWatchDogReboot(FALSE);
	APP_State_CleanNestState(APP_EOF);


	APP_Conn_DetachAllLinks();
<<<<<<< HEAD
	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN)
=======
	if(APP_ChgBat_GetBatStatus() >= APP_BAT_CHARGER_IN || app_PowerOff_CheckKeepFakeOff())
>>>>>>> db20e11 (second commit)
	{//Charging battery Now
		APP_State_AddTopState(APP_EOF, APP_FAKEOFF);
	}
	else
	{
		APP_State_AddTopState(APP_EOF, APP_OFF);
	}

	//Release current running timers.
	APP_Pairing_CancelTimer(TIMER_PAIRING_MODE_TIMEOUT);
	APP_PowerOff_CancelTimer(TIMER_AUTO_SWITCH_OFF_TIMEOUT);
	APP_PowerOff_CancelTimer(TIMER_SET_HW_POWER_OFF_TIMEOUT);
	APP_PowerOff_CancelTimer(TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT);
	APP_Pairing_CancelTimer(TIMER_PAIRING_CONNECTABLE_TIMEOUT);
	APP_RhoDomn_CancelMonitorTimer();
	if(APP_Conn_CheckTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING) != 0)
	{
		APP_Conn_ReleaseTimer(TIMER_ID_POWER_OFF_IN_ROLE_CHANNGING);
	}

	APP_ReConn_ClearCtl();
	APP_LinkKey_ClearConnectedFlag();

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		gAppCtl.appInfo[linkIndex].subState = APP_NONE_SUBSTATE;
		gAppCtl.appInfo[linkIndex].stateEntryExit = 0;
	}
    //logPrint(LOG_OS, PRINT_LEVEL_INFO, "[HALT DBG:4]", 0);
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

		case TIMER_AWS_IDLE_POWEROFF:
			if(BtAwsMce_IsDefaultRoleAgent())
			{
				if(APP_PowerNvkey_GetTimer(timerType))
				{
					DBG_LOG_APP_SYSTEM( "[APP_MCSync][Agent] Set AWS Idle Timer:%d s", 1, APP_PowerNvkey_GetTimer(timerType));
					FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, APP_PowerNvkey_GetTimer(timerType)*ONE_SEC);
				}
			}
			break;

		case TIMER_AUTO_SWITCH_OFF_SYNC_TIMEOUT:
			FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, 600);
			break;
<<<<<<< HEAD
		#ifdef MCSYNC_SHARE_MODE
=======
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case TIMER_SET_FOLLOWER_POWER_OFF_PKT_TIMEOUT:
			FW_SetTimer(&gPowerOffTimerHandlerData, timerType, NULL, 0, 2000);
			break;		
		#endif
		#ifdef XIAOAI_DEVELOPMENT
		case TIMER_XIAOAI_POWER_OFF:
			if(APP_PowerOff_IsPowerOn())
				APP_PowerOff_Loader(POWEROFF_REASON_XIAOAI);			
			break;
		
		case TIMER_XIAOAI_RESET:
			APP_Reset_Loader();
			break;
		#endif
		
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
	app_PowerOff_SetPoweredOnOffStatus((isOn) ? SYS_POWERED_ON : SYS_POWERED_OFF);

	if(isOn)
		SYS_SET_POWER_STATE();
	else
		SYS_CLEAR_POWER_STATE();
}

U8 APP_PowerOff_IsPowerOn(void)
{
	return (gAppPowerCtl.powerOnOffStatus == SYS_POWERED_ON) ? TRUE : FALSE;
}

void APP_PowerOff_SystemOff(void)
{
<<<<<<< HEAD
	//APP_AudioDspClose();
=======
    bool KeepFakeOff;
    //APP_AudioDspClose();
>>>>>>> db20e11 (second commit)
    DSP_AudioCommand_AudioOff();

	DBG_LOG_APP_SYSTEM( "[APP_PWROFF] System off, charger in:%d, power on:%d", 2, APP_ChgBat_IsChargerIn(), APP_PowerOff_IsPowerOn());

	if(APP_PowerOff_IsPowerOn())
	{
		return;
	}

<<<<<<< HEAD
	//if(APP_POWER_KEY_TO_DEEP_SLEEP_FEAT)
	if((!APP_ChgBat_IsChargerIn()) && captouch_wakeup_enable() && (!APP_ChgCase_IsCaseSmartType() || (DrvCharger_GetSmartCaseState() != STATE_CASE_OFF)))
=======
    KeepFakeOff = app_PowerOff_CheckKeepFakeOff();
	//if(APP_POWER_KEY_TO_DEEP_SLEEP_FEAT)
	if((!APP_ChgBat_IsChargerIn()) && captouch_wakeup_enable() && (!APP_ChgCase_IsCaseSmartType() || (DrvCharger_GetSmartCaseState() != STATE_CASE_OFF)) && !KeepFakeOff)
>>>>>>> db20e11 (second commit)
	{
		U8 PwrLv = 3;
		APP_AudioDspClose();
        //APP_SendRaceCmd(RACE_SET_CONSTRAINED_POWER_LEVEL, NULL, 0);//deep sleep
		APP_SendRaceCmd(0x0208, &PwrLv, sizeof(U8));
		APP_SendRaceCmd(0x020B, &PwrLv, sizeof(U8));
		APP_State_ReleaseTimerSystemOff();
	}
	else
	{
<<<<<<< HEAD
		if(APP_ChgBat_IsChargerIn())
=======
		if(APP_ChgBat_IsChargerIn() || KeepFakeOff)
>>>>>>> db20e11 (second commit)
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
	APP_ChgBat_ClearPowerOffState();
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


BOOL APP_PowerOff_FillRhoData(void *pData)
{
	APP_RHO_POWEROFF_CTL_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appPowerOff;

	if(!pDataToFill)
	{
		DBG_LOG_APP_SYSTEM( "[RHO_APP_PwrOff] Old Agent fill data error", 0);
		return FALSE;
	}

	FW_Memcpy(pDataToFill, gAppPowerCtl.autoSwitchOffMask, sizeof(U16)*FW_MAX_LINK_IDX);

	return TRUE;
}

BOOL APP_PowerOff_AssignRhoData(void *pData)
{
	APP_RHO_POWEROFF_CTL_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appPowerOff;

	if(!pDataToGet)
	{
		DBG_LOG_APP_SYSTEM( "[RHO_APP_PwrOff] New Agent Assign data error", 0);
		return FALSE;
	}

	FW_Memcpy(gAppPowerCtl.autoSwitchOffMask, pDataToGet, sizeof(U16)*FW_MAX_LINK_IDX);

	return TRUE;
}

BOOL APP_PowerOff_IsPowerOffAfterRHO(void)
{
	return (gAppPowerCtl.powerOffAfterRHO == TRUE);
}

void APP_PowerOff_ClearPowerOffAfterRHO(void)
{
	gAppPowerCtl.powerOffAfterRHO = FALSE;
}

