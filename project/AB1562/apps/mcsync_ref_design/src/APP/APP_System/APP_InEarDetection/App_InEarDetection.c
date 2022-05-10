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
#include "BtMCSync.h"
#include "App_InEarDetection.h"
#include "InEarDetection_common.h"
#include "InEarDetection.h"
#include "App_MCSync_Message.h"
#include "App_Avrcp.h"
#include "Avrcp_Interface.h"
#include "App_ResourceManager.h"
#include "App_MCSync_RHO.h"
#include "InEarDetection_NvkeyHandler.h"
#include "chargercase_common.h"
#include "DrvSmartCase.h"
#include "InEarDetection_DrvHandler.h"
#include "App_State.h"
#include "App_HFPKey.h"
#include "App_ChargeBattery.h"
#include "App_A2dp.h"
#include "App_RhoHandler.h"
#include "App_Hfp.h"
#include "APP_HfpScoHandler.h"
#include "App_System.h"
#include "App_Customer_Func.h"


/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_InEarDetection_Handler(Handler handler, U16 id, void *msg, U32 ext_id);

/**************************************************************************************************
* Variable
**************************************************************************************************/
static HandlerData gAppInEarDetectionHandle = { app_InEarDetection_Handler };
static APP_IN_EAR_DETECTION_CTL_STRU gAppInEarDetectionCtl;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U32 app_InEarDetection_Handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(id);
	UNUSED(msg);

	switch(id)
	{
		case APP_IN_EAR_DETECTION_DEBOUNCE_TIMER:
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] In APP_IN_EAR_DETECTION_DEBOUNCE_TIMER case, id:%d, in_ext:%d, role:%d", 3, id, ext_id, BtAwsMce_GetDefaultRole());
        /* fallthrough */
        case APP_IN_EAR_DETECTION_CHECK_STATUS:
			if(BtAwsMce_GetDefaultRole() == ROLE_AGENT)//check status, trigger MMI
			{
				if(!InEarDetection_IsDuringDelay())
					APP_InEarDetection_TriggerAction(APP_InEarDetection_GetStatus(), APP_InEarDetection_GetPartnerStatus());
			}
			break;
		case APP_IN_EAR_DETECTION_STATUS_IND:
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] In APP_IN_EAR_DETECTION_STATUS_IND case, id:%d, in_ext:%d, role:%d", 3, id, ext_id, BtAwsMce_GetDefaultRole());
			switch(ext_id)
			{
				case IN_EAR_DETECTION_OUT:
				/* fallthrough */
				case IN_EAR_DETECTION_IN:
					if(BtAwsMce_GetDefaultRole() == ROLE_AGENT)//check status, trigger MMI
					{
						if(!InEarDetection_IsDuringDelay())
							APP_InEarDetection_TriggerAction(APP_InEarDetection_GetStatus(), APP_InEarDetection_GetPartnerStatus());
					}
					else if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)//send status to agent
					{
						if(!APP_MCSync_Rho_IsRhoing())
							APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(ext_id);
					}
					break;
				default:
					break;
			}
			break;

		case APP_IN_EAR_DETECTION_RHO_IND:
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] In APP_IN_EAR_DETECTION_RHO_IND case, id:%d, in_ext:%d, role:%d", 3, id, ext_id, BtAwsMce_GetDefaultRole());
			APP_MCSync_Rho_Start();
			break;
		case APP_IN_EAR_DETECTION_DIS_SCO_IND:
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] In APP_IN_EAR_DETECTION_DIS_SCO_IND case, id:%d, in_ext:%d, role:%d", 3, id, ext_id, BtAwsMce_GetDefaultRole());
			APP_HfpSco_DisconnectRequest(APP_GetServiceBdAddr());
			break;
		default:
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] In default case, id:%d, in_ext:%d, role:%d", 3, id, ext_id, BtAwsMce_GetDefaultRole());
			break;
	}
	return 0;
}

static void app_InEarDetection_SingleEarInHandler(void)
{
	DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Single Ear In handler, last status:%d, feature:%d, mcsync state:%d", 3, APP_InEarDetection_GetLastTwoEarStatus(), APP_InEarDetection_IsFeatureOn(APP_IN_EAR_DETECTION_AIRPODS_A2DP_MMI_FEAT), MCSYNC_GetMcsyncState());

	/*if(APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE || (APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE_WITHOUT_SCO))
	{
		DBG_LOG_APP_SYSTEM("[APP InEarDetection] AudioTransfer - SingleEarInHandler", 0);

		if (!APP_SCO_EXISTS(APP_GetServiceBdAddr()))
		{
			DBG_LOG_APP_SYSTEM("[APP InEarDetection] !APP_SCO_EXISTS, CreateAudioConnection", 0);
			APP_HFP_CreateAudioConnection(APP_GetServiceBdAddr(), TRUE);
		}

		return;
	}*/
	switch(APP_InEarDetection_GetLastTwoEarStatus())
	{
		case TWO_EAR_IN:
			APP_InEarDetection_MusicPauseCtl();
			break;

		case TWO_EAR_OUT:
			if(MCSYNC_GetMcsyncState() == MCSYNC_LINK_CONNECTED)
			{
				//do nothing
			}
			else
			{
				APP_InEarDetection_MusicPlayCtl();
			}
			break;
			
		default:
			/* BTA-4827 */
			APP_InEarDetection_MusicPauseCtl();
			break;
	}
}

static void app_InEarDetection_SingleAgentInHandler(void)
{
    DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Single Agent In handler, state:%d", 1, APP_State_GetTopState(APP_GetServiceBdAddr()));
    if(APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE || (APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE_WITHOUT_SCO))
    {
        if(APP_MCSync_Rho_IsRhoing())
        {
            APP_MCSync_Rho_Stop();
            FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
            return;
        }
        switch(APP_InEarDetection_GetLastTwoEarStatus())
        {
            case TWO_EAR_OUT:
            case SINGLE_PARTNER_IN:
            case TWO_EAR_IN:
                FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
                break;
            case SINGLE_AGENT_IN:
                APP_HFP_CreateAudioConnection(APP_GetServiceBdAddr(), TRUE);
                break;
            default:
                break;
        }
        return;
    }
    app_InEarDetection_SingleEarInHandler();
}

static void app_InEarDetection_SinglePartnerInHandler()
{
    DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Single Partner In handler, state:%d", 1, APP_State_GetTopState(APP_GetServiceBdAddr()));
    if(APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE || (APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE_WITHOUT_SCO))
    {
        if(APP_MCSync_Rho_IsRhoing())
        {
            FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
            return;
        }
        switch(APP_InEarDetection_GetLastTwoEarStatus())
        {
            case TWO_EAR_OUT:
            case TWO_EAR_IN:
            case SINGLE_AGENT_IN:
                FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
                break;
            case SINGLE_PARTNER_IN:
                if(!APP_MCSync_Rho_IsRhoing())
                {
                    FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_RHO_IND, NULL, 0, 0);
                }
                break;
            default:
                break;
        }
        return;
    }
    else
    {
        if(APP_InEarDetection_GetLastTwoEarStatus() == SINGLE_PARTNER_IN)
            return;
        app_InEarDetection_SingleEarInHandler();
    }
}

static void app_InEarDetection_TwoEarInHandler()
{
    DBG_LOG_APP_SYSTEM("[APP_InEarDect] TwoEarIn handler", 0);
    //APP_InEarDetection_CancelRHOTimer();
    if((APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE) || (APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE_WITHOUT_SCO))
    {
        if(APP_MCSync_Rho_IsRhoing())
        {
            APP_MCSync_Rho_Stop();
            FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
            return;
        }
        switch(APP_InEarDetection_GetLastTwoEarStatus())
        {
            case TWO_EAR_OUT:
            case SINGLE_PARTNER_IN:
            case SINGLE_AGENT_IN:
                FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
                break;
            case TWO_EAR_IN:
                APP_HFP_CreateAudioConnection(APP_GetServiceBdAddr(), TRUE);
                break;
            default:
                break;
        }
        return;
    }
    APP_InEarDetection_MusicPlayCtl();
}

static void app_InEarDetection_TwoEarOutHandler()
{
    DBG_LOG_APP_SYSTEM("[APP_InEarDect] TwoEarOutHandler", 0);
    if((APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE) || (APP_State_GetTopState(APP_GetServiceBdAddr()) == APP_HFP_CALLACTIVE_WITHOUT_SCO))
    {
        if(APP_MCSync_Rho_IsRhoing())
        {
            APP_MCSync_Rho_Stop();
            FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
            return;
        }
        switch (APP_InEarDetection_GetLastTwoEarStatus())
        {
            case TWO_EAR_OUT:
                if (APP_SCO_EXISTS(APP_GetServiceBdAddr()))
                {
                    DBG_LOG_APP_SYSTEM("[APP_InEarDect] AudioTransfer, DisconnectRequest;", 0);
                    FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_DIS_SCO_IND, NULL, 0, 0);
                }
                break;
            case TWO_EAR_IN:
            case SINGLE_PARTNER_IN:
            case SINGLE_AGENT_IN:
                FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_CHECK_STATUS, NULL, 0, 0);
                break;
            default:
                break;
        }
        return;
    }
    APP_InEarDetection_MusicPauseCtl();
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/

U8 APP_InEarDetection_GetStatus()
{
	return InEarDetection_GetStatus();
}

U8 APP_InEarDetection_GetPartnerStatus()
{
	return (BtAwsMce_GetDefaultRole() == ROLE_AGENT) ? gAppInEarDetectionCtl.partnerStatus : APP_InEarDetection_GetStatus();
}

void APP_InEarDetection_SetPartnerStatus(U8 status)
{
	if(BtAwsMce_GetDefaultRole() == ROLE_AGENT)
		gAppInEarDetectionCtl.partnerStatus = status;
}

U8 APP_InEarDetection_GetLastTwoEarStatus()
{
	return (BtAwsMce_GetDefaultRole() == ROLE_AGENT) ? gAppInEarDetectionCtl.lastTwoEarStatus : TWO_EAR_OUT;
}

void APP_InEarDetection_SetLastTwoEarStatus(U8 status)
{
	APP_Customer_SetInEarTone(status);
	if(BtAwsMce_GetDefaultRole() == ROLE_AGENT)
	{
		gAppInEarDetectionCtl.lastTwoEarStatus = status;
	}
}

void APP_InEarDetection_Init(void)
{
	InEarDetection_Init(&gAppInEarDetectionHandle);
}

void APP_InEarDetection_TriggerAction(U8 agentStatus, U8 partnerStatus)
{
    if(!APP_Customer_IsInEarDetectionEnable())
        return;

    if(!APP_InEarDetection_IsEnable())
    {
        DBG_LOG_APP_SYSTEM( "[APP_InEarDect] APP In ear detection feature is off", 0);
        APP_InEarDetection_SetLastTwoEarStatus((partnerStatus << 1)|agentStatus);
        return;
    }

	if(APP_ChgCase_IsCaseSmartType())
	{
		DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Check1 status, charger case sate:%d, partner case state:%d", 2, DrvCharger_GetSmartCaseState(), APP_GetPartnerSmartChargeCaseState());

		if(DrvCharger_GetSmartCaseState() != STATE_OUT_OF_CASE)
			agentStatus = IN_EAR_DETECTION_OUT;

		if(APP_GetPartnerSmartChargeCaseState() != STATE_OUT_OF_CASE)
			partnerStatus = IN_EAR_DETECTION_OUT;
	}

	if(MCSYNC_GetMcsyncState() != MCSYNC_LINK_CONNECTED)
		partnerStatus = IN_EAR_DETECTION_OUT;

	DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Check2 status, agent status:%d, partner status:%d", 2, agentStatus, partnerStatus);

	switch((partnerStatus << 1)|agentStatus)
	{
		case TWO_EAR_OUT:
			app_InEarDetection_TwoEarOutHandler();
			break;
		case SINGLE_AGENT_IN:
			app_InEarDetection_SingleAgentInHandler();
			break;
		case SINGLE_PARTNER_IN:
			app_InEarDetection_SinglePartnerInHandler();
			break;
		case TWO_EAR_IN:
			app_InEarDetection_TwoEarInHandler();
			break;
		default:
            return;
	}
	APP_InEarDetection_SetLastTwoEarStatus((partnerStatus << 1)|agentStatus);
/*
	if((agentStatus == IN_EAR_DETECTION_OUT) && (partnerStatus == IN_EAR_DETECTION_OUT))//both out
	{
		app_InEarDetection_TwoEarOutHandler();
		APP_InEarDetection_SetLastTwoEarStatus(TWO_EAR_OUT);
	}
	else if((agentStatus == IN_EAR_DETECTION_IN) && (partnerStatus == IN_EAR_DETECTION_OUT))//agent in, partner out
	{
		app_InEarDetection_SingleAgentInHandler();
		APP_InEarDetection_SetLastTwoEarStatus(SINGLE_AGENT_IN);
	}
	else if((agentStatus == IN_EAR_DETECTION_OUT) && (partnerStatus == IN_EAR_DETECTION_IN))//agent out, partner in
	{
		app_InEarDetection_SinglePartnerInHandler();
		APP_InEarDetection_SetLastTwoEarStatus(SINGLE_PARTNER_IN);
	}
	else if((agentStatus == IN_EAR_DETECTION_IN) && (partnerStatus == IN_EAR_DETECTION_IN))//both in
	{
		app_InEarDetection_TwoEarInHandler();
		APP_InEarDetection_SetLastTwoEarStatus(TWO_EAR_IN);
	}

	return;
*/
}

BOOL APP_InEarDetection_IsFeatureOn(U16 feature)
{
	return (InEarDetection_NvKey_GetAppFeature() & feature) ? TRUE : FALSE;
}

BOOL APP_InEarDetection_GetAppFeature()
{
    return (InEarDetection_NvKey_GetAppFeature()) ? TRUE : FALSE;
}

void APP_InEarDetection_NvKeyInit()
{
	InEarDetection_NvKeyInit();
}

void APP_InEarDetection_SetPauseFlag(BOOL isPause)
{
	if(!APP_InEarDetection_IsFeatureOn(APP_IN_EAR_DETECTION_AIRPODS_A2DP_MMI_FEAT))
		return;

    DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Set Pause Flag, is Paused:%d", 1, isPause);
	gAppInEarDetectionCtl.pauseFlag = isPause;
}

BOOL APP_InEarDetection_GetPauseFlag(void)
{
	return gAppInEarDetectionCtl.pauseFlag;
}

BOOL APP_InEarDetection_IsA2DPStreaming(void)
{
	DBG_LOG_APP_SYSTEM( "[APP_InEarDect] Is A2DP Streaming, monitor:%d, avrcp play:%d, a2dp play:%d", 3, APP_Avrcp_isDisablePlayStatusMonitor(APP_GetServiceBdAddr()), APP_AvrcpIsMusicPlaying(APP_GetServiceBdAddr()), APP_a2dp_is_in_streaming(APP_GetServiceBdAddr()));

	if(APP_Avrcp_isDisablePlayStatusMonitor(APP_GetServiceBdAddr()))
	{
		if(APP_AvrcpIsMusicPlaying(APP_GetServiceBdAddr()))
		{
			return TRUE;
		}
	}
	else
	{
		if(APP_a2dp_is_in_streaming(APP_GetServiceBdAddr()))
		{
				return TRUE;
		}
	}
	return FALSE;
}

void APP_InEarDetection_MusicPauseCtl(void)
{
	if(APP_InEarDetection_IsA2DPStreaming())
	{
		APP_InEarDetection_SetPauseFlag(TRUE);
		AVRCP_SendOperation(APP_GetServiceBdAddr(), AVC_OPERATION_PAUSE, 0);
	}
}

void APP_InEarDetection_MusicPlayCtl(void)
{
    if(!APP_InEarDetection_IsFeatureOn(APP_IN_EAR_DETECTION_AIRPODS_A2DP_MMI_FEAT))
        return;

	if(!APP_InEarDetection_IsA2DPStreaming())
	{
		if(APP_InEarDetection_GetPauseFlag())
		{
			AVRCP_SendOperation(APP_GetServiceBdAddr(), AVC_OPERATION_PLAY, 0);
			APP_InEarDetection_SetPauseFlag(FALSE);
		}
	}
}

BOOL APP_InEarDetection_IsEnable(void)
{
	return (InEarDetection_NvKey_GetSensorType()) ? TRUE : FALSE;
}

void APP_InEarDetection_SetFeature(BOOL isOn)
{
    InEarDetection_NvKey_SetFeatureEnable(isOn);
}

void APP_InEarDetection_CancelRHOTimer(void)
{
	if(MSG_MessageSearchAllEx3(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_RHO_IND))
		FW_ReleaseTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_RHO_IND, 0);
}

U8 APP_InEarDetection_DecideTwoEarStatus(U8 agentStatus, U8 partnerStatus)
{
	if(APP_ChgCase_IsCaseSmartType())
	{
		if(DrvCharger_GetSmartCaseState() != STATE_OUT_OF_CASE)
			agentStatus = IN_EAR_DETECTION_OUT;

		if(APP_GetPartnerSmartChargeCaseState() != STATE_OUT_OF_CASE)
			partnerStatus = IN_EAR_DETECTION_OUT;
	}

	if((agentStatus == IN_EAR_DETECTION_OUT) && (partnerStatus == IN_EAR_DETECTION_OUT))//both out
	{
		return TWO_EAR_OUT;
	}
	else if((agentStatus == IN_EAR_DETECTION_IN) && (partnerStatus == IN_EAR_DETECTION_OUT))//agent in, partner out
	{
		return SINGLE_AGENT_IN;
	}
	else if((agentStatus == IN_EAR_DETECTION_OUT) && (partnerStatus == IN_EAR_DETECTION_IN))//agent out, partner in
	{
		return SINGLE_PARTNER_IN;
	}
	else if((agentStatus == IN_EAR_DETECTION_IN) && (partnerStatus == IN_EAR_DETECTION_IN))//both in
	{
		return TWO_EAR_IN;
	}
	else
	{
		return TWO_EAR_OUT;
	}
}

void APP_InEarDetection_SetDebounceTimer(void)
{
	FW_ReleaseTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_DEBOUNCE_TIMER, 0);
	FW_SetTimer(&gAppInEarDetectionHandle, APP_IN_EAR_DETECTION_DEBOUNCE_TIMER, NULL, 0, 0/*InEarDetection_NvKey_GetDebounceTime()*/);
}

/* RHO */
BOOL APP_InEarDetection_FillRhoData(void *pData)
{
	if(APP_InEarDetection_IsEnable())
	{
		APP_RHO_INEARDETECTION_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appInEarDection;

		if(!pDataToFill)
		{
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] Old Agent Fill RHO data error", 0);
			return FALSE;
		}
		pDataToFill->pauseFlag = gAppInEarDetectionCtl.pauseFlag;

		pDataToFill->isEnable = (APP_Customer_IsInEarDetectionEnable()) ? TRUE : FALSE;

        DBG_LOG_APP_SYSTEM("[APP_InEarDect] Old Agent Fill RHO data OK", 0);
	}
	return TRUE;
}

BOOL APP_InEarDetection_AssignRhoData(void *pData)
{
	if(APP_InEarDetection_IsEnable())
	{
		APP_RHO_INEARDETECTION_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appInEarDection;

		if(!pDataToGet)
		{
            DBG_LOG_APP_SYSTEM("[APP_InEarDect] New Agent Assign RHO data error", 0);
			return FALSE;
		}

		gAppInEarDetectionCtl.pauseFlag = pDataToGet->pauseFlag;
        gAppInEarDetectionCtl.lastTwoEarStatus = APP_InEarDetection_DecideTwoEarStatus(APP_InEarDetection_GetStatus(), APP_InEarDetection_GetPartnerStatus());

        APP_Customer_SetInEarEnableFlag(pDataToGet->isEnable);

        DBG_LOG_APP_SYSTEM("[APP_InEarDect] New Agent Assign RHO data OK:%d", 1, gAppInEarDetectionCtl.lastTwoEarStatus);
    }
    return TRUE;
}
