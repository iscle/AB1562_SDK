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


#ifndef _BTHFP_HH_
#define _BTHFP_HH_

void HFP_StartService(Handler app, U8 profileBit);
void HFP_StopService(Handler app, U8 profileBit);
BOOL HFP_ConnectRequest(BD_ADDR_T *pBdAddr);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
BOOL HSP_ConnectRequest(BD_ADDR_T *pBdAddr);
#endif
BOOL HFP_ConnectResponse(BD_ADDR_T *pBdAddr, BOOL isAccept);
BOOL HFP_DisconnectRequest(BD_ADDR_T *pBdAddr);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
void HFP_Send_CKPD200(BD_ADDR_T *pBdAddr);
#endif
void HFP_Send_ATA(BD_ADDR_T *pBdAddr);
void HFP_SetRemoteScn(BD_ADDR_T *pBdAddr, BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM_T *cfm);
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
void HSP_SetRemoteScn(BD_ADDR_T *pBdAddr, BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM_T *cfm);
#endif
U8 HFP_GetProfileState(BD_ADDR_T *pBdAddr);

void HFP_Send_CHUP(BD_ADDR_T *pBdAddr);

void HFP_Send_CLIP(BD_ADDR_T *pBdAddr,U8 isEnable);

void HFP_Send_ECNR(BD_ADDR_T *pBdAddr,BOOL isEnable);
#ifdef HFP_AT_CLCC
void HFP_Send_CLCC(BD_ADDR_T * pBdAddr);
#endif
#ifdef HFP_AT_BINP
void HFP_Send_BINP(BD_ADDR_T *pBdAddr);
#endif
#ifdef HFP_AT_CNUM
void HFP_Send_CNUM(BD_ADDR_T *pBdAddr);
#endif
void HFP_Send_BCC(BD_ADDR_T * pBdAddr);
void HFP_Send_CHLD(BD_ADDR_T * pBdAddr, U8 chldcode);
void HFP_Send_BVRA(BD_ADDR_T * pBdAddr, BOOL isEnable);
void HFP_Send_BLDN(BD_ADDR_T * pBdAddr);
void HFP_Send_VGS(BD_ADDR_T * pBdAddr, U8 vgs);
void HFP_Send_VGM(BD_ADDR_T * pBdAddr, U8 vgm);
void HFP_Send_XAPL(BD_ADDR_T * pBdAddr);
void HFP_Send_BAC(BD_ADDR_T * pBdAddr);
void HFP_Send_BCS(BD_ADDR_T * pBdAddr);
void HFP_Send_APLSIRI(BD_ADDR_T * pBdAddr);
void HFP_Send_XEVENTBatInquiry(BD_ADDR_T * pBdAddr);
void HFP_Send_ATDDIGIT(BD_ADDR_T *pBdAddr, U8 * pDigit, U8 length);
#ifdef HFP_AT_ATD_NUMBER
void HFP_Send_ATDNUMBER(BD_ADDR_T *pBdAddr, U16 index);
#endif
#ifdef HFP_AT_BTRH
void HFP_Send_BTRH(BD_ADDR_T * pBdAddr,BOOL isAccept);
#endif
#ifdef HFP_AT_VTS
void HFP_Send_VTS(BD_ADDR_T * pBdAddr, U8 DTFM);
#endif
void HFP_Send_APLEFM(BD_ADDR_T * pBdAddr, BOOL isEnable);
#ifdef HFP_AT_COPS
void HFP_Send_COPS(BD_ADDR_T * pBdAddr);
#endif
#ifdef HFP_AT_COPS_QUES
void HFP_Send_COPSQues(BD_ADDR_T * pBdAddr);
#endif

void HFP_Send_VendorAT(BD_ADDR_T * pBdAddr, U8 atno);
#ifdef XIAOAI_DEVELOPMENT
void HFP_Send_XiaomiStatus(BD_ADDR_T * pBdAddr, U8* ptr, U16 len);
#endif 

void HFP_SetInitVolume(BD_ADDR_T * pBdAddr, U8 vgs, U8 vgm);





BOOL HFP_IsConnected(BD_ADDR_T *pBdAddr);
BOOL HFP_IsDisconnected(BD_ADDR_T *pBdAddr);

BOOL HFP_IsSCOCodecMSBC(BD_ADDR_T * pBdAddr);
SINK HFP_GetSinkByBdAddr(BD_ADDR_T * pBdAddr);

U16 HFP_GetBRSFFeature(BD_ADDR_T * pBdAddr);
U8 HFP_GetSupportCodec(BD_ADDR_T * pBdAddr);
void HFP_SniffHandler(BD_ADDR_T * pBdAddr, U8 opCode);
BOOL HFP_IsSupportVoiceRecognition(BD_ADDR_T * pBdAddr);
BOOL HFP_IsSupportCodecNegoFeature(BD_ADDR_T * pBdAddr);
BOOL HFP_IsSupport3WayCalling(BD_ADDR_T * pBdAddr);

/**************************************************************************************************
* Public functions (RHO)
**************************************************************************************************/
BOOL BtHfpLock(void);
BOOL BtHfpUnlock(void);
BOOL BtHfpIsAllow(void);
U16 BtHfpGetRhoDataLength(void);
BOOL BtHfpGetRhoData(U8 *pBuffer);
BOOL BtHfpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pHfpData, U16 length);
BOOL BtHfpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pHfpData, U16 length);
void BtHfpResetTimer(void);
void BtHfpCancelAllTimer(void);

#endif
