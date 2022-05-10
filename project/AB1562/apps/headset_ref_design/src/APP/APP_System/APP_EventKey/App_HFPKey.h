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
 
 
#ifndef __APP_HFPKEY_HH_
#define __APP_HFPKEY_HH_

U16 APP_HFP_CheckValidKey(U8 index, U16 keyCode);
void APP_HFP_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U16 keyCode);
void APP_HFP_KeyAudioTransfer(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyRedial(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyVoiceRecognition(U16 opcode);
void APP_HFP_KeyAppleSiri(U16 opcode);
#ifdef HFP_AT_ATD_NUMBER
void APP_HFP_KeyDialMemory(BD_ADDR_T *pBdAddr);
#endif
#ifdef HFP_AT_ATD_DIGIT
void APP_HFP_KeyDialNumber(BD_ADDR_T *pBdAddr);
#endif
void APP_HFP_KeyCancelOutGoingCall(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyRejectCall(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyRejectCallSecondPhone(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyOnHoldCall(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyAcceptCall(BD_ADDR_T *pBdAddr);
void APP_HFP_KeyEndCall(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayRelNUDB(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayRelNAcp(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayRelNAcp_X(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayHoldNAcp(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayHoldNAcp_X(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayAdd(BD_ADDR_T *pBdAddr);
void APP_HFP_Key3WayCallTransfer(BD_ADDR_T *pBdAddr);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
void APP_HFP_KeyHSPButtonPressed(BD_ADDR_T *pBdAddr);
#endif
void APP_HFP_KeySetNREC(BD_ADDR_T *pBdAddr);
#ifdef HFP_AT_CNUM
void APP_HFP_KeyGetCallNum(BD_ADDR_T *pBdAddr);
#endif
#ifdef HFP_AT_CLCC
void APP_HFP_KeyQueryCallList(BD_ADDR_T *pBdAddr);
#endif
#ifdef HFP_AT_VTS
void APP_HFP_KeySendDTMF(BD_ADDR_T *pBdAddr);
#endif
#ifdef HFP_AT_BINP
void APP_HFP_KeyAttachLastVoiceTagRecord(BD_ADDR_T *pBdAddr);
#endif
void APP_HFP_KeySendCLIP(BD_ADDR_T *pBdAddr);

#endif //__APP_HFPKEY_HH_

