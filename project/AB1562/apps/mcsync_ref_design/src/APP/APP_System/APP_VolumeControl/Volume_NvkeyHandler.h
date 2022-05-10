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
 
 
#ifndef _APP_VOLUME_NVKEY_HANDLER_H_
#define _APP_VOLUME_NVKEY_HANDLER_H_

/**************************************************************************************************
* Enum
**************************************************************************************************/
#define INVALID_BEEP_PATTERN	0xFF

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	COMPONENT_SCO_WB,
	COMPONENT_SCO_NB,
	COMPONENT_A2DP,
	COMPONENT_LINEIN,
	COMPONENT_VPRT,
	COMPONENT_MIC_SCO,
	COMPONENT_MP3,
	COMPONENT_MIC_A2DP,
	COMPONENT_AIROTHRU_SPK,
	COMPONENT_AIROTHRU_MIC,
	COMPONENT_NUMBER
};

enum
{
	TIMER_MIC_MUTE_REMIND
};

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_VolumeNvkey_Init(void);
U8 APP_VolumeNvkey_GetScoVgsBySoundLevel(U8 soundLevel);
U8 APP_VolumeNvkey_GetScoMicVgmBySoundLevel(U8 soundLevel);
U8 APP_VolumeNvkey_GetBeepToneBySoundLevel(U8 component, U8 soundLevel);
U8 APP_VolumeNvkey_GetSoundLevelsBase(U8 component);
U8 APP_VolumeNvkey_GetSoundLevelsDec(U8 component);
U8 APP_VolumeNvkey_GetTotalSoundLevel(U8 component);
U8 APP_VolumeNvkey_GetDefaultSoundLevel(U8 component);
U8 APP_VolumeNvkey_GetPercentageBySoundLevel(U8 component, U8 soundLevel);
U8 APP_VolumeNvkey_GetAvrcpVolumeBySoundLevel(U8 soundLevel);
void APP_VolumeNvkey_WriteDefaultSoundLevel(U8 component, U8 soundLevel);
U16 APP_VolumeNvkey_GetTimer(U8 timerType);
void APP_VolumeNvKey_SetSoundLevelToFactoryValue(void);

#endif
