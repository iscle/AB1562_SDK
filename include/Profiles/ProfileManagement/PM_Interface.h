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

#ifndef _PROFILE_INTERFACE_H_
#define _PROFILE_INTERFACE_H_

#include "PM_ProfileId.h"
#include "PM_Reconnect.h"
#include "PM_Connection.h"
#include "PM_GapHandler.h"
#include "PM_LinkManager.h"
#include "PM_SniffHandler.h"
#include "PM_RhoHandler.h"

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	PM_REGISTER_APP_CFM = 0x0001,
	PM_CONNECT_CFM,
	PM_DISCONNECT_CFM,
	PM_REGISTER_ALL_PROFILE_CFM,
};

enum
{
	PM_REGISTER_GAP_CFM,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct PM_REGISTER_APP_CFM_T
{
	U8 status;
}PM_REGISTER_APP_CFM_T;

typedef struct PM_REGISTER_ALL_PROFILE_CFM_T
{
	U8 status;
}PM_REGISTER_ALL_PROFILE_CFM_T;

typedef struct PM_REGISTER_GAP_CFM_T
{
	U8 status;
}PM_REGISTER_GAP_CFM_T;

typedef struct PM_CONNECT_CFM_T
{
	BD_ADDR_T bdAddr;
	U8 profileID;
	U8 profileCntAll;
	U8 profileCntConnectedEvt;
}PM_CONNECT_CFM_T;

typedef struct PM_DISCONNECT_CFM_T
{
	BD_ADDR_T bdAddr;
	U8 profileID;
	U8 profileCntConnectedEvt;
	U8 isAlive;
    U8 status;
}PM_DISCONNECT_CFM_T;

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void PM_SetPowerOff(BOOL isOff);
BOOL PM_IsPoweringOff(void);
void PM_SetRejectConnectInd(BOOL isReject);
BOOL PM_IsRejectConnectInd(BD_ADDR_T *pBdAddr);
void PM_ResetTimer(void);

#endif //_PROFILE_INTERFACE_H_
