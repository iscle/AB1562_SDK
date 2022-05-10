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

#ifndef _BTAWSMCE_H_
#define _BTAWSMCE_H_

#include "MCSync_Interface.h"
#include "MCSync_DataHandler.h"

#define GENERATE_GROUP_ID(x)	(x << 12)


/**********************************************************
opcode = 4bit group id + 12 bit module id
***********************************************************/

enum //group_id
{
	MCSYNC_MODULE_GROUP_BT = 0,
	MCSYNC_MODULE_GROUP_GVA,
	MCSYNC_MODULE_GROUP_XAOAI,
	MCSYNC_MODULE_GROUP_AMA,
	MCSYNC_MODULE_GROUP_AUDIO,
	MCSYNC_MODULE_GROUP_SYS,
	MCSYNC_MODULE_GROUP_PPH,
	MCSYNC_MODULE_GROUP_NEWTON,
	MCSYNC_MODULE_GROUP_RESERVED1,
	MCSYNC_MODULE_GROUP_RESERVED2,
<<<<<<< HEAD
	MCSYNC_MODULE_GROUP_RESERVED3,
=======
	MCSYNC_MODULE_GROUP_RESERVED3, // used for Tile
>>>>>>> db20e11 (second commit)
	MCSYNC_MODULE_GROUP_RESERVED4,
	MCSYNC_MODULE_GROUP_RESERVED5,
	MCSYNC_MODULE_GROUP_RESERVED6,
	MCSYNC_MODULE_GROUP_RESERVED7,
	MCSYNC_MODULE_GROUP_RESERVED8,
	MCSYNC_MODULE_NUM_OF_GROUP, //16,   Not add group over 16
	MCSYNC_MODUKE_INVALID_GROUP = 0xFF,
};

typedef enum //module_id
{
    /*BT GROUP*/
    MCSYNC_MODULE_BT_GROUP_START            = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_BT),
    MCSYNC_MODULE_BT_STATE                  = MCSYNC_MODULE_BT_GROUP_START,
    MCSYNC_MODULE_BT_AGNUM,
    MCSYNC_MODULE_BT_CALLER_ID,
    MCSYNC_MODULE_BT_RHO_PARA,
    MCSYNC_MODULE_BT_RHO_STATE,
    MCSYNC_MODULE_BT_LINK_LIST,
    MCSYNC_MODULE_BT_BLE_LINK_LIST,
    MCSYNC_MODULE_BT_RECONNECT_STATE,
    MCSYNC_MODULE_BT_RECONNECT_MASK,
    MCSYNC_MODULE_BT_RETRANSMIT_FOLLOWER_DISC,
    MCSYNC_MODULE_BT_L2CAP_DATA,
<<<<<<< HEAD
    MCSYNC_MODULE_BT_GROUP_END              = MCSYNC_MODULE_BT_L2CAP_DATA,
=======
    MCSYNC_MODULE_BT_TAKE_OVER_STATE,
	MCSYNC_MODULE_BT_NOTICE_MASTER_PARTNER_TO_SHARE_MODE,
    MCSYNC_MODULE_BT_GROUP_END              = MCSYNC_MODULE_BT_NOTICE_MASTER_PARTNER_TO_SHARE_MODE,
>>>>>>> db20e11 (second commit)

    /*GVA GROUP*/
    MCSYNC_MODULE_GVA_GROUP_START           = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_GVA),
    MCSYNC_MODULE_GVA_GFP                   = MCSYNC_MODULE_GVA_GROUP_START,
    MCSYNC_MODULE_GVA_GROUP_END             = MCSYNC_MODULE_GVA_GFP,

    /*XAOAI GROUP*/
    MCSYNC_MODULE_XAOAI_GROUP_START         = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_XAOAI),
    MCSYNC_MODULE_XAOAI_XIAOAI              = MCSYNC_MODULE_XAOAI_GROUP_START,
    MCSYNC_MODULE_XAOAI_GROUP_END           = MCSYNC_MODULE_XAOAI_XIAOAI,

    /*AMA GROUP*/
    MCSYNC_MODULE_AMA_GROUP_START           = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_AMA),
    MCSYNC_MODULE_AMA_DATA                  = MCSYNC_MODULE_AMA_GROUP_START,
    MCSYNC_MODULE_AMA_GROUP_END             = MCSYNC_MODULE_AMA_DATA,

    /*AUDIO GROUP*/
    MCSYNC_MODULE_AUDIO_GROUP_START         = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_AUDIO),
    MCSYNC_MODULE_AUDIO_VP_IS_ENABLE        = MCSYNC_MODULE_AUDIO_GROUP_START,
    MCSYNC_MODULE_AUDIO_ANC_PASSTHRU,
    MCSYNC_MODULE_AUDIO_VPRT,
    MCSYNC_MODULE_AUDIO_STOP_VPRT,
    MCSYNC_MODULE_AUDIO_VPRT_PLAY_LATER,
    MCSYNC_MODULE_AUDIO_PEQ_REALTIME,
    MCSYNC_MODULE_AUDIO_VP_LANG_INDEX,
    MCSYNC_MODULE_AUDIO_DSP,
    MCSYNC_MODULE_AUDIO_SPEAKER_MUTE,
    MCSYNC_MODULE_AUDIO_LEAKAGE_DETECT_INFO,
    MCSYNC_MODULE_AUDIO_GROUP_END           = MCSYNC_MODULE_AUDIO_LEAKAGE_DETECT_INFO,

    /*SYS GROUP*/
    MCSYNC_MODULE_SYS_GROUP_START           = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_SYS),
    MCSYNC_MODULE_SYS_KEY_ACTION            = MCSYNC_MODULE_SYS_GROUP_START,
    MCSYNC_MODULE_SYS_DEVICE_NAME,
    MCSYNC_MODULE_SYS_LED,
    MCSYNC_MODULE_SYS_BDADDR,
    MCSYNC_MODULE_SYS_POWEROFF,
    MCSYNC_MODULE_SYS_FCD_RESET,
    MCSYNC_MODULE_SYS_BATTERY,
    MCSYNC_MODULE_SYS_CLEAR_LINK_KEY,
    MCSYNC_MODULE_SYS_GROUP_END             = MCSYNC_MODULE_SYS_CLEAR_LINK_KEY,

    /*PERIPHERAL(PPH) GROUP*/
    MCSYNC_MODULE_PPH_GROUP_START           = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_PPH),
    MCSYNC_MODULE_PPH_SMT_CHG_CSE           = MCSYNC_MODULE_PPH_GROUP_START,
    MCSYNC_MODULE_PPH_KEYCODE_ACTION,
    MCSYNC_MODULE_PPH_SMART_CHARGER_CASE_STATE,
    MCSYNC_MODULE_PPH_IN_EAR_DETECTION_STATUS,
    MCSYNC_MODULE_PPH_GROUP_END             = MCSYNC_MODULE_PPH_IN_EAR_DETECTION_STATUS,

    /*NEWTON GROUP*/
    MCSYNC_MODULE_NEWTON_GROUP_START        = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_NEWTON),
    MCSYNC_MODULE_NEWTON_AIROKEY_NOISE      = MCSYNC_MODULE_NEWTON_GROUP_START,
    MCSYNC_MODULE_NEWTON_AIROHA_KEY,
    MCSYNC_MODULE_NEWTON_AIROKEY_SETTING,
    MCSYNC_MODULE_NEWTON_GROUP_END          = MCSYNC_MODULE_NEWTON_AIROKEY_SETTING,

    /*RESERVED AE DATA*/
    MCSYNC_MODULE_RESERVED1_GROUP_START             = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_RESERVED1),
    MCSYNC_MODULE_RESERVED1_GROUP_AE_GENEREAL_DATA  = MCSYNC_MODULE_RESERVED1_GROUP_START,
    MCSYNC_MODULE_RESERVED1_GROUP_END               = MCSYNC_MODULE_RESERVED1_GROUP_AE_GENEREAL_DATA,

    /*PROFILE LAYER GROUP*/
<<<<<<< HEAD
    MCSYNC_MODULE_PROFILE_LAYER_GROUP_START         = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_RESERVED2),
    MCSYNC_MODULE_PROFILE_LAYER_GROUP_GET_BATTERY   = MCSYNC_MODULE_PROFILE_LAYER_GROUP_START,
    MCSYNC_MODULE_PROFILE_LAYER_GROUP_END           = MCSYNC_MODULE_PROFILE_LAYER_GROUP_GET_BATTERY,
=======
	MCSYNC_MODULE_PROFILE_LAYER_GROUP_START			= GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_RESERVED2),
	MCSYNC_MODULE_PROFILE_LAYER_GROUP_GET_BATTERY	= MCSYNC_MODULE_PROFILE_LAYER_GROUP_START,
	MCSYNC_MODULE_PROFILE_LAYER_TAKE_OVER_STATE,
	MCSYNC_MODULE_PROFILE_LAYER_GROUP_END			= MCSYNC_MODULE_PROFILE_LAYER_TAKE_OVER_STATE,

    /*Tile GROUP*/
    MCSYNC_MODULE_TILE_GROUP_START         = GENERATE_GROUP_ID(MCSYNC_MODULE_GROUP_RESERVED3),
    MCSYNC_MODULE_TILE_DATA                = MCSYNC_MODULE_TILE_GROUP_START,
    MCSYNC_MODULE_TILE_GROUP_END           = MCSYNC_MODULE_TILE_DATA,
>>>>>>> db20e11 (second commit)
}MCSYNC_MODULE_ID_E;


typedef enum AWSMCE_MSG_E
{
    AWSMCE_MSG_START,
    AWSMCE_START_SERVICE_CFM,
    AWSMCE_STOP_SERVICE_CFM,
    AWSMCE_CONNECT_CFM,
    AWSMCE_DISCONNECT_IND,
    AWSMCE_RECONNECT_IND, //5
    AWSMCE_RECONNECT_POWER_ON_IND,
    AWSMCE_PICO_CLOCK_CFM,
    AWSMCE_STATUS_CHANGE_IND,					//Agent   received
    AWSMCE_AGENT_A2DP_EVT_IND,					//Partner received
    AWSMCE_AGENT_SCO_EVT_IND, //10					//Partner received
    AWSMCE_SYNC_KEY_IND, 						//Partner received
    AWSMCE_SYNC_VPRT_IND,						//Partner received
    AWSMCE_SYNC_STOP_VPRT_IND,					//Partner received
    AWSMCE_SYNC_VPRT_PLAY_LATER_IND,
    AWSMCE_SYNC_CALLER_ID_IND, //20					//Partner received
    AWSMCE_SYNC_PEQREALTIME_IND,				//Partner received
    AWSMCE_RHO_PACKET_IND,						//Partner received
    AWSMCE_RHO_ROLE_CHANGE_IND,				    //both received
    AWSMCE_CANCEL_CONN_CFM,
    AWSMCE_MSG_SLAVE_VPRT_DELAY_IND,
    AWSMCE_AWS_IDLE_TIMER_CTL_IND,				//Agent	profile to app
    AWSMCE_SYNC_POWEROFF_IND,	//31				//Partner received
    AWSMCE_SYNC_KEYCODE_IND,
    AWSMCE_SYNC_SMART_CHARGE_CASE_STATE_IND,
    MCSYNC_ROLEMODECHANGE_CFM, //35
    MCSYNC_SETAIRPAIRING_CFM, //40
    AWSMCE_NEED_DISCONNECT_IND,
    AWSMCE_SYNC_GENERAL_DATA_IND,
    MCSYNC_SET_CURRENT_ROLE_CFM,			    //partner
    MCSYNC_SYNC_SHARE_MODE_INFO_IND,	//48			//Partner received
    AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND,
    AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND,
    AWSMCE_SYNC_OTHER_AGENT_BDA_IND,
    AWSMCE_SYNC_FOLLOWER_CONN_IND,
    AWSMCE_SYNC_FOLLOWER_DISC_IND,
    AWSMCE_SYNC_PARTNER_REQUEST_EXIT_MODE_IND,
    AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND,
    AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND,
    AWSMCE_SYNC_STATE_CHANGED_IND,
}AWSMCE_MSG_E;

enum
{
    MCSYNC_SHARE_SPECIFIC_ROLE_PARTNER = 0,
    MCSYNC_SHARE_SPECIFIC_ROLE_FOLLOWER1 = 8,
    MCSYNC_SHARE_SPECIFIC_ROLE_FOLLOWER2 = 4,
    MCSYNC_SHARE_SPECIFIC_ROLE_NOT_USED = 0xFF,
    MCSYNC_SHARE_SPECIFIC_ROLE_INVALID = 0xFF,
};

enum
{
    MCSYNC_FOLLOWER_STATE_DISCONNECTED,
    MCSYNC_FOLLOWER_STATE_CONNECTING,
    MCSYNC_FOLLOWER_STATE_CONNECTED,
    MCSYNC_FOLLOWER_STATE_DISCONNECTING
};

enum
{
	MCSYNC_NONE_EXIST,
	MCSYNC_SAWS_EXIST,
	MCSYNC_AWS_EXIST,
};

/*************************************************************************************************
* Define
**************************************************************************************************/
#define MCSYNC_SHARE_MODE_DISABLE					0x00
#define MCSYNC_SHARE_MODE_NORMAL_ENABLE				0x01
#define MCSYNC_SHARE_MODE_FOLLOWER_ENABLE			0x02

#define MCSYNC_MODULE_BT_GROUP_NUM     				(MCSYNC_MODULE_BT_GROUP_END 			- MCSYNC_MODULE_BT_GROUP_START				+1)
#define MCSYNC_MODULE_GVA_GROUP_NUM     			(MCSYNC_MODULE_GVA_GROUP_END 			- MCSYNC_MODULE_GVA_GROUP_START				+1)
#define MCSYNC_MODULE_XAOAI_GROUP_NUM     			(MCSYNC_MODULE_XAOAI_GROUP_END 			- MCSYNC_MODULE_XAOAI_GROUP_START			+1)
#define MCSYNC_MODULE_AMA_GROUP_NUM     			(MCSYNC_MODULE_AMA_GROUP_END 			- MCSYNC_MODULE_AMA_GROUP_START				+1)
#define MCSYNC_MODULE_AUDIO_GROUP_NUM     			(MCSYNC_MODULE_AUDIO_GROUP_END 			- MCSYNC_MODULE_AUDIO_GROUP_START			+1)
#define MCSYNC_MODULE_SYS_GROUP_NUM     			(MCSYNC_MODULE_SYS_GROUP_END 			- MCSYNC_MODULE_SYS_GROUP_START				+1)
#define MCSYNC_MODULE_PPH_GROUP_NUM     			(MCSYNC_MODULE_PPH_GROUP_END 			- MCSYNC_MODULE_PPH_GROUP_START				+1)
#define MCSYNC_MODULE_NEWTON_GROUP_NUM      		(MCSYNC_MODULE_NEWTON_GROUP_END 		- MCSYNC_MODULE_NEWTON_GROUP_START			+1)
#define MCSYNC_MODULE_RESERVED1_GROUP_NUM   		(MCSYNC_MODULE_RESERVED1_GROUP_END 		- MCSYNC_MODULE_RESERVED1_GROUP_START		+1)
#define MCSYNC_MODULE_PROFILE_LAYER_GROUP_NUM      	(MCSYNC_MODULE_PROFILE_LAYER_GROUP_END 	- MCSYNC_MODULE_PROFILE_LAYER_GROUP_START	+1)
<<<<<<< HEAD
=======
#define MCSYNC_MODULE_TILE_GROUP_NUM     			(MCSYNC_MODULE_TILE_GROUP_END 			- MCSYNC_MODULE_TILE_GROUP_START			+1)
>>>>>>> db20e11 (second commit)

/*************************************************************************************************
* Structure
**************************************************************************************************/

typedef struct AWS_MCE_START_SERVICE_CFM_T
{
	BT_GENERIC_STATUS_E status;
}AWSMCE_START_SERVICE_CFM_T;

typedef struct AWS_MCE_STOP_SERVICE_CFM_T
{
	BT_GENERIC_STATUS_E status;
}AWSMCE_STOP_SERVICE_CFM_T;

typedef struct AWS_MCE_CONNECT_CFM_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}AWSMCE_CONNECT_CFM_T;

typedef struct AWSMCE_STATUS_CHANGE_IND_T
{
	U8 role;
	BD_ADDR_T bdAddr;
}AWSMCE_STATUS_CHANGE_IND_T;

typedef struct AWS_MCE_DISCONNECT_IND_T
{
	BT_GENERIC_STATUS_E status;
	BD_ADDR_T bdAddr;
}AWSMCE_DISCONNECT_IND_T;

typedef struct AWS_MCE_PICO_CLOCK_CFM_T
{
	U8 status;
	U32 clock;
	U8 event;
}AWS_MCE_PICO_CLOCK_CFM_T;

typedef struct AWSMCE_SYNC_SMART_CHARGE_CASE_STATE_IND_T
{
	U8 smartChargeCaseState;
}AWSMCE_SYNC_SMART_CHARGE_CASE_STATE_IND_T;

typedef struct AWSMCE_SYNC_KEY_ACTION_IND_T
{
	U16 keyEventCode;
	U32 supportedState;

}AWSMCE_SYNC_KEY_ACTION_IND_T;

typedef struct AWSMCE_SYNC_VPRT_IND_T
{
	U16 rtIndex;
	U16 vpIndex;
	U16 eventIndex;
	U32 currentPicoClock;
	U8 overwriteTime;
}AWSMCE_SYNC_VPRT_IND_T;

typedef struct AWSMCE_SYNC_STOP_VPRT_IND_T
{
	U16 eventIndex;
	U32 currentPicoClock;
}AWSMCE_SYNC_STOP_VPRT_IND_T;

typedef struct AWSMCE_SYNC_VPRT_PLAY_LATER_IND_T
{
	U16 eventIndex;
	U32 clockToStart;
}AWSMCE_SYNC_VPRT_PLAY_LATER_IND_T;

typedef struct AWSMCE_SYNC_CALLER_ID_IND_T
{
	U8 callerIdLength;
	U8 callerIdData[1];
}AWSMCE_SYNC_CALLER_ID_IND_T;

typedef struct AWSMCE_AGENT_A2DP_EVT_IND_T
{
	BD_ADDR_T bdAddr;
	MCSYNC_A2DP_INFO_T a2dpInfo;
}AWSMCE_AGENT_A2DP_EVT_IND_T;

typedef struct AWSMCE_AGENT_SCO_EVT_IND_T
{
	BD_ADDR_T bdAddr;
	MCSYNC_SCO_INFO_T scoInfo;
}AWSMCE_AGENT_SCO_EVT_IND_T;

typedef struct AWSMCE_SYNC_PEQREALTIME_IND_T
{
	U16 dataLength;
	U16 reserved;
	U32 clockToStart;
	U8 data[1];
}AWSMCE_SYNC_PEQREALTIME_IND_T;

typedef struct AWSMCE_RHO_PACKET_IND_T
{
	U16 length;
	MCSYNC_RHO_PACKET_T rhoRxPkt;
}AWSMCE_RHO_PACKET_IND_T;

typedef struct AWSMCE_AWS_IDLE_TIMER_CTL_IND_T
{
	U8 cmd;
}AWSMCE_AWS_IDLE_TIMER_CTL_IND_T;

typedef struct AWSMCE_SYNC_POWEROFF_IND_T
{
	U8 reason;
	U8 isFollowerIFPKT:1;
	U8 isOnlyForFollower:1;
	U8 reserved:6;
}AWSMCE_SYNC_POWEROFF_IND_T;

typedef struct MCSYNC_ROLEMODECHANGE_CFM_T
{
	U8 status;
}MCSYNC_ROLEMODECHANGE_CFM_T;

typedef struct MCSYNC_SETAIRPAIRING_CFM_T
{
	U8 status;
}MCSYNC_SETAIRPAIRING_CFM_T;

typedef struct AWSMCE_SYNC_NEED_DISCONNECT_T
{
	U8 type;
}AWSMCE_SYNC_NEED_DISCONNECT_T;

typedef struct AWSMCE_SYNC_GENERAL_DATA_IND_T
{
	U32 length;
	U8 data[1];
}AWSMCE_SYNC_GENERAL_DATA_IND_T;

typedef struct MCSYNC_SET_CURRENT_ROLE_CFM_T
{
	U8 role;
}MCSYNC_SET_CURRENT_ROLE_CFM_T;

typedef struct MCSYNC_SYNC_SHARE_MODE_INFO_IND_T
{
	U8 state;
	U8 shareMode;
}MCSYNC_SYNC_SHARE_MODE_INFO_IND_T;

typedef struct AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND_T
{
	U8 soundLevel;
	U8 isFollowerIFPkt;
}AWSMCE_SYNC_FOLLOWER_SOUNDLEVEL_IND_T;

typedef struct AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND_T
{
	U8 specificRole;
}AWSMCE_SYNC_FOLLOWER_RETRIGGER_A2DP_TRANSFORM_IND_T;


typedef struct AWSMCE_SYNC_OTHER_AGENT_BDA_IND_T
{
	BD_ADDR_T bdAddr;
}AWSMCE_SYNC_OTHER_AGENT_BDA_IND_T;

typedef struct AWSMCE_SYNC_FOLLOWER_CONN_IND_T
{
	U8 		  doConn;
}AWSMCE_SYNC_FOLLOWER_CONN_IND_T;

typedef struct AWSMCE_SYNC_FOLLOWER_DISC_IND_T
{
	U8 isOnlyForFollower;
	U8 isFollowerIFPkt;
}AWSMCE_SYNC_FOLLOWER_DISC_IND_T;

typedef struct AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T
{
	U8 type;
	U8 value;
	U8 isFollowerIFPkt;
}AWSMCE_SYNC_AGENT_RETRANSMIT_FOLLOWER_DATA_IND_T;

typedef struct AWSMCE_INTERNAL_SMART_CHARGE_CASE_STATE_CMD_T
{
	U8 state;
}AWSMCE_INTERNAL_SMART_CHARGE_CASE_STATE_CMD_T;

typedef struct AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T
{
	U8 isLeaveShareMode;
	U8 isFollowerIFPkt;
}AWSMCE_SYNC_AGENT_NOTICE_FOLLOWER_LEAVE_IND_T;

typedef struct AWSMCE_SYNC_PARTNER_NOTICE_AGENT_POWER_OFF_IND_T
{
	U8 reason;
	U8 isFollowerIFPkt;
}AWSMCE_SYNC_PARTNER_NOTICE_AGENT_POWER_OFF_IND_T;

typedef struct AWSMCE_SYNC_KEYCODE_IND_T
{
	U16 keyCode;
}AWSMCE_SYNC_KEYCODE_IND_T;

typedef struct
{
    U8 newState;
} AWSMCE_SYNC_STATE_CHANGED_IND_T;

/*************************************************************************************************
* Define
**************************************************************************************************/
#define AWSMCE_AUTO_RECONN_AFTER_PWR_ON_FEAT 			0x0001
#define AWSMCE_SYNC_FACTORY_RESET_ROLE_FEAT				0x0002
#define AWSMCE_AUTO_DISCOVERABLE_FEAT 					0x0004
#define AWSMCE_AUTO_RECONNECT_AFTER_DISCONNECT_FEAT 	0x0008
#define AWSMCE_SYNC_PWR_OFF_KEY_FEAT 					0x0010

/*************************************************************************************************
* Public function
**************************************************************************************************/
void BtAwsMce_StartService(Handler app);
U8 BtAwsMce_GetDefaultRole(void);
U8 BtAwsMce_GetNvKeyRole(void);
BOOL BtAwsMce_IsSpecialLink(BD_ADDR_T *pBdAddr);
BOOL BtMCSync_IsFeatureSupport(U16 feature);
BD_ADDR_T* BtMCSync_ReadDeviceBdAddr(void);
BD_ADDR_T* BtAwsMce_ReadLocalBdAddr(void);
BD_ADDR_T* BtMCSync_ReadAgentBdAddr(void);
BD_ADDR_T* BtMCSync_ReadPartnerBdAddr(void);
BD_ADDR_T* BtMCSync_GetCurrentLinkBdAddr(void);
BD_ADDR_T* BtMCSync_GetNormalLinkBdAddr(void);
BOOL BtAwsMce_IsDefaultRoleAgent(void);
BOOL BtAwsMce_IsDefaultRolePartner(void);
BOOL BtAwsMce_IsDefaultRoleNone(void);
void BtAwsMce_KeyEnableDiscoverable(void);
void BtAwsMce_GetCurrentPiconetBtClock(BD_ADDR_T *pBdAddr, U8 event);
BOOL BtAwsMce_IsWithPartner(BD_ADDR_T *pBdAddr);
void BtAwsMce_AgentSavePartnerBdAddr(BD_ADDR_T *pBdAddr);
void BtAwsMce_WriteDefaultInfo(U8 defaultRole, BD_ADDR_T *pAgentBdAddr, BD_ADDR_T *pPartnerBdAddr);
U32 BtAwsMce_GetCurrentBtClock(void);
void BtAwsMce_Init(void);
void BtAwsMce_ClearAudioPathInfo(void);
void BtAwsMce_DisconnectAwsLink(void);
void BtAwsMce_DisconnectSpecialLink(void);
void BtAwsMce_SetAirPairing(BOOL isAirPairing);
BOOL BtAwsMce_IsInAirPairing(void);
void BtAwsMce_SetMpTestMode(BOOL isMpTestMode);
<<<<<<< HEAD
=======
void BtAwsMce_SetDiscoverableEnable(BOOL isEnable);
>>>>>>> db20e11 (second commit)
void BtAwsMce_SetSingleHSMode(BOOL isSingle);
U8 BtAwsMce_GetMcsyncState(void);
void BtAwsMce_PartnerCancelCreateConn(void);
void BtAwsMce_AgentDisconnAwsLink(BD_ADDR_T *pBdAddr);
BD_ADDR_T* BtAwsMce_AgentGetPartnerBdAddr(void);
BOOL BtMCSync_IsPhoneLinkUnderAgentRole(BD_ADDR_T *pBdAddr);
BOOL BtMCSync_IsRegistered(void);
void BtMCSync_ReadAFH(BD_ADDR_T *pBdAddr);
BOOL BtMCSync_PartnerMode(void);
<<<<<<< HEAD
void BtMCSync_AgentMode(void);
=======
BOOL BtMCSync_AgentMode(void);
>>>>>>> db20e11 (second commit)
void BtMCSync_AllowAllRoleConnSP(BOOL isAllowAllRoleConnSP);
U8 BtMCSync_GetCurrentRole(void);
void BtMCSync_SetDefaultDeviceBdAddr(void);
void BtMCSync_SetDefaultRoleByLocalBdAddr(void);
void BtMCSync_ResetSlaveRetryCount(void);
void BtMCSync_ClearLinkInfo(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Public function (Send Synchronize Information)
**************************************************************************************************/
void BtMCSync_SendSyncA2dpInfo(U8 codec, U8 avrcpVolume, U8 peqIndex, U8 scms, BOOL isGameMode, U8 numOfAg, U8 specificRole);
void BtMCSync_SendSyncScoInfo(U8 speakerSoundLevel, U8 micSoundLevel, U8 scoState, U8 topState);
void BtMCSync_SendSyncSmartChargeCaseStateInfo(U8 state);
void BtMCSync_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState);
void BtMCSync_SendSyncVpRtInfo(U16 rtIndex, U16 vpIndex, U16 eventIndex, U32 currentPicoClock, U8 overwriteTime);
void BtMCSync_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock);
void BtMCSync_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart);
void BtMCSync_SendSyncCallIdInfo(U8 *pCallerId, U8 length);
void BtMCSync_SendSyncPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart);
void BtMCSync_SendSyncDSPInfo(void *data, U32 length, U32 clockToStart);
void BtMCSync_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower);
void BtMCSync_SendSyncGeneralData(void *pData, U32 length);
void BtMCSync_SendLeakageDetectInfo(BOOL isStart, BOOL result);
void BtMCSync_SendSyncXiaoaiDataToAgent(U16 version);
void BtMCSync_SendSyncBatteryLevel(U8 batteryLevel);

BOOL bt_mcsync_send_data_by_module_id(U16 module_id, U16 length, U8 *p_data);

/**************************************************************************************************
* API, data module
**************************************************************************************************/
BOOL bt_mcsync_register_module(MODULE_TABLE_T *p, U8 num, U8 group_id);
void bt_mcsync_data_init(void);

/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL BtAwsMceLock();
BOOL BtAwsMceUnlock();
BOOL BtAwsMceIsAllow();
U16 BtAwsMceGetRhoDataLength();
BOOL BtAwsMceGetRhoData(U8 *pBuffer);
BOOL BtAwsMceApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);
BOOL BtAwsMceApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);
void BtAwsMceSetRhoState(U8 state);
U8	BtAwsMceGetRhoState(void);
BOOL BtMCSyncIsRhoing(void);
void BtAwsMceDisableAdvHandle(void);

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
void BtMCSync_SendSyncShareModeInfo(U8 state, U8 shareMode);
void BtMCSync_SendSyncFollowerSoundLevel(U8 soundLevel, U8 isFollowerIFPkt);
void BtMCSync_SendSyncOtherAgentBDA(BD_ADDR_T *pBdAddr);
void BtMCSync_SendSyncFollowerConn(U8 doConn);

void BtMCSync_SendSyncFollowerDisconn(U8 isOnlyForFollower, U8 isFollowerIFPkt);
void BtMCSync_SendSyncFollowerRetriggerA2dp(void);
void BtMCSync_SendSyncAgentRetransmitData(U8 type, U8 value, U8 isFollowerIFPkt);
void BtMCSync_SendSyncAgentNoticeFollowerLeave(U8 isLeaveShareMode, U8 isFollowerIFPkt);
void BtMCSync_SendExitShareRequest(void);
/**************************************************************************************************
* Plus Mode
**************************************************************************************************/
void BtMCSync_AgentStartShareMode(Handler handler);
void BtMCSync_AgentStopShareMode(Handler handler);
void BtMCSync_SetShareMode(U8 setMode);
U8   BtMCSync_GetShareMode(void);
void BtMCSync_FollowerScanOffEvent(void);
void BtMCSync_FollowerStopShareMode(void);
BOOL BtMCSync_IsShareModeBdAddr(BD_ADDR_T *pBdAddr);
void BtMCSync_SetShareModeBdAddr(BD_ADDR_T *pBdAddr);
BD_ADDR_T *BtMCSync_GetShareModeBdAddr(void);
void BtMCSyunc_SetMCSyncSharePairing(BOOL isEarbudsPlusPairing);
BOOL BtMCSync_IsInMCSyncSharePairing(void);
void BtMCSync_SetOtherAgentBDA(BD_ADDR_T *pOtherAgentBDA);
BD_ADDR_T* BtMCSync_GetOtherAgentBDA(void);
void BtMCSync_ClearShareState(void);
U8 BtMCSync_GetShareState(void);
#else
void BtMCSync_AgentStartShareMode(Handler handler);

#endif

/**************************************************************************************************
* Take over connection (API)
**************************************************************************************************/
<<<<<<< HEAD
#ifdef TAKE_OVER_LINK
=======
#ifdef AIR_TAKE_OVER_LINK_ENABLE
>>>>>>> db20e11 (second commit)
void 	BtMCSync_SetMcsyncTakeOverConnState(U8 state);
U8 		BtMCSync_GetMcsyncTakeOverConnState(void);
BOOL 	BtMCSync_CheckIsLowPrioLink(BD_ADDR_T *pBdAddr);
MCSYNC_LINK_STRU *BtMCSync_GetHighPrioLink(void);
MCSYNC_LINK_STRU *BtMCSync_GetLowPrioLink(void);

#endif
//MCSYNC_LINK_STRU *MCSYNC_GetLowPrioLink(void)
U8 BtMCSync_GetLinkStatus(void);


#endif //_BTAWSMCE_H_
