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
#include "App_EventOpcode.h"
#include "App_AvrcpKey.h"
#include "App_Avrcp.h"
#include "Avrcp_Interface.h"
#include "App_A2dp.h"
#include "A2dp_Interface.h"
#include "App_ResourceManager.h"

/**************************************************************************************************
* Prototype
**************************************************************************************************/
void APP_AVRCP_KeyPlay(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyStop(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyPause(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyRewindPush(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyFastForwardPush(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyForward(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyBackward(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyRewindRelease(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyFastForwardRelease(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyRepeatModeChange(BD_ADDR_T *pBdAddr);
void APP_AVRCP_KeyShuffleModeChange(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Variables
**************************************************************************************************/
/*static APP_KEY_EVT_STRU CODE APP_AVRCP_KeyEvtHandler[AVRCP_KEY_EVENT_NO] =
{
	{ APP_AVRCP_KeyPlay },					//AVRCP_KEY_EVENT_KEY_AVRCP_PLAY
	{ APP_AVRCP_KeyStop },					//AVRCP_KEY_EVENT_KEY_AVRCP_STOP 
	{ APP_AVRCP_KeyPause },					//AVRCP_KEY_EVENT_KEY_AVRCP_PAUSE
	{ NULL },								//AVRCP_KEY_EVENT_KEY_AVRCP_RECORD
	{ APP_AVRCP_KeyRewindPush },			//AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_PUSH, 
	{ APP_AVRCP_KeyFastForwardPush },		//AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_PUSH,	 	 
	{ NULL },								//AVRCP_KEY_EVENT_KEY_AVRCP_EJECT,
	{ APP_AVRCP_KeyForward },				//AVRCP_KEY_EVENT_KEY_AVRCP_FORWARD,	 				  
	{ APP_AVRCP_KeyBackward },				//AVRCP_KEY_EVENT_KEY_AVRCP_BACKWARD,		
	{ APP_AVRCP_KeyRewindRelease },			//AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_RELEASE, 
	{ APP_AVRCP_KeyFastForwardRelease },	//AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_RELEASE,  
	{ APP_AVRCP_KeyRepeatModeChange },		//AVRCP_KEY_EVENT_KEY_AVRCP_REPEAT_MODE_CHANGE,	 
	{ APP_AVRCP_KeyShuffleModeChange },		//AVRCP_KEY_EVENT_KEY_AVRCP_SHUFFLE_MODE_CHANGE,
	//{ APP_AVRCP_KeySwitch },//AVRCP_KEY_EVENT_KEY_AVRCP_ACTIVE_TOGGLE,
	//{ APP_AVRCP_KeyLeftChannel },//AVRCP_KEY_EVENT_KEY_KEY_LEFT_CHANNEL
	//{ APP_AVRCP_KeyRightChannel },//AVRCP_KEY_EVENT_KEY_KEY_RIGHT_CHANNEL
	//{ APP_AVRCP_KeyStereoChannel },//AVRCP_KEY_EVENT_KEY_KEY_STEREO_CHANNEL,
};*/

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_AVRCP_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U16 keyIndex)
{

	//if (APP_AVRCP_KeyEvtHandler[keyIndex].fKeyEvt)
	//{
		//APP_AVRCP_KeyEvtHandler[keyIndex].fKeyEvt(pBdAddr);
	//}


	switch(keyIndex)
	{
		case AVRCP_KEY_EVENT_KEY_AVRCP_PLAY:
			APP_AVRCP_KeyPlay(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_STOP:
			APP_AVRCP_KeyStop(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_PAUSE:
			APP_AVRCP_KeyPause(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_RECORD:
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_PUSH:
			APP_AVRCP_KeyRewindPush(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_PUSH:
			APP_AVRCP_KeyFastForwardPush(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_EJECT:
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_FORWARD:
			APP_AVRCP_KeyForward(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_BACKWARD:
			APP_AVRCP_KeyBackward(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_RELEASE:
			APP_AVRCP_KeyRewindRelease(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_RELEASE:
			APP_AVRCP_KeyFastForwardRelease(pBdAddr);
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_REPEAT_MODE_CHANGE:
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_SHUFFLE_MODE_CHANGE:
			break;
		case AVRCP_KEY_EVENT_KEY_AVRCP_ACTIVE_TOGGLE:
			break;
		case AVRCP_KEY_EVENT_KEY_KEY_LEFT_CHANNEL:
			break;
		case AVRCP_KEY_EVENT_KEY_KEY_RIGHT_CHANNEL:
			break;
		case AVRCP_KEY_EVENT_KEY_KEY_STEREO_CHANNEL:
			break;
		default:
			FW_Assert(FALSE);
			break;
	}
}

BOOL APP_AIR_CheckAVRCPKeyValid(U16 keyCode)
{
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	UNUSED(keyCode);
	/*
	if(A2DP_IsLinkFollower(gMMI_ctl.serviceInd))
	{
		MMI_DRV_KeyEventHandler(gMMI_ctl.serviceInd, keyCode);
		return FALSE;
	}
	*/
	#else
	UNUSED(keyCode);
	#endif
	return TRUE;
}



/**************************************************************************************************
* Public Functions (Key)
**************************************************************************************************/
void APP_AVRCP_KeyPlay(BD_ADDR_T *pBdAddr)
{
	if (AVRCP_IsConnected(pBdAddr))
		APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_KEY_PLAY);
	#if 0	//IOT with LG G2
	else if (MMI_a2dp_is_connected(linkIndex))
		bt_a2dp_start_request(pBdAddr);
	#endif

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if (!a2dp_is_connected(pBdAddr))
	{
		PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
	}
	#endif

}

void APP_AVRCP_KeyStop(BD_ADDR_T *pBdAddr)
{
	if (AVRCP_IsConnected(pBdAddr))
		APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_STOP);
	else
		bt_a2dp_suspend_request(pBdAddr);
}

void APP_AVRCP_KeyPause(BD_ADDR_T *pBdAddr)
{
	if (AVRCP_IsConnected(pBdAddr))
		APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_KEY_PAUSE);

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	else if (a2dp_is_connected(pBdAddr))
		bt_a2dp_suspend_request(pBdAddr);
	else
	{
		PM_ConnectProfile(pBdAddr, PROFILE_A2DP);
	}
	#endif
	
}

void APP_AVRCP_KeyRewindPush(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_REWIND);
}

void APP_AVRCP_KeyFastForwardPush(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_FAST_FORWARD);
}

void APP_AVRCP_KeyForward(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_FORWARD);
}

void APP_AVRCP_KeyBackward(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_BACKWARD);
}

void APP_AVRCP_KeyRewindRelease(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_INTERNAL_REWIND_RELEASE);
}

void APP_AVRCP_KeyFastForwardRelease(BD_ADDR_T *pBdAddr)
{
	APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_INTERNAL_FAST_FORWARD_RELEASE);
}

void APP_AVRCP_KeyRepeatModeChange(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkCtl = APP_GetAvrcpLinkInfo(pBdAddr);

	if (pLinkCtl->getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)
	{
		pLinkCtl->getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_KEY;
		APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_GET_REPEAT_MODE);
	}
}

void APP_AVRCP_KeyShuffleModeChange(BD_ADDR_T *pBdAddr)
{
	APP_AVRCP_CTL_STRU *pLinkCtl = APP_GetAvrcpLinkInfo(pBdAddr);

	if (pLinkCtl->getAppSettingsOwner == AVRCP_GET_APP_SETTINGS_NONE)
	{
		pLinkCtl->getAppSettingsOwner = AVRCP_GET_APP_SETTINGS_KEY;
		APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_GET_SHUFFLE_MODE);
	}
}
/*
void APP_AVRCP_KeySwitch(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

void APP_AVRCP_KeyLeftChannel(BD_ADDR_T *pBdAddr)
{
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_MONO_L);
	#else
	UNUSED(pBdAddr);
	#endif

	//APP_Media_PushMediaEvent(MEDIA_EVT_TWS_LEFT_CHANNEL);
}

void APP_AVRCP_KeyRightChannel(BD_ADDR_T *pBdAddr)
{
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_MONO_R);
	#else
	UNUSED(pBdAddr);
	#endif

	//APP_Media_PushMediaEvent(MEDIA_EVT_TWS_RIGHT_CHANNEL);
}

void APP_AVRCP_KeyStereoChannel(BD_ADDR_T *pBdAddr)
{
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	MMI_A2DP_SendCmd(linkIndex, A2DP_HW_AUDIO_CHANNEL_STEREO);
	#else
	UNUSED(pBdAddr);
	#endif
}
*/
