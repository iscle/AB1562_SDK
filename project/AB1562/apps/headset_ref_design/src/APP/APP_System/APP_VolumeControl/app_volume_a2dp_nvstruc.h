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
 
 
#ifndef _VOLUME_A2DP_H_
#define _VOLUME_A2DP_H_

#include "Volume_Setting.h"

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * A2dp Sound Level Setting 	@KeyID 0xF23B
 *
 */

/*NvkeyDefine NVKEYID_APP_VOLUME_A2DP_SETTING*/
typedef struct
{
	APP_VOL_PARA_STRU a2dpVolPara;
	U8 a2dpSoundlevelToAvrcpVolume[1];
	U8 musicSoundLevelToBeepTone[1];
	U8 a2dpSoundlevelToPercentage[1];
}VOLUME_A2DP_SETTING_STRU;

/*
#define SHORT_1_NOTE_1 0

{
	{//a2dpVolPara
		17,	//totalSoundLevel
		13,	//defaultSoundLevel
		{//DecreaseSoundLevelCtl
			15, 0
		}
	},
	
	{//a2dpSoundlevelToAvrcpVolume
		0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 127
	},
	
	{//musicSoundLevelToBeepTone
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1, 
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1,
		SHORT_1_NOTE_1
	},
	
	{//a2dpSoundlevelToPercentage
		0, 5, 11, 18, 24, 29, 35, 41, 47, 53, 59, 65, 71, 76, 84, 93, 100
	}
};
*/

#endif
