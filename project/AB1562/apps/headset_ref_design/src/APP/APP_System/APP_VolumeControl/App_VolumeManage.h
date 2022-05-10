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


#ifndef _APP_VOLUME_MANAGE_H_
#define _APP_VOLUME_MANAGE_H_

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	SET_MUTE_OFF,
	SET_MUTE_ON,
	TOGGLE_MUTE_ON_OFF
};

/**************************************************************************************************
* Define
**************************************************************************************************/
#define VOLUME_DONT_CARE 		0xFF

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_VolManage_SetSpkMuteByLink(BD_ADDR_T *pBdAddr, U8 muteCmd);
void APP_VolManage_Init(void);
void APP_VolManage_SpkVolumeUpDownHandler(BD_ADDR_T *pBdAddr, U16 keyCode, BOOL isFollowerCmd);//MMI_DRV_VoiceAdjustment
void APP_VolManage_SpkVolumeRemoteHandler(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component);
void APP_VolManage_MicVolumeUpDownHandler(BD_ADDR_T *pBdAddr, U16 keyCode);//MMI_DRV_MicVolHandler
void APP_VolManage_MicVolumeRemoteHandler(BD_ADDR_T *pBdAddr, U8 soundLevel, U8 component);
void APP_VolManage_AutoMICMuteOff(BD_ADDR_T *pBdAddr);//MMI_DRV_AutoMICMuteOff
void APP_VolManage_MicMuteHandler(BD_ADDR_T *pBdAddr, U8 muteCmd);//MMI_DRV_MicMuteHandler
void APP_VolManage_SetCurrentScoSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel);
void APP_VolManage_SetCurrentVgs(BD_ADDR_T *pBdAddr, U8 vgs);
U8 APP_VolManage_GetCurrentScoSoundLevel(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetCurrentScoVgs(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetScoSoundLevelByVgs(U8 vgs);
U8 APP_VolManage_GetVgsByScoSoundLevel(U8 soundLevel); //MMI_SearchSoundLevelByVgs
void APP_VolManage_SetCurrentA2dpSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel);
void APP_VolManage_SetCurrentAbsoluteVolume(BD_ADDR_T *pBdAddr, U8 absVol);
U8 APP_VolManage_GetCurrentA2dpSoundLevel(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetCurrentAbsoluteVolume(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetSoundLevelByAbsoluteVolume(U8 absVol);
U8 APP_VolManage_GetAbsoluteVolumeBySoundLevel(U8 soundLevel);
void APP_VolManage_SetCurrentScoMicSoundLevel(BD_ADDR_T *pBdAddr, U8 soundLevel);
void APP_VolManage_SetCurrentScoVgm(BD_ADDR_T *pBdAddr, U8 vgm);
U8 APP_VolManage_GetCurrentScoMicSoundLevel(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetCurrentScoVgm(BD_ADDR_T *pBdAddr);
U8 APP_VolManage_GetScoMicSoundLevelByVgm(U8 vgm);
U8 APP_VolManage_GetVgmByScoMicSoundLevel(U8 soundLevel);
#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
void APP_VolManage_SetCurrentMp3SoundLevel(U8 soundLevel);
U8 APP_VolManage_GetCurrentMp3SoundLevel(void);
#endif//MP3_LOCAL_PLAYBACK_MMI_CTRL
void APP_VolManage_SetCurrentVpRtSoundLevel(U8 soundLevel);
U8 APP_VolManage_GetCurrentVpRtSoundLevel(void);
#ifdef LINEIN_ENABLE
void APP_VolManage_SetCurrentLineInSoundLevel(U8 soundLevel);
U8 APP_VolManage_GetCurrentLineInSoundLevel(void);
#endif

#endif
