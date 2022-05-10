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
 
 
#ifndef __BT_SDPS_INTERFACE_H__
#define __BT_SDPS_INTERFACE_H__


/**************************************************************************************************
*Enum
**************************************************************************************************/



/**************************************************************************************************
*Structure
**************************************************************************************************/

/**************************************************************************************************
*Definition PNP ID
**************************************************************************************************/

//XiaoMi9
#define PNP_VENDOR_ID_XIAO_MI_COMPANY   0x038F
#define PNP_PRODUCT_ID_XIAO_MI_9        0x1200

//Sony phone
#define PNP_VENDOR_ID_SONY_XPERIA       0x0FCE
#define PNP_PRODUCT_ID_SONY_XPERIA_5    0x020A
#define PNP_PRODUCT_ID_SONY_XPERIA_1    0x0205

#define SDPS_INVALID_ID 0xFFFF

/**************************************************************************************************
* Public Functon prototype
**************************************************************************************************/
void bt_sdps_connect(BD_ADDR_T *pBdAddr);
void bt_sdps_disconnect(BD_ADDR_T *pBdAddr);
BOOL bt_sdps_pnp_check_aqua(BD_ADDR_T *pBdAddr);
void bt_sdps_clear_ctl_info_by_bdaddr(BD_ADDR_T *pBdAddr);

void bt_sdps_get_rho_data(BD_ADDR_T *pBdAddr, U8 *ptr, U16 length);
void bt_sdps_set_rho_data(BD_ADDR_T *pBdAddr, U8 *ptr, U16 length);
void bt_sdps_apply_partner_rho_data(BD_ADDR_T *pBdAddr);
BOOL bt_sdps_pnp_is_sony_xperia(BD_ADDR_T *pBdAddr);
BOOL bt_sdps_pnp_is_xiao_mi9(BD_ADDR_T *pBdAddr);

U16 bt_sdps_get_vendor_id(BD_ADDR_T *pBdAddr);
U16 bt_sdps_get_avrcp_TG_version(BD_ADDR_T *pBdAddr);

#endif //__BT_SDPS_INTERFACE_H__

