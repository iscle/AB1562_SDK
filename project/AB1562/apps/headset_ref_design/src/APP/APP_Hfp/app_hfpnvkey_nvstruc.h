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
 
 
#ifndef __APP_HFP_NVKEY__H__
#define __APP_HFP_NVKEY__H__

#include "nvkey.h"
#include "nvkey_list.h"


/******************************************************************************
* Definition
*******************************************************************************/
//#define NVKEY_ID_SCO_PKT_PARA 0x0007

//#define NVKEY_ID_PROFILE_APP_HFP_IMGCALLMEDIA 0x0015		Transfer to NVKEYID_APP_IMG_CALL_MEDIA_CONFIG
#include "app_hfpNvkey_imgcall_media_nvstruc.h"

#include "app_hfpnvkey_timer_nvstruc.h"

/******************************************************************************
* Enumeration
*******************************************************************************/
enum
{
	//default value in Nvkey
	TIMER_ID_NETWORK_SERVICE_INDICATION_REPEAT,				//NetworkServiceIndicatorRepeatTime;	//unit:   1s
	TIMER_ID_AUTO_ANSWER,									//autoAnswerTime;						//unit: 0.1s
	TIMER_ID_EST_SCO_AFTER_OGGCALL,							//estSCOAfterOggCallTime;				//unit: 0.1s
	TIMER_ID_AUDIO_TRANSFER_DETACH,							//AudioTransferDetachTime;				//unit: 0.5s
	//no default value	
	TIMER_ID_SEND_BAT_AT_CMD,								//sendBatteryAtCmdTimer;
	TIMER_ID_REJECT_CALL_MUTE_SCO,							//rejectCallMuteSCOTimer;
	TIMER_ID_WAIT_IMG_RMV_CALL_TRANSIT,						//waitCaImgRmvCallTransitTimer;			(Mantis 8867)
	TIMER_ID_AUDIO_DEV_ATCMD,								//audioDevATCmdTimer;
};

/******************************************************************************
* Structure
*******************************************************************************/
/**
 *@KeyID 0xF207
 */

/*NvkeyDefine NVKEYID_APP_HFP_SCO_PACKET_PARA*/
typedef struct APP_HFP_SCO_PKT_PARA_STRU 
{
	U16 SCOPktPara;			//0x03C7
	//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
	//to LM Version 1.1 by shift 5 bits to the left.
	//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
	//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
	//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
	//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
	//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
	//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
	//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
	//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
	//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
	//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used		
	U16 eSCOPktPara;		//0x003F
	//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
	//to LM Version 1.1 by shift 5 bits to the left.
	//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
	//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
	//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
	//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
	//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
	//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
	//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
	//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
	//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
	//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used
	U16 SCOOnlyHV3PktPara;	//0x03C4
	//Retrieve bits represents HV1, HV2 and HV3 of LM Version 2.0 and transform
	//to LM Version 1.1 by shift 5 bits to the left.
	//#define HCIPKTYPE_VER20_HV1 0x0001 //may be used
	//#define HCIPKTYPE_VER20_HV2 0x0002 //may be used
	//#define HCIPKTYPE_VER20_HV3 0x0004 //may be used
	//#define HCIPKTYPE_VER20_EV3 0x0008 //may be used
	//#define HCIPKTYPE_VER20_EV4 0x0010 //may be used
	//#define HCIPKTYPE_VER20_EV5 0x0020 //may be used
	//#define HCIPKTYPE_VER20_2EV3 0x0040 //may not be used
	//#define HCIPKTYPE_VER20_3EV3 0x0080 //may not be used
	//#define HCIPKTYPE_VER20_2EV5 0x0100 //may not be used
	//#define HCIPKTYPE_VER20_3EV5 0x0200 //may not be used
}PACKED APP_HFP_SCO_PKT_PARA_STRU;

BOOL App_HfpSCOPktPara_InitNvKey(void);
U16 App_Hfp_SCO_ReadNvkeyPktParaTypeSCO(void);
U16 App_Hfp_SCO_ReadNvkeyPktParaTypeeSCO(void);
U16 App_Hfp_SCO_ReadNvkeyPktParaTypeSCOHV3(void);

void APP_HfpNvkeyTimerInit();
U16 APP_HfpNvkeyGetTimerValue(U8 timerId);

	
#endif //__APP_HFP_NVKEY__H__
