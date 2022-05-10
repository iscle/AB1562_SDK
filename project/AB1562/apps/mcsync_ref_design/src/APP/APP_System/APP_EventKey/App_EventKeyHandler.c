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
#include "App_EventOpcode.h"
#include "App_ResourceManager.h"
#include "App_KeyManage.h"
#include "App_EventOpcode.h"
#include "key_nvkey.h"
#include "key.h"
#include "App_Nvkey_KeymapHandler.h"
#include "App_State.h"
#include "App_Media.h"
#include "App_CheckKeyValid.h"
#include "App_EventKeyHandler.h"
#include "App_TestMode.h"

//Key Functions
#include "App_AvrcpKey.h"
#include "App_Mp3Key.h"
#include "App_HFPKey.h"
#include "App_AEKey.h"

#include "App_SystemKey.h"
#include "App_TestMode.h"
#include "App_Nvram_Type.h"
#include "App_PowerOff.h"
#include "App_MCSyncKey.h"
#include "App_MCSync.h"
#include "BtMCSync.h"
#include "App_MCSync_RHO.h"
#include "App_MCSync_Message.h"
#ifdef LINEIN_ENABLE
#include "App_LineIn.h"
#endif


#include "App_ChargeBattery.h"
#ifdef PROFILE_AMA_ENABLE
#include "App_AMAKey.h"
#endif /* PROFILE_AMA_ENABLE */
#include "App_System.h"
#include "App_Customer.h"

#ifdef XIAOAI_DEVELOPMENT
#include "App_XiaoaiKey.h"
#endif

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
#include "App_MCSync_Share.h"
#endif

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void APP_EvtKey_KeyEvent(U8 keyIndex, U8 keyAction)
{
	U8 state;

	#if BOOMTECH_WORKAROUND
	if(APP_ChgBat_IsTimerSet(APP_CHGBAT_CHGOUT_SW_WORKAROUND_ID))
	{
		if(keyIndex == KEY1)
			return;
	}
	#endif

	if(APP_AWSMCE_KeyEvent(keyIndex, keyAction))
	{
		return;
	}

	if(!APP_KeyManage_CheckButtonSequence(keyIndex, keyAction)) // if mapping btnSequence, end this switch
	{
		state = APP_State_GetTopState(APP_GetServiceBdAddr());
		APP_NVKEY_KeyParseEventPrintLog(keyIndex, keyAction, state);
		APP_EvtKey_KeyHandler(APP_NVKEY_KeyParseEvent(keyIndex, keyAction, state)); //parser keyevent code
	}
}

void APP_EvtKey_KeyHandler(U16 keyEventCode)
{
	if (keyEventCode >= KEY_TOTAL_NO)
	{
		return;
	}

<<<<<<< HEAD
#ifdef MCSYNC_SHARE_MODE
=======
#ifdef AIR_MCSYNC_SHARE_MODE_ENABLE
>>>>>>> db20e11 (second commit)
	if(!APP_MCSync_Share_IsKeyEventAllowed(keyEventCode))
	{
		return;
	}
#endif
	if(APP_IsButtonLocked() && keyEventCode != KEY_BUTTONLOCKTOGGLE)
	{
		//APP_PRINTF(COLOR_EVT_KEY_HANDLER, PRINT_EVT_KEY_HANDLER, "key Locked: 0x%x", keyEventCode);
		return;
	}

	if(keyEventCode >= KEY_FOR_AE_START && keyEventCode <= KEY_FOR_AE256)
	{
		APP_Media_PushMediaEvent(keyEventCode);
		APP_AE_ProcessKeyEvent(&gAppCtl.serviceBdAddr, (keyEventCode - KEY_FOR_AE_START));
	}
	else
	{
		keyEventCode = APP_ChkKeyVld_CheckKeyValid(keyEventCode);
		if (keyEventCode != KEY_INVALID)
		{
			if(keyEventCode != KEY_CNLOUTGOING) //ignor key media event
			{
				if(!BtAwsMce_IsDefaultRoleAgent()) //ignor key event
				{
					switch(keyEventCode)
					{
						case KEY_DISCOVERABLE:
							#if SINGLE_HEADSET_MODE_ENABLE
							break;
							#else
							return;
							#endif

						//case KEY_VOICE_PROMPT_LANG_CHANGE:
							//break;
						default:
							break;
					}
				}
				APP_Media_PushMediaEvent(keyEventCode);
				APP_Customer_ClearPairListLedPattern(keyEventCode);
			}
			//APP_PRINTF(COLOR_EVT_KEY_HANDLER, PRINT_EVT_KEY_HANDLER, "keyEventCode: 0x%X", keyEventCode);
			APP_EvtKey_KeyEventHandler(&gAppCtl.serviceBdAddr, keyEventCode);
		}
	}
}

void APP_EvtKey_KeyEventHandler(BD_ADDR_T *pBdAddr, U16 keyEventCode)
{
	U16 keyEventIndex;
	BD_ADDR_T bdAddrZero = {{0}};

	if(APP_MCSync_Rho_IsRhoing())
	{
		DBG_LOG_APP_SYSTEM( "[Key]: invalid in RHOing", 0);
		return;
	}

	if(BtAwsMce_IsSpecialLink(pBdAddr))
	{
		DBG_LOG_APP_SYSTEM("KeyEventHandler isspecial bdAddr:0x%x%x", 2,FW_bdaddr_to_2U32(pBdAddr, TRUE), FW_bdaddr_to_2U32(pBdAddr, FALSE));
		pBdAddr = &bdAddrZero;
	}

	DBG_LOG_APP_SYSTEM( "[Key]: eventCode: %s, bdAddr: 0x%x%x", 3, APP_KeyEventLogString[keyEventCode], FW_bdaddr_to_2U32(pBdAddr,TRUE),FW_bdaddr_to_2U32(pBdAddr,FALSE));

	keyEventIndex = APP_ChkKeyVld_GetKeyEventIndex(keyEventCode);
	switch(APP_ChkKeyVld_GetKeyEventGroup(keyEventCode))
	{
		case SYSTEM_KEY_EVENT:
			APP_SystemKey_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		case AWSMCE_KEY_EVENT:
			APP_AWSMCE_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		case HFP_KEY_EVENT:
			#ifdef HFP_Profile
			APP_HFP_ProcessKeyEvent(pBdAddr, keyEventIndex);
			#endif
			break;
		case AVRCP_KEY_EVENT:
<<<<<<< HEAD
			#ifdef AVRCP_Profile
=======
			#ifdef AIR_AVRCP_PROFILE_ENABLE
>>>>>>> db20e11 (second commit)
			APP_AVRCP_ProcessKeyEvent(pBdAddr, keyEventIndex);
			#endif
			break;
		case FM_KEY_EVENT:
			#ifdef FM_ENABLE
//			MMI_FM_ProcessKeyEvent(pBdAddr, keyEventIndex);
			#endif
			break;
		case LINE_IN_KEY_EVENT:
			#ifdef LINEIN_ENABLE
			APP_LineIn_ProcessKeyEvent(pBdAddr, keyEventIndex);
			#endif
			break;
		case TESTMODE_KEY_EVENT:
			APP_TestMode_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		#ifdef HID_Profile
		case HID_KEY_EVENT:
//			MMI_HID_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		#endif
		#ifdef PROFILE_AMA_ENABLE
		case AMA_KEY_EVENT:
			APP_AMA_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		#endif
        #ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
        case MP3_KEY_EVENT:
            APP_MP3_ProcessKeyEvent(pBdAddr, keyEventIndex);
            break;
        #endif
		#ifdef XIAOAI_DEVELOPMENT
		case XIAOAI_KEY_EVENT:
			APP_Xiaoai_ProcessKeyEvent(pBdAddr, keyEventIndex);
			break;
		#endif
	}
}
