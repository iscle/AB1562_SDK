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

#ifndef _PM_RHOHANDLER_H_
#define _PM_RHOHANDLER_H_

#include "PM_SniffHandler.h"

/**************************************************************************************************
* Definition
**************************************************************************************************/
#define PM_MSG_ID_SINK_FLUSH 0xFFFE
#define PM_RESERVED_FOR_PNP     20
#define PM_RESERVED_FOR_PRODUCT_INFO     40

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	BD_ADDR_T bdAddr;
	U16 dataLength;
	U8 pData[1];
}PACKED PM_RHO_SINK_FLUSH_STRU;

typedef struct
{
	U8 state:7;
	U8 isAlive:1;
}PACKED PM_RHO_DATA_PROFILE_INFO_STRU;

typedef struct
{
	BD_ADDR_T testBd; //test
	PM_RHO_DATA_PROFILE_INFO_STRU profileInfo;
	U8 trafficBusy;
	U8 aclStatus;
	PM_SNIFF_INFO_STRU sniffInfo;
	U8 isPowerOff;
	U8 isSniffDelayTimerSet;
	U8 isSniffCheckAGTimerSet;
	U8 isAvrcpWorkAroundTimerSet;
	U8 isDisconnectTimerSet;
	U8 isReconnectAVRCPTimerSet;
	U8 isReconnectA2DPTimerSet;
	U8 isReconnectHFPTimerSet;
	U8 isReconnectHSPTimerSet;
	U8 pnpData[PM_RESERVED_FOR_PNP];
    U8 productInfo[PM_RESERVED_FOR_PRODUCT_INFO];
}PACKED PM_RHO_DATA_STRU;

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
BOOL PM_Lock(void);
BOOL PM_Unlock(void);
BOOL PM_IsLock(void);
BOOL PM_RecordMessage(U16 recordId, void *pRecordData, U16 length);
BOOL PM_IsAllow(void);
U16 PM_GetRhoDataLength(void);
BOOL PM_GetRhoData(U8 *pBuffer);
BOOL PM_ApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pPMData, U16 length);
BOOL PM_ApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pPMData, U16 length);

#endif //_PM_RHOHANDLER_H_
