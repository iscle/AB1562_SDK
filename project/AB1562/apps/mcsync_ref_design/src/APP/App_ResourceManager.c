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
#include "App_Avrcp.h"
#include "App_System.h"
#include "App_Interface.h"
#include "App_A2dp.h"
#include "App_Hfp.h"
#include "App_State.h"
#include "App_LinkKey.h"
#include "App_ReConnection.h"
#include "Hfp_atCommand.h"
#include "APP_HfpState.h"
#include "App_EventOpcode.h"
#include "App_Fcd.h"
#include "bt_aws_mce.h"
#include "BtMCSync.h"
#include "App_RhoHandler.h"
#include "App_RaceCmd.h"
#include "App_PowerOff.h"
#include "App_Pairing.h"
#include "App_SniffHandler.h"
#include "APP_Qos.h"
#include "App_PeqProcess.h"
#include "AudioDSP_StreamManager.h"
#include "App_Connection.h"
#include "App_MCSync.h"
#include "APP_AirApp.h"
#include "App_Mcsync_LinkKey.h"
#include "App_Mcsync_Reconnection.h"
#include "App_SmartChgCase.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#include "BtAma.h"
#include "App_AMAKey.h"
#endif
#include "App_InEarDetection.h"
#include "App_MCSync_Message.h"
#include "App_VolumeSetting.h"
#include "syslog.h"
#include "chargercase_common.h"
#include "ChargerSmartCase.h"
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
#include "App_Tile.h"
#endif
>>>>>>> db20e11 (second commit)

log_create_module(APP_RSRC_MNG, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_RHO_CTL,
	APP_RHO_LINK_INFO,
	APP_RHO_TEST_MODE,
	APP_RHO_RACECMD,
	APP_RHO_POWER_OFF,
	APP_RHO_PAIRING,
	APP_RHO_SYSTEM,
	APP_RHO_SNIFF,
	APP_RHO_QOS,
	APP_RHO_PEQ,
	APP_RHO_HFP,
	APP_RHO_AUDIO_DSP,
	APP_RHO_CONNECTION,
	APP_RHO_AWSMCE,
	APP_RHO_AVRCP,
	APP_RHO_A2DP,
	APP_RHO_AIRAPP,
	APP_RHO_AMA,
	APP_RHO_INEAR_DETECTION,
    APP_RHO_XIAOAI,
<<<<<<< HEAD
=======
    APP_RHO_TILE,
>>>>>>> db20e11 (second commit)
	APP_RHO_MODULE_NUM
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BOOL (*app_Fill_Handler)(void *pToBeFilled);
	BOOL (*app_Assign_Handler)(void *pToAssign);
	BOOL (*app_GetMsgNum_Handler)(void);
    void (*app_NewPartner_Hanlder)(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr);
}APP_RHO_HANDLE_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static BOOL app_FillRhoCtlData(void *pData);
static BOOL app_AssignRhoCtlData(void *pData);
static BOOL app_FillRhoLinkInfoData(void *pData);
static BOOL app_AssignRhoLinkInfoData(void *pData);
static BOOL app_FillRhoDataHandler(APP_RHO_DATA_STRU *pDataToFill);
static BOOL app_AssignRhoDataHandler(APP_RHO_DATA_STRU *pDataToGet);

/**************************************************************************************************
* Variables
**************************************************************************************************/
APP_CTL_STRU gAppCtl;
static const APP_RHO_HANDLE_STRU gAppRhoHandler[APP_RHO_MODULE_NUM] =
{
    {app_FillRhoCtlData, 			app_AssignRhoCtlData, 			NULL,                           NULL},
    {APP_AudioDSP_FillRhoData,		APP_AudioDSP_AssignRhoData,		NULL,                           NULL},
    {app_FillRhoLinkInfoData, 		app_AssignRhoLinkInfoData, 		NULL,                           NULL},
    {NULL, 							NULL, 							APP_TestMode_GetMsgNumInQueue,  NULL},
    {NULL, 							NULL, 							APP_RaceCmd_GetMsgNumInQueue,   NULL},
    {APP_PowerOff_FillRhoData, 		APP_PowerOff_AssignRhoData, 	NULL,                           NULL},
    {NULL, 							NULL, 							APP_Pairing_GetMsgNumInQueue,   NULL},
    {NULL, 							NULL, 							APP_System_GetMsgNumInQueue,    NULL},
    {NULL, 							NULL, 							NULL,                           NULL},
    {NULL, 							NULL, 							APP_Qos_GetMsgNumInQueue,       NULL},
    {APP_PeqProc_FillRhoData,		APP_PeqProc_AssignRhoData,		NULL,                           NULL},
    {APP_Hfp_FillRhoData,			APP_Hfp_AssignRhoData,			APP_Hfp_GetMsgNumInQueue,       NULL},
    {APP_Conn_FillRhoData,			APP_Conn_AssignRhoData,			APP_Conn_GetMsgNumInQueue,      APP_Conn_NewPartner},
    {APP_AWSMCE_FillRhoData,		APP_AWSMCE_AssignRhoData,		APP_AWSMCE_GetMsgNumInQueue,    NULL},
    {NULL,							NULL,							APP_Avrcp_GetMsgNumInQueue,     NULL},
    {APP_a2dp_fill_rho_data,		APP_a2dp_assign_rho_data,		APP_A2dp_GetMsgNumInQueue,      APP_A2dp_NewPartner},
    {APP_AirApp_FillRhoData,        APP_AirApp_AssignRhoData,		APP_AirApp_GetMsgNumInQueue,    NULL},
#ifdef PROFILE_AMA_ENABLE
    {APP_Ama_FillRhoData,       	APP_Ama_AssignRhoData,			NULL,                           APP_Ama_NewPartner},
#else
    {NULL,							NULL,							NULL,                           NULL},
#endif
    {APP_InEarDetection_FillRhoData,APP_InEarDetection_AssignRhoData,NULL,                          NULL},
#ifdef XIAOAI_DEVELOPMENT
    {APP_Xiaoai_FillRhoData,        APP_Xiaoai_AssignRhoData,       NULL,                           APP_Xiaoai_NewPartner},
#else
    {NULL,							NULL,							NULL,                           NULL},
#endif
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
    {APP_Tile_FillRhoData,          APP_Tile_AssignRhoData,         NULL,                           NULL},
#else
    {NULL,							NULL,							NULL,                           NULL},
#endif

>>>>>>> db20e11 (second commit)
};

AT_ESD APP_ESD_STRU gAppEsd;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_FillRhoCtlData(void *pData)
{
	APP_RHO_CTL_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appCtl;

	if(!pDataToFill)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_CTL] Fill RHO data error", 0);
		return FALSE;
	}

	FW_Memcpy(&pDataToFill->serviceBdAddr, &gAppCtl.serviceBdAddr, sizeof(BD_ADDR_T));
	pDataToFill->scoCount = gAppCtl.scoCount;
	pDataToFill->msgHandled = gAppCtl.msgHandled;
	pDataToFill->enterDiscoverableAfterDisc = gAppCtl.enterDiscoverableAfterDisc;
	pDataToFill->miscMask2 = gAppCtl.miscMask2;
	FW_Memcpy(&pDataToFill->vpRuntime, &gAppCtl.vpRuntime, sizeof(VoicePromptRuntime));
	FW_Memcpy(&pDataToFill->driverCtl, &gAppCtl.driverCtl, sizeof(APP_DRIVER_RELATED_CTL_STRU));
	pDataToFill->vprtSoundLevel = gAppCtl.vprtSoundLevel;
	pDataToFill->lineinSoundLevel = gAppCtl.lineinSoundLevel;
	pDataToFill->mp3SoundLevel = gAppCtl.mp3SoundLevel;
	FW_Memcpy(&pDataToFill->redialBdAddr, &gAppCtl.redialBdAddr, sizeof(BD_ADDR_T));
	FW_Memcpy(&pDataToFill->imgCallBdAddr, &gAppCtl.imgCallBdAddr, sizeof(BD_ADDR_T));
	pDataToFill->dspAncAtState = gAppCtl.dspAncAtState;
	pDataToFill->airoThruSoundLevel = gAppCtl.airoThruSoundLevel;
	FW_Memcpy(&pDataToFill->mcsyncCtl, &gAppCtl.awsmceCtl, sizeof(APP_AWSMCE_CTL_STRU));
    /* Do not change Mute Mask in RHO */
    /* FW_Memcpy(&pDataToFill->speakerMuteMask, &gAppCtl.speakerMuteMask, sizeof(U16)); */

	return TRUE;
}

static BOOL app_AssignRhoCtlData(void *pData)
{
	APP_RHO_CTL_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appCtl;

	if(!pDataToGet)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_CTL] Assign RHO data error", 0);
		return FALSE;
	}

	FW_Memcpy(&gAppCtl.serviceBdAddr, &pDataToGet->serviceBdAddr, sizeof(BD_ADDR_T));
	gAppCtl.scoCount = pDataToGet->scoCount;
	gAppCtl.msgHandled = pDataToGet->msgHandled;
	gAppCtl.enterDiscoverableAfterDisc = pDataToGet->enterDiscoverableAfterDisc;
	gAppCtl.miscMask2 = pDataToGet->miscMask2;
	FW_Memcpy(&gAppCtl.vpRuntime, &pDataToGet->vpRuntime, sizeof(VoicePromptRuntime));
	FW_Memcpy(&gAppCtl.driverCtl, &pDataToGet->driverCtl, sizeof(APP_DRIVER_RELATED_CTL_STRU));
	gAppCtl.vprtSoundLevel = pDataToGet->vprtSoundLevel;
	gAppCtl.lineinSoundLevel = pDataToGet->lineinSoundLevel;
	gAppCtl.mp3SoundLevel = pDataToGet->mp3SoundLevel;
	FW_Memcpy(&gAppCtl.redialBdAddr, &pDataToGet->redialBdAddr, sizeof(BD_ADDR_T));
	FW_Memcpy(&gAppCtl.imgCallBdAddr, &pDataToGet->imgCallBdAddr, sizeof(BD_ADDR_T));
	gAppCtl.dspAncAtState = pDataToGet->dspAncAtState;
	gAppCtl.airoThruSoundLevel = pDataToGet->airoThruSoundLevel;
	FW_Memcpy(&gAppCtl.awsmceCtl, &pDataToGet->mcsyncCtl, sizeof(APP_AWSMCE_CTL_STRU));
    /* Do not change Mute Mask in RHO */
    /* FW_Memcpy(&gAppCtl.speakerMuteMask, &pDataToGet->speakerMuteMask, sizeof(U16)); */

	return TRUE;
}

static BOOL app_FillRhoLinkInfoData(void *pData)
{
	APP_RHO_INFO_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appLinkInfo;
	APP_INFO_STRU *pAgLinkInfo;
	APP_STATE_STRU *pNestState;
	U8 stateIndex;

	if(!pDataToFill)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] Fill RHO data error", 0);
		return FALSE;
	}

	if(!(pAgLinkInfo = APP_GetAppLinkByBdAddr(APP_GetServiceBdAddr())))
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] Fill RHO data no AG Link", 0);
		return FALSE;
	}

	FW_Memcpy(&pDataToFill->bdAddr, &pAgLinkInfo->bdAddr, sizeof(BD_ADDR_T));
	pDataToFill->aclState = pAgLinkInfo->aclState;
	FW_Memcpy(&pDataToFill->linkPara, &pAgLinkInfo->linkPara, sizeof(APP_LINK_SETTING_STRU));
	FW_Memcpy(&pDataToFill->scoCtl, &pAgLinkInfo->scoCtl, sizeof(SCO_CTL_STRU));

	pNestState = pAgLinkInfo->pNestState;
	stateIndex = 0;
	while(pNestState)
	{
		if(stateIndex > 7)
		{
			DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] AG Link State number error", 0);
			FW_Assert(FALSE);
            break;
		}
		pDataToFill->nestState[stateIndex] = pNestState->nestState;
		pNestState = pNestState->pNextState;
		stateIndex++;
	}
	pDataToFill->nestStateNumber = stateIndex;

	pDataToFill->audioDevLinkMask = pAgLinkInfo->audioDevLinkMask;
	pDataToFill->subState = pAgLinkInfo->subState;
	pDataToFill->stateEntryExit = pAgLinkInfo->stateEntryExit;
	FW_Memcpy(&pDataToFill->hfpCtl, &pAgLinkInfo->hfpCtl, sizeof(APP_HFP_CTL_STRU));
	FW_Memcpy(&pDataToFill->a2dpCtl, &pAgLinkInfo->a2dpCtl, sizeof(APP_A2DP_CTL_STRU));
	FW_Memcpy(&pDataToFill->avrcpCtl, &pAgLinkInfo->avrcpCtl, sizeof(APP_AVRCP_CTL_STRU));
	pDataToFill->sniffProfileMask = pAgLinkInfo->sniffProfileMask;
	FW_Memcpy(&pDataToFill->qosCtl, &pAgLinkInfo->qosCtl, sizeof(APP_QOS_CTL));

	return TRUE;
}

static BOOL app_AssignRhoLinkInfoData(void *pData)
{
	APP_RHO_INFO_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appLinkInfo;
	APP_INFO_STRU *pNormalLink, *pSpecialLink;
	APP_STATE_STRU *pTmpNestState;
	U8 stateIndex, tempAclState;
	BD_ADDR_T tempBda;

	if(!pDataToGet)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] Assign RHO data error", 0);
		return FALSE;
	}

	if(!(pNormalLink = APP_GetEmptyAppLink()))
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] Assign RHO data no Link", 0);
		return FALSE;
	}

	FW_Memcpy(&pNormalLink->bdAddr, &pDataToGet->bdAddr, sizeof(BD_ADDR_T));
	APP_LinkKey_UpdateHistory(&pNormalLink->bdAddr, TRUE);
	pNormalLink->aclState = pDataToGet->aclState;
	FW_Memcpy(&pNormalLink->linkPara, &pDataToGet->linkPara, sizeof(APP_LINK_SETTING_STRU));
	FW_Memcpy(&pNormalLink->scoCtl, &pDataToGet->scoCtl, sizeof(SCO_CTL_STRU));
	if((pSpecialLink = APP_GetAppLinkByBdAddr(BtAwsMce_ReadLocalBdAddr())))
	{
		pNormalLink->scoCtl.scoSink = pSpecialLink->scoCtl.scoSink;
		pNormalLink->scoCtl.scoHandle = pSpecialLink->scoCtl.scoHandle;
		pNormalLink->scoCtl.scoCodec = pSpecialLink->scoCtl.scoCodec;
		pTmpNestState = pSpecialLink->pNestState;
		tempAclState = pSpecialLink->aclState;
		FW_Memcpy(&tempBda, &pSpecialLink->bdAddr, sizeof(BD_ADDR_T));
		FW_Memset(pSpecialLink, 0, sizeof(APP_INFO_STRU));
		pSpecialLink->pNestState = pTmpNestState;
		pSpecialLink->aclState = tempAclState;
		FW_Memcpy(&pSpecialLink->bdAddr, &tempBda, sizeof(BD_ADDR_T));

		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] New Agent Sco Sink:0x%x, BdAddr:0x%x%x", 3,
			(U32)pNormalLink->scoCtl.scoSink, FW_bdaddr_to_2U32(APP_GetBdAddrBySCOSink(pNormalLink->scoCtl.scoSink),TRUE), FW_bdaddr_to_2U32(APP_GetBdAddrBySCOSink(pNormalLink->scoCtl.scoSink),FALSE));
	}
	else
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] New Agent Sco Sink Error", 0);
	}

	APP_State_RmvStateAllForce();
	for(stateIndex = 0; stateIndex < pDataToGet->nestStateNumber; stateIndex++)
	{
		U8 stateIdx = pDataToGet->nestStateNumber - 1 - stateIndex;

		APP_State_AddTopState(&pDataToGet->bdAddr, pDataToGet->nestState[stateIdx]);
	}

	FW_Memcpy(&pNormalLink->audioDevLinkMask, &pDataToGet->audioDevLinkMask, sizeof(U16));
	pNormalLink->subState = pDataToGet->subState;
	pNormalLink->stateEntryExit = pDataToGet->stateEntryExit;
	FW_Memcpy(&pNormalLink->hfpCtl, &pDataToGet->hfpCtl, sizeof(APP_HFP_CTL_STRU));
	FW_Memcpy(&pNormalLink->a2dpCtl, &pDataToGet->a2dpCtl, sizeof(APP_A2DP_CTL_STRU));
	FW_Memcpy(&pNormalLink->avrcpCtl, &pDataToGet->avrcpCtl, sizeof(APP_AVRCP_CTL_STRU));

	FW_Memcpy(&pNormalLink->sniffProfileMask, &pDataToGet->sniffProfileMask, sizeof(U32));
	App_SniffCheck(&pNormalLink->bdAddr);

	FW_Memcpy(&pNormalLink->qosCtl, &pDataToGet->qosCtl, sizeof(APP_QOS_CTL));

	app_AwsMce_SendSyncAllInfo(&pNormalLink->bdAddr);
	return TRUE;
}

static BOOL app_FillRhoDataHandler(APP_RHO_DATA_STRU *pDataToFill)
{
	U8 appRhoNumIndex;

	for(appRhoNumIndex = 0; appRhoNumIndex < APP_RHO_MODULE_NUM; appRhoNumIndex++)
	{
		if(gAppRhoHandler[appRhoNumIndex].app_Fill_Handler && !gAppRhoHandler[appRhoNumIndex].app_Fill_Handler(pDataToFill))
		{
			DBG_LOG_APP_RSRC_MNG( "[APP] Fill RHO error: %s", 1, APP_RHO_MODULE_String[appRhoNumIndex]);
			return FALSE;
		}
	}

	return TRUE;
}

static BOOL app_AssignRhoDataHandler(APP_RHO_DATA_STRU *pDataToGet)
{
	U8 appRhoNumIndex;

	for(appRhoNumIndex = 0; appRhoNumIndex < APP_RHO_MODULE_NUM; appRhoNumIndex++)
	{
		if(gAppRhoHandler[appRhoNumIndex].app_Assign_Handler && !gAppRhoHandler[appRhoNumIndex].app_Assign_Handler(pDataToGet))
		{
			DBG_LOG_APP_RSRC_MNG( "[APP] Assign RHO error: %s", 1, APP_RHO_MODULE_String[appRhoNumIndex]);
			return FALSE;
		}
	}

	return TRUE;
}

/**************************************************************************************************
* Public Functions (Link)
**************************************************************************************************/
BOOL APP_IsSetAppLink(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	return (pLinkInfo) ? TRUE : FALSE;
}

BOOL APP_SetAppLinkBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;

	if (APP_IsSetAppLink(pBdAddr))
	{
<<<<<<< HEAD
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Already", 0);
		if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset() && !FW_IsBdAddrZero(BtMCSync_ReadAgentBdAddr())
		#ifdef MCSYNC_SHARE_MODE
=======
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Already Bda:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
		if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset() && !FW_IsBdAddrZero(BtMCSync_ReadAgentBdAddr())
		#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
			&& !BtMCSync_IsShareModeBdAddr(pBdAddr)		
		#endif
			/*#if SINGLE_HEADSET_MODE_ENABLE
			&& !APP_System_IsSingleHSMode()
			#endif*/
			)
		{
			APP_LinkKey_UpdateHistory(pBdAddr, TRUE);
		}
		return TRUE;
	}

	if ((pLinkInfo = APP_GetEmptyAppLink()))
	{
		FW_Memcpy(&pLinkInfo->bdAddr, pBdAddr, sizeof(BD_ADDR_T));
<<<<<<< HEAD
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr OK", 0);
=======
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr OK Bda:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
>>>>>>> db20e11 (second commit)
		return TRUE;
	}
	else
	{
<<<<<<< HEAD
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Fail (Full)", 0);
=======
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Fail (Full) Bda:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
>>>>>>> db20e11 (second commit)
		return FALSE;
	}
}

void APP_ClearAppLinkBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

<<<<<<< HEAD
=======
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] ClearAppLinkBdAddr Bda:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

>>>>>>> db20e11 (second commit)
	if(pLinkInfo && pLinkInfo->aclState == ACL_STATE_OFF && !APP_State_CheckLinkConnected(pBdAddr))
	{
		APP_STATE_STRU *pTmpNestState;
		APP_RemoveServiceBdAddr(pBdAddr);
		APP_State_SendMsgToHandler(pBdAddr, APP_LINK_DETACHED_EVT, APP_LINK_DETACHED_EVT, TRUE);

		pTmpNestState = pLinkInfo->pNestState;
		FW_Memset(pLinkInfo, 0, sizeof(APP_INFO_STRU));
		pLinkInfo->pNestState = pTmpNestState;
<<<<<<< HEAD
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Clear Link Addr", 0);
=======
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Clear Link Addr Bda:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
>>>>>>> db20e11 (second commit)
	}
}

APP_INFO_STRU *APP_GetEmptyAppLink()
{
	U8 linkIndex;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
<<<<<<< HEAD
=======
			DBG_LOG_APP_RSRC_MNG( "[APP_RM] GetEmptyAppLink Bda:0x%x%x", 2, FW_bdaddr_to_2U32(&gAppCtl.appInfo[linkIndex].bdAddr, TRUE), FW_bdaddr_to_2U32(&gAppCtl.appInfo[linkIndex].bdAddr, FALSE));
>>>>>>> db20e11 (second commit)
			return &gAppCtl.appInfo[linkIndex];
		}
	}

<<<<<<< HEAD
=======
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] GetEmptyAppLink NULL", 0);
>>>>>>> db20e11 (second commit)
	return NULL;
}

APP_INFO_STRU *APP_GetAppLinkByBdAddr(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;

	if(!pBdAddr || FW_IsBdAddrZero(pBdAddr))
		return NULL;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (FW_CmpBdAddr(pBdAddr, &gAppCtl.appInfo[linkIndex].bdAddr))
		{
			return &gAppCtl.appInfo[linkIndex];
		}
	}

	return NULL;
}

APP_INFO_STRU *APP_GetDefaultAppLink(void)
{
	return &gAppCtl.appInfo[DEFAULT_LINK_INDEX];
}

U8 APP_GetLinkIndexByBdAddr(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;

	if(!pBdAddr)
		return FW_MAX_LINK_IDX;
	//FW_Assert(pBdAddr != NULL);

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (FW_CmpBdAddr(pBdAddr, &gAppCtl.appInfo[linkIndex].bdAddr))
		{
			break;
		}
	}

	return linkIndex;
}

BD_ADDR_T *APP_GetBdAddrByLinkIndex(U8 linkIndex)
{
	if(FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		return NULL;

	return &gAppCtl.appInfo[linkIndex].bdAddr;
}

BOOL APP_IsLinkActiveDisconnecting(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr)
	{
		APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
		if(pLinkInfo)
		{
<<<<<<< HEAD
=======
			DBG_LOG_APP_RSRC_MNG( "[APP_RM] APP_IsLinkActiveDisconnecting, misc mask:0x%x", 1, pLinkInfo->linkPara.miscMask);
>>>>>>> db20e11 (second commit)
			return (pLinkInfo->linkPara.miscMask & APP_ACTIVE_DISCONNECT)? TRUE : FALSE;
		}
	}
	return FALSE;
}

<<<<<<< HEAD
=======
U8 APP_GetLinkActiveDisconnectingCnt(void)
{
	U8 linkIndex;
	U8 cnt = 0;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if( (gAppCtl.appInfo[linkIndex].linkPara.miscMask & APP_ACTIVE_DISCONNECT)
            && APP_IsActiveLink(&gAppCtl.appInfo[linkIndex].bdAddr)
            && !BtAwsMce_IsSpecialLink(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
			cnt++;
		}
	}

	return cnt;
}

>>>>>>> db20e11 (second commit)
void APP_SetACLState(APP_INFO_STRU * pLinkInfo, U8 aclState)
{
	if(pLinkInfo)
	{
		pLinkInfo->aclState = aclState;
	}
}

BOOL APP_IsActiveLink(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr)
	{
		APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
		if(pLinkInfo)
		{
			return (pLinkInfo->aclState == ACL_STATE_ON) ? TRUE : FALSE;
		}
	}
	return FALSE;
}

U8 APP_GetACLLinkCount(void)
{
	U8 linkIndex;
	U8 cnt = 0;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (gAppCtl.appInfo[linkIndex].aclState == ACL_STATE_ON)
		{
			cnt++;
		}
	}

	return cnt;
}

/**************************************************************************************************
* Public Functions (SCO)
**************************************************************************************************/
SINK APP_GetSCOSinkByBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
 	if(pLinkInfo)
	{
		if(pLinkInfo->scoCtl.scoSink)
			return pLinkInfo->scoCtl.scoSink;
	}
	return NULL;
}

BD_ADDR_T *APP_GetBdAddrBySCOSink(SINK sink)
{
	U8 linkIndex;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (gAppCtl.appInfo[linkIndex].scoCtl.scoSink == sink && !FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
			return &gAppCtl.appInfo[linkIndex].bdAddr;
		}
	}
	return NULL;
}

U16 APP_GetSCOHandleByBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo)
	{
		return pLinkInfo->scoCtl.scoHandle;
	}

	return 0;
}

void APP_SetSCOConnected(BD_ADDR_T *pBdAddr, SINK sink, U16 scoHandle, U8 scoCodec)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(!pLinkInfo)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_HFP] Set Sco connected Fail - LinkInfo NULL", 0);
		return;
	}

	pLinkInfo->scoCtl.scoSink = sink;
	pLinkInfo->scoCtl.scoHandle = scoHandle;
	if(pLinkInfo->scoCtl.scoState == SCO_STATE_ACTIVE_CONNECT)
	{
		pLinkInfo->scoCtl.isActiveConnected = TRUE;
	}
	pLinkInfo->scoCtl.scoState = SCO_STATE_CONNECTED;
	pLinkInfo->scoCtl.scoCodec = scoCodec;
}

void APP_SetSCODisconnected(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(!pLinkInfo)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Sco Disc Fail, Linkinfo NULL", 0);
		return;
	}

	pLinkInfo->scoCtl.scoSink = NULL;
	pLinkInfo->scoCtl.scoHandle = 0;
	pLinkInfo->scoCtl.scoState = SCO_STATE_DISC;
	pLinkInfo->scoCtl.isActiveConnected = FALSE;
	pLinkInfo->subState = APP_VDIAL_STOP;
}

BOOL APP_IsScoConnected(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo)
	{
		return (pLinkInfo->scoCtl.scoState >= SCO_STATE_CONNECTED);
	}
	return FALSE;
}

void APP_SetScoState(APP_INFO_STRU *pLinkInfo, U8 state)
{
	if(pLinkInfo)
	{
		pLinkInfo->scoCtl.scoState = state;

		if(state == SCO_STATE_ACTIVE_CONNECT)
		{
			pLinkInfo->scoCtl.isActiveConnected = TRUE;
		}
		else if(state == SCO_STATE_PASSIVE_CONNECT)
		{
			pLinkInfo->scoCtl.isActiveConnected = FALSE;
		}
	}
}

U8 APP_GetScoState(APP_INFO_STRU *pLinkInfo)
{
	if(pLinkInfo)
	{
		return pLinkInfo->scoCtl.scoState;
	}

	return SCO_STATE_DISC;
}

U8 APP_GetScoCodec(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo)
	{
		return pLinkInfo->scoCtl.scoCodec;
	}

	return SCO_CODEC_CVSD;
}

BOOL APP_IsScoActiveConnect(APP_INFO_STRU *pLinkInfo)
{
	if(pLinkInfo->scoCtl.isActiveConnected && (pLinkInfo->scoCtl.scoState == SCO_STATE_CONNECTED || pLinkInfo->scoCtl.scoState == SCO_STATE_ACTIVE_CONNECT))
	{
		return TRUE;
	}
	return FALSE;
}

/**************************************************************************************************
* Public Functions (Audio Device)
**************************************************************************************************/
U16 APP_GetAudioDevLinkMask(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo)
		return pLinkInfo->audioDevLinkMask;

	//FW_Assert(FALSE);
	return 0;
}

void APP_SetAudioDevLinkMask(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	APP_INFO_STRU *pLinkInfo;

	if(!(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END))
		return;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		pLinkInfo->audioDevLinkMask |= AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_LINK_START);
	}
}

void APP_ClearAudioDevLinkMask(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	APP_INFO_STRU *pLinkInfo;

	if(!(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END))
		return;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		pLinkInfo->audioDevLinkMask &= ~AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_LINK_START);
	}
}

void APP_ClearAllLinksAudioDevLinkMask(void)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		BD_ADDR_T * pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr)
		{
			if(APP_IsSetAppLink(pBdAddr))
			{
				APP_GetAppLinkByBdAddr(pBdAddr)->audioDevLinkMask = 0;
			}
		}
	}
}

void APP_SetSpeakerMuteMask(U32 mask)
{
	gAppCtl.speakerMuteMask |= mask;
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] (0x%x) set spk, new mask %d", 2, __builtin_return_address(0), gAppCtl.speakerMuteMask);
}

void APP_RmvSpeakerMuteMask(U32 mask)
{
	gAppCtl.speakerMuteMask &= ~mask;
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] (0x%x) rm spk, new mask %d", 2, __builtin_return_address(0),gAppCtl.speakerMuteMask);
}

U32 APP_GetSpeakerMuteMask(void)
{
	return gAppCtl.speakerMuteMask;
}

/**************************************************************************************************
* Public Functions (HFP)
**************************************************************************************************/
APP_HFP_CTL_STRU * APP_GetHfpLinkInfo(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;
	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		return &pLinkInfo->hfpCtl;
	}
	return (APP_HFP_CTL_STRU *)NULL;
}

BD_ADDR_T * APP_GetBdAddrByHfpLinkInfo(APP_HFP_CTL_STRU * pHfpLinkInfo)
{
	U8 linkIndex;

	if(!pHfpLinkInfo)
		return NULL;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(&gAppCtl.appInfo[linkIndex].hfpCtl == pHfpLinkInfo && !FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
			return &gAppCtl.appInfo[linkIndex].bdAddr;
		}
	}

	return NULL;
}

U8 APP_GetHfpLinkAudioTransfer(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;
	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		return pLinkInfo->hfpCtl.isAudioTransfer;
	}
	return 0;
}

void APP_SetHfpLinkAudioTransfer(BD_ADDR_T *pBdAddr, BOOL isAudioTransfer)
{
	APP_INFO_STRU *pLinkInfo;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		pLinkInfo->hfpCtl.isAudioTransfer = isAudioTransfer;
	}
}

U8 APP_GetHfpIsRejectByKey(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		return pLinkInfo->hfpCtl.isCallRejectedByKey;
	}
	return 0;
}

void APP_SetHfpIsRejectByKey(BD_ADDR_T *pBdAddr, BOOL isRejectByKey)
{
	APP_INFO_STRU *pLinkInfo;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		pLinkInfo->hfpCtl.isCallRejectedByKey = isRejectByKey;
	}
}

void APP_ClearHfpLinkInfo(BD_ADDR_T *pBdAddr)
{
	APP_HFP_CTL_STRU * pHfpLinkInfo;

	if((pHfpLinkInfo = APP_GetHfpLinkInfo(pBdAddr)))
	{
		APP_HFP_ReleaseAllTimer(pBdAddr);
		FW_Memset(pHfpLinkInfo,0, sizeof(APP_HFP_CTL_STRU));
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] APP_ClearHfpLinkInfo!!!!!!!", 0);
		if(FW_CmpBdAddr(APP_GetRedialBdaddr(), pBdAddr))
		{
			APP_SetRedialBdAddr(APP_HFP_SwitchLinkToNextHFPLink(pBdAddr));
		}
		if(FW_CmpBdAddr(APP_GetImgCallBdaddr(), pBdAddr))
		{
			APP_SetImgCallBdAddr(APP_HFP_SwitchLinkToNextHFPLink(pBdAddr));
		}
		//#ifdef _EXTENSION_ENTITIES_
		//gMMI_HFP_ctl.HfpCtl[linkIndex].DTMFPara = '0';
		//#endif
		//#ifdef _MEMORY_DIALING_
		//gMMI_HFP_ctl.HfpCtl[linkIndex].memoryDialIndex = 1; //[0-65535]
		//#endif
	}
}

/**************************************************************************************************
* Public Functions (AVRCP profile)
**************************************************************************************************/
APP_AVRCP_CTL_STRU * APP_GetAvrcpLinkInfo(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;
	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)))
	{
		return &pLinkInfo->avrcpCtl;
	}
	return (APP_AVRCP_CTL_STRU *)NULL;
}

void APP_ClearAvrcpLinkInfo(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pAvrcpLinkInfo;

	if((pAvrcpLinkInfo = APP_GetAvrcpLinkInfo(pBdAddr)))
	{
		APP_Avrcp_ReleaseAllTimer(pBdAddr);
		FW_Memset(pAvrcpLinkInfo, 0, sizeof(APP_AVRCP_CTL_STRU));
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] APP_ClearAvrcpLinkInfo!!!!!!!", 0);
	}
}

BD_ADDR_T * APP_GetBdAddrByAvrcpLinkInfo(APP_AVRCP_CTL_STRU * pAvrcpLinkInfo)
{
	U8 linkIndex;

	if(!pAvrcpLinkInfo)
		return NULL;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if(&gAppCtl.appInfo[linkIndex].avrcpCtl == pAvrcpLinkInfo && !FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
			return &gAppCtl.appInfo[linkIndex].bdAddr;
		}
	}

	return NULL;
}

/**************************************************************************************************
* Public Functions (Service Link)
**************************************************************************************************/
BOOL APP_IsServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	return FW_CmpBdAddr(&gAppCtl.serviceBdAddr, pBdAddr);
}

void APP_SetServiceBdAddr(BD_ADDR_T *pBdAddr)
{
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] APP SetServiceBdAddr bdAddr:0x%x%x", 2,FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	if(pBdAddr)
	{
		FW_Memcpy(&gAppCtl.serviceBdAddr, pBdAddr, sizeof(BD_ADDR_T));
	}
}

void APP_ClearServiceBdAddr(void)
{
	FW_Memset(&gAppCtl.serviceBdAddr, 0, sizeof(BD_ADDR_T));
}


BD_ADDR_T *APP_GetServiceBdAddr()
{
	if(!FW_IsBdAddrZero(&gAppCtl.serviceBdAddr))
		return &gAppCtl.serviceBdAddr;
	return NULL;
}

/**************************************************************************************************
* Public Functions (Multi Links)
**************************************************************************************************/
BD_ADDR_T *APP_GetNextNormalLink(BD_ADDR_T *pBdAddr)
{
#ifdef SUPPORT_MULTI_POINT
	U8 i, j, linkIndex;

	if(pBdAddr == APP_EOF)
	{
		return NULL;
	}

	linkIndex = APP_GetLinkIndexByBdAddr(pBdAddr);

	for(i = linkIndex + 1; i < (linkIndex + FW_MAX_LINK_IDX); i++)
	{
		j = i % FW_MAX_LINK_IDX;
		if(APP_IsActiveLink(APP_GetBdAddrByLinkIndex(j))/* && PRODUCT_AIR_NONE == gAppCtl.sdapCtl[j].remoteApplicationID*/)
		{
			return APP_GetBdAddrByLinkIndex(j);
		}
	}
#else
	UNUSED(pBdAddr);
#endif
	return APP_EOF;
}

void APP_SetRedialBdAddr(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr == NULL)
	{
		FW_Memset(&gAppCtl.redialBdAddr, 0, sizeof(BD_ADDR_T));
	}
	else if(FW_Memcmp(&gAppCtl.redialBdAddr, pBdAddr, sizeof(BD_ADDR_T)))
	{
		FW_Memcpy(&gAppCtl.redialBdAddr, pBdAddr, sizeof(BD_ADDR_T));
	}
}

BD_ADDR_T *APP_GetRedialBdaddr(void)
{
	if(!FW_IsBdAddrZero(&gAppCtl.redialBdAddr))
		return &gAppCtl.redialBdAddr;
	return NULL;
}

BOOL APP_CheckRedialBdaddrisZero()
{
	return FW_IsBdAddrZero(&gAppCtl.redialBdAddr);
}

void APP_SetImgCallBdAddr(BD_ADDR_T *pBdAddr)
{
	if(pBdAddr == NULL)
	{
		FW_Memset(&gAppCtl.imgCallBdAddr, 0, sizeof(BD_ADDR_T));
	}
	else if(FW_Memcmp(&gAppCtl.imgCallBdAddr, pBdAddr, sizeof(BD_ADDR_T)))
	{
		FW_Memcpy(&gAppCtl.imgCallBdAddr, pBdAddr, sizeof(BD_ADDR_T));
	}
}

BD_ADDR_T *APP_GetImgCallBdaddr(void)
{
	if(!FW_IsBdAddrZero(&gAppCtl.imgCallBdAddr))
		return &gAppCtl.imgCallBdAddr;
	return NULL;
}

APP_QOS_CTL * APP_SearchQosCtl(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo;

	if((pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr)) != (APP_INFO_STRU *)NULL)
		return &pLinkInfo->qosCtl;

	return (APP_QOS_CTL * )NULL;
}

/**************************************************************************************************
* Public Functions (AWS MCE)
**************************************************************************************************/
<<<<<<< HEAD
=======
U8 APP_GetNormalLinkNum()
{
	BD_ADDR_T *pBdAddr;
	U8 i, linkNum = 0;

	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if((pBdAddr = APP_GetBdAddrByLinkIndex(i)) && !FW_CmpBdAddr(pBdAddr, BtAwsMce_ReadLocalBdAddr()))
		{
			linkNum++;
		}
	}

	return linkNum;
}

BD_ADDR_T* APP_GetNormalLinkAddr()
{
	BD_ADDR_T *pBdAddr;
	U8 i, linkNum = 0;

	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if((pBdAddr = APP_GetBdAddrByLinkIndex(i)) && !FW_CmpBdAddr(pBdAddr, BtAwsMce_ReadLocalBdAddr()))
		{
			return pBdAddr;
		}
	}

	return NULL;
}

>>>>>>> db20e11 (second commit)
void APP_SetAgentState(U8 state)
{
	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
		gAppCtl.awsmceCtl.agentState = state;
}

U8 APP_GetAgentState(void)
{
	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
		return gAppCtl.awsmceCtl.agentState;

	return APP_OFF;
}

void APP_SetPartnerSmartChargeCaseState(U8 state)
{
	gAppCtl.awsmceCtl.partnerSmartChargeCaseState = state;

	if(!APP_SmtChgCse_IsConnToAGNeeded() && state != STATE_CASE_OFF)
		APP_Mcsync_SetNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	else
		APP_Mcsync_ClearNotReconnMask(MCS_RECONN_MASK_CHARGER_CASE);
	if(BtAwsMce_IsDefaultRoleAgent())
		ChargerSmartCase_SetPartnerCaseState(state);
}

U8 APP_GetPartnerSmartChargeCaseState(void)
{
	return gAppCtl.awsmceCtl.partnerSmartChargeCaseState;
}

void APP_SetAGNum(U8 AGNum)
{
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] set AG num:%d ->%d", 2, gAppCtl.awsmceCtl.numOfAG, AGNum);
	gAppCtl.awsmceCtl.numOfAG = AGNum;
}

U8 APP_GetAGNum(void)
{
	return gAppCtl.awsmceCtl.numOfAG;
}

void APP_SetAgentScoState(U8 agentScoState)
{
	gAppCtl.awsmceCtl.scoCtl.agentScoState = agentScoState;
}

U8 APP_GetAgentScoState(void)
{
	return gAppCtl.awsmceCtl.scoCtl.agentScoState;
}

void APP_SetAgentScoTopState(U8 agentScoTopState)
{
	gAppCtl.awsmceCtl.scoCtl.agentScoTopState = agentScoTopState;
}

U8 APP_GetAgentScoTopState(void)
{
	return gAppCtl.awsmceCtl.scoCtl.agentScoTopState;
}

void APP_AgentSavePartnerInfo(BD_ADDR_T *pBdAddr)
{
	FW_Memcpy(&gAppCtl.awsmceCtl.partnerBdAddr, pBdAddr, sizeof(BD_ADDR_T));
}

BD_ADDR_T *APP_AgentGetPartnerInfo(void)
{
	return &gAppCtl.awsmceCtl.partnerBdAddr;
}

/**************************************************************************************************
* Public Functions (RHO)
**************************************************************************************************/
BOOL APP_FillRhoData(void *pRhoStru)
{
	if(!pRhoStru)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP] Fill RHO memory error", 0);
		return FALSE;
	}

	if(!app_FillRhoDataHandler((APP_RHO_DATA_STRU *)pRhoStru))
	{
		DBG_LOG_APP_RSRC_MNG( "[APP] Fill RHO data error", 0);
		return FALSE;
	}

	FW_Memcpy(&((APP_RHO_DATA_STRU *)pRhoStru)->testBd, &gAppCtl.serviceBdAddr, 6); //test

	DBG_LOG_APP_RSRC_MNG( "[APP] Fill RHO data OK. bdAddr:0x%x%x", 2,
		FW_bdaddr_to_2U32(&((APP_RHO_DATA_STRU *)pRhoStru)->testBd, TRUE), FW_bdaddr_to_2U32(&((APP_RHO_DATA_STRU *)pRhoStru)->testBd, FALSE));

	return TRUE;
}

BOOL APP_AssignRhoData(void *pRhoStru)
{
	if(!pRhoStru)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP] Assign RHO memory error", 0);
		return FALSE;
	}

	if(!app_AssignRhoDataHandler((APP_RHO_DATA_STRU *)pRhoStru))
	{
		DBG_LOG_APP_RSRC_MNG( "[APP] Assign RHO data error", 0);
		return FALSE;
	}

	DBG_LOG_APP_RSRC_MNG( "[APP] Assign RHO data OK. bdAddr:0x%x%x", 2,
		FW_bdaddr_to_2U32(&((APP_RHO_DATA_STRU *)pRhoStru)->testBd, TRUE), FW_bdaddr_to_2U32(&((APP_RHO_DATA_STRU *)pRhoStru)->testBd, FALSE));

	return TRUE;
}

BOOL APP_IsAllowHandler(void)
{
	U8 totalMsgNum = 0, msgNum, appRhoNumIndex;
	BOOL isAllow = FALSE;

	for(appRhoNumIndex = 0; appRhoNumIndex < APP_RHO_MODULE_NUM; appRhoNumIndex++)
	{
		if(gAppRhoHandler[appRhoNumIndex].app_GetMsgNum_Handler && (msgNum = gAppRhoHandler[appRhoNumIndex].app_GetMsgNum_Handler()))
		{
			totalMsgNum += msgNum;
			DBG_LOG_APP_SYSTEM( "[APP] %s RHO Not Allow Msg Number:%d", 2, APP_RHO_MODULE_String[appRhoNumIndex], msgNum);
		}
	}

	if(!totalMsgNum)
		isAllow = TRUE;

	#ifdef PROFILE_AMA_ENABLE
	if(isAllow && AMA_SPEECH_STATE_LISTENING == AMA_Target_GetSpeechState(APP_GetServiceBdAddr()))
	{
		APP_AMA_Key_SpeechStop(APP_GetServiceBdAddr());
		return FALSE;
	}
	#endif

	return isAllow;
}

void APP_SetNewPartnerConfig(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pOldAgentAddr);
	SINK tmpSink;
	U8 tmpAclState, linkIndex, tmpScoCodec;
	U16 tmpScoHandle;
	APP_STATE_STRU *pTmpNestState;
    
	for(U8 appRhoNumIndex = 0; appRhoNumIndex < APP_RHO_MODULE_NUM; appRhoNumIndex++)
	{
		if(gAppRhoHandler[appRhoNumIndex].app_NewPartner_Hanlder)
		{
            gAppRhoHandler[appRhoNumIndex].app_NewPartner_Hanlder(pOldAgentAddr, pNewAgentAddr);
		}
	}

	if(!pLinkInfo)
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_LINKINFO] normal NULL no link", 0);
		return;
	}

	tmpSink = pLinkInfo->scoCtl.scoSink;
	tmpScoHandle = pLinkInfo->scoCtl.scoHandle;
	tmpScoCodec = pLinkInfo->scoCtl.scoCodec;
	tmpAclState = pLinkInfo->aclState;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pTmpNestState = gAppCtl.appInfo[linkIndex].pNestState;
		FW_Memset(&gAppCtl.appInfo[linkIndex], 0, sizeof(APP_INFO_STRU));
		gAppCtl.appInfo[linkIndex].pNestState = pTmpNestState;
	}

	APP_SetServiceBdAddr(pNewAgentAddr);
	FW_Memcpy(&pLinkInfo->bdAddr, pNewAgentAddr, sizeof(BD_ADDR_T));
	pLinkInfo->scoCtl.scoSink = tmpSink;
	pLinkInfo->scoCtl.scoHandle = tmpScoHandle;
	pLinkInfo->scoCtl.scoCodec = tmpScoCodec;
	pLinkInfo->aclState = tmpAclState;
	
	gAppCtl.speakerMuteMask = SPKR_MUTE_MASK_PARTNER_USE;

	DBG_LOG_APP_SYSTEM( "[APP_LINKINFO] New Partner Sco Sink:0x%d, Bda:0x%x%x", 3,
		(U32)pLinkInfo->scoCtl.scoSink, FW_bdaddr_to_2U32(APP_GetBdAddrBySCOSink(pLinkInfo->scoCtl.scoSink), TRUE), FW_bdaddr_to_2U32(APP_GetBdAddrBySCOSink(pLinkInfo->scoCtl.scoSink), FALSE));
}

void APP_SetEsdInfo(BOOL isAudioA2dpOn, BD_ADDR_T *pBdAddr)
{
	DBG_LOG_APP_RSRC_MNG( "[APP_RM] ESD: %d %d", 2, isAudioA2dpOn,
		gAppEsd.isAudioA2dpOn);

    gAppEsd.isAudioA2dpOn = isAudioA2dpOn;
    FW_Memcpy(&gAppEsd.audioA2dpBdAddr, pBdAddr, sizeof(BD_ADDR_T));
}

/**************************************************************************************************
* Internal Public Functions (APP Command / Event Message)
**************************************************************************************************/
APP_MSG_TYPE_STRU *APP_GetAppCmdMsgMemory(U16 opCode)
{
	APP_MSG_TYPE_STRU *pMsg;

	pMsg = (APP_MSG_TYPE_STRU *)FW_GetMemory(sizeof(APP_MSG_TYPE_STRU));

	if (pMsg != (APP_MSG_TYPE_STRU *)NULL)
	{
		FW_Memset((U8 *)pMsg, 0, sizeof(APP_MSG_TYPE_STRU));

		pMsg->msgOpcode = opCode;
	}

	return pMsg;
}


