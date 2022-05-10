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


#ifndef _APP_RESOURCEMANAGER_H_
#define _APP_RESOURCEMANAGER_H_

#include "App_Connection.h"
#include "AudioDSP_Registration.h"
#include "App_ReConnection.h"
#include "hfp.h"
#include "PM_Interface.h"


#if 0 /* Unused LOG_PRINT_156X */
<<<<<<< HEAD
#define DBG_LOG_APP_RSRC_MNG(_message, arg_cnt, ...) 
=======
#define DBG_LOG_APP_RSRC_MNG(_message, arg_cnt, ...)
>>>>>>> db20e11 (second commit)
#else
#define DBG_LOG_APP_RSRC_MNG(_message, arg_cnt, ...)   LOG_MSGID_I(APP_RSRC_MNG, _message, arg_cnt, ##__VA_ARGS__)
#endif

/**************************************************************************************************
* Define
**************************************************************************************************/
#define MAX_MULTI_POINT_NO 2

// link misc
#define APP_LINK_LOSS						0x01
#define APP_LINK_DETACHING					0x02
#define APP_ACTIVE_CONNECT					0x04
#define APP_ACTIVE_DISCONNECT				0x40
#define APP_IS_ASSIGNED_TO_BE_ACTIVE_LINK	0x80

//system misc
//Misc 2
#define APP_EXIT_DISCOVERABLE_STATE					0x20
#define APP_CREATE_CONN_AFTER_AUDIO_TRANSFER		0x80

//MISC 4
#define APP_MASK4_RECONNECT_LINK0					0x02
#define APP_MASK4_RECONNECT_LINK1					0x04


#define APP_MAX_ONGOING_CALL_NO		0x02

#define CURRENT_ACTIVE_LINK_CNT    	APP_GetACLLinkCount()

#define APP_SetWatchDogReboot(a)					(gAppCtl.driverCtl.isWatchdogReboot = (a))

#define APP_SetEnterDiscoverableAfterDisc(a)		(gAppCtl.enterDiscoverableAfterDisc = (a))
#define APP_IsEnterDiscoverableAfterDisc()			(gAppCtl.enterDiscoverableAfterDisc)

<<<<<<< HEAD
=======
#define APP_SetKeepDiscoverableAfterDisc(a)		    (gAppCtl.keepDiscoverableAfterDisc = (a))
#define APP_IsKeepDiscoverableAfterDisc()			(gAppCtl.keepDiscoverableAfterDisc)

>>>>>>> db20e11 (second commit)
#define APP_SetKeyInvalidPwOnLowBat(a)				(gAppCtl.driverCtl.keyInvalidInPwrOnLowBat = (a))
#define APP_IsKeyInvalidPwOnLowBat()				(gAppCtl.driverCtl.keyInvalidInPwrOnLowBat)

#define APP_SetNoConnectivityPwOnLowBat(a)			(gAppCtl.driverCtl.noConnectivityInPwrOnLowBat = (a))
#define APP_IsNoConnectivityPwOnLowBat()			(gAppCtl.driverCtl.noConnectivityInPwrOnLowBat)

#define APP_SetButtonLock(a)						(gAppCtl.driverCtl.isButtonLocked = (a))
#define APP_IsButtonLocked()						(gAppCtl.driverCtl.isButtonLocked)

#define APP_SetSwitchingSCO(a)						(gAppCtl.driverCtl.switchingSCO = (a))
#define APP_IsSwitchingSCO()						(gAppCtl.driverCtl.switchingSCO)

#define APP_SetRmvDetachState(a)					(gAppCtl.isRmvDetachState = (a))
#define APP_IsRmvDetachState()						(gAppCtl.isRmvDetachState)
/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SCO_STATE_DISC,
	SCO_STATE_ACTIVE_CONNECT,
	SCO_STATE_PASSIVE_CONNECT,
	SCO_STATE_CONNECTED,
	SCO_STATE_DISCONNECTING_WAIT_SEND_REQ,
	SCO_STATE_DISCONNECTING_WAIT_REQ_CPL,
};

enum
{
	DSP_ANC_AT_OFF,
	DSP_ANC_ON,
	DSP_AT_ON,
};

enum
{
	APP_CALL_NO_3WAY,
	APP_CALL_IN_3WAY,
};

enum
{
	ACL_STATE_OFF = 0,
	ACL_STATE_ON,
};

#ifdef PROFILE_AMA_ENABLE
enum
{
    APP_AMA_STATE_NONE,
    APP_AMA_STATE_CONNECTED,
};
#endif

enum
{
	PROCD_RELEASE,
	NOTPROCD
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 scoState:7;
	U8 isActiveConnected:1;
	SINK scoSink;
	U8 scoCodec;
    U16 scoHandle;
}SCO_CTL_STRU;

typedef struct
{
	U8 remoteIoCap;
	U8 remoteOOBDataPresent;
	U8 remoteMitm;
	U8 remoteBonding;
	U8 isIoCapRespSet;
}IOCAP_CTL_STRU;

typedef struct
{
	U8 soundLevelSco;
	U8 absVolume;
	U8 vgm;
	U8 vgs;
	U8 miscMask;
	U8 soundLevelA2DP;
	U8 avrcpVolume;
	U8 micSoundLevelA2DP;
	U8 micSoundLevelSco;


	//U8 scoType;
	//U8 scoPktType;
	//U8 txInterval; //transmission interval
	//U16 rxPacketLen;

	U8 isMicMute : 1;
	U8 isSpkMuteForMicMute : 1;
	U8 isSpkMuteForSCO : 1;
	U8 reserved : 5;

	IOCAP_CTL_STRU ioCtl;
}APP_LINK_SETTING_STRU;


typedef struct mST
{
	U8 nestState;
	struct mST *pNextState;
}APP_STATE_STRU;//MmiStateType;

#include "Hfp_Interface.h"
typedef struct
{
	U8 length;
	U8 callNumId[CALLER_ID_DIGIT_NO];
}PHONE_ID_STRU;

typedef struct
{
	U8 service;
	U8 call;
	U8 callsetup;
	U8 callheld;
	U8 signal;	//signaling channel
	U8 roam;	//media channel
	U8 chgbat;
}HFPInitIndicatorType;

typedef struct
{
	U8 isHsp;
	HFPInitIndicatorType indicator;
	U8 ongoingCallNo;
	U8 callState[APP_MAX_ONGOING_CALL_NO];
	U8 appleSiriStatus;
	PHONE_ID_STRU incomingCall;

	U8 isAudioTransfer:1;
	U8 reserved:2; // sendingVGSVGM flag for avoiding repeated sending AT+VGS --> 1530 is handle in HFP profile
	U8 isCallerNameGet:1;
	U8 isCallerIdGet:1;
	U8 isImgVPTriggered:1;
	U8 is3WayAdd:1;
	U8 isCallRejectedByKey : 1;

	//#ifdef _MEMORY_DIALING_
	U16 memoryDialIndex; //[0-65535]
	//#endif
	//#ifdef _EXTENSION_ENTITIES_
	U8 DTMFPara;
	//#endif
}APP_HFP_CTL_STRU;

typedef struct
{
	U8 reserved1;
}APP_A2DP_CTL_STRU;

typedef struct
{
	U8 isGetPlayStatusSupported : 1;
	U8 isSendingGetPlayStatus : 1;
	U8 isMusicPlay : 1;
	U8 isDisablePlayStatusMonitor : 1;
	U8 isPlayStatusWorking : 1;
	U8 reserved : 3;
	U8 playStatus;
	U8 lastTxOpIDForPlayPause;
	U8 getAppSettingsOwner;
}APP_AVRCP_CTL_STRU;

typedef struct
{
	U8			flag;
	U8			serviceType;
	U32			tokenRate;
	U32			peakBandwidth;
	U32			latency;
	U32			delayVariation;
	U8			cmdStatus;
}APP_QOS_CTL;

typedef struct
{
	BD_ADDR_T bdAddr;
	U8 aclState;
	APP_LINK_SETTING_STRU linkPara;
	SCO_CTL_STRU scoCtl;
	APP_STATE_STRU *pNestState;

	U16 audioDevLinkMask;

	U8 subState;
	U8 stateEntryExit;
	APP_HFP_CTL_STRU hfpCtl;
	APP_A2DP_CTL_STRU a2dpCtl;
	APP_AVRCP_CTL_STRU avrcpCtl;

	U32 sniffProfileMask;
	APP_QOS_CTL qosCtl;
}APP_INFO_STRU;// AppInfoType;

typedef struct
{
	//U8 resetPairedDeviceMediaCount;
	U8 keyDownState;
	//U8 resetLinkType;
	//U8 audioSwitchState:4;
	//U8 isAsyncSCOOn:1;
	//U8 reserved:3;

	U8 isButtonLocked:1;
	U8 dbgPrivateKey:1;
	U8 reserved:1;
	U8 isReset:1;
	U8 isWatchdogReboot:1;
	U8 keyInvalidInPwrOnLowBat:1;
	U8 noConnectivityInPwrOnLowBat:1;
	U8 switchingSCO:1;
}APP_DRIVER_RELATED_CTL_STRU;//MMIDriverRelatedCtlType

typedef struct
{
	U8 vpLangIsCfm;
	U8 vpSelectIndex;
	U8 vpSelectRound;
	U8 vpSelectOriginalIndex;
} VoicePromptRuntime;


typedef struct
{
	U8 agentScoState;
	U8 agentScoTopState;
}APP_AWSMCE_SCO_STRU;

typedef struct
{
	U8 agentState;
	U8 numOfAG;
	BD_ADDR_T partnerBdAddr;
	APP_AWSMCE_SCO_STRU scoCtl;
	U8 partnerSmartChargeCaseState;
}APP_AWSMCE_CTL_STRU;


typedef struct
{
	APP_INFO_STRU appInfo[FW_MAX_LINK_IDX];//mmiInfo

	BD_ADDR_T serviceBdAddr;//U8 serviceInd;
	BD_ADDR_T latestConnectedBdaddr;//U8 latestConnectedLinkInd
	//////////// DO NOT add any new variable above this warning!!!!!!!!!!! coze mmi_init would be fail

	U8 scoCount;//scoCnt
	U8 msgHandled;

	U8 isEsdPowerOn : 1;
	U8 enterDiscoverableAfterDisc : 1;
	U8 isMicEnableInA2DP : 1;
	U8 isMicAdjsutInA2DP : 1;
	U8 isVolumeSetInitInA2DP : 1;
	U8 lockDSP : 1;
	U8 DSPSuspendCpl : 1;
	U8 DSPSuspendCmdOneTime : 1;
	BD_ADDR_T *bdAddrMicCtlInA2DP;//linkIndexMicCtlInA2DP


	U8 miscMask2;

	VoicePromptRuntime vpRuntime;

	U8 connectableState;
	U8 discoverableState;
	APP_DRIVER_RELATED_CTL_STRU driverCtl;
	U8 vprtSoundLevel;
	U8 lineinSoundLevel;
	U8 mp3SoundLevel;
	BD_ADDR_T redialBdAddr;
	BD_ADDR_T imgCallBdAddr;
	U8 dspAncAtState;
	U8 airoThruSoundLevel;

	APP_AWSMCE_CTL_STRU awsmceCtl;

	U32 speakerMuteMask;
	U8 isRmvDetachState:1;
<<<<<<< HEAD
	U8 reserve:7;
=======
        U8 keepDiscoverableAfterDisc:1;
	U8 reserve:6;
>>>>>>> db20e11 (second commit)
	/*
#ifdef TRSPX_App
	U32 passkey[MAX_MULTI_POINT_NO];
	U8 connectedMode;
#endif
	*/
}APP_CTL_STRU; //AppCtlType;

typedef struct
{
    BD_ADDR_T audioA2dpBdAddr;
    U8 isAudioA2dpOn: 1;
} APP_ESD_STRU;


#ifdef WIN32
#pragma pack(push,1)
#endif

////////// APP Message Type //////////.
// Message Types
typedef struct
{
	BD_ADDR_T bdAddr;
	U8 playStatus;
	void (*fcallback)(BD_ADDR_T *pBdAddr, U8 isSuccessful);
}AVRCP_TASK_CMD_STRU;

// Message Body Union
typedef union
{
	AVRCP_TASK_CMD_STRU	avrcpTaskCmd;
}APP_MSG_BODY_STRU;


typedef struct
{
	U16 msgOpcode;
	U16 eventCode;
	U8 paraLen;
	APP_MSG_BODY_STRU pMsgBody;
}APP_MSG_TYPE_STRU;

#ifdef WIN32
#pragma pack(pop)
#endif

/**************************************************************************************************
* Variables
**************************************************************************************************/
extern APP_CTL_STRU gAppCtl;//gMMI_ctl
extern APP_ESD_STRU gAppEsd;

/**************************************************************************************************
* Public Functions (Link)
**************************************************************************************************/
BOOL APP_IsSetAppLink(BD_ADDR_T *pBdAddr);
BOOL APP_SetAppLinkBdAddr(BD_ADDR_T *pBdAddr);
void APP_ClearAppLinkBdAddr(BD_ADDR_T *pBdAddr);
APP_INFO_STRU *APP_GetEmptyAppLink();
APP_INFO_STRU *APP_GetAppLinkByBdAddr(BD_ADDR_T *pBdAddr);
APP_INFO_STRU *APP_GetDefaultAppLink(void);
U8 APP_GetLinkIndexByBdAddr(BD_ADDR_T *pBdAddr);
BD_ADDR_T *APP_GetBdAddrByLinkIndex(U8 linkIndex);

BOOL APP_IsLinkActiveDisconnecting(BD_ADDR_T *pBdAddr); //MMI_IsLinkActiveDisconnecting
<<<<<<< HEAD
=======
U8 APP_GetLinkActiveDisconnectingCnt(void);
>>>>>>> db20e11 (second commit)

void APP_SetACLState(APP_INFO_STRU * pLinkInfo, U8 aclState);
BOOL APP_IsActiveLink(BD_ADDR_T *pBdAddr);//MMI_IsActiveLink
U8 APP_GetACLLinkCount(void);

/**************************************************************************************************
* Public Functions (SCO)
**************************************************************************************************/
BD_ADDR_T *APP_GetBdAddrBySCOSink(SINK sink);
SINK APP_GetSCOSinkByBdAddr(BD_ADDR_T *pBdAddr);
U16 APP_GetSCOHandleByBdAddr(BD_ADDR_T *pBdAddr);
void APP_SetSCOConnected(BD_ADDR_T *pBdAddr, SINK sink, U16 scoHandle, U8 scoCodec);
void APP_SetSCODisconnected(BD_ADDR_T *pBdAddr);
BOOL APP_IsScoConnected(BD_ADDR_T *pBdAddr);
void APP_SetScoState(APP_INFO_STRU *pLinkInfo, U8 state);
U8 APP_GetScoState(APP_INFO_STRU *pLinkInfo);
U8 APP_GetScoCodec(BD_ADDR_T *pBdAddr);
BOOL APP_IsScoActiveConnect(APP_INFO_STRU *pLinkInfo);

#define APP_SCO_EXISTS(A)	(APP_IsScoConnected(A))    //SCO_EXISTS

/**************************************************************************************************
* Public Functions (Audio Device)
**************************************************************************************************/
U16 APP_GetAudioDevLinkMask(BD_ADDR_T *pBdAddr);
void APP_SetAudioDevLinkMask(BD_ADDR_T *pBdAddr, U8 audioDev);
void APP_ClearAudioDevLinkMask(BD_ADDR_T *pBdAddr, U8 audioDev);
void APP_ClearAllLinksAudioDevLinkMask(void);
void APP_SetSpeakerMuteMask(U32 mask);
void APP_RmvSpeakerMuteMask(U32 mask);
U32 APP_GetSpeakerMuteMask(void);

/**************************************************************************************************
* Public Functions (HFP Profile)
**************************************************************************************************/
APP_HFP_CTL_STRU * 	APP_GetHfpLinkInfo(BD_ADDR_T *pBdAddr);
BD_ADDR_T * APP_GetBdAddrByHfpLinkInfo(APP_HFP_CTL_STRU * pHfpLinkInfo);
U8 APP_GetHfpLinkAudioTransfer(BD_ADDR_T *pBdAddr);
void APP_SetHfpLinkAudioTransfer(BD_ADDR_T *pBdAddr, BOOL isAudioTransfer);

U8 APP_GetHfpIsRejectByKey(BD_ADDR_T *pBdAddr);
void APP_SetHfpIsRejectByKey(BD_ADDR_T *pBdAddr, BOOL isRejectByKey);
void APP_ClearHfpLinkInfo(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Public Functions (AVRCP Profile)
**************************************************************************************************/

APP_AVRCP_CTL_STRU * APP_GetAvrcpLinkInfo(BD_ADDR_T *pBdAddr);
void APP_ClearAvrcpLinkInfo(BD_ADDR_T *pBdAddr);
BD_ADDR_T * APP_GetBdAddrByAvrcpLinkInfo(APP_AVRCP_CTL_STRU * pAvrcpLinkInfo);

/**************************************************************************************************
* Public Functions (Service)
**************************************************************************************************/
BOOL APP_IsServiceBdAddr(BD_ADDR_T *pBdAddr);
void APP_SetServiceBdAddr(BD_ADDR_T *pBdAddr);
void APP_ClearServiceBdAddr(void);
BD_ADDR_T *APP_GetServiceBdAddr();

/**************************************************************************************************
* Public Function (Multi Links)
**************************************************************************************************/
BD_ADDR_T *APP_GetNextNormalLink(BD_ADDR_T *pBdAddr);//APP_GetNextNormalLink
void APP_SetRedialBdAddr(BD_ADDR_T *pBdAddr);
BD_ADDR_T *APP_GetRedialBdaddr(void);
BOOL APP_CheckRedialBdaddrisZero();
void APP_SetImgCallBdAddr(BD_ADDR_T *pBdAddr);
BD_ADDR_T *APP_GetImgCallBdaddr(void);
APP_QOS_CTL * APP_SearchQosCtl(BD_ADDR_T *pBdAddr);


/**************************************************************************************************
* Public Functions (AWS MCE)
**************************************************************************************************/
<<<<<<< HEAD
=======
U8 APP_GetNormalLinkNum();
BD_ADDR_T* APP_GetNormalLinkAddr();
>>>>>>> db20e11 (second commit)
void APP_SetAgentState(U8 state);
U8 APP_GetAgentState(void);
void APP_SetPartnerSmartChargeCaseState(U8 state);
U8 APP_GetPartnerSmartChargeCaseState(void);
void APP_SetAGNum(U8 AGNum);
U8 APP_GetAGNum(void);
void APP_SetAgentScoState(U8 agentScoState);
U8 APP_GetAgentScoState(void);
void APP_SetAgentScoTopState(U8 agentScoTopState);
U8 APP_GetAgentScoTopState(void);
void APP_AgentSavePartnerInfo(BD_ADDR_T *pBdAddr);
BD_ADDR_T *APP_AgentGetPartnerInfo(void);
BOOL APP_FillRhoData(void *pRhoStru);
BOOL APP_AssignRhoData(void *pRhoStru);
BOOL APP_IsAllowHandler(void);
void APP_SetNewPartnerConfig(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr);

/**************************************************************************************************
* Public Functions (ESD)
**************************************************************************************************/
void APP_SetEsdInfo(BOOL isAudioA2dpOn, BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Internal Public Functions (APP Command / Event Message)
**************************************************************************************************/
APP_MSG_TYPE_STRU *APP_GetAppCmdMsgMemory(U16 opCode);

#endif //_APP_RESOURCEMANAGER_H_
