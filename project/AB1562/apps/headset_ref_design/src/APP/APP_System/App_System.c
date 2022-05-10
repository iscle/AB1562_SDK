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
#include "App_System.h"
#include "App_KeyManage.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "App_PowerOn.h"
#include "App_PowerOff.h"
#include "App_Pairing.h"
#include "Pairing_NvkeyHandler.h"
#include "App_ResourceManager.h"
#include "App_A2dp.h"
#include "App_Avrcp.h"
#include "App_Hfp.h"
#include "App_A2dp.h"
#include "App_Avrcp.h"
#include "App_Connection.h"
#include "App_State.h"
#include "App_SniffHandler.h"
#include "App_EventKeyHandler.h"
#include "App_Nvram_Type.h"
#include "System_Nvkey.h"
#include "assert.h"
#include "Hfp_atCommand.h"
#include "App_ReConnection.h"
#include "APP_HfpScoHandler.h"
#include "App_LinkKey.h"
#include "System_Nvkey.h"
#include "App_Connection_NVKeyHandler.h"
#include "APP_AirApp.h"
#include "APP_Qos.h"
#include "app_reset_to_factory_nvstruc.h"
#include "app_system_information_nvstruc.h"
#include "nvkey_hdl.h"
#include "PM_System.h"
#include "PM_Nvkey.h"
#include "DeviceID.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#include "BtAma.h"
#endif
#include "syslog.h"
#ifdef AIROHA_BT_SPP_ENABLE
#include "APP_CustApp.h"
#endif
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
#include "App_Gfp.h"
#include "gfps_api.h"
#endif
#ifdef PROFILE_SWIFT_ENABLE
#include "App_Swift.h"
#endif
=======
#ifdef AIR_GFP_ENABLE
#include "App_Gfp.h"
#include "gfps_api.h"
#endif
#ifdef AIR_SWIFT_ENABLE
#include "App_Swift.h"
#endif
#ifdef AIR_GATT_OVER_BREDR_ENABLE
#include "bt_gatt.h"
#endif
>>>>>>> db20e11 (second commit)

log_create_module(APP_SYSTEM, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 isMpTestMode;
	bool isConDisNeed;
}APP_SYSTEM_STRU;

extern void APP_Conn_ReleaseQosTimer(APP_INFO_STRU * pLinkInfo);
/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_System_RegisterProfiles(void);
static void app_System_SetSecurity(U8 profileId);
static void app_System_SetServiceSecurity(void);
static void app_System_NvkeyInit(void);
static U32 app_System_KeyHandler(Handler t, U16 id, void *payload, U32 id_ext);
static void app_System_InitCfmHandler(BT_INIT_CFM_T * cfm);
static void app_System_DeInitCfmHandler(BT_DEINIT_CFM_T * cfm);
static void app_System_WriteLocalNameCfmHandler(BT_WRITE_LOCAL_NAME_CFM_T * cfm);
static void app_System_WriteCODCfmHandler(BT_WRITE_CLASS_OF_DEVICE_CFM_T * cfm);
static void app_System_WriteEIRDataCfmHandler(BT_WRITE_EIR_DATA_CFM_T * cfm);
static void app_System_AclOpenedIndHandler(BT_ACL_OPENED_IND_T * ind);
static void app_System_AclClosedIndHandler(BT_ACL_CLOSED_IND_T * ind);
static void app_System_IOCapabilityReqIndHandler(BT_SM_IO_CAPABILITY_REQ_IND_T * ind);
static void app_System_RemoteIOCapabilityIndHandler(BT_SM_REMOTE_IO_CAPABILITY_IND_T * ind);
static void app_System_PinCodeIndHandler(BT_SM_PIN_CODE_IND_T * ind);
static void app_System_UserConfirmationReqIndHandler(BT_SM_USER_CONFIRMATION_REQ_IND_T * ind);
static void app_System_UserPasskeyReqIndHandler(BT_SM_USER_PASSKEY_REQ_IND_T * ind);
static void app_System_SetServiceConfigCfmHandler(BT_SM_SET_SERVICE_CONFIG_CFM_T * cfm);
static void app_System_SetSniffSubratingCfmHandler(BT_SET_SNIFF_SUBRATING_CFM_T * cfm);
static void app_System_SetModeCfmHandler(BT_SET_MODE_CFM_T * cfm);
static void app_System_ModeChangeIndHandler(BT_MODE_CHANGE_IND_T * ind);
static U32 app_SystemHandler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_SYSTEM_STRU gAppSystemCtl;

static const HandlerData gAppSystemHandle = { app_SystemHandler };
static const HandlerData gSystemKeyHandler = {app_System_KeyHandler};

/**************************************************************************************************
* Static Functions (Init Handler)
**************************************************************************************************/
static void app_System_RegisterProfiles(void)
{
	APP_Conn_Register();
<<<<<<< HEAD
	#ifdef A2DP_Profile
	APP_A2dp_RegisterProfile();
	#endif
	#ifdef AVRCP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
	APP_A2dp_RegisterProfile();
	#endif
	#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	APP_Avrcp_RegisterProfile();
	#endif
	#ifdef HFP_Profile
	APP_Hfp_RegisterProfile();
	#endif
    #ifdef AIRAPP_Profile
    APP_AirApp_RegisterProfile();
    #endif
    #ifdef PROFILE_AMA_ENABLE
    APP_Ama_RegisterProfile();
    #endif
<<<<<<< HEAD
    #ifdef PROFILE_GFP_ENABLE
=======
    #ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
	APP_Gfp_RegisterProfile();
    #endif
    #ifdef AIROHA_BT_SPP_ENABLE
    APP_CustApp_RegisterProfile();
    #endif
<<<<<<< HEAD
=======

    #ifdef AIR_GATT_OVER_BREDR_ENABLE
    ATT_Enable_EDRBrearer();
    #endif

>>>>>>> db20e11 (second commit)
	device_id_start_service();
}

static void app_System_SetSecurity(U8 profileId)
{
	BT_SECURITY_SERVICE_CONFIG_T sec;

	sec.mode4_level = 2;
	sec.mode2_attr.MODE2_AHTHENTICATION = TRUE;
	sec.mode2_attr.MODE2_AHTHORIZATION = FALSE;
	sec.mode2_attr.MODE2_ENCRYPTION = TRUE;
	sec.protocol.protocol = 0;

	switch(profileId)
	{
		case PROFILE_A2DP:
			sec.protocol.psm = 0x19;
			break;

		case PROFILE_AVRCP:
			sec.protocol.psm = 0x17;
			break;
	}

	sec.IsIncomming = FALSE;
	BtSmSetServiceSecurityConfig((Handler)&gAppSystemHandle, sec);
	sec.IsIncomming = TRUE;
	BtSmSetServiceSecurityConfig((Handler)&gAppSystemHandle, sec);
}

static void app_System_SetBleSecurity(void)
{
    SYSTEM_INFORMATION_STRU *pInfo;
    BLE_SECURITY_SERVICE_CONFIG_T bleSecCfg;
    U8 size;
    pInfo = (SYSTEM_INFORMATION_STRU *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_SYSTEM_INFO_CONFIG);
	size = NVKEY_GetPayloadLength(NVKEYID_APP_SYSTEM_INFO_CONFIG);
    if (pInfo && size)
    {
        bleSecCfg.mode = pInfo->bleSecurityMode;
        bleSecCfg.level = pInfo->bleSecurityLevel;
    }
    else
    {
        FW_Memset(&bleSecCfg, 0, sizeof(BLE_SECURITY_SERVICE_CONFIG_T));
    }
    BleSmSetServiceSecurityConfig((Handler)&gAppSystemHandle, bleSecCfg);
}

static void app_System_SetServiceSecurity(void)
{
	app_System_SetSecurity(PROFILE_A2DP);
	app_System_SetSecurity(PROFILE_AVRCP);
    app_System_SetBleSecurity();
}

static void app_System_NvkeyInit(void)
{
	NVKEY_ENALBE_MPTEST_MODE_STRU *pControl = (NVKEY_ENALBE_MPTEST_MODE_STRU *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_ENABLE_MP_TEST_MODE);

	if(pControl)
	{
		gAppSystemCtl.isMpTestMode = pControl->isMpTestMode;
		if(gAppSystemCtl.isMpTestMode)
			DBG_LOG_APP_SYSTEM( "[APP_SYS] MP test mode ENABLED", 0);
	}

	BtSmSaveLinkKeyRecord((gAppSystemCtl.isMpTestMode) ? FALSE : TRUE);
	gAppSystemCtl.isConDisNeed = FALSE;
}

/**************************************************************************************************
* Static Functions (Message Handler)
**************************************************************************************************/
static U32 app_System_KeyHandler(Handler t, U16 id, void *payload, U32 id_ext)
{
    UNUSED(t);
    UNUSED(payload);
    APP_EvtKey_KeyEvent(id_ext, id);

    return 0;
}

static void app_System_InitCfmHandler(BT_INIT_CFM_T * cfm)
{
    DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_INIT_CFM %s", 1, cfm->status ? fail_log_string : success_log_string);

    if (!cfm->status)
    {
		APP_System_SetBtServiceClass();
		app_System_RegisterProfiles();
		app_System_SetServiceSecurity();

        if(!APP_PowerOff_IsPowerOn())
        {
		    APP_State_SetAutoPowerOnTimer();
        }
     }
}

static void app_System_DeInitCfmHandler(BT_DEINIT_CFM_T * cfm)
{
    DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_DEINIT_CFM %s, %d", 2, cfm->status ? fail_log_string : success_log_string, cfm->status);

    if (!cfm->status)
    {
        //BtWriteScanEnable(&gAppSystemHandle, (SCAN_ENABLE_T)SCAN_ENABLE_OFF);
    }
}

static void app_System_WriteLocalNameCfmHandler(BT_WRITE_LOCAL_NAME_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_WRITE_LOCAL_NAME_CFM %s", 1, cfm->hci_status ? fail_log_string : success_log_string);
}

static void app_System_WriteCODCfmHandler(BT_WRITE_CLASS_OF_DEVICE_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_WRITE_CLASS_OF_DEVICE_CFM %s", 1, cfm->hci_status ? fail_log_string : success_log_string);
}

static void app_System_WriteEIRDataCfmHandler(BT_WRITE_EIR_DATA_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_WRITE_EIR_DATA_CFM %s", 1, cfm->hci_status ? fail_log_string : success_log_string);
}

static void app_System_AclOpenedIndHandler(BT_ACL_OPENED_IND_T * ind)
{
	U8 linKeyIndex;
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_ACL_OPENED_IND %d: 0x%x%x", 3, ind->status, FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, TRUE), FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, FALSE));

	if(ind->status == HCI_ERCODE_SUCCESS)
	{
		linKeyIndex = APP_LinkKey_SearchLinkKeyIndex((BD_ADDR_T *)&ind->bdaddr, TRUE);
		if(APP_SetAppLinkBdAddr((BD_ADDR_T *)&ind->bdaddr))
		{
			APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr((BD_ADDR_T *)&ind->bdaddr);

			if(pLinkInfo == (APP_INFO_STRU *)NULL)
				return;

			APP_LinkKey_SetLinkSoundLevelByHistory((BD_ADDR_T *)&ind->bdaddr);
			pLinkInfo->scoCtl.scoCodec = SCO_CODEC_CVSD;
			APP_SetACLState(pLinkInfo, ACL_STATE_ON);
			{
				if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
				{
					gAppSystemCtl.isConDisNeed = TRUE;
				}
				APP_Pairing_ConnectableStateChangeWhenConnected();
			}

			if(0 == linKeyIndex) //IOT S10 & S9workaround
			{
                PM_ConnectionConflictStateMachine((BD_ADDR_T *)&ind->bdaddr, PM_CONN_CONFLICT_ACL_OPEN_EVT);
			}
		}
		//To speed up connection & disconnection process, we should avoid
		//sending Sniff_Req to remote AG if ACL is ready but SLC is not.
		//Slow connection problem appears very often when using smart phone
		//(e.g. HTC Desire or Wildfire) to connect with our system right after
		//disconnection of SLC when ACL still exists.
		App_SniffDisable((BD_ADDR_T *)&ind->bdaddr, APP_SNIFF_ACL_READY_SLC_NOT_YET);
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[APP_SYS] BT ACL OPEN FAIL", 0);
	}
}

static void app_System_AclClosedIndHandler(BT_ACL_CLOSED_IND_T * ind)
{
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)&ind->bdaddr;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_ACL_CLOSED_IND %d: 0x%x%x", 3, ind->reason, FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, TRUE), FW_bdaddr_to_2U32((BD_ADDR_T *)ind->bdaddr, FALSE));

	if(pLinkInfo != (APP_INFO_STRU *)NULL)
	{
		APP_SetACLState(pLinkInfo, ACL_STATE_OFF);
		if(APP_PowerOff_IsPowerOn()) // Not Power off
		{
			PM_ConnectionConflictStateMachine(pBdAddr, PM_CONN_CONFLICT_ACL_CLOSE_EVT);

			if(CURRENT_ACTIVE_LINK_CNT < FW_MAX_LINK_IDX)
			{
				if(!(APP_NO_CONNECTABLE_IN_LINE_IN_FEAT && APP_State_CheckInLineInState()))
				{
<<<<<<< HEAD
=======
                    if (APP_IsKeepDiscoverableAfterDisc() &&
                        1 >= CURRENT_ACTIVE_LINK_CNT &&
                        HCI_ERCODE_CONNECTION_TERMINATED_BY_LOCAL_HOST == ind->reason)
                    {
                        APP_Pairing_EnterDiscoverable();
                    }
#ifdef AIR_GFP_ENABLE
                    else if (gfp_is_enabled() && !APP_PairNvkey_IsFeatureOn(APP_DISCOVER_MODE_AFTER_DISCONNECT_FEAT))
                    {
                        APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
                    }
#endif
>>>>>>> db20e11 (second commit)
					APP_State_ConnectabilityHandle(CMD_SET_CONNECTABLE);
				}

				if(APP_PairNvkey_IsFeatureOn(APP_DISCOVER_MODE_AFTER_DISCONNECT_FEAT))
				{
					if(CURRENT_ACTIVE_LINK_CNT == 0)
					{
						APP_Pairing_EnterDiscoverable();
					}
					else
					{
						APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
					}
				}
			}

<<<<<<< HEAD
=======
            APP_SetKeepDiscoverableAfterDisc(FALSE);

>>>>>>> db20e11 (second commit)
			if(APP_IsLinkActiveDisconnecting(pBdAddr) && ind->reason != HCI_ERCODE_CONNECTION_TIMEOUT)
			{
				if(!CURRENT_ACTIVE_LINK_CNT)
				{
					if(APP_IsEnterDiscoverableAfterDisc())
					{
						APP_Pairing_EnterDiscoverable();
						APP_SetEnterDiscoverableAfterDisc(FALSE);
					}
				}
			}
			else
			{
				// Out-of-Range Case [Link loss]
				if(ind->reason == HCI_ERCODE_LMP_RESPONSE_TIMEOUT || ind->reason == HCI_ERCODE_CONNECTION_TIMEOUT)
				{
					if(APP_PairNvkey_IsFeatureOn(APP_DO_NOT_DISCO_DURING_LINK_LOSS_FEAT))
					{
						APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
					}
				}
			}
		}
		APP_LinkKey_SaveHistoryWhenDisconnect(pBdAddr);
		APP_HfpSco_DisconnectHandleByBdAddress(pBdAddr);
		APP_ReConn_DeleteListByBdAddr(pBdAddr);
		APP_Conn_ReleaseQosTimer(pLinkInfo);
		APP_ClearAppLinkBdAddr(pBdAddr);
	}

	switch(ind->reason)
	{
		case HCI_ERCODE_CONNECTION_TIMEOUT:
		case HCI_ERCODE_LMP_RESPONSE_TIMEOUT:
			pLinkInfo->linkPara.miscMask = APP_LINK_LOSS;
			if(APP_AUTO_RECONN_AFTER_LINK_LOSS_FEAT)
			{
				APP_Reconn_SetLinkLossBdAddr(pBdAddr);
				APP_Reconn_SetDelayTimer(RECONNECT_LINK_LOSS, App_ReConnNvkey_GetReconnectLinkLossTimer() * HALF_SEC);
			}
			break;
		case HCI_ERCODE_PIN_OR_KEY_MISSING:
			APP_ReConn_DeleteListByBdAddr(pBdAddr);
			APP_LinkKey_DeleteLinkHistory(pBdAddr);
			if(APP_AirApp_FOTA_Reconnect())
			{
				APP_AirApp_FOTA_DisableReconnect();
			}
			if(gAppSystemCtl.isConDisNeed == TRUE)
			{
				APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
			}
			break;
	}

	gAppSystemCtl.isConDisNeed = FALSE;
}

static void app_System_IOCapabilityReqIndHandler(BT_SM_IO_CAPABILITY_REQ_IND_T * ind)
{
	SYSTEM_INFORMATION_STRU *pAddr;
	U8 dataSize;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr((BD_ADDR_T *)&ind->bd_addr);

	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_IO_CAPABILITY_REQ_IND", 0);

	pAddr = (SYSTEM_INFORMATION_STRU*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_SYSTEM_INFO_CONFIG);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_SYSTEM_INFO_CONFIG);
	if(pAddr == NULL || !dataSize)
	{
        if (ind->isBle)
        {
            BLE_SM_IO_CAPABILITY_REQUEST_RSP_T rsp;
            rsp.io_capability = IO_CAP_NOINPUT_OUTPUT;
            rsp.oob_data = BLE_SM_OOB_DATA_NOT_PRESENTED;
            rsp.auth_req = BLE_SM_AUTH_REQ_NO_BONDING;
            rsp.key_distribution = 0;
            BleSmIoCapabilityResponse(ind->bd_addr, &rsp);
        }
        else
        {
		    BtSmIoCapabilityResponse(ind->bd_addr, IO_CAP_NOINPUT_OUTPUT, FALSE, TRUE, OOB_DATA_NOT_PRESERNT, 0);
        }
        return;
	}

	if(pLinkInfo)
	{
		if(pLinkInfo->linkPara.ioCtl.isIoCapRespSet)
		{
			pLinkInfo->linkPara.ioCtl.isIoCapRespSet = FALSE;
			if(	pLinkInfo->linkPara.ioCtl.remoteIoCap == IO_CAP_DISPLAY_YES_NO &&
				pLinkInfo->linkPara.ioCtl.remoteMitm &&
				pLinkInfo->linkPara.ioCtl.remoteBonding == 0x02)
			{
				BtSmIoCapabilityResponse(ind->bd_addr, pLinkInfo->linkPara.ioCtl.remoteIoCap, pLinkInfo->linkPara.ioCtl.remoteMitm,
										pLinkInfo->linkPara.ioCtl.remoteBonding, pLinkInfo->linkPara.ioCtl.remoteOOBDataPresent, pAddr->keyDistribution);
			}
			else
			{
				goto SEND_DEFAULT_IOCAP;
			}
		}
		else
		{
			SEND_DEFAULT_IOCAP:

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
			if (GFPSv2_is_processing()) {
				// GFPSv2 - set IO_CAP_DISPLAY_YES_NO for Google Fast Pair
				pAddr->ioCapability = IO_CAP_DISPLAY_YES_NO;
			}
#endif
			BtSmIoCapabilityResponse(ind->bd_addr, pAddr->ioCapability, FALSE, TRUE, pAddr->oobDataType, pAddr->keyDistribution);
		}
	}
	else
	{
        if (ind->isBle)
        {
            BLE_SM_IO_CAPABILITY_REQUEST_RSP_T rsp;

            rsp.io_capability = pAddr->bleIoCapbility;
            rsp.oob_data = pAddr->bleOobDataType;
            rsp.auth_req = pAddr->bleAuthReq;
            rsp.key_distribution = pAddr->bleKeyDistribution;
            BleSmIoCapabilityResponse(ind->bd_addr, &rsp);
        }
        else
        {
            BtSmIoCapabilityResponse(ind->bd_addr, IO_CAP_NOINPUT_OUTPUT, FALSE, TRUE, OOB_DATA_NOT_PRESERNT, 0);
        }
	}
}

static void app_System_RemoteIOCapabilityIndHandler(BT_SM_REMOTE_IO_CAPABILITY_IND_T * ind)
{
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)&ind->bd_addr;
	APP_INFO_STRU *pLinkInfo = APP_GetAppLinkByBdAddr(pBdAddr);

	if(pLinkInfo)
	{
		pLinkInfo->linkPara.ioCtl.remoteIoCap = ind->iocap;
		pLinkInfo->linkPara.ioCtl.remoteOOBDataPresent = ind->oob_data_present;
		pLinkInfo->linkPara.ioCtl.remoteMitm = ind->mitm;
		pLinkInfo->linkPara.ioCtl.remoteBonding = ind->bonding;
		pLinkInfo->linkPara.ioCtl.isIoCapRespSet = TRUE;
	}
}

static void app_System_PinCodeIndHandler(BT_SM_PIN_CODE_IND_T * ind)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_PIN_CODE_IND", 0);
	if(System_PinCodeNvKeyInit())
	{
		BtSmPinCodeResponse(ind->bd_addr, System_ReadNvkeyPinCodeLength(), System_ReadNvkeyPinCodePtr());
	}
	else
	{
		BtSmPinCodeResponse(ind->bd_addr, 0, NULL);
	}
}

static void app_System_UserConfirmationReqIndHandler(BT_SM_USER_CONFIRMATION_REQ_IND_T * ind)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_USER_CONFIRMATION_REQ_IND", 0);

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
	if (GFPSv2_is_processing()) {
		U32 nv = ind->passkey;
		U8 numeric[3];

		numeric[0] = (U8)(nv >> 16);
		numeric[1] = (U8)(nv >>  8);
		numeric[2] = (U8)(nv >>  0);
		GFPSv2_store_local_passkey(0, numeric);
	}
#endif
	BtSmUserConfirmationResponse(ind->bd_addr, TRUE);
}

static void app_System_UserPasskeyReqIndHandler(BT_SM_USER_PASSKEY_REQ_IND_T * ind)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_USER_PASSKEY_REQ_IND", 0);
    BtSmPasskeyResponse(ind->bd_addr, TRUE, 0);
}

static void app_System_SetServiceConfigCfmHandler(BT_SM_SET_SERVICE_CONFIG_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_SET_SERVICE_CONFIG_CFM cfm:%d", 1, cfm->status);
}

static void app_System_DeleteAllAuthDevice(BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T *cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T cfm:%d", 1, cfm->status);
}

static void app_system_encrytion_ind_handle(BT_SM_ENCRYPT_IND_T *ind)
{
    DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SM_ENCRYPT_IND ind:%x,%x", 2, ind->isEncrypted, ind->isKeyRefresh);

    if(ind->isEncrypted && !ind->isKeyRefresh) {
        APP_Qos_SetQos((BD_ADDR_T *) ind->bdAddr, QOS_SERVICE_GUARANTEED);
    }
}

static void app_System_SetSniffSubratingCfmHandler(BT_SET_SNIFF_SUBRATING_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SET_SNIFF_SUBRATING_CFM %s", 1, cfm->status ? fail_log_string : success_log_string);
}

static void app_System_SetModeCfmHandler(BT_SET_MODE_CFM_T * cfm)
{
	DBG_LOG_APP_SYSTEM( "[APP_SYS] BT_SET_MODE_CFM %s:%d", 2, cfm->status ? fail_log_string : success_log_string, cfm->status);
}

static void app_System_ModeChangeIndHandler(BT_MODE_CHANGE_IND_T * ind)
{
	UNUSED(ind);
}

static U32 app_SystemHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);
	switch(id)
	{
		case BT_INIT_CFM:
			app_System_InitCfmHandler((BT_INIT_CFM_T *)msg);
			break;
		case BT_DEINIT_CFM:
			app_System_DeInitCfmHandler((BT_DEINIT_CFM_T *)msg);
			break;
		case BT_GAP_DUTMODE_CFM:
			break;
		case BT_WRITE_LOCAL_NAME_CFM:
			app_System_WriteLocalNameCfmHandler((BT_WRITE_LOCAL_NAME_CFM_T *)msg);
			break;
		case BT_WRITE_CLASS_OF_DEVICE_CFM:
			app_System_WriteCODCfmHandler((BT_WRITE_CLASS_OF_DEVICE_CFM_T *)msg);
			break;
		case BT_WRITE_EIR_DATA_CFM:
			app_System_WriteEIRDataCfmHandler((BT_WRITE_EIR_DATA_CFM_T *)msg);
			break;
		case BT_ACL_OPENED_IND:
			app_System_AclOpenedIndHandler((BT_ACL_OPENED_IND_T *)msg);
			break;
		case BT_ACL_CLOSED_IND:
			app_System_AclClosedIndHandler((BT_ACL_CLOSED_IND_T *)msg);
			break;
		case BT_SM_IO_CAPABILITY_REQ_IND:
			app_System_IOCapabilityReqIndHandler((BT_SM_IO_CAPABILITY_REQ_IND_T *)msg);
			break;
		case BT_SM_REMOTE_IO_CAPABILITY_IND:
			app_System_RemoteIOCapabilityIndHandler((BT_SM_REMOTE_IO_CAPABILITY_IND_T *)msg);
			break;
		case BT_SM_PIN_CODE_IND:
			app_System_PinCodeIndHandler((BT_SM_PIN_CODE_IND_T *)msg);
			break;
		case BT_SM_USER_CONFIRMATION_REQ_IND:
			app_System_UserConfirmationReqIndHandler((BT_SM_USER_CONFIRMATION_REQ_IND_T *)msg);
			break;
		case BT_SM_USER_PASSKEY_REQ_IND:
			app_System_UserPasskeyReqIndHandler((BT_SM_USER_PASSKEY_REQ_IND_T *)msg);
			break;
		case BT_SM_SET_SERVICE_CONFIG_CFM:
			app_System_SetServiceConfigCfmHandler((BT_SM_SET_SERVICE_CONFIG_CFM_T *)msg);
			break;
		case BT_SM_DELETE_ALL_AUTH_DEVICE_CFM:
			app_System_DeleteAllAuthDevice((BT_SM_DELETE_ALL_AUTH_DEVICE_CFM_T *)msg);
			break;
        case BT_SM_ENCRYPT_IND:
            app_system_encrytion_ind_handle((BT_SM_ENCRYPT_IND_T *)msg);
            break;
		case BT_SET_SNIFF_SUBRATING_CFM:
			app_System_SetSniffSubratingCfmHandler((BT_SET_SNIFF_SUBRATING_CFM_T *)msg);
			break;
		case BT_SET_MODE_CFM:
			app_System_SetModeCfmHandler((BT_SET_MODE_CFM_T *)msg);
			break;
		case BT_MODE_CHANGE_IND:
			app_System_ModeChangeIndHandler((BT_MODE_CHANGE_IND_T *)msg);
			break;
		case PM_REGISTER_GAP_CFM:
			DBG_LOG_APP_SYSTEM( "[APP_SYS] gap reg status:[%d]", 1, ((PM_REGISTER_GAP_CFM_T *)msg)->status );
		default:
			DBG_LOG_APP_SYSTEM( "[APP_SYS] Rx Unknown Message:%x", 1, id);
			break;
	}
	return 0;
}

/**************************************************************************************************
* Internal Public Functions
**************************************************************************************************/
const HandlerData *APP_GetSystemHandle(void)
{
	return &gAppSystemHandle;
}

void APP_System_SetBtServiceClass(void)
{

	PM_System_SetDeviceName();
	PM_System_SetCOD();
}

void APP_System_Init(void)
{
	APP_KeyManage_Init((Handler)&gSystemKeyHandler);
	PM_GAP_RegisterApp((Handler)&gAppSystemHandle);
	System_TimerNvkeyInit();
	app_System_NvkeyInit();
}

BOOL APP_System_IsMpTestMode(void)
{
	return (gAppSystemCtl.isMpTestMode) ? TRUE : FALSE;
}

void APP_System_MpTestModeSetToDefault(void)
{
	NVKEY_ENALBE_MPTEST_MODE_STRU mpTestModeEnable;

	mpTestModeEnable.isMpTestMode = 0;
	nvkey_hdl_save_nvkey(NVKEYID_APP_ENABLE_MP_TEST_MODE, sizeof(NVKEY_ENALBE_MPTEST_MODE_STRU), &mpTestModeEnable);
}

void APP_System_SetToDefaultNvkey(void)
{
	APP_RESET_TO_FACTORY_STRU *pDefault = (APP_RESET_TO_FACTORY_STRU *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_RESET_TO_FACTORY_DEFAULT);

	if(pDefault)
	{
		nvkey_hdl_save_nvkey(NVKEYID_APP_RESET_TO_FACTORY_CONTROL, sizeof(APP_RESET_TO_FACTORY_STRU), pDefault);
	}
}
void APP_System_Discoverable(void)
{
	// Assume that user doesnt want to recoonect to phone
	APP_ReConn_ClearCtl();
	APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
    if ( GFPSv2_IsEnable() )
    {
        App_GfpsAdv_Update(1);
    }
#endif

#ifdef PROFILE_SWIFT_ENABLE 
=======
#ifdef AIR_GFP_ENABLE
    gfp_ble_adv_update(GFP_UPDATE_BLE_ADV_REASON_DISCOVERABLE_ENTER);
#endif

#ifdef AIR_SWIFT_ENABLE 
>>>>>>> db20e11 (second commit)
    App_SwiftAdv_Update();
#endif

}
