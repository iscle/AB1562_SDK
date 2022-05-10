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

#ifndef _DRV_BATTERY_H_
#define _DRV_BATTERY_H_

#include "types.h"
#include "log.h"


//BatteryPara
#define MMI_IPHONE_BATTERY_LEVELS 9


#define VBAT_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(VBAT,fmt,cnt,##arg)
#define VBAT_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(VBAT,fmt,cnt,##arg)
#define VBAT_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(VBAT,fmt,cnt,##arg)

typedef U16 VoltageType;

typedef struct
{
    VoltageType lowBatLevel;
    VoltageType shutDownLowBat;
}LowBatteryType;

typedef struct
{
    VoltageType checkpoint1;
    VoltageType checkpoint2;
    VoltageType checkpoint3;
}BatteryCheckpoint;


////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
<<<<<<< HEAD
EXTERN VOID DRV_BAT_Init(VOID);
EXTERN U8 DRV_BAT_GetLevelInPercent(void);
EXTERN U8 DRV_BAT_IsBelowInitBat(void);
EXTERN U8 DRV_BAT_IsBelowLowBat(void);
EXTERN U8 DRV_BAT_IsBelowShutDownBat(void);
EXTERN U8 DRV_BAT_3v3_proc(void);
=======
EXTERN VOID pmu_bat_init(VOID);
EXTERN U32 pmu_bat_avg_adc(void);
EXTERN U8 pmu_bat_get_perc(void);
EXTERN U8 DRV_BAT_IsBelowInitBat(void);
EXTERN U8 DRV_BAT_IsBelowLowBat(void);
EXTERN U8 DRV_BAT_IsBelowShutDownBat(void);
EXTERN U16 pmu_bat_get_pure_adc(VOID);
EXTERN void pmu_bat_3v3_proc(void);
>>>>>>> db20e11 (second commit)

EXTERN U32 pmu_bat_volt_to_adc(U32 volt);
EXTERN U32 pmu_bat_adc_to_volt(U32 adc);
EXTERN U32 pmu_bat_volt_to_perc(U32 volt);
<<<<<<< HEAD
EXTERN U16 DRV_BAT_GetThousandPercent(void);
=======
EXTERN U16 pmu_bat_get_k_perc(void);
>>>>>>> db20e11 (second commit)

#endif /* _DRV_BATTERY_H_ */

