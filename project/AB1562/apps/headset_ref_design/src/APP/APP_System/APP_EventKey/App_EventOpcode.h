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
 
 
#ifndef _APP_EVENTOPCODE_H_
#define _APP_EVENTOPCODE_H_

#include "App_Media.h"
/**************************************************************************************************
* Define
**************************************************************************************************/
// ============================
// ======= System Events =======
// ============================
#define APP_HFP_EVT_OFFSET	0x20

#define APP_DRIVER_EVT_OFFSET		0x80

#define APP_INTERNAL_OFFSET		0xF9//MMI_INTERNAL_OFFSET

//BAT-EVT Opcode Range: 0x70 ~ 0x77
#define APP_BAT_EVT_OFFSET		0x7F//MMI_BAT_EVT_OFFSET
//GAP Opcode Range: 0x80 ~ 0xAC
#define APP_STATUS_EVT_OFFSET	0x91//MMI_STATUS_EVT_OFFSET

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_NONE_EVT,
	APP_FAKE_MEDIA_EVT = APP_DRIVER_EVT_OFFSET,
	//MMI internal use cmd 
	APP_KEY_EVENT = 0X01 + APP_INTERNAL_OFFSET,
	APP_LINK_DETACHED_EVT,
	APP_DETACH_LINK_TIMER_TIMEOUT,
	APP_CHECK_POWERON_TIMER,
	APP_CHARGING_TO,
	APP_PWROFF_CMD,
	APP_RESET_PAIRED_DEVICE_CMD,
	//--------------------------------------------------------
};

#undef EVENT_NAME
#define EVENT_NAME(name) KEY_##name,

enum
{
	KEY_INVALID,
	#include "App_KeyEvent.h"
	KEY_FOR_AE_START = 0x0500,
	KEY_FOR_AE1 = KEY_FOR_AE_START,
	#include "App_KeyEventReserved.h"
	KEY_TOTAL_NO,
};

enum
{
	SYSTEM_KEY_EVENT,
	AWSMCE_KEY_EVENT,
	HFP_KEY_EVENT,
	AVRCP_KEY_EVENT,
	FM_KEY_EVENT,
	LINE_IN_KEY_EVENT,
	TESTMODE_KEY_EVENT,
	HID_KEY_EVENT,
	AMA_KEY_EVENT,
	MP3_KEY_EVENT,
	GVA_KEY_EVENT,
	XIAOAI_KEY_EVENT,
	KEY_EVENT_GROUP_NO,
	NOT_KEY_EVENT,
};

enum
{
	SYSTEM_KEY_EVENT_KEY_RECONNECT_USER_INIT,					//KEY_RECONNECT_USER_INIT				//0x0001
	SYSTEM_KEY_EVENT_KEY_DISCOVERABLE,							//KEY_DISCOVERABLE,
	SYSTEM_KEY_EVENT_KEY_NONDISCOVERABLE, 						//KEY_NONDISCOVERABLE,
	SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_DISCOVERABLE,				//KEY_CONNECTED_TO_DISCOVERABLE,
	SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_CONNECTABLE,				//KEY_CONNECTED_TO_CONNECTABLE,			//0x0005
	SYSTEM_KEY_EVENT_KEY_NFC_DISCOVREABLE,						//KEY_NFC_DISCOVREABLE,
	SYSTEM_KEY_EVENT_KEY_CONFIRMATION_ACCEPT,					//KEY_CONFIRMATION_ACCEPT,
	SYSTEM_KEY_EVENT_KEY_CONFIRMATION_REJECT,					//KEY_CONFIRMATION_REJECT,
	SYSTEM_KEY_EVENT_KEY_DEBUG_KEY_TOGGLE,						//KEY_DEBUG_KEY_TOGGLE,
	SYSTEM_KEY_EVENT_KEY_VOICEUP,								//KEY_VOICEUP,							//0x000A
	SYSTEM_KEY_EVENT_KEY_VOICEDN,								//KEY_VOICEDN,
	SYSTEM_KEY_EVENT_KEY_BUZZER_MUTE_TOGGLE,					//KEY_BUZZER_MUTE_TOGGLE,
	SYSTEM_KEY_EVENT_KEY_TOGGLE_VOLUME,							//KEY_TOGGLE_VOLUME,
	SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_NORMAL,				//KEY_VOLUME_ORIENTATION_NORMAL,
	SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_INVERT,				//KEY_VOLUME_ORIENTATION_INVERT,		//0x000F
	SYSTEM_KEY_EVENT_KEY_TOGGLE_LED,							//KEY_TOGGLE_LED,						//0x0010
	SYSTEM_KEY_EVENT_KEY_ENABLE_LED,							//KEY_ENABLE_LED,
	SYSTEM_KEY_EVENT_KEY_DISABLE_LED,							//KEY_DISABLE_LED,
	SYSTEM_KEY_EVENT_KEY_CANCEL_LED_INDICATION,					//KEY_CANCEL_LED_INDICATION,
	SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_1,						//KEY_MEDIA_TRIGGER_1
	SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_2,						//KEY_MEDIA_TRIGGER_2,,					//0x0015
	SYSTEM_KEY_EVENT_KEY_POWER_ON_THEN_ENTERDISCOVERABLE,		//KEY_POWER_ON_THEN_ENTERDISCOVERABLE,
	SYSTEM_KEY_EVENT_KEY_POWER_ON,								//KEY_POWER_ON,
	SYSTEM_KEY_EVENT_KEY_POWER_OFF,								//KEY_POWER_OFF,
	SYSTEM_KEY_EVENT_KEY_RESET,									//KEY_RESET,
	SYSTEM_KEY_EVENT_KEY_RESET_PAIRED_DEVICES,					//KEY_RESET_PAIRED_DEVICES,				//0x001A
	SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_UP,					//KEY_MAX_DEVICE_ALLOWED_UP,
	SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_DOWN,				//KEY_MAX_DEVICE_ALLOWED_DOWN,
	SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_TOGGLE,				//KEY_MAX_DEVICE_ALLOWED_TOGGLE,
	SYSTEM_KEY_EVENT_KEY_RECONNECT_AFTER_POWER_ON,				//KEY_RECONNECT_AFTER_POWER_ON,
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_TOGGLE,					//KEY_VOICE_PROMPT_TOGGLE,				//0x001F
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_ENABLE,					//KEY_VOICE_PROMPT_ENABLE,				//0x0020
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_DISABLE,					//KEY_VOICE_PROMPT_DISABLE,
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CHANGE,				//KEY_VOICE_PROMPT_LANG_CHANGE,
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_SELECT,				//KEY_VOICE_PROMPT_LANG_SELECT,
	SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CONFIRM,				//KEY_VOICE_PROMPT_LANG_CONFIRM,
	SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_PLAY,				//KEY_TEST_VOICE_PROMPT_PLAY,			//0x0025
	SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_NEXT,				//KEY_TEST_VOICE_PROMPT_NEXT,
	SYSTEM_KEY_EVENT_KEY_BUTTONLOCKON,							//KEY_BUTTONLOCKON,
	SYSTEM_KEY_EVENT_KEY_BUTTONLOCKOFF,							//KEY_BUTTONLOCKOFF,
	SYSTEM_KEY_EVENT_KEY_BUTTONLOCKTOGGLE,						//KEY_BUTTONLOCKTOGGLE,
	SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_ENABLE,					//KEY_VOICE_COMMAND_ENABLE,				//0x002A
	SYSTEM_KEY_EVENT_KEY_MICUP,									//KEY_MICUP,
	SYSTEM_KEY_EVENT_KEY_MICDN,									//KEY_MICDN,
	SYSTEM_KEY_EVENT_KEY_MIC_MUTE_TOGGLE,						//KEY_MIC_MUTE_TOGGLE,
	SYSTEM_KEY_EVENT_KEY_MIC_MUTE_ON,							//KEY_MIC_MUTE_ON,
	SYSTEM_KEY_EVENT_KEY_MIC_MUTE_OFF,							//KEY_MIC_MUTE_OFF,						//0x002F
	SYSTEM_KEY_EVENT_KEY_SCO_SWITCH,							//KEY_SCO_SWITCH,						//0x0030
	SYSTEM_KEY_EVENT_KEY_PEQ_GROUP_CHANGE,						//KEY_PEQ_GROUP_CHANGE
	SYSTEM_KEY_EVENT_KEY_MULTI_A2DP_PLAY_MODE_TOGGLE,			//KEY_MULTI_A2DP_PLAY_MODE_TOGGLE
	SYSTEM_KEY_EVENT_KEY_CHECK_STATUS_OF_CONNECTION,			//KEY_CHECK_STATUS_OF_CONNECTION
	SYSTEM_KEY_EVENT_KEY_WHAT_CAN_I_SAY,						//KEY_WHAT_CAN_I_SAY
	SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_CANCEL,					//KEY_VOICE_COMMAND_CANCEL				//0x0035
	SYSTEM_KEY_EVENT_KEY_VOICE_MAKE_A_CALL,						//KEY_VOICE_MAKE_A_CALL
	SYSTEM_KEY_EVENT_KEY_CHECK_STATUS,							//KEY_CHECK_STATUS
	SYSTEM_KEY_EVENT_KEY_CHANGE_SOUND_EFFECT,					//KEY_CHANGE_SOUND_EFFECT
	SYSTEM_KEY_EVENT_KEY_CONTROL_DBB_ON_OFF,					//KEY_CONTROL_DBB_ON_OFF
	SYSTEM_KEY_CONTROL_MIC_A2DP_MODE,							//KEY_CONTROL_MIC_A2DP_MODE				//0x003A
	SYSTEM_KEY_ENABLE_DUT_MODE_COMMAND,							//KEY_ENABLE_DUT_MODE_COMMAND
	SYSTEM_KEY_EVENT_KEY_MIC_L_ENABLE,							//KEY_MIC_L_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_R_ENABLE,							//kEY_MIC_R_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_ENABLE,						//KEY_MIC_TO_SPK_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_DISABLE,					//KEY_MIC_TO_SPK_DISABLE				//0x003F
	SYSTEM_KEY_EVENT_KEY_ENABLE_EC_NR,							//KEY_ENABLE_EC_NR						//0x0040
	SYSTEM_KEY_EVENT_KEY_DISABLE_EC_NR,							//KEY_DISABLE_EC_NR
	SYSTEM_KEY_EVENT_KEY_2K_TONE_ENABLE,						//KEY_2K_TONE_ENABLE
	SYSTEM_KEY_EVENT_KEY_2K_TONE_DISABLE,						//KEY_2K_TONE_DISABLE
	SYSTEM_KEY_EVENT_KEY_SWITCH_STEREO_MONO,					//KEY_SWITCH_STEREO_MONO
	SYSTEM_KEY_EVENT_KEY_AIROTHRU,								//KEY_AIROTHRU							//0x0045
	SYSTEM_KEY_EVENT_KEY_ROLE_CHANGE, 							//KEY_ROLE_CHANGE
	SYSTEM_KEY_EVENT_KEY_ANC_SWITCH, 							//KEY_ANC_SWITCH
	SYSTEM_KEY_EVENT_KEY_RESET_TO_FACTORY,						//KEY_RESET_TO_FACTORY
	SYSTEM_KEY_EVENT_KEY_ANC_ON, 								//KEY_ANC_ON
	SYSTEM_KEY_EVENT_KEY_ANC_OFF, 								//KEY_ANC_OFF
	SYSTEM_KEY_EVENT_KEY_DSP_RESET,								//KEY_DSP_RESET
	SYSTEM_KEY_EVENT_KEY_REPORT_BATTERY_STATUS,					//KEY_REPORT_BATTERY_STATUS
	SYSTEM_KEY_EVENT_KEY_VAD_ON,								//KEY_VAD_ON
	SYSTEM_KEY_EVENT_KEY_VAD_OFF,								//KEY_VAD_OFF
	SYSTEM_KEY_EVENT_KEY_GAME_MODE_ON,							//KEY_GAME_MODE_ON
	SYSTEM_KEY_EVENT_KEY_GAME_MODE_OFF,							//KEY_GAME_MODE_OFF
	SYSTEM_KEY_EVENT_KEY_GAME_MODE_TOGGLE,						//KEY_GAME_MODE_TOGGLE
<<<<<<< HEAD
	SYSTEM_KEY_EVENT_KEY_AGENT_STARTS_SHARE_MODE,               //KEY_AGENT_STARTS_SHARE_MODE
	SYSTEM_KEY_EVENT_KEY_AGENT_STOPS_SHARE_MODE,                //KEY_AGENT_STOPS_SHARE_MODE
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_STARTS_SHARE_MODE,            //KEY_FOLLOWER_STARTS_SHARE_MODE
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_STOPS_SHARE_MODE,             //KEY_FOLLOWER_STOPS_SHARE_MODE
	SYSTEM_KEY_EVENT_KEY_AGENT_TOGGLE_SHARE_MODE,               //KEY_AGENT_TOGGLE_SHARE_MODE
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_TOGGLE_SHARE_MODE,            //KEY_FOLLOWER_TOGGLE_SHARE_MODE
=======
	SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_START,				//KEY_MASTER_SHARE_MODE_START
	SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_STOP,				//KEY_MASTER_SHARE_MODE_STOP
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_START,				//KEY_FOLLOWER_SHARE_MODE_START
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_STOP,				//KEY_FOLLOWER_SHARE_MODE_STOP
	SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_TOGGLE,				//KEY_MASTER_SHARE_MODE_TOGGLE
	SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_TOGGLE,			//KEY_FOLLOWER_SHARE_MODE_TOGGLE
>>>>>>> db20e11 (second commit)
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_MAIN_MIC_ENABLE,		//KEY_MIC_TO_SPK_3MIC_MAIN_MIC_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF1_MIC_ENABLE,		//KEY_MIC_TO_SPK_3MIC_REF1_MIC_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF2_MIC_ENABLE,		//KEY_MIC_TO_SPK_3MIC_REF2_MIC_ENABLE
	SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_DISABLE,				//KEY_MIC_TO_SPK_3MIC_DISABLE
	
	SYSTEM_KEY_EVENT_NO,
};

enum
{
	AIR_KEY_EVENT_KEY_AIR_ENTER_AIR_PAIRING,
	AIR_KEY_EVENT_KEY_AIR_EXIT_AIR_PAIRING,
	AIR_KEY_EVENT_KEY_AIR_ENTER_AIR_SECURE_PAIRING,
	AWSMCE_KEY_EVENT_KEY_CONNECT,
	AWSMCE_KEY_EVENT_KEY_DISCONNECT,
	AWSMCE_KEY_EVENT_KEY_ENABLEDISCOVERABLE,
	AWSMCE_KEY_EVENT_KEY_DISABLEDISCOVERABLE,
	AWSMCE_KEY_EVENT_KEY_RHO,
	AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_START,
	AWSMCE_KEY_EVENT_KEY_EARBUDS_MODE_AIR_PAIRING_STOP,
	AIR_KEY_EVENT_NO,
};

enum
{
	HFP_KEY_EVENT_KEY_AUDIO_TRANSFER,
	HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION,
	HFP_KEY_EVENT_KEY_RDIAL,									//0x0035
	HFP_KEY_EVENT_KEY_DIALMEM,
	HFP_KEY_EVENT_KEY_DIALNUM,
	HFP_KEY_EVENT_KEY_CNLOUTGOING,
	HFP_KEY_EVENT_KEY_REJCALL,
	HFP_KEY_EVENT_KEY_REJCALL_SECOND_PHONE,
	HFP_KEY_EVENT_KEY_ONHOLD_CALL,								//0x003B
	HFP_KEY_EVENT_KEY_ACPCALL,
	HFP_KEY_EVENT_KEY_ENDCALL,
	HFP_KEY_EVENT_KEY_3WAYRELNUDUB,
	HFP_KEY_EVENT_KEY_3WAYRELNACP,
	HFP_KEY_EVENT_KEY_3WAYRELNACP_X,							//0x0040
	HFP_KEY_EVENT_KEY_3WAYHOLDNACP,
	HFP_KEY_EVENT_KEY_3WAYHOLDNACP_X,
	HFP_KEY_EVENT_KEY_3WAYADD,
	HFP_KEY_EVENT_KEY_3WAYCALLTRANSFER,
	HFP_KEY_EVENT_KEY_HSP_BUTTON_PRESSED,
	HFP_KEY_EVENT_KEY_DISABLE_NREC,								//0x0046
	HFP_KEY_EVENT_KEY_GETCALLNUM,
	HFP_KEY_EVENT_KEY_QUERY_CALL_LIST,
	HFP_KEY_EVENT_KEY_SEND_DTMF,
	HFP_KEY_EVENT_KEY_ATTACH_LAST_VOICE_TAG_RECORDED,
	HFP_KEY_EVENT_KEY_SEND_CLIP,								//0x004B
	HFP_KEY_EVENT_KEY_RESERVED1,
	HFP_KEY_EVENT_KEY_RESERVED2,
	HFP_KEY_EVENT_KEY_SET_VOICE_RECOGNITION_CANCEL,
	HFP_KEY_EVENT_KEY_RESERVED3,
	HFP_KEY_EVENT_KEY_SIRI_START_OR_CONTINUE,					//0x0050
	HFP_KEY_EVENT_KEY_SIRI_END,									//0x0051
	HFP_KEY_EVENT_KEY_RESERVED4,
	HFP_KEY_EVENT_KEY_RESERVED5,
	HFP_KEY_EVENT_NO,
};

enum
{
	AVRCP_KEY_EVENT_KEY_AVRCP_PLAY,
	AVRCP_KEY_EVENT_KEY_AVRCP_STOP,
	AVRCP_KEY_EVENT_KEY_AVRCP_PAUSE,							//0x0056
	AVRCP_KEY_EVENT_KEY_AVRCP_RECORD,
	AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_PUSH,
	AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_PUSH,
	AVRCP_KEY_EVENT_KEY_AVRCP_EJECT,
	AVRCP_KEY_EVENT_KEY_AVRCP_FORWARD,							//0x005B
	AVRCP_KEY_EVENT_KEY_AVRCP_BACKWARD,
	AVRCP_KEY_EVENT_KEY_AVRCP_REWIND_RELEASE,
	AVRCP_KEY_EVENT_KEY_AVRCP_FAST_FORWARD_RELEASE,
	AVRCP_KEY_EVENT_KEY_AVRCP_REPEAT_MODE_CHANGE,
	AVRCP_KEY_EVENT_KEY_AVRCP_SHUFFLE_MODE_CHANGE,				//0x0060
	AVRCP_KEY_EVENT_KEY_AVRCP_ACTIVE_TOGGLE,
	AVRCP_KEY_EVENT_KEY_KEY_LEFT_CHANNEL,
	AVRCP_KEY_EVENT_KEY_KEY_RIGHT_CHANNEL,
	AVRCP_KEY_EVENT_KEY_KEY_STEREO_CHANNEL,
	AVRCP_KEY_EVENT_NO,
};

enum
{
	FM_KEY_EVENT_KEY_FM_POWERON,
	FM_KEY_EVENT_KEY_FM_POWEROFF,
	FM_KEY_EVENT_KEY_SEEKUP,
	FM_KEY_EVENT_KEY_SEEKDOWN,									//0x0066
	FM_KEY_EVENT_KEY_TUNEUP,
	FM_KEY_EVENT_KEY_TUNEDN,
	FM_KEY_EVENT_KEY_FM_MUTETOGGLE,
	FM_KEY_EVENT_NO,
};

enum
{
	LINE_IN_KEY_EVENT_KEY_ENTER_LINEIN_MODE,
	LINE_IN_KEY_EVENT_KEY_EXIT_LINEIN_MODE,
	LINE_IN_KEY_EVENT_KEY_LINEIN_MUTE_TOGGLE,
	LINE_IN_KEY_EVENT_NO,
};

enum
{
	TESTMODE_KEY_EVENT_KEY_ENTER_TESTMODE,
	TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_CHANNEL,
	TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_POWER,
	TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_STATE,
	TESTMODE_KEY_EVENT_KEY_SWITCH_TEST_MODE_TX_PACKET_TYPE,
	TESTMODE_KEY_EVENT_NO,
};

enum
{
	HID_KEY_EVENT_KEY_SELFIE,
	HID_KEY_EVENT_SWITCH_SELFIE,
	HID_KEY_EVENT_NO,
};

enum
{
	AMA_KEY_EVENT_KEY_SPEECH_TOGGLE,
	AMA_KEY_EVENT_KEY_SPEECH_START,
	AMA_KEY_EVENT_KEY_SPEECH_STOP,
	AMA_KEY_EVENT_NO,
};

enum
{
    MP3_KEY_EVENT_KEY_MP3_POWERON, 							//KEY_MP3_POWERON
    MP3_KEY_EVENT_KEY_MP3_POWEROFF, 							//KEY_MP3_POWEROFF
    MP3_KEY_EVENT_KEY_MP3_PLAY, 								//KEY_MP3_PLAY
    MP3_KEY_EVENT_KEY_MP3_STOP, 								//KEY_MP3_STOP							//0x004A
    MP3_KEY_EVENT_KEY_MP3_PAUSE, 							//KEY_MP3_PAUSE
    MP3_KEY_EVENT_KEY_MP3_FORWARD, 							//KEY_MP3_FORWARD
    MP3_KEY_EVENT_KEY_MP3_BACKWARD, 							//KEY_MP3_BACKWARD
    MP3_KEY_EVENT_NO,
};

enum
{
	GVA_KEY_EVENT_PRESS,
	GVA_KEY_EVENT_VOICE_QUERY,
	GVA_KEY_EVENT_NOTIFY,
	GVA_KEY_EVENT_CANCEL,
	GVA_KEY_EVENT_RELEASE,
	GVA_KEY_EVENT_NO,
};

enum
{
	XIAOAI_KEY_EVENT_KEY_FUNC_2,
	XIAOAI_KEY_EVENT_KEY_SPEECH_START,
	XIAOAI_KEY_EVENT_KEY_FUNC_1,
	XIAOAI_KEY_EVENT_NO,
};

/**************************************************************************************************
* Structures
**************************************************************************************************/
typedef struct
{
	void (*fKeyEvt)(BD_ADDR_T *pBdAddr);
}APP_KEY_EVT_STRU;//MMI_KEY_EVT

typedef struct
{
	U8 keyEventGroup;
	U16 keyEventIndex;
}KEY_EVT_MAP_TYPE_STRU;//KeyEvtMapType

typedef struct
{
	U16 (*fKeyCheckValid)(U16 keyEventCode);
}APP_KEY_CHECK_STRU;//MMI_KEY_CHECK

#endif //_APP_EVENTOPCODE_H_

