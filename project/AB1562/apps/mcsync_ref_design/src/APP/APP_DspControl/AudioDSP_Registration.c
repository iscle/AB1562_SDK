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


#include "bt.h"
#include "AudioDSP_Registration.h"
#include "AudioDSP_StreamManager.h"
#include "App_A2dp.h"
#include "App_Avrcp.h"
#include "App_ResourceManager.h"
#include "a2dp.h"
#include "A2dp_Interface.h"
#include "nvkey.h"
#include "nvkey_list.h"
#include "App_State.h"
#include "App_Interface.h"
#include "App_State.h"
#include "audiodsp_nvkeypriority_nvstruc.h"
#include "App_A2dp_NVKeyHandler.h"
#include "App_Media.h"
#include "App_PowerOff.h"
#include "App_Connection.h"
#include "APP_HfpState.h"
#include "App_VolumeManage.h"
#include "APP_HfpScoHandler.h"
#include "AudioDSP_AncPassThru.h"
#ifdef LINEIN_ENABLE
#include "App_LineIn.h"
#endif
#include "App_RhoHandler.h"


/******************************************************************************
 * Definition
*******************************************************************************/
#define CHECK_AUDIO_TIME 1000 //ms

/******************************************************************************
 * Enumeration
*******************************************************************************/
enum
{
	AUDIO_TIMER_RESUME_SOUND,
};

/******************************************************************************
 * Variable
*******************************************************************************/
static U32 app_AudioResumeHandler(Handler handler, U16 id, void *msg, U32 ext_id);
static const HandlerData gAppAudioResumeHandle = { app_AudioResumeHandler };

APP_AUDIO_DEVICE_STRU gAppAudioDevCtl;

/******************************************************************************
 * Static Functions (Audio Device)
*******************************************************************************/
static U16 app_AudioReg_GetAudioDevSysMask()
{
	return gAppAudioDevCtl.audioDevSysMask;
}

static void app_AudioReg_SetAudioDevSysMask(U8 audioDev)
{
	if(!(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END))
		return;

	gAppAudioDevCtl.audioDevSysMask |= AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_SYS_START);
}

static void app_AudioReg_ClearAudioDevSysMask(U8 audioDev)
{
	if(!(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END))
		return;

	gAppAudioDevCtl.audioDevSysMask &= ~AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_SYS_START);
}

/******************************************************************************
 *Public Functions (Audio Device)
*******************************************************************************/
void APP_AudioReg_SetCurrentAudioDevice(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	gAppAudioDevCtl.currentAudioDev = audioDev;

	if(pBdAddr)
	{
		FW_Memcpy(&gAppAudioDevCtl.currentBdAddr, pBdAddr, sizeof(BD_ADDR_T));
		APP_SetServiceBdAddr(pBdAddr);
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Set Current Audio Dev[%d]:0x%x%x", 3, audioDev, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	}
	else
	{
		FW_Memset(&gAppAudioDevCtl.currentBdAddr, 0, sizeof(BD_ADDR_T));
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg]Set Current Audio Dev[%d]:System", 1, audioDev);
	}
}

BOOL APP_IsAudioDeviceRegistered(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	if(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END && pBdAddr)
	{
		return (APP_GetAudioDevLinkMask(pBdAddr) & AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_LINK_START)) ?
				TRUE : FALSE;
	}
	else if(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END)
	{
		return (app_AudioReg_GetAudioDevSysMask() & AUDIO_DEVICE_BIT_MASK(audioDev - AUDIO_DEVICE_SYS_START)) ?
				TRUE : FALSE;
	}

	return FALSE;
}

BOOL APP_IsSCOGroupAudioDeviceRegistered(BD_ADDR_T *pBdAddr)
{
	if(APP_IsAudioDeviceRegistered(pBdAddr, AUDIO_DEVICE_SCO))
		return TRUE;
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_IsAudioDeviceRegistered(pBdAddr, AUDIO_DEVICE_SCO_HSP))
		return TRUE;
#endif
	if(APP_IsAudioDeviceRegistered(pBdAddr, AUDIO_DEVICE_SCO_IDLE))
		return TRUE;

	return FALSE;
}



U8 APP_GetCurrentAudioDevice(void)
{
	return gAppAudioDevCtl.currentAudioDev;
}

BD_ADDR_T *APP_GetCurrentAudioBdAddr(void)
{
	if(FW_IsBdAddrZero(&gAppAudioDevCtl.currentBdAddr))
		return NULL;

	return &gAppAudioDevCtl.currentBdAddr;
}

static U8 app_AudioRegGetAudioPriority(U8 audioDev)
{
	U8 pri, *pPri;

	//FW_Assert(audioDev <= AUDIO_DEVICE_NUM);
	if(audioDev > AUDIO_DEVICE_NUM)
		return 0xFF;

	pPri = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_AUDIO_DEVICE_PRIORITY);
	for(pri = 0; pri < sizeof(AUDIO_PRIORITY_STRU); pri++)
	{
		if(pPri[pri] == audioDev)
			return pri;
	}

	return 0;
}

static U8 app_GetAudioDeviceByPriority(U8 pri)
{
	U8 *pPri = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_AUDIO_DEVICE_PRIORITY);

	if(pri < sizeof(AUDIO_PRIORITY_STRU))
		return pPri[pri];

	return AUDIO_DEVICE_NONE;
}

static BOOL app_SetAudioDeviceByLink(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	if(!APP_PowerOff_IsPowerOn())
		return FALSE;

	if(APP_IsAudioDeviceRegistered(pBdAddr, audioDev))
	{
		APP_AudioReg_SetCurrentAudioDevice(pBdAddr, audioDev);
		return TRUE;
	}

	return FALSE;
}

static BOOL app_SetAudioDeviceSys(U8 audioDev)
{
	if(!APP_PowerOff_IsPowerOn())
		return FALSE;

	if(APP_IsAudioDeviceRegistered(NULL, audioDev))
	{
		APP_AudioReg_SetCurrentAudioDevice(NULL, audioDev);
		return TRUE;
	}

	return FALSE;
}

static BOOL app_SetAudioDeviceToAnotherLink(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	U8 linkIndex;
	BD_ADDR_T *pBdAddrAnother;

	if(!APP_PowerOff_IsPowerOn())
		return FALSE;

	for(linkIndex = 0; linkIndex < FW_MAX_LINK_IDX; linkIndex++)
	{
		pBdAddrAnother = APP_GetBdAddrByLinkIndex(linkIndex);

		if(pBdAddrAnother && !FW_CmpBdAddr(pBdAddrAnother, pBdAddr) && app_SetAudioDeviceByLink(pBdAddrAnother, audioDev))
		{
			return TRUE;
		}
	}

	return FALSE;
}

/******************************************************************************
 * Static Functions
*******************************************************************************/
<<<<<<< HEAD
#ifdef A2DP_Profile
=======
#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
#ifdef SUPPORT_MULTI_POINT
static void app_AudioRegSendAnotherLinkPauseMusic(BD_ADDR_T *pBdAddr)
{
	U8 linkIndex;

	for(linkIndex = 0; linkIndex < MAX_MULTI_POINT_NO; linkIndex++)
	{
		BD_ADDR_T * pOtherBdAddr = APP_GetBdAddrByLinkIndex(linkIndex);
		if(pOtherBdAddr && !FW_CmpBdAddr(pOtherBdAddr, pBdAddr))
		{
			APP_A2dp_SendPauseMusic(pOtherBdAddr, TRUE);
		}
	}
}
#endif
#endif

static BD_ADDR_T *app_IsOtherLinkAudioDevRegistered(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	BD_ADDR_T *pOtherBdAddr;
	U8 i;
	U8 startLinkInd = APP_GetLinkIndexByBdAddr(pBdAddr);

	for(i = startLinkInd; i < startLinkInd + FW_MAX_LINK_IDX; i++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(i % FW_MAX_LINK_IDX);

		if(!FW_CmpBdAddr(pOtherBdAddr, pBdAddr) && APP_IsAudioDeviceRegistered(pOtherBdAddr, audioDev))
			return pOtherBdAddr;
	}

	return NULL;
}

static BD_ADDR_T *app_IsOtherMusicPlaying(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pOtherBdAddr;
	U8 i;
	U8 startLinkInd = APP_GetLinkIndexByBdAddr(pBdAddr);

	for(i = startLinkInd; i < startLinkInd + FW_MAX_LINK_IDX; i++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(i % FW_MAX_LINK_IDX);

		if(!FW_CmpBdAddr(pOtherBdAddr, pBdAddr) && APP_a2dp_is_in_streaming(pOtherBdAddr))
			return pOtherBdAddr;
	}

	return NULL;
}

static BD_ADDR_T *app_IsOtherMusicResume(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pOtherBdAddr;
	U8 i;
	U8 startLinkInd = APP_GetLinkIndexByBdAddr(pBdAddr);

	for(i = startLinkInd; i < startLinkInd + FW_MAX_LINK_IDX; i++)
	{
		pOtherBdAddr = APP_GetBdAddrByLinkIndex(i % FW_MAX_LINK_IDX);

		if(!FW_CmpBdAddr(pOtherBdAddr, pBdAddr) && a2dp_is_resume(pOtherBdAddr))
			return pOtherBdAddr;
	}

	return NULL;
}

/*
	S: Streaming, R: Resume
	(Sx, Ro), (So, Ro)	--> No play, has to resume
	(So, Rx)			--> Playing
	(Sx, Rx)			--> No music

	Other\Current
	O\C	  |	So,Ro |	So,Rx |	Sx,Ro |	Sx,Rx
	--------------------------------------
	So,Ro |	  C	  |	  C	  |   C	  |   O
	--------------------------------------
	So,Rx |	  O	  |   C	  |   O	  |   O
	--------------------------------------
	Sx,Ro |   C	  |   C	  |   C	  |   O
	--------------------------------------
	Sx,Rx |	  C	  |   C	  |   C	  |
*/
static BOOL app_SetAudioDeviceMusicResume(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	BD_ADDR_T *pOtherBdAddr;

	if(!APP_IsAudioDeviceRegistered(pBdAddr, audioDev))
		return FALSE;

	if(!app_IsOtherLinkAudioDevRegistered(pBdAddr, audioDev))
	{
		if(app_SetAudioDeviceByLink(pBdAddr, audioDev))
			return TRUE;
	}

<<<<<<< HEAD
	#ifdef A2DP_Profile
=======
	#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	if(APP_a2dp_is_in_streaming(pBdAddr))
	{
	}
	else if((pOtherBdAddr = app_IsOtherMusicPlaying(pBdAddr)))
	{
		return FALSE;
	}
	else if(a2dp_is_resume(pBdAddr))
	{
	}
	else if((pOtherBdAddr = app_IsOtherMusicResume(pBdAddr)))
	{
		return FALSE;
	}
	#endif

	if(app_SetAudioDeviceByLink(pBdAddr, audioDev))
		return TRUE;

	return FALSE;
}

/******************************************************************************
 * Static Functions
*******************************************************************************/
static BOOL app_IsScoRelativeAduioDev(U8 audioDev)
{
	if(audioDev == AUDIO_DEVICE_SCO || audioDev == AUDIO_DEVICE_SCO_HSP ||
		audioDev == AUDIO_DEVICE_SCO_IDLE || audioDev == AUDIO_DEVICE_KEY_AT_CMD)
		return TRUE;

	return FALSE;
}

static void app_AudioRegPauseCurrentMusic(BOOL isPause)
{
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	U8 currentActiveDev = APP_GetCurrentAudioDevice();

	if(!pCurrentActiveLink)
		return;

	APP_A2dp_StopDSP(pCurrentActiveLink);

	if(isPause && (currentActiveDev == AUDIO_DEVICE_MUSIC || currentActiveDev == AUDIO_DEVICE_AVRCP_PLAY))
	{
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg]Pause Current Music:0x%x%x", 2, FW_bdaddr_to_2U32(pCurrentActiveLink, TRUE), FW_bdaddr_to_2U32(pCurrentActiveLink, FALSE));
		APP_A2dp_SendPauseMusic(pCurrentActiveLink, FALSE);
		#endif
	}
}

static BOOL app_WhetherSetMusicAudioDev()
{
	if(APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_LAST_IN_FIRST_PLAYED_NO_PAUSE_OTHERS_FEAT))
	{
		app_AudioRegPauseCurrentMusic(FALSE);
		return TRUE;
	}
	else if(APP_A2dp_NVKeyIsFeatureOn(APP_MUSIC_LAST_IN_FIRST_PLAYED_AND_PAUSE_OTHERS_FEAT))
	{
<<<<<<< HEAD
		#ifdef A2DP_Profile
=======
		#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
		app_AudioRegPauseCurrentMusic(TRUE);
		#endif
		return TRUE;
	}
<<<<<<< HEAD
	#ifdef AVRCP_Profile
=======
	#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
	else if(APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_MEMPUT_WHEN_OTHER_IN_MUSIC_FEAT))
	{
		//Mantis 10306 cover some phone can't change AVRCP Play status
		#if 0
		U8 playStatus = APP_Avrcp_GetPlayStatus(pCurrentActiveLink);

		if(playStatus == AVRCP_PLAYBACK_STATUS_PAUSED || playStatus == AVRCP_PLAYBACK_STATUS_STOPPED)
		{
			app_AudioRegPauseCurrentMusic(FALSE);
			return TRUE;
		}
		#endif
	}
	#endif

	return FALSE;
}

/*
	Ca: Call active, In: Incoming, Out: Outgoing
	Other\Current
	O\C   |	  Ca  |   In  |  Out  | Idle
	--------------------------------------
	Ca	  |	  C	  |	  O	  |   O	  |   O
	--------------------------------------
	In	  |	  C	  |   C	  |   C	  |   O
	--------------------------------------
	Out	  |   C	  |   O	  |   C	  |   O
	--------------------------------------
	Idle  |	  C	  |   C	  |   C	  |	  O
*/
static BOOL app_WhetherSetScoAudioDev(BD_ADDR_T *pBdAddr)
{
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();

	if(APP_State_CheckLinkInCallActiveState(pCurrentActiveLink))
	{
		#ifdef HFP_Profile
		//if(APP_MAX_ONGOING_CALL_NO == APP_HFP_SearchCallStatusInd(pCurrentActiveLink, APP_CALL_HELD))
		if(APP_MAX_ONGOING_CALL_NO != APP_HFP_SearchCallStatusInd(pCurrentActiveLink, APP_CALL_ACTIVE))
			return FALSE;
		#endif

		if(APP_State_CheckLinkInCallActiveState(pBdAddr))
			return TRUE;
		else
			return FALSE;
	}
	else if(APP_State_CheckLinkInCallActiveState(pBdAddr))
	{
		return TRUE;
	}
	else if(APP_State_GetTopState(pCurrentActiveLink) == APP_HFP_OUTGOING)
	{
		return FALSE;
	}
	else if(APP_State_GetTopState(pCurrentActiveLink) == APP_HFP_INCOMMING)
	{
		if(APP_State_GetTopState(pBdAddr) == APP_HFP_OUTGOING)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return TRUE;
	}

	return FALSE;
}

/******************************************************************************
 * Static Functions (Handler)
*******************************************************************************/
static U32 app_AudioResumeHandler(Handler handler, U16 id, void *msg, U32 ext_id)
{
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	U8 currentActiveDev = APP_GetCurrentAudioDevice();

	#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	#endif

	UNUSED(handler);
	UNUSED(msg);
	UNUSED(ext_id);

	switch(id)
	{
		case AUDIO_TIMER_RESUME_SOUND:
			switch(currentActiveDev)
			{
				case AUDIO_DEVICE_FM:
					if(!APP_State_CheckNestStateByLink(APP_EOF, APP_FM))
						APP_State_AddTopState(APP_EOF, APP_FM);
					break;

				#ifdef LINEIN_ENABLE
				case AUDIO_DEVICE_LINE_IN:
					if(!APP_State_CheckNestStateByLink(APP_EOF, APP_LINE_IN))
					{
						if(APP_EXIT_LINE_IN_WHEN_BT_CONNECED_FEAT && APP_Conn_CheckProfileOnAllLinks())
							break;

						APP_State_AddTopState(APP_EOF, APP_LINE_IN);
					}
					break;
				#endif

				#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
				case AUDIO_DEVICE_MP3:
					if(!APP_State_CheckNestStateByLink(APP_EOF, APP_MP3))
					{
						APP_State_AddTopState(APP_EOF, APP_MP3);
						drv_mp3_remember_last_status();
						FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
						audioDspPara.audioType = AUDIO_MP3;
						APP_AudioDspPlay(&audioDspPara);
					}
					break;
				#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

				case AUDIO_DEVICE_MUSIC:
				case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
				case AUDIO_DEVICE_AVRCP_PLAY:
<<<<<<< HEAD
					#ifdef A2DP_Profile
=======
					#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
					if(a2dp_is_resume(pCurrentActiveLink))
					{
						APP_A2dp_SendResume(pCurrentActiveLink);
					}
					if(a2dp_is_in_streaming(pCurrentActiveLink))
					{
						APP_A2dp_DspPlay(pCurrentActiveLink);
					}
					else if(!a2dp_is_resume(pCurrentActiveLink))
					{
						APP_AudioDeviceDeRegister(pCurrentActiveLink, currentActiveDev);
						APP_ResumeOtherAudioDevice(pCurrentActiveLink, currentActiveDev);
					}
					#endif
					break;

				case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                    
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                    
>>>>>>> db20e11 (second commit)
				case AUDIO_DEVICE_SCO_HSP:
#endif                    
				case AUDIO_DEVICE_SCO_IDLE:
					APP_HfpSco_SendDSPCmd(pCurrentActiveLink);
					break;
			}
			break;
	}

	return 0;
}

/******************************************************************************
 * Public Functions
*******************************************************************************/
BOOL APP_AudioDeviceRegister(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	U8 currentActiveDev = APP_GetCurrentAudioDevice();
	U8 priIn, priCurr;
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	//1. Record new audio device
	if(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END && pBdAddr)
	{
		APP_SetAudioDevLinkMask(pBdAddr, audioDev);
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Reg[%d]:0x%x%x", 3, audioDev, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
	}
	else if(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END)
	{
		app_AudioReg_SetAudioDevSysMask(audioDev);
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Reg[%d]:System", 1, audioDev);
	}
	else
    {
        DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Reg[%d] Error!!", 1, audioDev);
        FW_Assert(FALSE);
		return FALSE;
    }


	//2. Check if audio device is active
	if(AUDIO_DEVICE_NONE == currentActiveDev)
	{
		goto SET_AUDIO_DEVICE;
	}

	//3. Get priority of current audio
	priIn = app_AudioRegGetAudioPriority(audioDev);
	priCurr = app_AudioRegGetAudioPriority(currentActiveDev);


    DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Current Dev id:%d priority:%d vs Coming Dev id:%d  priority:%d", 4, currentActiveDev ,priCurr ,audioDev, priIn);

	if(priCurr > priIn) //High priority is active now
	{
		if((audioDev == AUDIO_DEVICE_MUSIC || audioDev == AUDIO_DEVICE_AVRCP_PLAY) &&
			(currentActiveDev == AUDIO_DEVICE_MUSIC || currentActiveDev == AUDIO_DEVICE_AVRCP_PLAY) &&
			!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr))
		{
			if(app_WhetherSetMusicAudioDev())
				goto SET_AUDIO_DEVICE;
		}

		if((audioDev == AUDIO_DEVICE_MUSIC) && (currentActiveDev == AUDIO_DEVICE_AMA))
		{
			goto SET_AUDIO_DEVICE;
		}

		return FALSE;
	}
	else if(priCurr < priIn) //Disable low priority audio device
	{
		switch(currentActiveDev)
		{
			case AUDIO_DEVICE_SCO_IDLE:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                
>>>>>>> db20e11 (second commit)
			case AUDIO_DEVICE_SCO_HSP:
#endif                
			case AUDIO_DEVICE_SCO:
				if(!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) || !app_IsScoRelativeAduioDev(audioDev))
				{
					FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
					FW_Memcpy(&audioDspPara.bdAddr, pCurrentActiveLink, sizeof(BD_ADDR_T));
					audioDspPara.audioType = AUDIO_SCO;
					APP_AudioDspStop(&audioDspPara);
				}
				break;

			case AUDIO_DEVICE_MUSIC:
			case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			case AUDIO_DEVICE_AVRCP_PLAY:
				switch(audioDev)
				{
					case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
					case AUDIO_DEVICE_CALL_STATE:
						if(!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr))
						{
							APP_A2dpDspStop(pCurrentActiveLink);
<<<<<<< HEAD
							#ifdef A2DP_Profile
=======
							#ifdef AIR_A2DP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
							if(!APP_A2dp_NVKeyIsFeatureOn(APP_A2DP_MEMPUT_WHEN_CALLACTIVE_FEAT) && APP_IsActiveLink(pBdAddr))
							{
								#ifdef SUPPORT_MULTI_POINT
								app_AudioRegSendAnotherLinkPauseMusic(pBdAddr);
								#endif
							}
							#endif
						}
						break;

					case AUDIO_DEVICE_MUSIC:
					case AUDIO_DEVICE_AVRCP_PLAY:
						if(!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr))
						{
							if(APP_Avrcp_isDisablePlayStatusMonitor(pBdAddr) && APP_Avrcp_isDisablePlayStatusMonitor(pCurrentActiveLink))
							{
								break;
							}
							if(!app_WhetherSetMusicAudioDev())
								return FALSE;
						}
						break;

					case AUDIO_DEVICE_LINE_IN:
						app_AudioRegPauseCurrentMusic((!APP_A2DP_MEMPUT_WHEN_LINE_IN_FEAT) ? TRUE : FALSE);
						break;

					case AUDIO_DEVICE_FM:
						app_AudioRegPauseCurrentMusic((!APP_A2DP_MEMPUT_WHEN_FM_ON_FEAT) ? TRUE : FALSE);
						break;

					#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
					case AUDIO_DEVICE_MP3:
						app_AudioRegPauseCurrentMusic((1/*!MMI_A2DP_MEMPUT_WHEN_MP3_ON_FEAT*/) ? TRUE : FALSE);
						break;
					#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

					case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                        
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                        
>>>>>>> db20e11 (second commit)
					case AUDIO_DEVICE_SCO_HSP:
#endif                        
					case AUDIO_DEVICE_SCO_IDLE:
					case AUDIO_DEVICE_KEY_AT_CMD:
						APP_A2dpDspStop(pCurrentActiveLink);
						APP_A2dp_SendPauseMusic(pBdAddr, TRUE);
						break;

					case AUDIO_DEVICE_MIC_TO_SPK:
						break;

					case AUDIO_DEVICE_AMA:
						break;

					default:
						FW_Assert(FALSE);
						break;
				}
				break;

			case AUDIO_DEVICE_LINE_IN:
				APP_State_RmvState(APP_EOF, APP_LINE_IN);
				break;

			case AUDIO_DEVICE_FM:
				APP_State_RmvState(APP_EOF, APP_FM);
				/*if(audioDev == AUDIO_DEVICE_LINE_IN)
				{
					if(!MMI_LINE_IN_DETECT_BY_GPIO)
					{
						APP_AudioDeviceDeRegister(NULL, AUDIO_DEVICE_FM);
						//De-registered
					}
				}*/
				break;

			#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
			case AUDIO_DEVICE_MP3:
				FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
				audioDspPara.audioType = AUDIO_MP3;
				APP_AudioDspStop(&audioDspPara);
				APP_State_RmvState(APP_EOF, APP_MP3);
				break;
			#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

			case AUDIO_DEVICE_MIC_TO_SPK:
				break;
		}
	}
	else //Same priority
	{
		switch(audioDev)
		{
			case AUDIO_DEVICE_LINE_IN:
			case AUDIO_DEVICE_FM:
			#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
			case AUDIO_DEVICE_MP3:
			#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
				FW_Assert(FALSE);
				break;

			case AUDIO_DEVICE_MUSIC:
			case AUDIO_DEVICE_AVRCP_PLAY:
				if(FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) || !app_WhetherSetMusicAudioDev())
					return FALSE;
				break;

			case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			case AUDIO_DEVICE_CALL_STATE:
				if(!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr))
					return FALSE;
				break;

			case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                
>>>>>>> db20e11 (second commit)
			case AUDIO_DEVICE_SCO_HSP:
#endif                
				if(!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr))
				{
					if(app_WhetherSetScoAudioDev(pBdAddr))
					{
						FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
						FW_Memcpy(&audioDspPara.bdAddr, pCurrentActiveLink, sizeof(BD_ADDR_T));
						audioDspPara.audioType = AUDIO_SCO;
						APP_AudioDspStop(&audioDspPara);
					}
					else
						return FALSE;
				}
				break;

			case AUDIO_DEVICE_SCO_IDLE:
				break;

			case AUDIO_DEVICE_MIC_TO_SPK:
				break;

			case AUDIO_DEVICE_KEY_AT_CMD:
				return FALSE;
				break;
		}
	}

	SET_AUDIO_DEVICE:
	APP_AudioReg_SetCurrentAudioDevice(pBdAddr, audioDev);

	return TRUE;
}

void APP_AudioDeviceDeRegister(BD_ADDR_T *pBdAddr, U8 audioDev)
{
	//Delete audio device
	if(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END && pBdAddr)
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Dereg[%d]:0x%x%x", 3, audioDev, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
		APP_ClearAudioDevLinkMask(pBdAddr, audioDev);
	}
	else if(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END)
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Dereg[%d]: System", 1, audioDev);
		app_AudioReg_ClearAudioDevSysMask(audioDev);
	}
	else
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Dereg[%d] Error!!", 1, audioDev);
        FW_Assert(FALSE);
		return;
	}
}

void APP_ResumeOtherAudioDevice(BD_ADDR_T *pBdAddrIn, U8 audioDev)
{
	U32 resumeTime;
	U8 priIn, pri, j, startLinkInd, checkDev;
    BD_ADDR_T bdAddr, *pBdAddr;
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	U8 currentActiveDev = APP_GetCurrentAudioDevice();
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

    //Prevent from varying while the input pointer address ("pBdAddrIn") is service "BdAddr".
	//It will modify service "BdAddr" when set new audio device.
    DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Resume Other[%d]:BdAddr:0x%x%x", 3, audioDev, FW_bdaddr_to_2U32(pBdAddrIn, TRUE), FW_bdaddr_to_2U32(pBdAddrIn, FALSE));
    DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Current[%d]:BdAddr:0x%x%x", 3, currentActiveDev, FW_bdaddr_to_2U32(pCurrentActiveLink, TRUE), FW_bdaddr_to_2U32(pCurrentActiveLink, FALSE));

	if(pBdAddrIn)
	{
		pBdAddr = &bdAddr;
		FW_Memcpy(pBdAddr, pBdAddrIn, sizeof(BD_ADDR_T));
	}
	else
		pBdAddr = NULL;

	if(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END && pBdAddr)
	{
	}
	else if(AUDIO_DEVICE_SYS_START <= audioDev && audioDev <= AUDIO_DEVICE_SYS_END)
	{
	}
	else
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Audio Resume Other[%d]:Error!!", 1, audioDev);
        FW_Assert(FALSE);
		return;
	}

	//1. Check if audio device is active
	if(currentActiveDev != audioDev || (!FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) && (pCurrentActiveLink || pBdAddr)))
		return;

	//2. Check if the same audio device is active at other link
	if(AUDIO_DEVICE_LINK_START <= audioDev && audioDev <= AUDIO_DEVICE_LINK_END)
		if(app_SetAudioDeviceToAnotherLink(pBdAddr, audioDev))
			goto ACTIVATE_NEW_AUDIO_DEV;

	//3. Search the lower priority audio device
	if((priIn = app_AudioRegGetAudioPriority(audioDev)))
	{
		startLinkInd = (!pBdAddr) ? 0 : APP_GetLinkIndexByBdAddr(pBdAddr);
		pri = priIn;

		while(pri--)
		{
			checkDev = app_GetAudioDeviceByPriority(pri);

			switch(checkDev)
			{
				case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE                   
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE                   
>>>>>>> db20e11 (second commit)
				case AUDIO_DEVICE_SCO_HSP:
#endif                    
				case AUDIO_DEVICE_SCO_IDLE:
				case AUDIO_DEVICE_CALL_STATE:
				case AUDIO_DEVICE_KEY_AT_CMD:
					for(j = startLinkInd; j < startLinkInd + FW_MAX_LINK_IDX; j++)
					{
						BD_ADDR_T *pOtherBdAddr = APP_GetBdAddrByLinkIndex(j % FW_MAX_LINK_IDX);

						if(pOtherBdAddr && app_SetAudioDeviceByLink(pOtherBdAddr, checkDev))
							goto ACTIVATE_NEW_AUDIO_DEV;
					}
					break;

				case AUDIO_DEVICE_MUSIC:
				case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
				case AUDIO_DEVICE_AVRCP_PLAY:
					for(j = startLinkInd; j < startLinkInd + FW_MAX_LINK_IDX; j++)
					{
						BD_ADDR_T *pOtherBdAddr = APP_GetBdAddrByLinkIndex(j % FW_MAX_LINK_IDX);

						if(pOtherBdAddr && app_SetAudioDeviceMusicResume(pOtherBdAddr, checkDev))
							goto ACTIVATE_NEW_AUDIO_DEV;
					}
					break;

				case AUDIO_DEVICE_LINE_IN:
					if(app_SetAudioDeviceSys(AUDIO_DEVICE_LINE_IN))
						goto ACTIVATE_NEW_AUDIO_DEV;
					break;

				case AUDIO_DEVICE_FM:
					if(app_SetAudioDeviceSys(AUDIO_DEVICE_FM))
						goto ACTIVATE_NEW_AUDIO_DEV;
					break;

				#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
				case AUDIO_DEVICE_MP3:
					if(app_SetAudioDeviceSys(AUDIO_DEVICE_MP3))
						goto ACTIVATE_NEW_AUDIO_DEV;
					break;
				#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

				case AUDIO_DEVICE_MIC_TO_SPK:
					if(app_SetAudioDeviceSys(AUDIO_DEVICE_MIC_TO_SPK))
						goto ACTIVATE_NEW_AUDIO_DEV;
					break;
			}
		}
	}

	APP_AudioReg_SetCurrentAudioDevice(NULL, AUDIO_DEVICE_NONE);


	ACTIVATE_NEW_AUDIO_DEV:

	pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	currentActiveDev = APP_GetCurrentAudioDevice();

	resumeTime = CHECK_AUDIO_TIME;

    DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] New Resume Other[%d]:BdAddr:0x%x%x", 3, audioDev, FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
    DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] New Current[%d]:BdAddr:0x%x%x", 3, currentActiveDev, FW_bdaddr_to_2U32(pCurrentActiveLink, TRUE), FW_bdaddr_to_2U32(pCurrentActiveLink, FALSE));

	switch(audioDev)
	{
		case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
        case AUDIO_DEVICE_SCO_HSP:
#endif            
			if(FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) && (currentActiveDev == AUDIO_DEVICE_CALL_STATE))
			{
                if(APP_IsAudioDeviceRegistered(pBdAddr, AUDIO_DEVICE_SCO_IDLE))
                {
                    //same type of device--> set active directly
                    resumeTime = 0L;
                    break;
                }
			}
            /* fallthrough */
		case AUDIO_DEVICE_SCO_IDLE:
			if(FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) &&
			(currentActiveDev == AUDIO_DEVICE_SCO || currentActiveDev == AUDIO_DEVICE_SCO_IDLE || currentActiveDev == AUDIO_DEVICE_SCO_HSP))
			{
				//same type of device--> set active directly
			}
			else
			{
				FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
				FW_Memcpy(&audioDspPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
				audioDspPara.audioType = AUDIO_SCO;
				APP_AudioDspStop(&audioDspPara);
			}
			resumeTime = 0L;
			break;

		case AUDIO_DEVICE_KEY_AT_CMD:
			resumeTime = 1000L;
			break;
		case AUDIO_DEVICE_CALL_STATE:
			resumeTime = 100L;
			break;

		case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			resumeTime = 100L;
		case AUDIO_DEVICE_MUSIC:
		case AUDIO_DEVICE_AVRCP_PLAY:
			if(FW_CmpBdAddr(pCurrentActiveLink, pBdAddr) &&
			(currentActiveDev == AUDIO_DEVICE_MUSIC_CALL_NOTIFY || currentActiveDev == AUDIO_DEVICE_MUSIC || currentActiveDev == AUDIO_DEVICE_AVRCP_PLAY))
			{
				//same type of device--> set active directly
			}
			else
			{
				APP_A2dpDspStop(pBdAddr);
			}
			break;

		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case AUDIO_DEVICE_MP3:
			resumeTime = 100L;
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			audioDspPara.audioType = AUDIO_MP3;
			APP_AudioDspStop(&audioDspPara);
			break;
		#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL

		case AUDIO_DEVICE_MIC_TO_SPK:
			resumeTime = 0;
			break;

		default:
			resumeTime = CHECK_AUDIO_TIME;
			break;
	}

	FW_SetTimer((Handler)&gAppAudioResumeHandle, AUDIO_TIMER_RESUME_SOUND, NULL, 0, resumeTime);
}

BOOL APP_IsDSPInSCO(BD_ADDR_T *pBdAddr)
{
	switch(APP_GetCurrentAudioDevice())
	{
		case AUDIO_DEVICE_SCO:
		case AUDIO_DEVICE_SCO_IDLE:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
		case AUDIO_DEVICE_SCO_HSP:
#endif            
			if(FW_CmpBdAddr(APP_GetCurrentAudioBdAddr(), pBdAddr))
			{
				return TRUE;
			}
			break;
		default:
			break;
	}
	return FALSE;
}

BOOL APP_IsDSPActive(BD_ADDR_T *pBdAddr)
{
	if(FW_CmpBdAddr(APP_GetCurrentAudioBdAddr(), pBdAddr))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL APP_IsLinkInHighAudioPriority(BD_ADDR_T *pBdAddr)
{
	U8 currAudioDevice = APP_GetCurrentAudioDevice();

	UNUSED(pBdAddr);

<<<<<<< HEAD
=======
	 DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] IsLinkInHighAudioPriority, curr audio device:%d", 1, currAudioDevice);
>>>>>>> db20e11 (second commit)
	if(currAudioDevice == AUDIO_DEVICE_NONE)
		return TRUE;

	if(currAudioDevice >= AUDIO_DEVICE_SYS_START && currAudioDevice <= AUDIO_DEVICE_SYS_END)
		return TRUE;

	return FALSE;
}

void APP_AudioDeviceStopActiveDevice(void)
{
	BD_ADDR_T *pCurrentActiveLink = APP_GetCurrentAudioBdAddr();
	U8 currentActiveDev = APP_GetCurrentAudioDevice();
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	switch(currentActiveDev)
	{
		case AUDIO_DEVICE_FM:
			if(!APP_State_CheckNestStateByLink(APP_EOF, APP_FM))
				APP_State_AddTopState(APP_EOF, APP_FM);
			break;

		case AUDIO_DEVICE_LINE_IN:
		case AUDIO_DEVICE_MIC_TO_SPK:
			//APP_AudioDspStop(NULL, AUDIO_AUX);
			break;

#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case AUDIO_DEVICE_MP3:
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			audioDspPara.audioType = AUDIO_MP3;
			APP_AudioDspStop(&audioDspPara);
			break;
#endif//MP3_LOCAL_PLAYBACK_MMI_CTRL
		case AUDIO_DEVICE_MUSIC:
		case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
		case AUDIO_DEVICE_AVRCP_PLAY:
			APP_A2dpDspStop(pCurrentActiveLink);
			break;

		case AUDIO_DEVICE_SCO:
		case AUDIO_DEVICE_SCO_IDLE:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
		case AUDIO_DEVICE_SCO_HSP:
#endif            
		case AUDIO_DEVICE_KEY_AT_CMD:
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			FW_Memcpy(&audioDspPara.bdAddr, pCurrentActiveLink, sizeof(BD_ADDR_T));
			audioDspPara.audioType = AUDIO_SCO;
			APP_AudioDspStop(&audioDspPara);
			break;

		case AUDIO_DEVICE_NONE:
		case AUDIO_DEVICE_CALL_STATE:
			break;

        case AUDIO_DEVICE_AMA:
            DBG_LOG_APP_DSP_CONTROL("APP_AudioDeviceStopActiveDevice AMA", 0);
            break;
		default:
			FW_Assert(FALSE);
			break;
	}
}

void APP_AudioDeviceDeRegisterAllDevice(void)
{
	AudioDsp_ANCPassThruPowerOffHandler();
	APP_AudioDeviceStopActiveDevice();
	APP_AudioReg_SetCurrentAudioDevice(NULL, AUDIO_DEVICE_NONE);
	gAppAudioDevCtl.audioDevSysMask = 0;
	APP_ClearAllLinksAudioDevLinkMask();
	FW_ReleaseTimer((Handler)&gAppAudioResumeHandle, AUDIO_TIMER_RESUME_SOUND, 0);
}

U8 APP_IsActiveAudioDevInAUX(void)
{
	return (APP_GetCurrentAudioDevice() == AUDIO_DEVICE_LINE_IN) ? TRUE : FALSE;
}

U8 APP_IsActiveAudioDevInCall(void)
{
	switch(APP_GetCurrentAudioDevice())
	{
		case AUDIO_DEVICE_SCO:
<<<<<<< HEAD
#ifdef PROFILE_HEADSET_ENABLE            
=======
#ifdef AIR_HEADSET_PROFILE_ENABLE            
>>>>>>> db20e11 (second commit)
		case AUDIO_DEVICE_SCO_HSP:
#endif            
		case AUDIO_DEVICE_SCO_IDLE:
		case AUDIO_DEVICE_CALL_STATE:
		case AUDIO_DEVICE_MUSIC_CALL_NOTIFY:
			return TRUE;
		default:
			return FALSE;
	}
}

U8 APP_IsActiveAudioDevInMusic(void)
{
	switch(APP_GetCurrentAudioDevice())
	{
		case AUDIO_DEVICE_MUSIC:
		case AUDIO_DEVICE_AVRCP_PLAY:
			return TRUE;
		default:
			return FALSE;
	}
}

void APP_AudioDevSetResumeTimer(U32 time)
{
	FW_SetTimer((Handler)&gAppAudioResumeHandle, AUDIO_TIMER_RESUME_SOUND, NULL, 0, time);
}

BOOL APP_AudioDev_IsHighPriDevRegisted(U8 audioDev)
{
	U8 priIn = app_AudioRegGetAudioPriority(audioDev);
	U8 priCur = app_AudioRegGetAudioPriority(APP_GetCurrentAudioDevice());

	if(APP_GetCurrentAudioDevice() == AUDIO_DEVICE_NONE)
		return FALSE;

	priIn = app_AudioRegGetAudioPriority(audioDev);
	priCur = app_AudioRegGetAudioPriority(APP_GetCurrentAudioDevice());

	return (priCur > priIn) ? TRUE : FALSE;
}


BOOL APP_DspDevReg_FillRhoData(void *pData)
{
	APP_RHO_DSP_DEV_REG_STRU *pDataToFill = &((APP_RHO_STREAM_MANAGE_STRU *)pData)->appDevReg;

	if(!pDataToFill)
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Fill RHO data error", 0);
		return FALSE;
	}

	FW_Memcpy(&pDataToFill->currentBdAddr, &gAppAudioDevCtl.currentBdAddr, sizeof(BD_ADDR_T));
	pDataToFill->currentAudioDev = gAppAudioDevCtl.currentAudioDev;
	FW_Memcpy(&pDataToFill->audioDevSysMask, &gAppAudioDevCtl.audioDevSysMask, sizeof(gAppAudioDevCtl.audioDevSysMask));

	return TRUE;
}

BOOL APP_DspDevReg_AssignRhoData(void *pData)
{
	APP_RHO_DSP_DEV_REG_STRU *pDataToGet = &((APP_RHO_STREAM_MANAGE_STRU *)pData)->appDevReg;

	if(!pDataToGet)
	{
		DBG_LOG_APP_DSP_CONTROL( "[Audio Reg] Assign RHO data error", 0);
		return FALSE;
	}

	FW_Memcpy(&gAppAudioDevCtl.currentBdAddr, &pDataToGet->currentBdAddr, sizeof(BD_ADDR_T));
	gAppAudioDevCtl.currentAudioDev = pDataToGet->currentAudioDev;
	FW_Memcpy(&gAppAudioDevCtl.audioDevSysMask, &pDataToGet->audioDevSysMask, sizeof(U16));

	return TRUE;
}


