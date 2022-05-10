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
#include "APP_Qos.h"
#include "FW_Api.h"
#include "bt_gap.h"
#include "App_System.h"
#include "App_ResourceManager.h"
#include "App_State.h"

#define DBG_LOG_APP_QOS(_message, arg_cnt, ...)  LOG_MSGID_I(APP_QOS, _message, arg_cnt, ##__VA_ARGS__)

log_create_module(APP_QOS, PRINT_LEVEL_INFO);


static U32 app_QosHandler(Handler handler, U16 id, void *msg, U32 handler_id);

static const HandlerData gAppQosHandler = { app_QosHandler };

static void app_Qos_GapQosSetup(BD_ADDR_T *pBdAddr, APP_QOS_CTL *pAppQos)
{
	if(!APP_State_CheckLinkConnected(pBdAddr))
	{
		pAppQos->cmdStatus = QOS_CMD_STATUS_NONE;
		return;
	}

	if(pAppQos && pAppQos->cmdStatus != QOS_CMD_STATUS_SEND)
	{
		DBG_LOG_APP_QOS( "[APP_QOS] [%x,%x,%x]Set Qos Cmd", 3, pBdAddr->addr[0], pBdAddr->addr[1], pBdAddr->addr[2]);
		BtQosSetup((Handler)&gAppQosHandler, &pBdAddr->addr, pAppQos->flag, pAppQos->serviceType, pAppQos->tokenRate, pAppQos->peakBandwidth, pAppQos->latency, pAppQos->delayVariation);
	}
}

static void app_Qos_SetQosParam(APP_QOS_CTL *pAppQos, U8 serviceType)
{
	if(pAppQos == (APP_QOS_CTL * )NULL)
		return;

	switch(serviceType)
	{
		case QOS_SERVICE_BEST_EFFORT:
			pAppQos->flag = QOS_FLAG_NONE;
			pAppQos->serviceType = QOS_SERVICE_BEST_EFFORT;
			pAppQos->tokenRate = QOS_TOKEN_RATE_FULL;
			pAppQos->peakBandwidth =QOS_PEAK_BANDWIDTH_FULL;
			pAppQos->latency = QOS_LANTENCY_BEST_EFFORT_TIME;
			pAppQos->delayVariation = QOS_NO_DELAY_VARIATION;
			break;

		case QOS_SERVICE_GUARANTEED:
			pAppQos->flag = QOS_FLAG_NONE;
			pAppQos->serviceType = QOS_SERVICE_GUARANTEED;
			pAppQos->tokenRate = QOS_TOKEN_RATE_FULL;
			pAppQos->peakBandwidth =QOS_PEAK_BANDWIDTH_FULL;
			pAppQos->latency = QOS_LANTENCY_GUARANTEED_TIME;
			pAppQos->delayVariation = QOS_NO_DELAY_VARIATION;
			break;
	}
}

static void app_Qos_SetupCfmHandler(BD_ADDR_T *pBdAddr, APP_QOS_CTL *pAppQos, BT_QOS_SETUP_CFM_T *msg)
{
	if(pAppQos == (APP_QOS_CTL *)NULL)
		return;

	DBG_LOG_APP_QOS( "[APP_QOS] [%x,%x,%x]Qos Cmd Cfm:%d", 4, pBdAddr->addr[0], pBdAddr->addr[1], pBdAddr->addr[2], msg->status);

	if(msg->status == HCI_ERCODE_SUCCESS)
	{
		pAppQos->cmdStatus = QOS_CMD_STATUS_NONE;
	}
	else if(msg->status != HCI_ERCODE_UNKNOWN_CONNECTION_IDENTIFIER && msg->status != HCI_ERCODE_QOS_REJECTED)
	{
		pAppQos->cmdStatus = QOS_CMD_STATUS_REPOP;
		app_Qos_GapQosSetup(pBdAddr, pAppQos);
	}
}

static U32 app_QosHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	APP_QOS_CTL *pAppQos;
	UNUSED(handler);
	UNUSED(handler_id);
	BT_QOS_SETUP_CFM_T *ptr = msg;

	if((pAppQos = APP_SearchQosCtl( ((BD_ADDR_T *)ptr->bdaddr) ) ) == (APP_QOS_CTL *)NULL )
		return 0;

	switch(id)
	{
		case BT_QOS_SETUP_CFM:
			app_Qos_SetupCfmHandler((BD_ADDR_T *)ptr->bdaddr, pAppQos, ptr);
			break;

	}

	return 0;
}

void APP_Qos_SetQos(BD_ADDR_T *pBdAddr, U8 serviceType)
{
	APP_QOS_CTL *pAppQos;

	if(pBdAddr == (BD_ADDR_T *)NULL)
		return;

	if((pAppQos = APP_SearchQosCtl(pBdAddr)) != (APP_QOS_CTL *)NULL && pAppQos->serviceType != serviceType)
	{
		app_Qos_SetQosParam(pAppQos, serviceType);
		app_Qos_GapQosSetup(pBdAddr, pAppQos);
	}
}

