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
#include "App_Connection.h"
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "App_Nvram_Type.h"
#include "App_ReConnection.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_LinkKey.h"
#include "App_PowerOff.h"
#include "App_Media.h"
#include "App_Hfp.h"
#include "App_SniffHandler.h"
#include "APP_HfpScoHandler.h"
#include "APP_HfpIncomingCallMediaHandler.h"
#include "APP_Qos.h"
#include "App_EventOpcode.h"
#include "PM_Interface.h"
#include "App_System.h"
#include "APP_AirApp.h"
#include "system.h"
#include "App_Fcd.h"
#ifdef AIROHA_WWE_ENABLE
#include "App_Vad.h"
#endif
#include "drv_audio_nvkey.h"
#include "App_Ble.h"


/**************************************************************************************************
* Definition
**************************************************************************************************/
log_create_module(APP_CONN, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	TIMER_ID_CLEAR_LINK_KEY,
	TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY,
	TIMER_ID_ACTIVE_DISCONNECT_OTHER_PROFILES,
	TIMER_ID_QOS,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 currentSupportDeviceNo;
	U8 isProfileRegistered;
	BD_ADDR_T activeDiscBda;
}APP_CONNECTION_CTL_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_Connection_Handler(Handler handler, U16 id, void *msg, U32 handler_id);
static U32 app_ConnectionTimer_Handler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static const HandlerData gAppConnectionHandle = { app_Connection_Handler };
static const HandlerData gAppConnectionTimerHandle = { app_ConnectionTimer_Handler };
static APP_CONNECTION_CTL_STRU gAppConnCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static void app_Conn_SetConnected(BD_ADDR_T *pBdAddr)
{
	BOOL isInConnected;

	APP_SetAppLinkBdAddr(pBdAddr);
	APP_AddServiceBdAddr(pBdAddr);

	////Load link history info which was set in EEPROM
	APP_LinkKey_LoadHistory(pBdAddr);

	#ifdef LINEIN_ENABLE
	if(APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && APP_State_CheckInLineInState())
	{
		APP_State_RmvState(APP_EOF, APP_LINE_IN);
	}
	#endif

	isInConnected = APP_State_CheckNestStateByLink(pBdAddr, APP_CONNECTED) ? TRUE : FALSE;
	APP_State_InsertState(pBdAddr, APP_CONNECTED, APP_State_CheckNestStateByLink(pBdAddr, APP_CONDISCABLE) ? APP_CONDISCABLE : APP_CONNECTABLE);

	DBG_LOG_APP_Connection( "[APP_CONN] PM_CONNECT_CFM isInConn:%d ConnDevCnt:%d", 2, isInConnected, APP_State_CountConnectedDev());
	if(APP_State_CountConnectedDev() >= APP_Conn_GetSupportDeviceNo())
	{
		if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
		{
			APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
		}
	}

	App_SniffEnable(pBdAddr, APP_SNIFF_ACL_READY_SLC_NOT_YET);

	if(!isInConnected)
	{
		APP_Media_PushMediaEvent((U16)(MEDIA_EVT_SLC_CONNECTED + APP_State_CountConnectedDev()-1));
		FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, NULL, (U32)APP_GetAppLinkByBdAddr(pBdAddr), 5*ONE_SEC);
	}

	//MMI_HCI_SendVCmdAudioQosSetup(linkIndex, SERVICE_TYPE_BEST_EFFORT, FALSE, FALSE);
}

static void app_Conn_ConnectCfmHandler(PM_CONNECT_CFM_T * cfm)
{
	DBG_LOG_APP_Connection( "[APP_CONN] PM_CONNECT_CFM PID:0x%x ConnProfileCnt:%d bda:0x%x%x", 4, cfm->profileID, cfm->profileCntConnectedEvt, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
	//memcpy((U8*)&gAppCtl.latestConnectedBdaddr.addr[0], (U8*)&cfm->bdAddr, sizeof(BD_ADDR_T)); //Read rssi
	APP_ReConn_DeleteInvalidList();

	FW_ReleaseTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_ACTIVE_DISCONNECT_OTHER_PROFILES, 0);

	if(cfm->profileCntConnectedEvt && cfm->profileID == PROFILE_A2DP)
	{
		PM_SetReconnectProfile(&cfm->bdAddr, PROFILE_AVRCP, APP_ReConn_GetProfileMaskByBdAddr(&cfm->bdAddr) ? 0 : 1000);
	}

#ifdef AIROHA_WWE_ENABLE
    if(cfm->profileID == PROFILE_HANDSFREE && drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ALWAYS_ON_FEATURE))
    {
        APP_VAD_On();
    }
#endif

	// Any profile is connected we call this is in connected state
	if (cfm->profileCntConnectedEvt == 1)
	{
		app_Conn_SetConnected(&cfm->bdAddr);
	}
//	MMI_TIMER_ReleaseDetachTimer(linkIndex);

	if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset())
	{
		if(!APP_LinkKey_IsBdAddrInHistory(&cfm->bdAddr))
		{
			APP_LinkKey_UpdateHistory(&cfm->bdAddr, TRUE);
		}
	}
}

void APP_Conn_RemoveConnectedState(BD_ADDR_T *pBdAddr, U8 profileCnt)
{
	U8 state;
	if(!profileCnt)
	{
		state = APP_State_GetTopState(pBdAddr);

		if(state >= APP_HFP_INCOMMING && state <= APP_HFP_CAMULTY)
		{
			APP_State_RmvState(pBdAddr, state);
		}
		APP_State_RmvState(pBdAddr, APP_CONNECTED);
		APP_ClearAppLinkBdAddr(pBdAddr);
	}
}

static void app_Conn_DisconnectCfmHandler(PM_DISCONNECT_CFM_T *cfm)
{
	BD_ADDR_T *pBdAddr = &cfm->bdAddr;

	DBG_LOG_APP_Connection("[APP_CONN] PM_DISCONNECT_CFM PID:0x%x, profile cnt:%d, isActice Disc:%d", 3, cfm->profileID,
				cfm->profileCntConnectedEvt, APP_IsLinkActiveDisconnecting(pBdAddr));

#ifdef AIROHA_WWE_ENABLE
	if(cfm->profileID == PROFILE_HANDSFREE && drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ALWAYS_ON_FEATURE))
	{
		BD_ADDR_T *pNextBdAddr = APP_GetNextNormalLink(pBdAddr);
		if(pNextBdAddr == APP_EOF || !PM_IsProfileConnected(pNextBdAddr, PROFILE_HANDSFREE))
		{
			APP_VAD_Off();
		}
	}
#endif
	if(cfm->profileID == PROFILE_AVRCP)
	{
		//FW_Memcpy(&gAppConnCtl.activeDiscBda, pBdAddr, sizeof(BD_ADDR_T));
		//FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_ACTIVE_DISCONNECT_OTHER_PROFILES, NULL, 0, 800);
	}
	else
	{
		FW_ReleaseTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_ACTIVE_DISCONNECT_OTHER_PROFILES, 0);
	}
	
<<<<<<< HEAD
#ifdef PROFILE_AMA_ENABLE
    if (!PM_IsProfileConnected(pBdAddr, PROFILE_A2DP) && !PM_IsProfileConnected(pBdAddr, PROFILE_HANDSFREE) && !PM_IsProfileConnected(pBdAddr, PROFILE_AVRCP))
    {
=======
#if defined(PROFILE_AMA_ENABLE) || defined(AIR_GFP_ENABLE)
    if (!PM_IsProfileConnected(pBdAddr, PROFILE_A2DP) && !PM_IsProfileConnected(pBdAddr, PROFILE_HANDSFREE) && !PM_IsProfileConnected(pBdAddr, PROFILE_AVRCP))
    {
#ifdef PROFILE_AMA_ENABLE
>>>>>>> db20e11 (second commit)
        if (PM_IsProfileConnected(pBdAddr, PROFILE_AMA))
        {
            APP_Ama_Disconnect(pBdAddr);
            PM_DisconnectSpecificProfile(pBdAddr, PROFILE_AMA);
        }
<<<<<<< HEAD
=======
#endif

#ifdef AIR_GFP_ENABLE
        if (PM_IsProfileConnected(pBdAddr, PROFILE_GFP))
        {
            PM_DisconnectSpecificProfile(pBdAddr, PROFILE_GFP);
        }
#endif
>>>>>>> db20e11 (second commit)
    }
#endif

	// active release link
	if(!APP_PowerOff_IsPowerOn() || APP_IsLinkActiveDisconnecting(pBdAddr))
	{
		APP_Conn_RemoveConnectedState(pBdAddr, cfm->profileCntConnectedEvt);
		APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
	}
	else
	{
		if (!APP_IsActiveLink(pBdAddr))
			goto REMOVE_CONNECTED_IMMEDIATLY;

		if(cfm->profileCntConnectedEvt)
			return;


		//if(gMMI_ctl.mmiTimerInfo.connTimer[pBdAddr].delayRemoveConnectedTimer == (OST XDATA_PTR)NULL) //asus zenfone 5
		//{
		//	SYS_SetTimer(&gMMI_ctl.mmiTimerInfo.connTimer[pBdAddr].delayRemoveConnectedTimer, 640L);
		//	return;
		//}

		REMOVE_CONNECTED_IMMEDIATLY:
		APP_Conn_RemoveConnectedState(pBdAddr, cfm->profileCntConnectedEvt);
	}
}

static U32 app_Connection_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch (id)
	{
		case PM_REGISTER_APP_CFM:
			DBG_LOG_APP_Connection( "[APP_CONN] PM_REGISTER_APP_CFM", 0);
			break;

		case PM_CONNECT_CFM:
			app_Conn_ConnectCfmHandler((PM_CONNECT_CFM_T *)msg);
			break;

		case PM_DISCONNECT_CFM:
			app_Conn_DisconnectCfmHandler((PM_DISCONNECT_CFM_T *)msg);
			break;

		case PM_REGISTER_ALL_PROFILE_CFM:
			gAppConnCtl.isProfileRegistered = TRUE;
            APP_Ble_StartGattService();
			if(
				(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE) || APP_State_CheckNestStateByLink(APP_EOF, APP_CONNECTABLE)) ||
				( APP_AirApp_FOTA_Reconnect() || (SYS_BOOT_REASON_STATUS() == BOOT_CODE_FROM_DEEP_SLEEP) || !APP_PowerOff_IsPowerOn())
			)
			{
				APP_State_SendMsgToHandler(NULL, APP_CHECK_POWERON_TIMER, APP_CHECK_POWERON_TIMER, FALSE);
			}
			else
			{
				return 2;
			}
			break;

		default:
			DBG_LOG_APP_Connection( "[APP_CONN] Undefined message = %d", 1, id);
			break;
	}
	return 0;
}

static U32 app_ConnectionTimer_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    APP_INFO_STRU *pLinkInfo;
	UNUSED(handler);
	UNUSED(msg);

	switch(id)
	{
		case TIMER_ID_CLEAR_LINK_KEY:
			APP_LinkKey_ClearAllHistory();
			break;

		case TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY:
			if(LINK_DISCONNECTED == APP_Conn_GetAgLinkState())
				FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_CLEAR_LINK_KEY, NULL, 0, 0);
			else
				APP_ConnSetTimerWaitDisconnectedClearLinkKey();
			break;

		case TIMER_ID_ACTIVE_DISCONNECT_OTHER_PROFILES:
			APP_Conn_ReleaseProfileLinkAndDetach(&gAppConnCtl.activeDiscBda);
			break;
        case TIMER_ID_QOS:
            pLinkInfo = (APP_INFO_STRU *)handler_id;
            DBG_LOG_APP_Connection( "[APP_CONN] TIMER_ID_QOS, aclState:%d, BdAddr:0x%x%x", 3, pLinkInfo->aclState, FW_bdaddr_to_2U32(&pLinkInfo->bdAddr, TRUE), FW_bdaddr_to_2U32(&pLinkInfo->bdAddr, FALSE));
            if(pLinkInfo->aclState == ACL_STATE_ON)
            {
                if(!APP_AudioDspIsStreaming(&pLinkInfo->bdAddr, AUDIO_A2DP))
                {
                    APP_Qos_SetQos(&pLinkInfo->bdAddr, QOS_SERVICE_BEST_EFFORT);
                }
            }
            break;
	}

	return 0;
}

static void app_Conn_InitSupportDeviceNo(void)
{
	gAppConnCtl.currentSupportDeviceNo = APP_MAX_PAGE_DEVICE;
	PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
}

/**************************************************************************************************
* Public Functions (Connection API)
**************************************************************************************************/
void APP_RemoveServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pNextBdAddr;

	if(!APP_PowerOff_IsPowerOn() || !pBdAddr)
		return;

	if(APP_IsServiceBdAddr(pBdAddr))
	{
		switch(APP_GetCurrentAudioDevice())
		{
			case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                
>>>>>>> db20e11 (second commit)
			case AUDIO_DEVICE_SCO_HSP:
#endif                
			case AUDIO_DEVICE_SCO_IDLE:
			case AUDIO_DEVICE_MUSIC:
			case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			case AUDIO_DEVICE_CALL_STATE:
			case AUDIO_DEVICE_AVRCP_PLAY:
			case AUDIO_DEVICE_KEY_AT_CMD:
				DBG_LOG_APP_Connection( "[APP_SYS] Zombie Audio Device", 0);
				/*fallthrough*/
			case AUDIO_DEVICE_LINE_IN:
			case AUDIO_DEVICE_FM:
			case AUDIO_DEVICE_MIC_TO_SPK:
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
			case AUDIO_DEVICE_MP3:
#endif//MP3_LOCAL_PLAYBACK_MMI_CTRL

			case AUDIO_DEVICE_NONE:
                if((pNextBdAddr = APP_GetNextNormalLink(pBdAddr)) == APP_EOF)
                {
                    APP_ClearServiceBdAddr();
                }
                if(pNextBdAddr)
                    APP_SetServiceBdAddr(pNextBdAddr);
                else
                    APP_ClearServiceBdAddr();
				break;

			default:
				FW_Assert(FALSE);
                break;
		}
	}
}

void APP_AddServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pNextBdAddr;

	if(!APP_PowerOff_IsPowerOn() || !pBdAddr)
		return;

	if(	APP_IsLinkInHighAudioPriority(pBdAddr))
	{
		pNextBdAddr = APP_GetNextNormalLink(pBdAddr);
		if(pNextBdAddr && APP_State_CheckNestStateByLink(pNextBdAddr, APP_CONNECTED))
			return;

		APP_SetServiceBdAddr(pBdAddr);
	}
}

void APP_Conn_CreateActiveProfileConnection(BD_ADDR_T * pBdAddr)
{
	PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
	PM_ConnectProfile(pBdAddr, PROFILE_HANDSFREE);
	PM_ConnectProfile(pBdAddr, PROFILE_AVRCP);
	#ifdef PROFILE_AMA_ENABLE
	PM_ConnectProfile(pBdAddr, PROFILE_IAP2);
	#endif
	//APP_Reconn_SetReconnectingBdAddr(pBdAddr);
}

void APP_Conn_ReleaseProfileLinkAndDetach(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if (!pLinkInfo)
	{
		if(!APP_PowerOff_IsPowerOn())
		{
			APP_PowerOff_Proce();
		}
		DBG_LOG_APP_Connection( "[APP_CONN] No Link to Detach", 0);
		return;
	}


	if (pLinkInfo->linkPara.miscMask & APP_LINK_DETACHING)
		return;

	pLinkInfo->linkPara.miscMask |= APP_ACTIVE_DISCONNECT;
	App_SniffDisable(pBdAddr, APP_SNIFF_DISC);
	APP_ReConn_DeleteListByBdAddr(pBdAddr);

	APP_ImgMedia_AudioVoiceHandler(pBdAddr, IMG_AUDIO_VOICE_STOP);
	APP_HfpSco_DisconnectRequest(pBdAddr);
	//release one profile at a time
	//force detach and power off if timeout
	if (!PM_DisconnectLoadedProfile(pBdAddr) || APP_SCO_EXISTS(pBdAddr))
	{

	}
	else if (APP_IsActiveLink(pBdAddr))
	{
		BtSdpServerDisconnectReq((U8 *)pBdAddr, SDP_SERVER_DISCON_SINGLE);
		if(!APP_State_GetConnectedCount() && !APP_PowerOff_IsPowerOn())
		{
			APP_State_AddTopState(APP_EOF, APP_DETACHING_LINK);
		}
	}
}

void APP_Conn_ReleaseMultiLink(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr)
		{
			APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
		}
	}
}

BOOL APP_Conn_CheckProfileOnAllLinks(void)
{
	U8 linkIndex;
	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddr && PM_GetConnectedProfileCount(pBdAddr, TRUE))  // not include SDP
		{
			return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Conn_Register(void)
{
	PM_RegisterApp((Handler)&gAppConnectionHandle);
	app_Conn_InitSupportDeviceNo();
}

U8 APP_Conn_GetSupportDeviceNo(void)
{
	return gAppConnCtl.currentSupportDeviceNo;
}

void APP_Conn_SetSupportDeviceNo(BOOL isIncrease)
{
	if(isIncrease)
	{
		if(gAppConnCtl.currentSupportDeviceNo < APP_MAX_PAGE_DEVICE)
		{
			gAppConnCtl.currentSupportDeviceNo++;
			PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
		}
	}
	else
	{
		if (gAppConnCtl.currentSupportDeviceNo > 1)
		{
			gAppConnCtl.currentSupportDeviceNo--;
			PM_SetMaximumEDRLinkNum(gAppConnCtl.currentSupportDeviceNo);
		}
	}
}

BOOL APP_Conn_PowerOffHandler(void)
{
	APP_ReConn_ClearCtl();

	if(CURRENT_ACTIVE_LINK_CNT)
	{
		APP_Conn_ReleaseMultiLink();
		return TRUE;
	}
	return FALSE;
}
BOOL APP_Conn_IsProfilesReady(void)
{
	return (gAppConnCtl.isProfileRegistered) ? TRUE : FALSE;
}

U8 APP_Conn_GetAgLinkState(void)
{
	BD_ADDR_T *pBdAddr;
	U8 agState = PM_Conn_GetAgLinkState();

	if(agState == LINK_DISCONNECTED)
	{
		pBdAddr = APP_GetServiceBdAddr();
		if(APP_IsActiveLink(pBdAddr))
			return LINK_DISCONNECTING;
	}

	return agState;
}

U16 APP_ConnCheckTimerClearLinkKey(void)
{
	return FW_CheckTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_CLEAR_LINK_KEY, 0);
}

void APP_ConnSetTimerWaitDisconnectedClearLinkKey(void)
{
	FW_SetTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_WAIT_DISCONNECTED_CLEAR_LINK_KEY, NULL, 0, 0);
}

void APP_Conn_ReleaseQosTimer(APP_INFO_STRU * pLinkInfo)
{
    FW_ReleaseTimer((Handler)&gAppConnectionTimerHandle, TIMER_ID_QOS, (U32)pLinkInfo);
}

