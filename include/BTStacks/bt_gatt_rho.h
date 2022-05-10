/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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

#ifndef _BT_GATT_RHO_HANDLER_H_
#define _BT_GATT_RHO_HANDLER_H_

typedef struct
{
    const char *moduleNameString;    //must be a string with LOG_STRING_NEW attribute
    BOOL (*funcLock)(void);
    BOOL (*funcUnlock)(void);
    BOOL (*funcIsAllow)(void);
    U16  (*funcGetLength)(void);
    BOOL (*funcGetData)(U8 *pBuffer);
    BOOL (*funcApplyAgentData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
    BOOL (*funcApplyPartnerData)(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
}GATT_MCSYNC_RHO_MODULE_CTL_STRU;


BOOL BtGattRegisterRhoModule(GATT_MCSYNC_RHO_MODULE_CTL_STRU * pModule);

/**************************************************************************************************
* public function prototype for MCSYNC RHO
**************************************************************************************************/

BOOL BtGattLock(void);
BOOL BtGattUnlock(void);
BOOL BtGattIsAllow(void);
U16  BtGattGetRhoDataLength(void);
BOOL BtGattGetRhoData(U8 *pBuffer);
BOOL BtGattApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
BOOL BtGattApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);


#endif  //_BT_GATT_RHO_HANDLER_H_
