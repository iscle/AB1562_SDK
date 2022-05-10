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
#include "AudioDSP_Vp.h"
#include "audiodsp_vpnvkey_nvstruc.h"
#include "nvkey_hdl.h"
#include "AudioDSP_StreamManager.h"
#include "bt_race_command.h"
#include "App_MCSync_Message.h"
#include "System_Nvkey.h"


/**************************************************************************************************
* Definition
**************************************************************************************************/

#define IS_VP_DISABLED				0x04


/*************************************************************************************************
* Enumeration
**************************************************************************************************/

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{

	U8 isMiscEnabled;

	U8 defaultLangId;
	U8 selectRound;
	U8 selectTime;
}APP_VP_CTL_STRU;

/**************************************************************************************************
* Variable
**************************************************************************************************/
APP_VP_CTL_STRU gAppVpCtl;

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_VpInit(void)
{
	VP_SEL_PARA_NVKEY_STRU *pPara = (VP_SEL_PARA_NVKEY_STRU *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_VP_SELECT_PARA);
    
    if(pPara)
    {
        gAppVpCtl.defaultLangId = pPara->langId;
        gAppVpCtl.selectRound = pPara->selectRound;
        gAppVpCtl.selectTime = pPara->selectTime;

        drv_vp_set_lang(pPara->langId);
        
        race_hostaudio_set_is_vp_on(APP_VpIsVoicePromptEnabled());
    }
    else
    {
        DBG_LOG_APP_DSP_CONTROL( "[VP] Init langID Fail", 0);
    } 
}

U8 APP_VpGetSelectRound()
{
	return gAppVpCtl.selectRound;
}

U8 APP_VpGetSelectTime()
{
	return gAppVpCtl.selectTime;
}

void APP_VpSetLanguage(U8 langId)
{
    if(drv_vp_set_lang(langId))
    {
        VP_SEL_PARA_NVKEY_STRU vpSelNvkey;

        vpSelNvkey.langId = langId;
        vpSelNvkey.selectRound = gAppVpCtl.selectRound;
        vpSelNvkey.selectTime = gAppVpCtl.selectTime;
        nvkey_hdl_save_nvkey(NVKEYID_APP_VP_SELECT_PARA, sizeof(VP_SEL_PARA_NVKEY_STRU), &vpSelNvkey);

		APP_MCSYNC_SendSyncVPLangIndex(APP_VpGetCurrentLangId());       
    }
}

void APP_VpSetLanguageNext()
{
	U8 langId;

	langId = (drv_vp_get_lang_id() + 1) % drv_vp_get_lang_cnt();
	APP_VpSetLanguage(langId);
}

void APP_VpSetLanguagePrev()
{
	U8 langId;

	if(drv_vp_get_lang_id() == 0)
		langId = drv_vp_get_lang_cnt() - 1;
	else
		langId = drv_vp_get_lang_id() - 1;
	APP_VpSetLanguage(langId);
}

void APP_VpSetLanguageToDefault()
{
	APP_VpSetLanguage(gAppVpCtl.defaultLangId);
}

U8 APP_VpGetLangCnt(void)
{
    return drv_vp_get_lang_cnt();
}

U8 APP_VpGetCurrentLangId(void)
{
    return drv_vp_get_lang_id();
}

void APP_VpEnableVoicePrompt(void)
{
//	gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled &= ~IS_VP_DISABLED;
	gAppVpCtl.isMiscEnabled &= ~IS_VP_DISABLED;
    race_hostaudio_set_is_vp_on(APP_VpIsVoicePromptEnabled());
//	MMI_SECTOR_SetUpdateFlag(TRUE);
}

void APP_VpDisableVoicePrompt(void)
{
//	gMMI_driver_variation_nvram.misc_para.init.isMiscEnabled |= IS_VP_DISABLED;
	gAppVpCtl.isMiscEnabled |= IS_VP_DISABLED;
    race_hostaudio_set_is_vp_on(APP_VpIsVoicePromptEnabled());
//	MMI_SECTOR_SetUpdateFlag(TRUE);
}

BOOL APP_VpIsVoicePromptEnabled(void)
{
	return (gAppVpCtl.isMiscEnabled & IS_VP_DISABLED) ? FALSE : TRUE;
}

U8 APP_VpGetMiscEnabled(void)
{
    return gAppVpCtl.isMiscEnabled;
}

void APP_VpSetMiscEnabled(U8 isMiscEnabled)
{
    gAppVpCtl.isMiscEnabled = isMiscEnabled;
    
    race_hostaudio_set_is_vp_on(APP_VpIsVoicePromptEnabled());
}

void APP_VpSetVpLangToFactoryValue(void)
{
	VP_SEL_PARA_NVKEY_STRU para;
	U8 vpLangFactoryValue = System_ReadNvkeyVpLangFactoryValue();

	DBG_LOG_APP_DSP_CONTROL( "[VP] Set to default language value:%d", 1, vpLangFactoryValue);

	para.langId = vpLangFactoryValue;
	para.selectRound = gAppVpCtl.selectRound;
	para.selectTime = gAppVpCtl.selectTime;

	nvkey_hdl_save_nvkey(NVKEYID_APP_VP_SELECT_PARA, sizeof(VP_SEL_PARA_NVKEY_STRU), &para);
}

