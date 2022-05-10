/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

#ifndef _CHARGER_CASE_COMMON_H_
#define _CHARGER_CASE_COMMON_H_

/*************************************************************************************************
* Enum
**************************************************************************************************/
typedef enum CHARGER_MSG_E {
    CHARGER_CHARGER_OUT_IND,
    CHARGER_CHARGING_IND,
    CHARGER_CHARGER_COMPLETE_IND,
} CHARGER_MSG_E;

typedef enum SMART_CASE_MSG_E {
    SMART_CASE_OFF_IND,
    SMART_CASE_OUT_OF_CASE_IND,
    SMART_CASE_LID_CLOSE_IND,
    SMART_CASE_LID_OPEN_IND,
} SMART_CASE_MSG_E;

typedef enum SMART_RSV_MSG_E {

    SMART_RSV_0_IND,
    SMART_RSV_1_IND,
    SMART_RSV_2_IND,
    SMART_RSV_3_IND,
    SMART_RSV_4_IND,

    SMARTV2_CASE_BATTERY_IND, //5
    SMARTV2_KEY_IND,
    SMARTV2_USR1_IND,
    SMARTV2_USR2_IND,
    SMARTV2_USR3_IND,

} SMART_RSV_MSG_E;

typedef enum CHARGER_STATE_E {
    STATE_CHARGER_OUT,
    STATE_CHARGING,
    STATE_CHARGER_CPL,
} CHARGER_STATE_E;

typedef enum SMART_CASE_STATE_E {
    STATE_CASE_OFF,
    STATE_OUT_OF_CASE,
    STATE_LID_CLOSE,
    STATE_LID_OPEN,
} SMART_CASE_STATE_E;

typedef enum CHARGER_CASE_EVT_E {
    EVT_CHARGER_IN_INT,
    EVT_CHARGER_CPL_INT,
    EVT_RECHG_INT,
    EVT_CHATGER_CPL_PATTERN,
    EVT_BT_ON_PATTERN,
    EVT_BT_OFF_PATTERN,
    EVT_CHARGER_OUT_PATTERN,
} CHARGER_CASE_EVT_E;


#endif//__CHARGER_CASE_COMMON_HH
