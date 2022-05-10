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
#include "App_SystemKey.h"
#include "App_EventOpcode.h"
#include "App_ReConnection.h"
#include "App_State.h"
#include "App_Interface.h"
#include "App_Connection.h"
#include "App_VolumeManage.h"
#include "App_Media.h"
#include "App_PowerOn.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "AudioDSP_Registration.h"
#include "App_ChargeBattery.h"
#include "App_LinkKey.h"
#include "App_Nvram_Type.h"
#include "App_A2dp.h"
#include "App_VpKey.h"
#include "App_Media.h"
#include "APP_HfpScoHandler.h"
#include "AudioDSP_Vp.h"
#include "System_Nvkey.h"
#include "App_System.h"
#include "App_PeqManage.h"
#include "AudioDSP_StreamManager.h"
#include "App_Pairing.h"
#include "APP_AudioDriver.h"
#include "drv_mic_to_spk.h"
#ifdef AIROHA_WWE_ENABLE
#include "App_Vad.h"
#endif
#include "drv_audio_nvkey.h"
#include "drv_sco.h"
#include "DSP_ANC_Control.h"
#include "AudioDSP_AncPassThru.h"
#include "APP_AudioDriver.h"
#ifdef PROFILE_AMA_ENABLE
#include "BtAma.h"
#endif
<<<<<<< HEAD
=======

#ifdef AIR_GFP_ENABLE
#include "App_Gfp.h"
#endif
>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_SystemKey_ReconnectUserInit(BD_ADDR_T *pBdAddr);
static void app_SystemKey_Discoverable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_NonDiscoverable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ConnectedToDiscoverable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ConnectedToConnectable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_NFCDiscoverable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ConfirmationAccept(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ConfirmationReject(BD_ADDR_T *pBdAddr);
static void app_SystemKey_DbgKeyToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoiceUp(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoiceDown(BD_ADDR_T *pBdAddr);
static void app_SystemKey_BuzzerMuteToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ToggleVolume(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VolOrientationNormal(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VolOrientationInvert(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ToggleLED(BD_ADDR_T *pBdAddr);
static void app_SystemKey_EnableLED(BD_ADDR_T *pBdAddr);
static void app_SystemKey_DisableLED(BD_ADDR_T *pBdAddr);
static void app_SystemKey_CancelLEDIndication(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MediaTrigger(BD_ADDR_T *pBdAddr);
static void app_SystemKey_PowerOnDiscoverable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_PowerOn(BD_ADDR_T *pBdAddr);
static void app_SystemKey_PowerOff(BD_ADDR_T *pBdAddr);
static void app_SystemKey_Reset(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ResetPairedDevices(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MaxDeviceAllowUp(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MaxDeviceAllowDown(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MaxDeviceAllowToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ReconnectAfterPowerOn(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptEnable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptDisable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptLanChange(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptLanSel(BD_ADDR_T *pBdAddr);
static void app_SystemKey_VoicePromptLanCfm(BD_ADDR_T *pBdAddr);
static void app_SystemKey_TestVoicePromptPlay(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ButtonLockOn(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ButtonLockOff(BD_ADDR_T *pBdAddr);
static void app_SystemKey_ButtonLockToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicUp(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicDown(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicMuteToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicMuteOn(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicMuteOff(BD_ADDR_T *pBdAddr);
static void app_SystemKey_SCOSwitch(BD_ADDR_T *pBdAddr);
static void app_SystemKey_PEQGroupChange(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MultiPlayModeToggle(BD_ADDR_T *pBdAddr);
static void app_SystemKey_CheckStatusOfConnection(BD_ADDR_T *pBdAddr);
static void app_SystemKey_EnableDUTModeCMD(BD_ADDR_T *pBdAddr);//MMI_DRV_EnableDUTModeCMD
static void app_SystemKey_MicEnableL(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicEnableR(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicToSpkEnable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_MicToSpkDisable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_EnableECNR(BD_ADDR_T *pBdAddr);
static void app_SystemKey_DisableECNR(BD_ADDR_T *pBdAddr);
static void app_SystemKey_2KToneEnable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_2KToneDisable(BD_ADDR_T *pBdAddr);
static void app_SystemKey_SwitchStereoMono(BD_ADDR_T *pBdAddr);
static void app_SystemKey_AiroThru(BD_ADDR_T *pBdAddr);
static void app_SystemKey_RoleChange(BD_ADDR_T *pBdAddr);
#ifdef ANC_FF_MMI_CTRL
static void app_SystemKey_AncSwitch(BD_ADDR_T *pBdAddr);
static void app_SystemKey_AncOn(BD_ADDR_T *pBdAddr);
static void app_SystemKey_AncOff(BD_ADDR_T *pBdAddr);
#endif//ANC_FF_MMI_CTRL
static void app_SystemKey_DspReset();
static void app_SystemKey_ResetToFactory(BD_ADDR_T * pBdAddr);
static void app_SystemKey_ReportBatStatus(BD_ADDR_T * pBdAddr);
#ifdef AIROHA_WWE_ENABLE
static void app_SystemKey_VadOn(BD_ADDR_T * pBdAddr);
static void app_SystemKey_VadOff(BD_ADDR_T * pBdAddr);
#endif
static void app_SystemKey_GameModeOn(BD_ADDR_T * pBdAddr);
static void app_SystemKey_GameModeOff(BD_ADDR_T * pBdAddr);
static void app_SystemKey_GameModeToggle(BD_ADDR_T * pBdAddr);
static void app_SystemKey_MicToSpk3MicMainEnable(BD_ADDR_T * pBdAddr);
static void app_SystemKey_MicToSpk3MicRef1Enable(BD_ADDR_T * pBdAddr);
static void app_SystemKey_MicToSpk3MicRef2Enable(BD_ADDR_T * pBdAddr);
static void app_SystemKey_MicToSpk3MicDisable(BD_ADDR_T * pBdAddr);

static U16 app_SystemKey_CheckKeyReconnect(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyVoicePrompt(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyDiscoverable(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyVoiceUpDown(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyPowerOff(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyMicUpDown(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyMuteOnOff(U16 keyEventCode);
static U16 app_SystemKey_CheckKeySCOSwitch(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyUnderA2DP(U16 keyEventCode);
static U16 app_SystemKey_CheckKeyAiroThru(U16 keyEventCode);
#ifdef ANC_FF_MMI_CTRL
static U16 app_SystemKey_CheckKeyAncOn(U16 keyEventCode);
#endif
#ifdef AIROHA_WWE_ENABLE
static U16 app_SystemKey_CheckKeyVadOff(U16 keyEventCode);
#endif
/**************************************************************************************************
* Define
**************************************************************************************************/

/**************************************************************************************************
* Variables
**************************************************************************************************/
static APP_KEY_EVT_STRU const gSystemKeyEvtHandler[SYSTEM_KEY_EVENT_NO] =  //MMI_DRV_KeyEvtHandler
{
	{app_SystemKey_ReconnectUserInit}, 			//SYSTEM_KEY_EVENT_KEY_RECONNECT_USER_INIT
	{app_SystemKey_Discoverable}, 				//SYSTEM_KEY_EVENT_KEY_DISCOVERABLE,
	{app_SystemKey_NonDiscoverable}, 			//SYSTEM_KEY_EVENT_KEY_NONDISCOVERABLE,
	{app_SystemKey_ConnectedToDiscoverable}, 	//SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_DISCOVERABLE,
	{app_SystemKey_ConnectedToConnectable}, 	//SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_CONNECTABLE,   //0x05
	{app_SystemKey_NFCDiscoverable}, 			//SYSTEM_KEY_EVENT_KEY_NFC_DISCOVREABLE,
	{app_SystemKey_ConfirmationAccept},			//SYSTEM_KEY_EVENT_KEY_CONFIRMATION_ACCEPT,
	{app_SystemKey_ConfirmationReject}, 		//SYSTEM_KEY_EVENT_KEY_CONFIRMATION_REJECT,
	{app_SystemKey_DbgKeyToggle}, 				//SYSTEM_KEY_EVENT_KEY_DEBUG_KEY_TOGGLE,
	{app_SystemKey_VoiceUp}, 					//SYSTEM_KEY_EVENT_KEY_VOICEUP,	  //0x0a
	{app_SystemKey_VoiceDown}, 					//SYSTEM_KEY_EVENT_KEY_VOICEDN,
	{app_SystemKey_BuzzerMuteToggle}, 			//SYSTEM_KEY_EVENT_KEY_BUZZER_MUTE_TOGGLE,
	{app_SystemKey_ToggleVolume}, 				//SYSTEM_KEY_EVENT_KEY_TOGGLE_VOLUME,
	{app_SystemKey_VolOrientationNormal}, 		//SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_NORMAL,    //0x0F
	{app_SystemKey_VolOrientationInvert}, 		//SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_INVERT,   //0x10
	{app_SystemKey_ToggleLED}, 					//SYSTEM_KEY_EVENT_KEY_TOGGLE_LED,
	{app_SystemKey_EnableLED}, 					//SYSTEM_KEY_EVENT_KEY_ENABLE_LED,
	{app_SystemKey_DisableLED}, 				//SYSTEM_KEY_EVENT_KEY_DISABLE_LED,
	{app_SystemKey_CancelLEDIndication}, 		//SYSTEM_KEY_EVENT_KEY_CANCEL_LED_INDICATION,
	{app_SystemKey_MediaTrigger}, 				//SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_1,	//0x15
	{app_SystemKey_MediaTrigger}, 				//SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_2,
	{app_SystemKey_PowerOnDiscoverable}, 		//SYSTEM_KEY_EVENT_KEY_POWER_ON_THEN_ENTERDISCOVERABLE, // For NFC
	{app_SystemKey_PowerOn}, 					//SYSTEM_KEY_EVENT_KEY_POWER_ON,
	{app_SystemKey_PowerOff}, 					//SYSTEM_KEY_EVENT_KEY_POWER_OFF,
	{app_SystemKey_Reset}, 						//SYSTEM_KEY_EVENT_KEY_RESET,		//0x1a
	{app_SystemKey_ResetPairedDevices}, 		//SYSTEM_KEY_EVENT_KEY_RESET_PAIRED_DEVICES,  //wait ringtone voice prompt
	{app_SystemKey_MaxDeviceAllowUp}, 			//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_UP,
	{app_SystemKey_MaxDeviceAllowDown}, 		//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_DOWN,
	{app_SystemKey_MaxDeviceAllowToggle}, 		//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_TOGGLE,
	{app_SystemKey_ReconnectAfterPowerOn}, 		//SYSTEM_KEY_EVENT_KEY_RECONNECT_AFTER_POWER_ON,		//0x1F
	{app_SystemKey_VoicePromptToggle}, 			//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_TOGGLE,		//0x20
	{app_SystemKey_VoicePromptEnable}, 			//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_ENABLE,
	{app_SystemKey_VoicePromptDisable}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_DISABLE,
	{app_SystemKey_VoicePromptLanChange}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CHANGE,
	{app_SystemKey_VoicePromptLanSel}, 			//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_SELECT,
	{app_SystemKey_VoicePromptLanCfm}, 			//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CONFIRM,		//0x25
	{app_SystemKey_TestVoicePromptPlay}, 		//SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_PLAY,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_NEXT,
	{app_SystemKey_ButtonLockOn},				//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKON,
	{app_SystemKey_ButtonLockOff}, 				//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKOFF,
	{app_SystemKey_ButtonLockToggle}, 			//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKTOGGLE,			//0x2a
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_ENABLE,
	{app_SystemKey_MicUp}, 						//SYSTEM_KEY_EVENT_KEY_MICUP,		//0x40
	{app_SystemKey_MicDown}, 					//SYSTEM_KEY_EVENT_KEY_MICDN,		 //0x41
	{app_SystemKey_MicMuteToggle}, 				//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_TOGGLE,
	{app_SystemKey_MicMuteOn}, 					//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_ON,
	{app_SystemKey_MicMuteOff}, 				//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_OFF,
	{app_SystemKey_SCOSwitch},					//SYSTEM_KEY_EVENT_KEY_SCO_SWITCH,
	{app_SystemKey_PEQGroupChange}, 			//SYSTEM_KEY_EVENT_KEY_PEQ_GROUP_CHANGE
	{app_SystemKey_MultiPlayModeToggle},		//SYSTEM_KEY_EVENT_KEY_MULTI_A2DP_PLAY_MODE_TOGGLE
	{app_SystemKey_CheckStatusOfConnection}, 	//SYSTEM_KEY_EVENT_KEY_CHECK_STATUS_OF_CONNECTION
	{NULL},										//SYSTEM_KEY_EVENT_KEY_WHAT_CAN_I_SAY
	{NULL},										//SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_CANCEL
	{NULL},										//SYSTEM_KEY_EVENT_KEY_VOICE_MAKE_A_CALL
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CHECK_STATUS
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CHANGE_SOUND_EFFECT
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CONTROL_DBB_ON_OFF
	#ifdef SUPPORT_MIC_IN_A2DP_MODE
	{app_SystemKey_ControlMicInA2DPMode}, 		//SYSTEM_KEY_CONTROL_MIC_A2DP_MODE
	#else
	{NULL},										//SYSTEM_KEY_CONTROL_MIC_A2DP_MODE
	#endif
	{app_SystemKey_EnableDUTModeCMD}, 			//SYSTEM_KEY_ENABLE_DUT_MODE_COMMAND
	{app_SystemKey_MicEnableL}, 				//SYSTEM_KEY_EVENT_KEY_MIC_L_ENABLE
	{app_SystemKey_MicEnableR}, 				//SYSTEM_KEY_EVENT_KEY_MIC_R_ENABLE
	{app_SystemKey_MicToSpkEnable}, 			//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_ENABLE
	{app_SystemKey_MicToSpkDisable},			//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_DISABLE
	{app_SystemKey_EnableECNR}, 				//SYSTEM_KEY_EVENT_KEY_ENABLE_EC_NR
	{app_SystemKey_DisableECNR}, 				//SYSTEM_KEY_EVENT_KEY_DISABLE_EC_NR
	{app_SystemKey_2KToneEnable}, 				//SYSTEM_KEY_EVENT_KEY_2K_TONE_ENABLE
	{app_SystemKey_2KToneDisable}, 				//SYSTEM_KEY_EVENT_KEY_2K_TONE_DISABLE
	{app_SystemKey_SwitchStereoMono}, 			//SYSTEM_KEY_EVENT_KEY_SWITCH_STEREO_MONO
	{app_SystemKey_AiroThru}, 					//SYSTEM_KEY_EVENT_KEY_AIROTHRU
	{app_SystemKey_RoleChange}, 				//SYSTEM_KEY_EVENT_KEY_ROLE_CHANGE
#ifdef ANC_FF_MMI_CTRL
	{app_SystemKey_AncSwitch}, 					//SYSTEM_KEY_EVENT_KEY_ANC_SWITCH
#else
	{NULL},
#endif//ANC_FF_MMI_CTRL
	{app_SystemKey_ResetToFactory},				//SYSTEM_KEY_EVENT_KEY_RESET_TO_FACTORY
#ifdef ANC_FF_MMI_CTRL
	{app_SystemKey_AncOn}, 						//SYSTEM_KEY_EVENT_KEY_ANC_ON
	{app_SystemKey_AncOff}, 					//SYSTEM_KEY_EVENT_KEY_ANC_OFF
#else
	{NULL},
	{NULL},
#endif//ANC_FF_MMI_CTRL

	{app_SystemKey_DspReset},					//SYSTEM_KEY_EVENT_KEY_DSP_RESET
	{app_SystemKey_ReportBatStatus},			//SYSTEM_KEY_EVENT_KEY_REPORT_BATTERY_STATUS
#ifdef AIROHA_WWE_ENABLE
    {app_SystemKey_VadOn},                      //SYSTEM_KEY_EVENT_KEY_VAD_ON
    {app_SystemKey_VadOff},                     //SYSTEM_KEY_EVENT_KEY_VAD_OFF
#else
    {NULL},                                     //SYSTEM_KEY_EVENT_KEY_VAD_ON
    {NULL},                                     //SYSTEM_KEY_EVENT_KEY_VAD_OFF
#endif
	{app_SystemKey_GameModeOn},					//SYSTEM_KEY_EVENT_KEY_GAME_MODE_ON
	{app_SystemKey_GameModeOff},				//SYSTEM_KEY_EVENT_KEY_GAME_MODE_OFF
	{app_SystemKey_GameModeToggle},				//SYSTEM_KEY_EVENT_KEY_GAME_MODE_TOGGLE
<<<<<<< HEAD
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_STARTS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_STOPS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_STARTS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_STOPS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_TOGGLE_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_TOGGLE_SHARE_MODE
=======
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_START
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_STOP
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_START
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_STOP
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MASTER_SHARE_MODE_TOGGLE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_SHARE_MODE_TOGGLE
>>>>>>> db20e11 (second commit)
	{app_SystemKey_MicToSpk3MicMainEnable},		//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_MAIN_MIC_ENABLE
	{app_SystemKey_MicToSpk3MicRef1Enable},		//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF1_MIC_ENABLE
	{app_SystemKey_MicToSpk3MicRef2Enable},		//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF2_MIC_ENABLE
	{app_SystemKey_MicToSpk3MicDisable},		//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_DISABLE
};

static APP_KEY_CHECK_STRU const gSystemKeyCheckValidHandler[SYSTEM_KEY_EVENT_NO] = //MMI_DRV_KeyCheckValidHandler
{
	{app_SystemKey_CheckKeyReconnect}, 			//SYSTEM_KEY_EVENT_KEY_RECONNECT_USER_INIT
	{app_SystemKey_CheckKeyDiscoverable}, 		//SYSTEM_KEY_EVENT_KEY_DISCOVERABLE,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_NONDISCOVERABLE,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_DISCOVERABLE,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CONNECTED_TO_CONNECTABLE,   //0x05
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_NFC_DISCOVREABLE,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CONFIRMATION_ACCEPT,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CONFIRMATION_REJECT,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_DEBUG_KEY_TOGGLE,
	{app_SystemKey_CheckKeyVoiceUpDown}, 		//SYSTEM_KEY_EVENT_KEY_VOICEUP,	  //0x0a
	{app_SystemKey_CheckKeyVoiceUpDown}, 		//SYSTEM_KEY_EVENT_KEY_VOICEDN,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_BUZZER_MUTE_TOGGLE,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_TOGGLE_VOLUME,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_NORMAL,    //0x0F
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_VOLUME_ORIENTATION_INVERT,   //0x10
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_TOGGLE_LED,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_ENABLE_LED,
	{NULL},										//SYSTEM_KEY_EVENT_KEY_DISABLE_LED,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CANCEL_LED_INDICATION,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_1,	//0x15
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MEDIA_TRIGGER_2,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_POWER_ON_THEN_ENTERDISCOVERABLE, // For NFC
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_POWER_ON,
	{app_SystemKey_CheckKeyPowerOff}, 			//SYSTEM_KEY_EVENT_KEY_POWER_OFF,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_RESET,		//0x1a
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_RESET_PAIRED_DEVICES,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_UP,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_DOWN,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MAX_DEVICE_ALLOWED_TOGGLE,
	{app_SystemKey_CheckKeyReconnect}, 			//SYSTEM_KEY_EVENT_KEY_RECONNECT_AFTER_POWER_ON,		//0x1F
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_TOGGLE,		//0x20
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_ENABLE,
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_DISABLE,
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CHANGE,
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_SELECT,
	{app_SystemKey_CheckKeyVoicePrompt}, 		//SYSTEM_KEY_EVENT_KEY_VOICE_PROMPT_LANG_CONFIRM,		//0x25
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_PLAY,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_TEST_VOICE_PROMPT_NEXT,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKON,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKOFF,
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_BUTTONLOCKTOGGLE,			//0x2a
	{NULL},                                     //SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_ENABLE,
	{app_SystemKey_CheckKeyMicUpDown}, 			//SYSTEM_KEY_EVENT_KEY_MICUP,		//0x40
	{app_SystemKey_CheckKeyMicUpDown}, 			//SYSTEM_KEY_EVENT_KEY_MICDN,		 //0x41
	{app_SystemKey_CheckKeyMuteOnOff}, 			//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_TOGGLE,
	{app_SystemKey_CheckKeyMuteOnOff}, 			//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_ON,
	{app_SystemKey_CheckKeyMuteOnOff}, 			//SYSTEM_KEY_EVENT_KEY_MIC_MUTE_OFF,
	{app_SystemKey_CheckKeySCOSwitch},			//SYSTEM_KEY_EVENT_KEY_SCO_SWITCH
	{NULL},										//SYSTEM_KEY_EVENT_KEY_PEQ_GROUP_CHANGE
	{app_SystemKey_CheckKeyUnderA2DP}, 			//SYSTEM_KEY_EVENT_KEY_MULTI_A2DP_PLAY_MODE_TOGGLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_CHECK_STATUS_OF_CONNECTION
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_WHAT_CAN_I_SAY
	{NULL},										//SYSTEM_KEY_EVENT_KEY_VOICE_COMMAND_CANCEL
	{NULL},										//SYSTEM_KEY_EVENT_KEY_VOICE_MAKE_A_CALL
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CHECK_STATUS
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CHANGE_SOUND_EFFECT
	{NULL},										//SYSTEM_KEY_EVENT_KEY_CONTROL_DBB_ON_OFF
	{NULL}, 									//SYSTEM_KEY_CONTROL_MIC_A2DP_MODE
	{NULL}, 									//SYSTEM_KEY_ENABLE_DUT_MODE_COMMAND
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MIC_L_ENABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MIC_R_ENABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_ENABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_DISABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_ENABLE_EC_NR
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_DISABLE_EC_NR
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_2K_TONE_ENABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_2K_TONE_DISABLE
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_SWITCH_STEREO_MONO
	{app_SystemKey_CheckKeyAiroThru}, 			//SYSTEM_KEY_EVENT_KEY_AIROTHRU
	{NULL}, 									//SYSTEM_KEY_EVENT_KEY_ROLE_CHANGE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_ANC_SWITCH
	{NULL},										//SYSTEM_KEY_RESET_TO_FACTORY
#ifdef ANC_FF_MMI_CTRL
	{app_SystemKey_CheckKeyAncOn},			//SYSTEM_KEY_EVENT_KEY_ANC_ON
	{NULL},										//SYSTEM_KEY_EVENT_KEY_ANC_OFF
#else
	{NULL},
	{NULL},
#endif

	{NULL},										//SYSTEM_KEY_EVENT_KEY_DSP_RESET
	{NULL},										//SYSTEM_KEY_EVENT_KEY_REPORT_BATTERY_STATUS
#ifdef AIROHA_WWE_ENABLE
    {NULL},                                     //SYSTEM_KEY_EVENT_KEY_VAD_ON
    {app_SystemKey_CheckKeyVadOff},             //SYSTEM_KEY_EVENT_KEY_VAD_OFF
#else
    {NULL},                                     //SYSTEM_KEY_EVENT_KEY_VAD_ON
    {NULL},                                     //SYSTEM_KEY_EVENT_KEY_VAD_OFF
#endif
	{NULL},										//SYSTEM_KEY_EVENT_KEY_GAME_MODE_ON
	{NULL},										//SYSTEM_KEY_EVENT_KEY_GAME_MODE_OFF
	{NULL},										//SYSTEM_KEY_EVENT_KEY_GAME_MODE_TOGGLE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_STARTS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_STOPS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_STARTS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_STOPS_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_AGENT_TOGGLE_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_FOLLOWER_TOGGLE_SHARE_MODE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_MAIN_MIC_ENABLE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF1_MIC_ENABLE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_MIC_TO_SPK_3MIC_REF2_MIC_ENABLE
	{NULL},										//SYSTEM_KEY_EVENT_KEY_TO_SPK_3MIC_DISABLE
};


/**************************************************************************************************
* static Functions (Key)
**************************************************************************************************/
static void app_SystemKey_ReconnectUserInit(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_ReConn_Reconnect(RECONNECT_USER_INIT);
}

static void app_SystemKey_Discoverable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
<<<<<<< HEAD
=======
    if (CURRENT_ACTIVE_LINK_CNT)
    {
        /* Pressed when there is at least one SP connected. */
        APP_SetKeepDiscoverableAfterDisc(TRUE);
    }
>>>>>>> db20e11 (second commit)
	APP_System_Discoverable();
}

static void app_SystemKey_NonDiscoverable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
	{
		APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
	}
}

static void app_SystemKey_ConnectedToDiscoverable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ReConn_ClearCtl();
	APP_Conn_ReleaseMultiLink();
	if(!CURRENT_ACTIVE_LINK_CNT)
	{
		APP_Pairing_EnterDiscoverable();
	}
	else
	{
		APP_SetEnterDiscoverableAfterDisc(TRUE);
	}
}

static void app_SystemKey_ConnectedToConnectable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ReConn_ClearCtl();
	APP_Conn_ReleaseMultiLink();
}

static void app_SystemKey_NFCDiscoverable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	//if(CURRENT_ACTIVE_LINK_CNT)
	//{
	//	U8 i;
	//	for(i = 0; i < FW_MAX_LINK_IDX; i++)
	//	{
	//		BD_ADDR_T *pBdAddr = APP_GetBdAddrByLinkIndex(i);
	//		if(pBdAddr && APP_IsActiveLink(pBdAddr))
	//		{
	//			SYS_SetTimer(&gAppCtl.mmiTimerInfo.connTimer[i].nfcDetachIfNoProfileTimer,3*ONE_SEC);
	//		}
	//	}
	//}

	APP_State_DiscoverabilityHandle(CMD_SET_NORMAL_DISCOVERABLE);
}

static void app_SystemKey_ConfirmationAccept(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

static void app_SystemKey_ConfirmationReject(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

static void app_SystemKey_DbgKeyToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	gAppCtl.driverCtl.dbgPrivateKey = ~gAppCtl.driverCtl.dbgPrivateKey;
	//MMI_HCI_SendWriteSimplePairingDebugMode(gAppCtl.driverCtl.dbgPrivateKey);
}

static void app_SystemKey_VoiceUp(BD_ADDR_T *pBdAddr)
{
	APP_VolManage_SpkVolumeUpDownHandler(pBdAddr, KEY_VOICEUP, FALSE);
}

static void app_SystemKey_VoiceDown(BD_ADDR_T *pBdAddr)
{
	APP_VolManage_SpkVolumeUpDownHandler(pBdAddr, KEY_VOICEDN, FALSE);
}

static void app_SystemKey_BuzzerMuteToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

static void app_SystemKey_ToggleVolume(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled ^= IS_VOLUME_ORIENTATION_REVERSED;
	//MMI_SECTOR_SetUpdateFlag(TRUE);
}

static void app_SystemKey_VolOrientationNormal(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled &= ~(IS_VOLUME_ORIENTATION_REVERSED);
	//MMI_SECTOR_SetUpdateFlag(TRUE);
}

static void app_SystemKey_VolOrientationInvert(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled |= IS_VOLUME_ORIENTATION_REVERSED;
	//MMI_SECTOR_SetUpdateFlag(TRUE);
}

static void app_SystemKey_ToggleLED(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//MMI_LED_KeyOnOffHandler(TOGGLE_LED);
}

static void app_SystemKey_EnableLED(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//MMI_LED_KeyOnOffHandler(ENABLE_LED);
}

static void app_SystemKey_DisableLED(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//MMI_LED_KeyOnOffHandler(DISABLE_LED);
}

static void app_SystemKey_CancelLEDIndication(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//DRIVER_Cmd_LED_Restart(DRIVER_CMD_LED_RESTART_PARA_ONLY_FG);
}

static void app_SystemKey_MediaTrigger(BD_ADDR_T *pBdAddr)
{
	if (APP_HFP_INCOMMING == APP_State_GetTopState(pBdAddr))
	{
		APP_AudioDriver_SpecificEventVPStopCmd(MEDIA_EVT_ENTER_INCOMING);
	}
}

static void app_SystemKey_PowerOnDiscoverable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_PowerOn_SetEnterDiscoverable(TRUE);
	if(!APP_PowerOff_IsPowerOn())
	{
		APP_PowerOn_Loader();
	}
}

static void app_SystemKey_PowerOn(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(!APP_PowerOff_IsPowerOn())
	{
		APP_PowerOn_Loader();
	}
}

static void app_SystemKey_PowerOff(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_PowerOff_Loader(POWEROFF_REASON_KEY);
	#ifdef OLED_ENABLE
//	OLED_DisplayOff();
	#endif
}

static void app_SystemKey_Reset(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_Reset_Loader();
}

static void app_SystemKey_ResetPairedDevices(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ReConn_ClearCtl();
	APP_Conn_ReleaseMultiLink();

	if(LINK_DISCONNECTED == APP_Conn_GetAgLinkState())
		APP_LinkKey_ClearAllHistory();
	else
		APP_ConnSetTimerWaitDisconnectedClearLinkKey();

	APP_System_MpTestModeSetToDefault();
}

static void app_SystemKey_ResetToFactory(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	app_SystemKey_ResetPairedDevices(NULL);
	APP_VpSetLanguageToDefault();
	APP_System_SetToDefaultNvkey();

	if(!System_WriteNvkeyDeviceNameToDefault())
	{
		DBG_LOG_APP_SYSTEM( "[APP_SystemKey] Reset device name successful", 0);
		APP_System_SetBtServiceClass();
	}
	else
	{
		DBG_LOG_APP_SYSTEM( "[APP_SystemKey] Reset device name fail", 0);
	}

	#ifdef PROFILE_AMA_ENABLE
	AMA_Target_FactoryReset();
	#endif
<<<<<<< HEAD
=======
    #ifdef AIR_GFP_ENABLE
    APP_Gfp_ResetToFactory();
    #endif
>>>>>>> db20e11 (second commit)
}


static void app_SystemKey_MaxDeviceAllowUp(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_Conn_SetSupportDeviceNo(TRUE);
}

static void app_SystemKey_MaxDeviceAllowDown(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_Conn_SetSupportDeviceNo(FALSE);
}

static void app_SystemKey_MaxDeviceAllowToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(APP_Conn_GetSupportDeviceNo() < APP_MAX_PAGE_DEVICE)
	{
		APP_Conn_SetSupportDeviceNo(TRUE);
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_MAX_DEVICE_ALLOWED_UP);
	}
	else
	{
		APP_Conn_SetSupportDeviceNo(FALSE);
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_MAX_DEVICE_ALLOWED_DOWN);
	}
}

static void app_SystemKey_ReconnectAfterPowerOn(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ReConn_Reconnect(RECONNECT_PWR_ON);
}

static void app_SystemKey_VoicePromptToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if(APP_VpIsVoicePromptEnabled())
	{
		APP_VpDisableVoicePrompt();
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_VOICE_PROMPT_DISABLE);
	}
	else
	{
		APP_VpEnableVoicePrompt();
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_VOICE_PROMPT_ENABLE);
	}
}

static void app_SystemKey_VoicePromptEnable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_VpEnableVoicePrompt();
}

static void app_SystemKey_VoicePromptDisable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_VpDisableVoicePrompt();
}

static void app_SystemKey_VoicePromptLanChange(BD_ADDR_T *pBdAddr)
{

	UNUSED(pBdAddr);

	APP_VpSetLanguageNext();
}

static void app_SystemKey_VoicePromptLanSel(BD_ADDR_T *pBdAddr)
{
	if(APP_CONDISCABLE == APP_State_GetTopState(pBdAddr))
	{
		APP_State_AddTopState(APP_EOF, APP_VOICE_PROMPT_LANG_SELECT);
	}
}

static void app_SystemKey_VoicePromptLanCfm(BD_ADDR_T *pBdAddr)
{
	if(APP_VOICE_PROMPT_LANG_SELECT == APP_State_GetTopState(pBdAddr))
	{
		gAppCtl.vpRuntime.vpLangIsCfm = TRUE;
		APP_State_RmvState(APP_EOF, APP_VOICE_PROMPT_LANG_SELECT);
	}
}

static void app_SystemKey_TestVoicePromptPlay(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_Vp_KeyPlayAll();
}

static void app_SystemKey_ButtonLockOn(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_SetButtonLock(TRUE);
}

static void app_SystemKey_ButtonLockOff(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_SetButtonLock(FALSE);
}

static void app_SystemKey_ButtonLockToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	if (APP_IsButtonLocked())
	{
		APP_SetButtonLock(FALSE);
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_BUTTONLOCKOFF);
	}
	else
	{
		APP_SetButtonLock(TRUE);
		APP_Media_PushMediaEvent(MEDIA_EVT_KEY_BUTTONLOCKON);
	}
}

static void app_SystemKey_MicUp(BD_ADDR_T *pBdAddr)
{
	APP_VolManage_MicVolumeUpDownHandler(pBdAddr, KEY_MICUP);
}

static void app_SystemKey_MicDown(BD_ADDR_T *pBdAddr)
{
	APP_VolManage_MicVolumeUpDownHandler(pBdAddr, KEY_MICDN);
}

static void app_SystemKey_MicMuteToggle(BD_ADDR_T *pBdAddr)
{
	APP_VolManage_MicMuteHandler(pBdAddr, TOGGLE_MUTE_ON_OFF);
}

static void app_SystemKey_MicMuteOn(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_VolManage_MicMuteHandler(pBdAddr, SET_MUTE_ON);
}

static void app_SystemKey_MicMuteOff(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_VolManage_MicMuteHandler(pBdAddr, SET_MUTE_OFF);
}

static void app_SystemKey_SCOSwitch(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T * pOtherBdAddr;
	pOtherBdAddr = APP_GetNextNormalLink(pBdAddr);

	if(pOtherBdAddr && APP_IsSCOGroupAudioDeviceRegistered(pOtherBdAddr))
	{
		//APP_SetSwitchingSCO(TRUE);
		//APP_HfpSco_SendDSPCmd(pOtherBdAddr);
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO_IDLE);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE        
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE        
>>>>>>> db20e11 (second commit)
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO_HSP);
#endif
		APP_ResumeOtherAudioDevice(pBdAddr, AUDIO_DEVICE_SCO);
	}
}

//#ifdef TWS_SETTINGS
//extern PUBLIC void MMI_Share_KeyChangePEQCmd(U8 linkIndex);
//#endif
static void app_SystemKey_PEQGroupChange(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

//#ifdef PEQ_ENABLE
//	#ifdef TWS_SETTINGS
//	if(A2DP_IsLinkFollower(linkIndex))
//	{
//		MMI_Share_KeyChangePEQCmd(linkIndex);
//	}
//	else
//	#endif
	{
		APP_PeqManage_ChangePeqGroup();
	}
//#endif

}

static void app_SystemKey_MultiPlayModeToggle(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//U16 feature = APP_FEATURE_CTL_A2DP^0x0202;
	//LightDbgPrint("USR - A2DP Feature:%X",(U32)MMI_FEATURE_CTL_A2DP);
}

static void app_SystemKey_CheckStatusOfConnection(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pBdAddr && PM_GetConnectedProfileCount(pBdAddr, TRUE))
		{
			APP_Media_PushMediaEvent(MEDIA_EVT_SLC_CONNECTED);
			return;
		}
	}
	APP_Media_PushMediaEvent(MEDIA_EVT_SLC_DISCONNECTED);
}

static void app_SystemKey_EnableDUTModeCMD(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	PM_GAP_EnableDutMode();
}

static void app_SystemKey_MicEnableL(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
    drv_sco_mic_switch(0);
}

static void app_SystemKey_MicEnableR(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
    drv_sco_mic_switch(1);
}

static void app_SystemKey_MicToSpkEnable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_start(TRUE, MICSPKTEST_MIC_TO_SPK);
}

static void app_SystemKey_MicToSpkDisable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_stop();
}

static void app_SystemKey_EnableECNR(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_HfpSco_SetEcnr(FALSE);
}

static void app_SystemKey_DisableECNR(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_HfpSco_SetEcnr(TRUE);
}

static void app_SystemKey_2KToneEnable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_start(TRUE, MICSPKTEST_TONE);
}

static void app_SystemKey_2KToneDisable(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_stop();
}

static void app_SystemKey_SwitchStereoMono(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	/*
		MMIMsgType XDATA_PTR msgPtr = MMI_GetMMIMsgTypeCmdMemory(DRIVER_DSP_SWITCH_STEREO_MONO_CMD);

		OSMQ_Put(OSMQ_DRIVER_Command_ptr, (U8 XDATA_PTR)msgPtr);
	*/
}

static void app_SystemKey_AiroThru(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ANCPASSTHRU_PARA_STRU para;
	para.action = ACTION_TOGGLE;
	para.sync = TRUE;
	para.userRegister = USER_PASSTHRU_OPEN;
	para.flash_no = FILTER_PASS_THRU_1;
	para.type = HAL_AUDIO_PASSTHRU_TYPE_FF;
	para.runtime_gain = 0;
	APP_ANCPassThruHandler(para);
    AudioDsp_ANCPassThruKeyTriggered();
}

static void app_SystemKey_RoleChange(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

#ifdef ANC_FF_MMI_CTRL
static void app_SystemKey_AncSwitch(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_ANCPASSTHRU_PARA_STRU para;
	para.action = ACTION_TOGGLE;
	para.sync = TRUE;
	para.userRegister = USER_ANC_OPEN;
	para.flash_no = FILTER_ANC_1;
	para.type = HAL_AUDIO_ANC_TYPE_HYBRID;
	para.runtime_gain = 0;
	APP_ANCPassThruHandler(para);
    AudioDsp_ANCPassThruKeyTriggered();
}

static void app_SystemKey_AncOn(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_ANCPASSTHRU_PARA_STRU para;
	para.action = ACTION_ON;
	para.sync = TRUE;
	para.userRegister = USER_ANC_OPEN;
	para.flash_no = FILTER_ANC_1;
	para.type = HAL_AUDIO_ANC_TYPE_HYBRID;
    audio_anc_get_runtime_gain(&para.runtime_gain);
	APP_ANCPassThruHandler(para);
    AudioDsp_ANCPassThruKeyTriggered();
}

static void app_SystemKey_AncOff(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_ANCPASSTHRU_PARA_STRU para;
	para.action = ACTION_OFF;
	para.sync = TRUE;
	para.userRegister = USER_ANC_PASSTHRU_CLOSE;
	para.flash_no = FILTER_ANC_1;
	para.type = HAL_AUDIO_ANC_TYPE_HYBRID;
	para.runtime_gain = 0;
	APP_ANCPassThruHandler(para);
    AudioDsp_ANCPassThruKeyTriggered();
}
#endif//ANC_FF_MMI_CTRL

static void app_SystemKey_DspReset()
{
	APP_AudioDSPRealtimeSuspend();
	APP_AudioDSPRealtimeResume();
}

static void app_SystemKey_ReportBatStatus(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	APP_ChgBat_ReportBatteryStatus();
}

#ifdef AIROHA_WWE_ENABLE
static void app_SystemKey_VadOn(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	APP_VAD_On();
}

static void app_SystemKey_VadOff(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	APP_VAD_Off();
}
#endif

static void app_SystemKey_GameModeOn(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	APP_A2dp_GameModeOn();
}

static void app_SystemKey_GameModeOff(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	APP_A2dp_GameModeOff();
}

static void app_SystemKey_GameModeToggle(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
    APP_A2dp_GameModeToggle();
}

static U16 app_SystemKey_CheckKeyReconnect(U16 keyEventCode)
{
	if(/*gAppCtl.reconnectCtl.disableReconnect || */CURRENT_ACTIVE_LINK_CNT >= APP_Conn_GetSupportDeviceNo())
	{
		DBG_LOG_APP_SYSTEM( "[APP_SystemKey] Max Link", 0);
		keyEventCode = KEY_INVALID;
	}
	return keyEventCode;
}

static void app_SystemKey_MicToSpk3MicMainEnable(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_start(FALSE, MICSPKTEST_MIC_TO_SPK_3MIC);
	drv_mic_to_spk_3mic_switch(MAIN_MIC);
}
static void app_SystemKey_MicToSpk3MicRef1Enable(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_start(FALSE, MICSPKTEST_MIC_TO_SPK_3MIC);
	drv_mic_to_spk_3mic_switch(REF1_MIC);
}

static void app_SystemKey_MicToSpk3MicRef2Enable(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_start(FALSE, MICSPKTEST_MIC_TO_SPK_3MIC);
	drv_mic_to_spk_3mic_switch(REF2_MIC);
}

static void app_SystemKey_MicToSpk3MicDisable(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
	drv_mic_to_spk_stop();
}

static U16 app_SystemKey_CheckKeyVoicePrompt(U16 keyEventCode)
{
	if(APP_VOICE_PROMPT_FUNC_OFF_FEAT)
	{
		keyEventCode = KEY_INVALID;
	}

    if(keyEventCode == KEY_VOICE_PROMPT_LANG_CHANGE && APP_AudioDspIsStreaming(NULL, AUDIO_VP))
    {
        keyEventCode = KEY_INVALID;
    }
	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyDiscoverable(U16 keyEventCode)
{
	if(CURRENT_ACTIVE_LINK_CNT == APP_Conn_GetSupportDeviceNo())
	{
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyVoiceUpDown(U16 keyEventCode)
{
	#ifdef FM_ENABLE
	if(APP_State_CheckInFMState())// && gMMI_FM_ctl.isDriverFMBusy)
	{
		keyEventCode = KEY_INVALID;
	}
	#endif

	if(APP_AudioDspIsStreaming(NULL, AUDIO_VP) /*|| APP_AudioDspIsStreaming(NULL, AUDIO_RT)*/)
	{
		keyEventCode = KEY_INVALID;
	}
    
    if(APP_AudioDspIsStreaming(NULL, AUDIO_RT) && MEDIA_EVT_PLAYING_BEEP_SYNC == APP_AudioDriver_GetPlayingID())
    {
        keyEventCode = KEY_INVALID;
    }


	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyPowerOff(U16 keyEventCode)
{
	if(APP_PowerOff_IsTimerSet(TIMER_DISPOWEROFF_AFTER_POWERON_TIMEOUT))
	{
		keyEventCode = KEY_INVALID;
	}
	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyMicUpDown(U16 keyEventCode)
{
	if(APP_AudioDspIsStreaming(NULL, AUDIO_VP) || APP_AudioDspIsStreaming(NULL, AUDIO_RT))
	{
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyMuteOnOff(U16 keyEventCode)
{
/*
	switch(DRIVER_CurrentPlayingMediaID())
	{
		case MEDIA_EVT_KEY_MIC_MUTE_ON:
		case MEDIA_EVT_KEY_MIC_MUTE_OFF:
			return KEY_INVALID;
		default:
			return keyEventCode;
	}
*/
	return keyEventCode;
}

static U16 app_SystemKey_CheckKeySCOSwitch(U16 keyEventCode)
{

	if(gAppCtl.scoCount <= 1 || gAppCtl.scoCount > FW_MAX_LINK_IDX /*|| APP_IsSwitchingSCO()*/)
	{
		return KEY_INVALID;
	}
	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyUnderA2DP(U16 keyEventCode)
{
	if(APP_IsActiveAudioDevInMusic())
	{
		return KEY_INVALID;
	}
	return keyEventCode;
}

static U16 app_SystemKey_CheckKeyAiroThru(U16 keyEventCode)
{
	if(!APP_AudioIsPermittedToPlay(AUDIO_ANC))
	{
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}

#ifdef ANC_FF_MMI_CTRL
static U16 app_SystemKey_CheckKeyAncOn(U16 keyEventCode)
{

	if(APP_AudioDspIsStreaming(NULL, AUDIO_ANC))
	{
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}
#endif


#ifdef AIROHA_WWE_ENABLE
static U16 app_SystemKey_CheckKeyVadOff(U16 keyEventCode)
{
	if(drv_audio_nvkey_is_feature_on(AUDIO_DSP_VAD_ALWAYS_ON_FEATURE))
	{
		keyEventCode = KEY_INVALID;
	}

	return keyEventCode;
}
#endif

/**************************************************************************************************
* Public Functions
**************************************************************************************************/

void APP_SystemKey_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U8 eventIndex)
{
	if(gSystemKeyEvtHandler[eventIndex].fKeyEvt)
	{
		gSystemKeyEvtHandler[eventIndex].fKeyEvt(pBdAddr);
	}
}

U16 APP_SystemKey_CheckKeyValid(U8 keyEventIndex, U16 keyEventCode)
{
	if(gSystemKeyCheckValidHandler[keyEventIndex].fKeyCheckValid)
	{
		keyEventCode = gSystemKeyCheckValidHandler[keyEventIndex].fKeyCheckValid(keyEventCode);
	}
	return keyEventCode;
}


