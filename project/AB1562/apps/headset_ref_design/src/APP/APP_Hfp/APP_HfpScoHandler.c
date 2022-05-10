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
#include "Hfp_atCommand.h"
#include "Hfp_Interface.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_Registration.h"
#include "App_ResourceManager.h"
#include "APP_HfpScoHandler.h"
#include "App_Hfp.h"
#include "App_State.h"
#include "App_Media.h"
#include "PM_Interface.h"
#include "app_hfpnvkey_nvstruc.h"
#include "App_System.h"
#include "App_Nvram_Type.h"
#include "App_VolumeManage.h"
#include "App_SniffHandler.h"
#include "drv_sco.h"


extern BOOL g_is_fota_ongoing;
/**************************************************************************************************
* Define
**************************************************************************************************/
#define AIRMODE_MSBC	0x03 //spec: Transparent Data
#define AIRMODE_CVSD	0x02 //spec: CVSD




static U32 app_HfpSCOHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static const HandlerData gAppHfpSCOHandle = { app_HfpSCOHandler };
/**************************************************************************************************
* Structure
**************************************************************************************************/
/*Static*/
static void app_HfpSCO_AddSCOAudioLink(BD_ADDR_T *pBdAddr)
{
	U8 audioDev;

	audioDev = AUDIO_DEVICE_SCO_IDLE;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE   
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE   
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsHspConnected(pBdAddr))
	{
		audioDev = AUDIO_DEVICE_SCO_HSP;
		goto SET_SCO_AUDIO_DEVICE;
	}
#endif
	DBG_LOG_APP_HFP( "[APP_HFP] Sco Add Link State:%d, %d, %d, %d", 4,
			APP_State_CheckLinkInIncomingState(pBdAddr), APP_State_CheckOtherLinksInIncomingState(pBdAddr),
			APP_State_CheckLinkInOutGoingState(pBdAddr), APP_State_CheckLinkInCallActiveState(pBdAddr));

	if((APP_State_CheckLinkInIncomingState(pBdAddr) && !APP_State_CheckOtherLinksInIncomingState(pBdAddr))
		|| APP_State_CheckLinkInOutGoingState(pBdAddr) || APP_State_CheckLinkInCallActiveState(pBdAddr))
	{
		if(APP_AudioDeviceRegister(pBdAddr, AUDIO_DEVICE_SCO))
		{
			DBG_LOG_APP_HFP( "[APP_HFP] Reg Sco and Send DSP Cmd", 0);
			APP_HfpSco_SendDSPCmd(pBdAddr);
		}
	}
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	SET_SCO_AUDIO_DEVICE:
#endif	
	if(APP_AudioDeviceRegister(pBdAddr, audioDev))
	{
		DBG_LOG_APP_HFP( "[APP_HFP] Reg AudioDev and Send DSP Cmd", 0);
		APP_HfpSco_SendDSPCmd(pBdAddr);
	}
}

static void app_HfpSCO_RemoveSCOAudioLink(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_SCO;
	APP_AudioDspStop(&audioDspPara);

<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_Hfp_IsHspConnected(pBdAddr))
	{
		APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_SCO_HSP);
	}
	else
#endif        
	{
		APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_SCO);
		APP_AudioDeviceDeRegister(pBdAddr, AUDIO_DEVICE_SCO_IDLE);
	}
	//#if defined HFP_Profile && defined MMI_SUPPORT_MULTI_HFP_FEAT
	//if(APP_HFP_SearchCallStatusInd(pBdAddr, APP_CALL_ACTIVE) >= APP_MAX_ONGOING_CALL_NO || APP_EOF == APP_GetNextNormalLink(pBdAddr))
	//#endif
	{
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE	
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE	
>>>>>>> db20e11 (second commit)
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO_HSP);
#endif
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO);
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO_IDLE);
	}
//#ifdef SUPPORT_MIC_IN_A2DP_MODE
	//if(APP_GetDspAncAtState() == DSP_AT_ON)
	//{
	//	MMI_LineIn_SendDriverVolCmd();
	//}
//#endif
}

static void app_HfpSCO_ConnOKHandle(BD_ADDR_T *pBdAddr, SINK sink, U16 scoHandle, U8 codec)
{
	PM_SetLinkTrafficBusy(pBdAddr, TRUE);
	APP_SetSCOConnected(pBdAddr, sink, scoHandle, codec);

    gAppCtl.scoCount++;
	//if(gMMI_ctl.scoCnt++ ==0)
	//{
	//	MMI_HCI_SendWritePageScanActivity();
	//}
	//MMI_ScanHandlerWhenSCO(TRUE);

	APP_Media_PushMediaEvent(MEDIA_EVT_AUDIO_LINK_OPEN);
	app_HfpSCO_AddSCOAudioLink(pBdAddr);
	App_SniffDisable(pBdAddr, APP_SNIFF_SCO);
}

static void app_HfpSCO_ConnOKProfileHandle(BD_ADDR_T *pBdAddr)
{
	APP_HFP_ScoConnOKHandler(pBdAddr);
}

static void app_HfpSCO_ConnFailHandle(BD_ADDR_T *pBdAddr)
{
	PM_SetLinkTrafficBusy(pBdAddr, FALSE);
	APP_SetSCODisconnected(pBdAddr);
	app_HfpSCO_RemoveSCOAudioLink(pBdAddr);
}

static void app_HfpSCO_ConnFailProfileHandle(BD_ADDR_T *pBdAddr)
{
	APP_HFP_ScoConnFailHandler(pBdAddr);
}

static void app_HfpSCO_DiscHandle(BD_ADDR_T *pBdAddr)
{
	if(APP_SCO_EXISTS(pBdAddr))
	{
        gAppCtl.scoCount--;
		//if (--gMMI_ctl.scoCnt == 0)
		//{
		//	if(APP_PowerOff_IsPowerOn()) // Not Power off
		//	{
		//		//There is no sco link now, MMI needs to change scan window parameters back to the original settings.
		//		MMI_ScanHandlerWhenSCO(FALSE);
		//	}
		//}
		APP_Media_PushMediaEvent(MEDIA_EVT_AUDIO_LINK_CLOSE);
		//MMI_HCI_SendWritePageScanActivity();
	}
	APP_SetSCODisconnected(pBdAddr);
	//APP_SetSwitchingSCO(FALSE);
	app_HfpSCO_RemoveSCOAudioLink(pBdAddr);
	App_SniffEnable(pBdAddr, APP_SNIFF_SCO);
}

static void app_HfpSCO_DiscProfileHandle(BD_ADDR_T *pBdAddr)
{
	APP_HFP_ScoDiscHandler(pBdAddr);

	if(APP_IsLinkActiveDisconnecting(pBdAddr))
	{
		APP_Conn_ReleaseProfileLinkAndDetach(pBdAddr);
	}
}

static void app_HfpSCOSettingConnConfig(BT_SYNC_CONFIG_PARAM_T* config, BOOL isCodecMSBC, U8 linkType)
{
	config->max_latency = 0x0014;
	config->retx_effort = 0x01;
	config->voice_settings = isCodecMSBC ? 0x0003 : 0;

	DBG_LOG_APP_HFP( "[APP_HFP] link type:[%s]", 1, APP_HFPScoLinkType[linkType]);

	if(linkType == LINK_TYPE_SCO)
	{
		config->packet_type = App_Hfp_SCO_ReadNvkeyPktParaTypeSCO();//0x03C7;//SCO
	}
	else if(linkType == LINK_TYPE_SCO_ONLY_HV3)
	{
		config->packet_type = App_Hfp_SCO_ReadNvkeyPktParaTypeSCOHV3();//0x03C4;//SCO /*HV3*/
	}
	else
	{
		config->packet_type = App_Hfp_SCO_ReadNvkeyPktParaTypeeSCO();//0x003F;//eSCO
	}
	config->rx_bandwidth = 0x1F40;
	config->tx_bandwidth = 0x1F40;

	//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
	//to LM Version 1.1 by shift 5 bits to the left.
	//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
	//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
	//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
	//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
	//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
	//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
	//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
	//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
	//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
	//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used

}

static void app_HfpSCORegHandler(BT_SYNC_REGISTER_CFM_T *cfm)
{
	UNUSED(cfm);
}

static void app_HfpSCOConnectIndHandler(BT_SYNC_CONNECT_IND_T *ind)
{
	BT_SYNC_CONFIG_PARAM_T config;
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)ind->bd_addr;
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(!pLinkInfo || (!HFP_IsSCOCodecCVSD(pBdAddr) && !HFP_IsSCOCodecMSBC(pBdAddr)))
	{
		DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_CONNECT_IND reject - no resource", 0);
		BtSyncConnectResponse((Handler)&gAppHfpSCOHandle, ind->bd_addr, FALSE, ind->link_type, HCI_ERCODE_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES, &config);
		return;
	}

	switch(APP_GetScoState(pLinkInfo))
	{
		case SCO_STATE_DISC:
		case SCO_STATE_ACTIVE_CONNECT:
			DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_CONNECT_IND accept state %d", 1, APP_GetScoState(pLinkInfo));
			APP_SetScoState(pLinkInfo, SCO_STATE_PASSIVE_CONNECT);
			app_HfpSCOSettingConnConfig(&config, HFP_IsSCOCodecMSBC(pBdAddr), ind->link_type);
			BtSyncConnectResponse((Handler)&gAppHfpSCOHandle, ind->bd_addr, TRUE, ind->link_type, 0, &config);
			break;

		case SCO_STATE_PASSIVE_CONNECT:
		case SCO_STATE_CONNECTED:
		case SCO_STATE_DISCONNECTING_WAIT_SEND_REQ:
		case SCO_STATE_DISCONNECTING_WAIT_REQ_CPL:
			DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_CONNECT_IND reject - wrong state %d", 1, APP_GetScoState(pLinkInfo));
			BtSyncConnectResponse((Handler)&gAppHfpSCOHandle, ind->bd_addr, FALSE, ind->link_type, HCI_ERCODE_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES, &config);
			break;
	}
}

static U8 app_Sco_AirModeToCodec(U8 airmode)
{
	return (airmode == AIRMODE_MSBC) ? SCO_CODEC_MSBC : SCO_CODEC_CVSD;
}

static void app_HfpSCOConnectCfmHandler(BT_SYNC_CONNECT_CFM_T *cfm)
{
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)cfm->bd_addr;
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_CONNECT_CFM - status:%d, scoHandler:%d, linkType:%d, airMode:%d", 4, cfm->hci_status, cfm->sco_handle, cfm->link_type, cfm->airmode);
	//DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_CONNECT_CFM - LocalScoState:%d, AgentScoState:%d, sink:%d", 3, APP_GetScoState(pLinkInfo), APP_GetAgentScoState(), cfm->sink);

	if(pLinkInfo)
	{
		if(!cfm->hci_status)
		{
			switch(APP_GetScoState(pLinkInfo))
			{
				case SCO_STATE_DISC:
				case SCO_STATE_ACTIVE_CONNECT:
				case SCO_STATE_PASSIVE_CONNECT:
					//APP_AudioDspStop(pBdAddr, AUDIO_SCO); //codec switch or sco packet type change might needs to restart dsp (not sure 2018.01.14)
					app_HfpSCO_ConnOKHandle(pBdAddr, cfm->sink, cfm->sco_handle, app_Sco_AirModeToCodec(cfm->airmode));
					app_HfpSCO_ConnOKProfileHandle(pBdAddr);
					break;
				case SCO_STATE_CONNECTED:
					break;
				case SCO_STATE_DISCONNECTING_WAIT_SEND_REQ:
				case SCO_STATE_DISCONNECTING_WAIT_REQ_CPL:
					BtSyncDisconnectRequest((Handler)&gAppHfpSCOHandle, cfm->sink, 0x13);
					break;
			}

			/* Active fota will suspend DSP, Background fota will not suspend DSP, Background fota do not Disconnect esco */
			if(g_is_fota_ongoing && APP_AudioIsDspSuspend() && SCO_STATE_DISCONNECTING_WAIT_SEND_REQ != APP_GetScoState(pLinkInfo) && SCO_STATE_DISCONNECTING_WAIT_REQ_CPL != APP_GetScoState(pLinkInfo))
				APP_HfpSco_DisconnectRequest(pBdAddr);
		}
		else
		{
			app_HfpSCO_ConnFailHandle(pBdAddr);
			app_HfpSCO_ConnFailProfileHandle(pBdAddr);
		}
	}
	else
	{
		BtSyncDisconnectRequest((Handler)&gAppHfpSCOHandle, cfm->sink, 0x13);
	}

}

static void app_HfpSCODisconnectHandle(SINK sink)
{
	APP_HfpSco_DisconnectHandleByBdAddress(APP_GetBdAddrBySCOSink(sink));
}

static void app_HfpSCODisconnectIndHandler(BT_SYNC_DISCONNECT_IND_T *msg)//Has to add BdAddr in structure
{
	app_HfpSCODisconnectHandle(msg->sink);
}

static void app_HfpSCODisconnectCfmHandler(BT_SYNC_DISCONNECT_CFM_T *msg)//Has to add BdAddr in structure
{
	app_HfpSCODisconnectHandle(msg->sink);
}


static U32 app_HfpSCOHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{

	UNUSED(handler);
	UNUSED(handler_id);

	DBG_LOG_APP_HFP( "[APP_HFP] app_HfpSCOHandler - %s, id:%x", 2, APP_HFPScoHandle[ id - BT_SYNC_REGISTER_CFM ], id);

	switch(id)
	{
		case BT_SYNC_REGISTER_CFM:
			app_HfpSCORegHandler((BT_SYNC_REGISTER_CFM_T*)msg);
			break;

		case BT_SYNC_UNREGISTER_CFM:

			break;

		case BT_SYNC_CONNECT_IND:
			app_HfpSCOConnectIndHandler((BT_SYNC_CONNECT_IND_T*)msg);
			break;

		case BT_SYNC_CONNECT_CFM:
			app_HfpSCOConnectCfmHandler((BT_SYNC_CONNECT_CFM_T*)msg);
			break;

		case BT_SYNC_DISCONNECT_IND:
			app_HfpSCODisconnectIndHandler((BT_SYNC_DISCONNECT_IND_T *)msg);
			break;

		case BT_SYNC_DISCONNECT_CFM:
			app_HfpSCODisconnectCfmHandler((BT_SYNC_DISCONNECT_CFM_T *)msg);
			break;

		default:
			break;
	}
	return 0;
}


/*Public*/
void APP_HfpSco_Register(void)
{
    PM_GAP_RegisterScoApp((Handler)&gAppHfpSCOHandle);
}

void APP_HfpSco_ConnectRequest(BD_ADDR_T *pBdAddr, BOOL isCodecMSBC, U8 linkType)
{
	static BT_SYNC_CONFIG_PARAM_T config;
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(!pLinkInfo)
	{
		return;
	}

	switch(APP_GetScoState(pLinkInfo))
	{
		case SCO_STATE_DISC:
			APP_SetScoState(pLinkInfo, SCO_STATE_ACTIVE_CONNECT);
			app_HfpSCOSettingConnConfig(&config, isCodecMSBC, linkType);
			BtSyncConnectRequest((Handler)&gAppHfpSCOHandle, HFP_GetSinkByBdAddr(pBdAddr), &config);
			break;
		case SCO_STATE_ACTIVE_CONNECT:
			break;
		case SCO_STATE_PASSIVE_CONNECT:
			APP_SetScoState(pLinkInfo, SCO_STATE_ACTIVE_CONNECT);
			break;
		case SCO_STATE_CONNECTED:
			break;

		case SCO_STATE_DISCONNECTING_WAIT_SEND_REQ:
		case SCO_STATE_DISCONNECTING_WAIT_REQ_CPL:
			break;
	}
}

void APP_HfpSco_DisconnectRequest(BD_ADDR_T *pBdAddr)
{
	APP_INFO_STRU * pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);
	if(!pLinkInfo)
	{
		return;
	}

	switch(APP_GetScoState(pLinkInfo))
	{
		case SCO_STATE_ACTIVE_CONNECT:
			APP_SetScoState(pLinkInfo, SCO_STATE_DISCONNECTING_WAIT_SEND_REQ);
			break;
		case SCO_STATE_PASSIVE_CONNECT:
			APP_SetScoState(pLinkInfo, SCO_STATE_DISCONNECTING_WAIT_SEND_REQ);
			break;
		case SCO_STATE_CONNECTED:
			APP_SetScoState(pLinkInfo, SCO_STATE_DISCONNECTING_WAIT_REQ_CPL);
			BtSyncDisconnectRequest((Handler)&gAppHfpSCOHandle, APP_GetSCOSinkByBdAddr(pBdAddr), 0x13);
			break;
		case SCO_STATE_DISC:
		case SCO_STATE_DISCONNECTING_WAIT_SEND_REQ:
		case SCO_STATE_DISCONNECTING_WAIT_REQ_CPL:
			break;
	}
}

void APP_HfpSco_DisconnectAllSco(void)
{
	U8 i;
	BD_ADDR_T *pBdAddr;

	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if((pBdAddr = APP_GetBdAddrByLinkIndex(i)))
			APP_HfpSco_DisconnectRequest(pBdAddr);
	}
}

void APP_HfpSco_SendDSPCmd(BD_ADDR_T *pBdAddr)
{
    AUDIO_DSP_START_PARA_STUR audioDspPara;

    if(!APP_SCO_EXISTS(pBdAddr))
        return;

    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
    FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
    audioDspPara.audioType = AUDIO_SCO;
    audioDspPara.codec = APP_GetScoCodec(pBdAddr);
    APP_AudioDspPlay(&audioDspPara);
}

void APP_HfpSco_DisconnectHandleByBdAddress(BD_ADDR_T *pBdAddr)
{
	BOOL isSCOExist;

	if(!pBdAddr)
	{
		DBG_LOG_APP_HFP( "[APP_HFP] BT_SYNC_DISC_IND fail - BdAddress Null", 0);
		return;
	}

	isSCOExist = (APP_SCO_EXISTS(pBdAddr)) ? TRUE : FALSE;
	app_HfpSCO_DiscHandle(pBdAddr);

	if(isSCOExist)
	{
		app_HfpSCO_DiscProfileHandle(pBdAddr);
	}
	else
	{
		APP_SetHfpLinkAudioTransfer(pBdAddr, FALSE);
	}
}

U8 APP_HfpSco_IsSupportCodecWideBand(BD_ADDR_T* pBdAddr)
{
	return (APP_Hfp_GetSupportCodec(pBdAddr) != SCO_CODEC_CVSD);
}


void APP_HfpSco_SetEcnr(BOOL isEcnrOff)
{
    drv_sco_set_ecnr_off(isEcnrOff);
}

void APP_HfpSco_EnableLightMonoMode(BD_ADDR_T *pBdAddr)
{
	drv_sco_set_light_mono_mode(pBdAddr, TRUE);
}

void APP_HfpSco_DisableLightMonoMode(BD_ADDR_T *pBdAddr)
{
	drv_sco_set_light_mono_mode(pBdAddr, FALSE);
}

