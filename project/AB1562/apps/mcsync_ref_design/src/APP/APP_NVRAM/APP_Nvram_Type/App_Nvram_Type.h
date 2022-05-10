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
 
 
#ifndef _APP_NVRAM_TYPE_H_
#define _APP_NVRAM_TYPE_H_

#include "bt.h"
#include "App_Interface.h"
#include "NvramType_Nvkey.h"

/**************************************************************************************************
* Define (Feature)
**************************************************************************************************/
#define APP_REBOOT_ON_RESET_LINK_HISTORY_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x01)
#define APP_DEBUG_PRIVATE_KEY_FEAT							(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x02)
#define APP_POWER_DROP_SAVE_LINK_HISTORY_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x08)
#define APP_FORCE_USE_ESCO_IN_AUDIO_TRANSFER_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x10)
#define APP_DISABLE_AG_ECNR_IN_SCO							(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x20)
#define APP_RESET_INSTEAD_OF_POWEROFF_FOR_REGRESSION_TEST	(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x40)
#define APP_FIX_ESD_FEAT									(APP_NvramType_FeturCtl_GetAppFeatureControl(system) & 0x80)							

#define APP_ENABLE_HANDLING_VGM_FEAT						(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0001)
#define APP_REMEMBER_SOUND_LEVEL_CHANGE_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0002)
#define APP_CYCLIC_VOLUME_ENABLED_FEAT						(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0004)
#define APP_VOICE_PROMPT_FUNC_OFF_FEAT						(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0010)
#define APP_READ_OUT_CALLER_ID_BY_VP_FEAT					(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0100)
#define APP_IGNORE_CALLER_VOICE_IF_OTHER_LINK_IN_CALL_FEAT	(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x0200)
#define APP_DO_NOT_RING_WHEN_WATCHDOG_TO_PWON_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(audio1) & 0x2000)

#define APP_PLAY_DEFAULT_RING_INCOMING_FEAT					(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x01)
#define APP_VP_RINGTONE_1OF2_FEAT							(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x02)
#define APP_QUEUE_RINGTONE_FEAT								(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x04)
#define APP_DISCARD_VP_WHEN_IN_CALLWITHSCO_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x08)
#define APP_PLAY_BEEP_ON_REMOTE_INDICATION_VGS_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x20)
#define APP_DISCARD_VP_WHEN_IN_MUSIC_FEAT					(APP_NvramType_FeturCtl_GetAppFeatureControl(audio2) & 0x40)

#define APP_MUTE_SPEAKER_AS_MICROPHONE_FEAT							(APP_FEATURE_CTL_AUDIO3 & 0x0002)
#define APP_CLOSE_AMP_WHILE_SPEAKER_MUTED_FEAT						(APP_FEATURE_CTL_AUDIO3 & 0x0004)
#define APP_ENABLE_SYNC_MUTE_MIC_TO_ALL_LINK_FEAT					(APP_FEATURE_CTL_AUDIO3 & 0x0008)
#define APP_AUTO_UNMUTE_ENTER_OR_EXIT_CALL_FEAT						(APP_FEATURE_CTL_AUDIO3 & 0x0040)	
#define APP_MUTE_A2DP_WHEN_VOLUME_IS_ZERO_FEAT 						(APP_FEATURE_CTL_AUDIO3 & 0x0200)
#define APP_MUTE_LINE_IN_WHEN_VOLUME_IS_ZERO_FEAT 					(APP_FEATURE_CTL_AUDIO3 & 0x0400)	


#define APP_MEDIA_LED_WITH_FILTER_EFFECT_FEAT					(APP_NvramType_FeturCtl_GetAppFeatureControl(led) & 0x02)
#define APP_LED_PWM_COMPENSATE_IN_DIFFERENT_BATTERY_LEVEL 		(APP_NvramType_FeturCtl_GetAppFeatureControl(led) & 0x04)	
#define APP_SHOW_CONDISCABLE_LED_ONLY_ONE_CONNECTED_DEVICE_FEAT	(APP_NvramType_FeturCtl_GetAppFeatureControl(led) & 0x10)

#define APP_FORCE_USE_OF_HSP_CONNECTION_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(HSP) & 0x01)
#define APP_NOT_CHG_STATE_ACP_HSP_RING_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(HSP) & 0x02)


#define APP_ALWAYS_ROLE_SWITCH_TO_MASTER_FEAT					(gAppNvramType.roleSwitchCnt /*APP_CONNECTION) & 0x0001*/)
#define APP_ROLE_SWITCH_TO_MASTER_DURING_ACCEPT_CONN_FEAT		(APP_CONNECTION & 0x0002)
#define APP_SUPPORT_HFP_FEAT									(APP_CONNECTION & 0x0004)
#define APP_SUPPORT_HSP_FEAT									(APP_CONNECTION & 0x0008)
#define APP_SUPPORT_A2DP_SINK_FEAT								(APP_CONNECTION & 0x0010)
#define APP_SUPPORT_AVRCP_FEAT									(APP_CONNECTION & 0x0020)
#define APP_SUPPORT_HFP_VERSION16_FEAT							(APP_CONNECTION & 0x0040)
#define APP_SUPPORT_MULTI_HFP_FEAT								(APP_CONNECTION & 0x0080)
#define APP_SUPPORT_ALL_ROLE_ENTER_DISCOVER_FEAT				(APP_CONNECTION & 0x0100)
#define APP_SUPPORT_TWS_SHARE_FEAT								(APP_CONNECTION & 0x0200)
#define APP_SUPPORT_RECONN_CLOSE_SCAN_FEAT						(APP_CONNECTION & 0x0400)
#ifdef HID_Profile
#define APP_SUPPORT_HID_FEAT									(APP_CONNECTION & 0x1000)
#endif
//#define APP_ALWAYS_ROLE_SWITCH_TO_SLAVE_FEAT            		(APP_CONNECTION & 0x2000)
#define APP_SUPPORT_SCO_CLOSE_SCAN_FEAT							(APP_CONNECTION & 0x4000)
#define APP_SUPPORT_AVRCP_KEY_SYNC								(APP_CONNECTION & 0x8000)

#define APP_SUPPORT_A2DP_FEAT									(APP_SUPPORT_A2DP_SINK_FEAT) 	

#define APP_LINE_IN_CYCLIC_VOLUME_ENABLED_FEAT		(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0001)
#define APP_A2DP_MEMPUT_WHEN_LINE_IN_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0002)
#define APP_IGNORE_RING_VP_WHEN_LINE_IN_FEAT		(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0008)
#define APP_NO_CONNECTION_IN_LINE_IN_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0010)
#define APP_NO_DISCAVERABLE_IN_LINE_IN_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0040)
#define APP_NO_CONNECTABLE_IN_LINE_IN_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0080)
#define APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT		(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0100)
#define APP_NO_CONNECTED_MEDIA_EVT_IN_LINE_IN_FEAT	(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0400)
#define APP_ENTER_LINEIN_SEND_AVRCP_PAUSE_FEAT		(APP_NvramType_FeturCtl_GetAppFeatureControl(lineInFeature) & 0x0800)

#define APP_FM_CYCLIC_VOLUME_ENABLED_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0001)
#define APP_A2DP_MEMPUT_WHEN_FM_ON_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0002)	   
#define APP_IGNORE_RING_VP_WHEN_FM_ON_FEAT			(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0008)
#define APP_FM_CHIP_IS_REPLACED_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0010)
#define APP_NO_DISCAVERABLE_IN_FM_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0040)
#define APP_NO_CONNECTABLE_IN_FM_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0080)
#define APP_NO_CONNECTION_IN_FM_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0100)
#define APP_UNMUTE_WHEN_SEEK_TUNE_CHANNEL			(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0200)
#define APP_NO_CONNECTED_MEDIA_EVT_IN_FM_FEAT		(APP_NvramType_FeturCtl_GetAppFeatureControl(FMFeature) & 0x0800)

#define APP_CC_UART_ENABLE											(APP_NvramType_FeturCtl_GetAppFeatureControl(customerUART) & 0x01)
#define APP_CONFIRMATION_ACCEPT_REJECT_VIA_KEY						(APP_NvramType_FeturCtl_GetAppFeatureControl(customerUART) & 0x02)
#define APP_SPP_DATA_TO_UART_FEAT									(APP_NvramType_FeturCtl_GetAppFeatureControl(customerUART) & 0x04)

#define APP_VOICE_DIAL_START_AND_END_THE_SAME_KEY_FEAT				(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x01)	
#define APP_FAST_DETECT_FOR_ROTARY_ENCODER_FEAT						(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x02)	
#define APP_KEY_MAP_MAPPING_BY_KEY_DOWN_STATE_FEAT					(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x04)
#define APP_CANCEL_VR_WHEN_RDIAL_FEAT								(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x20)
#define APP_UKEY_SWITCH_ONOFF_TYPE_FEAT 							(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x40)
#define APP_POWER_KEY_TO_DEEP_SLEEP_FEAT							(APP_NvramType_FeturCtl_GetAppFeatureControl(keys) & 0x80)

#define APP_AIR_SYNC_POWER_KEY_FEAT									(APP_NvramType_FeturCtl_GetAppFeatureControl(airFeature) & 0x01)
#define APP_AIR_SCO_RELAY_WITHOUT_CALL								(APP_NvramType_FeturCtl_GetAppFeatureControl(airFeature) & 0x02)



/**************************************************************************************************
* Define
**************************************************************************************************/
#define INVALID_BEEP_PATTERN	0xFF

/**************************************************************************************************
* Define (Product Type)
**************************************************************************************************/
#define PRODUCT_TYPE_HEADSET	0x01
#define PRODUCT_TYPE_SPEAKER	0x02

#define PRODUCT_ID_TWS			0x0123
#define PRODUCT_VER_TWS			0x0100

#define PRODUCT_AIR_NONE		0x00
#define PRODUCT_AIR_TWS			0x01
#define PRODUCT_AIR_SOUNDBAR	0x02
#define PRODUCT_AIR_SCO_WOOFER	0x04
#define PRODUCT_AIR_A2DP_WOOFER	0x08

#define GET_PRODUCT_AIR_TYPE(A)	(U8)((A)& 0x000F)

/**************************************************************************************************
* Enum
**************************************************************************************************/


/**************************************************************************************
* Structure
***************************************************************************************/
typedef struct
{
	U8 afterPowerOn;
	U8 afterLinkLoss;
	U8 userInitiated;
	U8 watchDogReTryCnt;
	U8 linkLossReTryCnt;
	U8 autoRecTryCnt;
	U8 profileRetryCnt;
}RECONNECT_INFO_STRU;//ReConnectInfo

typedef struct
{
/*
	PinCodeCtl pinCodeCtl;
	U8 inquiryRSSITH;
*/
	U8 pTwsKChannel;//twsKChannel
/*
	U8 twsLinkLossRetryCnt;
	U32 twsKIdleChangeBdaddrTime; //unit:100ms
	U32 twsKWaitChangeBdaddrTime; //unit:100ms
	U8 linkLossLoopCnt;
	U8 Reserved3[24];
	PageScanWindowSize pageScanWindowSize;
	ScoPktPara scoPktPara;
	ChangePktTypePara changePktTypePara;
	SimplePairingPara simplePairingPara;
	MMI1IntInterval mmi1IntInterval;
	//MMIBtnSeqInfo mmiBtnSeqInfo[MAX_BTN_SEQ_NO];
	SniffPara sniffPara;
	U8 retrySniffCnt;
	U8 rssiDefaultBoundry;
	U8 lineInAutoDetectKeyIndex;
	ProductType productType;
	AirParaType airPara;
*/
	RECONNECT_INFO_STRU *pReConnectInfo;//reConnectInfo
	APP_FEATURE_CTL_STRU *pAppFeatureCtl;//mmiFeatureCtl
/*
	VoicePromptConfig vpConfig;
	U8 key1GpioIndex;
	LedFilterMap ledFilterMap[MAX_LED_FILTER_NO]; 
	MMIFCCInfo mmiFCCInfo;	
*/
	U8 storedLinkKeyCnt;
/*
	MmiAvrcpSettings mmiAvrcpSettings;
	U8 mmiHfpCINDMask;
	U8 fmSeekTH;
	TWS_PARA tws_para;
	// *************************** IMPORTANT************************************
	// flash code version: 001.62971.3204.0
	// NOTICE - structure definition above matches specified flash code version.
	//          DO NOT modify it. ONLY APPENDing fields is allowed.
	// *************************** IMPORTANT************************************
	PairCtl	pariCtl;
	U8 roleSwitchCnt;
	U8 mcuADCSource;
	U32 GENERIC_PTR sbcFlashAddress;
	U8 roleChangeBatteryDiff; //in percent
	U8 roleChangeBatMonitorTime; //unit: half minute
	U8 roleChangeBatteryThreshold; //in percent
	U8 firstConnectProfile;
	U8 reserved10[64];
	U8 incomingCallVoicePriority[4];
*/
}APP_NVRAM_TYPE_STRU;//MMI_NVRAM_TYPE

/**************************************************************************************************
* Variable
**************************************************************************************************/
extern APP_NVRAM_TYPE_STRU gAppNvramType;;//gMMI_nvram

/**************************************************************************************************
* Public function
**************************************************************************************************/
void APP_NvramType_Init(void);
U16 APP_NvramType_FeturCtl_GetAppFeatureControl(U8 feature);

#endif
