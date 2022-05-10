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
#include "App_ResourceManager.h"
#include "AudioDSP_StreamManager.h"
#include "APP_AudioDriver.h"
#include "drv_rt.h"
#include "drv_vp.h"

#include "App_Media.h"
#include "App_VolumeManage.h"
#include "Volume_NvkeyHandler.h"
#include "FW_Api.h"
#include "syslog.h"
#if LAB_TUNNING_APP
#include "drv_recorder.h"
#endif

#define VP_RT_INVALID_ID          0xFF

log_create_module(APP_AUDIO_DRIVER, PRINT_LEVEL_INFO);

static U32 app_AudioDriver_SubSinkHandler(Handler handler, U16 id, void *msg, U32 handler_id);

APP_DRIVER_SUBSINK_HDL_STRU	gAudioDriver_SubSink_Hdl;

static const HandlerData gAudioDriverSubSinkHdl = { app_AudioDriver_SubSinkHandler };

static void app_AudioDriver_SubSinkRemoveQBuf(void)
{
	U8 cmdIndex;

	if(gAudioDriver_SubSink_Hdl.qCmdNum == 0)
		return;

	gAudioDriver_SubSink_Hdl.qCmdNum--;
	for(cmdIndex = 0; cmdIndex < gAudioDriver_SubSink_Hdl.qCmdNum; cmdIndex++)
	{
		gAudioDriver_SubSink_Hdl.qCmd[cmdIndex] = gAudioDriver_SubSink_Hdl.qCmd[cmdIndex + 1];
	}
	FW_Memset(&gAudioDriver_SubSink_Hdl.qCmd[cmdIndex], 0, sizeof(APP_DRIVER_SUBSINK_Q_STRU));
}


static void app_VPRTCtrl_LogPrint(BOOL isStart)
{
	U8 cmdType = (U8)gAudioDriver_SubSink_Hdl.qCmd[0].cmdType;
	U16 id = (U16)gAudioDriver_SubSink_Hdl.qCmd[0].id;
	U16 srcIndex = (U16)gAudioDriver_SubSink_Hdl.qCmd[0].srcIndex;

	DBG_LOG_APP_AUDIO_DRIVER("[Media] QcmdType<0x%x>, QcmdId<0x%x>, QcmdSrcIdx<0x%x>", 3, cmdType, id, srcIndex);


	if(id <= MEDIA_EVT_KEY_INTERNAL_TOTAL_NO)//Media evtKey
	{
		if(isStart)
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Start]: MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent1LogString[id], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
		else
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Stop]:  MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent1LogString[id], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
	}
	else if((MEDIA_EVT_BAT_LOW <= id) && (id <= MEDIA_EVT_NUMBER))
	{
		if(isStart)
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Start]: MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent2LogString[id - MEDIA_EVT_SYSTEM_BASE], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
		else
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Stop]:  MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent2LogString[id - MEDIA_EVT_SYSTEM_BASE], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
	}
	else if((MEDIA_EVT_SLC_CONNECTED <= id) && (id < MEDIA_EVT_UART_CMD_RESERVE))
	{
		if(isStart)
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Start]: MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent3LogString[id - MEDIA_EVT_CONNECTION_BASE], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
		else
			DBG_LOG_APP_AUDIO_DRIVER("[Media][%s Stop]:  MediaEvent: %s, %s", 3, APP_AudioTypeLogString[cmdType], APP_MediaEvent3LogString[id - MEDIA_EVT_CONNECTION_BASE], (cmdType == AUDIO_VP) ? APP_RTLogString[srcIndex] : APP_VPLogString[srcIndex]);
	}
	else
	{
	}
}

static U32 app_AudioDriver_SubSinkHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);
	AUDIO_DSP_START_PARA_STUR audioDspPara;

	switch(id)
	{
        case DRIVER_SUBSINK_STATE_END:
            PLAY_EVT_END:
            switch(gAudioDriver_SubSink_Hdl.qCmd[0].cmdType)
            {
                case AUDIO_VP:
                case AUDIO_RT:
                    app_VPRTCtrl_LogPrint(FALSE);
                    app_AudioDriver_SubSinkRemoveQBuf();
                    gAudioDriver_SubSink_Hdl.subSinkState = DRIVER_SUBSINK_STATE_END;
                    break;
            }
        /* fallthrough */
		case DRIVER_SUBSINK_STATE_START:
			SEND_SUBSINK_CMD:
			if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_END && gAudioDriver_SubSink_Hdl.qCmdNum > 0)
			{
				if(!APP_AudioDspIsStreaming(NULL, AUDIO_RT) && !APP_AudioDspIsStreaming(NULL, AUDIO_VP))
				{
					switch(gAudioDriver_SubSink_Hdl.qCmd[0].cmdType)
					{
						case AUDIO_VP:
						case AUDIO_RT:
							gAudioDriver_SubSink_Hdl.subSinkState = DRIVER_SUBSINK_STATE_START;
							app_VPRTCtrl_LogPrint(TRUE);
							FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
							audioDspPara.audioType = gAudioDriver_SubSink_Hdl.qCmd[0].cmdType;
							audioDspPara.codec = gAudioDriver_SubSink_Hdl.qCmd[0].srcIndex;
							if(gAudioDriver_SubSink_Hdl.qCmd[0].srcIndex == VP_RT_INVALID_ID || !APP_AudioDspPlay(&audioDspPara)) {                     
								goto PLAY_EVT_END;
                            }
							APP_VolManage_SpkVolumeRemoteHandler(NULL, APP_VolManage_GetCurrentVpRtSoundLevel(), COMPONENT_VPRT);
							break;
						case AUDIO_FAKE_EVT:
							MSG_MessageSendEx(gAudioDriver_SubSink_Hdl.callBackHdl, gAudioDriver_SubSink_Hdl.callBackMsgID, NULL, gAudioDriver_SubSink_Hdl.qCmd[0].id);
							app_AudioDriver_SubSinkRemoveQBuf();
							goto SEND_SUBSINK_CMD;
							break;
						case AUDIO_INVALID:
							app_AudioDriver_SubSinkRemoveQBuf();
							goto SEND_SUBSINK_CMD;
							break;
					}
				}
				else
				{
					FW_SetTimer((Handler)&gAudioDriverSubSinkHdl, DRIVER_SUBSINK_STATE_START, NULL, 0, 10);
				}
			}
			break;

		case VP_END_OF_PLAY:
		case RT_END_OF_PLAY:
			goto PLAY_EVT_END;
			break;
	}

	return 0;
}

BOOL APP_AudioDriver_SendSubSinkCmd(U8 cmdType, U16 srcIndex, U16 id)
{
	if(cmdType != AUDIO_VP && cmdType != AUDIO_RT && cmdType != AUDIO_FAKE_EVT)
		return FALSE;

	if(gAudioDriver_SubSink_Hdl.qCmdNum < DRIVER_SUBSINK_Q_MAX)
	{
		gAudioDriver_SubSink_Hdl.qCmd[gAudioDriver_SubSink_Hdl.qCmdNum].cmdType = cmdType;
		gAudioDriver_SubSink_Hdl.qCmd[gAudioDriver_SubSink_Hdl.qCmdNum].srcIndex = srcIndex;
		gAudioDriver_SubSink_Hdl.qCmd[gAudioDriver_SubSink_Hdl.qCmdNum++].id = id;
		FW_MessageSend((Handler)&gAudioDriverSubSinkHdl, DRIVER_SUBSINK_STATE_START, NULL);
		return TRUE;
	}
	return FALSE;
}

void APP_AudioDriver_SubSinkClear(void)
{
	FW_Memset(&gAudioDriver_SubSink_Hdl, 0, sizeof(APP_DRIVER_SUBSINK_HDL_STRU));
}

U8 APP_AudioDriver_GetSubSinkQNum(void)
{
	return gAudioDriver_SubSink_Hdl.qCmdNum;
}

void APP_AudioDriver_Init(Handler msgHdl, U16 msgID)
{
	APP_AudioDriver_SubSinkClear();

	gAudioDriver_SubSink_Hdl.callBackHdl = msgHdl;
	gAudioDriver_SubSink_Hdl.callBackMsgID = msgID;
	drv_vp_register_end_handler((Handler)&gAudioDriverSubSinkHdl);
	drv_rt_register_end_handler((Handler)&gAudioDriverSubSinkHdl);
}

static void app_AudioDriver_ClearSubSinkCmd(U8 cmdType, U16 id)
{
	U8 cmdIndex, queNum;
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	if(cmdType != AUDIO_VP && cmdType != AUDIO_RT)
		return;

	if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_START)
	{
		if(gAudioDriver_SubSink_Hdl.qCmd[0].cmdType == cmdType && gAudioDriver_SubSink_Hdl.qCmd[0].id == id)
		{
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			audioDspPara.audioType = cmdType;
			APP_AudioDspStop(&audioDspPara);
		}
	}

	if(gAudioDriver_SubSink_Hdl.qCmdNum < 2)
		return;

	queNum = gAudioDriver_SubSink_Hdl.qCmdNum;
	for(cmdIndex = 1; cmdIndex < gAudioDriver_SubSink_Hdl.qCmdNum; cmdIndex++)
	{
		if(gAudioDriver_SubSink_Hdl.qCmd[cmdIndex].cmdType == cmdType && gAudioDriver_SubSink_Hdl.qCmd[cmdIndex].id == id)
		{
			gAudioDriver_SubSink_Hdl.qCmd[cmdIndex].cmdType = AUDIO_INVALID;
		}
	}
}

void APP_AudioDriver_ClearSubSinkCmd(U16 id)
{
	app_AudioDriver_ClearSubSinkCmd(AUDIO_RT, id);
	app_AudioDriver_ClearSubSinkCmd(AUDIO_VP, id);
}

<<<<<<< HEAD
=======
void APP_AudioDriver_ClearAllSubSinkCmd(void)
{
	U8 cmdIndex;
	U16 id = NO_MEDIA;
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;

	if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_START)
	{
		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
		audioDspPara.audioType = gAudioDriver_SubSink_Hdl.qCmd[0].cmdType;
		APP_AudioDspStop(&audioDspPara);

		id = gAudioDriver_SubSink_Hdl.qCmd[0].id;
	}

	if(gAudioDriver_SubSink_Hdl.qCmdNum < 2)
		return;

	for(cmdIndex = 1; cmdIndex < gAudioDriver_SubSink_Hdl.qCmdNum; cmdIndex++)
	{
		gAudioDriver_SubSink_Hdl.qCmd[cmdIndex].cmdType = AUDIO_INVALID;
	}

	//have to send a fake media event to APP Media to notify the media event was stopped
	if(id != NO_MEDIA)
		APP_AudioDriver_SendSubSinkCmd(AUDIO_FAKE_EVT, 0, id);
}

>>>>>>> db20e11 (second commit)
void APP_AudioDriver_SpecificEventVPStopCmd(U16 id)
{
	app_AudioDriver_ClearSubSinkCmd(AUDIO_VP, id);
}

void APP_AudioDriver_SpecificEventRTStopCmd(U16 id)
{
	app_AudioDriver_ClearSubSinkCmd(AUDIO_RT, id);
}

void APP_AudioDriver_SendVoicepromptStopCmd(void)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	
	if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_START)
	{
		if(gAudioDriver_SubSink_Hdl.qCmd[0].cmdType == AUDIO_VP)
		{
			DBG_LOG_APP_AUDIO_DRIVER("[    APP][AudioDriver]VoicepromptStop:%x", 1, gAudioDriver_SubSink_Hdl.qCmd[0].id);
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			audioDspPara.audioType = AUDIO_VP;
			APP_AudioDspStop(&audioDspPara);
		}
	}
}

void APP_AudioDriver_SendRingtoneStopCmd(void)
{
	AUDIO_DSP_STOP_PARA_STUR audioDspPara;
	
	if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_START)
	{
		if(gAudioDriver_SubSink_Hdl.qCmd[0].cmdType == AUDIO_RT)
		{
			DBG_LOG_APP_AUDIO_DRIVER("[    APP][AudioDriver]RingtoneStop:%x", 1, gAudioDriver_SubSink_Hdl.qCmd[0].id);
			FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
			audioDspPara.audioType = AUDIO_RT;
			APP_AudioDspStop(&audioDspPara);
		}
	}
}

void APP_AudioDriver_SendRingtoneCmd(U16 id, U16 srcIndex)
{
	APP_AudioDriver_SendSubSinkCmd(AUDIO_RT, srcIndex, id);
}

U16 APP_AudioDriver_GetPlayingID(void)
{
    U16 id = MEDIA_EVT_INVALID;
    if(gAudioDriver_SubSink_Hdl.subSinkState == DRIVER_SUBSINK_STATE_START)
    {
        id = gAudioDriver_SubSink_Hdl.qCmd[0].id;
    }
    return id;
}

