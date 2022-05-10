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
 
 
#ifndef	__APP_HFP_STATE_H__
#define __APP_HFP_STATE_H__

enum
{
	APP_CALL_EMPTY,
	APP_CALL_ACTIVE, //ON CALL
	APP_CALL_INCOMMING,
	APP_CALL_OUTGOING,
	APP_CALL_HELD,
	APP_CALL_STATE_NUM,
};

enum
{
	CALLHELD_NO_CALLS_HELD = 0,
	CALLHELD_CALLS_HELD_ACTIVE, //both active & held call
	CALLHELD_ON_HOLD_NOACTIVE, //only held call
};

enum
{
	CALL_NO_ACTIVE = 0,
	CALL_ACTIVE,
};

enum
{
	CALLSETUP_NO_CALSETUP = 0,
	CALLSETUP_INCOMMING,
	CALLSETUP_OUTGOING,
	CALLSETUP_REMOTE_ALERTED,
};

PUBLIC U8 		APP_HFP_SearchCallStatusInd(BD_ADDR_T *pBdAddr, U8 callStatus);
PUBLIC BOOL 	APP_HFP_TransitCallStatus(BD_ADDR_T *pBdAddr, U8 beforeStatus, U8 afterStatus);
U8 				APP_HFP_CountCallsInCallStatus(BD_ADDR_T *pBdAddr, U8 callStatus);
PUBLIC void 	APP_HFP_CallheldIndicationHandler_ForNoActiveCalls_HF(BD_ADDR_T *pBdAddr);
PUBLIC void 	APP_HFP_CallheldIndicationHandler_ForNoCallsHeld_HF(BD_ADDR_T *pBdAddr);
PUBLIC void 	APP_HFP_Init_HF(void);
BD_ADDR_T *		APP_HFP_SwitchLinkToNextHFPLink(BD_ADDR_T *pBdAddr);
void 			APP_HFP_SendEvtToStateHandle(BD_ADDR_T *pBdAddr, U16 evtCode);

#endif

