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
#include "BtMCSync.h"

/**************************************************************************************************
* Enumeration
**************************************************************************************************/


/**************************************************************************************************
 * Structure
**************************************************************************************************/


/**************************************************************************************************
* Public Function
**************************************************************************************************/
BOOL APP_Customer_PartnerSyncAirohaKey(AWSMCE_SYNC_KEY_ACTION_IND_T *ind);
BOOL APP_Customer_IsInEarEnable(void);
void APP_Customer_SetInEarEnableFlag(U8 isEnable);
void APP_Customer_Timer2_Control(void);
void APP_Customer_Timer6_Control(void);
U8 APP_Customer_UpdateBatStatus_Adv(U8 batStatus);
void APP_Customer_SetConnected_Control(void);
void APP_Customer_SetCloseAdvTimer(void);
void APP_Customer_SendSyncGeneralData(void);
U8 APP_Customer_Sync_Parnter_Data(U8* pValue);
void APP_Customer_Rho_Done(void);
void APP_Customer_PowerOff_Handler(void);
U32 APP_Customer_LidOnBatteryHanler(U32 ext_id);
void APP_Customer_KeyIndex0Handler(void);
void APP_Customer_KeyIndex1Handler(void);
BOOL APP_Customer_SmartKeyHandler(void);
void APP_Customer_SmartUsr1IndHandler(void);
void APP_Customer_MCSyncConntingHandler(void);
U8 APP_Customer_SetCloseAdv(void);
void APP_Customer_SetInEarTone(U8 status);
void app_Customer_SetServiceSecurity(void);
void APP_Customer_ConnectionNoiseControl(void);
BOOL APP_Customer_IsInEarDetectionEnable(void);
BOOL APP_Customer_IsVDHandledByGattVoice(BD_ADDR_T * pBdAddr, U8 action);
void APP_Customer_SendSyncInfo(void);
void app_Customer_AirohaKey(BD_ADDR_T * pBdAddr);
BOOL APP_Customer_HandleSetPublicAddrCfm(void);

