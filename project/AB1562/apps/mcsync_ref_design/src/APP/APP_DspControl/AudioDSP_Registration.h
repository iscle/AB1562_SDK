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
 
 
#ifndef _AUDIO_DSP_REGISTRATION_H_
#define _AUDIO_DSP_REGISTRATION_H_


/******************************************************************************
 * Define
*******************************************************************************/
#define AUDIO_DEVICE_LINK_NUM	(AUDIO_DEVICE_LINK_END - AUDIO_DEVICE_LINK_START + 1)
#define AUDIO_DEVICE_SYS_NUM	(AUDIO_DEVICE_SYS_END - AUDIO_DEVICE_SYS_START + 1)

#define AUDIO_DEVICE_BIT_MASK(x) (0x0001 << (x))

/******************************************************************************
 * Enumeration
*******************************************************************************/
enum
{
	AUDIO_DEVICE_NONE,

	AUDIO_DEVICE_LINK_START,
	AUDIO_DEVICE_SCO = AUDIO_DEVICE_LINK_START, //(1)
	AUDIO_DEVICE_SCO_HSP,
	AUDIO_DEVICE_SCO_IDLE,
	AUDIO_DEVICE_MUSIC,
	AUDIO_DEVICE_MUSIC_CALL_NOTIFY, //(5)
	AUDIO_DEVICE_CALL_STATE,
	AUDIO_DEVICE_AVRCP_PLAY,
	AUDIO_DEVICE_KEY_AT_CMD,
	AUDIO_DEVICE_AMA,
	AUDIO_DEVICE_LINK_END = AUDIO_DEVICE_AMA, //(9)

	AUDIO_DEVICE_SYS_START,
	AUDIO_DEVICE_LINE_IN = AUDIO_DEVICE_SYS_START, //(10)
	AUDIO_DEVICE_FM, //(11)

	AUDIO_DEVICE_MIC_TO_SPK,
	AUDIO_DEVICE_MP3,
	AUDIO_DEVICE_SYS_END = AUDIO_DEVICE_MP3, //(14)

	AUDIO_DEVICE_NUM = AUDIO_DEVICE_SYS_END //(14)
}; //Audio Device


/******************************************************************************
 * Structure
*******************************************************************************/
typedef struct
{
	BD_ADDR_T currentBdAddr;
	U8 currentAudioDev;
	U16 audioDevSysMask;
}APP_AUDIO_DEVICE_STRU;


/******************************************************************************
 * Public Functions
*******************************************************************************/
BD_ADDR_T *APP_AudioRegGetCurrentAudioLink();
BOOL APP_AudioDeviceRegister(BD_ADDR_T *pBdAddr, U8 audioDev);
void APP_AudioDeviceDeRegister(BD_ADDR_T *pBdAddr, U8 audioDev);
void APP_ResumeOtherAudioDevice(BD_ADDR_T *pBdAddrIn, U8 audioDev);


U8 APP_GetCurrentAudioDevice();
BD_ADDR_T *APP_GetCurrentAudioBdAddr();

void APP_AudioReg_SetCurrentAudioDevice(BD_ADDR_T *pBdAddr, U8 audioDev);
BOOL APP_IsAudioDeviceRegistered(BD_ADDR_T *pBdAddr, U8 audioDev);
BOOL APP_IsSCOGroupAudioDeviceRegistered(BD_ADDR_T *pBdAddr);
BOOL APP_IsDSPInSCO(BD_ADDR_T *pBdAddr);
BOOL APP_IsDSPActive(BD_ADDR_T *pBdAddr);
BOOL APP_IsLinkInHighAudioPriority(BD_ADDR_T *pBdAddr);
void APP_AudioDeviceDeRegisterAllDevice(void);


U8 APP_IsActiveAudioDevInAUX(void);//MMI_DRV_IsActiveAudioDevInAUX
U8 APP_IsActiveAudioDevInCall(void);//MMI_DRV_IsActiveAudioDevInCall
U8 APP_IsActiveAudioDevInMusic(void);//MMI_DRV_IsActiveAudioDevInMusic
void APP_AudioDevSetResumeTimer(U32 time);
BOOL APP_AudioDev_IsHighPriDevRegisted(U8 audioDev);
BOOL APP_DspDevReg_FillRhoData(void *pData);
BOOL APP_DspDevReg_AssignRhoData(void *pData);

#endif //_AUDIO_DSP_REGISTRATION_H_
