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

#ifdef XIAOAI_DEVELOPMENT

#include "bt_xiaoai.h"
#include "App_PowerOff.h"
#include "App_Xiaoai.h"
#include "Power_NvkeyHandler.h"
#include "PM_Interface.h"
#include "App_Connection.h"
#include "AudioDSP_StreamManager.h"
#include "BtMCSync.h"
#include "App_EventOpcode.h"
#include "App_EventKeyHandler.h"
#include "App_XiaoaiAdv.h"
#include "App_Xiaoai_VendorHandler.h"
#include "App_Ble.h"
#include "App_RhoHandler.h"
#include "App_MCSync_RHO.h"
#include "App_LinkKey.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/


typedef struct
{
	APP_XIAOAI_LINK_INFO_T linkInfo[FW_MAX_LINK_IDX];
}APP_XIAOAI_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_XiaoaiHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static BOOL app_XiaoaiGattServerReady(void);
static BOOL app_XiaoaiEnable(void);
static BOOL app_XiaoaiDisable(void);

/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static HandlerData gAppXiaoaiHandle = { app_XiaoaiHandler };
static APP_XIAOAI_STRU gAppXiaoaiCtl;

static const APP_BLE_ADV_HANDLE_DESC gAppXiaoaiDesc =
{
    BLE_ADV_XIAOAI_MASK,
    app_XiaoaiGattServerReady,
    app_XiaoaiEnable,
    app_XiaoaiDisable,    
};

static APP_BLE_ADV_HANDLE_NODE gAppXiaoaiNode = {
    &gAppXiaoaiDesc,
    NULL,
};

log_create_module(APP_XIAOAI, PRINT_LEVEL_INFO);



/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static BOOL app_XiaoaiGattServerReady(void)
{
    return app_XiaoaiEnable();
}

static BOOL app_XiaoaiEnable(void)
{
    return APP_XiaoaiEnableAdv();
}

static BOOL app_XiaoaiDisable(void)
{
    return APP_XiaoaiDisableAdv();
}

static BOOL app_XiaoaiSetLink(BD_ADDR_T *pBdAddr)
{
	U8 i;
	
	if(!pBdAddr || FW_IsBdAddrZero(pBdAddr))
		return FALSE;
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(!FW_Memcmp(&gAppXiaoaiCtl.linkInfo[i].bdAddr, pBdAddr, sizeof(BD_ADDR_T)))
			return TRUE;
	}
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(FW_IsBdAddrZero(&gAppXiaoaiCtl.linkInfo[i].bdAddr))
		{
			FW_Memcpy(&gAppXiaoaiCtl.linkInfo[i].bdAddr, pBdAddr, sizeof(BD_ADDR_T));
			return TRUE;
		}
	}
	
	return FALSE;
}

static BOOL app_Xiaoai_ClearLink(BD_ADDR_T *pBdAddr)
{
	U8 i;
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(!FW_Memcmp(&gAppXiaoaiCtl.linkInfo[i].bdAddr, pBdAddr, sizeof(BD_ADDR_T)))
		{
			FW_Memset(&gAppXiaoaiCtl.linkInfo[i].bdAddr, 0, sizeof(BD_ADDR_T));
			return TRUE;
		}
	}
	
	return FALSE;
}

static APP_XIAOAI_LINK_INFO_T *APP_Xiaoai_GetLinkInfo(void)
{
	U8 i;
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(!FW_IsBdAddrZero(&gAppXiaoaiCtl.linkInfo[i].bdAddr))
			return &gAppXiaoaiCtl.linkInfo[i];
	}
	return (APP_XIAOAI_LINK_INFO_T *)NULL;
}

static APP_XIAOAI_LINK_INFO_T *APP_Xiaoai_GetEmptyLinkInfo(void)
{
	U8 i;
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(FW_IsBdAddrZero(&gAppXiaoaiCtl.linkInfo[i].bdAddr))
			return &gAppXiaoaiCtl.linkInfo[i];
	}
	return (APP_XIAOAI_LINK_INFO_T *)NULL;
}


/**************************************************************************************************
* Static Functions (Handler)
**************************************************************************************************/
static void app_XiaoaiStartServiceCfmHandler(xiaoai_start_service_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] StartServiceCfmHandler: status: %d", 1, cfm->status);
	
    if(cfm->status == BT_STATUS_SUCCESS)
    {
        xiaoai_set_adv_interval(0x64, 0x64);
    }
}

static void app_XiaoaiStopServiceCfmHandler(xiaoai_stop_service_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] StopServiceCfmHandler: status: %d", 1, cfm->status);
}

static void app_XiaoaiBleSetAdvParamsCfmHandler(xiaoai_ble_set_adv_params_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] BleSetAdvParamsCfmHandler: status: %d", 1, cfm->status);

	
    if(cfm->status == BT_STATUS_SUCCESS)
    {
		//U8* pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BLE_BDADDR);

		//#ifdef MIUI_FAST_PAIRING_FEAT
			//BleSetLocalRandomAddrReq(&LocalBleHandler, BLE_ADDR_WRITE_STATIC, (BD_ADDR_T *)pAddr);
			//BleSetLocalRandomAddrReq((Handler)&gAppAirAppHandler, BLE_ADDR_WRITE_STATIC, (BD_ADDR_T *)pAddr);
		//xiaoai_set_local_random_addr_req((xiaoai_bdaddr_t *)pAddr);
		//#else
			//BleSetLocalRandomAddrReq((Handler)&gAppXiaoaiHandle, BLE_ADDR_WRITE_STATIC, (BD_ADDR_T *)pAddr);
		//#endif
    }    
}

static void app_XiaoaiBleSetLocalRandomAddrCfmHandler(xiaoai_ble_set_local_random_addr_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI]BleSetLocalRandomAddrCfmHandler cfm_status:0x%x", 1, cfm->status);

    if(cfm->status == BT_STATUS_SUCCESS)
    {
    }    
}


U8 gRspData[14] = 
{
	0x0D, 0xFF,
	0x00, 0x04, //vid
	0x08, 0x03, 0x02,
	0x00, 0x94, //product id
	0x00, 0x00, 0x00, 0x00, //virtual addr
	0x00,//byte 13
};


static void app_XiaoaiBleSetAdvDataCfmHandler(xiaoai_ble_set_adv_data_cfm_t * cfm)
{
	BLE_EXT_SCAN_RSP_DATA_T rsp;
	
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] BleSetAdvDataCfmHandler cfm_status:0x%x", 1, cfm->status);
	
	//#if defined(VERSION_168) || defined(VERSION_MIAIR2)
	{
		U8 *pBda = (U8 *)BtAwsMce_ReadLocalBdAddr();
		U16 vid = xiaoai_nvkey_get_device_vid();
		U16 pid = xiaoai_nvkey_get_device_pid();
		
		gRspData[2] = (U8)(vid >> 8);
		gRspData[3] = (U8)vid;
		gRspData[7] = (U8)(pid >> 8);
		gRspData[8] = (U8)pid;
		
		DBG_LOG_APP_XIAOAI("[APP_XIAOAI] vid:0x%02x%02x, pid:0x%02x%02x", 4, gRspData[2], gRspData[3], gRspData[7], gRspData[8]);

		memcpy(&gRspData[9], &pBda[2], 4);
		rsp.sizeRspData = sizeof(gRspData);
		rsp.scanRspData = gRspData;
		BleSetExtendedScanResponseDataReq(&gAppXiaoaiHandle, BLE_ADV_XIAOAI, &rsp);
	}
	//#else
	//xiaoai_set_adv_enable_req(TRUE);
	//#endif
}

static void app_XiaoaiBleSetScanResponseDataCfmHandler(xiaoai_ble_set_scan_response_data_cfm_t *cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI]BleSetScanResponseDataCfmHandler cfm_status:0x%x", 1, cfm->status);
	//xiaoai_set_adv_enable_req(TRUE);
}

static void app_XiaoaiBleSetAdvEnableCfmHandler(xiaoai_ble_set_adv_enable_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI]BleSetAdvEnableCfmHandler cfm_status:0x%x", 1, cfm->status);
}

static void app_XiaoaiConnectCfmHandler(xiaoai_connect_cfm_t * cfm)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiConnectCfmHandler: status: %d", 1, cfm->status);
	
	if(cfm->status == 0) 
		app_XiaoaiSetLink((BD_ADDR_T *)&cfm->bdaddr);

	APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
}

static void app_XiaoaiDisconnectIndHandler(xiaoai_disconnect_ind_t * ind)
{
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiDisconnectIndHandler: status: %d", 1, ind->status);

    app_Xiaoai_ClearLink((BD_ADDR_T *)&ind->bdaddr);
    if(!APP_MCSync_Rho_IsRhoing())
    {
        APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
    }
}

static void app_XiaoaiDisconnectCfmHandler(xiaoai_disconnect_cfm_t * cfm)
{
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiDisconnectCfmHandler: status: %d", 1, cfm->status);
    app_Xiaoai_ClearLink((BD_ADDR_T *)&cfm->bdaddr);
    if(!APP_MCSync_Rho_IsRhoing())
    {
        APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
    }
}

static void app_XiaoaiSpeechStartCfmHandler(xiaoai_speech_start_cfm_t *cfm)
{
    AUDIO_DSP_START_PARA_STUR audioDspPara;

    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiSpeechStartCfmHandler: status:%d", 1, cfm->status);

    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
    FW_Memcpy(&audioDspPara.bdAddr, &cfm->bdaddr, sizeof(BD_ADDR_T));
    audioDspPara.bitRate = 16;

    audioDspPara.audioType = AUDIO_XIAOAI;
    //if(APP_AudioDeviceRegister(&ind->bdaddr, AUDIO_DEVICE_XIAOAI))
    {
        APP_AudioDspPlay(&audioDspPara);
    }
}

static void app_XiaoaiSpeechStopCfmHandler(xiaoai_speech_stop_cfm_t *cfm)
{
    AUDIO_DSP_STOP_PARA_STUR audioDspPara;
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiSpeechStopCfmHandler: status: %d", 1, cfm->status);

    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
    FW_Memcpy(&audioDspPara.bdAddr, &cfm->bdaddr, sizeof(BD_ADDR_T));
    audioDspPara.audioType = AUDIO_XIAOAI;
    APP_AudioDspStop(&audioDspPara);
}

static void app_XiaoaiSpeechStartIndHandler(xiaoai_speech_start_ind_t *ind)
{
    AUDIO_DSP_START_PARA_STUR audioDspPara;

    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiSpeechStartIndHandler", 0);

    FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
    FW_Memcpy(&audioDspPara.bdAddr, &ind->bdaddr, sizeof(BD_ADDR_T));
    audioDspPara.bitRate = 16;
    audioDspPara.audioType = AUDIO_XIAOAI;

    //if(APP_AudioDeviceRegister(&ind->bdaddr, AUDIO_DEVICE_XIAOAI))
    {
        APP_AudioDspPlay(&audioDspPara);
    }
    //open DSP
    xiaoai_send_start_speech_rsp(&ind->bdaddr, TRUE, ind->op_code_sn);
}

static void app_XiaoaiSpeechStopIndHandler(xiaoai_speech_stop_ind_t *ind)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiSpeechStopIndHandler", 0);
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, &ind->bdaddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_XIAOAI;	
    APP_AudioDspStop(&audioDspPara);

	xiaoai_send_stop_speech_rsp(&ind->bdaddr, TRUE, ind->op_code_sn);
}

static void app_XiaoaiDevRebootIndHandler(xiaoai_dev_reboot_ind_t *ind)
{
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiDevRebootIndHandler:%d", 1, ind->reboot_type);
    if(ind->reboot_type == XIAOAI_DEV_REBOOT_TYPE_RESTART)
    {
       APP_PowerOff_SetTimer(TIMER_XIAOAI_RESET);
    }
    else if(ind->reboot_type == XIAOAI_DEV_REBOOT_TYPE_POWER_OFF)
    {
        APP_PowerOff_SetTimer(TIMER_XIAOAI_POWER_OFF);
    }
}

static void app_XiaoaiReconnectEDRIndHandler(xiaoai_reconnect_edr_ind_t *ind)
{
    BD_ADDR_T *pBdAddr;

	if(FW_IsBdAddrZero((BD_ADDR_T *)&ind->bdaddr))
		pBdAddr = APP_LinkKey_GetNormalLinkBdAddr(0);
	else
		pBdAddr = (BD_ADDR_T *)&ind->bdaddr;
	
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiReconnectEDRIndHandler:BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));

	if((APP_INFO_STRU *)NULL == APP_GetAppLinkByBdAddr(pBdAddr))
    {
        PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
        PM_ConnectProfile(pBdAddr, PROFILE_HANDSFREE);
        PM_ConnectProfile(pBdAddr, PROFILE_AVRCP);
    }
}

static void app_XiaoaiDisconnectEDRIndHandler(xiaoai_disconnect_edr_ind_t *ind)
{
    UNUSED(ind);
    APP_Conn_ReleaseMultiLink();
}

static void app_XiaoaiEnterDiscoverableIndHandler(void)
{
    APP_EvtKey_KeyHandler(KEY_DISCOVERABLE);
}

static void app_XiaoaiDeletePairedDevIndHandler(xiaoai_delete_paired_dev_ind_t *ind)
{
    BDADDR_T addr;
    addr.type = 0;
    FW_Memcpy(&addr.addr, &ind->bdaddr, 6);
    BtSmDeleteAuthDevice((Handler)&gAppXiaoaiHandle, addr);     
}

static void app_XiaoaiSetDeviceInfo(BD_ADDR_T *pBdAddr, U8 opCodeSN, U8 attr_num, xiaoiai_attr_element_t *attr)
{
    for(U8 i = 0; i < attr_num ; i++)
    {
        DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiSetDeviceInfo, i:%d, attr:%d, setting:%d", 3, i, attr[i].xm_attr, attr[i].setting);
        
        switch(attr[i].xm_attr)
        {
            case XM_ATTR_DEVICE_POWER_MODE:
            
                break;
            case XM_ATTR_DEVICE_FUNC_KEY:
            
                break;
            case XM_ATTR_DEVICE_HOTWORD:
            
                break;
            case XM_ATTR_DEVICE_SUP_POWER_SAVE_NEW:
            
                break;
            case XM_ATTR_DEVICE_ANC_MODE:
            
                break;
        }
    }
	xiaoai_core_send_rsp_set_device_info((xiaoai_bdaddr_t *)pBdAddr, XIAOAI_RCSP_STATUS_SUCCESS,opCodeSN);
}

static void app_XiaoaiGetDeviceInfo(BD_ADDR_T *pBdAddr, U8 opCodeSN, U8* ptr, U16 dataLen)
{
	xiaoai_core_send_rsp_get_device_info((xiaoai_bdaddr_t *)pBdAddr, XIAOAI_RCSP_STATUS_SUCCESS, opCodeSN, ptr, dataLen);
}

static void app_XiaoaiGetDeviceRunInfo(BD_ADDR_T *pBdAddr, U8 opCodeSN, U8* ptr, U16 dataLen, U32 mask)
{
	UNUSED(mask);
	xiaoai_core_send_rsp_get_device_run_info((xiaoai_bdaddr_t *)pBdAddr, XIAOAI_RCSP_STATUS_SUCCESS, opCodeSN, ptr, dataLen);
}

static U32 app_XiaoaiHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiHandler: id:0x%x", 1, id);
	switch(id)
	{
		//CONNECTION
		case XIAOAI_START_SERVICE_CFM:
			app_XiaoaiStartServiceCfmHandler((xiaoai_start_service_cfm_t *)msg);
			break;

		case XIAOAI_STOP_SERVICE_CFM:
			app_XiaoaiStopServiceCfmHandler((xiaoai_stop_service_cfm_t *)msg);
			break;
		
		case XIAOAI_CONNECT_CFM:
			app_XiaoaiConnectCfmHandler((xiaoai_connect_cfm_t *)msg);
			break;
		
		case XIAOAI_DISCONNECT_IND:
			app_XiaoaiDisconnectIndHandler((xiaoai_disconnect_ind_t *)msg);
			break;
		
		case XIAOAI_DISCONNECT_CFM:
			app_XiaoaiDisconnectCfmHandler((xiaoai_disconnect_cfm_t *)msg);
			break;

		//Speech
		case XIAOAI_SPEECH_START_CFM:
			app_XiaoaiSpeechStartCfmHandler((xiaoai_speech_start_cfm_t *)msg);
			break;
		
		case XIAOAI_SPEECH_STOP_CFM:
			app_XiaoaiSpeechStopCfmHandler((xiaoai_speech_stop_cfm_t *)msg);
			break;
		
		case XIAOAI_SPEECH_START_IND:
			app_XiaoaiSpeechStartIndHandler((xiaoai_speech_start_ind_t *)msg);
			break;
		
		case XIAOAI_SPEECH_STOP_IND:
			app_XiaoaiSpeechStopIndHandler((xiaoai_speech_stop_ind_t *)msg);
			break;

        case XIAOAI_BLE_SET_ADV_PARAMS_CFM:
            app_XiaoaiBleSetAdvParamsCfmHandler((xiaoai_ble_set_adv_params_cfm_t *)msg);
            break;
            
        case XIAOAI_BLE_SET_LOCAL_RANDOM_ADDR_CFM:
            app_XiaoaiBleSetLocalRandomAddrCfmHandler((xiaoai_ble_set_local_random_addr_cfm_t *)msg);
            break;

        case XIAOAI_BLE_SET_ADV_DATA_CFM:
            app_XiaoaiBleSetAdvDataCfmHandler((xiaoai_ble_set_adv_data_cfm_t *)msg);
            break;
		
		case BT_BLE_SET_SCAN_RESPONSE_DATA_CFM: //need change to xiaoai
            app_XiaoaiBleSetScanResponseDataCfmHandler((xiaoai_ble_set_scan_response_data_cfm_t *)msg);
            break;
            
        case XIAOAI_BLE_SET_ADV_ENABLE_CFM:
            app_XiaoaiBleSetAdvEnableCfmHandler((xiaoai_ble_set_adv_enable_cfm_t *)msg);
            break;
            
        case XIAOAI_DEV_REBOOT_IND:
            app_XiaoaiDevRebootIndHandler((xiaoai_dev_reboot_ind_t *)msg);
            break;
            
        case XIAOAI_RECONNECT_EDR_IND:
            app_XiaoaiReconnectEDRIndHandler((xiaoai_reconnect_edr_ind_t *)msg);
            break;
            
        case XIAOAI_DISCONNECT_EDR_IND:
            app_XiaoaiDisconnectEDRIndHandler((xiaoai_disconnect_edr_ind_t *)msg);
            break;
            
        case XIAOAI_ENTER_DISCOVERABLE_IND:
            app_XiaoaiEnterDiscoverableIndHandler();
            break;
            
        case XIAOAI_DELETE_PAIRED_DEV_IND:
            app_XiaoaiDeletePairedDevIndHandler((xiaoai_delete_paired_dev_ind_t *)msg);
            break;

		//VP ind
		case XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI: //請打開小愛同學手機客戶端
			DBG_LOG_APP_XIAOAI("[APP_XIAOAI] XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI", 0);
			break;
		
		case XIAOAI_VP_IND_ENABLING_XIAOAI: //正在喚醒小愛同學
			DBG_LOG_APP_XIAOAI("[APP_XIAOAI] XIAOAI_VP_IND_ENABLING_XIAOAI", 0);
			break;
		
		case XIAOAI_VP_IND_PLEASE_CONNECT_EDR: //藍芽未連接 請通過手機藍芽連接我吧
			DBG_LOG_APP_XIAOAI("[APP_XIAOAI] XIAOAI_VP_IND_PLEASE_CONNECT_EDR", 0);
			break;
		
		case XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI_AND_CONNECT: //請打開小愛同學手機客戶端進行設備連接
			DBG_LOG_APP_XIAOAI("[APP_XIAOAI] XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI_AND_CONNECT", 0);
			break;

		case XIAOAI_VENDOR_CMD_IND:
			App_Xiaoai_VendorHandler((BD_ADDR_T *)(&((xiaoiai_msg_send_vendor_cmd_ind_t *)msg)->bdaddr), ((xiaoiai_msg_send_vendor_cmd_ind_t *)msg)->op_code_sn, (U8 *)(&((xiaoiai_msg_send_vendor_cmd_ind_t *)msg)->data), ((xiaoiai_msg_send_vendor_cmd_ind_t *)msg)->data_length); 
			break;

		case XIAOAI_SET_DEVICE_INFO_IND:
			app_XiaoaiSetDeviceInfo((BD_ADDR_T *)(&((xiaoiai_set_device_info_t *)msg)->bdaddr), ((xiaoiai_set_device_info_t *)msg)->op_code_sn, ((xiaoiai_set_device_info_t *)msg)->attr_num, &((xiaoiai_set_device_info_t *)msg)->attr[0]);
			break;

		case XIAOAI_GET_DEVICE_INFO_IND:
			app_XiaoaiGetDeviceInfo((BD_ADDR_T *)(&((xiaoiai_get_device_info_t *)msg)->bdaddr), ((xiaoiai_get_device_info_t *)msg)->op_code_sn, (U8 *)(&((xiaoiai_get_device_info_t *)msg)->data), ((xiaoiai_get_device_info_t *)msg)->data_length); 
			break;

		case XIAOAI_GET_DEVICE_RUN_INFO_IND:
			app_XiaoaiGetDeviceRunInfo((BD_ADDR_T *)(&((xiaoiai_get_device_run_info_t *)msg)->bdaddr), ((xiaoiai_get_device_run_info_t *)msg)->op_code_sn, (U8 *)(&((xiaoiai_get_device_run_info_t *)msg)->data), ((xiaoiai_get_device_run_info_t *)msg)->data_length, ((xiaoiai_get_device_run_info_t *)msg)->mask); 
			break;
		default:
			DBG_LOG_APP_XIAOAI("[APP_XIAOAI] app_XiaoaiHandler:unknown id :0x%x", 1, id);
			break;
	}

	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_Xiaoai_Init(void)
{
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI]APP_Xiaoai_Init", 0);
	APP_Xiaoai_RegisterProfile();
	return TRUE;
}

void APP_Xiaoai_RegisterProfile(void)
{
    APP_Ble_RegisterAdvHandle(&gAppXiaoaiNode);
	if (XIAOAI_STATUS_OK != xiaoai_start_service((xiaoai_handler)&gAppXiaoaiHandle, 1))
	{
        DBG_LOG_APP_XIAOAI("[APP_XIAOAI] StartService fail!", 0);
	}
	//U8* ptr;
	
	//ptr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_BLE_BLE_ADV_ENABLE);
	
	//if((*ptr == FALSE) || !ptr)
	//{
	//	 MSG_MessageSendLater((Handler)&gAirAppStartGATTServerHandler,0,NULL,200);
	//}
}

BD_ADDR_T *APP_XiaoaiGetLink(void)
{
	U8 i;
	
	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if(!FW_IsBdAddrZero(&gAppXiaoaiCtl.linkInfo[i].bdAddr))
			return &gAppXiaoaiCtl.linkInfo[i].bdAddr;
	}
	
	return NULL;
}

void APP_Xiaoai_SetPartnerVersion(U16 version)
{
    xiaoai_set_remote_device_version(version);
}

void APP_Xiaoai_MCSyncConnectedCfmHandler(void)
{
    if(BtAwsMce_IsDefaultRolePartner())
    {
        BtMCSync_SendSyncXiaoaiDataToAgent(xiaoai_get_local_device_version());
    }
    else if(BtAwsMce_IsDefaultRoleAgent())
    {
        xiaoai_send_tws_status_cmd(TWS_CONNECTED);
    }
}

void APP_Xiaoai_MCSyncDisconnectedIndHandler(void)
{
    if(BtAwsMce_IsDefaultRoleAgent())
    {
        xiaoai_send_tws_status_cmd(TWS_DISCONNECTED);
        if(APP_PowerOff_IsPowerOn())
        {
            xiaoai_send_tws_status_cmd(TWS_CONNECTING);
        }
    }    
}

void APP_Xiaoai_ACLOpenIndHandler(BD_ADDR_T *pBdAddr)
{
    xiaoai_send_edr_status_cmd((xiaoai_bdaddr_t *)pBdAddr, EDR_STATUS_ACL_OPEN);
}

void APP_Xiaoai_ACLCloseIndHandler(BD_ADDR_T *pBdAddr)
{
    xiaoai_send_edr_status_cmd((xiaoai_bdaddr_t *)pBdAddr, EDR_STATUS_ACL_CLOSED);
}

void APP_Xiaoai_ReleaseLinkHandler(void)
{
    xiaoai_send_notify_f2a_bt_op_cmd(BT_OP_DISCONNECT_EDR);
}

void APP_Xiaoai_ReconnectHandler(void)
{
    xiaoai_send_notify_f2a_bt_op_cmd(BT_OP_CONNECT_EDR);
}

BOOL APP_Xiaoai_IsConnected(void)
{
	return (APP_XiaoaiGetLink() == NULL) ? FALSE : TRUE;
}

/**************************************************************************************************
* Public API (RHO)
**************************************************************************************************/
BOOL APP_Xiaoai_FillRhoData(void *pData)
{
	APP_XIAOAI_LINK_INFO_T *pLinkInfo;
	APP_RHO_XIAOAI_STRU *pDataToFill = &((APP_RHO_DATA_STRU *)pData)->appXiaoai;

	if(!pDataToFill)
	{
		DBG_LOG_APP_XIAOAI("[APP_XIAOAI] Fill RHO data error", 0);
		return FALSE;
	}

	pLinkInfo = APP_Xiaoai_GetLinkInfo();
	if(pLinkInfo)
	{
		FW_Memcpy(&pDataToFill->linkInfo, pLinkInfo, sizeof(APP_XIAOAI_LINK_INFO_T));
	}
    else
    {
        FW_Memset(&pDataToFill->linkInfo, 0, sizeof(APP_XIAOAI_LINK_INFO_T));
    }

	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] Fill RHO data OK", 0);
	return TRUE;
}

BOOL APP_Xiaoai_AssignRhoData(void *pData)
{
	APP_XIAOAI_LINK_INFO_T *pLinkInfo;
	APP_RHO_XIAOAI_STRU *pDataToGet = &((APP_RHO_DATA_STRU *)pData)->appXiaoai;

	if(!pDataToGet)
	{
		DBG_LOG_APP_XIAOAI("[APP_XIAOAI] Assign RHO data error", 0);
		return FALSE;
	}

	pLinkInfo = APP_Xiaoai_GetEmptyLinkInfo();
	if(!pLinkInfo)
	{
		DBG_LOG_APP_XIAOAI("[APP_XIAOAI] Assign RHO empty link", 0);
		return FALSE;
	}
	FW_Memcpy(pLinkInfo, &pDataToGet->linkInfo, sizeof(APP_XIAOAI_LINK_INFO_T));
    
    pLinkInfo = APP_Xiaoai_GetLinkInfo();
    if(pLinkInfo == (APP_XIAOAI_LINK_INFO_T *)NULL)
    {
        APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
    }

	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] Assign RHO data OK", 0);
	return TRUE;
}

void APP_Xiaoai_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, pOldAgentAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_XIAOAI;	
    APP_AudioDspStop(&audioDspPara);

    app_Xiaoai_ClearLink(pOldAgentAddr);
    UNUSED(pNewAgentAddr);
    
    APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
   
    DBG_LOG_APP_XIAOAI("[APP_XIAOAI] NewPartner OK", 0);
}

#endif //XIAOAI_DEVELOPMENT

