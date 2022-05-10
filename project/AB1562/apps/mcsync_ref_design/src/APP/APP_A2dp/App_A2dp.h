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
 
 
#ifndef __APP_A2DP_H__
	#define __APP_A2DP_H__


/****************************************************************************************************
* Define
******************************************************************************************************/
#define MUSIC_EXISTS(A)							APP_a2dp_is_connected(A)

/****************************************************************************************************
* Enumeration
******************************************************************************************************/
enum
{
	APP_A2DP_DELAY_SUSPEND_TIMER
};

/****************************************************************************************************
* Public functions
******************************************************************************************************/
BOOL APP_a2dp_is_connected(BD_ADDR_T *pBdAddr);
BOOL APP_a2dp_is_in_streaming(BD_ADDR_T *pBdAddr);
void APP_A2dp_ClearResumeState(BD_ADDR_T *pBdAddr);
<<<<<<< HEAD
#ifdef AVRCP_Profile
=======
#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_A2dp_MonitorPlayStatus(BD_ADDR_T *pBdAddr, U8 isSuccessful);
void APP_A2dp_CheckPlayStatusConsistency(BD_ADDR_T *pBdAddr, U8 playStatus, U8 needDelay);
void APP_A2dp_PlayStatusChanged(BD_ADDR_T *pBdAddr, U8 playStatus);
void APP_A2dp_PlayStatusTheSame(BD_ADDR_T *pBdAddr, U8 playStatus);
#endif
void APP_A2dp_SendResume(BD_ADDR_T *pBdAddr);
void APP_A2dp_SendDSPEnableCmd(BD_ADDR_T *pBdAddr);
void APP_A2dp_StopDSP(BD_ADDR_T *pBdAddr);
void APP_A2dp_SendPauseMusic(BD_ADDR_T *pBdAddr, U8 isCall);
void APP_A2dp_ClearCtlData(BD_ADDR_T *pBdAddr);
Handler APP_A2DP_GetDspStartHandler(void);
void APP_A2dp_RegisterProfile(void);
void APP_A2dp_NotifyDSPStatus(BD_ADDR_T *pBdAddr, BOOL isEnable);
void APP_A2dp_Init(void);
void APP_A2dp_DspPlay(BD_ADDR_T* pBdAddr);
void APP_A2dpDspStop(BD_ADDR_T* pBdAddr);
void APP_A2dp_GameModeOn(void);
void APP_A2dp_GameModeOff(void);
void APP_A2dp_GameModeToggle(void);

BOOL APP_a2dp_fill_rho_data(void *pData);
BOOL APP_a2dp_assign_rho_data(void *pData);
U8 APP_A2dp_GetMsgNumInQueue(void);
void APP_A2dp_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr);

#endif
