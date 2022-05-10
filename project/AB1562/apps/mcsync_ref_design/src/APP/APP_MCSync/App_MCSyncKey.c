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
#include "App_ResourceManager.h"
#include "App_MCSyncKey.h"
#include "App_ReConnection.h"
#include "App_Connection.h"
#include "App_EventOpcode.h"
#include "PM_Interface.h"
#include "BtMCSync.h"
#include "App_MCSync.h"
#include "App_Interface.h"
#include "App_State.h"
#include "App_MCSync_RHO.h"
#include "App_MCSync_State.h"
#include "App_MCSync_System.h"
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif

/*************************************************************************************************
* Public function
**************************************************************************************************/
void APP_AWSMCE_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U16 keyIndex)
{
	switch(keyIndex)
	{
		case AIR_KEY_EVENT_KEY_AIR_ENTER_AIR_PAIRING:
			APP_AWSMCE_KeyEnterAirPairing();
			break;
<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
		case AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_START:
			APP_AWSMCE_KeyEnterMCSyncSharePairing();
			break;

		case AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_STOP:
			APP_AWSMCE_KeyExitMCSyncSharePairing();
			break;
#else
		case AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_START:
		case AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_STOP:
			break;
#endif
		case AWSMCE_KEY_EVENT_KEY_CONNECT:
			APP_AWSMCE_KeyConnect();
			break;
		case AWSMCE_KEY_EVENT_KEY_DISCONNECT:
			APP_AWSMCE_KeyDisConnect(pBdAddr);
			break;
		case AWSMCE_KEY_EVENT_KEY_ENABLEDISCOVERABLE:
			APP_AWSMCE_KeyEnableDiscoverable();
			break;
		case AWSMCE_KEY_EVENT_KEY_DISABLEDISCOVERABLE:
			APP_AWSMCE_KeyDisableDiscoverable();
			break;
		case AWSMCE_KEY_EVENT_KEY_RHO:
			APP_AWSMCE_KeyRho();
			break;
		
		default:
			FW_Assert(FALSE);
			break;
	}
}

void APP_AWSMCE_KeyEnterAirPairing()
{
	APP_MCSYNC_StateStartPairing();
}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_AWSMCE_KeyEnterMCSyncSharePairing(void)
{
	APP_MCSync_Share_KeySharePairingStart();
}

void APP_AWSMCE_KeyExitMCSyncSharePairing(void)
{
    APP_MCSync_Share_KeySharePairingStop();
}
#endif

void APP_AWSMCE_KeyConnect()
{
	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
		APP_ReConn_Reconnect(RECONNECT_AWSMCE);
}

void APP_AWSMCE_KeyDisConnect(BD_ADDR_T *pBdAddr)
{
	if(BtAwsMce_GetDefaultRole() == ROLE_PARTNER)
		PM_DisconnectSpecificProfile(pBdAddr,PROFILE_MCSYNC);
}

void APP_AWSMCE_KeyEnableDiscoverable()
{
	if(BtAwsMce_IsDefaultRoleAgent())
		BtAwsMce_KeyEnableDiscoverable();
}

void APP_AWSMCE_KeyDisableDiscoverable()
{

}

void APP_AWSMCE_KeyRho()
{
	APP_MCSync_Rho_Start();
}

static APP_KEY_CHECK_STRU CODE APP_AWSMCE_KeyCheckValidHandler[AIR_KEY_EVENT_NO] =
{
	{NULL}, //AIR_KEY_EVENT_KEY_AIR_ENTER_AIR_PAIRING
	{NULL}, //AIR_KEY_EVENT_KEY_AIR_EXIT_AIR_PAIRING
	{NULL}, //AIR_KEY_EVENT_KEY_AIR_ENTER_AIR_SECURE_PAIRING
	{NULL}, //AWSMCE_KEY_EVENT_KEY_CONNECT
	{NULL}, //AWSMCE_KEY_EVENT_KEY_DISCONNECT,
	{NULL}, //AWSMCE_KEY_EVENT_KEY_ENABLEDISCOVERABLE,
	{NULL}, //AWSMCE_KEY_EVENT_KEY_DISABLEDISCOVERABLE,
	{NULL}, //AWSMCE_KEY_EVENT_KEY_RHO
	{NULL},	//AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_START,
	{NULL}, //AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_STOP,
};

U16 APP_AWSMCE_CheckValidKey(U8 index, U16 keyCode)
{
	if(APP_AWSMCE_KeyCheckValidHandler[index].fKeyCheckValid)
	{
		keyCode = APP_AWSMCE_KeyCheckValidHandler[index].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

