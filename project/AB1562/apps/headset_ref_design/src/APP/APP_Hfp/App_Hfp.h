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
 
 
#ifndef _APP_HFP_H_
#define _APP_HFP_H_


#define DBG_LOG_APP_HFP(_message, arg_cnt, ...)  LOG_MSGID_I(APP_HFP, _message, arg_cnt, ##__VA_ARGS__)

//**************************************************************************************************
//* Public Functions
//**************************************************************************************************
void APP_Hfp_StartService(U8 profileBit);

//**************************************************************************************************
//* Public Functions (Connection)
//**************************************************************************************************
void APP_Hfp_regVendorAT();
void APP_Hfp_RegisterProfile();
BOOL APP_Hfp_IsConnected(BD_ADDR_T *pBdAddr);//MMI_HFP_IsConnected

//**************************************************************************************************
//* Public Functions (battery send)
//**************************************************************************************************
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
BOOL APP_Hfp_IsHspConnected(BD_ADDR_T * pBdAddr);
#endif
BOOL APP_Hfp_IsFeatureOn(U32 feature);

PUBLIC BD_ADDR_T *APP_Hfp_GetNextHfpLink(BD_ADDR_T * pBdAddr);
//**************************************************************************************************
//* Public Functions (APP_HFP_SendATCmdByLink)
//**************************************************************************************************
BOOL APP_SendATCmdUnderConnection(BD_ADDR_T *pBdAddr, U8 opcode);
void APP_HFP_SendATCmdByLink(BD_ADDR_T *pBdAddr, U8 opcode);
void APP_HFP_StopIncomingVPRing(BD_ADDR_T *pBdAddr);
void APP_HFP_GetCallSetupRemoteAlert(BD_ADDR_T *pBdAddr, BOOL isUnderCallActive);
void APP_HFP_ScoConnOKHandler(BD_ADDR_T *pBdAddr);
void APP_HFP_ScoConnFailHandler(BD_ADDR_T *pBdAddr);
void APP_HFP_ScoDiscHandler(BD_ADDR_T *pBdAddr);
void APP_HFP_CreateAudioConnection(BD_ADDR_T *pBdAddr, BOOL isViaKey);
void APP_HFP_SetTimer(BD_ADDR_T *pBdAddr, U16 id, U32 timeValue);
void APP_HFP_ReleaseTimer(BD_ADDR_T *pBdAddr, U16 id);
void APP_HFP_ReleaseAllTimer(BD_ADDR_T *pBdAddr);
U16 APP_HFP_CheckTimer(BD_ADDR_T *pBdAddr, U16 id);
U8 APP_Hfp_GetSupportCodec(BD_ADDR_T * pBdAddr);
BOOL APP_Hfp_IsCodecWideBand(U8 codec);
BOOL APP_Hfp_IsAGSiriSupported(void);

#endif //_APP_HFP_H_
