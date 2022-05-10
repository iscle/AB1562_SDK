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

#ifndef _BTXIAOAI_H_
#define _BTXIAOAI_H_
#include "bt.h"


/**************************************************************************************************
* Enumeration
**************************************************************************************************/

/*
   Status return from API functions
*/
typedef enum{
    XIAOAI_STATUS_OK = 0,
    XIAOAI_STATUS_ERROR,
    XIAOAI_STATUS_OUT_OF_MEMORY,
    XIAOAI_STATUS_BUSY,
    XIAOAI_STATUS_LOW_BATTERY,
    XIAOAI_STATUS_OTA_TIMEOUT,
    XIAOAI_STATUS_OTA_ERROR,
    XIAOAI_STATUS_OTA_ERROR_INCORRECT_CHECKSUM,  
}xiaoai_status_e;

/*
   Message ID, notify APP
   CFM: confirmation, response APP after APP sends a command / request
   IND: indication, notify APP there is a event
*/
typedef enum{
    XIAOAI_START_SERVICE_CFM,
    XIAOAI_STOP_SERVICE_CFM,
    XIAOAI_CONNECT_CFM,
    XIAOAI_DISCONNECT_CFM,
    XIAOAI_DISCONNECT_IND,

    XIAOAI_BLE_SET_ADV_PARAMS_CFM,
    XIAOAI_BLE_SET_LOCAL_RANDOM_ADDR_CFM,
    XIAOAI_BLE_SET_ADV_DATA_CFM,
    XIAOAI_BLE_SET_ADV_ENABLE_CFM,
    
    XIAOAI_INTERNAL_MSG_OFFSET,     //internal for 153x
}xiaoai_msg_e;

/*
	EDR link state
*/
typedef enum
{
	XIAOAI_EDR_LINK_DISCONNECTED,
	XIAOAI_EDR_LINK_CONNECTING,
	XIAOAI_EDR_LINK_CONNECTED,
	XIAOAI_EDR_LINK_DISCONNECTING,
}xiaoai_edr_link_state_e;

/**************************************************************************************************
* Structures
**************************************************************************************************/
typedef struct{
	unsigned char addr[6];
}xiaoai_bdaddr_t;

/**************************************************************************************************
* Structures
**************************************************************************************************/
typedef struct{
    unsigned char status;
}xiaoai_start_service_cfm_t;

typedef struct{
    unsigned char status;
}xiaoai_stop_service_cfm_t;

typedef struct{
    unsigned char status;
    xiaoai_bdaddr_t bdaddr;
}xiaoai_connect_cfm_t;

typedef struct{
    unsigned char status;
    xiaoai_bdaddr_t bdaddr;
}xiaoai_disconnect_cfm_t;

typedef struct{
    unsigned char status;
    xiaoai_bdaddr_t bdaddr;
}xiaoai_disconnect_ind_t;

typedef struct{
    unsigned char status;
}xiaoai_ble_set_adv_params_cfm_t;

typedef struct{
    unsigned char status;
}xiaoai_ble_set_local_random_addr_cfm_t;

typedef struct{
    unsigned char status;
}xiaoai_ble_set_adv_data_cfm_t;

typedef struct{
    unsigned char status;
}xiaoai_ble_set_scan_response_data_cfm_t;

typedef struct{
    unsigned char status;
}xiaoai_ble_set_adv_enable_cfm_t;


/**************************************************************************************************
* Definition
**************************************************************************************************/
typedef struct xiaoai_handler_data *xiaoai_handler;
/*
The handler function's return value is meaningful for whether the internal system will free the memory
(pointed by the 3rd pamameter) or not. If it returns 0, we'll free it; if it returns 1, we won't free it and
the user can re-use the memory, if it returns 2, the message will be put into the end of the queue. By default, it should returns 0.
*/
typedef struct xiaoai_handler_data { U32 (*handler)(xiaoai_handler, U16, void*, U32); } xiaoai_handler_data;

/**************************************************************************************************
* API (Connection Relative)
**************************************************************************************************/
xiaoai_status_e xiaoai_start_service(xiaoai_handler app_handler, unsigned char max_link_num);
xiaoai_status_e xiaoai_stop_service(xiaoai_handler app_handler);

xiaoai_status_e xiaoai_start_speech(xiaoai_bdaddr_t *pBdAddr);
xiaoai_status_e xiaoai_stop_speech(xiaoai_bdaddr_t *pBdAddr);

xiaoai_status_e xiaoai_set_adv_enable_req(unsigned char is_enable);
xiaoai_status_e xiaoai_set_adv_data_req(unsigned char len_adv_data, unsigned char *p_adv_data);
xiaoai_status_e xiaoai_set_local_random_addr_req(xiaoai_bdaddr_t *p_bdaddr);
xiaoai_status_e xiaoai_set_adv_interval(unsigned short adv_interval_min, unsigned short adv_interval_max);

/**************************************************************************************************
* API (command respense)
**************************************************************************************************/
#define EDR_STATUS_ACL_CLOSED   0
#define EDR_STATUS_ACL_OPEN     1
xiaoai_status_e xiaoai_send_edr_status_cmd(xiaoai_bdaddr_t *pBdAddr, unsigned char status);

#define BT_OP_DISCONNECT_EDR    0
#define BT_OP_CONNECT_EDR       1
xiaoai_status_e xiaoai_send_notify_f2a_bt_op_cmd(unsigned char bt_op);

#define TWS_CONNECTED   0
#define TWS_CONNECTING  1
#define TWS_DISCONNECTED  2
xiaoai_status_e xiaoai_send_tws_status_cmd(unsigned char status);

xiaoai_status_e xiaoai_send_battery_status_cmd(void);

/**************************************************************************************************
* API (SPEECH)
**************************************************************************************************/
unsigned char *xiaoai_voice_stream_sink_claim(U32 size);
xiaoai_status_e xiaoai_voice_stream_sink_flush(void);
xiaoai_status_e xiaoai_voice_stream_clear_speech_ctl(void);

/**************************************************************************************************
* API (Authentication)
**************************************************************************************************/

/**************************************************************************************************
* API (misc)
**************************************************************************************************/
unsigned short xiaoai_get_local_device_version(void);
void xiaoai_set_remote_device_version(unsigned short version);
/**************************************************************************************************
*
*
* (for internal 153x)
*
*
**************************************************************************************************/

/**************************************************************************************************
* Enumeration
**************************************************************************************************/

typedef enum{   
    /* rx command */
	XIAOAI_GET_DEVICE_INFO_IND = XIAOAI_INTERNAL_MSG_OFFSET,		//opcode:0x02
    XIAOAI_DEV_REBOOT_IND,											//opcode:0x03                       
    XIAOAI_RECONNECT_EDR_IND,		//opcode:0x04   //internal
    XIAOAI_DISCONNECT_EDR_IND,		//opcode:0x06   //internal
    XIAOAI_SET_DEVICE_INFO_IND,		//opcode:0x08	  //internal
    XIAOAI_GET_DEVICE_RUN_INFO_IND,									//opcode:0x09 
    XIAOAI_ENTER_DISCOVERABLE_IND,	//opcode:0x0f   //internal
    XIAOAI_DELETE_PAIRED_DEV_IND,   //opcode:0x0f   //internal

    XIAOAI_SPEECH_START_IND,		//opcode:0xd0   //internal
    XIAOAI_SPEECH_STOP_IND,			//opcode:0xd1   //internal

	XIAOAI_VENDOR_CMD_IND,			//opcode:0xf1	  //internal

    /* rx response cfm */
    XIAOAI_SPEECH_START_CFM,		//opcode:0xd0   //internal
    XIAOAI_SPEECH_STOP_CFM,			//opcode:0xd1
    XIAOAI_SPEECH_LONG_HOLD_CFM,	//opcode:0xd3
	
	/* VP indication */
	XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI, //請打開小愛同學手機客戶端
	XIAOAI_VP_IND_ENABLING_XIAOAI, //正在喚醒小愛同學
	XIAOAI_VP_IND_PLEASE_CONNECT_EDR, //藍芽未連接 請通過手機藍芽連接我吧
	XIAOAI_VP_IND_PLEASE_ENABLE_XIAOAI_AND_CONNECT, //請打開小愛同學手機客戶端進行設備連接
}xiaoai_msg_internal_e;

/*
   Response status
*/

typedef enum{
    XIAOAI_RCSP_STATUS_SUCCESS,						//0
    XIAOAI_RCSP_STATUS_FAIL,
    XIAOAI_RCSP_STATUS_UNKOWN_CMD,
    XIAOAI_RCSP_STATUS_BUSY,
    XIAOAI_RCSP_STATUS_RESOURSE,
    XIAOAI_RCSP_STATUS_CRC_ERROR,					//5
    XIAOAI_RCSP_STATUS_ALL_DATA_CRC_ERROR,
    XIAOAI_RCSP_STATUS_PARAM_ERROR,
    XIAOAI_RCSP_STATUS_RESPONSE_DATA_OVER_LIMIT,
    XIAOAI_RCSP_STATUS_NOT_SUPPORT,
    XIAOAI_RCSP_STATUS_PARTIL_OPERATION_FAILED,		//10
    XIAOAI_RCSP_STATUS_UNREACHABLE,
}xiaoai_rcsp_status_e;

enum
{
	XIAOAI_SPEECH_STOPPED,
	XIAOAI_SPEECH_STARTING,
	XIAOAI_SPEECH_STARTED,
	XIAOAI_SPEECH_TRANSFERING_VOICE_DATA,
	XIAOAI_SPEECH_STOPPING,
};


/**************************************************************************************************
* Structures (for 153x)
**************************************************************************************************/
typedef enum{
    XIAOAI_DEV_REBOOT_TYPE_RESTART = 0,
    XIAOAI_DEV_REBOOT_TYPE_POWER_OFF = 1,
}xiaoai_dev_reboot_type_e;

typedef struct
{
    xiaoai_dev_reboot_type_e reboot_type;
}xiaoai_dev_reboot_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
}xiaoai_reconnect_edr_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
}xiaoai_disconnect_edr_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
}xiaoai_delete_paired_dev_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
}xiaoai_speech_start_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
}xiaoai_speech_stop_ind_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
    unsigned char status;
}xiaoai_speech_start_cfm_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
    unsigned char status;
}xiaoai_speech_stop_cfm_t;

typedef struct{
    xiaoai_bdaddr_t bdaddr;
    unsigned char status;
}xiaoai_speech_long_hold_cfm_t;

typedef struct
{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
	unsigned short data_length;
	unsigned char data[1];
}xiaoiai_msg_send_vendor_cmd_ind_t;



typedef enum xm_set_dev_attr_list_e
{
	XM_ATTR_DEVICE_POWER_MODE,//0
	XM_ATTR_DEVICE_FUNC_KEY,
	XM_ATTR_DEVICE_HOTWORD,
	XM_ATTR_DEVICE_SUP_POWER_SAVE_NEW,
	XM_ATTR_DEVICE_ANC_MODE,
	//
	XM_ATTR_DEVICE_LIST_NUM,
    XM_ATTR_DEVICE_LIST_MAX = 32,
}xm_set_dev_attr_list_e;/* range : 0~31 */

typedef struct
{
	unsigned char xm_attr; /* xm_set_dev_attr_list_e */
	unsigned char setting;
}xiaoiai_attr_element_t;

typedef struct
{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
    unsigned char attr_num;
	xiaoiai_attr_element_t attr[1];
}xiaoiai_set_device_info_t;

typedef struct
{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
	unsigned short data_length;
	unsigned char data[1];
}xiaoiai_get_device_info_t;

typedef struct
{
    xiaoai_bdaddr_t bdaddr;
    unsigned char op_code_sn;
	unsigned short data_length;
	unsigned int mask;
	unsigned char data[1];
}xiaoiai_get_device_run_info_t;


xiaoai_status_e xiaoai_send_start_speech_rsp(xiaoai_bdaddr_t *pBdAddr, bool isAccept, unsigned char opcode_sn);
xiaoai_status_e xiaoai_send_stop_speech_rsp(xiaoai_bdaddr_t *pBdAddr, bool isAccept, unsigned char opcode_sn);
xiaoai_status_e xiaoai_core_send_rsp_set_device_info(xiaoai_bdaddr_t * pBdAddr, U8 status, U8 opCodeSN);
xiaoai_status_e xiaoai_core_send_rsp_get_device_info(xiaoai_bdaddr_t * pBdAddr, U8 status, U8 opCodeSN, U8 *ptr, U16 len);

/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL xiaoai_lock();
BOOL xiaoai_unlock();
BOOL xiaoai_is_allow_rho();
unsigned short xiaoai_get_rho_data_length();
BOOL xiaoai_get_rho_data(unsigned char *pBuffer);
BOOL xiaoai_apply_agent_rho_data(xiaoai_bdaddr_t *pBdAddr, unsigned char *pData, unsigned short length);
BOOL xiaoai_apply_partner_rho_data(xiaoai_bdaddr_t *pBdAddr, unsigned char *pData, unsigned short length);

/**************************************************************************************************
* API (VP Indication)
**************************************************************************************************/
typedef struct{
    xiaoai_bdaddr_t bdaddr;
}xiaoai_vp_ind_t;

//
unsigned short xiaoai_nvkey_get_device_vid(void); //opcode 0x02: bit 3 vid
unsigned short xiaoai_nvkey_get_device_pid(void); //opcode 0x02: bit 3 pid
xiaoai_bdaddr_t * xiaoai_nvkey_get_edr_bdaddr(void); 	//opcode 0x09: bit 0 edr bdaddr
unsigned char xiaoai_nvkey_get_major_id(void);
unsigned char xiaoai_nvkey_get_minor_id(void);


unsigned char xiaoai_bat_get_local_level_in_percent(void); //opcode 0x02: bit 2 local device battery

unsigned char xiaoai_get_tws_status(void); 		//opcode 0x09: bit 6 tws status


xiaoai_status_e xiaoai_vendor_send_rsp(xiaoai_bdaddr_t * pBdAddr, U8 status, U8 *ptr, U16 len, U8 opCodeSN);

U16 xiaoai_get_pkt_u16(U8 *ptr);
U32 xiaoai_get_pkt_u32(U8 *ptr);
U16 xiaoai_transform_endian_u16(U16 value);
U32 xiaoai_transform_endian_u32(U32 value);

xiaoai_status_e xiaoai_speech_wake_up_app(xiaoai_bdaddr_t *pBdAddr);

xiaoai_status_e xiaoai_core_send_cmd_report_device_status(xiaoai_bdaddr_t * pBdAddr, U8 * ptr, U16 len);
U8 xiaoai_get_speech_state(void);

#endif //_BTXIAOAI_H_

