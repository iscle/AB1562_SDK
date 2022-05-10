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
 
 
#ifndef __APP_CONNECTION_NVKEY_RECONNECT_CONFIG_H__
#define __APP_CONNECTION_NVKEY_RECONNECT_CONFIG_H__


/**************************************************************************************************
* Structure
**************************************************************************************************/
/*NvkeyDefine NVKEYID_APP_RECONNECT_CONFIG */
typedef struct
{
	/*
	#define APP_DISCOVER_AFTER_POWERON_RECONNECT_FAIL_FEAT	0x01
	*/
	U8	feature;									/**< @Value 0x00		@Desc Reconnect Feature "APP_Connection_NvkeyHandler.h" */
	U8	linkLossRetryCnt;							/**< @Value 0xFF		@Desc Link Loss Retry Count 0xFF = Alwyas */
	U8	twsLinkLossRetryCnt;						/**< @Value 0xFF		@Desc TWS Link Loss Retry Count 0xFF = Alwyas */
	U8 	watchDogRetryCnt;							/**< @Value 0x0A		@Desc Watch Dog Link Loss Retry Count 0xFF = Alwyas */
	U8  reconnectdurationLinkLossTimer;				/**< @Value 0x0A		@Desc After Link Loss Reconnect Start Delay unit:0.5sec */
	U8	autoRetryCnt;								/**< @Value 0x0A		@Desc Reconnect User Init Count 0xFF = Alwyas */
	U8	actionAfterPowerOn;							/**< @Value 0x02		@Desc Reconnect After Power On Scenario */
	U8	actionUserInitiated;						/**< @Value 0x02		@Desc Reconnect User Init Scenario */
	U8	actionAfterLinkLoss;						/**< @Value 0x03		@Desc Reconnect Link Loss Scenario */
	U8 	reConnectDelayTimer;						/**< @Value 0x05		@Desc Reconnect Delay Timer Unit:1 Sec */
	U8 	watchDogReconnectTimer;						/**< @Value 0x05		@Desc watchDog Reconnect Timer Unit:0.5 Sec */
}PACKED APP_RECONNECT_CONFIG_STRU;

#endif //__APP_CONNECTION_NVKEY_RECONNECT_CONFIG_H__
