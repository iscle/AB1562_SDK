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

#ifndef __PM_MESSAGE_HH_
#define __PM_MESSAGE_HH_

/**************************************************************************************************
* Define
**************************************************************************************************/
#define MSG_MESSAGE_BT_SDP				MSG_MESSAGE_BT_PROFILE_BASE
#define MSG_MESSAGE_BT_PROFILE_HSP 		(MSG_MESSAGE_BT_PROFILE_BASE + 0x1000)	//0x2000
#define MSG_MESSAGE_BT_PROFILE_HFP 		(MSG_MESSAGE_BT_PROFILE_BASE + 0x2000)  //0x3000
#define MSG_MESSAGE_BT_PROFILE_A2DP 	(MSG_MESSAGE_BT_PROFILE_BASE + 0x3000)  //0x4000
#define MSG_MESSAGE_BT_PROFILE_AVRCP 	(MSG_MESSAGE_BT_PROFILE_BASE + 0x4000)  //0x5000
#define MSG_MESSAGE_BT_PROFILE_MCSYNC	(MSG_MESSAGE_BT_PROFILE_BASE + 0x5000)  //0x6000
#define MSG_MESSAGE_BT_PM				(MSG_MESSAGE_BT_PROFILE_BASE + 0x6000)  //0x7000

//#define PM_SNIFF_DELAY_TIME_ID		MSG_MESSAGE_BT_PM

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	PM_SNIFF_DELAY_TIME_ID = MSG_MESSAGE_BT_PM,
	PM_CHECK_REMOTE_AG_ACCEPT_ENTER_SNIFF_TIMER_ID,
	PM_AVRCP_DISCONNECT_WORK_AROUND,
	PM_DISCONNECT_PROFILE,
	PM_CONNECT_IND_TIMER,

};

#endif
