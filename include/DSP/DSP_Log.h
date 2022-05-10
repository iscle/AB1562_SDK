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
#ifndef _DSP_LOG_H_
#define _DSP_LOG_H_

#include "types.h"
#include "log.h"



////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    DSP_WARNING_RESERVED            = 0,
    DSP_WARNING_SINK_EMPTY,
    DSP_WARNING_STREAMING_DISABLE,
    DSP_WARNING_MEMORY_FLUSH,
    DSP_WARNING_FEATURE_NULL,
    DSP_WARNING_SCO_WAIT_DM2L,
    DSP_WARNING_SBC_FRAME_MISMATCH,
    DSP_WARNING_SBC_FRAME_ERROR,
    DSP_WARNING_CALLBACK_READSIZE_MODIFICATION,
    DSP_WARNING_FEATURE_FRAME_MISMATCH,
    DSP_WARNING_DRIFT_COMPENSATION_MAXIMUM,
    DSP_WARNING_WAIT_SRC_DONE,
    DSP_WARNING_TRY_TO_DISABLE_CLOCK,
    DSP_WARNING_ZEROPADDING_DPC_TIMEOUT,
    DSP_WARNING_UPDATE_FEATURE_FAIL,
    DSP_WARNING_SOURCE_EMPTY,
    DSP_WARNING_CALLBACK_RESOLUTION_MISMATCH,
    DSP_WARNING_AMA_SOURCE_FULL,

    DSP_WARNING_MAX_NUM,
} DSP_WARNING_MSG;



////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// DSPMEM FUNCTION DECLARATIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
EXTERN void DSP_LOG_WarningPrint (DSP_WARNING_MSG type, U32 argc, ...);



#endif /* _DSP_LOG_H_ */

