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


#ifndef _APP_LINK_KEY_H_
#define _APP_LINK_KEY_H_

#include "bt.h"
#include "app_nvram_drivervariation_nvstruc.h"
#include "Peq_NvkeyHandler.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/


/**************************************************************************************************
* Define
**************************************************************************************************/
#define NVKEY_NORMAL_LINK_KEY_NO		10


#define NVKEY_LINK_KEY_MISC_MASK_CONNECTED				0x01
#define NVKEY_LINK_KEY_MISC_MASK_HFP					0x02
#define NVKEY_LINK_KEY_MISC_MASK_HSP					0x04
#ifdef HID_Profile
#define NVKEY_LINK_KEY_MISC_MASK_HID					0x10
#endif


/**************************************************************************************************
* Variable
**************************************************************************************************/


/**************************************************************************************************
* Structure
**************************************************************************************************/


/**************************************************************************************************
* Public Function
**************************************************************************************************/
U8 APP_LinkKey_LoadPeqIndex(U8 component);
void APP_LinkKey_SavePeqIndex(U8 component, U8 peqIndex);
void APP_LinkKey_LoadHistory(BD_ADDR_T *pBdAddr);
BD_ADDR_T *APP_LinkKey_GetNormalLinkBdAddr(U8 index);
BD_ADDR_T *APP_LinkKey_GetFcdBdAddr(void);
U8 APP_LinkKey_GetNormalPDLNumber(void);
BOOL APP_LinkKey_CheckKeyStatusOK(U8 i);
BOOL APP_LinkKey_CheckFCDKeyState(void);
void APP_LinkKey_ClearConnectedFlag(void);
void APP_LinkKey_ClearAllHistory(void);
void APP_LinkKey_DeleteLinkHistory(BD_ADDR_T *pbdAddr);
BOOL APP_LinkKey_IsBdAddrInHistory(BD_ADDR_T *pBdAddr);
BOOL APP_LinkKey_IsNormalLinkWatchdog(U8 index);
void APP_LinkKey_ResetLinkHistory(void);
void APP_LinkKey_SaveHistoryInRunTime(BD_ADDR_T *pbdAddr);
void APP_LinkKey_SaveHistoryWhenDisconnect(BD_ADDR_T *pbdAddr);
LINK_HISTORY_STRU *APP_LinkKey_SearchHistoryByBdAddr(BD_ADDR_T *pBdAddr, bool isNeedOk, bool isNeedShiftTop);
U8 APP_LinkKey_SearchLinkKeyIndex(BD_ADDR_T *pBdAddr, U8 needKeyOk);
void APP_LinkKey_SetLinkSoundLevelByHistory(BD_ADDR_T *pBdAddr);
U8 APP_LinkKey_ShiftLinkKeyToTop (U8 targetIndex);
void APP_LinkKey_UpdateHistory (BD_ADDR_T *pBdAddr, U8 isSuccessful);
void APP_LinkKey_Init(void);

#endif
