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


<<<<<<< HEAD
#ifndef _GFP_RHOHANDLER_H_
#define _GFP_RHOHANDLER_H_

=======
#ifndef __APP_GFP_RHO_HANDLER_H__
#define __APP_GFP_RHO_HANDLER_H__

#include "gfp_features.h"
#ifdef GFP_MCSYNC_ENABLE
>>>>>>> db20e11 (second commit)
#include "bt_gap.h"


/**************************************************************************************************
* Public Functions
**************************************************************************************************/
<<<<<<< HEAD
BOOL gfp_rho_lock();
BOOL gfp_rho_unlock();
BOOL gfp_rho_is_allow();
U16  gfp_rho_get_data_length(void);
BOOL gfp_rho_get_data(U8 *pBuffer);
BOOL gfp_rho_apply_agent_data(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
BOOL gfp_rho_apply_partner_data(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);


U16 BtGfpGetRhoDataLength();
BOOL BtGfpGetRhoData(U8 *pBuffer);
BOOL BtGfpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);
BOOL BtGfpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);

#endif //_GFP_RHOHANDLER_H_
=======
BOOL APP_Gfp_RhoLock(void);
BOOL APP_Gfp_RhoUnlock(void);
BOOL APP_Gfp_RhoIsAllow(void);
U16 APP_Gfp_RhoGetDataLength(void);
BOOL APP_Gfp_RhoGetData(U8 *pBuffer);
BOOL APP_Gfp_RhoNewAgentProcess(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);
BOOL APP_Gfp_RhoNewPartnerProcess(BD_ADDR_T *pBdAddr, U8 *pData, U16 length);

#endif /* GFP_MCSYNC_ENABLE */

#endif /* __APP_GFP_RHO_HANDLER_H__ */

>>>>>>> db20e11 (second commit)
