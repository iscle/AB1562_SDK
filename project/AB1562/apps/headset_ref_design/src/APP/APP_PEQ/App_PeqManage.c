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
#include "App_PeqManage.h"
#include "App_PeqProcess.h"
#include "AudioDSP_Registration.h"
#include "AudioDSP_StreamManager.h"
#include "Peq_NvkeyHandler.h"
#include "App_PeqMount.h"

/**************************************************************************************************
* Public functions
**************************************************************************************************/
void APP_PeqManage_LoadPeqGroup(U8 peq_path)
{
	DBG_LOG_APP_PEQ( "[APP_PEQ]Load PEQ component:%s", 1, APP_PeqComponentLogString[peq_path]);
	APP_PeqProc_Load(peq_path);
}

void APP_PeqManage_ChangePeqGroup(void)
{
    U8 peq_path;
    switch(APP_AudioDspGetMainPath())
    {
        case AUDIO_A2DP:
            peq_path = PEQ_A2DP;
            break;
        #ifdef LINEIN_ENABLE
        case AUDIO_LINEIN:
            peq_path = PEQ_LINE_IN;
            break;
        #endif
		#ifdef MP3_LOCAL_PLAYBACK_MMI_CTRL
		case AUDIO_MP3:
			peq_path = PEQ_MP3;
			break;
		#endif //MP3_LOCAL_PLAYBACK_MMI_CTRL
		default:
			DBG_LOG_APP_PEQ( "[APP_PEQ]No Mainpath:%d", 1, APP_AudioDspGetMainPath());
			return;
    }

	DBG_LOG_APP_PEQ( "[APP_PEQ]Change PEQ path:%s", 1, APP_PeqComponentLogString[peq_path]);
	APP_PeqProc_ChangeToNext(peq_path);
	APP_PeqProc_Load(peq_path);
}

void APP_PeqManage_SetPeqGroup(U8 peq_path, U8 groupIndex)
{
	DBG_LOG_APP_PEQ( "[APP_PEQ]Set PEQ component:%s", 1, APP_PeqComponentLogString[peq_path]);
	APP_PeqProc_SetIndex(peq_path, groupIndex);
	APP_PeqProc_Load(peq_path);
}

void APP_PeqManage_RealtimeUpdate(U8 *pPeqPara, U16 length)
{
	APP_PeqProc_RealtimeUpdate(*(pPeqPara), (pPeqPara + 1), length - 1);
}
