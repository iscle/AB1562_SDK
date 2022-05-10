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
#include "AudioDSP_Vp.h"
#include "AudioDSP_StreamManager.h"


/**************************************************************************************************
* Variable
**************************************************************************************************/
static U16 gVpEvtIdx;

static U32 app_VpKeyHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static const HandlerData gAppVpKeyHandle = { app_VpKeyHandler };

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static U32 app_VpKeyHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	
	switch(id)
	{
		case VP_END_OF_PLAY:
			if(gVpEvtIdx != 0xFF)
			{
				if(!APP_AudioDspIsStreaming(NULL, AUDIO_VP))
				{
					FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
					audioDspPara.audioType = AUDIO_VP;
					audioDspPara.codec = gVpEvtIdx;
					if(gVpEvtIdx == VP_EVT_CALLER_ID || !APP_AudioDspPlay(&audioDspPara))
						MSG_MessageSendEx((Handler)&gAppVpKeyHandle, VP_END_OF_PLAY, NULL, 0);
					
					gVpEvtIdx++;
					
				}
				else
				{
					MSG_MessageSendEx((Handler)&gAppVpKeyHandle, VP_END_OF_PLAY, NULL, 1000);
				}
			}
			break;
	}
	
	return 0;
}

/**************************************************************************************************
* Public Functions (Key)
**************************************************************************************************/
void APP_Vp_KeyPlayAll()
{
	AUDIO_DSP_START_PARA_STUR audioDspPara;
	
	if(!APP_AudioDspIsStreaming(NULL, AUDIO_VP) && gVpEvtIdx == 0xFF)
	{
		gVpEvtIdx = 0;
		drv_vp_register_end_handler((Handler)&gAppVpKeyHandle);
		
		FW_Memset(&audioDspPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
		audioDspPara.audioType = AUDIO_VP;
		audioDspPara.codec = gVpEvtIdx;
		if(!APP_AudioDspPlay(&audioDspPara))
			MSG_MessageSendEx((Handler)&gAppVpKeyHandle, VP_END_OF_PLAY, NULL, 0);
		
		gVpEvtIdx++;
	}
}
