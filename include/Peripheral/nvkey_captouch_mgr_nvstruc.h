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

#ifndef _NVKEY_CAPTOUCH_MGR_NVSTRUC_H_
#define _NVKEY_CAPTOUCH_MGR_NVSTRUC_H_

#include "types.h"


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Type Defintions /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF015
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_CAPTOUCH_LOWPOWER*/
typedef struct
{
    U8 lpmswitch;
    U8 captouchChannelBitMap;
    U8 touchMavg[4];
    U8 touchAvg[4];
    U8 coarseCapValue[4];
    S16 highThre[4];
    S16 lowThre[4];
    S8 finecap[4];
}PACKED LPM_SETTING_STRU;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF016
  */
/*NvkeyDefine NVKEYID_PERIPHERAL_CAPTOUCH_NORMAL*/
typedef struct
{
    U8 swtune_en;
    U8 captouchChannelBitMap;
    U8 touchMavg[4];
    U8 touchAvg[4];
    U8 coarseCapValue[4];
    S16 highThre[4];
    S16 lowThre[4];
    U8  eardetect;
    U8 swDebounceTime;
    U8 cfineBaseAvgNum;
    U8 baseKFrozeTime;
}PACKED NORMAL_SETTING_STRU;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF017
  */
/*NvkeyDefine SDWU_SETTING_STRU*/
typedef struct
{
    U8 sdwu_en;
    U8 sdtime;
    U8 wutime;
}PACKED SDWU_SETTING_STRU;
/**
 *
 * Captouch Info 	@KeyID 0xF2B6
 *
 */
/*NvkeyDefine NVKEYID_APP_CAPTOUCH_FEATURE_CONTROL*/
typedef struct
{
    U8 IsCapTouchEnable;
}PACKED CAPTOUCH_FEATURE_CONTROL_STRU;

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF021
  */
/*NvkeyDefine TUNE_SETTING_STRU*/
typedef struct
{
    U8 coarseCapValue[4];
    S8 fineCap[4];
    S16 earCheckBase[4];
}PACKED TUNE_SETTING_STRU;

//NVKEYID_PERIPHERAL_CAPTOUCH_FORCETOUCH_SETTING     = 0xF022,

/**
  *  @brief Use brief, otherwise the index won't have a brief explanation.
  *  @KeyID 0xF022
  */
/*NvkeyDefine TUNE_SETTING_STRU*/
#ifdef  HAL_CAPTOUCH_FORCE_TOUCH_ENABLED
typedef struct
{
    U8  forcetouch;
    S16 pre_thr;
    S16 rel_thr;
    S16 lev_thr;
    S16 cba_thr;
    S16 ini_delay;
    U8  reserved;
}PACKED FORCETOUCH_SETTING_STRU;
#endif
#ifdef  HAL_CAPTOUCH_SW_AUTO_SUSPEND_ENABLED
typedef struct
{
    U8  lpsd_ch_bitmap;
    U8  ch_bitmap;
    U8  lpwu_ch_bitmap;
    U8  time[4];
    U8  reserved[1];
}PACKED AUTOSUSPEND_SETTING_STRU;
#endif

////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#endif /* _NVKEY_CAPTOUCH_MGR_NVSTRUC_H_ */

