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
#include "race_cmd.h"
#include "message_system.h"
#include "App_ResourceManager.h"
#include "App_RaceCmd.h"
#include "App_RaceCmd_KeyCode.h"
#include "App_RaceCmd_Connection.h"
#include "App_RaceCmd_Fcd.h"
#include "App_RaceCmd_Fota.h"
#include "App_RaceCmd_Anc.h"
#include "App_RaceCmd_AirDump.h"
#include "App_RaceCmd_System.h"
#include "App_RaceCmd_RealTime.h"
#include "app_racecmd_hostaudio.h"
#include "App_System.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 raceChannel;
}APP_RACE_CMD_CTL;

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_RACE_CMD_CTL gAppRaceCmdCtl;

static U32 app_RaceCmdHandler(Handler app, U16 id, void *msg, U32 id_ext);
static const HandlerData gAppRaceCmdHandle = { app_RaceCmdHandler };

/**************************************************************************************************
* Static Functions (Handler)
**************************************************************************************************/
static U32 app_RaceCmdHandler(Handler app, U16 id, void *msg, U32 id_ext)
{
	UNUSED(app);
	UNUSED(id_ext);

	if(id == MESSAGE_RACE_CMD)
	{
		MESSAGE_RACE_CMD_T *raceMsg = (MESSAGE_RACE_CMD_T *)msg;

		if(raceMsg->race_cmd.header.hdr.type == RACE_TYPE_RESPONSE)
		{
			switch(raceMsg->race_cmd.header.id)
			{
				default:
					DBG_LOG_APP_SYSTEM( "[APP_RACE] Unhandled Race Command ID:%d\n", 1, raceMsg->race_cmd.header.id);
					break;
			}
		}
	}

	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_RaceCmdInit()
{
	gAppRaceCmdCtl.raceChannel = RACE_HandlerPlug((Handler)&gAppRaceCmdHandle, RACE_CHANNEL_TYPE_SOFTWARE);
	APP_RaceCmd_KeyCodeInit();
	APP_RaceCmd_ConnectionInit();
	APP_RaceCmd_FCD_Init();
	APP_RaceCmd_AirDump_Init();
	APP_RaceCmd_Anc_Init();
	APP_RaceCmd_FotaInit();
	APP_RaceCmd_SystemInit();
    APP_RaceCmd_RealTimeInit();
    app_racecmd_hostaudio_init();
}

BOOL APP_SendRaceCmdByChannelID(U8 raceType, U16 raceId, U8 *pData, U16 dataLen, U8 channelId)
{
	U8 *pRacePkt;

	if(channelId)
	{
		if((pRacePkt = (U8 *)RACE_ClaimPacket(raceType, raceId, dataLen, channelId)))
		{
			if(pData && dataLen)
			{
				FW_Memcpy(pRacePkt, pData, dataLen);
			}
			RACE_FlushPacket(pRacePkt, dataLen);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL APP_SendRaceCmd(U16 raceId, U8 *pData, U16 dataLen)
{
	return APP_SendRaceCmdByChannelID(RACE_TYPE_COMMAND, raceId, pData, dataLen, gAppRaceCmdCtl.raceChannel);
}

