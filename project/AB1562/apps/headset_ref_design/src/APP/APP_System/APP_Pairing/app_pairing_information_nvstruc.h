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
 
 
#ifndef _PAIRING_INFORMATION_PATH_H_
#define _PAIRING_INFORMATION_PATH_H_

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * Audio Path 	@KeyID 0xF21D
 *
 */
 
typedef struct
{
	U16 PairModeTimeout;				//unit:s
	U16 PairModeTimeoutIfPDLIsEmpty;	//unit:s
	U16 ConnectableTimeout;				//unit:s
}PAIRING_TIMER_STRU;

/*NvkeyDefine NVKEYID_APP_PAIRING_INFORMATION*/
typedef struct
{
	U8 maxPageDevice;					//Range: 1 or 2
	U8 discoverableIfPdlLessThan;		//Range: 0~255
	U8 actionAfterPairingModeTimeout;	//action enum defined in Pairing_NvkeyHandler.h (line:22)
	U16 feature;						//feature bit defined in Pairing_NvkeyHandler.h (line:7 )
	PAIRING_TIMER_STRU timer;
}PACKED PAIRING_INFOR_STRU;


/*
PAIRING_INFOR_STRU pairingInfo = 
{	
	//maxPageDevice
	#ifdef TRSPX_App
	1,
	#else
	2, 
	#endif

	//discoverableIfPdlLessThan
	0,
	
	//actionAfterPairingModeTimeout 
	POWER_OFF_IF_PDL_IS_EMPTY, //0:Return to Connectable, 1:Power Off, 2:Power off if PDL is empty
	
	0x0080,
	
	{//timer
		300,
		300,
		600
	},
	
	
};

*/

#endif
