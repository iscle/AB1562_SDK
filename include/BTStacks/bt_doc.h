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

#ifndef __RROFILE_HH__
#define __RROFILE_HH__

/*!
    @defgroup BT_TOP Bluetooth
    @{
        AB1530 bluetooth API was grouped by \ref BT_PRO and \ref BT_LIB layers. 
        
        \ref BT_PRO API is the upper layer of \ref BT_LIB, it was implemented 
        by \ref BT_LIB according to the specification was defined in [Traditional Profile Specifications](https://www.bluetooth.com/specifications/profiles-overview). 
        
        \ref BT_LIB API is more
        fundamental part of bluetooth, it provides the features which defined in [Core Specifications](https://www.bluetooth.com/specifications/bluetooth-core-specification) 
        and [Protocol Specifications](https://www.bluetooth.com/specifications/protocol-specifications).
        
        Generally, application use function call to operation bluetooth operation and received response with message.
        According appearance reason, message can be seprate to two type: <b>Indication</b> and <b>Confirmation</b>.
        
        Confirmation message is the response of function and indication was used for unsolicited event. In convention,
        the confirmations use _CFM postfix and indications use _IND. 

        \image html bt_top_arch.png "Figure : AB1530 bluetooth API architecure"
        \anchor bt_top_arch 

		@defgroup BT_PRO Profiles
		@defgroup BT_LIB Libraries
    @}
*/

/*!
    @addtogroup BT_PRO
    @{
        \image html bt_top_arch_pro.png "Figure : AB1530 bluetooth API architecure (Profile)"

        @defgroup BT_PRO_SPP SPP
        @{            
            @defgroup BT_PRO_SPP_MACRO  Macros
            @defgroup BT_PRO_SPP_DS Data Structures
            @defgroup BT_PRO_SPP_FN Functions
        @}

        @defgroup BT_PRO_A2DP A2DP
        @defgroup BT_PRO_AVRCP AVRCP
        @defgroup BT_PRO_HFP HFP
        @defgroup BT_PRO_HS HS

    @}
*/
#endif
