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
#include "AudioDSP_AncPassThru.h"
#include "drv_sidetone.h"
#ifdef LINEIN_ENABLE
#include "drv_line_in.h"
#endif
#include "drv_anc.h"
#include "DSP_ANC_Control.h"
#include "AudioDSP_StreamManager.h"


U8 gUserRegister;
U8 gKeyTriggered = 0;

static void APP_ANCPassThru_Close(void)
{
    AUDIO_DSP_STOP_PARA_STUR audioDspStopPara;
    FW_Memset(&audioDspStopPara, 0, sizeof(AUDIO_DSP_STOP_PARA_STUR));
    audioDspStopPara.audioType = AUDIO_ANC;
    APP_AudioDspStop(&audioDspStopPara);
}

static void APP_ANCPassThru_UserClose(void)
{
    AudioDsp_SetANCPassThruRegister(USER_ANC_PASSTHRU_CLOSE);
    APP_ANCPassThru_Close();
}

static void APP_ANCPassThru_Open(void)
{
    AUDIO_DSP_START_PARA_STUR audioDspStartPara;

    if(!AudioDsp_IsUserANCPassThruRegister())
    {
        DBG_LOG_APP_DSP_CONTROL( "[APP][DSP] AiroThru Start Fail", 0);
        return;
    }

    FW_Memset(&audioDspStartPara, 0, sizeof(AUDIO_DSP_START_PARA_STUR));
    audioDspStartPara.audioType = AUDIO_ANC;
    APP_AudioDspPlay(&audioDspStartPara);
}

static void APP_ANCPassThru_UserOpen(APP_ANCPASSTHRU_PARA_STRU para)
{
    AudioDsp_SetANCPassThruRegister(para.userRegister);
    drv_anc_set_para(para.flash_no, para.type, para.runtime_gain);
    APP_ANCPassThru_Open();
}

/******************************************************************************
 * Public Function
*******************************************************************************/
void AudioDsp_SetANCPassThruRegister(U8 userRegister)
{
    gUserRegister = userRegister;
}

U8 AudioDsp_GetANCPassThruRegister(void)
{
    return gUserRegister;
}

BOOL AudioDsp_IsUserANCPassThruRegister(void)
{
    return (gUserRegister == USER_ANC_PASSTHRU_CLOSE) ? FALSE : TRUE;
}

void AudioDsp_ANCPassThruOnByNVKeySettings(void)
{
    U8 enable;
    APP_ANCPASSTHRU_PARA_STRU para;

    audio_anc_passthru_get_status_from_nvkey(&enable, (anc_filter_type_t *)&para.flash_no, (hal_audio_anc_type_t *)&para.type, &para.runtime_gain);

    if(enable)
    {
        para.action = ACTION_ON;
        para.sync = FALSE;
        if(para.type == HAL_AUDIO_PASSTHRU_TYPE_FF)
            para.userRegister = USER_PASSTHRU_OPEN;
        else if(para.type < HAL_AUDIO_PASSTHRU_TYPE_FF)
            para.userRegister = USER_ANC_OPEN;
        else
            return;
        APP_ANCPassThruHandler(para);
    }
}

void AudioDsp_ANCPassThruPowerOffHandler(void)
{
    //if(gKeyTriggered) //BTA-7972
        audio_anc_passthru_save_status_into_nvkey();

    gKeyTriggered = FALSE;
    if(drv_anc_is_playing())
    {
        APP_ANCPassThru_Close();
    }
}

U8 APP_ANCPassThruHandler(APP_ANCPASSTHRU_PARA_STRU para)
{
    U8 status = 0;
    U8 userReg = AudioDsp_GetANCPassThruRegister();

    switch(para.action)
    {
        case ACTION_OFF:
            APP_ANCPassThru_UserClose();
            break;
        case ACTION_ON:
            APP_ANCPassThru_UserOpen(para);
            break;
        case ACTION_TOGGLE:
            switch(userReg)
            {
                case USER_ANC_PASSTHRU_CLOSE:
                    APP_ANCPassThru_UserOpen(para);
                    break;
                case USER_ANC_OPEN:
                case USER_PASSTHRU_OPEN:
                    if(para.userRegister != userReg)
                    {
                        APP_ANCPassThru_UserOpen(para);
                    }
                    else
                    {
                        APP_ANCPassThru_UserClose();
                    }
                    break;
            }
            break;
        case ACTION_SET_VOL:
            drv_anc_set_para(ANC_PARA_FLASH_NO_CHANGE, ANC_PARA_TYPE_NO_CHANGE, para.runtime_gain);
            status = audio_anc_passthru_set_runtime_gain(para.runtime_gain);
            break;
    }

    return status;
}

void AudioDsp_ANCPassThruKeyTriggered(void)
{
    gKeyTriggered = TRUE;
}

