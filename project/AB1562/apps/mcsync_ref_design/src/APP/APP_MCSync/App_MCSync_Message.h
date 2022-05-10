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

#ifndef _APP_MCSYNC_MESSAGE_
#define _APP_MCSYNC_MESSAGE_

#include "BtMCSync.h"

/*************************************************************************************************
* Public function
**************************************************************************************************/
/* agent to client*/
void APP_MCSYNC_SendSyncA2dpInfo(BD_ADDR_T *pBdAddr, U8 specificRole);
void APP_MCSYNC_SendSyncScoInfo(BD_ADDR_T *pBdAddr);
void APP_MCSYNC_SendSyncStateInfo(BD_ADDR_T *pBdAddr);
void APP_MCSYNC_SendSyncSmartChargeCaseStateInfo(U8 state);
void APP_MCSYNC_SendSyncRhoStateInfo(U8 state);
void APP_MCSYNC_SendSyncVPInfo(BOOL isEnable);
void APP_MCSYNC_SendSyncANCPassThruInfo(U8 userRegister, U8 enable, U8 flash_no, U8 type, S16 runtime_gain, U32 clockToStart);
void APP_MCSYNC_SendSyncLEDInfo(U8 patternIndex, U8 *patternType, U8 overwriteTime, BOOL isOverwrite, U32 clockToStart, U8 action);
void APP_MCSYNC_SendSyncNumOfAGInfo(U8 numOfAG);
void APP_MCSYNC_SendSyncVPLangIndex(U8 index);
void APP_MCSYNC_SendSyncCallIdInfo(U8 *pCallerId, U8 length);
void APP_MCSYNC_SendPEQRealTimeInfo(U8 *pData, U16 dataLength, U32 clockToStart);
void APP_MCSYNC_SendSyncPowerOff(U8 reason, BOOL isFollowerIFPKT, BOOL isOnlyForFollower);
<<<<<<< HEAD
void APP_MCSYNC_SendSyncGFPInfo(U8 *pData, U32 length);
=======
#ifdef AIR_TILE_ADV_ENABLE
void APP_MCSYNC_SendSyncTileInfo(BOOL isAgent);
#endif
>>>>>>> db20e11 (second commit)
void APP_MCSYNC_SendSyncFCDReset(U8 reason);
void APP_MCSYNC_SendSyncSpeakerMute(BOOL isMute);

/* client to agent */
void APP_MCSYNC_SendSyncKeyInfo(U16 keyEventCode, U32 supportedState);
void APP_MCSYNC_SendSyncKeyCodeInfo(U16 keyCode);
void APP_MCSYNC_SendSyncBdAddrInfo(void);
void APP_MCSYNC_SendSyncRhoParaInfo(U16 batPercent, S8 rssi);
void APP_MCSYNC_SendSyncSmtChgCseInfo(BOOL isTimerSet);
void APP_MCSYNC_SendSyncPartnerInEarDetectionStatus(U8 status);

/* Both side */
void APP_MCSYNC_SendSyncVpRtInfo(U16 rtIndex, U16 vpIndex, U16 eventIndex, U32 clockToStart, U8 overwriteTime);
void APP_MCSYNC_SendSyncStopVpRtInfo(U16 eventIndex, U32 currentPicoClock);
void APP_MCSYNC_SendSyncVpRtPlayLater(U16 eventIndex, U32 clockToStart);

void APP_MCSYNC_SendReconnectMask(U8 notReconnMask);
void APP_MCSYNC_SendReconnectState(U8 type, BD_ADDR_T *pBdAddr);
void APP_MCSYNC_SendLinkList(U8 * pData, U32 length);

void APP_MCSYNC_SendSyncClearLinkKey(void);
void APP_MCSYNC_SendSyncGeneralData(U8 *pData, U32 length);
void APP_MCSYNC_SendSyncFollowerDisc(U8 isFollowerIFPkt, U8 isOnlyForFollower);
<<<<<<< HEAD

#ifdef MCSYNC_SHARE_MODE
=======
void APP_MCSYNC_SendNoticeMasterPartnerToShareMode(U8 enterShareMode);

#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_MCSYNC_SendSyncShareModeInfo(U8 state, U8 shareMode);
/*Share Mode*/
/* Follower to Agent */
void APP_MCSYNC_SendSyncFollowerSoundLevel(U8 soundLevel, U8 isFollowerIFPkt);

/* Agent to Partner*/
void APP_MCSYNC_SendSyncOtherAgentBDA(BD_ADDR_T *pBdAddr);

/* Partner to Agent*/
void APP_MCSYNC_SendExitRequest(void);
#endif

<<<<<<< HEAD
=======
#ifdef AIR_TAKE_OVER_LINK_ENABLE
void APP_MCSYNC_SendSyncTakeOverStateInfo(void);
#endif
>>>>>>> db20e11 (second commit)


#endif //_APP_AWSMCE_MESSAGE_

