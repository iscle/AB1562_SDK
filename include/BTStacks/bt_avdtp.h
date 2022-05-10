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


#ifndef _BT_AVDTP_H_
#define _BT_AVDTP_H_

#include "transform.h"


#define AVDTP_SIGNAL_IDENTIFIER_BITS_FIELD 0x3F
#define AVDTP_GET_UNACCEPT_RESP			0xFF
#define AVDTP_GET_ACCEPT_RESP			0x01

enum
{
	AVDTP_DISCOVER = 0x01,
	AVDTP_GET_CAPABILITIES,
	AVDTP_SET_CONFIGURATION,
	AVDTP_GET_CONFIGURATION,
	AVDTP_RECONFIGURE,
	AVDTP_OPEN,
	AVDTP_START,
	AVDTP_CLOSE,
	AVDTP_SUSPEND,
	AVDTP_ABORT,
	AVDTP_SECURITY_CONTROL,
	AVDTP_GET_ALL_CAPABILITIES,
	AVDTP_DELAYREPORT,
	AVDTP_LAST_SIGNALING
};

enum
{
	AVDTP_NO_ERROR,

	//ACP to INT, Signal Response header Error Code
	AVDTP_BAD_HEADER_FORMAT = 0x01,	//All signals

	//ACP to INT, Signal Response Payload Error Codes
	AVDTP_BAD_LENGTH = 0x11,		//All signals
	AVDTP_BAD_ACP_SEID,				//All signals
	AVDTP_SEP_IN_USE,				//Support in "Set Configuration" signal only.
	AVDTP_SEP_NOT_IN_USE,			//Support in "ReConfigure" signal only.
	AVDTP_BAD_SERV_CATEGORY = 0x17,	//Support in "Set Configuration and "ReConfigure"" signals only.
	AVDTP_BAD_PAYLOAD_FORMAT,		//All signals
	AVDTP_NOT_SUPPORT_COMMAND,		//All signals
	AVDTP_INVALID_CAPABILITIES,		//All signals

	//ACP to INT, Signal Response Transport Service Capabilities Error Codes
	AVDTP_BAD_RECOVERY_TYPE = 0x22,
	AVDTP_BAD_MEDIA_TRANSPORT_FORMAT,
	AVDTP_BAD_RECOVERY_FORMAT = 0x25,
	AVDTP_BAD_ROHC_FORMAT,
	AVDTP_BAD_CP_FORMAT,
	AVDTP_BAD_MULTIPLEXING_FORMAT,
	AVDTP_UNSUPPORTED_CONFIGURATION,

	//ACP to INT, Procedure Error Codes
	AVDTP_BAD_STATE = 0x31,
};

enum
{
	AVDTP_SIGNAL_CHANNEL_TYPE,
	AVDTP_MEDIA_CHANNEL_TYPE,
	AVDTP_CHANNEL_TYPE_NUM,
};

enum
{
	AVDTP_COMMAND_MSG,
	AVDTP_GENERAL_REJECT_MSG,
	AVDTP_RESPONSE_ACCEPT_MSG,
	AVDTP_RESPONSE_REJECT_MSG,
};

enum
{
	AVDTP_SINGLE_PACKET,
	AVDTP_START_PACKET,
	AVDTP_CONTINUE_PACKET,
	AVDTP_END_PACKET,
};

/** AVDTP **/

typedef struct BT_AVDTP_REGISTER_CFM
{
	U8 status;
}BT_AVDTP_REGISTER_CFM_T;

typedef struct BT_AVDTP_UNREGISTER_CFM
{
	U8 status;
}BT_AVDTP_UNREGISTER_CFM_T;

typedef struct BT_AVDTP_SIGNAL_CONNECT_CFM
{
	U8 status;
	BD_ADDR_T bdAddr;
}BT_AVDTP_SIGNAL_CONNECT_CFM_T;

typedef struct BT_AVDTP_SIGNAL_CONNECT_IND
{
	BD_ADDR_T 	bdAddr;
	U16 		psm;
	U16 		channelId;
	U8 			identifier;
}BT_AVDTP_SIGNAL_CONNECT_IND_T;

typedef struct BT_AVDTP_SIGNAL_DISCONNECT_CFM
{
	U8 status;
	BD_ADDR_T bdAddr;
}BT_AVDTP_SIGNAL_DISCONNECT_CFM_T;

typedef struct BT_AVDTP_SIGNAL_DISCONNECT_IND
{
	U8 identifier;
	SINK sink;
	U8 status;
}BT_AVDTP_SIGNAL_DISCONNECT_IND_T;

typedef struct BT_AVDTP_MEDIA_CONNECT_CFM
{
	U8 status;
	U8 cid;
	BD_ADDR_T bdAddr;
}BT_AVDTP_MEDIA_CONNECT_CFM_T;

typedef struct BT_AVDTP_MEDIA_CONNECT_IND
{
	U8 identifier;
	U16 channelId;
	BD_ADDR_T bdAddr;
}BT_AVDTP_MEDIA_CONNECT_IND_T;

typedef struct BT_AVDTP_MEDIA_DISCONNECT_CFM
{
	U8 status;
	BD_ADDR_T bdAddr;
}BT_AVDTP_MEDIA_DISCONNECT_CFM_T;



// AVDTP MSG -> A2DP -> GAVDP
typedef struct BT_AVDTP_SIGNAL_DISCOVER_CMD_IND_T
{
	BD_ADDR_T bdAddr;
}BT_AVDTP_SIGNAL_DISCOVER_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_CAPABILITIES_CMD_IND_T
{
	U8 acpSEID;
	U32 sourceSize;
	BD_ADDR_T bdAddr;
}BT_AVDTP_SIGNAL_GET_CAPABILITIES_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_CMD_IND_T
{
	U8 acpSEID;
	U32 sourceSize;
	BD_ADDR_T bdAddr;
}BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_SET_CONFIGURATION_CMD_IND_T
{
	U8 intSEID;
	U8 acpSEID;
	BD_ADDR_T bdAddr;
	U32 sourceSize;
	U8 configData[1];
}BT_AVDTP_SIGNAL_SET_CONFIGURATION_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_CONFIGURATION_CMD_IND_T
{
	U8 acpSEID;
	BD_ADDR_T bdAddr;
	U32 sourceSize;
}BT_AVDTP_SIGNAL_GET_CONFIGURATION_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_RECONFIGURE_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U32 sourceSize;
	U16 configTypeLength;
	U8 acpSEID;
	U8 configData[1];
}BT_AVDTP_SIGNAL_RECONFIGURE_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_OPEN_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U32 sourceSize;
	U8 acpSEID;
}BT_AVDTP_SIGNAL_OPEN_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_START_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U8 acpSEID;
	U32 sourceSize;
	U8 signalPDU[1];
}BT_AVDTP_SIGNAL_START_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_CLOSE_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U8 acpSEID;
	U32 sourceSize;
	U8 signalPDU[1];
}BT_AVDTP_SIGNAL_CLOSE_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_SUSPEND_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U8 acpSEID;
	U32 sourceSize;
	U8 signalPDU[1];
}BT_AVDTP_SIGNAL_SUSPEND_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_ABORT_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U32 sourceSize;
	U8 acpSEID;
}BT_AVDTP_SIGNAL_ABORT_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_SECURITY_CONTROL_CMD_IND_T
{
	U8 acpSEID;
	BD_ADDR_T bdAddr;
	U32 sourceSize;
	U8 para[1];
}BT_AVDTP_SIGNAL_SECURITY_CONTROL_CMD_IND_T;

typedef struct BT_AVDTP_SIGNAL_DELAY_REPORT_CMD_IND_T
{
	BD_ADDR_T bdAddr;
	U32 sourceSize;
}BT_AVDTP_SIGNAL_DELAY_REPORT_CMD_IND_T;


/*=============================================
AVDTP signal command response accept
AVDTP -> GAVDP
==============================================*/

typedef struct BT_AVDTP_SIGNAL_DISCOVER_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
	U32 dataSize;
	U8 data[1];
}BT_AVDTP_SIGNAL_DISCOVER_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_CAPABILITIES_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
	U32 dataSize;
	U8 data[1];
}BT_AVDTP_SIGNAL_GET_CAPABILITIES_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_SET_CONFIGURATION_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_SET_CONFIGURATION_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_OPEN_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_OPEN_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_START_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_START_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_SUSPEND_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_SUSPEND_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
	U32 dataSize;
	U8 data[1];
}BT_AVDTP_SIGNAL_GET_ALL_CAPABILITIES_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_RECONFIGURE_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_RECONFIGURE_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_GET_CONFIGURATION_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_GET_CONFIGURATION_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_CLOSE_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_CLOSE_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_ABORT_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_ABORT_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_DELAY_REPORT_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_DELAY_REPORT_RSP_IND_T;

typedef struct BT_AVDTP_SIGNAL_SECURITY_CONTROL_RSP_IND_T
{
	BD_ADDR_T bdAddr;
	U8 status;
}BT_AVDTP_SIGNAL_SECURITY_CONTROL_RSP_IND_T;

typedef struct
{
	U8 rfa2:1;			//First para if it exists.
	U8 inUse:1;
	U8 firstAcpSEID:6;
	U8 rfa3:3;
	U8 tsep:1;
	U8 mediaType:4;
}ACP_SEID_INFO_STRU;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
}AVDTP_SIGNAL_HEADER_STRU;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 para[1];		//place holder
}AvdtpSignalType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSEID;
	U8 intSEID;
	U8 configData[1];
}AvdtpSignalConfigType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSEID;
	U8 configData[1];
}AvdtpSignalReconfigType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 firstACPSEID;
	U8 otherACPSEID[1];
}AvdtpSignalSEIDType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSeid;
	U8 errorCode;
}AvdtpSignalSuspendRejectType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSeid;
	U8 errorCode;
}AvdtpSignalStartRejectType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 serviceCategory;
	U8 errorCode;
}AvdtpSignalReconfigureRejectType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 para[1];
}AvdtpSignalGeneralRejectType;

typedef struct
{
	union {
		U8 b;
		struct {
			U8 messageType:2;		//Message Type
			U8 packetType:2;		//Packet Type
			U8 transactionLabel:4;	//Transaction Label
		} f;
	} signalHeader;
	U8 signalIdentifier;
	U8 acpSEID;
	U8 para[1];		//place holder
}AvdtpSignalSecurityType;

typedef struct
{
	U8 para[1];		//place holder
}AvdtpMediaType;

/*AVDTP*/
void BtAvdtpRegisterL2cap(Handler app);
void BtAvdtpUnregisterL2cap(Handler app);
void BtAvdtpSignalConnectRequest(BD_ADDR_T *pBdAddr, U16 mtu);
void BtAvdtpSignalDisconnectRequest(BD_ADDR_T *pBdAddr);
void BtAvdtpSignalConnectResponse(BD_ADDR_T *pBdAddr, BOOL response, U16 channelID, U8 identifier, U16 mtu);

void BtAvdtpMediaConnectRequest(Handler app, BD_ADDR_T *pBdAddr, U16 mtu);
void BtAvdtpMediaDisconnectRequest(BD_ADDR_T *pBdAddr);
void BtAvdtpMediaConnectResponse(Handler app, BD_ADDR_T *pBdAddr, BOOL response, U16 channelID, U8 identifier, U16 mtu);

void BtAvdtpSendRespDiscover(BD_ADDR_T *pBdAddr, U8 seidNum, U8 * seidPtr);
void BtAvdtpSendRespGetCapabilities(BD_ADDR_T *pBdAddr, U8 capabilityPacketSize, U8 *pServiceCapabilityData);
void BtAvdtpSendRespGetAllCapabilities(BD_ADDR_T *pBdAddr, U8 capabilityPacketSize, U8 *pServiceCapabilityData);
void BtAvdtpSendRespSetConfiguration(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespGetConfiguration(BD_ADDR_T *pBdAddr, U8 *pServiceCapability, U8 capabilitySize);
void BtAvdtpSendRespReconfigure(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespDelayReport(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespOpen(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespStart(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespClose(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespSuspend(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespAbort(BD_ADDR_T *pBdAddr);
void BtAvdtpSendRespSecurityControl(BD_ADDR_T *pBdAddr, U8 dataSize, U8 *pData);
void BtAvdtpSendRespSimpleReject(BD_ADDR_T *pBdAddr, U8 signalID, U8 errorCode, U8 identifier);



void BtAvdtpSendCmdDelayReport(BD_ADDR_T *pBdAddr, U8 remoteSEID, U16 defaultDelayReport);
void BtAvdtpSendCmdGetConfiguration(BD_ADDR_T *pBdAddr, U8 remoteSEID);
void BtAvdtpSendCmdReconfigure(BD_ADDR_T *pBdAddr, U8 remoteSEID, U8 configDataSize, U8 *pConfigDataBegin);
void BtAvdtpSendCmdDiscover(BD_ADDR_T *pBdAddr);
void BtAvdtpSendCmdGetCapabilities(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdGetAllCapabilities(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdSetConfiguration(BD_ADDR_T *pBdAddr, U8 acpSeid, U8 intSeid, U8 configLen, U8 *pConfigParam);
void BtAvdtpSendCmdOpen(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdStart(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdClose(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdSuspend(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdAbort(BD_ADDR_T *pBdAddr, U8 acpSeid);
void BtAvdtpSendCmdSecurityControl(BD_ADDR_T *pBdAddr, U8 acpSeid, U8 cpDataLen, U8 *cpDataPtr);
BOOL BtAvdtpIsAACAllow(BD_ADDR_T *pBdAddr);

U8 BtAvdtpGetMediaChannelId(BD_ADDR_T *pBdAddr);
U8 BtAvdtpGetIsMediaPlay(BD_ADDR_T *pBdAddr);
U16 BTAvdtpGetMTU(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL BtAvdtpLock();
BOOL BtAvdtpUnlock();
BOOL BtAvdtpIsAllow();
U16 BtAvdtpGetRhoDataLength();
BOOL BtAvdtpGetRhoData(U8 *pBuffer);
BOOL BtAvdtpApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAvdtpData, U16 length);
BOOL BtAvdtpApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAvdtpData, U16 length);

#endif //_BT_AVDTP_H_
