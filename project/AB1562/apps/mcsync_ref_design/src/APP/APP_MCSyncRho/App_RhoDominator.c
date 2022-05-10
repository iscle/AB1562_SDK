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
#include "App_RhoDominator.h"
#include "BtMCSync.h"
#include "App_MCSyncRho_NvkeyHandler.h"
#include "app_mcsyncrho_config_nvstruc.h"
#include "App_ChargeBattery.h"
#include "App_Battery.h"
#include "bt_gap.h"
#include "App_ResourceManager.h"
#include "App_MCSync_Message.h"
#include "App_MCSync_RHO.h"
#include "App_RhoHandler.h"
<<<<<<< HEAD
=======
#include "chargercase_common.h"
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Define
**************************************************************************************************/
#define RHO_DOMN_MONITOR_TIMER_ID	BT_READ_RSSI_CFM - 1

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	RHO_NOT_ALLOW,
	RHO_DONT_CARE,
	RHO_NEED
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U16 rhoBat;
	S8 rhoRssi;
}APP_RHO_DOMN_RARA_STRU;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static S8 app_RhoDomn_GetAgentRssiPara(void);
static U16 app_RhoDomn_GetAgentBatPara(void);
static S8 app_RhoDomn_GetPartnerRssiPara(void);
static U16 app_RhoDomn_GetPartnerBatPara(void);
static void app_RhoDomn_SetAgentRssiPara(S8 rssi);
static void app_RhoDomn_SetAgentBatPara(U16 batPercent);
static U8 app_RhoDomn_CheckBatTrigStatus(U16 partnerBat);
static U8 app_RhoDomn_CheckRssiTrigStatus(S8 partnerRssi);
static U8 app_RhoDomn_GetTrigStatus(U8 trigIndex);
static BOOL app_RhoDomn_IsRhoNeeded(U8 *trigPriority, U8 *trigStatus);
static void app_RhoDomn_MonitorHandler(void);
static U32 app_RhoDomn_Handler(Handler handler, U16 id, void *msg, U32 ext_id);

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_RHO_DOMN_RARA_STRU gAppAgentPara, gAppPartnerPara;
static const HandlerData gAppRhoDomnHandle = { app_RhoDomn_Handler };
static U8 gAppRhoDomnFeatureBit[APP_RHO_TRIGGER_NUM] =
{
	APP_RHO_TRIGGER_FEATURE_BAT,
	APP_RHO_TRIGGER_FEATURE_RSSI
};

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static S8 app_RhoDomn_GetAgentRssiPara(void)
{
	return gAppAgentPara.rhoRssi;
}

static U16 app_RhoDomn_GetAgentBatPara(void)
{
	return gAppAgentPara.rhoBat;
}

static S8 app_RhoDomn_GetPartnerRssiPara(void)
{
	return gAppPartnerPara.rhoRssi;
}

static U16 app_RhoDomn_GetPartnerBatPara(void)
{
	return gAppPartnerPara.rhoBat;
}

static void app_RhoDomn_SetAgentRssiPara(S8 rssi)
{
	gAppAgentPara.rhoRssi = rssi;
}

static void app_RhoDomn_SetAgentBatPara(U16 batPercent)
{
	gAppAgentPara.rhoBat = batPercent;
}

static U8 app_RhoDomn_CheckBatTrigStatus(U16 partnerBat)
{
	U16 agentBat = app_RhoDomn_GetAgentBatPara();
	U16 batGap = (agentBat >= partnerBat) ? (agentBat - partnerBat) : (partnerBat - agentBat);
	U16 rhoBat = APP_MCSyncNvkey_GetRhoPercent();
	U16 rhoGapBat = APP_MCSyncNvkey_GetRhoGapPercent();

	if((agentBat >= rhoBat) && (partnerBat >= rhoBat))
	{
		return RHO_DONT_CARE;
	}

	if((agentBat > partnerBat) || (batGap < rhoGapBat))
	{
		return RHO_NOT_ALLOW;
	}

	DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO for %s: agent%d, partner:%d", 3, MCSync_RHO_REASON_String[APP_RHO_TRIGGER_BAT], agentBat, partnerBat);
	DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO for %s: thre:%d, GapThre:%d", 3, MCSync_RHO_REASON_String[APP_RHO_TRIGGER_BAT], rhoBat, rhoGapBat);

	return RHO_NEED;
}

static U8 app_RhoDomn_CheckRssiTrigStatus(S8 partnerRssi)
{
	S8 agentRssi = app_RhoDomn_GetAgentRssiPara();
	U8 rssiGap = (agentRssi >= partnerRssi) ? (agentRssi - partnerRssi) : (partnerRssi - agentRssi);
	S8 rhoRssi = APP_MCSyncNvkey_GetRhoRssi();
	U8 rhoGapRssi = APP_MCSyncNvkey_GetRhoGapRssi();

	if((agentRssi >= rhoRssi) && (partnerRssi >= rhoRssi))
	{
		return RHO_DONT_CARE;
	}

	if((agentRssi > partnerRssi) || (rssiGap < rhoGapRssi))
	{
		return RHO_NOT_ALLOW;
	}

	DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO for %s: agent%d, partner:%d", 3, MCSync_RHO_REASON_String[APP_RHO_TRIGGER_RSSI], agentRssi, partnerRssi);
	DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO for %s: thre:%d, GapThre:%d", 3, MCSync_RHO_REASON_String[APP_RHO_TRIGGER_RSSI], rhoRssi, rhoGapRssi);

	return RHO_NEED;
}

static U8 app_RhoDomn_GetTrigStatus(U8 trigIndex)
{
	switch(trigIndex)
	{
		case APP_RHO_TRIGGER_BAT:
			return app_RhoDomn_CheckBatTrigStatus(app_RhoDomn_GetPartnerBatPara());
		case APP_RHO_TRIGGER_RSSI:
			return app_RhoDomn_CheckRssiTrigStatus(app_RhoDomn_GetPartnerRssiPara());
		default:
			FW_Assert(FALSE);
			break;
	}

	return RHO_DONT_CARE;
}

static BOOL app_RhoDomn_IsRhoNeeded(U8 *trigPriority, U8 *trigStatus)
{
	U8 totalIndex, trigIndex, maxIndex, priorityMax = 0;

	for(totalIndex = 0; totalIndex < APP_RHO_TRIGGER_NUM; totalIndex++)
	{
		for(trigIndex = 0; trigIndex < APP_RHO_TRIGGER_NUM; trigIndex++)
		{
			if((priorityMax == 0 && trigPriority[trigIndex] != 0) || priorityMax < trigPriority[trigIndex])
			{
				priorityMax = trigPriority[trigIndex];
				maxIndex = trigIndex;
			}
		}

		if(priorityMax == 0)
		{
			return FALSE;
		}

		DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO Status:%s, Reason:%s", 2, MCSync_RHO_STATUS_String[trigStatus[maxIndex]], MCSync_RHO_REASON_String[maxIndex]);

		if(trigStatus[maxIndex] == RHO_DONT_CARE)
		{
			trigPriority[maxIndex] = 0;
			priorityMax = 0;
		}
		else
		{
			return (trigStatus[maxIndex] == RHO_NEED) ? TRUE : FALSE;
		}
	}

	return FALSE;
}

static void app_RhoDomn_MonitorHandler(void)
{
	U8 trigIndex, trigPriority[APP_RHO_TRIGGER_NUM], trigStatus[APP_RHO_TRIGGER_NUM];

	for(trigIndex = 0; trigIndex < APP_RHO_TRIGGER_NUM; trigIndex++)
	{
		if(APP_MCSyncNvkey_IsRhoFeatureOn(gAppRhoDomnFeatureBit[trigIndex]))
		{
			trigPriority[trigIndex] = APP_MCSyncNvkey_GetRhoPriority(trigIndex);
			trigStatus[trigIndex] = app_RhoDomn_GetTrigStatus(trigIndex);
		}
        else
        {
            trigPriority[trigIndex] = 0;
            trigStatus[trigIndex] = RHO_DONT_CARE;
        }
	}

	if(app_RhoDomn_IsRhoNeeded(trigPriority, trigStatus))
	{
<<<<<<< HEAD
		APP_MCSync_Rho_Start();
=======
        DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO Dominator Partner Chg State: %d", 1, APP_GetPartnerSmartChargeCaseState());
        if (APP_GetPartnerSmartChargeCaseState() == STATE_OUT_OF_CASE)
        {
        	APP_MCSync_Rho_Start();
        }
>>>>>>> db20e11 (second commit)
	}
}

static U32 app_RhoDomn_Handler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	UNUSED(handler);
	UNUSED(ext_id);

	DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO Dominator id:%d", 1, id);

	switch(id)
	{
		case BT_READ_RSSI_CFM:
			if(((BT_READ_RSSI_CFM_T *)msg)->hci_status == HCI_STATUS_SUCCESS)
			{
				DBG_LOG_APP_McsyncRho( "[APP MCSync] RHO Dominator BatPercent:%d, RSSI:%d", 2, APP_Battery_GetBatInThousandPercent(), ((BT_READ_RSSI_CFM_T *)msg)->rssi);

				if(BtAwsMce_IsDefaultRolePartner())
				{
					APP_MCSYNC_SendSyncRhoParaInfo(APP_Battery_GetBatInThousandPercent(), ((BT_READ_RSSI_CFM_T *)msg)->rssi);
					APP_RhoDomn_MonitorTimerHandler();
				}
				else
				{
					app_RhoDomn_SetAgentRssiPara(((BT_READ_RSSI_CFM_T *)msg)->rssi);
					app_RhoDomn_SetAgentBatPara(APP_Battery_GetBatInThousandPercent());
					app_RhoDomn_MonitorHandler();
				}
			}
			break;
		case RHO_DOMN_MONITOR_TIMER_ID:
			if(APP_GetServiceBdAddr())
			{
				BtReadRssi((Handler)&gAppRhoDomnHandle, &APP_GetServiceBdAddr()->addr);
			}
			break;

		default:
			break;
	}

	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_RhoDomn_SetPartnerRssiPara(S8 rssi)
{
	gAppPartnerPara.rhoRssi = rssi;
}

void APP_RhoDomn_SetPartnerBatPara(U16 batPercent)
{
	gAppPartnerPara.rhoBat = batPercent;
}

HandlerData *APP_RhoDomn_GetHandle(void)
{
	return (Handler)&gAppRhoDomnHandle;
}

void APP_RhoDomn_MonitorTimerHandler(void)
{
	if(APP_GetAGNum() > 0)
	{
		FW_SetTimer((Handler)&gAppRhoDomnHandle, RHO_DOMN_MONITOR_TIMER_ID, NULL, 0, APP_MCSyncNvkey_GetRhoTimer(RHO_TIMER_ID_MONITOR_PARA)*ONE_SEC);
	}
	else
	{
		FW_ReleaseTimer((Handler)&gAppRhoDomnHandle, RHO_DOMN_MONITOR_TIMER_ID, 0);
	}
}

void APP_RhoDomn_CancelMonitorTimer(void)
{
	FW_ReleaseTimer((Handler)&gAppRhoDomnHandle, RHO_DOMN_MONITOR_TIMER_ID, 0);
}
<<<<<<< HEAD
=======

void APP_RhoDomn_StartMonitorTimer(void)
{
	FW_ReleaseTimer((Handler)&gAppRhoDomnHandle, RHO_DOMN_MONITOR_TIMER_ID, 0);
    FW_SetTimer((Handler)&gAppRhoDomnHandle, RHO_DOMN_MONITOR_TIMER_ID, NULL, 0, APP_MCSyncNvkey_GetRhoTimer(RHO_TIMER_ID_MONITOR_PARA)*ONE_SEC);
}

>>>>>>> db20e11 (second commit)
