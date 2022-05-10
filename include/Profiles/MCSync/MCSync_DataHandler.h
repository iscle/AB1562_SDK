/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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

#ifndef _MCSYNC_DATA_HADNDLER_H_
#define _MCSYNC_DATA_HADNDLER_H_

#include "bt_aws_mce.h"
#include "profile_a2dp_parameter_nvstruc.h"
#include "MCSync_LinkManager.h"
#include "MCSync_InternalHandler.h"

/*************************************************************************************************
* Define
**************************************************************************************************/
#define MCSYNC_TLV_HEADER_SIZE 	sizeof(BT_AWS_MCE_IF_PACKET_HEADER)
#define MCSYNC_PEQ_HEADER 		(sizeof(U8)+sizeof(U16)+sizeof(U16)+sizeof(U32)) // opcode+ datalength reserved piclo

/*************************************************************************************************
* Enum
**************************************************************************************************/
/*
	MCSYNC_APP_SYNC_AIROKEY_NOISE,//newton
	MCSYNC_APP_SYNC_DEVICE_NAME,//sys
	MCSYNC_APP_SYNC_AIROHA_KEY,//newton
	MCSYNC_APP_SYNC_AIROKEY_SETTING,//newton
	MCSYNC_APP_SYNC_VPRT,//audio
	MCSYNC_APP_SYNC_STOP_VPRT,//audio
	MCSYNC_APP_SYNC_VPRT_PLAY_LATER,//audio
	MCSYNC_APP_SYNC_CALLER_ID,//bt
	MCSYNC_APP_SYNC_PEQ_REALTIME,//audio
	MCSYNC_APP_SYNC_DSP,//audio
	MCSYNC_APP_SYNC_POWEROFF,//sys
	MCSYNC_APP_SYNC_GENERAL_DATA,//ae
	MCSYNC_APP_SYNC_IN_EAR_DETECTION_STATUS,//peripheral
	MCSYNC_APP_SYNC_LEAKAGE_DETECT_INFO,//audio
	MCSYNC_APP_XIAOAI,//xiaoai
	MCSYNC_APP_SYNC_SHARE_MODE_INFO,//share
	MCSYNC_APP_SYNC_OTHER_AGENT_BDA,//share
	MCSYNC_APP_SYNC_FOLLOWER_CONN,//share
	MCSYNC_APP_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA,//share
	MCSYNC_APP_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE, //35//share
	MCSYNC_APP_SYNC_AGENT_RETRIGGER_FOLLOWER_A2DP_TRANSFORM,//share
	MCSYNC_APP_SYNC_BATTERY,//sys
	MCSYNC_OPCODE_MODULE_SEND_DATA,
	//when add app's OpCode, please also add to log.c(MCSync_IF_AppOpcode_String[][MAX_LOG_STRING])
*/

enum
{
	MCSYNC_APP_SYNC_AIROKEY_NOISE,
	MCSYNC_APP_SYNC_DEVICE_NAME, //5
	MCSYNC_APP_SYNC_AIROHA_KEY,
	MCSYNC_APP_SYNC_AIROKEY_SETTING,
	MCSYNC_APP_SYNC_VPRT, //10
	MCSYNC_APP_SYNC_STOP_VPRT,
	MCSYNC_APP_SYNC_VPRT_PLAY_LATER,
	MCSYNC_APP_SYNC_CALLER_ID,
	MCSYNC_APP_SYNC_PEQ_REALTIME,
	MCSYNC_APP_SYNC_DSP,
	MCSYNC_APP_SYNC_POWEROFF, //20
	MCSYNC_APP_SYNC_GENERAL_DATA,
	MCSYNC_APP_SYNC_IN_EAR_DETECTION_STATUS,
	MCSYNC_APP_SYNC_LEAKAGE_DETECT_INFO, //35
	MCSYNC_APP_XIAOAI,
	MCSYNC_APP_SYNC_SHARE_MODE_INFO,
	MCSYNC_APP_SYNC_OTHER_AGENT_BDA, //38
	MCSYNC_APP_SYNC_FOLLOWER_CONN,
	MCSYNC_APP_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA,
	MCSYNC_APP_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE, //41
	MCSYNC_APP_SYNC_AGENT_RETRIGGER_FOLLOWER_A2DP_TRANSFORM,
	MCSYNC_APP_SYNC_BATTERY,
    MCSYNC_OPCODE_MODULE_SEND_DATA,
	//when add app's OpCode, please also add to log.c(MCSync_IF_AppOpcode_String[][MAX_LOG_STRING])

	/* number of APP's OpCode */
	MCSYNC_APP_OPCODE_NUM,
};

enum
{
	MCSYNC_LED_AGENT_SET_BG,
	MCSYNC_LED_AGENT_SET_FG,
	MCSYNC_LED_AGENT_SET_RESUME_BG,
	MCSYNC_LED_PARTNER_ASK_RESUME_BG,
	MCSYNC_LED_SET_ACTION_NUM,
};

enum
{
    MCSYNC_PARTNER_REQUEST_EXIT_MODE                        = 0x02,
	MCSYNC_FOLLOWER_SYNC_RETRIGGER_FOLLOWER_A2DP_TRANSFORM  = 0x04,
	MCSYNC_FOLLOWER_SYNC_SOUNDLEVEL 						= 0x08,
	MCSYNC_FOLLOWER_SYNC_DISC								= 0x09,
	MCSYNC_FOLLOWER_SYNC_KEYCODE							= 0x0B,
	MCSYNC_FOLLOWER_SYNC_SMART_CHARGING_CASE_STATE			= 0x0C,
	MCSYNC_FOLLOWER_SYNC_POWER_OFF							= 0x0D,
};

typedef enum MCSYNC_SEND_IFPKT_MSG_E
{
	MCSYNC_SEND_IFPKT_SUCCESS,
	MCSYNC_SEND_IFPKT_FAIL,
	MCSYNC_SEND_IFPKT_FAIL_NEED_RETRY,
}MCSYNC_SEND_IFPKT_MSG_E;

enum
{
	MCSYNC_AWS_IDLE_TIMER_CANCEL,
	MCSYNC_AWS_IDLE_TIMER_SET,
};

enum
{
	MCSYNC_A2DP_STATE_NONE,
	MCSYNC_A2DP_STATE_READY,
	MCSYNC_A2DP_STATE_PLAYING,
	MCSYNC_A2DP_STATE_PLAYDROP,
};

/*************************************************************************************************
* Structure
**************************************************************************************************/

typedef struct
{
	BD_ADDR_T bdAddr;
	U16 cid;
	U8 codec;
	U8 scms;
}MCSYNC_PARTNER_AUDIO_PATH_INFO;

typedef struct
{
	MCSYNC_PARTNER_AUDIO_PATH_INFO audioPathInfo;
}MCSYNC_PARTNER_A2DP_CTL_INFO;

typedef struct
{
	U8 *pData;
	U16 dataLength;
	BOOL isA2dpIFSend;
}MCSYNC_A2DP_CTL_INFO;

typedef struct
{
	U8 *pData;
	U16 dataLength;
	BOOL isAmaIFSend;
}MCSYNC_AMA_DATA_CTL_INFO;


typedef union MCSYNC_A2DP_CODEC_SETTING
{
	SBCCodecSettings	sbc;
	AACCodecSettings	aac;
} PACKED MCSYNC_A2DP_CODEC_SETTING;

typedef struct MCSYNC_A2DP_AIROHA_CONFIG
{
	U8	enableSCMS;
	U8	peqIndex;
	U8	isMediaPlay;
	BOOL isGameMode;
	U8 numOfAg;
}PACKED MCSYNC_A2DP_AIROHA_CONFIG;

typedef struct MCSYNC_A2DP_CODEC_INFO_T
{
	U8	codecType;		// 0:SBC, 2:AAC, 0xFF:vendor-Specific
	U8	sepType;		// a2dp role, 0:source, 1:sink
	U8	codecInfoLen;	// The length of the following codec
	MCSYNC_A2DP_CODEC_SETTING	codecSetting;
} PACKED MCSYNC_A2DP_CODEC_INFO_T;

typedef struct MCSYNC_A2DP_INFO_T
{
	U16							cid;		//media channel
	U16							l2capMtu;			//mtu
	U8							state;				//0:none, 1:ready 2:playing 3:play drop
	U8							volume;				//volume
	U8							specificRole;
	U8							reserved;			//reserved 1 byte
	U16							drift;				//a2dp drift value
	U16							raio:4;				//ratio
	U16							btClkIntra:12;		//the Bluetooth clock in microseconds.
	U32							btClk;				//the Bluetooth clock in 312.5us units.
	U32							asi;				//packet ts
	MCSYNC_A2DP_AIROHA_CONFIG	airohaA2dpConfig;	//3byte
	MCSYNC_A2DP_CODEC_INFO_T	a2dpCodecConfig;	//a2dp codec info 16
} PACKED MCSYNC_A2DP_INFO_T;

typedef struct MCSYNC_SCO_INFO_T
{
	U8							speakerSoundLevel;	//0-15 in HFP spec
	U8							micSoundLevel;
	U8							scoState;
	U8							topState;
} PACKED MCSYNC_SCO_INFO_T;

typedef struct MCSYNC_APPREPORT_AIROKEY_SYNC_NOISE_T
{
	U8 action;
}PACKED MCSYNC_APPREPORT_AIROKEY_SYNC_NOISE_T;

typedef struct MCSYNC_APPREPORT_AIROKEY_SYNC_DEVICE_NAME_T
{
	U16 length;
	U8 name[1];
}PACKED MCSYNC_APPREPORT_AIROKEY_SYNC_DEVICE_NAME_T;

typedef struct MCSYNC_APPREPORT_AIROKEY_SYNC_KEY_T
{
	U8 rightAction;
	U8 leftAction;
	U8 noiseCtlMask;
}PACKED MCSYNC_APPREPORT_AIROKEY_SYNC_KEY_T;

typedef struct MCSYNC_APPREPORT_AIROKEY_SYNC_SETTING_T
{
	U8 isEarDetectEnable;
}PACKED MCSYNC_APPREPORT_AIROKEY_SYNC_SETTING_T;

typedef struct MCSYNC_APPREPORT_SYNC_VPRT_T
{
	U8 overwriteTime;
	U8 reserved;
	U16 rtIndex;
	U16 vpIndex;
	U16 eventIndex;
	U32 currentPicoClock;
}PACKED MCSYNC_APPREPORT_SYNC_VPRT_T;

typedef struct MCSYNC_APPREPORT_SYNC_STOP_VPRT_T
{
	U16 eventIndex;
	U32 currentPicoClock;
}PACKED MCSYNC_APPREPORT_SYNC_STOP_VPRT_T;

typedef struct MCSYNC_APPREPORT_SYNC_VPRT_PLAY_LATER_T
{
	U16 eventIndex;
	U32 clockToStart;
}PACKED MCSYNC_APPREPORT_SYNC_VPRT_PLAY_LATER_T;

typedef struct MCSYNC_APPREPORT_SYNC_CALLER_ID_T
{
	U8 callerIdLength;
	U8 callerIdData[1];
}PACKED MCSYNC_APPREPORT_SYNC_CALLER_ID_T;

typedef struct MCSYNC_RHO_ROLE_CHANGE_IND_T
{
	BD_ADDR_T bdAddr;
	HCI_STATUS_E status;
	AWS_MCE_ROLE_E role;
}PACKED MCSYNC_RHO_ROLE_CHANGE_IND_T;

typedef struct MCSYNC_CANCEL_CONN_CFM_T
{
	HCI_STATUS_E status;
}PACKED MCSYNC_CANCEL_CONN_CFM_T;

typedef struct MCSYNC_SYNC_PEQ_REALTIME_T
{
	U16 dataLength;
	U16 reserved;
	U32 clockToStart;
	U8 data[1];
}PACKED MCSYNC_SYNC_PEQ_REALTIME_T;

typedef struct MCSYNC_APPREPORT_SYNC_DSP_T
{
	U32 clockToStart;
	U32 length;
	U8 data[1];
}PACKED MCSYNC_APPREPORT_SYNC_DSP_T;

typedef struct MCSYNC_APPREPORT_SYNC_POWEROFF_T
{
	U8 reason;
    U8 isFollowerIFPKT:1;
	U8 isOnlyForFollower:1;
    U8 reserved:6;
}MCSYNC_APPREPORT_SYNC_POWEROFF_T;


typedef struct MCSYNC_APPREPORT_SYNC_GENERAL_DATA_T
{
	U32 length;
	U8 data[1];
}PACKED MCSYNC_APPREPORT_SYNC_GENERAL_DATA_T;

#ifdef MTK_LEAKAGE_DETECTION_ENABLE
typedef struct MCSYNC_APPREPORT_LEAKAGE_DETECT_INFO_T
{
	BOOL isStart;
	BOOL result;
}PACKED MCSYNC_APPREPORT_LEAKAGE_DETECT_INFO_T;
#endif

typedef struct MCSYNC_APP_XIAOAI_T
{
	U8	XiaoaiOpCode;
	U8 data[1];
}PACKED MCSYNC_APP_XIAOAI_T;

typedef struct MCSYNC_APPREPORT_SYNC_SHARE_MODE_INFO_T
{
	U8 state;
	U8 shareMode;
}MCSYNC_APPREPORT_SYNC_SHARE_MODE_INFO_T;

typedef struct MCSYNC_APPREPORT_SYNC_OTHER_AGENT_BDA_INFO_T
{
	BD_ADDR_T bdAddr;
}PACKED MCSYNC_APPREPORT_SYNC_OTHER_AGENT_BDA_INFO_T;

typedef struct MCSYNC_APPREPORT_SYNC_FOLLOWER_CONN_T
{
	U8 		  doConn;
}PACKED MCSYNC_APPREPORT_SYNC_FOLLOWER_CONN_T;

typedef struct MCSYNC_APPREPORT_SYNC_AGENT_RETRASMIT_FOLLOWER_DATA_T
{
	U8 type;
	U8 value;
	U8 isFollowerIFPkt;
}PACKED MCSYNC_APPREPORT_SYNC_AGENT_RETRASMIT_FOLLOWER_DATA_T;

typedef struct MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_T
{
	U8 isLeaveShareMode;
	U8 isFollowerIFPkt;
}PACKED MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_T;

typedef struct MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_RETRIGGER_A2DP_T
{
	U8 codec;
	U8 avrcpVolume;
	U8 peqIndex;
	U8 scms;
	U8 isGameMode;
	U8 numOfAg;
	U8 action;
	MCSYNC_A2DP_CODEC_INFO_T codecInfo;
}PACKED MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_RETRIGGER_A2DP_T;

typedef struct MCSYNC_APPREPROT_SYNC_BATTERY_T
{
	U8 batteryLevel;
}PACKED MCSYNC_APPREPROT_SYNC_BATTERY_T;

typedef struct MCSYNC_APPREPORT_MODULE_DATA_T
{
	U16	module_id;
    U16 length;
	U8  data[1];
}PACKED MCSYNC_APPREPORT_MODULE_DATA_T;

typedef union MCSYNC_APPREPORT_BODY_T
{
	MCSYNC_APPREPORT_AIROKEY_SYNC_NOISE_T	syncAiroKeyNoise;
	MCSYNC_APPREPORT_AIROKEY_SYNC_DEVICE_NAME_T		syncDeviceName;
	MCSYNC_APPREPORT_AIROKEY_SYNC_KEY_T		syncAirohaKey;
	MCSYNC_APPREPORT_AIROKEY_SYNC_SETTING_T	syncAiroKeySetting;
	MCSYNC_APPREPORT_SYNC_VPRT_T			syncVpRt;
	MCSYNC_APPREPORT_SYNC_STOP_VPRT_T		syncStopVpRt;
	MCSYNC_APPREPORT_SYNC_VPRT_PLAY_LATER_T	syncVpRtPlayLater;
	MCSYNC_APPREPORT_SYNC_CALLER_ID_T		syncCallerId;
	MCSYNC_APPREPORT_SYNC_DSP_T				syncDSP;
	MCSYNC_APPREPORT_SYNC_POWEROFF_T		syncPowerOff;
	MCSYNC_APPREPORT_SYNC_GENERAL_DATA_T    syncGeneralData;
	#ifdef MTK_LEAKAGE_DETECTION_ENABLE
	MCSYNC_APPREPORT_LEAKAGE_DETECT_INFO_T	syncLeakageDetectInfo;
	#endif
	#ifdef XIAOAI_DEVELOPMENT
	MCSYNC_APP_XIAOAI_T syncXiaoai;
	#endif
    MCSYNC_APPREPROT_SYNC_BATTERY_T					syncBattery;
    MCSYNC_APPREPORT_MODULE_DATA_T      module_data;
    MCSYNC_APPREPORT_SYNC_SHARE_MODE_INFO_T			syncshareMode;
	MCSYNC_APPREPORT_SYNC_OTHER_AGENT_BDA_INFO_T	syncOtherAgentBDA;
	MCSYNC_APPREPORT_SYNC_FOLLOWER_CONN_T			syncFollowerConn;
	MCSYNC_APPREPORT_SYNC_AGENT_RETRASMIT_FOLLOWER_DATA_T syncAgentRetransmitFollowerData;
	MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_T syncAgentNoticeFollowerLeave;
	MCSYNC_APPREPORT_SYNC_AGENT_NOTICE_FOLLOWER_RETRIGGER_A2DP_T syncAgentNoticeFollowerRetriggerA2dp;
} PACKED MCSYNC_APPREPORT_BODY_T;

typedef struct MCSYNC_APP_REPORT_T
{
	U8 opCode;
	MCSYNC_APPREPORT_BODY_T appReportPtr;
} PACKED MCSYNC_APP_REPORT_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_SOUNDLEVEL_INFO_T
{
	U8 soundLevel;
	U8 isFollowerIFPkt;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_SOUNDLEVEL_INFO_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_DISC_INFO_T
{
	U8 isOnlyForFollower;
	U8 isFollowerIFPkt;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_DISC_INFO_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_RETRIGGER_A2DP_INFO_T
{
	U8 specifilRole;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_RETRIGGER_A2DP_INFO_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_KEYCODE_INFO_T
{
	U16 keyCode;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_KEYCODE_INFO_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_SMART_CHARGING_CASE_STATE_INFO_T
{
	U8 state;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_SMART_CHARGING_CASE_STATE_INFO_T;

typedef struct MCSYNC_FOLLOWERREPORT_SYNC_POWEROFF_INFO_T
{
	U8 reason;
	U8 isFollowerIFPKT:1;
	U8 isOnlyForFollower:1;
	U8 reserved:6;
}PACKED MCSYNC_FOLLOWERREPORT_SYNC_POWEROFF_INFO_T;


typedef union MCSYNC_FOLLOWER_BODY_T
{
	MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_SOUNDLEVEL_INFO_T 		followerSoundLevel;
	MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_DISC_INFO_T		  		followerDisc;
	MCSYNC_FOLLOWERREPORT_SYNC_FOLLOWER_RETRIGGER_A2DP_INFO_T 	followerRetriggerA2dp;
	MCSYNC_FOLLOWERREPORT_SYNC_KEYCODE_INFO_T				  	followerKeyCode;
	MCSYNC_FOLLOWERREPORT_SYNC_SMART_CHARGING_CASE_STATE_INFO_T followerSmartState;
	MCSYNC_FOLLOWERREPORT_SYNC_POWEROFF_INFO_T					followerPowerOff;
}PACKED MCSYNC_FOLLOWER_BODY_T;

typedef struct MCSYNC_APP_PEQ_T
{
	U8 opCode;
	MCSYNC_SYNC_PEQ_REALTIME_T appPEQPtr;
} PACKED MCSYNC_APP_PEQ_T;

typedef struct MCSYNC_RHO_PACKET_T
{
	U16 rpCount;
	U8 pHostData[1];
} PACKED MCSYNC_RHO_PACKET_T;

typedef struct MCSYNC_FOLLOWER_PACKET_T
{
	U8 followerIFType;
	MCSYNC_FOLLOWER_BODY_T followerData;
}PACKED MCSYNC_FOLLOWER_PACKET_T;

typedef union MCSYNC_MSG_BODY_T
{
	MCSYNC_A2DP_INFO_T	a2dpInfo;
	MCSYNC_SCO_INFO_T	scoInfo;
	MCSYNC_APP_REPORT_T	appReport;
	MCSYNC_APP_PEQ_T	appPeq;
	MCSYNC_RHO_PACKET_T rhoPacket;
	MCSYNC_FOLLOWER_PACKET_T followerReport;
} PACKED MCSYNC_MSG_BODY_T;

typedef struct MCSYNC_TLV_T
{
	BT_AWS_MCE_IF_PACKET_HEADER	header;
	MCSYNC_MSG_BODY_T			msgBodyPtr;
} PACKED MCSYNC_TLV_T;

typedef struct
{
	BD_ADDR_T bdAddr;
	U8 speakerSoundLevel;
	U8 micSoundLevel;
	U8 state;
}MCSYNC_SCO_DATA_STRU;

/*************************************************************************************************
* New mcsync data structure
**************************************************************************************************/
typedef struct
{
	U8 role;
	U8 batteryLevelInPercent;
	U8 channelID;
	U16 raceID;
}MCSYNC_GET_BATTERY_STRU;

<<<<<<< HEAD
=======
typedef struct
{
	U8 takeOverState;
}MCSYNC_TAKE_OVER_STATE_STRU;


>>>>>>> db20e11 (second commit)
/*************************************************************************************************
* Public function (IF data)
**************************************************************************************************/

typedef struct
{
    U16 module_id;
    void (*module_callback)(U16 length, U8 *p_data);
}MODULE_TABLE_T;

typedef struct
{
	MODULE_TABLE_T *pDataHead;
	U8 num_of_group;
}MODULE_IF_RECORD_STRU_T;


/*************************************************************************************************
* Public function
**************************************************************************************************/
U8		MCSYNC_SendRhoIfPkt(U8 *pData, U16 dataLen);
void	MCSYNC_IFDataHandler(MCSYNC_LINK_STRU *pLinkInfo, SOURCE source);
U8		MCSYNC_SendSyncA2dpInfo(AWSMCE_INTERNAL_A2DP_CMD_T *msg);
U8		MCSYNC_SendSyncScoInfo(U8 speakerSoundLevel, U8 micSoundLevel, U8 scoState, U8 topState);
U8		MCSYNC_SendSyncSmartChargeCaseStateInfo(U8 state);
U8		MCSYNC_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState);
U8 		MCSYNC_SendSyncVpRtInfo(U16 rtIndex, U16 vpIndex, U16 eventIndex, U32 currentPicoClock, U8 overwriteTime);
U8 		MCSYNC_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock);
U8		MCSYNC_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart);
U8		MCSYNC_SendSyncCallerIdInfo(U8 *pCallerId, U8 length);
U8 		MCSYNC_SendSyncPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart);
U8		MCSYNC_SendSyncDSPInfo(U32 clockToStart, U32 length, U8 *pData);
U8 		MCSYNC_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower);

void 	MCSYNC_IFPktPartnerCtlInfoInit(void);
void 	MCSYNC_ClearAudioPathInfo(void);
U8      MCSYNC_SendSyncGeneralData(U8 *pData, U32 length);
void 	MCSYNC_SendA2dpDataAgain(void);
U8      mcsync_SendIFPkt(U8 type, U16 length, U8* pValue);
U8		MCSYNC_SendLeakageDetectInfo(BOOL isStart, BOOL result);
U8		MCSYNC_SendSyncShareModeInfo(U8 state, U8 shareMode);
U8 		MCSYNC_SendFollowerSoundlevel(U8 soundLevel, U8 isFollowerIFPkt);
U8 		MCSYNC_SendOtherAgentBDA(BD_ADDR_T *pBdAddr);
U8 		MCSYNC_SendFollowerConn(U8 doConn);
U8 		MCSYNC_SendFollowerDisc(U8 isOnlyForFollower, U8 isFollowerIFPkt);
U8 		MCSYNC_SendFollowerA2dpRetrigger();
U8      MCSYNC_SendPartnerRequestExitMode(void);
U8 		MCSYNC_SendAgentRetransmitFollowerData(U8 type, U8 value, U8 isFollowerIFPkt);
U8 		MCSYNC_SendAgentNoticeFollowerLeave(U8 isLeaveShareMode, U8 isFollowerIFPkt);
U8		MCSYNC_SendBatteryLevel(U8 batteryLevel);

U8 MCSYNC_SendLeakageDetectInfo(BOOL isStart, BOOL result);

void MCSYNC_SendBatteryMsgInfo(U16 raceID, U8 channelID);
<<<<<<< HEAD
=======
void MCSYNC_SendTakeOverState(void);
>>>>>>> db20e11 (second commit)


/*************************************************************************************************
* Public function (IF Data)
**************************************************************************************************/

BOOL mcsync_data_register_module(MODULE_TABLE_T *p, U8 num, U8 group_id);
U8   mcsync_data_send_data_by_module_id(U16 module_id, U16 length, U8 *p_data);
void mcsync_data_init(void);

#endif

