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
 
 
#ifndef __PM_NVKEY_SNIFF_PARAMETER_H__
	#define __PM_NVKEY_SNIFF_PARAMETER_H__
	
	/***************************************************************************************
	 *	PM SNIFF STRUCT 0xF219 (NVKEYID_PROFILE_PM_SNIFF_PARAM)
	 ***************************************************************************************/

	/**
	 *@KeyID 0xF219
	*/
	/*NvkeyDefine NVKEYID_PROFILE_PM_SNIFF_PARAM */
	typedef struct PM_SNIFF_PARAMTER_STRU
	{
		U16 sniffMaxInterval;		/**< @Value 0x0368 		@Desc sniff max interval  @Range: 0x0000~0xFFFF*/
		U16 sniffMinInterval;		/**< @Value 0x02D8		@Desc sniff min interval  @Range: 0x0000~0xFFFF*/
		U16 sniffAttempCount;		/**< @Value 0x0004		@Desc sniff attemp  @Range: 0x0000~0xFFFF*/
		U16 sniffTimeOut;			/**< @Value 0x0001		@Desc sniff timeout  @Range: 0x0000~0xFFFF*/
		U8  sniffCount;				/**< @Value 0x03		@Desc sniff count  @Range: 0x00~0xFF*/

		//Sniff Subrating
		U16 maxLatency;//0x07D0 -> 2000 ms
		U16 minRemoteTimeout;//0x07D0 -> 2000 ms
		U16 miniLocalTimeout;//0x07D0 -> 2000 ms	

	}PACKED PM_SNIFF_PARAMTER_STRU;

	
	/**
	 *@KeyID 0xF2A0
	*/
	/*NvkeyDefine NVKEYID_PROFILE_PM_SNIFF_TIMER */
	typedef struct PM_SMIFF_TIMER_STRU
	{
		U16 sniffTimer;//0x401f => 8000 ms
		U16 checkRemoteAGAcceptEnterSniffTimer;//0x0BB8 => 3000 ms
	}PACKED PM_SMIFF_TIMER_STRU;

#endif //__PM_NVKEY_SNIFF_PARAMETER_H__
