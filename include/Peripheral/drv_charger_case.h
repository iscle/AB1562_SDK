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

#ifndef _DRV_CHARGER_CASE_H_
#define _DRV_CHARGER_CASE_H_

#include "types.h"
#include "hal_platform.h"

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    U8 chgCaseEnable;
    U8 inputGPIO;
    U8 outputGPIO;
    BOOL mmiTimerEnable;
    U16 mmiTime;
}PACKED DRV_CHG_CASE_INFO_STRU, *PTR_DRV_CHG_CASE_INFO_STRU;


typedef struct
{
    U32 ioIndex;
    U32 status;
}DRV_CHG_CASE_GPIO_STRU, *PTR_DRV_CHG_CASE_GPIO_STRU;

typedef U32 IO_LATCH_BIT;
#define IO_LATCH_ALL_CLEAN          ((IO_LATCH_BIT)0)
#define IO_LATCH_CHARGER_CASE       ((IO_LATCH_BIT)1 << 0)

typedef union union_io_latch
{
    U32 value;
    struct stru_io_latch
    {
        U32 CHARGER_CASE   : 1;
        U32 RESERVED       : 1;
    } field;

} IO_LATCH_UNION;

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN IO_LATCH_BIT gIoLatchBitMap;



////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U32 IO_Latch_Status(VOID);
EXTERN VOID IO_Latch_SetBit(IO_LATCH_BIT SetBit);
EXTERN VOID IO_Latch_CleanBit(IO_LATCH_BIT CleanBit);
EXTERN VOID IO_Latch_Enable(VOID);

EXTERN U32 CHG_GPIO_OUTPUT_HIGH(hal_gpio_pin_t ioIndex);
EXTERN U32 CHG_GPIO_OUTPUT_LOW(hal_gpio_pin_t ioIndex_o, hal_gpio_pin_t ioIndex_i);
EXTERN U32 CHG_GPIO_INPUT_INITIAL(hal_gpio_pin_t ioIndex);
EXTERN U32 CHG_GPIO_INPUT_STATUS(hal_gpio_pin_t ioIndex);
EXTERN void CHG_GPIO_INT_Setting(U32 ioIndex);

EXTERN U32 DRV_ChgCase_GPIO_INT_Setting(U32 ioIndex);
EXTERN U32 DRV_ChgCase_GPIO_Init(U8 ioIndex, U8 ioDirection, U8 ioType);
EXTERN VOID DRV_ChgCase_GPIO_Output_Set(U8 ioIndex, U8 ioType);
EXTERN VOID DRV_ChgCase_MMI_Timer_KeepClk_LatchIO(VOID);

#endif /* _DRV_CHARGER_CASE_H_ */


