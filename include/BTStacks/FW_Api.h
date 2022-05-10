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

#ifndef _GENERAL_API_H_
#define _GENERAL_API_H_
<<<<<<< HEAD
#include "bt.h"
=======
#include "bt_gap.h"
#include "message.h"
>>>>>>> db20e11 (second commit)
#include "nvkey.h"
#include "nvkey_list.h"
#include "stdlib.h"
#include "assert.h"

/**************************************************************************************************
* Typedef
**************************************************************************************************/

typedef void (*FW_CALL_BACK_FUNC)(void* Para);
typedef struct fw_stru_os_dpc
{
    /*! callback function */
    FW_CALL_BACK_FUNC fCallBack;

    /*! length of parameter */
    U32 Length;

    /*! dpc status */
    U32 dpcStatus;

    /*! freeRTOS timer handle */
    void * timerHandle;

} FW_OS_DPC_STRU, * FW_OS_DPC_STRU_PTR;


typedef struct fw_stru_os_dpc_block
{
    /*! dpc handle */
    FW_OS_DPC_STRU_PTR dpcHandle;

    /*! dpc special key */
    U32 dpcKey;

} FW_OS_DPC_BLOCK_STRU, * FW_OS_DPC_BLOCK_STRU_PTR;

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SUCCESS,
	NO_SUCCESS,
	NO_TYPE,
	NO_DATA,
	NO_MEMORY,
	NO_DATA_IN_QUEUE,
};

enum
{
	QUEUE_TYPE_AVCTP,
	QUEUE_TYPE_AVDTP,
	QUEUE_TYPE_A2DP,
	QUEUE_TYPE_AWSMCE,
	QUEUE_TYPE_AVRCP,
	QUEUE_TYPE_HFP,
	QUEUE_TYPE_PM,
	QUEUE_TYPE_APP,
	QUEUE_TYPE_AWSMCE_SWITCH_AGENT_ROLE,
	QUEUE_TYPE_AMA,
	QUEUE_TYPE_IAP2,
    QUEUE_TYPE_L2CAP,
	QUEUE_TYPE_NUM,
};

/**************************************************************************************************
* Define
**************************************************************************************************/
#define BIT_MASK(n)         (1UL << (n))
#define BIT_MASK32(n)       ((U32)1 << (n))
#define BIT_MASK16(n)       ((U16)1 << (n))
#define BIT_MASK8(n)        ((U8)1 << (n))
#define BIT_FIELD(n,p)      ((n) << (p))
#define U32_BIT_MASK(n)     BIT_MASK32(n)
#define U16_BIT_MASK(n)     BIT_MASK16(n)
#define U8_BIT_MASK(n)      BIT_MASK8(n)

#define MAX_LINK_IDX 		4
#define FW_MAX_LINK_IDX 	3
#define FW_AMA_MAX_LINK_IDX 1

#define INVAILD_CLAIM_SIZE 	0xFFFFFFFF
#define INVALID_NO_SINK 	0x000000FF

#define ENDIAN_TRANSFORM_U16(A)	((((A) & 0x00FF) << 8) + (((A) & 0xFF00) >> 8))
#define ENDIAN_TRANSFORM_U32(xx) ((xx << 24) | ((xx << 8) & 0X00FF0000) | ((xx >> 8) & 0X0000FF00) | ((xx >> 24) & 0X000000FF))
#define FW_GetMemory malloc
#define FW_Assert assert

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
U32 FW_GetMaximum(U32 var1, U32 var2);
U32 FW_GetMinimum(U32 var1, U32 var2);
//void FW_Assert(BOOL expr);
BOOL FW_CmpBdAddr(BD_ADDR_T *pBdAddr1, BD_ADDR_T *pBdAddr2);
BOOL FW_Memcmp(void *ptr1, void *ptr2, U32 num);
void FW_Memcpy(void *pDestination, CONST void *pSource, U32 num);
void FW_FreeMemory(void *pMem);
void FW_Memset(void *pDestination, S32 value, U32 num);
U8 FW_number_of_ones(U8 n);
U32 FW_bdaddr_to_2U32(BD_ADDR_T *pBdAddr, BOOL isformer);
BOOL FW_IsBdAddrZero(BD_ADDR_T *pBdAddr);
U32 FW_GetRandomNum(U32 min, U32 max, U32 diff, U32 seed);
U32 FW_GetSysClk(void);

/**************************************************************************************************
* Public Functions (Queue)
**************************************************************************************************/
U8 FW_Queue_Push(void *dataPtr, U8 type);
void *FW_Queue_Pop(U8 type);
U8 FW_Get_Queue_Num(U8 type);
U8 FW_Queue_Push_Front(void *dataPtr, U8 type);
U8 FW_Queue_TypeClear(U8 type);
U8 FW_Queue_AllClear(void);

/**************************************************************************************************
* Public Functions (Timer)
**************************************************************************************************/
void FW_SetTimer(Handler handler, U16 timerId, void *msg, U32 linkAddr, U32 delay); /*ms*/
void FW_ReleaseTimer(Handler handler, U16 timerId, U32 linkAddr);
void FW_ReleaseTimerByHandler(Handler handler);
void FW_ReleaseTimerByLinkAddr(Handler handler, U32 linkAddr);
U16 FW_CheckTimer(Handler handler, U16 timerId, U32 linkAddr);
U16 FW_CheckTimerByLinkAddr(Handler handler, U32 linkAddr);
void FW_MessageSend(Handler handler, U16 id, void *message);
void FW_MessageSendEx(Handler handler, U16 id, void *message, U32 id_ext);
void FW_SetDPCTimer(FW_OS_DPC_BLOCK_STRU_PTR pDpcBlk, FW_CALL_BACK_FUNC fCallBack, U32 delayLength);
void FW_SetDPCTimerWithData(FW_OS_DPC_BLOCK_STRU_PTR pDpcBlk, FW_CALL_BACK_FUNC fCallBack, U32 delayLength, void *pData, U16 dataLen);
void FW_ReleaseDPCTimer(FW_OS_DPC_BLOCK_STRU_PTR pDpcBlk);

/****************************************************************************************************
* Public (Audio Mask)
*****************************************************************************************************/
BOOL FW_AudioIsDspOpen(void);
BOOL FW_AudioIsBtThroughputBusy(void);

/****************************************************************************************************
* Public special function
*****************************************************************************************************/
void FW_MakeSpecialBDA(BD_ADDR_T *pBdAddr);

#endif //_GENERAL_API_H_
