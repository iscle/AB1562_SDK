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
#include "feature_config.h"
#ifdef AIROHA_BT_SPP_ENABLE

#include "bt.h"
#include "spp.h"
#include "App_MCSync_RHO.h"
#include "APP_AirApp.h"

static U32 APP_CustAppHandler(Handler handler, U16 id, void *msg, U32 handler_id);

static const HandlerData gAppCustAppHandler = {APP_CustAppHandler};

//const U8 dataSample[5] = {'a','b','c','d','e'};

BD_ADDR_T tempBdAddr;

static void APP_CustApp_RxDataHandler(SPP_RX_DATA_IND_T * ind)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Rx Data Ind: BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Spp Rx Data:%d, %d, %d, %d, %d", 5,ind->length, ind->data[0], ind->data[1], ind->data[2], ind->data[3]);
}

static void APP_CustApp_TxAvailableIndHandler(SPP_TX_AVAILABLE_IND_T * ind)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Tx Available Ind: BdAddr:0x%x%x", 2, FW_bdaddr_to_2U32(&ind->bdAddr, TRUE), FW_bdaddr_to_2U32(&ind->bdAddr, FALSE));
}

static void APP_CustApp_StartServiceCfmHandler(SPP_START_SERVICE_CFM_T * cfm)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Start Service Cfm: %d", 1, cfm->status);
}

static void APP_CustApp_StopServiceCfmHandler(SPP_STOP_SERVICE_CFM_T * cfm)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Stop Service Cfm: %d", 1, cfm->status);
}

static void APP_CustApp_ConnectCfmHandler(SPP_CONNECT_CFM_T * cfm)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Connect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);
	//SPP_TxData(&cfm->bdAddr, 5, &dataSample);
	//FW_SetTimer((Handler)&gAppCustAppHandler, 10, NULL, 0, 20 * ONE_SEC);
	//FW_Memcpy(&tempBdAddr, (U8 *)&cfm->bdAddr, sizeof(BD_ADDR_T));
}

static void APP_CustApp_DisconnectCfmHandler(SPP_DISCONNECT_CFM_T * cfm)
{
	DBG_LOG_APP_AIRAPP( "[APP_CustSPP] Disconnect Cfm: BdAddr:0x%x%x, status: %d", 3, FW_bdaddr_to_2U32(&cfm->bdAddr, TRUE), FW_bdaddr_to_2U32(&cfm->bdAddr, FALSE), cfm->status);
	APP_MCSync_Rho_CustAppDisconnectNotify();
}

static U32 APP_CustAppHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(handler_id);

	switch(id)
	{
		//case 10:
		//	SPP_TxData(&tempBdAddr, 5, &dataSample);
		//	break;

		case SPP_RX_DATA_IND:
			APP_CustApp_RxDataHandler((SPP_RX_DATA_IND_T *)msg);
			break;
		case SPP_TX_AVAILABLE_IND:
			APP_CustApp_TxAvailableIndHandler((SPP_TX_AVAILABLE_IND_T *)msg);
			break;
		case SPP_START_SERVICE_CFM:
			APP_CustApp_StartServiceCfmHandler((SPP_START_SERVICE_CFM_T *)msg);
			break;
		case SPP_STOP_SERVICE_CFM:
			APP_CustApp_StopServiceCfmHandler((SPP_STOP_SERVICE_CFM_T *)msg);
			break;
		case SPP_CONNECT_CFM:
			APP_CustApp_ConnectCfmHandler((SPP_CONNECT_CFM_T *)msg);
			break;
		case SPP_DISCONNECT_CFM:
			APP_CustApp_DisconnectCfmHandler((SPP_DISCONNECT_CFM_T *)msg);
			break;
		default:
			break;
	}
	return 0;
}

void APP_CustApp_Init(void)
{
	
}

void APP_CustApp_RegisterProfile(void)
{
    SPP_StartService((Handler)&gAppCustAppHandler, 11);
}

#endif //AIROHA_BT_SPP_ENABLE
