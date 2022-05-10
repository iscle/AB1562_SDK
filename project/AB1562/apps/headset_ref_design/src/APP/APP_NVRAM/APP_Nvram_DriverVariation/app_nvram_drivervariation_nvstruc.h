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
 
 
#ifndef _APP_NVRAM_DRIVER_VARIATION_H_
#define _APP_NVRAM_DRIVER_VARIATION_H_

#include "bt.h"
#include "App_ResourceManager.h"
#include "Peq_NvkeyHandler.h"

/**************************************************************************************************
* Define
**************************************************************************************************/


#if defined(HID_SOC)
	#define NVRAM_LINK_KEY_ITEMS	4
//#elif defined(HEADSET_SOC)
#else
	#define NVRAM_LINK_KEY_ITEMS	10
	#define NVRAM_AIR_LINK_KEY_NO	2
	
	#define NVRAM_LINK_KEY_MISC_MASK_CONNECTED				0x01
	#define NVRAM_LINK_KEY_MISC_MASK_HFP					0x02
	#define NVRAM_LINK_KEY_MISC_MASK_HSP					0x04
	#ifdef HID_Profile
	#define NVRAM_LINK_KEY_MISC_MASK_HID					0x10
	#endif
	
	#define NVRAM_PROFILE_NOROLE	0
	#define NVRAM_PROFILE_LEFT		0x01
	#define NVRAM_PROFILE_RIGHT		0x02
	#define NVRAM_PROFILE_RELAY		0x04
	#define NVRAM_PROFILE_FOLLOW	0x08

	#define IS_LED_DISABLED 	 			0x02
	#define IS_VP_DISABLED					0x04
	#define IS_RINGTONE_DISABLED			0x08
	#define IS_MULTIPOINTS_ENABLE			0x10
	#define IS_VOLUME_ORIENTATION_REVERSED	0x20
	#define IS_HID_ENABLED         			0x40
	
	#define APP_SECTOR_DATA_VALID			0xAA
	//MMI_SECTOR_DATA_VALID
	#define APP_SECTOR_DATA_INVALID			0x00
	//MMI_SECTOR_DATA_INVALID

	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_ISMISCENABLED			(IS_MULTIPOINTS_ENABLE|IS_HID_ENABLED)
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_ISMISCENABLED
	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FM_HISTORY_FREQ			1062
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FM_HISTORY_FREQ
	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMHISTORYVOL				6
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_FMHISTORYVOL
	
	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINHISTORYVOL			14
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_LINEINHISTORYVOL
	
	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_DEFAULTSCOSOUNDLEVEL		13
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_DEFAULTSCOSOUNDLEVEL
//	#define APP_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_VPLANGINDEX				VOICE_LANG_DEFAULT
	//MMI_DRIVER_VARIATION_NVRAM_NONZERO_DEFAULT_VPLANGINDEX
#endif

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	LINK_KEY_NG,
	LINK_KEY_OK,
};

/**************************************************************************************
* Structure
***************************************************************************************/
/*NvkeyDefine NVKEYID_APP_NVRAM_DRIVER_VARIATION*/
/*NvkeyDefine NVKEYID_APP_NVRAM_DRIVER_VARIATION_AIR*/
/*NvkeyDefine NVKEYID_APP_NVRAM_DRIVER_VARIATION_FCD*/
typedef struct
{
	U8 key_status;
	BD_ADDR_T linkData;
	U8 scoSoundlevel;
	U8 scoMicSoundlevel;
	U8 profile;
	U8 a2dpSoundLevel;
	U8 reserved2;
	U8 miscMask;
}PACKED LINK_HISTORY_STRU;//LinkHistoryType

typedef struct
{
	U8 isMiscEnabled;
	U8 LineInHistoryVol;
	U8 LineInSmallStepVol;
	U8 defaultSCOSoundLevel;
	U8 smallStepSoundLevelIdle;
	U8 vpLangIndex;
	U8 peqIndex[PEQ_PATH_NUM];
}APP_DRIVER_BACKUP_NVRAM_TYPE_STRU;//DRIVER_BACKUP_NVRAM_TYPE

/*NvkeyDefine NVKEYID_APP_NVRAM_DRIVER_VARIATION_MISCPARA*/
typedef struct
{
	//non-zero init variables;
	APP_DRIVER_BACKUP_NVRAM_TYPE_STRU init;

	//zero-safe variables;
//	BD_ADDR_T agInd[MAX_MULTI_POINT_NO_FOR_SECTOR];
//	U8 PEQRecord[PEQ_PATH_NUM]; //A2DP and Line-In
//	U8 reserved[PEQ_PATH_NUM];
//	U8 PEQSectorMode[PEQ_PATH_NUM]; //0->default PEQ, 1->custom PEQ
}APP_MISC_PARA_TYPE_STRU;//MISC_PARA_TYPE


/**********************************************************************************************
* Prototype
*************************************************************************************************/
//void APP_NvramDrvVar_Init(void);

/**************************************************************************************************
* Variable
**************************************************************************************************/

/**************************************************************************************************
* Public Function
**************************************************************************************************/

U8 APP_NvramDrvVar_GetLinkHistoryNum(void);
U32 APP_NvramDrvVar_GetPayloadAddr(void);
U32 APP_NvramDrvVar_GetFCDPayloadAddr(void);
U32 APP_NvramDrvVar_GetMiscParaAddr(void);
U16 APP_NvramDrvVar_WritePayload(void *ptr, U16 length);
U16 APP_NvramDrvVar_WriteFCDPayload(void *ptr);
U16 APP_NvramDrvVar_WriteMiscPara(void *ptr);

#endif
