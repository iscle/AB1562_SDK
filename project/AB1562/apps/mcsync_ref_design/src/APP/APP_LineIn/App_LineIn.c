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
#include "App_LineIn.h"
#include "App_Avrcp.h"
#include "App_VolumeManage.h"
#include "App_Nvram_Type.h"
#include "App_ResourceManager.h"
#include "App_State.h"
#include "AudioDSP_Registration.h"
#include "App_Connection.h"
#include "App_A2dp.h"
#include "Avrcp_Interface.h"
#include "App_Media.h"
#include "App_EventOpcode.h"
#include "AudioDSP_StreamManager.h"
#include "App_VolumeLineinProcess.h"
#include "App_EventKeyHandler.h"
#include "app_lineIn_nvkey_nvstruc.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"

#ifdef LINEIN_ENABLE


#define DBG_LOG_APP_LINE_IN(_message, arg_cnt, ...)  LOG_MSGID_I(APP_LINE_IN, _message, arg_cnt, ##__VA_ARGS__)

log_create_module(APP_LINE_IN, PRINT_LEVEL_INFO);

/**************************************************************************************************
* Define
**************************************************************************************************/


/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 isLineInMute:1;
	U8 DSPSoundbarDownsampleSel:3;
	U8 isLineInProhibited:1;
	U8 isPayloadEncode:1;
	U8 reserved:2;
	U8 currentsoundLevel;
	U8 smallStepVol;
	U8 absVolume;
}APP_LINE_IN_CTRL;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_LineInEntry(BD_ADDR_T *pBdAddr);
static void app_LineInHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg);
static void app_LineInExit(BD_ADDR_T *pBdAddr);
static void app_LineIn_KeyPowerOn(BD_ADDR_T *pBdAddr);
static void app_LineIn_KeyPowerOff(BD_ADDR_T *pBdAddr);
static void app_LineIn_KeyMuteToggle(BD_ADDR_T *pBdAddr);
static U16 app_LineIn_CheckKeyPowerOn(U16 keyCode);

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_LINE_IN_CTRL gAppLineInCtl;
LINE_IN_NVKEY_STRU *pLineInNvKey;

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateLineInHandler =
{
	app_LineInEntry, app_LineInHandler, app_LineInExit, //MMI_LINE_IN
};

static APP_KEY_EVT_STRU const gAppLineInKeyEvtHandler[LINE_IN_KEY_EVENT_NO] =
{
	{app_LineIn_KeyPowerOn}, //LINE_IN_KEY_EVENT_KEY_ENTER_LINEIN_MODE
	{app_LineIn_KeyPowerOff}, //LINE_IN_KEY_EVENT_KEY_EXIT_LINEIN_MODE
	{app_LineIn_KeyMuteToggle}, //LINE_IN_KEY_EVENT_KEY_LINEIN_MUTE_TOGGLE
};

static APP_KEY_CHECK_STRU const gAppLineInKeyCheckValidHandler[LINE_IN_KEY_EVENT_NO] =
{
	{app_LineIn_CheckKeyPowerOn}, //LINE_IN_KEY_EVENT_KEY_ENTER_LINEIN_MODE
	{NULL}, //LINE_IN_KEY_EVENT_KEY_EXIT_LINEIN_MODE
	{NULL}, //LINE_IN_KEY_EVENT_KEY_LINEIN_MUTE_TOGGLE
};

static void app_LineIn_SetMuteOnOff(BOOL isMuteOn)
{
	gAppLineInCtl.isLineInMute = isMuteOn;
}

static BOOL app_LineIn_GetMuteStatus(void)
{
	return gAppLineInCtl.isLineInMute;
}

static BOOL app_LineIn_IsProhibited(void)
{
	return gAppLineInCtl.isLineInProhibited;
}

static void app_LineIn_ReadNvkey(void)
{
	LINE_IN_NVKEY_STRU *pData;

	if(((NVKEY_GetPayloadLength(NVKEYID_APP_LINE_IN_CFG) % sizeof(LINE_IN_NVKEY_STRU)) == 0) &&
		(pData = (LINE_IN_NVKEY_STRU *) NVKEY_GetPayloadFlashAddress(NVKEYID_APP_LINE_IN_CFG)))
	{
		pLineInNvKey = pData;
	}
}

static void app_LineIn_KeyPowerOn(BD_ADDR_T *pBdAddr)
{
	if(APP_AudioDeviceRegister(APP_EOF, AUDIO_DEVICE_LINE_IN))
	{
		if(APP_NO_CONNECTION_IN_LINE_IN_FEAT)
		{
			APP_Conn_ReleaseMultiLink();
		}
		if (APP_ENTER_LINEIN_SEND_AVRCP_PAUSE_FEAT)
		{
<<<<<<< HEAD
			#ifdef AVRCP_Profile
=======
			#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			if (APP_AvrcpIsMusicPlaying(pBdAddr) && APP_a2dp_is_in_streaming(pBdAddr))
			{
				APP_AvrcpDecideOperation(pBdAddr, AVC_OPERATION_KEY_PAUSE);
			}
			#else
			UNUSED(pBdAddr);
			#endif
		}
		APP_State_AddTopState(APP_EOF, APP_LINE_IN);
	}
}

static void app_LineIn_KeyPowerOff(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if (APP_State_CheckNestStateByLink(APP_EOF, APP_LINE_IN))
	{
		APP_State_RmvState(APP_EOF, APP_LINE_IN);

		if(!APP_IsAudioDeviceRegistered(APP_EOF, AUDIO_DEVICE_FM))
			APP_Media_PushMediaEvent(MEDIA_EVT_EXIT_LINE_IN_MODE);

label_clear_line_in:
		APP_AudioDeviceDeRegister(APP_EOF, AUDIO_DEVICE_LINE_IN);
		APP_ResumeOtherAudioDevice(APP_EOF, AUDIO_DEVICE_LINE_IN);
		if(APP_NO_CONNECTABLE_IN_LINE_IN_FEAT)
		{
			APP_State_ConnectabilityHandle(CMD_SET_RESUME_CONNECTABLE);
		}

		if(APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT)
		{
			APP_State_DiscoverabilityHandle(CMD_SET_RESUME_DISCOVERABLE);
		}

		/*if(APP_NO_CONNECTION_IN_LINE_IN_FEAT)
		{
			if(gMMI_ctl.miscMask4 & MMI_MASK4_RECONNECT_LINK0)
				MMI_Reconnect(RECONNECT_FIRST_ONCE);
			if((gMMI_ctl.miscMask4 & MMI_MASK4_RECONNECT_LINK1))
				MMI_Reconnect(RECONNECT_SECONDARY_ONCE);
			gMMI_ctl.miscMask4 &= ~(MMI_MASK4_RECONNECT_LINK0 |MMI_MASK4_RECONNECT_LINK1);
		}*/
	}
	else if (!APP_State_CheckNestStateByLink(APP_EOF, APP_LINE_IN)
		&& APP_IsAudioDeviceRegistered(NULL, AUDIO_DEVICE_LINE_IN))
	{
		DBG_LOG_APP_LINE_IN( "[APP_LINEIN] Clear Line-in audiodev only", 0);
		goto label_clear_line_in;
	}
}

static void app_LineIn_KeyMuteToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_LineIn_MuteControl(!app_LineIn_GetMuteStatus());
}

static U16 app_LineIn_CheckKeyPowerOn(U16 keyCode)
{
	if(!APP_LineIn_IsFeatureOn())
	{
		return KEY_INVALID;
	}

	if(APP_AudioDspIsStreaming(NULL, AUDIO_ANC))
	{
		return KEY_INVALID;
	}

	if(!APP_NO_CONNECTION_IN_LINE_IN_FEAT)
	{
		if(APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && APP_Conn_CheckProfileOnAllLinks())
		{
			keyCode = KEY_INVALID;
		}
	}

	if(APP_State_CheckNestStateByLink(APP_EOF, APP_LINE_IN)
		|| app_LineIn_IsProhibited()
		|| APP_IsAudioDeviceRegistered(NULL, AUDIO_DEVICE_LINE_IN))
	{
		keyCode = KEY_INVALID;
	}

	return keyCode;
}

static void app_LineInEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(APP_NO_CONNECTABLE_IN_LINE_IN_FEAT)
		APP_State_ConnectabilityHandle(CMD_SET_NON_CONNECTABLE_ANYWAY);

	if(APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT)
		APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE_ANYWAY);

	if(!APP_AudioDev_IsHighPriDevRegisted(AUDIO_LINEIN))
		APP_Media_PushMediaEvent(MEDIA_EVT_ENTER_LINE_IN_MODE);

	/*SYS_ReleaseTimer(&gMMI_ctl.audioDevCtl.resumeTimer);

	for(i = 0; i < MAX_MULTI_POINT_NO; i++)
	{
		if(gMMI_ctl.sdapCtl[i].remoteApplicationID == PRODUCT_AIR_NONE)
			MMI_HCI_SendRelSco(i);
	}*/

	key_map_gpio_cancle_linein_timer();
}

static void app_LineInHandler(BD_ADDR_T *pBdAddr, APP_MSG_TYPE_STRU *pMsg)
{
	UNUSED(pBdAddr);
	AUDIO_DSP_START_PARA_STUR audioDspPara;

	gAppCtl.msgHandled = PROCD_RELEASE;

	if (pMsg->eventCode == APP_FAKE_MEDIA_EVT)
	{
		if(pMsg->msgOpcode == MEDIA_EVT_ENTER_LINE_IN_MODE && !APP_AudioDspIsStreaming(pBdAddr, AUDIO_LINEIN))
		{
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
			FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
			audioDspPara.audioType = AUDIO_LINEIN;
			APP_AudioDspPlay(&audioDspPara);
		}
		else if(pMsg->msgOpcode == MEDIA_EVT_MUTE_OFF)
		{
			APP_LineIn_MuteControl(FALSE);
		}
	}
	else
	{
		gAppCtl.msgHandled = NOTPROCD;
	}
}

static void app_LineInExit(BD_ADDR_T *pBdAddr)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
	FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
	audioDspPara.audioType = AUDIO_LINEIN;
	APP_AudioDspStop(&audioDspPara);
}

static void app_LineIn_ResumeEncoder(U8 currState, U8 oldState)
{
	if(oldState != currState && (APP_GetCurrentAudioDevice() == AUDIO_DEVICE_LINE_IN) /*&& !gMMI_ctl.audioDevCtl.resumeTimer*/)
	{
		/*MMI_LineIn_DriverCmd(DRIVER_STOP_LINE_IN_CMD);
		MMI_LineIn_DriverCmd(DRIVER_LINE_IN_CMD);*/
	}
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_LineIn_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U8 evtIndex)
{
	if(gAppLineInKeyEvtHandler[evtIndex].fKeyEvt)
	{
		gAppLineInKeyEvtHandler[evtIndex].fKeyEvt(pBdAddr);
	}
}

U16 APP_LineIn_CheckValidKey(U8 evtIndex, U16 keyCode)
{
	if(gAppLineInKeyCheckValidHandler[evtIndex].fKeyCheckValid)
	{
		keyCode = gAppLineInKeyCheckValidHandler[evtIndex].fKeyCheckValid(keyCode);
	}
	return keyCode;
}

void APP_LineIn_Init(void)
{
	/*MMI_StateHandlerSet[MMI_LINE_IN] = &MMI_StateLineInHandler;

	MMI_LineIn_SetVolByLevel(DEFAULT_LINEIN_SOUND_LEVEL, DEFAULT_LINEIN_SMALL_STEP_LEVEL);

	if(MMI_AUX_FS_SEL == PCM_FS_SEL_2KHZ)
		gMMI_LineIn_ctl.DSPSoundbarDownsampleSel = SW_DOWNSAMPLE;
	else
		gMMI_LineIn_ctl.DSPSoundbarDownsampleSel = HW_DOWNSAMPLE;

#ifdef PEQ_ENABLE
	LightDbgPrint("Line-in: PEQ set number %d", (U8)PEQ_GetModeNumber(PEQ_LINE_IN));
#endif
*/
	app_LineIn_ReadNvkey();
}

void APP_LineIn_MuteControl(BOOL isMuteOn)
{
	if(isMuteOn)
	{
		app_LineIn_SetMuteOnOff(TRUE);
		APP_VolLinein_SetMuteOnOff(TRUE);
		APP_Media_PushMediaEvent(MEDIA_EVT_MUTE_ON);
	}
	else
	{
		app_LineIn_SetMuteOnOff(FALSE);
		APP_VolLinein_SetMuteOnOff(FALSE);
		APP_Media_PushMediaEvent(MEDIA_EVT_MUTE_OFF);
	}
}

void APP_LineIn_EncodedDataHandler(void)
{
	/*U8 XDATA_PTR ptr1;

	while(ptr1 = (U8 XDATA_PTR)OSMQ_MCU_DSP_Get(OSMQ_DSP_LineIn_Encoded_Payload_ptr))
	{
<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		if(MMI_LineIn_IsDSPOpen())
		{
			if(!MMI_A2DP_EncodedDataHandler(ptr1))
				OSMEM_Put(ptr1);
		}
		else
	#endif
		{
			OSMEM_Put(ptr1);
		}
	}*/
}

void APP_LineIn_DisableFunction(void)
{
	gAppLineInCtl.isLineInProhibited = TRUE;
	if(APP_State_CheckNestStateByLink(NULL, APP_LINE_IN))
	{
		APP_Media_PushMediaEvent(KEY_EXIT_LINEIN_MODE);
		APP_EvtKey_KeyEventHandler(NULL, KEY_EXIT_LINEIN_MODE);
	}
}

void APP_LineIn_EnableFunction(void)
{
	gAppLineInCtl.isLineInProhibited = FALSE;
	/*DRIVER_ResetLineInKeyTimer();*/
}

void APP_LineIn_EnablePayloadEncoder(void)
{
	U8 oldIsPayloadEncode = gAppLineInCtl.isPayloadEncode;

	gAppLineInCtl.isPayloadEncode = TRUE;

	app_LineIn_ResumeEncoder(TRUE, oldIsPayloadEncode);
}

void APP_LineIn_DisablePayloadEncoder(void)
{
	U8 oldIsPayloadEncode = gAppLineInCtl.isPayloadEncode;

	gAppLineInCtl.isPayloadEncode = FALSE;

	app_LineIn_ResumeEncoder(FALSE, oldIsPayloadEncode);
}

U8 APP_LineIn_GetLineInDetectIo(void)
{
	return pLineInNvKey ? pLineInNvKey->lineInDetectIo : LINE_IN_NO_DTETCT;
}

U8 APP_LineIn_GetLineInDetectTime(void)
{
	return pLineInNvKey ? pLineInNvKey->lineInDetectTime : 0;
}

U8 APP_LineIn_GetLineInMapKey(void)
{
	return pLineInNvKey ? pLineInNvKey->lineInMapKey : KEY_NOT_USED;
}

BOOL APP_LineIn_IsFeatureOn(void)
{
	return pLineInNvKey ? ((pLineInNvKey->lineInFeatureOnOff) ? TRUE : FALSE) : FALSE;
}

#endif //LINEIN_ENABLE
