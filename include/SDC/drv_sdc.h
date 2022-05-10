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

#pragma once

#include "types.h"

#define SDC_ERR_NoCard          1
#define RWStatus_Idle           0
#define RWStatus_Transfering    1

#define MASS_eMMC_TIMEOUT_IN_MS  200

#define eMMC_init_Fail         1
#define eMMC_init_OK           0

#define SDC_LOG_MSG_ID_INFO(_message, arg_cnt,...) printf(_message, ##__VA_ARGS__)

typedef enum
{
    eMMC4bit = 1,
    eMMC8bit,
    SD_Legacy,
    SD_20,
    SD_20_NormalCapacity,
    SD_20_HighCapacity,
}CardType;

typedef enum
{
    None,
    Initial,
    Ready,
    Transfering,
}CardSate;

typedef struct SDCStatus
{
    volatile U8 CardState;
    volatile U8 CardType;
    volatile U8 SupportCmd23;
    volatile U16 Card_RCA;
}SDCStatus_t;

typedef int (*pfTransferComplete)(void);

EXTERN volatile SDCStatus_t g_SDCStatus;


// standard API for future use
EXTERN U8 DRV_SDC_Init_SDCard();
EXTERN U8 DRV_SDC_Init_eMMC(U8 BitMode);
EXTERN U32 DRV_SDC_GetCurStatus();
EXTERN U32 DRV_SDC_GetCapacity();

// Please note SD    1.0 Raw_offset is the really offset.
//             SDHC      Raw_offset is block base
EXTERN U8 DRV_SDC_Read(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);
EXTERN U8 DRV_SDC_Write(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);

// Please note SD    1.0 Raw_offset is the really offset.
//             SDHC      Raw_offset is block base
EXTERN U8 DRV_SDC_ReadF(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);
EXTERN U8 DRV_SDC_WriteF(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);

EXTERN VOID DRV_SDC_Release();

EXTERN U8 DRV_MASS_eMMC_Init_Loop(void);

EXTERN VOID DRV_SDC_deInit(U8 path);

EXTERN VOID DRV_eMMC_deInit(U8 path);

EXTERN VOID DRV_SDC_ENABLE_CLOCK();

EXTERN VOID DRV_SDC_DISENABLE_CLOCK();

EXTERN U8 DRV_SDC_Read_Block(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);

EXTERN U8 DRV_SDC_Write_Block(U32 Raw_Offset, U8 *pBuffer, U32 DataLen);

#define DRV_SDC_MSG_CARD_REMOVE         MSG_MESSAGE_SDC_BASE + 0
#define DRV_SDC_MSG_CARD_INSERT         MSG_MESSAGE_SDC_BASE + 1
#define DRV_SDC_MSG_CARD_READY          MSG_MESSAGE_SDC_BASE + 2
#define DRV_SDC_MSG_TRANS_CMPL          MSG_MESSAGE_SDC_BASE + 3
#define DRV_SDC_MSG_INIT_RDY_CHK		MSG_MESSAGE_SDC_BASE + 4
#define DRV_SDC_MSG_REINIT				MSG_MESSAGE_SDC_BASE + 5

//#endif

