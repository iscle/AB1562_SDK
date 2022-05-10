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

#ifndef NTC_HH
#define NTC_HH

#include "types.h"
#include "message.h"

/*********************************************************************************
*   defines
**********************************************************************************/
enum LEVEL
{
    LEVEL_RISK_LOW = 0,
    LEVEL_ABNORMAL_LOW = 1,
    LEVEL_NORMAL = 2,
    LEVEL_ABNORMAL_HIGH = 3,
    LEVEL_RISK_HIGH = 4,
};

enum LEVEL_UNCHAR
{
    LEVEL_UNCHAR_SECURE_LOW = 0,
    LEVEL_UNCHAR_NORMAL_LOW = 1,
    LEVEL_UNCHAR_NORMAL = 2,
    LEVEL_UNCHAR_NORMAL_HIGH = 3,
    LEVEL_UNCHAR_SECURE_HIGH = 4,
};

enum LAST_CHARGER_STAUTS
{
    LAST_CHARGER_INIT = 0,
    LAST_CHARGER = 1,
    LAST_NOT_CHARGER = 2,
};


typedef enum NTC_MSG_E
{
	NTC_LEVEL_CHANGE_IND,
    NTC_LEVEL_CHANGE_UNCHAR_IND,
}NTC_MSG_E;

typedef struct NTC_LEVEL_CHANGE_IND_T
{
	enum LEVEL level;
}NTC_LEVEL_CHANGE_IND_T;

typedef struct NTC_LEVEL_CHANGE_UNCHAR_IND_T
{
	enum LEVEL_UNCHAR level;
}NTC_LEVEL_CHANGE_UNCHAR_IND_T;


/*********************************************************************************
*   API
**********************************************************************************/
void NTC_StartStop_Init(VOID);
void NTCRegsiterHandler(Handler app);
void ntc_set_warm_recharger_temperature(S8 temp);
void ntc_set_cool_recharger_temperature(S8 temp);


#endif
