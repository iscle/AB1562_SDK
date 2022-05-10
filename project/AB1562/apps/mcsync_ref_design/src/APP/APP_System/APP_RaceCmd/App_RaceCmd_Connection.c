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
#include "App_Connection.h"
#include "App_Mcsync_Reconnection.h"
#include "BtMCSync.h"
#include "bt_aws_mce.h"
#include "MCSync.h"
#include "PM_Connection.h"
#include "App_System.h"
/**************************************************************************************************
* Variable
**************************************************************************************************/
static U32 app_RaceCmd_ConnectionHandler(Handler app, U16 id, void *msg, U32 id_ext);
HandlerData gAppRaceCmd_ConnectionHandle = { app_RaceCmd_ConnectionHandler };

/**************************************************************************************************
* Static Functions
**************************************************************************************************/

static void app_RaceCmd_AppConnectionDisconnectBT30Handler(MSG_ID_RACE_TO_APP_CONNECTION_DISCONNECT_BT30_T *pRaceCmd)
{
	//AB_console_printf("ooooooooooooooooooooooooooooooooooooooooooooooooooo");
	//AB_console_printf("[%x][%x][%x][%x][%x][%x]", pRaceCmd->bdAddr.addr[0], pRaceCmd->bdAddr.addr[1], pRaceCmd->bdAddr.addr[2], pRaceCmd->bdAddr.addr[3], pRaceCmd->bdAddr.addr[4],pRaceCmd->bdAddr.addr[5]);
	APP_Conn_ReleaseProfileLinkAndDetach(&pRaceCmd->bdAddr);
}

static void app_race_cmd_app_connection_enable_pts_test(MSG_ID_RACE_TO_APP_CONNECTION_ENABLE_PTS_TEST_T *msg)
{
    if(msg && msg->is_enable) {
        if(APP_Mcsync_IsInSuperReconnectMode()) {
            APP_Mcsync_ExitSuperReconnectMode(FALSE);
        }
        APP_System_FcdSingleHSModeState();
        APP_System_EnterSingleHSMode();
        if (BtAwsMce_GetMcsyncState() == MCSYNC_LINK_CONNECTED || BtAwsMce_GetMcsyncState() == MCSYNC_LINK_CONNECTING) {
                BtAwsMce_DisconnectAwsLink();
        }

<<<<<<< HEAD
        pm_connection_set_block_role_slave(true);
=======
        PM_Conn_SetBlockRoleSlave(true);
>>>>>>> db20e11 (second commit)
    }
}

static U32 app_RaceCmd_ConnectionHandler(Handler app, U16 id, void *msg, U32 id_ext)
{
	UNUSED(app);
	UNUSED(id_ext);
	
	switch(id)
	{
		case MSG_ID_RACE_TO_APP_CONNECTION_DISCONNECT_BT30:
			app_RaceCmd_AppConnectionDisconnectBT30Handler((MSG_ID_RACE_TO_APP_CONNECTION_DISCONNECT_BT30_T *)msg);
			break;
        case MSG_ID_RACE_TO_APP_CONNECTION_ENABLE_PTS_TEST:
            app_race_cmd_app_connection_enable_pts_test((MSG_ID_RACE_TO_APP_CONNECTION_ENABLE_PTS_TEST_T *)msg);
            break;
		default:
			break;
	}
	
	
	return 0;
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_RaceCmd_ConnectionInit()
{
	RACE_App_Connection_RegisterHandler(&gAppRaceCmd_ConnectionHandle);		
}

U8 APP_RCmdConn_GetMsgNumInQueue(void)
{
	return MSG_MessageSearchAllHandler(&gAppRaceCmd_ConnectionHandle);
}
