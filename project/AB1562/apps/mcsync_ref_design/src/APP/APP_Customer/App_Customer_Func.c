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
#include "App_Customer_Func.h"
#include "App_InEarDetection.h"
#include "App_Media.h"
#include "BtMCSync.h"
#include "drv_battery.h"
#include "App_MCSync.h"
#include "DSP_SDK.h"
#include "ChargerBatteryCaseNvkeyHandler.h"
#include "App_ChargeBattery.h"
#include "App_Battery.h"
#include "App_CustomerTimer.h"
#include "bt_gap.h"
#include "App_AEKey.h"
#include "App_PowerOn.h"
#include "App_PowerOff.h"
#include "Hfp_LinkManager.h"
#include "Avrcp_Interface.h"
#include "App_Avrcp.h"
#include "App_EventOpcode.h"
#include "nvkey_hdl.h"
#include "App_MCSync_Message.h"
#include "App_EventKeyHandler.h"
#include "DeviceID_CustomerSdpHandler.h"
#include "App_EventKeyHandler.h"
#include "DeviceID_CustomerSdpHandler.h"
#include "App_EventKeyHandler.h"
#include "App_SystemKey.h"
#include "App_ResourceManager.h"
#include "PM_Nvkey.h"
#include "MCSync_AirohaKeyHandler.h"
#include "AudioDSP_AncPassThru.h"
#include "DSP_ANC_Control.h"
#include "InEarDetection_NvkeyHandler.h"
<<<<<<< HEAD
#include "gfps_api.h"
#ifdef PROFILE_SWIFT_ENABLE
=======
#ifdef AIR_SWIFT_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_Swift.h"
#endif
/**************************************************************************************************
* Variable
**************************************************************************************************/
<<<<<<< HEAD
static U8 byLidOnCnt = 0;
=======
>>>>>>> db20e11 (second commit)

/**************************************************************************************************
* Static Function (Handler)
**************************************************************************************************/

/**************************************************************************************************
* Public Function
**************************************************************************************************/

void APP_Customer_SetInEarTone(U8 status)
{
	UNUSED(status);
}

BOOL APP_Customer_HandleSetPublicAddrCfm(void)
{
	return FALSE;
}

BOOL APP_Customer_PartnerSyncAirohaKey(AWSMCE_SYNC_KEY_ACTION_IND_T *ind)
{
	UNUSED(ind);
	return FALSE;
}

BOOL APP_Customer_IsInEarEnable(void)
{
	return TRUE;
}

void APP_Customer_SetInEarEnableFlag(U8 isEnable)
{
	UNUSED(isEnable);
	InEarDetection_NvKey_SetFeatureEnable(TRUE);
}

U8 APP_Customer_UpdateBatStatus_Adv(U8 batStatus)
{
	UNUSED(batStatus);
	return TRUE;
}

void APP_Customer_SetConnected_Control(void)
{
	
}

void APP_Customer_Timer2_Control(void)
{
	
}

void APP_Customer_Timer6_Control(void)
{
	
}

void APP_Customer_SetCloseAdvTimer(void)
{
	
}

void APP_Customer_SendSyncGeneralData(void)
{
	
}

U8 APP_Customer_Sync_Parnter_Data(U8 *pValue)
{
<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
    App_gfp_parnter_batt( pValue );
#else
	UNUSED(pValue);
#endif
=======
	UNUSED(pValue);
>>>>>>> db20e11 (second commit)
    return TRUE;
}

void APP_Customer_Rho_Done(void)
{
	
}

void APP_Customer_PowerOff_Handler(void)
{
	
}

U32 APP_Customer_LidOnBatteryHanler(U32 ext_id)
{
	UNUSED(ext_id);

<<<<<<< HEAD
#ifdef PROFILE_GFP_ENABLE
    if ( APP_PowerOff_IsPowerOn() )
    {
        if ( ++byLidOnCnt == 2 )
        {
            byLidOnCnt = 0;
            App_GfpsAdv_Update(0);
        }
    }
#endif	
    
=======
>>>>>>> db20e11 (second commit)
	return TRUE;
}

void APP_Customer_KeyIndex0Handler(void)
{
<<<<<<< HEAD

#ifdef PROFILE_GFP_ENABLE
    if ( PM_GetNormalLinkNum() == 0 )
        App_GfpsAdv_Update(1);
#endif	

#ifdef PROFILE_SWIFT_ENABLE
    App_SwiftAdv_Update();
#endif	
=======
#ifdef AIR_SWIFT_ENABLE
    App_SwiftAdv_Update();
#endif
>>>>>>> db20e11 (second commit)

}

void APP_Customer_KeyIndex1Handler(void)
{
	
}

BOOL APP_Customer_SmartKeyHandler(void)
{
	return FALSE;
}

void APP_Customer_SmartUsr1IndHandler(void)
{
<<<<<<< HEAD
    byLidOnCnt = 0; 
=======
>>>>>>> db20e11 (second commit)
}

void APP_Customer_MCSyncConntingHandler(void)
{
	
}

U8 APP_Customer_SetCloseAdv(void)
{
	return TRUE;
}

void app_Customer_SetServiceSecurity(void)
{

}

void APP_Customer_ConnectionNoiseControl(void)
{

}

BOOL APP_Customer_IsInEarDetectionEnable(void)
{
	return TRUE;
}

BOOL APP_Customer_IsVDHandledByGattVoice(BD_ADDR_T * pBdAddr, U8 action)
{
    UNUSED(pBdAddr);
    UNUSED(action);
    return FALSE;
}

void APP_Customer_SendSyncInfo(void)
{

}

void app_Customer_AirohaKey(BD_ADDR_T * pBdAddr)
{
	UNUSED(pBdAddr);
}

