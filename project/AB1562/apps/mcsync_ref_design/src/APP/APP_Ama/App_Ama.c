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

#ifdef PROFILE_AMA_ENABLE

#include "App_Ama.h"
#include "BtAma.h"
#include "App_RaceCmd_Ama.h"
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "A2dp_Interface.h"
#include "BtHfp.h"
#include "App_A2dp.h"
#include "Avrcp_Interface.h"
#include "Ama_NvkeyHandler.h"
#include "App_Avrcp.h"
#include "App_PowerOff.h"
#include "FW_Api.h"

#include "App_RhoHandler.h"
#include "App_MCSync_RHO.h"
#include "App_Mcsync_LinkKey.h"

#include "App_Ble.h"

//#define AMA_UT

log_create_module(APP_AMA, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Enum
**************************************************************************************************/
#ifdef AMA_UT
typedef enum
{
	APP_AMA_TIMER_ID_NONE,
	APP_AMA_TIMER_ID_DISCONNECT,
	APP_AMA_TIMER_ID_GET_CENTRAL_INFORMATION,
	APP_AMA_TIMER_ID_OVERRIDE_ASSISTANT,
	APP_AMA_TIMER_ID_FACTORY_RESET,
    APP_AMA_TIMER_ID_SET_DEVICE_CONFIG,
} APP_AMA_TIMER_ID_E;
#endif

/**************************************************************************************************
* Structure
**************************************************************************************************/

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_ama_handler(Handler handler, U16 id, void *msg, U32 handler_id);
#ifdef AMA_UT
static U32 app_ama_timer_handler(Handler handler, U16 id, void *msg, U32 handler_id);
#endif
static BOOL app_ama_is_a2dp_connected(BD_ADDR_T *pBdAddr);
static BOOL app_ama_is_hfp_connected(BD_ADDR_T *pBdAddr);
static void app_ama_config_speech_para(AMA_PROVIDE_SPEECH_T *pSpeechParam);

static BOOL app_AmaGattServerReady(void);
static BOOL app_AmaEnable(void);
static BOOL app_AmaDisable(void);

/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static const HandlerData gAppAmaHandle = { app_ama_handler };

#ifdef AMA_UT
static const HandlerData gAppAmaTimerHandle = { app_ama_timer_handler };
#endif

APP_AMA_CTL_STRU gAppAmaCtl[FW_MAX_LINK_IDX];

static const APP_BLE_ADV_HANDLE_DESC gAppAmaDesc =
{
    BLE_ADV_AMA_MASK,
    app_AmaGattServerReady,
    app_AmaEnable,
    app_AmaDisable,    
};

static APP_BLE_ADV_HANDLE_NODE gAppAmaNode = {
    &gAppAmaDesc,
    NULL,
};

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_AmaGattServerReady(void)
{
    return app_AmaEnable();
}

static BOOL app_AmaEnable(void)
{
    if(APP_Ama_IsAmaConnected())
    {
        DBG_LOG_APP_AMA( "[APP_Ama] AMA is Connected", 0);
        return FALSE;
    }
    
    if (AMA_STATUS_OK != AMA_Target_EnableAdv())
    {
        DBG_LOG_APP_AMA( "[APP_Ama] AMA_Target_EnableAdv fail!", 0);
        return FALSE;
    }

    return TRUE;
}

static BOOL app_AmaDisable(void)
{
    if (AMA_STATUS_OK != AMA_Target_DisableAdv())
    {
        DBG_LOG_APP_AMA( "[APP_Ama] AMA_Target_DisableAdv fail!", 0);
        return FALSE;
    }

    return TRUE;
}

#ifdef AMA_UT
static U32 app_ama_timer_handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	UNUSED(msg);

	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();

	switch(id)
	{
		case APP_AMA_TIMER_ID_DISCONNECT:
			if(pAmaLinkInfo)
			{
				if (AMA_STATUS_OK != AMA_Target_DisconnectRequest(&pAmaLinkInfo->bdAddr))
				{
					DBG_LOG_APP_AMA("[APP_Ama] AMA_DisconnectRequest fail !", 0);
				}
			}
			break;

		case APP_AMA_TIMER_ID_GET_CENTRAL_INFORMATION:
			if(pAmaLinkInfo)
			{
				if (AMA_STATUS_OK != AMA_Target_GetCentralInformation(&pAmaLinkInfo->bdAddr))
				{
					DBG_LOG_APP_AMA("[APP_Ama] AMA_GetCentralInformation fail !", 0);
				}
			}
			break;

		case APP_AMA_TIMER_ID_OVERRIDE_ASSISTANT:
			if(pAmaLinkInfo)
			{
				if (AMA_STATUS_OK != AMA_Target_OverrideAssistant(&pAmaLinkInfo->bdAddr))
				{
					DBG_LOG_APP_AMA("[APP_Ama] AMA_OverrideAssistant fail !", 0);
				}
			}
			break;

		case APP_AMA_TIMER_ID_FACTORY_RESET:
			AMA_Target_FactoryReset();
			break;

        case APP_AMA_TIMER_ID_SET_DEVICE_CONFIG:
			if(pAmaLinkInfo)
			{
	            if (AMA_STATUS_OK != AMA_Target_SetDeviceConfiguration(&pAmaLinkInfo->bdAddr, TRUE, TRUE))
	            {
	                DBG_LOG_APP_AMA("[APP_Ama] AMA_SetDeviceConfiguration fail !", 0);
	            }
			}
            break;

        default:
            break;
	}
	return 0;
}
#endif

static void app_ama_start_service_cfm_handler(AMA_START_SERVICE_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_START_SERVICE_CFM status:%d", 1, cfm->status);
}

static void app_ama_stop_service_cfm_handler(AMA_STOP_SERVICE_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_STOP_SERVICE_CFM status:%d", 1, cfm->status);

#ifdef AMA_UT
	AMA_Target_StartService((Handler)&gAppAmaHandle, FW_MAX_LINK_IDX);
#endif
}

static void app_ama_enable_adv_cfm_handler(AMA_ENABLE_ADV_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_ENABLE_ADV_CFM status:%d", 1, cfm->status);
}

static void app_ama_disable_adv_cfm_handler(AMA_DISABLE_ADV_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_DISABLE_ADV_CFM status:%d", 1, cfm->status);
}

static void app_ama_connect_cfm_handler(AMA_CONNECT_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_CONNECT_CFM status:%d, BdAddr:0x%x%x", 3, cfm->status, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
	if(cfm->status == BT_STATUS_SUCCESS)
	{
		U8 status = AMA_CONNECTION_STATUS_CONNECTED;
		APP_RaceCmd_AMANotifyToAirapp(AMA_NOTIFY_ID_CONNECTION_STATUS, &status, sizeof(U8));

		APP_Ama_SetLinkInfo(&cfm->bdAddr, APP_AMA_STATE_CONNECTED);
        APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
        #ifdef AMA_UT
		AMA_STATE_T state;
		state.feature = AMA_STATE_FEATURE_A2DP_CONNECTED;
        state.valueType = AMA_VALUE_TYPE_BOOLEAN;
        state.value.boolean = 1;
        AMA_Target_SynchronizeState(&cfm->bdAddr, &state);
        state.feature = AMA_STATE_FEATURE_HFP_CONNECTED;
        AMA_Target_SynchronizeState(&cfm->bdAddr, &state);
		FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_GET_CENTRAL_INFORMATION, 0, 0, 10000);
        #endif
	}
}

static void app_ama_disconnect_handler(BD_ADDR_T * p_bd_addr)
{
    AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	APP_Ama_ClearLinkInfo(p_bd_addr);

    //APP_AudioDeviceDeRegister(p_bd_addr, AUDIO_DEVICE_AMA);
    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
    FW_Memcpy(&audioDspPara.bdAddr, p_bd_addr, sizeof(BD_ADDR_T));
    audioDspPara.audioType = AUDIO_AMA;
    APP_AudioDspStop(&audioDspPara);

    if(!APP_MCSync_Rho_IsRhoing())
    {
        APP_Ble_EnableAdv(BLE_ADV_AMA_MASK);
    }    
}

static void app_ama_disconnect_ind_handler(AMA_DISCONNECT_IND_T * ind)
{	
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_DISCONNECT_IND status:%d, BdAddr:0x%x%x", 3, ind->status, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
    app_ama_disconnect_handler(&ind->bdAddr);
}

static void app_ama_disconnect_cfm_handler(AMA_DISCONNECT_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_DISCONNECT_CFM status:%d, BdAddr:0x%x%x", 3, cfm->status, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));

	U8 status = AMA_CONNECTION_STATUS_DISCONNECTED;
	APP_RaceCmd_AMANotifyToAirapp(AMA_NOTIFY_ID_CONNECTION_STATUS, &status, sizeof(U8));
    
    app_ama_disconnect_handler(&cfm->bdAddr);
    // only for test
    #ifdef AMA_UT
    FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_GET_CENTRAL_INFORMATION, 0, 0, 10000);
    #endif
}

static void app_ama_start_speech_cfm_handler(AMA_START_SPEECH_CFM_T * cfm)
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;

	DBG_LOG_APP_AMA( "[APP_Ama] AMA_START_SPEECH_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));

	//if(APP_AudioDeviceRegister(&cfm->bdAddr, AUDIO_DEVICE_AMA))
	{
		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
		FW_Memcpy(&audioDspPara.bdAddr, &cfm->bdAddr, sizeof(BD_ADDR_T));
		audioDspPara.audioType = AUDIO_AMA;
		audioDspPara.bitRate = 32;
		APP_AudioDspPlay(&audioDspPara);
	}
}

static void app_ama_provide_speech_ind_handler(AMA_PROVIDE_SPEECH_IND_T * ind)
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;
    AMA_PROVIDE_SPEECH_T speechParam;

	app_ama_config_speech_para(&speechParam);

	DBG_LOG_APP_AMA( "[APP_Ama] AMA_PROVIDE_SPEECH_IND BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));

	//if(APP_AudioDeviceRegister(&ind->bdAddr, AUDIO_DEVICE_AMA))
	{
		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
		FW_Memcpy(&audioDspPara.bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T));
		audioDspPara.audioType = AUDIO_AMA;
		audioDspPara.bitRate = 32;
		APP_AudioDspPlay(&audioDspPara);
		AMA_Target_ProvideSpeechResponse(&ind->bdAddr, &speechParam, AMA_ERROR_CODE_SUCCESS);
	}
	/*else
	{
		AMA_Target_ProvideSpeechResponse(&ind->bdAddr, &speechParam, AMA_ERROR_CODE_UNKNOWN);
	}
	*/
}

static void app_ama_stop_speech_ind_handler(AMA_STOP_SPEECH_IND_T * ind)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_STOP_SPEECH_IND errorCode:%d, BdAddr:0x%x%x", 3, ind->errorCode, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
	//APP_AudioDeviceDeRegister(&ind->bdAddr, AUDIO_DEVICE_AMA);
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_AMA;
	APP_AudioDspStop(&audioDspPara);
	AMA_Target_StopSpeechResponse(&ind->bdAddr);
    //APP_ResumeOtherAudioDevice(&ind->bdAddr, AUDIO_DEVICE_AMA);
}

static void app_ama_stop_speech_cfm_handler(AMA_STOP_SPEECH_CFM_T * cfm)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_STOP_SPEECH_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
	//APP_AudioDeviceDeRegister(&cfm->bdAddr, AUDIO_DEVICE_AMA);
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, &cfm->bdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_AMA;
	APP_AudioDspStop(&audioDspPara);
    //APP_ResumeOtherAudioDevice(&cfm->bdAddr, AUDIO_DEVICE_AMA);
}

static void app_ama_end_point_speech_ind_handler(AMA_ENDPOINT_SPEECH_IND_T * ind)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_ENDPOINT_SPEECH_IND BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
	//APP_AudioDeviceDeRegister(&ind->bdAddr, AUDIO_DEVICE_AMA);
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, &ind->bdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_AMA;
	APP_AudioDspStop(&audioDspPara);
	AMA_Target_EndpointSpeechResponse(&ind->bdAddr);
    //APP_ResumeOtherAudioDevice(&ind->bdAddr, AUDIO_DEVICE_AMA);
}

static void app_ama_end_point_speech_cfm_handler(AMA_ENDPOINT_SPEECH_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_ENDPOINT_SPEECH_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
}

static void app_ama_speech_state_ind_handler(AMA_SPEECH_STATE_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SPEECH_STATE_IND state:%d, BdAddr:0x%x%x", 3, ind->state, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
}

static void app_ama_get_state_cfm_handler(AMA_GET_STATE_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_GET_STATE_CFM BdAddr:0x%x%x, feature:0x%x, Type:%d, value:%d", 5, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE),
																			cfm->state.feature, cfm->state.valueType, cfm->state.value.integer);
}

static void app_ama_get_state_ind_handler(AMA_GET_STATE_IND_T * ind)
{
	AMA_STATE_T state;
	AMA_ERROR_CODE_E errCode = AMA_ERROR_CODE_SUCCESS;

	DBG_LOG_APP_AMA( "[APP_Ama] AMA_GET_STATE_IND BdAddr:0x%x%x, feature:0x%x", 3, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE), ind->feature);

	state.feature = ind->feature;

	switch (ind->feature)
	{
		case AMA_STATE_FEATURE_BT_DISCOVERABLE:
			state.valueType = AMA_VALUE_TYPE_BOOLEAN;
			state.value.integer = 1;
			break;
        case AMA_STATE_FEATURE_A2DP_ENABLED:
        case AMA_STATE_FEATURE_HFP_ENABLED:
            state.valueType = AMA_VALUE_TYPE_BOOLEAN;
			state.value.integer = 1;
            break;
		case AMA_STATE_FEATURE_A2DP_CONNECTED:
			state.valueType = AMA_VALUE_TYPE_BOOLEAN;
			state.value.integer = app_ama_is_a2dp_connected(&ind->bdAddr);
			break;
		case AMA_STATE_FEATURE_HFP_CONNECTED:
			state.valueType = AMA_VALUE_TYPE_BOOLEAN;
			state.value.integer = app_ama_is_hfp_connected(&ind->bdAddr);
			break;
		default:
			errCode = AMA_ERROR_CODE_UNSUPPORTED;
			state.valueType = AMA_VALUE_TYPE_BOOLEAN;
			state.value.integer = 0;
			break;
	}
	AMA_Target_GetStateResponse(&ind->bdAddr, &state, errCode);
}

static void app_ama_set_state_ind_handler(AMA_SET_STATE_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SET_STATE_IND BdAddr:0x%x%x, feature:0x%x, Type:%d, value:%d", 5, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE),
																			ind->state.feature, ind->state.valueType, ind->state.value.integer);
	switch(ind->state.feature)
	{
		case AMA_STATE_FEATURE_AVRCP_OVERRIDE:
			APP_Ama_SetLinkAVRCPOverride(&ind->bdAddr, ind->state.value.boolean);
			break;

		default:
			AMA_Target_SetStateResponse(&ind->bdAddr, AMA_ERROR_CODE_UNSUPPORTED);
			return;
	}
	AMA_Target_SetStateResponse(&ind->bdAddr, AMA_ERROR_CODE_SUCCESS);
}

static void app_ama_sync_state_cfm_handler(AMA_SYNCHRONIZE_STATE_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SYNCHRONIZE_STATE_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
}

static void app_ama_sync_state_ind_handler(AMA_SYNCHRONIZE_STATE_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SYNCHRONIZE_STATE_IND BdAddr:0x%x%x, feature:0x%x, Type:%d, value:%d", 5, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE),
																			ind->state.feature, ind->state.valueType, ind->state.value);
	AMA_Target_SynchronizeStateResponse(&ind->bdAddr);
}

static void app_ama_issue_media_control_cfm_handler(AMA_ISSUE_MEDIA_CONTROL_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_ISSUE_MEDIA_CONTROL_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
}

static void app_ama_issue_media_control_ind_handler(AMA_ISSUE_MEDIA_CONTROL_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_ISSUE_MEDIA_CONTROL_IND control:%d, BdAddr:0x%x%x", 3, ind->control, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));

	switch(ind->control)
	{
		case AMA_MEDIA_CONTROL_PLAY:
			APP_AvrcpDecideOperation(&ind->bdAddr, AVC_OPERATION_PLAY);
			break;
		case AMA_MEDIA_CONTROL_PAUSE:
			APP_AvrcpDecideOperation(&ind->bdAddr, AVC_OPERATION_PAUSE);
			break;
		case AMA_MEDIA_CONTROL_PLAY_PAUSE:
			APP_AvrcpDecideOperation(&ind->bdAddr, AVC_OPERATION_KEY_PLAY);
			break;
		case AMA_MEDIA_CONTROL_NEXT:
			APP_AvrcpDecideOperation(&ind->bdAddr, AVC_OPERATION_FORWARD);
			break;
		case AMA_MEDIA_CONTROL_PREVIOUS:
			APP_AvrcpDecideOperation(&ind->bdAddr, AVC_OPERATION_BACKWARD);
			break;
		default:
			AMA_Target_IssueMediaControlResponse(&ind->bdAddr, AMA_ERROR_CODE_UNKNOWN);
			return;
	}
	AMA_Target_IssueMediaControlResponse(&ind->bdAddr, AMA_ERROR_CODE_SUCCESS);
}

static void app_ama_forward_at_cmd_ind_handler(AMA_FORWARD_AT_COMMAND_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_FORWARD_AT_COMMAND_IND BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
	AMA_Target_ForwardATCommandResponse(&ind->bdAddr, AMA_ERROR_CODE_SUCCESS);
}

static void app_ama_incoming_call_cfm_handler(AMA_INCOMING_CALL_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_INCOMING_CALL_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));
}

static void app_ama_switch_transport_ind_handler(AMA_SWITCH_TRANSPORT_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SWITCH_TRANSPORT_IND oldBdAddr:0x%x%x, newBdAddr:0x%x%x", 4, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE), FW_bdaddr_to_2U32(&ind->newBdAddr, TRUE), FW_bdaddr_to_2U32(&ind->newBdAddr, FALSE));
	APP_Ama_UpdateBdAddr(&ind->bdAddr, &ind->newBdAddr);
}

static void app_ama_update_device_info_ind_handler(AMA_UPDATE_DEVICE_INFORMATION_IND_T * ind)
{
	U8 * p;

	DBG_LOG_APP_AMA( "[APP_Ama] AMA_UPDATE_DEVICE_INFORMATION_IND BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));

    p = &ind->name[0];
    LOG_D(LOG_GERNERAL, "[APP_Ama] name: %x %x %x %x %x %x", 6, p[0], p[1], p[2], p[3], p[4], p[5]);
}

static void app_ama_override_assistant_ind_handler(AMA_OVERRIDE_ASSISTANT_IND_T * ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_OVERRIDE_ASSISTANT_IND errorCode:%d, BdAddr:0x%x%x", 3, ind->errorCode, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
	#ifdef AMA_UT
	FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_DISCONNECT, 0, 0, 10000);
	#endif
}

static void app_ama_get_central_info_cfm_handler(AMA_GET_CENTRAL_INFORMATION_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_GET_CENTRAL_INFORMATION_CFM errorCode:%d, BdAddr:0x%x%x, platform:%d", 4, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->platform);
	LOG_D(LOG_GERNERAL, "[APP_Ama] %x %x %x %x %x %x %x %x", 8, cfm->name[0], cfm->name[1], cfm->name[2], cfm->name[3], cfm->name[4], cfm->name[5], cfm->name[6], cfm->name[7]);

	#ifdef AMA_UT
	FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_OVERRIDE_ASSISTANT, 0, 0, 10000);
	#endif
}

static void app_ama_override_assistant_cfm_handler(AMA_OVERRIDE_ASSISTANT_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_OVERRIDE_ASSISTANT_CFM errorCode:%d, BdAddr:0x%x%x", 3, cfm->errorCode, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE));

	#ifdef AMA_UT
	FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_FACTORY_RESET, 0, 0, 5000);
	#endif
}


#if 0
static void app_ama_factory_reset_cfm_handler(AMA_FACTORY_RESET_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_FACTORY_RESET_CFM status:%d", 1, cfm->status);

	if(cfm->status == BT_STATUS_SUCCESS)
		APP_Reset_Loader();

	#ifdef AMA_UT
    FW_SetTimer(&gAppAmaTimerHandle, APP_AMA_TIMER_ID_SET_DEVICE_CONFIG, 0, 0, 5000);
    #endif
}
#endif

static void app_ama_get_data_cfm_handler(AMA_GET_DATA_CFM_T * cfm)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_GET_DATA_CFM status:%d", 1, cfm->status);

	if(cfm->status == BT_STATUS_SUCCESS)
	{
        APP_Mcsync_SendAmaData(cfm->pData, cfm->length);
	}
}

static void app_ama_get_locales_ind_handler(AMA_GET_LOCALES_IND_T *ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_GET_LOCALES_IND", 0);

	AMA_Target_GetLocalesResponse(&ind->bdAddr, NULL, AMA_ERROR_CODE_UNSUPPORTED);
}

static void app_ama_set_locale_ind_handler(AMA_SET_LOCALE_IND_T *ind)
{
	DBG_LOG_APP_AMA( "[APP_Ama] AMA_SET_LOCALE_IND id:%d", 1, ind->locale_id);

	AMA_Target_SetLocaleResponse(&ind->bdAddr, AMA_ERROR_CODE_UNSUPPORTED);
}


static U32 app_ama_handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
		/* CONNECTION */
		case AMA_START_SERVICE_CFM:
			app_ama_start_service_cfm_handler((AMA_START_SERVICE_CFM_T *)msg);
			break;

		case AMA_STOP_SERVICE_CFM:
			app_ama_stop_service_cfm_handler((AMA_STOP_SERVICE_CFM_T *)msg);
			break;

		case AMA_ENABLE_ADV_CFM:
			app_ama_enable_adv_cfm_handler((AMA_ENABLE_ADV_CFM_T *)msg);
			break;

		case AMA_DISABLE_ADV_CFM:
			app_ama_disable_adv_cfm_handler((AMA_DISABLE_ADV_CFM_T *)msg);
			break;

		case AMA_CONNECT_CFM:
			app_ama_connect_cfm_handler((AMA_CONNECT_CFM_T *)msg);
			break;

		case AMA_DISCONNECT_IND:
			app_ama_disconnect_ind_handler((AMA_DISCONNECT_IND_T *)msg);
			break;

		case AMA_DISCONNECT_CFM:
			app_ama_disconnect_cfm_handler((AMA_DISCONNECT_CFM_T *)msg);
			break;

		/* SPEECH */
		case AMA_START_SPEECH_CFM:
			app_ama_start_speech_cfm_handler((AMA_START_SPEECH_CFM_T *)msg);
			break;

		case AMA_PROVIDE_SPEECH_IND:
			app_ama_provide_speech_ind_handler((AMA_PROVIDE_SPEECH_IND_T *)msg);
			break;

		case AMA_STOP_SPEECH_IND:
			app_ama_stop_speech_ind_handler((AMA_STOP_SPEECH_IND_T *)msg);
			break;

		case AMA_STOP_SPEECH_CFM:
			app_ama_stop_speech_cfm_handler((AMA_STOP_SPEECH_CFM_T *)msg);
			break;

		case AMA_ENDPOINT_SPEECH_IND:
			app_ama_end_point_speech_ind_handler((AMA_ENDPOINT_SPEECH_IND_T *)msg);
			break;

		case AMA_ENDPOINT_SPEECH_CFM:
			app_ama_end_point_speech_cfm_handler((AMA_ENDPOINT_SPEECH_CFM_T *)msg);
			break;

		case AMA_SPEECH_STATE_IND:
			app_ama_speech_state_ind_handler((AMA_SPEECH_STATE_IND_T *)msg);
			break;

		/* STATE */
		case AMA_GET_STATE_CFM:
			app_ama_get_state_cfm_handler((AMA_GET_STATE_CFM_T *)msg);
			break;

		case AMA_GET_STATE_IND:
			app_ama_get_state_ind_handler((AMA_GET_STATE_IND_T *)msg);
			break;

		case AMA_SET_STATE_IND:
			app_ama_set_state_ind_handler((AMA_SET_STATE_IND_T *)msg);
			break;

		case AMA_SYNCHRONIZE_STATE_CFM:
			app_ama_sync_state_cfm_handler((AMA_SYNCHRONIZE_STATE_CFM_T *)msg);
			break;

		case AMA_SYNCHRONIZE_STATE_IND:
			app_ama_sync_state_ind_handler((AMA_SYNCHRONIZE_STATE_IND_T *)msg);
			break;

		/* MEDIA */
		case AMA_ISSUE_MEDIA_CONTROL_CFM:
			app_ama_issue_media_control_cfm_handler((AMA_ISSUE_MEDIA_CONTROL_CFM_T *)msg);
			break;

		case AMA_ISSUE_MEDIA_CONTROL_IND:
			app_ama_issue_media_control_ind_handler((AMA_ISSUE_MEDIA_CONTROL_IND_T *)msg);
			break;

		/* Cellular Calling */
		case AMA_FORWARD_AT_COMMAND_IND:
			app_ama_forward_at_cmd_ind_handler((AMA_FORWARD_AT_COMMAND_IND_T *)msg);
			break;

		case AMA_INCOMING_CALL_CFM:
			app_ama_incoming_call_cfm_handler((AMA_INCOMING_CALL_CFM_T *)msg);
			break;

		/* MISC IND*/
		case AMA_SWITCH_TRANSPORT_IND:
			app_ama_switch_transport_ind_handler((AMA_SWITCH_TRANSPORT_IND_T *)msg);
			break;

		case AMA_UPDATE_DEVICE_INFORMATION_IND:
			app_ama_update_device_info_ind_handler((AMA_UPDATE_DEVICE_INFORMATION_IND_T *)msg);
			break;

		case AMA_OVERRIDE_ASSISTANT_IND:
			app_ama_override_assistant_ind_handler((AMA_OVERRIDE_ASSISTANT_IND_T *)msg);
			break;

        /* MISC CFM*/
		case AMA_GET_CENTRAL_INFORMATION_CFM:
			app_ama_get_central_info_cfm_handler((AMA_GET_CENTRAL_INFORMATION_CFM_T *)msg);
			break;

		case AMA_OVERRIDE_ASSISTANT_CFM:
			app_ama_override_assistant_cfm_handler((AMA_OVERRIDE_ASSISTANT_CFM_T *)msg);
			break;

		case AMA_FACTORY_RESET_CFM:
			//app_ama_factory_reset_cfm_handler((AMA_FACTORY_RESET_CFM_T *)msg);
			break;

        case AMA_DATA_CHANGED_IND:
            APP_Mcsync_SyncAmaData();
            break;

        case AMA_ADD_DATA_APPLY_PARTNER_CFM:
            APP_Mcsync_SyncAmaData();
            break;

        case AMA_GET_DATA_CFM:
            app_ama_get_data_cfm_handler((AMA_GET_DATA_CFM_T *)msg);
            break;

        case AMA_ADD_DATA_APPLY_AGENT_CFM:
            break;

        case AMA_GET_LOCALES_IND:
            app_ama_get_locales_ind_handler((AMA_GET_LOCALES_IND_T *)msg);
            break;

        case AMA_SET_LOCALE_IND:
            app_ama_set_locale_ind_handler((AMA_SET_LOCALE_IND_T *)msg);
            break;
            
		default:
			DBG_LOG_APP_AMA( "[APP_Ama] Unknow Ind:%d", 1, id);
			break;
	}

	return 0;
}

static BOOL app_ama_is_a2dp_connected(BD_ADDR_T *pBdAddr)
{
    BD_ADDR_T *pService = APP_GetServiceBdAddr();

    if(pBdAddr)
    {
        if(a2dp_is_connected(pBdAddr))
        {
            return TRUE;
        }
    }
    if(pService)
        return a2dp_is_connected(pService);

    return FALSE;
}

static BOOL app_ama_is_hfp_connected(BD_ADDR_T *pBdAddr)
{
    BD_ADDR_T *pService = APP_GetServiceBdAddr();

    if(pBdAddr)
    {
        if(HFP_IsConnected(pBdAddr))
        {
            return TRUE;
        }
    }
    if(pService)
        return HFP_IsConnected(pService);

    return FALSE;
}

static void app_ama_config_speech_para(AMA_PROVIDE_SPEECH_T *pSpeechParam)
{
    pSpeechParam->audioProfile = APP_Ama_NvkeyGetAudioProfile();
    pSpeechParam->audioFormat  = APP_Ama_NvkeyGetAudioFormat();
    pSpeechParam->audioSource  = APP_Ama_NvkeyGetAudioSource();
}

/**************************************************************************************************
* Public API
**************************************************************************************************/
void APP_Ama_RegisterProfile(void)
{
	if (AMA_STATUS_OK != AMA_Target_StartService((Handler)&gAppAmaHandle, FW_AMA_MAX_LINK_IDX))
	{
        DBG_LOG_APP_AMA( "[APP_Ama] AMA_Target_StartService fail!", 0);
	}
}

APP_AMA_CTL_STRU *APP_Ama_GetEmptyLinkInfo(void)
{
	U8 i;

	for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(APP_AMA_STATE_NONE == gAppAmaCtl[i].amaState)
		{
			return &gAppAmaCtl[i];
		}
	}

	return NULL;
}

void APP_Ama_SetLinkInfo(BD_ADDR_T *pBdAddr, U8 state)
{
	U8 i;
	APP_AMA_CTL_STRU *pLinkInfo;

	for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if (!FW_Memcmp(pBdAddr, &gAppAmaCtl[i].bdAddr, sizeof(BD_ADDR_T)))
		{
			gAppAmaCtl[i].amaState = state;
			return;
		}
	}

	if((APP_AMA_CTL_STRU *)NULL == (pLinkInfo = APP_Ama_GetEmptyLinkInfo()))
	{
		DBG_LOG_APP_AMA( "[APP_Ama] APP_Ama_SetLinkInfo no resource", 0);
		return;
	}

	FW_Memcpy(&pLinkInfo->bdAddr, pBdAddr, sizeof(BD_ADDR_T));
	pLinkInfo->amaState = state;
}

void APP_Ama_ClearLinkInfo(BD_ADDR_T *pBdAddr)
{
	U8 i = 0;

	for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
	    if (!FW_Memcmp(pBdAddr, &gAppAmaCtl[i].bdAddr, sizeof(BD_ADDR_T)))
	    {
			FW_Memset(&gAppAmaCtl[i], 0, sizeof(APP_AMA_CTL_STRU));
	    }
	}
}

APP_AMA_CTL_STRU *APP_Ama_GetLinkInfo(void)
{
    U8 i;
	APP_AMA_CTL_STRU *pLinkInfo;

    for (i = 0; i < FW_MAX_LINK_IDX; i++)
    {
        if (APP_AMA_STATE_CONNECTED == gAppAmaCtl[i].amaState)
        {
			pLinkInfo = &gAppAmaCtl[i];
            DBG_LOG_APP_AMA( "[APP_Ama] getBdAddr: %x %x %x %x %x %x", 6, pLinkInfo->bdAddr.addr[0], 
				pLinkInfo->bdAddr.addr[1], pLinkInfo->bdAddr.addr[2], pLinkInfo->bdAddr.addr[3], 
				pLinkInfo->bdAddr.addr[4], pLinkInfo->bdAddr.addr[5]);
            return pLinkInfo;
        }
    }
    return NULL;
}


void APP_Ama_SetLinkAVRCPOverride(BD_ADDR_T *pBdAddr, BOOL isAVRCPOverride)
{

	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();

	if(pAmaLinkInfo)
	{
		if(FW_CmpBdAddr(&pAmaLinkInfo->bdAddr, pBdAddr))
		{
			pAmaLinkInfo->isAVRCPOverride = isAVRCPOverride;
		}
	}
}

BOOL APP_Ama_GetLinkAVRCPOverride(void)
{
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();

	if(pAmaLinkInfo)
	{
		return pAmaLinkInfo->isAVRCPOverride;
	}

	return FALSE;

}

void APP_Ama_UpdateBdAddr(BD_ADDR_T *pBdAddr, BD_ADDR_T *pSwitchTransportBdAddr)
{
    U8 i;

    for (i = 0; i < FW_MAX_LINK_IDX; i++)
    {
        if (!FW_Memcmp(pBdAddr, &gAppAmaCtl[i].bdAddr, sizeof(BD_ADDR_T)))
        {
            FW_Memcpy(&gAppAmaCtl[i].bdAddr, pSwitchTransportBdAddr, sizeof(BD_ADDR_T));
            return;
        }
    }
}

BOOL APP_Ama_Init(void)
{
	if(APP_Ama_NvkeyInit())
	{
        APP_Ble_RegisterAdvHandle(&gAppAmaNode);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void APP_Ama_IncomingCallHandler(U8 *pMsg, U8 length)
{
	APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	if(pAmaLinkInfo)
	{
		AMA_Target_IncommingCall(&pAmaLinkInfo->bdAddr, pMsg, length);
	}
}

BOOL APP_Ama_IsAmaConnected(void)
{
	return (APP_Ama_GetLinkInfo()) ? TRUE: FALSE;
}

void APP_Ama_GetData(void)
{
    AMA_Target_GetData();
}

void APP_Ama_Disconnect(BD_ADDR_T *pBdAddr)
{
    APP_AMA_CTL_STRU *pAmaLinkInfo = APP_Ama_GetLinkInfo();
	if(pAmaLinkInfo)
	{
		AMA_Target_ResetConnection(&pAmaLinkInfo->bdAddr, 0, TRUE);
	}
}

/**************************************************************************************************
* Public API (RHO)
**************************************************************************************************/
BOOL APP_Ama_FillRhoData(void *pData)
{
	APP_AMA_CTL_STRU *pLinkInfo;
	APP_RHO_AMA_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appAma;

	if(!pDataToFill)
	{
		DBG_LOG_APP_AMA("[APP_Ama] Fill RHO data error", 0);
		return FALSE;
	}

	pLinkInfo = APP_Ama_GetLinkInfo();
	if(pLinkInfo)
	{
		FW_Memcpy(pDataToFill, pLinkInfo, sizeof(APP_AMA_CTL_STRU));
	}

	DBG_LOG_APP_AMA("[APP_Ama] Fill RHO data OK", 0);
	return TRUE;
}

BOOL APP_Ama_AssignRhoData(void *pData)
{
	APP_AMA_CTL_STRU *pLinkInfo;
	APP_RHO_AMA_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appAma;

	if(!pDataToGet)
	{
		DBG_LOG_APP_AMA("[APP_Ama] Assign RHO data error", 0);
		return FALSE;
	}

	pLinkInfo = APP_Ama_GetEmptyLinkInfo();
	if(!pLinkInfo)
	{
		DBG_LOG_APP_AMA("[APP_Ama] Assign RHO empty link", 0);
		return FALSE;
	}
	FW_Memcpy(pLinkInfo, pDataToGet, sizeof(APP_AMA_CTL_STRU));
    
    APP_Ble_EnableAdv(BLE_ADV_AMA_MASK);

	DBG_LOG_APP_AMA("[APP_Ama] Assign RHO data OK", 0);
	return TRUE;
}

void APP_Ama_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr)
{
	APP_AMA_CTL_STRU *pLinkInfo;

    UNUSED(pOldAgentAddr);
    UNUSED(pNewAgentAddr);
    
    APP_Ble_DisableAdv(BLE_ADV_AMA_MASK);
	if((pLinkInfo = APP_Ama_GetLinkInfo()))
		APP_Ama_ClearLinkInfo(&pLinkInfo->bdAddr);  

	UNUSED(pOldAgentAddr); UNUSED(pNewAgentAddr);
}

#endif //PROFILE_AMA_ENABLE

