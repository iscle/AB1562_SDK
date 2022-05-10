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


#ifndef _A2DP_LINK_MANAGER_H_
#define _A2DP_LINK_MANAGER_H_

	#include "PM_Connection.h"
	#include "profile_a2dp_parameter_nvstruc.h"
	
	
	#if 0 /* Unused LOG_PRINT_156X */
	#define DBG_LOG_PROFILE_A2DP(_message, arg_cnt, ...)    printf(_message, ##__VA_ARGS__)
	#else	
	#define DBG_LOG_PROFILE_A2DP(_message, arg_cnt, ...)    LOG_MSGID_I(LIB_A2DP, _message, arg_cnt, ##__VA_ARGS__)
	#endif

	enum
	{
		A2DP_ACTIVE_CONN_INIT_STATE = 0,
		A2DP_ACTIVE_SET_CLOSE_A2DP_STATE,	
		A2DP_ACTIVE_SEND_CLOSE_A2DP_COMMAND_STATE,
		A2DP_ACTIVE_SEND_CLOSE_MEDIA_COMMAND_STATE,
		A2DP_ACTIVE_SET_DISCOVER_TIME_STATE,
		A2DP_ACTIVE_SEND_DISCOVER_COMMAND_STATE,
<<<<<<< HEAD
	}; // CODEC_SWITCH_WORKAROUND_MI9:A2DP ACTIVE CONN WORK-AROUND STATE
=======
	}; // AIR_CODEC_SWITCH_WORKAROUND_MI9_ENABLE:A2DP ACTIVE CONN WORK-AROUND STATE
>>>>>>> db20e11 (second commit)


	#define A2DP_DELAY_HANDLE_COMMAND_ID_BASE 0x1000
	enum
	{
		CHECK_A2DP_MEDIA_CHANNEL_EST_TIME = A2DP_DELAY_HANDLE_COMMAND_ID_BASE,
		COMMON_A2DP_TIME,
<<<<<<< HEAD
		CLOSE_A2DP_TIME, //CODEC_SWITCH_WORKAROUND_MI9
		ACTIVE_DISCOVER_TIME,//CODEC_SWITCH_WORKAROUND_MI9
=======
		CLOSE_A2DP_TIME, //AIR_CODEC_SWITCH_WORKAROUND_MI9_ENABLE
		ACTIVE_DISCOVER_TIME,//AIR_CODEC_SWITCH_WORKAROUND_MI9_ENABLE
>>>>>>> db20e11 (second commit)
	};
	enum
	{
		A2DP_CONFIGURED = PROFILE_STATE_START,
		A2DP_OPEN,
		A2DP_STREAMING, //L2CAP channel for signaling is already open.
		A2DP_CLOSING,
		A2DP_ABORTING,
		A2DP_INVALID_STATE = 0xFF,
	};

	enum
	{
		A2DP_SUBSTATE_BEGINNING_STATE,
		A2DP_SUBSTATE_IDLE_WAIT_DISCOVER_RESP,
		A2DP_SUBSTATE_IDLE_WAIT_GET_CAPABILITY_RESP,
		A2DP_SUBSTATE_IDLE_WAIT_SET_CONFIG_RESP,
		A2DP_SUBSTATE_CONFIG_WAIT_OPEN_RESP,
		A2DP_SUBSTATE_OPEN_WAIT_MMI_DSP_RESP,   //5
		A2DP_SUBSTATE_OPEN_MMI_NO_RESP_REPLY_FAIL,
		A2DP_SUBSTATE_OPEN_WAIT_MEDIA_CHANNEL_READY,
		A2DP_SUBSTATE_OPEN_WAIT_SEND_MEDIA_CHANNEL,
		A2DP_SUBSTATE_OPEN_WAIT_START,
		A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE_AFTER_ACTIVE_MEDIA_CHANNEL,
		A2DP_SUBSTATE_OPEN_WAIT_START_RESPONSE,
		A2DP_SUBSTATE_OPEN_WAIT_SUSPEND,
		A2DP_SUBSTATE_OPEN_SEND_SUSPEND,
		A2DP_SUBSTATE_OPEN_WAIT_SUSPEND_RESPONSE,
		A2DP_SUBSTATE_OPEN_WAIT_SEND_CLOSE,
		A2DP_SUBSTATE_STREAM_GET_MMI_DSP_RESP_WAIT_AIR_READY,
		A2DP_SUBSTATE_STREAM_WAIT_DSP_ENABLE_FOR_START_RESP, //20  //tws
		A2DP_SUBSTATE_STREAM_WAIT_MMI_DSP_RESP,
		A2DP_SUBSTATE_STREAM_WAIT_SUSPEND_RESP,
		A2DP_SUBSTATE_STREAM_SEND_SUSPEND_REQ_FOR_TWS,
		A2DP_SUBSTATE_CLOSE_WAIT_CLOSE_RESP,
		A2DP_SUBSTATE_CLOSE_CLOSE_MEDIA_CHANNEL,
		A2DP_SUBSTATE_CLOSE_GET_CLOSE_RESP_WAIT_CLOSE_MEDIA_CHANNEL,
		A2DP_SUBSTATE_ABORT_WAIT_PASSIVE_RELEASE_MEDIA_CHANNEL,
		A2DP_SUBSTATE_ABORT_WAIT_ACTIVE_RELEASE_MEDIA_CHANNEL,
		A2DP_SUBSTATE_ABORT_WAIT_ABORT_RESP,
		A2DP_SUBSTATE_STREAM_WAIT_RECONFIGURE_RESP,
		A2DP_SUBSTATE_WAIT_GET_CONFIGURATION,
		A2DP_SUBSTATE_INVALID = 0xFF,
	};

	typedef struct
	{
		U8 avdtpSuspendStartPara;
	}A2DP_SIGNAL_CMD_STRU;

	typedef struct
	{
		BD_ADDR_T bdAddr;
		U8 codec;
	}A2DP_LINK_CODEC_STRU;

	typedef struct
	{
		SBCCodecSettings sbc;
		AACCodecSettings aac;
		U8 vendor[13];
	}CODEC_SETTINGS_STRU;

	#define RECONFIG_DATA_MAX	10
	typedef struct
	{
		U8 remoteSeid;
		U16 dataLength;
		U8 data[RECONFIG_DATA_MAX];
	}RECONFIGURE_SETTING_STRU;

	typedef struct
	{
		BD_ADDR_T bdAddr;
		PROFILE_INFO profileInfo;
		A2DP_SIGNAL_CMD_STRU cmdPara;
		U8 substate;
		U8 *discovSeidInfoPtr;
		U8 isPassive:1;
		U8 isSdpReady:1;
		U8 isResume:1;
		U8 mediaChannelConnected:1;
		U8 enableDelayReport:1;
		U8 enableSCMS:1;
<<<<<<< HEAD
		U8 reconfigStatus:1; //CODEC_SWITCH_WORKAROUND_SONY_XPERIA
=======
		U8 reconfigStatus:1; //AIR_CODEC_SWITCH_WORKAROUND_SONY_XPERIA_ENABLE
>>>>>>> db20e11 (second commit)
		U8 reserved:1;
		U8 localSeid;
		U8 remoteSeid;
		U8 type;
		U8 codec;
		U8 pendingSignalID;
		U16 delayReportValue;
<<<<<<< HEAD
		U8 changeActiveConn; //CODEC_SWITCH_WORKAROUND_MI9
=======
		U8 changeActiveConn; //AIR_CODEC_SWITCH_WORKAROUND_MI9_ENABLE
>>>>>>> db20e11 (second commit)

		CODEC_SETTINGS_STRU codecSettings;
		RECONFIGURE_SETTING_STRU reconfigSetting;
	}A2DP_LINK_STRU;

	A2DP_LINK_STRU* 	a2dp_get_link_by_bdaddr(BD_ADDR_T *pBdAddr);
	A2DP_LINK_STRU* 	a2dp_get_empty_link(void);
	void 				a2dp_set_link_state(A2DP_LINK_STRU * pLinkInfo, U8 state);
	U8 					a2dp_get_link_state(A2DP_LINK_STRU * pLinkInfo);
	void 				a2dp_set_link_substate(A2DP_LINK_STRU * pLinkInfo, U8 substate);
	U8 					a2dp_get_link_substate(A2DP_LINK_STRU * pLinkInfo);
	void 				a2dp_set_link_bdaddr(A2DP_LINK_STRU *pLinkInfo, BD_ADDR_T * pBdAddr);

	void 				a2dp_clear_substate_and_timer(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_clear_active_connection_state(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_print_link_state_and_substate(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_set_media_data_play(A2DP_LINK_STRU *pLinkInfo, BOOL isPlay);
	void 				a2dp_command_complete(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_set_timer(A2DP_LINK_STRU *pLinkInfo, U16 id, U32 delay);
	void 				a2dp_cancel_timer(A2DP_LINK_STRU *pLinkInfo, U16 id);
	void 				a2dp_cancel_all_timer(A2DP_LINK_STRU *pLinkInfo);
	BOOL 				a2dp_check_timer(A2DP_LINK_STRU *pLinkInfo, U16 id);
	void				a2dp_clear_link(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_set_sdp_ready(A2DP_LINK_STRU *pLinkInfo, U8 isReady);
	BOOL 				a2dp_is_remote_sdp_ready(A2DP_LINK_STRU *pLinkInfo);
	void 				a2dp_get_link_SBC_setting(BD_ADDR_T* pBdAddr, SBCCodecSettings *sbc);
	void				a2dp_get_link_AAC_setting(BD_ADDR_T * pBdAddr,AACCodecSettings * aac);
	void 				a2dp_set_link_delay_report_value(BD_ADDR_T* pBdAddr, U16 delayValue);
	U16 				a2dp_get_link_delay_report_value(BD_ADDR_T* pBdAddr);

/**************************************************************************************************
* Internal Public Functions (RHO)
**************************************************************************************************/
A2DP_LINK_STRU *a2dp_get_Ag_link_info();

#endif //_A2DP_LINK_MANAGER_H_
