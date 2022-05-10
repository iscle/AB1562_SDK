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
 
 
#ifndef __PM_NVKEY_H__
#define __PM_NVKEY_H__

#include "nvkey.h"
#include "nvkey_list.h"
#include "pm_nvkey_sniff_parameter_nvstruc.h"

//#define NVKEY_ID_PM_SNIFF_PARAMETER 	0x0019		Transfer to NVKEYID_PROFILE_PM_SNIFF_PARAM
//#define NVKEY_ID_PM_SNIFF_TIMER_OFFSET	0x00A0	Transfer to NVKEYID_PROFILE_PM_SNIFF_TIMER

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * @KeyID 0xF21C
 * 
 */

/*NvkeyDefine NVKEYID_APP_SYSTEM_INFO_CONFIG*/
typedef struct
{
	U32 classOfDevice;		  	/**< @Value 0x41423135333020425420486F7374    @Desc  AB1530 BT Host */ 
	U8 ioCapability;
	U8 oobDataType;
	U8 keyDistribution;
	U8 pinCodelength;
	//U8 *pinCode;
	U8 bleIoCapbility;
    U8 bleOobDataType;
    U8 bleAuthReq;
    U8 bleKeyDistribution;
    U8 bleSecurityMode;
    U8 bleSecurityLevel;
}PACKED SYSTEM_INFORMATION_STRU;


/**************************************************************************************************
* Public functions
**************************************************************************************************/	
BOOL PM_TimerNvkeyInit(void);
U16 PM_ReadNvkeySniffTimer(void);
U16 PM_ReadNvkeyCheckRemoteAGAcceptEnterSniffTimer(void);
void PM_SniffNvkeyInit(void);
U8 PM_ReadNvkeySniffCount(void);
U16 PM_ReadNvkeySniffMaxLatency(void);
U16 PM_ReadNvkeySniffMiniLocalTimeout(void);
U16 PM_ReadNvkeySniffMinRemoteTimeout(void);
void PM_ReadNvkeySniffPara(LINK_MODE_PARAM_T * param);

BOOL PM_NvKey_IsUserDefineDeviceNameExist();
U16 PM_NvKey_GetDefaultDeviceNameLength();
U8* PM_NvKey_GetDefaultDeviceNameAddr();
U16 PM_NvKey_GetUserDeviceNameLength();
U8* PM_NvKey_GetUserDeviceNameAddr();
U8 *PM_Nvkey_GetDeviceName();
U16 PM_Nvkey_GetDeviceNameLength();

<<<<<<< HEAD
=======
U16 PM_NvKey_GetDefaultBleAdvDataLength(void);
U8 *PM_NvKey_GetDefaultBleAdvDataAddr(void);
BOOL PM_NvKey_IsUserDefineBleAdvDataExist(void);
U8 *PM_Nvkey_GetBleAdvData(void);
U16 PM_Nvkey_GetBleAdvDataLength(void);

>>>>>>> db20e11 (second commit)
#endif //__PM_NVKEY_H__
