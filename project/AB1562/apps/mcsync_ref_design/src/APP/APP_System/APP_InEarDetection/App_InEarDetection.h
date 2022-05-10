/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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


#ifndef _APP_IN_EAR_DETECTION_HH_
#define _APP_IN_EAR_DETECTION_HH_

/**************************************************************************************************
* Define
**************************************************************************************************/
#define APP_IN_EAR_DETECTION_AIRPODS_A2DP_MMI_FEAT	0x0001
#define APP_IN_EAR_DETECTION_HFP_MMI_FEAT			0x0002
/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	TWO_EAR_OUT,
	SINGLE_AGENT_IN,
	SINGLE_PARTNER_IN,
	TWO_EAR_IN,
};

/**************************************************************************************************
 * Structure
**************************************************************************************************/
typedef struct
{
	U8 lastTwoEarStatus;
	U8 partnerStatus;
	BOOL pauseFlag;
}APP_IN_EAR_DETECTION_CTL_STRU;


/**************************************************************************************************
* Public Functions
**************************************************************************************************/

U8 APP_InEarDetection_GetStatus();
U8 APP_InEarDetection_GetPartnerStatus();
void APP_InEarDetection_SetPartnerStatus(U8 status);
U8 APP_InEarDetection_GetLastTwoEarStatus();
U8 APP_InEarDetection_DecideTwoEarStatus(U8 agentStatus, U8 partnerStatus);
void APP_InEarDetection_SetLastTwoEarStatus(U8 status);
void APP_InEarDetection_Init(void);
void APP_InEarDetection_TriggerAction(U8 agentStatus, U8 partnerStatus);
BOOL APP_InEarDetection_IsFeatureOn(U16 feature);
void APP_InEarDetection_NvKeyInit();
void APP_InEarDetection_SetPauseFlag(BOOL isPause);
BOOL APP_InEarDetection_GetPauseFlag(void);
void APP_InEarDetection_MusicPauseCtl(void);
BOOL APP_InEarDetection_IsA2DPStreaming(void);
void APP_InEarDetection_MusicPlayCtl(void);
BOOL APP_InEarDetection_IsEnable(void);
void APP_InEarDetection_SetFeature(BOOL isOn);
void APP_InEarDetection_CancelRHOTimer(void);
void APP_InEarDetection_SetDebounceTimer(void);
BOOL APP_InEarDetection_FillRhoData(void *pData);
BOOL APP_InEarDetection_AssignRhoData(void *pData);
BOOL APP_InEarDetection_GetAppFeature(void);

#endif//_APP_IN_EAR_DETECTION_HH_

