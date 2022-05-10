/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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
#include "App_Fcd.h"
#include "App_ReConnection.h"
#include "App_State.h"
#include "App_Interface.h"
#include "App_Connection.h"
#include "App_Media.h"
#include "App_PowerOn.h"
#include "App_PowerOff.h"
#include "App_System.h"
#include "App_LinkKey.h"
#include "AudioDSP_Vp.h"
#include "System_Nvkey.h"
#include "App_Mcsync_Reconnection.h"
#include "AudioDSP_StreamManager.h"
#include "AudioDSP_Registration.h"
#include "syslog.h"
#include "BtMCSync.h"
#include "App_MCSync_Message.h"

log_create_module(APP_FCD, PRINT_LEVEL_INFO);

//**************************************************************************************************
//* Prototype
//**************************************************************************************************
static U32 app_Fcd_ResetTimerHandler(Handler app, U16 id, void *msg, U32 id_ext);

//**************************************************************************************************
//* Variable
//**************************************************************************************************/
static const HandlerData checkResetHandler = { app_Fcd_ResetTimerHandler };
APP_FCD_STRU gAppFcdCtl;

//**************************************************************************************************
//* Static Functions
//**************************************************************************************************

static void app_FCD_ResetPairedDevices(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	APP_ReConn_ClearCtl();
	APP_LinkKey_ClearAllHistory();
	APP_Conn_ReleaseMultiLink();
	APP_Conn_ReleaseAWSLink();
	APP_System_MpTestModeSetToDefault();

	FW_SetTimer((Handler)&checkResetHandler,0,NULL,0, 2*ONE_SEC );
}

static U32 app_Fcd_ResetTimerHandler(Handler app, U16 id, void *msg, U32 id_ext)
{
	UNUSED(app);
	UNUSED(id);
	UNUSED(msg);
	UNUSED(id_ext);

	APP_Reset();
	return 0;
}

//**************************************************************************************************
//* Public Functions
//**************************************************************************************************
void APP_FCD_ResetToFactory(U16 type)
{
	DBG_LOG_APP_FCD( "[APP_SystemKey] APP_FCD_ResetToFactory %d", 1, type);
	APP_MCSYNC_SendSyncFCDReset(type);

	gAppFcdCtl.isFcdReset = 1;
	gAppFcdCtl.reset_type = type;

	APP_AudioDspClose();
	APP_AudioDeviceDeRegisterAllDevice();

	if(type & FCD_RESET_PAIRDEVICE)
		app_FCD_ResetPairedDevices(NULL);

	else if(type & FCD_RESET_VPLANG)
		APP_VpSetLanguageToDefault();

	else if(type & FCD_RESET_DEFAULT_NVKEY)
		APP_System_SetToDefaultNvkey();

	else if(type & FCD_RESET_MCE_SETTING)
		BtAwsMce_WriteDefaultInfo(ROLE_NONE, NULL, NULL);

	else if(type & FCD_RESET_DEVICE)
	{
		if(!System_WriteNvkeyDeviceNameToDefault())
		{
			DBG_LOG_APP_FCD( "[APP_SystemKey] Reset device name successful", 0);
			APP_System_SetBtServiceClass();
		}
		else
		{
			DBG_LOG_APP_FCD( "[APP_SystemKey] Reset device name fail", 0);
		}
	}
}

BOOL APP_FCD_IsFcdReset(void)
{
	return gAppFcdCtl.isFcdReset;
}

