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


#ifndef _APP_RE_CONNECTION_H_
#define _APP_RE_CONNECTION_H_

/**************************************************************************************************
* Enum
**************************************************************************************************/
//The order can not be changed!!!!!
enum
{
	RECONNECT_TYPE_NULL,
	RECONNECT_PWR_ON,
	RECONNECT_USER_INIT,
	RECONNECT_LAST_ONCE_AUDIOTRANSFER,
	RECONNECT_RESERVED3,
	RECONNECT_LAST_ONCE_VOICE_DIAL, //5
	RECONNECT_FIRST_ONCE,
	RECONNECT_RESERVED1,
	RECONNECT_FIRST_ONCE_VOICE_DIAL,
	RECONNECT_SECONDARY_ONCE,
	RECONNECT_RESERVED2, //10
	RECONNECT_SECONDARY_ONCE_VOICE_DIAL,
	RECONNECT_AWSMCE,
	RECONNECT_LINK_LOSS,
	RECONNECT_WATCHDOG, //14
	RECONNECT_CHECK,
	RECONNECT_TWS_FOLLOW,
	RECONNECT_TWS_RELAY,
	ESD_RECONNECT_CHECK,
};

enum
{
	RECONNECT_UNNECESSARY,
	RECONNECT_LAST_ONE,
	RECONNECT_LAST_TWO,
	RECONNECT_LIST,
	RECONNECT_LAST_DISC,
	RECONNECT_AG_IND_0,
	RECONNECT_AG_IND_1,
	RECONNECT_AG_IND_0_1,
	RECONNECT_AWSMCE_CONNECT,
	RECONNECT_FAST_DONGLE,
	RECONNECT_ALL_DROPPED_LINKS,
	RECONNECT_CURRENT_LINK,
	RECONNECT_TWS_0011,
	RECONNECT_TWS_0101,
	RECONNECT_MODE_TOTAL_NO,
};


/**************************************************************************************************
* Define
**************************************************************************************************/
#define RECONNECT_PROFILE_HFP		0x01
#define RECONNECT_PROFILE_A2DP		0x02
#define RECONNECT_PROFILE_AVRCP		0x04
#define RECONNECT_PROFILE_HSP		0x08
#define RECONNECT_PROFILE_CUSTAPP	0x20

/**************************************************************************************************
* Variable
**************************************************************************************************/


/**************************************************************************************************
* Structure
**************************************************************************************************/





/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_ReConn_DeleteProfileMask(BD_ADDR_T *pBdAddr, U8 profileID, U8 reason);
U8   APP_ReConn_GetProfileMaskByBdAddr(BD_ADDR_T *pBdAddr);
void APP_ReConn_DeleteInvalidList(void);//MMI_Reconnect_DeleteInvalidList
void APP_ReConn_ClearCtl(void);//MMI_Reconnect_ClearCtl
BOOL APP_ReConn_ToUARTIndex(U8 index);//MMI_Reconnect_ToUARTIndex
void APP_Reconn_SetDelayTimer(U8 type, U32 delayTime);
void APP_Reconn_SetLinkLossBdAddr(BD_ADDR_T *pBdAddr);
void APP_ReConn_Reconnect(U8 type);//MMI_Reconnect
void APP_ReConn_DeleteListByBdAddr(BD_ADDR_T * pBdAddr);
bool APP_IsHFPConnecting(BD_ADDR_T *pBdAddr);
BOOL App_IsProfileInReconnectList(BD_ADDR_T *pBdAddr, U8 profileId);
void App_ClearProfileInReconnectList(BD_ADDR_T *pBdAddr, U8 profileId);

#endif
