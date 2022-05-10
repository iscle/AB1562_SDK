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

#ifndef _PM_LINK_MANAGER_H_
#define _PM_LINK_MANAGER_H_

#include "PM_SniffHandler.h"
#include "PM_RhoHandler.h"


#if 0//LOG_PRINT_156X
#define DBG_LOG_PROFILE_PM(_message, arg_cnt, ...)	printf(_message, ##__VA_ARGS__)
#else
#define DBG_LOG_PROFILE_PM(_message, arg_cnt, ...)   LOG_MSGID_I(PM, _message, arg_cnt, ##__VA_ARGS__)
#endif

/**************************************************************************************************
* Define
**************************************************************************************************/
#define MAX_ACTIVE_PROFILE		10

#define RHO_PROFILE_HFP			0x01
#define RHO_PROFILE_A2DP		0x02
#define RHO_PROFILE_AVRCP		0x04
#define RHO_PROFILE_HSP			0x08
#define RHO_PROFILE_MCSYNC		0x10

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	PROFILE_DISCONNECTED,
	PROFILE_CONNECTING_SDP,
	PROFILE_CONNECTING_PROTOCOL,
	PROFILE_CONNECTED,
	PROFILE_DISCONNECTING,

	PROFILE_STATE_START = 0x10,
};

enum
{
	PM_ACL_NOT_EXIST,
	PM_ACL_EXISTED,
};

enum
{
	LINK_DISCONNECTED,
	LINK_CONNECTING,
	LINK_CONNECTED,
	LINK_DISCONNECTING,
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct stProfileInfo PROFILE_INFO;
typedef struct stProfileDesc PROFILE_DESC;

#ifdef WIN32
#pragma pack(push,1)
#endif
struct stProfileInfo
{
	PROFILE_DESC const * profileDesc;
	PROFILE_INFO * nextProfile;
	U8 state:7;
	U8 isAlive:1;
};

typedef BOOL (*PROFILE_CONNECT_REQUEST)(BD_ADDR_T *pBdAddr);
typedef BOOL (*PROFILE_DISCONNECT_REQUEST)(BD_ADDR_T *pBdAddr);


struct stProfileDesc
{
	U8 profileId;
	PROFILE_CONNECT_REQUEST		profileConnectRequest;
	PROFILE_DISCONNECT_REQUEST	ProfileDisconnectRequest;
};

typedef struct stProfileNode PROFILE_NODE;

struct stProfileNode
{
	PROFILE_DESC const * profileDesc;
	PROFILE_NODE * nextNode;
};

typedef struct
{
	U8 profileID[MAX_ACTIVE_PROFILE];
	U8 activeProfileCount;
}ACTIVE_PROFILE;

#ifdef WIN32
#pragma pack(pop)
#endif

typedef struct stSdpInfo SDP_INFO;

struct stSdpInfo
{
	Handler app;
	SDP_INFO * nextSdpInfo;
};

typedef struct
{
	U8 isUsed;
	BD_ADDR_T bdAddr;
	PROFILE_INFO * loadedProfileInfoList;

	PROFILE_INFO sdpProfileInfo;
	SDP_INFO * pSdpCtl;
	Handler serviceApp;
	U8 trafficBusy;
	//U32 sniffProfileMask;
	//U8 sniffCount;
	//U16 mode;
	ACTIVE_PROFILE * pActiveConnectProfileList;
	U8 aclStatus;
	PM_SNIFF_INFO_STRU sniffInfo;
	U8 linkState;
}PM_INFO_STRU;

/**************************************************************************************************
* Public functions
**************************************************************************************************/
PM_INFO_STRU * PM_SearchCtlInfoByBdAddr(BD_ADDR_T * pBdAddr);
PM_SNIFF_INFO_STRU * PM_SearchSniffInfoByBdAddr(BD_ADDR_T * pBdAddr);
PM_INFO_STRU * PM_SetCtlInfoByBdAddr(BD_ADDR_T * pBdAddr);
void 	PM_ClearCtlInfoByBdAddr(BD_ADDR_T * pBdAddr);
void 	PM_ClearSniffInfoByBdAddr(BD_ADDR_T * pBdAddr);
U8 		PM_GetSdpClientState(PM_INFO_STRU * pLinkInfo);
void	PM_SetSdpClientState(PM_INFO_STRU * pLinkInfo, U8 state);
void 	PM_SetNewSdpClientInfo(PM_INFO_STRU * pLinkInfo, SDP_INFO * pSdpInfo, Handler app);
void 	PM_ClearAllSdpClientInfo(PM_INFO_STRU * pLinkInfo);
void 	PM_ClearSdpClientInfoByApp(PM_INFO_STRU * pLinkInfo, Handler app);
Handler PM_GetServiceApp(PM_INFO_STRU * pLinkInfo);
void 	PM_SetServiceApp(PM_INFO_STRU * pLinkInfo, Handler serviceApp);
void PM_SetLinkTrafficBusy(BD_ADDR_T *pBdAddr, bool isBusy); //MMI_IsLinkTrafficBusy
BOOL 	PM_IsLinkTrafficBusy(BD_ADDR_T *pBdAddr);
void PM_AddActiveConnectProfile(BD_ADDR_T *pBdAddr, U8 profileID);
BOOL PM_IsProfileActiveConnect(BD_ADDR_T *pBdAddr, U8 profileID);
void PM_RemoveActiveConnectProfile(BD_ADDR_T *pBdAddr, U8 profileID);
ACTIVE_PROFILE *PM_GetActiveProfileList(BD_ADDR_T *pBdAddr);
void PM_SetAclStatus(BD_ADDR_T* pBdAddr, U8 aclStatus);
U8 PM_GetAclStatus(BD_ADDR_T* pBdAddr);
U8 PM_GetLinkState(PM_INFO_STRU *pLinkInfo);
U8 PM_GetAgLinkState(void);
void PM_SetLinkState(PM_INFO_STRU *pLinkInfo, U8 state);
U8 PM_GetCtlInfoNum(void);

U8 PM_GetLinkStateByBdAddr(BD_ADDR_T *pBdAddr);
BOOL PM_IsProfileConnectedInAnyLink(U8 profileId);
U8      PM_GetNormalLinkNum(void);
/**************************************************************************************************
* Public functions (RHO)
**************************************************************************************************/
PM_INFO_STRU *PM_GetAgLinkInfo(void);
BOOL PM_FillRhoData(PM_RHO_DATA_STRU *pRhoStru);
BOOL PM_AssignRhoData(PM_RHO_DATA_STRU *pRhoStru);

#endif//_PM_LINK_MANAGER_H_
