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
	APP_Conn_ReleaseProfileLinkAndDetach(&pRaceCmd->bdAddr);
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
