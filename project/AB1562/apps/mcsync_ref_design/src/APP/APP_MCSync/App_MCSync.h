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


#ifndef _APP_AWS_MCE_H_
#define _APP_AWS_MCE_H_


#if 0 /* Unused LOG_PRINT_156X */
#define DBG_LOG_APP_MCSYNC(_message, arg_cnt, ...)
#else
#define DBG_LOG_APP_MCSYNC(_message, arg_cnt, ...)   LOG_MSGID_I(APP_MCSYNC, _message, arg_cnt, ##__VA_ARGS__)
#endif


/**************************************************************************************************
* Enumeration
**************************************************************************************************/
enum
{
	APP_MCS_TIMER_ID_CHECK_AUTO_DISCOVERABLE_WHEN_PARTNER_CONNECTED,
	APP_MCS_TIMER_AIR_PAIRING,
	APP_MCS_TIMER_CHECK_RHO,
<<<<<<< HEAD
    APP_MCS_TIMER_SYNC_BATT,
=======
>>>>>>> db20e11 (second commit)
	APP_MCS_TIMER_A2DP_RETRIGGER,
};

/**************************************************************************************************
* Structure
***************************************************************************************************/
typedef struct
{
	BD_ADDR_T deviceAddr;
	S8 rssi;
	U8 isFoundRssi:1;
	U8 reserved:7;
}APP_MCSYNC_INQUIRY_STRU;

typedef struct
{
	APP_MCSYNC_INQUIRY_STRU 	inquiryPara;
	U8 reasonForDisconnect;
<<<<<<< HEAD
=======
	#ifdef AIR_TAKE_OVER_LINK_ENABLE 
	U8 partnerTakeOverState;
	#endif
>>>>>>> db20e11 (second commit)
}APP_MCSYNC_STRU;

typedef struct
{
	U8 index;
	U32 picoClock;
}APP_MCSYNC_VPRT_LOG_RECORD_STRU;

/**************************************************************************************************
* Enum
***************************************************************************************************/

/**************************************************************************************************
* Public function
**************************************************************************************************/
void APP_AWSMCE_RegisterProfile(void);
BOOL APP_AWSMCE_KeyEvent(U8 keyIndex, U8 keyAction);
U8 APP_AWSMCE_GetMsgNumInQueue(void);

BOOL APP_AWSMCE_FillRhoData(void *pData);
BOOL APP_AWSMCE_AssignRhoData(void *pData);
BD_ADDR_T* APP_AWSMCE_GetBestRSSIAddr(void);
void APP_AWSMCE_SetRSSIInfo(S8 rssi, BD_ADDR_T* pBdAddr);
BOOL APP_AWSMCE_DecideRole(U8* pTargetBdAddr);

void app_AwsMce_SendSyncAllInfo(BD_ADDR_T* pBdAddr);
BOOL APP_AWSMCE_KeyCode(U16 keyCode);
void APP_AWSMCE_SetReasonForDisconnect(U8 reason);
U8 APP_AWSMCE_GetReasonForDisconnect(void);
void APP_AWSMCE_SetTimer(U8 type, U32 time);
void APP_AWSMCE_ReleaseTimer(U8 type);
void APP_MCSync_SendAncInfo(U32 clockToStart, U8 userRegister);
U16 APP_AWSMCE_CheckTimer(U8 type);

/**************************************************************************************************
* Public function (MCSync Data)
**************************************************************************************************/
void APP_MCSync_SyncAgentStateIndHandler(U8 agentState);
void APP_MCSync_SyncRhoStateIndHandler(U8 state);
void APP_MCSync_SyncAgentVPIsEnableIndHandler(BOOL isEnable);
void APP_MCSync_SyncAgentANCPassThruIndHandler(U8 userRegister, U8 enable, U8 flash_no, U8 type, S16 runtime_gain);
void APP_MCSync_SyncLEDIndHandler(U8 patternIndex, U8* patternType, U8 overwriteTime, BOOL isOverwrite, U32 clockToStart, U8 action);
void APP_MCSync_SyncSmartChargeCaseStateIndHandler(U8 smartChargeCaseState);
void APP_MCSync_SyncPartnerKeyIndHandler(U16 keyEventCode, U32 supportedState);
void APP_MCSync_SyncRhoParaInfoIndHandler(U16 batPercent, S8 rssi);
void APP_MCSync_SyncSmtChgCseInfoIndHandler(BOOL isTimerSet);
void APP_MCSync_InEarDetectionStatusIndHandler(U8 status);
void APP_MCSync_SyncPowerOffIndHandler(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower);
void APP_MCSync_LinkListIndHandler(U8 * pData, U32 length);
void APP_MCSync_ReconnectStateIndHandler(U8 type, BD_ADDR_T *pBdAddr);
void APP_MCSync_ReconnectMaskIndHandler(U8 notReconnMask);
void APP_MCSync_SyncKeyCodeInfoIndHandler(U16 keyCode);
<<<<<<< HEAD
void APP_MCSync_SyncGFPInfoIndHandler(void *pData, U32 length);
=======
#ifdef AIR_TILE_ADV_ENABLE
void APP_MCSync_SyncTileInfoIndHandler(U8 opcode, U8 streamState);
#endif
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Public function (MCSync Init)
**************************************************************************************************/
void APP_MCSync_Init(void);



#endif
