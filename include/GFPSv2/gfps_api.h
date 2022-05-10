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

#ifndef __AIROHA_GFPS_V2_API_H__
#define __AIROHA_GFPS_V2_API_H__

#include "bt.h"
#include "Gfp_Bt.h"

<<<<<<< HEAD
#define GFPS_BLE_ADV_DATA_LEN   0x1F
#define GSPS_BLE_IDLE           0x00
#define GFPS_BLE_ADV_STOP       0x01
#define GFPS_BLE_ADVERTISING    0x02
#define GFPS_BLE_ADV_CHANGING   0x08
#define GFPS_BLE_DISCONNECTED   0x10
#define GFPS_BLE_CONNECTED      0x12

typedef struct {
  U8 len;
  U8 adv_data[GFPS_BLE_ADV_DATA_LEN];
} gfps_ble_adv_data_t;

typedef struct _cb_app_func_module_ {
  U8 (*is_bt_discoverable)();
  void (*set_bt_discoverable)(U8 action);
  U8 (*get_bt_connected_cnt)();
  U8 (*get_bt_maximum_link)();
} app_cbfunc_module_t;

U8 GFPSv2_update_adv_data(gfps_ble_adv_data_t *p_adv);
U8 GFPSv2_update_adv_para(BLE_ADV_PARAMS_T *advParams);
U8 GFPSv2_update_scan_response(U8 *rsp);
U8 GFPSv2_set_random_addr(U8 *raddr);
U8 GFPSv2_Enable(U8 enable, app_cbfunc_module_t *module);
U8 GFPSv2_IsEnable();
U8 GFPSv2_is_processing();
void GFPSv2_store_local_passkey(U8 linkIdx, U8 *key);
void App_GfpsAdv_Disable();
void App_GfpsAdv_Update( uint8_t advType );
void App_GfpsAdv_DataParmUpdate();
void App_GfpsAdv_DataParmUpdate_2();
void App_Gfps_Spp_Disconnect();
void App_GfpsAdv_Update_BatteryLevel();
void App_gfp_set_batt_adv();
uint8_t App_gfp_parnter_batt( uint8_t *pVal );
void App_gfp_set_agent_case_LidOn();
void App_gfp_set_agent_case_LidOff();
void App_gfp_set_agent_case_Out();
uint8_t get_random_num();


=======

#define GFPS_BLE_ADV_DATA_LEN   0x1F


typedef enum
{
    GFP_MCSYNC_CMD_ID_NONE,

    GFP_MCSYNC_CMD_ID_MERGE_ACCT_KEY_LIST_REQ,
    GFP_MCSYNC_CMD_ID_MERGE_ACCT_KEY_LIST_RSP,

    GFP_MCSYNC_CMD_ID_UPDATE_PERSONALIZED_NAME_REQ,
    GFP_MCSYNC_CMD_ID_UPDATE_PERSONALIZED_NAME_RSP,

    GFP_MCSYNC_CMD_ID_MAX = 0xFF
}gfp_mcsync_cmd_id_enum;


typedef enum
{
    GFP_UPDATE_BLE_ADV_REASON_ENABLE,
    GFP_UPDATE_BLE_ADV_REASON_FORCE,
    GFP_UPDATE_BLE_ADV_REASON_DISCOVERABLE_ENTER,
    GFP_UPDATE_BLE_ADV_REASON_DISCOVERABLE_EXIT,
    GFP_UPDATE_BLE_ADV_REASON_LID_ON,
    GFP_UPDATE_BLE_ADV_REASON_LID_OFF,
    GFP_UPDATE_BLE_ADV_REASON_LID_STATUS_NO_CHANGE,
    GFP_UPDATE_BLE_ADV_REASON_BATTERY_INFO_CHANGES,
    GFP_UPDATE_BLE_ADV_REASON_ACCOUNT_KEY_LIST_CHANGES,
    GFP_UPDATE_BLE_ADV_REASON_SALT_CHANGES,
    GFP_UPDATE_BLE_ADV_REASON_SCAN_RSP_CHANGES,

    GFP_UPDATE_BLE_ADV_REASON_MAX
}gfp_update_ble_adv_reason_enum;


typedef enum
{
    GFP_RET_SUCCESS,
    GFP_RET_FAIL,
    GFP_RET_INVALID_PARM,
    GFP_RET_NVKEY_READ_FAIL,
    GFP_RET_NOT_ENOUGH_MEMORY,

    GFP_RET_MAX = 0xFF
}gfp_ret_enum;


typedef enum
{
    GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_NONE,

    GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_LEFT_BUD,
    GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_RIGHT_BUD,
    GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_CASE,

    GFP_ADV_DATA_BATTERY_COMPONENT_TYPE_MAX = 0xFF
}gfp_adv_data_battery_component_type_enum;


typedef struct
{
    U8 (*is_bt_discoverable)(void);
    void (*set_bt_discoverable)(U8 action);
    U8 (*get_acl_link_count)(void);
    U8 (*get_smartcase_battery_level)(void);
    U8 (*get_ag_num)(void);
    void (*reconn_clear_ctl)(void);
    void (*conn_release_multilink)(void);
} gfp_reg_app_func_struct;


U8 GFPSv2_is_processing(void);
void GFPSv2_store_local_passkey(U8 linkIdx, U8 *key);
>>>>>>> db20e11 (second commit)
U8 Gfp_GetLinkState(GFP_LINK_STRU * pLinkInfo);
GFP_LINK_STRU * Gfp_GetLinkByBdAddr(BD_ADDR_T *pBdAddr);
GFP_LINK_STRU * Gfp_GetEmptyLink(void);
U8 Gfp_GetServiceState(void);
<<<<<<< HEAD
U8 App_GfpEnableStat();

=======
gfp_ret_enum gfp_init(gfp_reg_app_func_struct *app_func);
BOOL gfp_is_enabled(void);
void gfp_ble_adv_set_gatt_server_status(BOOL ready);
BOOL gfp_ble_adv_enable(void);
BOOL gfp_ble_adv_disable(void);
BOOL gfp_ble_adv_update(gfp_update_ble_adv_reason_enum reason);
BOOL gfp_ble_adv_is_enabled(void);;
uint8_t *gfp_get_model_id(void);
void gfp_acct_key_reset(void);
uint8_t *gfp_ble_rpa_get_reverse_rpa(void);
uint8_t gfp_adv_data_battery_value_get(gfp_adv_data_battery_component_type_enum component_type);
void gfp_set_partner_battery_level(U8 battery_level);
void gfp_set_partner_charging_status(U8 charging_status);
U8 gfp_get_partner_charging_status(void);
void gfp_personalized_name_clear(void);
void gfp_spp_connect_handle(void);
void gfp_spp_disconnect_handle(void);
BOOL gfp_mcsync_send_merge_acct_key_list_req(BOOL merge_based_on_req);
void gfp_mcsync_handle_merge_acct_key_list_req(U16 length, U8 *p_data);
void gfp_mcsync_handle_merge_acct_key_list_rsp(U16 length, U8 *p_data);
BOOL gfp_mcsync_send_update_personalized_name_req(BOOL update_based_on_req);
void gfp_mcsync_handle_update_personalized_name_req(U16 length, U8 *p_data);
void gfp_mcsync_handle_update_personalized_name_rsp(U16 length, U8 *p_data);
BOOL gfp_rho_lock(void);
BOOL gfp_rho_unlock(void);
BOOL gfp_rho_is_allow(void);
BOOL gfp_rho_is_locked(void);
U16 gfp_rho_get_data_length(void);
BOOL gfp_rho_get_data(U8 *pBuffer);
BOOL gfp_rho_new_agent_process(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length);
BOOL gfp_rho_new_partner_process(BD_ADDR_T *pBdAddr, U8 *pAppData, U16 length);
void LeGFPSv2_Init(void);
>>>>>>> db20e11 (second commit)

#endif //__AIROHA_GFPS_V2_API_H__

