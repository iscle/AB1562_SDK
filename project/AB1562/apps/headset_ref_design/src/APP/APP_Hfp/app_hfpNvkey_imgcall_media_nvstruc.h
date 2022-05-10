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
 
 
#ifndef __APP_HFP_NVKEY_IMGCALLMEDIA__H__
	#define __APP_HFP_NVKEY_IMGCALLMEDIA__H__

	/***************************************************************************************
	 *	APP_HFP_NVKEY_IMGCALLMEDIA STRUCT 0xF215 (NVKEYID_APP_IMG_CALL_MEDIA_CONFIG)
	 ***************************************************************************************/


	/**
	 *@KeyID 0xF215
	*/
	/*NvkeyDefine NVKEYID_APP_IMG_CALL_MEDIA_CONFIG */
	typedef struct Config_AppHfpImgCallMedia
	{
		U32 callerIDRepeatTime; 	//unit:ms, Val:20000, Range:0-0xFFFFFFFF	/**< @Value 0x00002710	@Desc callerIDRepeatTime */
		
		/*
			enum
			{
				APP_HFP_IMG_MEDIA_NONE,
				APP_HFP_IMG_MEDIA_VP,
				APP_HFP_IMG_MEDIA_CALLER_NAME,
				APP_HFP_IMG_MEDIA_CALLER_ID,
				APP_HFP_IMG_MEDIA_RINGTONE,
				APP_HFP_IMG_MEDIA_NUM = APP_HFP_IMG_MEDIA_RINGTONE,
			};
			U8 	ImgCallMediaPriority[APP_HFP_IMG_MEDIA_NUM];
			*/
		U8	ImgCallMediaPriority0;		/*APP_HFP_IMG_MEDIA_VP */ 			/**< @Value 0x01		@Desc ImgCallMediaPriority0 */
		U8	ImgCallMediaPriority1;		/*APP_HFP_IMG_MEDIA_CALLER_ID */ 	/**< @Value 0x03		@Desc ImgCallMediaPriority1 */
		U8	ImgCallMediaPriority2;		/*APP_HFP_IMG_MEDIA_CALLER_NAME */ 	/**< @Value 0x02		@Desc ImgCallMediaPriority2 */
		U8	ImgCallMediaPriority3;		/*APP_HFP_IMG_MEDIA_RINGTONE */ 	/**< @Value 0x04		@Desc ImgCallMediaPriority3 */
		
	}PACKED NVKEY_IMG_CALL_MEDIA_STRU;

	
	
#endif
