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

log_create_module(APP_RSRC_MNG, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Variables
**************************************************************************************************/
APP_CTL_STRU gAppCtl;
AT_ESD APP_ESD_STRU gAppEsd;

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
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Already", 0);
		if(!APP_System_IsMpTestMode() && !APP_FCD_IsFcdReset())
			APP_LinkKey_UpdateHistory(pBdAddr, TRUE);
		return TRUE;
	}

	if ((pLinkInfo = APP_GetEmptyAppLink()))
	{
		FW_Memcpy(&pLinkInfo->bdAddr, pBdAddr, sizeof(BD_ADDR_T));
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr OK", 0);
		return TRUE;
	}
	else
	{
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Set Link Addr Fail (Full)", 0);
		return FALSE;
	}
}

void APP_ClearAppLinkBdAddr(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo && pLinkInfo->aclState == ACL_STATE_OFF && !APP_State_CheckLinkConnected(pBdAddr))
	{
		APP_STATE_STRU *pTmpNestState;
		APP_RemoveServiceBdAddr(pBdAddr);
		APP_State_SendMsgToHandler(pBdAddr, APP_LINK_DETACHED_EVT, APP_LINK_DETACHED_EVT, TRUE);

		pTmpNestState = pLinkInfo->pNestState;
		FW_Memset(pLinkInfo, 0, sizeof(APP_INFO_STRU));
		pLinkInfo->pNestState = pTmpNestState;
		DBG_LOG_APP_RSRC_MNG( "[APP_RM] Clear Link Addr", 0);
	}
}

APP_INFO_STRU *APP_GetEmptyAppLink()
{
	U8 linkIndex;

	for (linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		if (FW_IsBdAddrZero(&gAppCtl.appInfo[linkIndex].bdAddr))
		{
			return &gAppCtl.appInfo[linkIndex];
		}
	}

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
			return (pLinkInfo->linkPara.miscMask & APP_ACTIVE_DISCONNECT)? TRUE : FALSE;
		}
	}
	return FALSE;
}

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

void APP_SetEsdInfo(BOOL isAudioA2dpOn, BD_ADDR_T *pBdAddr)
{
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


