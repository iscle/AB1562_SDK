/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
#include "drv_encoder.h"
#include "AudioDSP_StreamManager.h"
#include "APP_Encoder.h"

enum
{
	APP_ENCODER_FRAME_16K_SIZE = 40,
	APP_ENCODER_FRAME_32K_SIZE = 80,
	APP_ENCODER_FRAME_64K_SIZE = 160,
};

static U32 app_encoder_handler(Handler handler, U16 id, void *msg, U32 handler_id);
static const HandlerData gAppEncoderHandle = { app_encoder_handler };

static APP_ENCODER_HANDLER_STRU *gAppEncoderDataHandler;

//static U32 app_encoder_get_frame_size_by_bit_rate(void)
//{
    //U32 frameSize = 0;
    
    //switch(gDrvEncoderCtl.bitRate)
    //{
    //    case ENCODER_BITRATE_64KBPS:
    //        frameSize = APP_ENCODER_FRAME_64K_SIZE;
    //        break;

    //    case ENCODER_BITRATE_32KBPS:
    //        frameSize = APP_ENCODER_FRAME_32K_SIZE;
    //        break;

    //    case ENCODER_BITRATE_16KBPS:
    //        frameSize = APP_ENCODER_FRAME_16K_SIZE;
    //        break;
    //    default:
    //        break;
    //}
    //return frameSize;
    //return APP_ENCODER_FRAME_32K_SIZE;
//}

static void app_encoder_get_data(void)
{
    if(gAppEncoderDataHandler)
    {
        U32 frameSize = gAppEncoderDataHandler->frameSize;
        U32 dspBufsize = drv_encoder_get_data_size();

        if(dspBufsize >= frameSize)
        {
            U8 * ptr = malloc(frameSize);
            if(ptr)
            {
                for( ; dspBufsize >= frameSize ; dspBufsize -= frameSize)
                {
                    if(gAppEncoderDataHandler->AllocateMemory(frameSize))
                    {
                        drv_encoder_get_data_from_dsp(ptr, frameSize);
                        gAppEncoderDataHandler->DataHandler(ptr, frameSize);
                    }
                    else
                    {
                        break;
                    }
                }
                FW_FreeMemory(ptr);
            }
        }
    }
    else
    {
        U32 dspBufsize = drv_encoder_get_data_size();
        if(dspBufsize)
        {
            U8 * ptr = malloc(dspBufsize);
            if(ptr)
            {
                drv_encoder_get_data_from_dsp(ptr, dspBufsize);
                FW_FreeMemory(ptr);
            }
        }       
    }
#if 0 //bellow is example
    U32 dspBufsize, frameSize;
    U8 frameNum;
    U8 *pData;
    U8 index;
    
    frameSize = drv_encoder_get_frame_size_by_bit_rate();

    if(frameSize == 0)
    {
        return;
    }

    dspBufsize = drv_encoder_get_data_size();
    frameNum = dspBufsize /frameSize;

    for(index = 0; index < frameNum; index++)
    {
        pData = StreamSinkClaim(frameSize);

        if(pData)
        {
            drv_encoder_get_data_from_dsp(pData, frameSize);
            StreamSinkFlush();
        }
        else
        {
            break;
        }
        
    }
#endif
}

static U32 app_encoder_handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
    UNUSED(handler); 
    UNUSED(handler_id); 
    UNUSED(msg);
    switch(id)
    {
        case MSG_DATA_IN:
            app_encoder_get_data();
            break;

        default:
            break;
    }
    return 0;    
}

void APP_Encoder_Close(BD_ADDR_T * pBdAddr)
{
    AUDIO_DSP_STOP_PARA_STUR audioDspStopPara;
    FW_Memset(&audioDspStopPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
    FW_Memcpy(&audioDspStopPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
    audioDspStopPara.audioType = AUDIO_ENCODER;
    APP_AudioDspStop(&audioDspStopPara);        
}

void APP_Encoder_Open(BD_ADDR_T * pBdAddr)
{
    AUDIO_DSP_START_PARA_STUR audioDspStartPara;

    FW_Memset(&audioDspStartPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
    FW_Memcpy(&audioDspStartPara.bdAddr, pBdAddr, sizeof(BD_ADDR_T));
    audioDspStartPara.audioType = AUDIO_ENCODER;
    audioDspStartPara.bitRate = 32;
    APP_AudioDspPlay(&audioDspStartPara);     
}

void APP_Encoder_Init(void)
{
    drv_encoder_init((Handler)&gAppEncoderHandle);
}

void APP_Encoder_RegisterDataHandler(APP_ENCODER_HANDLER_STRU *handler)
{
    if(handler->AllocateMemory && handler->DataHandler && handler->frameSize)
        gAppEncoderDataHandler = handler;
}
