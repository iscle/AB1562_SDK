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


#ifndef _APP_MCSYNC_RHO_H_
#define _APP_MCSYNC_RHO_H_

#include "MCSync_DataHandler.h"

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_MCSync_Rho_Init(void);
BOOL APP_MCSync_Rho_Start(void);
void APP_MCSync_Rho_AirAppDisconnectNotify(void);
void APP_MCSync_Rho_BleDisconnectNotify(U8 status);
void APP_MCSync_Rho_CustAppDisconnectNotify(void);
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
=======
#ifdef AIR_GFP_ENABLE
>>>>>>> db20e11 (second commit)
void APP_MCSync_Rho_GfpSppDisconnectNotify(void);
#endif
void APP_MCSync_Rho_PacketHandler(MCSYNC_RHO_PACKET_T *pRxPacket, U16 length);
void APP_MCSync_Rho_RoleChangeIndHandler(U8 newRole, BOOL status);
void APP_MCSync_Rho_ModeChangeIndHandler(BD_ADDR_T *pBdAddr, U8 mode);
void APP_MCSync_Rho_NonConnectableNotify(void);
BOOL APP_MCSync_Rho_IsRhoing(void);
BOOL APP_MCSync_RHO_IsReceiveRHOPkt(void);
BOOL APP_MCSync_Rho_IsAbleToStartRho(void);
void APP_MCSync_Rho_SetRhoState(U8 state);
void APP_MCSync_Rho_StatusChangeIndHandler(U8 role);
BOOL APP_MCSync_Rho_Stop(void);
<<<<<<< HEAD
=======
#ifdef AIR_TILE_ADV_ENABLE	
void app_MCSync_Rho_PartnerBleDisconnectNotify(U8 status); //add for tile
#endif
>>>>>>> db20e11 (second commit)


#endif //_APP_MCSYNC_RHO_H_
