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


#ifndef _A2DP_H_
#define _A2DP_H_

#include "types.h"
#include "bt.h"
#include "A2dp_NVKeyHandler.h"

#ifdef _A2DP_C_
  #define A2DP_EXTERN
#else
  #define A2DP_EXTERN extern
#endif

#ifdef _A2DP_PARAMETER_C_
  #define A2DP_PARAMETER_EXTERN
#else
  #define A2DP_PARAMETER_EXTERN extern
#endif


/**************************************************************************************************
* Define
**************************************************************************************************/
#define A2DP_CAPS_MEDIA_TRANSPORT		0x01
#define A2DP_CAPS_CONTENT_PROTECTION	0x04
#define A2DP_CAPS_DELAY_REPORTING		0x08

#define MAX_A2DP_SEP_NUM	4

#define GAVDP_TIMER_100		(3*ONE_SEC)  // by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds
#define GAVDP_TIMER_INTERNAL	(2*ONE_SEC)  // by spec GAVDP 1.3 page 30 section 8. Tgavdp is proposed between 0.5~3 seconds

#define SBC_SNK_BITMAP 	0x01
#define AAC_SNK_BITMAP 	0x02
#define CELT_SNK_BITMAP 0x04
#define SBC_SRC_BITMAP 	0x08

#define SUPPORT_SBC_SNK		(a2dp_nvkey_get_support_codec() & SBC_SNK_BITMAP)
#define SUPPORT_AAC_SNK		(a2dp_nvkey_get_support_codec() & AAC_SNK_BITMAP)
#define SUPPORT_CELT_SNK	(a2dp_nvkey_get_support_codec() & CELT_SNK_BITMAP)
#define SUPPORT_SBC_SRC		(a2dp_nvkey_get_support_codec() & SBC_SRC_BITMAP)

#define A2DP_SEID_BITMAP  a2dp_nvkey_get_support_codec() // Bit 0: SBC_SNK, Bit 1: AAC_SNK, Bit2: CELT_SNK, Bit3: SBC_SRC
#define A2DP_SEID_NUM 2

#define	A2DP_SBC_SAMPLE_FREQ_16000		0x80
#define	A2DP_SBC_SAMPLE_FREQ_32000		0x40
#define	A2DP_SBC_SAMPLE_FREQ_44100		0x20
#define	A2DP_SBC_SAMPLE_FREQ_48000		0x10
#define A2DP_SBC_CHANNEL_MODE_MONO		0x08
#define A2DP_SBC_CHANNEL_MODE_DUAL		0x04
#define A2DP_SBC_CHANNEL_MODE_STEREO	0x02
#define A2DP_SBC_CHANNEL_MODE_JOINT_STEREO	0x01
#define	A2DP_SBC_BLOCK_LENGTH_4 		0x80
#define	A2DP_SBC_BLOCK_LENGTH_8 		0x40
#define	A2DP_SBC_BLOCK_LENGTH_12 		0x20
#define	A2DP_SBC_BLOCK_LENGTH_16 		0x10
#define A2DP_SBC_SUBBAND_NUMBER_4		0x08
#define A2DP_SBC_SUBBAND_NUMBER_8		0x04
#define A2DP_SBC_ALLOCATION_SNR			0x02
#define A2DP_SBC_ALLOCATION_LOUDNESS	0x01
#define A2DP_SBC_MIN_BITPOOL_VALUE		0x08	// dr chi suggests
#if defined(TWS_SETTINGS) && !defined(BLUETOOTH_SPEAKER)
#define A2DP_SBC_MAX_BITPOOL_VALUE  	0x23  // 35, AiroStereoHeadset
#else
#define A2DP_SBC_MAX_BITPOOL_VALUE  	0x35  // 53, by spec
#endif

#define A2DP_AAC_OBJECT_TYPE_MPEG2_AAC_LC			0x80
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_LC			0x40
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_LTP			0x20
#define A2DP_AAC_OBJECT_TYPE_MPEG4_AAC_SCALABLE		0x10

#define A2DP_AAC_SAMPLE_FREQ_8000		0x80 //byte 1
#define A2DP_AAC_SAMPLE_FREQ_11025		0x40
#define A2DP_AAC_SAMPLE_FREQ_12000		0x20
#define A2DP_AAC_SAMPLE_FREQ_16000		0x10
#define A2DP_AAC_SAMPLE_FREQ_22050		0x08
#define A2DP_AAC_SAMPLE_FREQ_24000		0x04
#define A2DP_AAC_SAMPLE_FREQ_32000		0x02
#define A2DP_AAC_SAMPLE_FREQ_44100		0x01
#define A2DP_AAC_SAMPLE_FREQ_48000		0x80  //byte2
#define A2DP_AAC_SAMPLE_FREQ_64000		0x40
#define A2DP_AAC_SAMPLE_FREQ_88200		0x20
#define A2DP_AAC_SAMPLE_FREQ_96000		0x10
#define A2DP_AAC_CHANNELS_1				0x08
#define A2DP_AAC_CHANNELS_2				0x04
#define A2DP_AAC_VARIABLE_BIT_RATE		0x80
#define A2DP_AAC_BIT_RATE				320000L //To be defined
#define A2DP_MAX_SEID_MATCH_NUM			6
#define A2DP_MAX_CAPS_MATCH_SIZE		26


//#define A2DP_SRC_Profile

#define CMD_WAITING 	FALSE
#define CMD_COMPLETE	TRUE

#define INVALID_CID 0
#define INVALID_CODEC 0
#define INVALID_SCMS 0

#define NO_CODEC_CHANGE 0xFF

#define DELAY_REPORT_MAX_DEVIATION			300 //unit: 0.1 ms
#define DELAY_REPORT_RECOMMEND_DEVIATION	150 //unit: 0.1 ms


/**************************************************************************************************
* Typedef
**************************************************************************************************/

typedef struct
{
	U8	txTrafficState;
	U8	relayCnt;
	U8	hwAudioChannel;
	//OST XDATA_PTR pktDroppedTimer;
	//OST XDATA_PTR badChannelSwithToGoodTimer;
	U8 	airFollowerDropPktCount;
	U8 	audioSource;
	U8 	audioSourceLinkIndex;
	//OST XDATA_PTR delaySendSuspendTimer;
}A2DP_RELAY_CTL;

typedef	struct
{
	A2DP_RELAY_CTL relayCtl;
	U8 gDebugPringCnt;
	U8 gNoRxMemCnt;
	U8 openCodec;
}A2DP_CTL_STRU;

typedef struct
{
	U8 tsep;
	U8 preferSeid;
	U8 count;
	U8 index;
	U8 seid[A2DP_MAX_SEID_MATCH_NUM];
	U8 acpSeid;
	U8 intSeid;
	U8 caps;
	U8 configLen;
	U8 configParam[A2DP_MAX_CAPS_MATCH_SIZE];
}A2dpDiscovSeidInfo;

typedef struct
{
	U8 rfa:1;
	U8 inUse:1;
	U8 seid:6;
	U8 rfa2:3;
	U8 tsep:1;
	U8 mediaType:4;
}A2DP_SEID_INFO;

typedef struct
{
	U8 sbcCodecInfo;
	U16 aacCodecInfo;
}A2DP_CLIENT_CODEC_INFO_STRU;
/**************************************************************************************************
* Enum define
**************************************************************************************************/
enum
{
	A2DP_SEP_SRC,
	A2DP_SEP_SNK
};

enum
{
	A2DP_AUDIO,
	A2DP_VIDEO,
	A2DP_MULTIMEDIA,
};

enum
{
	MEDIA_TRANSPORT 	= 0x01,
	REPORTING			= 0x02,
	RECOVERY			= 0x03,
	CONTENT_PROTECTION	= 0x04,
	HEADER_COMPRESSION	= 0x05,
	MULTIPLEXING		= 0x06,
	MEDIA_CODEC			= 0x07,
	DELAY_REPORTING		= 0x08,
};

enum
{
	AUDIO_TYPE = 0x00,
	VIDEO_TYPE = 0x01,
	MULTIMEDIA_TYPE = 0x02,
};

enum
{
	SBC_CODEC_TYPE 			= 0x00,
	MP3_CODEC_TYPE			= 0x01,
	AAC_CODEC_TYPE			= 0x02,
	ATRAC_CODEC_TYPE		= 0x04,
	CELT_CODEC_TYPE			= 0x05,
	NON_A2DP_CODEC_TYPE		= 0xFF,
};

enum
{
	A2DP_CP_NONE,
	A2DP_CP_DTCP,
	A2DP_CP_SCMS_T
};

enum
{
	A2DP_CHANNEL_SUSPEND_FAIL_EVT = 0x04,
	A2DP_CHANNEL_FOLLOWER_AUX,
	A2DP_CHANNEL_FOLLOWER_MUSIC,
	A2DP_CHANNEL_SOURCE_START_EVT,
	A2DP_CHANNEL_SOURCE_SUSPEND_EVT,
};

enum
{
	A2DP_TO_APP_NO_ERROR = 0,
	A2DP_TO_APP_RECONFIGURE_RESP_ACCEPT,
	A2DP_TO_APP_RECONFIGURE_RESP_REJECT,
	A2DP_TO_APP_RECONFIGURE_RESP_CONTINUE_ANOTHER_SEID,
	A2DP_TO_APP_GET_CAPABILITIES_RESP_ACCEPT,
	A2DP_TO_APP_GET_CAPABILITIES_RESP_REJECT,
	A2DP_TO_APP_DISCOVER_RESP_ACCEPT,
	A2DP_TO_APP_DISCOVER_RESP_REJECT,
};


/**************************************************************************************************
* Public api
**************************************************************************************************/
void 		a2dp_set_handle(Handler app);
Handler 	a2dp_get_app_handler(void);
U8 			a2dp_get_service_state(void);
void 		a2dp_set_service_state(U8 serviceState);
void 		a2dp_set_sdp_service_handle(U32 serviceHandle);
U32 		a2dp_get_sdp_service_handle(void);
void 		a2dp_clear_service_ctl(void);
void        a2dp_set_use_client_bit_rate(BOOL isOn);
BOOL 		a2dp_is_AAC_allowed_in_link(BD_ADDR_T *pBdAddr);
BOOL 		a2dp_is_resume(BD_ADDR_T *pBdAddr);
void 		a2dp_set_resume(BD_ADDR_T *pBdAddr, BOOL isResume);
void 		a2dp_set_link_used_codec(BD_ADDR_T *pBdAddr, U8 preferCodec);
void 		a2dp_set_client_codec_Info(U8 codecType, U16 codecInfo);
U8 			a2dp_get_SBC_sample_rate_index(BD_ADDR_T *pBdAddr);
U8 			a2dp_get_AAC_sample_rate_index(BD_ADDR_T *pBdAddr);
BOOL 		a2dp_is_delay_report_needed(void);
U16			a2dp_get_dsp_buffer_depth(void);
U32			a2dp_get_dsp_delay_report_value(void);
HandlerData *a2dp_get_stream_handler(void);

#endif

