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

#ifndef _APP_RHOHANDLER_H_
#define _APP_RHOHANDLER_H_

#include "App_ResourceManager.h"
#include "App_TestMode.h"
#include "Peq_NvkeyHandler.h"

#ifdef XIAOAI_DEVELOPMENT
#include "App_Xiaoai.h"
#endif

#ifdef OPPO_DEVELOPMENT
#include "App_Oppo.h"
#endif

<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
#include "App_Tile.h"
#endif

>>>>>>> db20e11 (second commit)
/**************************************************************************************************
* Definition
**************************************************************************************************/
#define APP_MSG_ID_SINK_FLUSH 0xFFFE

#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_APP_McsyncRho(_message, arg_cnt, ...)   printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_APP_McsyncRho(_message, arg_cnt, ...)   LOG_MSGID_I(APP_MCS_RHO, _message, arg_cnt, ##__VA_ARGS__)
#endif

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T bdAddr;
	U16 dataLength;
	U8 pData[1];
}PACKED APP_RHO_SINK_FLUSH_STRU;

typedef struct
{
	BD_ADDR_T serviceBdAddr;//U8 serviceInd;
	//BD_ADDR_T latestConnectedBdaddr;//U8 latestConnectedLinkInd
	//////////// DO NOT add any new variable above this warning!!!!!!!!!!! coze mmi_init would be fail

	U8 scoCount;
	U8 msgHandled;

	BOOL enterDiscoverableAfterDisc;
	//U8 isMicEnableInA2DP : 1;
	//U8 isMicAdjsutInA2DP : 1;
	//U8 isVolumeSetInitInA2DP : 1;
	//U8 lockDSP : 1;
	//U8 DSPSuspendCpl : 1;
	//U8 DSPSuspendCmdOneTime : 1;

	//BD_ADDR_T *bdAddrMicCtlInA2DP;
	U8 miscMask2;

	VoicePromptRuntime vpRuntime;

	//U8 connectableState;
	//U8 discoverableState;
	APP_DRIVER_RELATED_CTL_STRU driverCtl;
	U8 vprtSoundLevel;
	U8 lineinSoundLevel;
	U8 mp3SoundLevel;
	BD_ADDR_T redialBdAddr;
	BD_ADDR_T imgCallBdAddr;
	U8 dspAncAtState;
	U8 airoThruSoundLevel;
	APP_AWSMCE_CTL_STRU mcsyncCtl;
	U32 speakerMuteMask;
}APP_RHO_CTL_STRU;

typedef struct
{
	BD_ADDR_T bdAddr;
	U8 aclState;
	APP_LINK_SETTING_STRU linkPara;
	SCO_CTL_STRU scoCtl;

	U8 nestStateNumber;
	U8 nestState[7];

	U16 audioDevLinkMask;

	U8 subState;
	U8 stateEntryExit;
	APP_HFP_CTL_STRU hfpCtl;
	APP_A2DP_CTL_STRU a2dpCtl;
	APP_AVRCP_CTL_STRU avrcpCtl;

	U32 sniffProfileMask;
	APP_QOS_CTL qosCtl;
}APP_RHO_INFO_STRU;

typedef struct
{
	U16 autoSwitchOffMask[FW_MAX_LINK_IDX];
}APP_RHO_POWEROFF_CTL_STRU;

typedef struct
{
	U8 PEQGroup[PEQ_PATH_NUM];
}APP_RHO_PEQ_CTL_STRU;

typedef struct
{
	U8 imgAudioVoiceState;
	BD_ADDR_T bdAddr;
	U8 queueNo;
}APP_RHO_HFP_IMG_MEDIA_STRU;

typedef struct
{
	APP_RHO_HFP_IMG_MEDIA_STRU appHfpImgMedia;
}APP_RHO_HFP_STRU;

typedef struct
{
	U8 isMiscEnabled;
}APP_RHO_DSP_VP_STRU;

typedef struct
{
	U8 isSideToneOn;
}APP_RHO_DSP_SIDETONE_STRU;

typedef struct
{
	BD_ADDR_T bdAddr;
	BOOL lightMonoMode;
	BOOL IsEcnrOff;
}APP_RHO_DSP_SCO_STRU;

typedef struct
{
	U8 userRegister;
}APP_RHO_DSP_ANC_PASSTHRU_STRU;

typedef struct
{
	BD_ADDR_T bdAddr;
}APP_RHO_DSP_A2DP_STRU;

typedef struct
{
	BD_ADDR_T currentBdAddr;
	U8 currentAudioDev;
	U16 audioDevSysMask;
}APP_RHO_DSP_DEV_REG_STRU;

typedef struct
{
    BD_ADDR_T mainCtlBdAddr;
    BD_ADDR_T assistantCtlBdAddr;
    U8 pathCtl_audioType;
}APP_RHO_DSPCTL_STRU;

typedef struct
{
	APP_RHO_DSP_VP_STRU appVP;
	APP_RHO_DSP_SIDETONE_STRU appSideTone;
	APP_RHO_DSP_SCO_STRU appSCO;
	APP_RHO_DSP_ANC_PASSTHRU_STRU appAiroThru;
	APP_RHO_DSP_A2DP_STRU appA2DP;
	APP_RHO_DSP_DEV_REG_STRU appDevReg;
    APP_RHO_DSPCTL_STRU appDSPCtl;
}APP_RHO_STREAM_MANAGE_STRU;

typedef struct
{
	U8 currentSupportDeviceNo;
    U8 isQosTimerSet;
}APP_RHO_CONN_STRU;

typedef struct
{
	S8 rssi;
	BD_ADDR_T deviceAddr;
	BOOL isFoundRssi;
    U8 isShareMode;
}APP_RHO_MCSYNC_STRU;

#define IS_A2DP_TRANSFORM_EXIST     0x01
#define IS_DELAY_SUSPEND_TIMER_SET  0x02
#define IS_CHECK_SDPS_TIMER_SET     0x04
#define IS_LOW_LATENCY_SET          0x08
typedef struct
{
	U8 state;
    U8 bitMask;
}APP_RHO_A2DP_STRU;

typedef struct
{
	U8 advdata[31];
    U8 advsize;
    U8 randomAddr[6];

}APP_RHO_AIRAPP_STRU;

typedef struct
{
    BD_ADDR_T	bdAddr;
    U8			amaState;
    BOOL		isAVRCPOverride;
}APP_RHO_AMA_STRU;

typedef struct
{
    BOOL pauseFlag;
    BOOL isEnable;
}APP_RHO_INEARDETECTION_STRU;




typedef struct
{
    BD_ADDR_T testBd;
    APP_RHO_CTL_STRU appCtl;
    APP_RHO_INFO_STRU appLinkInfo;
    APP_RHO_POWEROFF_CTL_STRU appPowerOff;
    APP_RHO_PEQ_CTL_STRU appPEQ;
    APP_RHO_HFP_STRU appHfp;
    APP_RHO_STREAM_MANAGE_STRU appStreamManage;
    APP_RHO_CONN_STRU appConn;
    APP_RHO_MCSYNC_STRU appMCSync;
    APP_RHO_A2DP_STRU appA2dp;
    APP_RHO_AIRAPP_STRU appAirApp;
    APP_RHO_AMA_STRU appAma;
    APP_RHO_INEARDETECTION_STRU appInEarDection;
#ifdef XIAOAI_DEVELOPMENT
    APP_RHO_XIAOAI_STRU appXiaoai;
#endif
#ifdef OPPO_DEVELOPMENT
    APP_RHO_OPPO_STRU appOppo;
#endif
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE
	APP_RHO_TILE_STRU appTile;
#endif
>>>>>>> db20e11 (second commit)
}PACKED APP_RHO_DATA_STRU;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL APP_Lock(void);
BOOL APP_Unlock(void);
BOOL APP_IsLock(void);
BOOL APP_RecordMessage(U16 recordId, void *pRecordData, U16 length);
BOOL APP_IsAllow(void);
U16 APP_GetRhoDataLength(void);
BOOL APP_GetRhoData(U8 *pBuffer);
BOOL APP_ApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length);
BOOL APP_ApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length);

#endif //_APP_RHOHANDLER_H_
