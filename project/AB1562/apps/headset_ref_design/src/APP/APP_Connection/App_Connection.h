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
 
 
#ifndef _APP_CONNECTION_H_
#define _APP_CONNECTION_H_


/**************************************************************************************************
* Definition
**************************************************************************************************/
#if 0//LOG_PRINT_156X
#define DBG_LOG_APP_Connection(_message, arg_cnt, ...)   printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_APP_Connection(_message, arg_cnt, ...)   LOG_MSGID_I(APP_CONN, _message, arg_cnt, ##__VA_ARGS__)
#endif

/**************************************************************************************************
* Public Functions (Connection API)
**************************************************************************************************/
void APP_RemoveServiceBdAddr(BD_ADDR_T *pBdAddr);
void APP_AddServiceBdAddr(BD_ADDR_T *pBdAddr);

void APP_Conn_ReleaseProfileLinkAndDetach(BD_ADDR_T *pBdAddr);
void APP_Conn_ReleaseMultiLink(void);//MMI_ReleaseMultiLink
BOOL APP_Conn_CheckProfileOnAllLinks(void);//MMI_CheckProfileOnAllLinks

void APP_Conn_CreateActiveProfileConnection(BD_ADDR_T * pBdAddr);

/**************************************************************************************************
* Public Functions (Notify)
**************************************************************************************************/
void APP_Conn_Register(void);

U8 APP_Conn_GetSupportDeviceNo(void);
void APP_Conn_SetSupportDeviceNo(BOOL isIncrease);
BOOL APP_Conn_PowerOffHandler(void);
BOOL APP_Conn_IsProfilesReady(void);
U8 APP_Conn_GetAgLinkState(void);
U16 APP_ConnCheckTimerClearLinkKey(void);
void APP_ConnSetTimerWaitDisconnectedClearLinkKey(void);

#endif //_APP_CONNECTION_H_
