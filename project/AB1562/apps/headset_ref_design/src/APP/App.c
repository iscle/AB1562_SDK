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
#include "App_RaceCmd.h"
#include "App_System.h"
#include "App_Nvram_Type.h"
#include "App_A2dp.h"
#include "App_Avrcp.h"
#include "App_Media.h"
#include "App_LedControl.h"
#include "APP_HfpState.h"
#include "App_State.h"
#include "App_ChargeBattery.h"
#include "App_LinkKey.h"
#include "App_Pairing.h"
#include "App_VolumeManage.h"
#include "App_PowerOff.h"
#include "App_Connection_NVKeyHandler.h"
#include "App_Nvkey.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_FeatureManager.h"
#include "drv_audio_nvkey.h"
#include "APP_AiroThru.h"
#include "App_SniffHandler.h"
#include "App_PeqProcess.h"
#ifdef LINEIN_ENABLE
#include "App_LineIn.h"
#endif

#if AIROHA_BT_LE_ENABLE
#include "APP_CustApp_LE.h"
#endif
#ifdef PROFILE_AMA_ENABLE
#include "App_Ama.h"
#endif

#if NTC_DEVELOPMENT
#include "App_Ntc.h"
#endif

#ifdef AIROHA_BT_SPP_ENABLE
#include "APP_CustApp.h"
#endif

#include "App_GattServer.h"
#include "App_Ble.h"

#ifdef PROFILE_AMA_ENABLE
#include "BtIap2.h"
#endif


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_Init(void)
{
	APP_Nvkey_Init();
	APP_RaceCmdInit();
	#ifdef LINEIN_ENABLE
	APP_LineIn_Init();
	#endif
	APP_System_Init();
	APP_NvramType_Init();
	APP_A2dp_Init();
	APP_Avrcp_Init();
	APP_Media_Init();
	APP_LED_Init();
	APP_HFP_Init_HF();
	drv_audio_nvkey_init(PRODUCT_HEADSET);
	APP_ChgBat_Init();						//in WatchDog Reboot will use ChgBat, ChaBat Init before State Init
	APP_LinkKey_Init();
	APP_Peq_Init();							//must init after app linkKey APP_LinkKey_Init();
	APP_State_Init();
	APP_VolManage_Init();
	App_ConnNvkey_Init();
	APP_PowerOff_Init();
	APP_AudioDspInit();
	APP_AudioDsp_FeatureEntry_Init();
	APP_AiroThru_Init();

	APP_Battery_Init();

	#ifdef PROFILE_AMA_ENABLE
	APP_Ama_Init();
	#endif
	#ifdef AIROHA_BT_SPP_ENABLE
	APP_CustApp_Init();
	#endif

	#if NTC_DEVELOPMENT
	APP_NTC_Init();
	#endif

    APP_Ble_Init();
    // !!!ALWAYS the first GATT service!!!
    APP_GattServer_Init();

    #if AIROHA_BT_LE_ENABLE
    APP_CustApp_LE_Init();
    #endif

	#ifdef PROFILE_AMA_ENABLE
	BtIAP2NVKeyInit();
	#endif
}


