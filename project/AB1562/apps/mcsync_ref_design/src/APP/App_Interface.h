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


#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_

#include "App_Nvram_Type.h"
#include "Volume_NvkeyHandler.h"
#include "Pairing_NvkeyHandler.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
//#if USING_DIRECT_NVRAM_MISC_TYPE
	#define APP_DIS_PDL_LESS_THAN   	APP_PairNvkey_GetDiscoverableIfPdlLessThan()
	#define APP_MAX_PAGE_DEVICE 		APP_PairNvkey_GetMaxPageDevice()
	#define APP_CONNECTION 				APP_NvramType_FeturCtl_GetAppFeatureControl(connection)
	#define APP_PNP_PRODUCT_ID 			gAppNvramType.productType.pnpProductID
	#define APP_PNP_PRODUCT_VER 		gAppNvramType.productType.pnpProductVersion
	#define APP_AUDIO_PRIORITY 			gAppNvramType.audioPriority.priority
	#define APP_FEATURE_CTL_AUDIO3  	APP_NvramType_FeturCtl_GetAppFeatureControl(audio3)
	#define APP_AUX_FS_SEL 				gAppNvramType.airPara.AUX_FS_SEL
	#define APP_FEATURE_CTL_A2DP 		APP_NvramType_FeturCtl_GetAppFeatureControl(A2DP)
	#define APP_SIMPLE_PAIRING_PARA 	gAppNvramType.simplePairingPara
//#else
/*
	extern  U8 	  						gMMI_discoverableIfPdlLessThan;
	#define MMI_DIS_PDL_LESS_THAN 		gMMI_discoverableIfPdlLessThan
	extern  U8   						gMMI_maxPageDevice;
	#define MMI_MAX_PAGE_DEVICE 		gMMI_maxPageDevice
	extern  U16   						gMMI_connection;
	#define MMI_CONNECTION 				gMMI_connection
	extern  U16   						gMMI_pnp_product_id;
	#define MMI_PNP_PRODUCT_ID 			gMMI_pnp_product_id
	extern  U16   						gMMI_pnp_product_ver;
	#define MMI_PNP_PRODUCT_VER 		gMMI_pnp_product_ver
	extern U8  							gMMI_audio_priority[AUDIO_DEVICE_NUM];
	#define MMI_AUDIO_PRIORITY 			gMMI_audio_priority
	extern U16  						gMMI_feature_ctl_audio3;
	#define MMI_FEATURE_CTL_AUDIO3 		gMMI_feature_ctl_audio3
	extern U8  							gMMI_aux_fs_sel;
	#define MMI_AUX_FS_SEL 				gMMI_aux_fs_sel
	extern U16 		 					gMMI_feature_ctl_a2dp;
	#define MMI_FEATURE_CTL_A2DP 		gMMI_feature_ctl_a2dp
	extern SimplePairingPara  			gMMI_simple_pairing_para;
	#define MMI_SIMPLE_PAIRING_PARA 	gMMI_simple_pairing_para
*/
//#endif

#define APP_EOF						NULL
#define	INDEX_EOF					0xFF
#define DEFAULT_LINK_INDEX			0

#define VOLUME_DONT_CARE			0xFF

//APP AVRCP feature
#define APP_MUSIC_PLAY_PAUSE_ONEKEY_FEAT						(gAppAvrcpFeature.featureControlAppAvrcp & 0x01)
#define APP_KEY_PLAY_PAUSE_DONT_GET_PLAY_STATUS					(gAppAvrcpFeature.featureControlAppAvrcp & 0x02)

#define CURRENT_A2DP_MIC_SOUND_LEVEL(pBdAddr)					APP_GetAppLinkByBdAddr(pBdAddr)->linkPara.micSoundLevelA2DP

#define APP_GET_MAX_SOUND_LEVEL(component)						(APP_VolumeNvkey_GetTotalSoundLevel(component) - 1)
#define APP_MAX_SCO_SOUND_LEVEL									(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_SCO_WB) - 1)
#define APP_MAX_SCO_MIC_SOUND_LEVEL								(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_MIC_SCO) - 1)
#define APP_MAX_A2DP_SOUND_LEVEL								(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_A2DP) - 1)
#define APP_MAX_A2DP_MIC_SOUND_LEVEL							(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_MIC_A2DP) - 1)
#ifdef LINEIN_ENABLE
#define APP_MAX_LINE_IN_SOUND_LEVEL								(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_LINEIN) - 1)
#endif
#define APP_MAX_VPRT_SOUND_LEVEL								(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_VPRT) - 1)
#define APP_MAX_MP3_SOUND_LEVEL									(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_MP3) - 1)
#define APP_MAX_AIROTHRU_SOUND_LEVEL							(APP_VolumeNvkey_GetTotalSoundLevel(COMPONENT_AIROTHRU_SPK) - 1)

#define APP_DEFAULT_SCO_SOUND_LEVEL								APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_SCO_WB)
#define APP_DEFAULT_SCO_MIC_SOUND_LEVEL							APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_MIC_SCO)
#define APP_DEFAULT_A2DP_SOUND_LEVEL							APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_A2DP)
#define	APP_DEFAULT_A2DP_MIC_SOUND_LEVEL						APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_MIC_A2DP)
#ifdef LINEIN_ENABLE
#define	APP_DEFAULT_LINE_IN_SOUND_LEVEL							APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_LINEIN)
#endif
#define APP_DEFAULT_VPRT_SOUND_LEVEL							APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_VPRT)
#define APP_DEFAULT_MP3_SOUND_LEVEL								APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_MP3)
#define	APP_DEFAULT_AIROTHRU_SOUND_LEVEL						APP_VolumeNvkey_GetDefaultSoundLevel(COMPONENT_AIROTHRU_SPK)

#define APP_MUTE_CANCEL_WHEN_VOL_CHANGE_FEAT						(APP_FEATURE_CTL_AUDIO3 & 0x0010)
#define APP_ALLOW_VOL_ADJUST_WHEN_MUTED_FEAT						(APP_FEATURE_CTL_AUDIO3 & 0x0020)
#define APP_REMEMBER_SOUND_LEVEL_CHANGE_IN_RUNTIME_FEAT				(APP_FEATURE_CTL_AUDIO3 & 0x0800)


#define KEY_MAP_MMI_ALL_STATES 0xFFFFFFFF

#define	KEY_MAP_APP_OFF                         (U32)1<<APP_OFF
#define	KEY_MAP_APP_CONDISCABLE                 (U32)1<<APP_CONDISCABLE
#define	KEY_MAP_APP_CONNECTABLE                 (U32)1<<APP_CONNECTABLE
#define	KEY_MAP_APP_CONNECTED                   (U32)1<<APP_CONNECTED
#define	KEY_MAP_APP_HFP_INCOMMING               (U32)1<<APP_HFP_INCOMMING
#define	KEY_MAP_APP_HFP_OUTGOING                (U32)1<<APP_HFP_OUTGOING
#define	KEY_MAP_APP_HFP_CALLACTIVE              (U32)1<<APP_HFP_CALLACTIVE
#define	KEY_MAP_APP_HFP_CALLACTIVE_WITHOUT_SCO  (U32)1<<APP_HFP_CALLACTIVE_WITHOUT_SCO
#define	KEY_MAP_APP_HFP_CAIMG                   (U32)1<<APP_HFP_CAIMG
#define	KEY_MAP_APP_HFP_CAOGG                   (U32)1<<APP_HFP_CAOGG
#define	KEY_MAP_APP_HFP_CAMULTY                 (U32)1<<APP_HFP_CAMULTY
#define	KEY_MAP_APP_FAKEON                      (U32)1<<APP_FAKEON
#define	KEY_MAP_APP_FAKEOFF                     (U32)1<<APP_FAKEOFF
#define	KEY_MAP_APP_DETACHING_LINK              (U32)1<<APP_DETACHING_LINK
#define	KEY_MAP_APP_TEST_MODE                   (U32)1<<APP_TEST_MODE
#define	KEY_MAP_APP_FM	                        (U32)1<<APP_FM
#define	KEY_MAP_APP_LINE_IN	                    (U32)1<<APP_LINE_IN
#define	KEY_MAP_APP_EEPROM_UPDATING	            (U32)1<<APP_UPDATING
#define	KEY_MAP_APP_EEPROM_UPDATED_OK	        (U32)1<<APP_UPDATED_OK
#define	KEY_MAP_APP_EEPROM_UPDATED_FAIL	        (U32)1<<APP_UPDATED_FAIL
#define	KEY_MAP_APP_VOICE_PROMPT_LANG_SELECT	(U32)1<<APP_VOICE_PROMPT_LANG_SELECT
#define	KEY_MAP_APP_TWS_PAIRING					(U32)1<<APP_AWS_PAIRING
#define	KEY_MAP_APP_INQUIRY						(U32)1<<APP_INQUIRY
#define	KEY_MAP_MAPP_MP_STATE					(U32)1<<APP_MP_STATE
#define	KEY_MAP_APP_MP3							(U32)1<<APP_MP3
#define	KEY_MAP_APP_STATE_RESERVED2				(U32)1<<APP_STATE_RESERVED2
#define	KEY_MAP_APP_STATE_RESERVED3				(U32)1<<APP_STATE_RESERVED3
#define	KEY_MAP_APP_STATE_RESERVED4				(U32)1<<APP_STATE_RESERVED4
#define	KEY_MAP_APP_STATE_RESERVED5				(U32)1<<APP_STATE_RESERVED5
#define	KEY_MAP_APP_STATE_RESERVED6				(U32)1<<APP_STATE_RESERVED6
#define	KEY_MAP_APP_STATE_RESERVED7				(U32)1<<APP_STATE_RESERVED7

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	APP_NONE_SUBSTATE,

	APP_INIT_SYS_CONFIGED,

	APP_SBY_NONE_SUBSTATE = APP_INIT_SYS_CONFIGED,
	APP_VDIAL_STOP = APP_SBY_NONE_SUBSTATE,
	APP_VDIAL_CONNECTING_HFP,
	APP_VDIAL_STARTING,
	APP_VDIAL_STARTED,
	APP_VDIAL_STOPPING,

	APP_RDIAL_STARTING,
};




#endif //_APP_INTERFACE_H_
