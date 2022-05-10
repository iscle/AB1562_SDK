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
 
 
#ifndef _APP_PAIRING_NVKEY_HANDLER_H_
#define _APP_PAIRING_NVKEY_HANDLER_H_

/**************************************************************************************************
* Define
**************************************************************************************************/
#define	APP_ALWAYS_ENA_DISCOVER_MODE_FEAT		0x0001
#define APP_ENTER_DISCOVER_AFTER_PWR_ON_FEAT	0x0002
#define APP_DO_NOT_DISCO_DURING_LINK_LOSS_FEAT	0x0004
#define APP_DISCOVER_MODE_AFTER_DISCONNECT_FEAT	0x0008
#define APP_DISCOVER_MODE_AFTER_RECOVERY_FEAT	0x0010

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	TIMER_PAIRING_MODE_TIMEOUT,
	TIMER_PAIRING_MODE_PDL_EMPTY_TIMEOUT,
	TIMER_PAIRING_CONNECTABLE_TIMEOUT
};

enum
{//actionAfterPairingModeTimeout
	RETURN_TO_CONNECTABLE,
	POWER_OFF,
	POWER_OFF_IF_PDL_IS_EMPTY
};

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PairNvkey_Init(void);
U16 APP_PairNvkey_GetTimer(U16 timerType);
bool APP_PairNvkey_IsFeatureOn(U16 feature);
U8 APP_PairNvkey_GetDiscoverableIfPdlLessThan(void);
U8 APP_PairNvkey_GetActionAfterPairingModeTimeout(void);
U8 APP_PairNvkey_GetMaxPageDevice(void);

#endif
