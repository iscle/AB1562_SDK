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

#ifndef _PWR_MGR_H_
#define _PWR_MGR_H_

#include "types.h"
#include "log.h"


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define PWR_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(PWR,fmt,cnt,##arg)
#define PWR_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(PWR,fmt,cnt,##arg)
#define PWR_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(PWR,fmt,cnt,##arg)

typedef U32 DRV_PWR_CTRL_MASK;
#define DRV_PWR_CTRL_MASK_NONE     ((DRV_PWR_CTRL_MASK)0)
#define DRV_PWR_CTRL_MASK_ANC      ((DRV_PWR_CTRL_MASK)1 << 0)
#define DRV_PWR_CTRL_MASK_EMMC     ((DRV_PWR_CTRL_MASK)1 << 1)
#define DRV_PWR_CTRL_MASK_SDC      ((DRV_PWR_CTRL_MASK)1 << 2)
#define DRV_PWR_CTRL_MASK_VAD      ((DRV_PWR_CTRL_MASK)1 << 3)
#define DRV_PWR_CTRL_MASK_CHG      ((DRV_PWR_CTRL_MASK)1 << 4)
#define DRV_PWR_CTRL_MASK_ALL      ((DRV_PWR_CTRL_MASK)(DRV_PWR_CTRL_MASK_ANC | DRV_PWR_CTRL_MASK_EMMC | DRV_PWR_CTRL_MASK_SDC | DRV_PWR_CTRL_MASK_VAD | DRV_PWR_CTRL_MASK_CHG))

////////////////////////////////////////////////////////////////////////////////
// Type Defintions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum
{
    PWR_PLAN_USER_DEF = 0,
    PWR_PLAN_ORG,
    PWR_PLAN_NEW,
};

enum
{
    BUCK_SETTING_DISABLE = 0,
    BUCK_SETTING_ENABLE,
};

enum
{
    SLEEP_PWR_SETTING_DISABLE = 0,
    SLEEP_PWR_SETTING_ENABLE,
};

enum
{
    TOUCH_FEATURE_DISABLE = 0,
    TOUCH_FEATURE_ENABLE,
};

enum
{
    NVKEY_DISABLE = 0,
    NVKEY_ENABLE,
};

enum
{
    BuckMV_NONE = 0,
    BuckMV_ANC = 1,
    BuckMV_EMMC = 2,
    BuckMV_Flash = 4,
};

enum
{
    BuckMv_16,
    BuckMv_18,
    BuckMv_RESERVED,
};

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF00C
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_SYS_BUCK_SETTING*/
typedef struct
{
    U8 enable;
    U8 Buck_LV_freq;
    U8 Buck_LV_thrd;
    U8 Buck_MV_freq;
    U8 Buck_MV_thrd;
    U8 Inductor_type;
}PACKED BUCK_SETTING_STRU;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF006
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_SYS_POWER_LEVEL*/
typedef U8 SYS_POWER_LV;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF00D
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_SYS_SLEEP_POWER*/
typedef struct
{
    U8 enable;
    U8 Buck_MV;
    U8 LDO_HV;
    U8 LDO_LV2;
    U8 REV2;
    U8 REV3;
    U8 REV4;
    U8 REV5;
    U8 REV6;
    U8 REV7;
    U8 REV8;
}PACKED SYS_SLEEP_PWR;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF012
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_SYS_DEEP_SLEEP_POWER*/
typedef struct
{
    U8 enable_hv_ret_keep;
    U8 hv_ret_keep;
    U8 hv_ret_vol_dslp;
    U8 REV1;
    U8 REV2;
    U8 REV3;
    U8 REV4;
    U8 REV5;
    U8 REV6;
    U8 REV7;
    U8 REV8;
}PACKED SYS_DEEP_SLEEP_PWR;

typedef struct
{
    U8 Buck_MV_EN;
    U8 LDO_HV_EN;
    U8 LDO_LV2_EN;
}PACKED SYS_SLEEP_PWR_INFO;

typedef struct
{
    U32 timeout_us;
    U16 adc_thrd;
}PACKED VCHG_CTRL_STRU;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF011
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_SW_CHG_INTR_CTRL*/
typedef struct
{
    U8 enable;
    VCHG_CTRL_STRU vchg_falling;
    VCHG_CTRL_STRU vchg_rising;
    U8 chg_intr_filter_en;
    U8 rev1;
    U16 rev2;
}PACKED SW_CHG_INTR_CTRL_STRU;

typedef struct
{
    U16 indirect_03;
}WIRE3_VAL_STRU;

typedef struct
{
    U8 enable;
    U8 hv_ret_vol;
}HV_RET_VOL_DSLP_STRU;

////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern SYS_SLEEP_PWR_INFO sleep_pwr_info;
EXTERN WIRE3_VAL_STRU gWire3_val;
EXTERN U8 gVAD_Status;
EXTERN HV_RET_VOL_DSLP_STRU gHV_ret_info;

EXTERN U32 gVolCtrlMask;
EXTERN U8 gBuckMvMask;
EXTERN U8 gBuckMvSel;

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN VOID SYS_Constrained_Power_Level_Active(VOID);
EXTERN VOID SYS_Constrained_Power_Level_Resume(VOID);
EXTERN VOID SYS_Get_Power_Plan_from_NVKEY(VOID);
EXTERN U8 SYS_Get_Power_Plan(VOID);
EXTERN VOID SYS_LDO_LV_Disable_for_NewPwrPlan(VOID);
EXTERN VOID SYS_BUCK_Freq_Thrd_Setting(VOID);
EXTERN VOID SYS_Sleep_Pwr_Setting(VOID);
EXTERN VOID SYS_Deep_Sleep_Pwr_Setting(VOID);

EXTERN void DRV_Set_BulkMV18(U8 mask);
EXTERN void DRV_Set_BulkMV16(U8 mask);
EXTERN void DRV_PwrCtrl_NrmlSlp_Enable(U32 mask);
EXTERN void DRV_PwrCtrl_NrmlSlp_Disable(U32 mask);

EXTERN VOID SYS_GET_CHG_CONFIG_from_NVKEY(VOID);
EXTERN VOID SYS_Get_CHG_Case_Type_from_NVKEY(VOID);
EXTERN VOID SYS_BuckMV_Thrd_Sniff(VOID);
EXTERN VOID SYS_BuckMV_Thrd_Recover(VOID);

#endif /* _PWR_MGR_H_ */

