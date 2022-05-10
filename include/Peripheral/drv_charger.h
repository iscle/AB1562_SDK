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

#ifndef _DRV_CHARGER_H_
#define _DRV_CHARGER_H_

#include "types.h"
#include "os_memory.h"
#include "hal.h"
#include "log.h"
#include "hal_pmu_auxadc_2562.h"

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef U8 CHARGER_STATUS;
#define CHARGER_IDLE                ((CHARGER_STATUS)0)
#define CHARGER_TRICKLE_FIRST       ((CHARGER_STATUS)1)
#define CHARGER_CC_MODE1            ((CHARGER_STATUS)2)
#define CHARGER_CC_MODE2            ((CHARGER_STATUS)3)
#define CHARGER_CV_INITIAL          ((CHARGER_STATUS)4)
#define CHARGER_CV_MODE             ((CHARGER_STATUS)5)
#define CHARGER_COMPLETE            ((CHARGER_STATUS)6)
#define CHARGER_TRICKLE_RECHG       ((CHARGER_STATUS)7)

typedef U8 CHARGER_INT;
#define CHARGER_NONE_INT            ((CHARGER_INT)0)
#define CHARGER_PLUG_INT            ((CHARGER_INT)1)
#define CHARGER_UNPLUG_INT          ((CHARGER_INT)2)
#define CHARGER_COMPLETE_INT        ((CHARGER_INT)4)
#define CHARGER_RECHARGE            ((CHARGER_INT)8)

typedef U8 CHG_CASE_TYPE;
#define CHG_CASE_NONE               ((CHG_CASE_TYPE)0)
#define CHG_CASE_XY                 ((CHG_CASE_TYPE)1)
#define CHG_CASE_SMART_V1           ((CHG_CASE_TYPE)2)
#define CHG_CASE_SMART_V2           ((CHG_CASE_TYPE)3)

#define CHARGER_ENDISABLE           0x1000

#define DRV_CHARGER_MSG_CHARGERIN       MSG_MESSAGE_CHARGER_BASE + 0
#define DRV_CHARGER_MSG_CHARGEROUT      MSG_MESSAGE_CHARGER_BASE + 1
#define DRV_CHARGER_MSG_CHARGERCPL      MSG_MESSAGE_CHARGER_BASE + 2
#define DRV_CHARGER_MSG_RECHARGER       MSG_MESSAGE_CHARGER_BASE + 3
#define DRV_CHARGER_MSG_GPIO_Y          MSG_MESSAGE_CHARGER_BASE + 4
#define DRV_CHARGER_MSG_CHG_CASE_GPIO   MSG_MESSAGE_CHARGER_BASE + 4

#define DRV_CHARGER_RCHG_VALUE_MAX  255
#define DRV_CHARGER_RCHG_VALUE_MIN  0
#define DRV_CHARGER_DAC_VALUE_MAX   1023
#define DRV_CHARGER_DAC_VALUE_MIN   0

#define DRV_CHARGER_HIGH_BYTE_MASK  0xFF00
#define DRV_CHARGER_LOW_BYTE_MASK   0x00FF
#define DRV_CHARGER_MP3_SELECT_eMMC_SD     (0xF2D0)
#define DRV_CHARGER_USB_SELECT_eMMC     (0)
#define DRV_CHARGER_USB_SELECT_SD       (3)

#define NUM_OF_POINT 10


#define CHG_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(CHG,fmt,cnt,##arg)
#define CHG_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(CHG,fmt,cnt,##arg)
#define CHG_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(CHG,fmt,cnt,##arg)

enum
{
    INTERNAL_CHARGER_CC1_MODE,
    INTERNAL_CHARGER_CC2_MODE,
};

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum {
    CHARGER_NONE,
    CHARGER_EXIST,
};

typedef struct
{
    U16 currentValue;
    U16 rchgSelCCValue;
}PACKED NVKEY_CURRENT_CONFIG_RCHG_DATA;

typedef struct
{
    U16 currentValue;
    U16 currentDacValue;
}PACKED NVKEY_CURRENT_CONFIG_DAC_DATA;

typedef struct
{
    U8 calCount;
    U8 select;
    NVKEY_CURRENT_CONFIG_RCHG_DATA rchgData[NUM_OF_POINT];
}PACKED NVKEY_INTERNAL_CHARGER_CURRENT_CONFIG_CC1;

typedef struct
{
    U8 calCount;
    U8 select;
    NVKEY_CURRENT_CONFIG_RCHG_DATA rchgData[NUM_OF_POINT];
}PACKED NVKEY_INTERNAL_CHARGER_CURRENT_CONFIG_CC2;

typedef struct
{
    U8 enable;
    U8 type;
    U32 timeout_us;
    U16 adc_thrd;
    U8 chg_intr_filter_en;
    U8 reserved;
}PACKED SW_CHG_STRU;

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN U8 gChargerStatus;
EXTERN OSMQ OSMQ_CHG_INTR_Flag;
EXTERN SW_CHG_STRU gCHG_CTRL;


////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN BOOL DRV_CHARGER_IsPlugInState(VOID);
EXTERN BOOL DRV_CHARGER_IsCompleteState(VOID);
EXTERN U8 DRV_CHARGER_GetPlugStatus(VOID);
EXTERN U8 DRV_CHARGER_GetChargerOffStatus(VOID);
EXTERN VOID DRV_CHARGER_SetChargerOffEn(VOID);
EXTERN VOID DRV_CHARGER_SetChargerOffDis(VOID);

EXTERN VOID DRV_CHARGER_MsgHdlr(VOID);
EXTERN void DRV_CHARGER_IntrHdlr(uint16_t reg);
EXTERN VOID DRV_CHARGER_INT_init(VOID);
EXTERN void DRV_EOC_MsgHdlr(VOID);

EXTERN VOID DRV_CHARGER_ChkChgState(VOID);

EXTERN VOID DRV_PMU_EnableMMITimer(IN   U32 TimeInMs);

U8 DRV_CHARGER_InternalChargerConstCurrent1(U8 disCount);
U8 DRV_CHARGER_InternalChargerConstCurrent2(U8 disCount);

EXTERN U16 drv_auxadc_get_value(pmu_adc_channel_t ch);
<<<<<<< HEAD
EXTERN VOID drv_charger_pfm_config(U8 op);
=======
EXTERN VOID pmu_charger_pfm_config(U8 op);
>>>>>>> db20e11 (second commit)

#endif /* _DRV_CHARGER_H_ */
