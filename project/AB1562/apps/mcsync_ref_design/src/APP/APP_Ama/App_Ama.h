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

#ifndef _APP_AMA_H_
#define _APP_AMA_H_

#define DBG_LOG_APP_AMA(_message, arg_cnt, ...)      LOG_MSGID_I(APP_AMA, _message, arg_cnt, ##__VA_ARGS__)

/**************************************************************************************************
* Structure
**************************************************************************************************/


typedef struct
{
	BD_ADDR_T	bdAddr;
	U8			amaState;
	BOOL		isAVRCPOverride;
}APP_AMA_CTL_STRU;

/**************************************************************************************************
* Public function
**************************************************************************************************/

void APP_Ama_RegisterProfile(void);
//BOOL APP_Ama_Enable(void);    /* control adv only in App_Ble.c */
//BOOL APP_Ama_Disable(void);   /* control adv only in App_Ble.c */
APP_AMA_CTL_STRU *APP_Ama_GetEmptyLinkInfo(void);
void APP_Ama_SetLinkInfo(BD_ADDR_T *pBdAddr, U8 state);
void APP_Ama_ClearLinkInfo(BD_ADDR_T *pBdAddr);
APP_AMA_CTL_STRU *APP_Ama_GetLinkInfo(void);
void APP_Ama_SetLinkAVRCPOverride(BD_ADDR_T *pBdAddr, BOOL isAVRCPOverride);
BOOL APP_Ama_GetLinkAVRCPOverride(void);
void APP_Ama_UpdateBdAddr(BD_ADDR_T *pBdAddr, BD_ADDR_T *pSwitchTransportBdAddr);
BOOL APP_Ama_Init(void);
void APP_Ama_IncomingCallHandler(U8 *pMsg, U8 length);
BOOL APP_Ama_IsAmaConnected(void);
void APP_Ama_GetData(void);
void APP_Ama_Disconnect(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* Public API (RHO)
**************************************************************************************************/
BOOL APP_Ama_FillRhoData(void *pData);
BOOL APP_Ama_AssignRhoData(void *pData);
void APP_Ama_NewPartner(BD_ADDR_T *pOldAgentAddr, BD_ADDR_T *pNewAgentAddr);

#endif /* _APP_AMA_H_ */

