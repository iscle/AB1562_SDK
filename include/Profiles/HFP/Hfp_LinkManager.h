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
 
 
#ifndef _HFP_LINKCONTROL_HH_
#define _HFP_LINKCONTROL_HH_

#include "hfp.h"
#include "BtHfpHsp_Settings.h"
#include "PM_Connection.h"

/**************************************************************************************************
* Define
**************************************************************************************************/
//Following defines the optional functions of AG
#define AG_BRSF_THREEWAY_CALLING 			0X0001
#define AG_BRSF_EC_NR 						0X0002
#define AG_BRSF_VOICE_RECOGNITION 			0X0004
#define AG_BRSF_INBAND_RINGTONE 			0X0008 //in-band ringtone capability
#define AG_BRSF_VOICE_TAG					0X0010 //attach a no to a voice tag
#define AG_BRSF_REJECT_CALL					0X0020 //ability to reject a call
#define AG_BRSF_ENHCALL_STATUS				0X0040 //enhanced call status
#define AG_BRSF_ENHCALL_CTRL				0X0080 //enhanced call control
#define AG_BRSF_ENHERR_RESCODE				0X0100 //enhanced error result codes
#define AG_BRSF_CODEC_NEGOTIATION			0X0200 //codec negotiation 
#define AG_BRSF_HF_INDICATORS				0X0400 //HF indicators 

#define HF_BRSF_ECNR						0x0001
#define HF_BRSF_CALL_WAITING_3WAYCALLING	0x0002
#define HF_BRSF_CLI_PRESENTATION			0x0004
#define HF_BRSF_VOICE_RECOGNITION			0x0008
#define HF_BRSF_REMOTE_VOLUME_CONTROL		0x0010
#define HF_BRSF_ENHANCED_CALL_STATUS		0x0020
#define HF_BRSF_ENHANCED_CALL_CONTROL		0x0040
#define HF_BRSF_CODEC_NEGOTIATION			0x0080
#define HF_BRSF_INDICATORS					0x0100
#define HF_BRSF_ESCO_S4_SUPPORT				0x0200

#define AG_SUPPORT_MASK_CHLD_0				0x01
#define AG_SUPPORT_MASK_CHLD_1				0x02
#define AG_SUPPORT_MASK_CHLD_1x				0x04
#define AG_SUPPORT_MASK_CHLD_2				0x08
#define AG_SUPPORT_MASK_CHLD_2x				0x10
#define AG_SUPPORT_MASK_CHLD_3				0x20
#define AG_SUPPORT_MASK_CHLD_4				0x40

#define HFP_PUSH_CMD_NO		10
#define	HFP_MAX_XBAT_LEVEL	9

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	HFP_CMD_STATE_IDLE,					//0
	HSP_CMD_STATE_SLC_ESTABLISH,
	HFP_CMD_STATE_SLC_ESTABLISH,
	HFP_CMD_STATE_ATA,
	HFP_CMD_STATE_ATD_NUMBER,
	HFP_CMD_STATE_AT_APLEFM_ENABLE,		//5
	HFP_CMD_STATE_AT_APLEFM_DISABLE,	
	HFP_CMD_STATE_AT_APLSIRI,
	HFP_CMD_STATE_AT_BAC,
	HFP_CMD_STATE_AT_BCC,
	HFP_CMD_STATE_AT_BCS,				//10
	HFP_CMD_STATE_AT_BIA,				
	HFP_CMD_STATE_AT_BIEV_BAT_SEND,
	HFP_CMD_STATE_AT_BIND,
	HFP_CMD_STATE_AT_BIND_EQUAL_QUES,
	HFP_CMD_STATE_AT_BINP,				//15
	HFP_CMD_STATE_AT_BLDN,				
	HFP_CMD_STATE_AT_BRSF,
	HFP_CMD_STATE_AT_BTRH_QUES,
	HFP_CMD_STATE_AT_BTRH_ACCEPT,
	HFP_CMD_STATE_AT_BTRH_REJECT,		//20
	HFP_CMD_STATE_AT_BVRA_ENABLE,		
	HFP_CMD_STATE_AT_BVRA_DISABLE,
	HFP_CMD_STATE_AT_CHLD_EQUAL_QUES,
	HFP_CMD_STATE_AT_CHLD,
	HFP_CMD_STATE_AT_CHUP,				//25
	HFP_CMD_STATE_AT_CIND_EQUAL_QUES,	
	HFP_CMD_STATE_AT_CIND_QUES,			
	HSP_CMD_STATE_AT_CKPD200,
	HFP_CMD_STATE_AT_CLCC,
	HFP_CMD_STATE_AT_CMEE_ENABLE,		//30
	HFP_CMD_STATE_AT_CMEE_DISABLE,
	HFP_CMD_STATE_AT_CMER,				
	HFP_CMD_STATE_AT_CNUM,
	HFP_CMD_STATE_AT_CNUM_QUES,
	HFP_CMD_STATE_AT_CLIP_ENABLE,		//35
	HFP_CMD_STATE_AT_CLIP_DISABLE,
	HFP_CMD_STATE_AT_CCWA_ENABLE,		
	HFP_CMD_STATE_AT_CCWA_DISABLE,
	HFP_CMD_STATE_AT_CSCS,
	HFP_CMD_STATE_AT_COPS,				//40
	HFP_CMD_STATE_AT_COPS_QUES,
	HFP_CMD_STATE_AT_IPHONE_BAT_SEND,	
	HFP_CMD_STATE_AT_NREC_ENABLE,
	HFP_CMD_STATE_AT_NREC_DISABLE,
	HFP_CMD_STATE_AT_VGS,				//45
	HFP_CMD_STATE_AT_VGM,
	HFP_CMD_STATE_AT_VTS,				
	HFP_CMD_STATE_AT_XAPL,
	HFP_CMD_STATE_AT_XEVENT_BAT_INQ,
	HFP_CMD_STATE_AT_XEVENT_BAT_SEND,	//50
    HFP_CMD_STATE_ATD_DIGIT,
	HFP_CMD_STATE_AT_XIAOMI_STATUS,
	HFP_CMD_STATE_AT_VENDOR,
};

enum
{
	HFP_CMD_SUBSTATE_IDLE,
	HFP_SLC_CMD_SUBSTATE_SEND_BRSF_CMD,
	HFP_SLC_CMD_SUBSTATE_WAIT_PLUS_BRSF,
	HFP_SLC_CMD_SUBSTATE_WAIT_BRSF_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_BAC_CMD,
	HFP_SLC_CMD_SUBSTATE_WAIT_BAC_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_CIND_EQU_QUES,
	HFP_SLC_CMD_SUBSTATE_WAIT_PLUS_CIND,
	HFP_SLC_CMD_SUBSTATE_WAIT_CIND_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_CIND_QUES,
	HFP_SLC_CMD_SUBSTATE_WAIT_2ND_PLUS_CIND,
	HFP_SLC_CMD_SUBSTATE_2ND_CIND_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_CMER,
	HFP_SLC_CMD_SUBSTATE_WAIT_CMER_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_CHLD_EQU_QUES,
	HFP_SLC_CMD_SUBSTATE_WAIT_PLUS_CHLD,
	HFP_SLC_CMD_SUBSTATE_WAIT_CHLD_EQU_QUES_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_BIND,
	HFP_SLC_CMD_SUBSTATE_WAIT_BIND_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_BIND_EQUAL_QUES,
	HFP_SLC_CMD_SUBSTATE_WAIT_BIND_EQUAL_QUES_OK,
	HFP_SLC_CMD_SUBSTATE_SEND_BIND_QUES,
	HFP_SLC_CMD_SUBSTATE_WAIT_BIND_QUES_OK,
	
	HFP_COPS_CMD_SUBSTATE_WAIT_PLUS_COPS,
	HFP_COPS_CMD_SUBSTATE_GOT_PLUS_COPS,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct
{
	U8 pushIndex;
	U8 popIndex;
	U8 cmdCode[HFP_PUSH_CMD_NO];
}HFP_PUSH_CMD_CTL_STRU;

typedef struct hfp_link_stru HFP_LINK_STRU ;

struct hfp_link_stru 
{
	BD_ADDR_T bdAddr;
	PROFILE_INFO profileInfo;
	U8 remoteHspScn;
	U8 remoteHfpScn;
	U8 serverChannel;
	SINK sink;
	U8 cmdState;
	U8 cmdSubstate;

	void (*resultCodeFunc)(HFP_LINK_STRU * pLinkInfo, U8 *dataPtr, U8 index, U32 dataLength);

	U8 spkTargetVolume:4;
	U8 spkSentVolume:4;
	U8 micTargetVolume:4;
	U8 micSentVolume:4;
	
	U8 vgsWaitingOK:1;
	U8 vgmWaitingOK:1;
	U8 SCOCodec:2;
	U8 xeventBatEnable:1;
	U8 xaplBatEnable:1;
	U8 bievBatEnable:1;
	U8 inbandRingtone:1;

	U16 remoteBRSF;
	U8 chldSupportMask;
	U32 cindBitMask;
	U8 appleSiriStatus;
	HFP_IND_TABLE_STRU indTable;
	HFP_PUSH_CMD_CTL_STRU * pPushedCmdCtl;
	U16 atPayloadNum;
	U8  *pAtPayload;
	U8  xaplBatLv;
	U8  xeventBatLv;
	U8  bievBatLv;
};

/**************************************************************************************************
* Public functions
**************************************************************************************************/
HFP_LINK_STRU * hfp_GetLinkByBdAddr(BD_ADDR_T *pBdAddr);
HFP_LINK_STRU * hfp_GetEmptyLink(void);
HFP_LINK_STRU * hfp_GetLinkBySink(SINK sink);
BD_ADDR_T * 	hfp_GetBdAddrByHfpLinkInfo(HFP_LINK_STRU * pHfpLinkInfo);
void 			hfp_SetLinkState(HFP_LINK_STRU * pLinkInfo, U8 state);
U8 				hfp_GetLinkState(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetLinkBdAddr(HFP_LINK_STRU * pLinkInfo, BD_ADDR_T * pBdAddr);
void 			hfp_SetLinkSink(HFP_LINK_STRU * pLinkInfo, SINK sink);
SINK			hfp_GetLinkSink(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetLinkSCOCodec(HFP_LINK_STRU * pLinkInfo, U8 codec);
U8 				hfp_GetLinkSCOCodec(HFP_LINK_STRU * pLinkInfo);
void 			hfp_ClearLink(HFP_LINK_STRU * pLinkInfo);
BOOL 			hfp_IsRemoteHspScnReady(HFP_LINK_STRU * pLinkInfo);
BOOL 			hfp_IsRemoteHfpScnReady(HFP_LINK_STRU * pLinkInfo);
U8 				hfp_GetRemoteHspScn(HFP_LINK_STRU * pLinkInfo);
U8 				hfp_GetRemoteHfpScn(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetRemoteHspScn(HFP_LINK_STRU * pLinkInfo, U8 scn);
void 			hfp_SetRemoteHfpScn(HFP_LINK_STRU * pLinkInfo, U8 scn);
void 			hfp_SetLinkCmdState(HFP_LINK_STRU * pLinkInfo, U8 cmdState);
U8 				hfp_GetLinkCmdState(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetLinkCmdSubstate(HFP_LINK_STRU * pLinkInfo, U8 cmdSubstate);
U8 				hfp_GetLinkCmdSubstate(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetServerChannel(HFP_LINK_STRU * pLinkInfo, U8 serverChannel);
U8 				hfp_GetServerChannel(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetResultCodeFunc(HFP_LINK_STRU * pLinkInfo, void const * func);
void 			hfp_SetBRSFFeature(HFP_LINK_STRU * pLinkInfo, U16 feature);
U16 			hfp_GetBRSFFeature(HFP_LINK_STRU * pLinkInfo);
void			hfp_SetAppleSiriStatus(HFP_LINK_STRU * pLinkInfo, U8 siriStatus);
U8				hfp_GetAppleSiriStatus(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetXeventBatEnable(HFP_LINK_STRU * pLinkInfo, U8 isEnable);
BOOL			hfp_IsXeventBatEnable(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetXeventBatLevel(HFP_LINK_STRU * pLinkInfo, U8 batLevel);
U8 				hfp_GetXeventBatLevel(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetXaplBatEnable(HFP_LINK_STRU * pLinkInfo, U8 isEnable);
BOOL 			hfp_IsXaplBatEnable(HFP_LINK_STRU * pLinkInfo);
void			hfp_SetXaplBatLevel(HFP_LINK_STRU * pLinkInfo, U8 batLevel);
U8 				hfp_GetXaplBatLevel(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetBIEVBatEnable(HFP_LINK_STRU * pLinkInfo, U8 isEnable);
BOOL 			hfp_IsBIEVBatEnable(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetBievBatLevel(HFP_LINK_STRU * pLinkInfo, U8 batLevel);
U8 				hfp_GetBievBatLevel(HFP_LINK_STRU * pLinkInfo);
void 			hfp_SetInbandRingtoneEnable(HFP_LINK_STRU * pLinkInfo, U8 isEnable);
BOOL 			hfp_IsInbandRingtoneEnable(HFP_LINK_STRU * pLinkInfo);
BOOL			hfp_IsAGFeatureSupport(HFP_LINK_STRU * pLinkInfo, U16 feature);
void 			hfp_RecordIndicator(HFP_LINK_STRU * pLinkInfo, U8 localInd, U8 remoteInd);
void 			hfp_RecordIndicatorValue(HFP_LINK_STRU * pLinkInfo, U8 localInd, U8 value);
U8 				hfp_FindLocalIndicatorByRemoteIndicator(HFP_LINK_STRU * pLinkInfo, U8 remoteInd);
void			hfp_SetCindMask(HFP_LINK_STRU * pLinkInfo, U32 cindBitMask);
void			hfp_AddChldSupportMask(HFP_LINK_STRU * pLinkInfo, U8 mask);
U8 				hfp_SDAPParseRemoteScn(BT_SDP_SERVICE_SEARCH_ATTRIBUTE_CFM_T * evtPtr);
U8* 			hfp_GetTxPtr(SINK sink, U32 claim_size);

/**************************************************************************************************
* Public Functions (RHO)
**************************************************************************************************/
HFP_LINK_STRU *HFP_GetAgLinkInfo(void);


#endif //_HFP_LINKCONTROL_HH_
