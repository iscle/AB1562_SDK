/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
#include "bt.h"
#include "bt_xiaoai.h"
#include "App_XiaoaiAdv.h"
#include "App_LinkKey.h"
#include "App_Xiaoai.h"
#include "BtMCSync.h"
#include "App_Ble.h"

#ifdef XIAOAI_DEVELOPMENT

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_XiaoaiAdv_Handler(Handler handler, U16 id, void *msg, U32 handler_id);

/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static BOOL gIsXiaoaiAdvEnable;
static HandlerData gAppXiaoaiAdvHandle = { app_XiaoaiAdv_Handler };


U8 gXiaoaiAdv[31] =
{
	0x02, 0x01, 0x1a, 0x1b, 0xff, 0x8f, 0x03, //byte 0 ~ byte 6, already fixed
	0x16, //byte 7, MIUI TYPE length 
	0x01, //byte 8, MIUI TYPE 01
	0x01, //byte 9, AllocationID / MajorID
	0x53,//LEFT_CONNECTABLE | LEFT_DISCOVERABLE | LEFT_IS_ADVERTISER| Subld_mac_device, //0x53, //byte 10
	SMART_CASE_OPENED | APP_TWS_CONNECTED, //0x42, //byte 11
	0xe3,//0x80~0xe4 L battery ,byte 12
	0xe3,//0x80~0xe4 R battery,byte 13
	0x5c,//case battery,byte 14
	0x00, 0x00, 0x00, //byte 15 ~ byte 17, LAP of paired phone
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //right earbud bda
	0x00, //byte 24, ADV counts
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //byte 25 ~ byte 30, left earbud bda
};

/*
static void app_XiaoaiDumpAdvData()
{
	U8 index;
	char str[63];
	
	memset(str, '\0', 63);
	
	for(index = 0; index < 31; index ++)
	{
		sprintf((char *)(str+(index*2)), "%02x", gXiaoaiAdv[index]);
    }
    printf("[APP Xiaoai] AdvData: %s", str);
}
*/

static void app_XiaoaiSetAdv_FixedByte(void)
{
	gXiaoaiAdv[0] = 0x02;
	gXiaoaiAdv[1] = 0x01;
	gXiaoaiAdv[2] = 0x1a;
	gXiaoaiAdv[3] = 0x1b;
	gXiaoaiAdv[4] = 0xff;
	gXiaoaiAdv[5] = 0x8f;
	gXiaoaiAdv[6] = 0x03;
	gXiaoaiAdv[7] = 0x16;	//MIUI TYPE length
	gXiaoaiAdv[8] = 0x01;	//MIUI TYPE 01
	gXiaoaiAdv[9] = xiaoai_nvkey_get_major_id();
}

static void app_XiaoaiSetAdv_LRStatus(void)
{
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->isThisDevL = 1;
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->minorID = xiaoai_nvkey_get_minor_id();
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->rsv = 0;
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->isLDiscoverable = 1;
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->isRDiscoverable = 0;
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->isLConnectable = 1;
	((XIAOAI_LR_STATUS_STRU *) &gXiaoaiAdv[LR_STATUS_BYTE])->isRConnectable = 0;
}

static void app_XiaoaiSetAdv_EarBudsStatus(void)
{
	U8 result, isTwsConnected, isTwsConnecting;
	
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->rsv = 0;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isCaseLidOpen = 1;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isEDRConnected = 0;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isEDRPaired = 0;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isMACEncrypted = 0;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isLROutOfCase = 0;

	result = xiaoai_get_tws_status();

	if(result == 0)
	{
		isTwsConnected = 1;
		isTwsConnecting = 0;
	}
	else
	{
		isTwsConnected = 0;
		isTwsConnecting = 1;	
	}
	
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isTWSConnected = isTwsConnected;
	((XIAOAI_EARBUDS_STATUS_STRU *) &gXiaoaiAdv[EARBUDS_STATUS_BTYE])->isTWSConnecting = isTwsConnecting;
}

static void app_xiaoai_SetAdv_LRCaseBattery()
{
	//printf("[XIAOAI] (%d)(0x%02x)!!!",xiaoai_bat_get_local_level_in_percent(),xiaoai_bat_get_local_level_in_percent());

	((XIAOAI_BATTERY_STATUS_STRU *)&gXiaoaiAdv[L_BATTERY_BYTE])->batInPercent = xiaoai_bat_get_local_level_in_percent();
	((XIAOAI_BATTERY_STATUS_STRU *)&gXiaoaiAdv[L_BATTERY_BYTE])->isCharging = 1;
	((XIAOAI_BATTERY_STATUS_STRU *)&gXiaoaiAdv[R_BATTERY_BYTE])->batInPercent = xiaoai_bat_get_local_level_in_percent();
	((XIAOAI_BATTERY_STATUS_STRU *)&gXiaoaiAdv[R_BATTERY_BYTE])->isCharging = 1;

	gXiaoaiAdv[CASE_BATTERY_BYTE] = 0x5c;
}

static void app_xiaoai_SetAdv_LastAGBda()
{
	BD_ADDR_T *pBda = APP_LinkKey_GetNormalLinkBdAddr(0);

	if(pBda)
	{
		gXiaoaiAdv[LAST_AG_LAP_LSB_BYTE] = pBda->addr[0];
		gXiaoaiAdv[LAST_AG_LAP_02_BYTE] = pBda->addr[1];
		gXiaoaiAdv[LAST_AG_LAP_MSB_BYTE] = pBda->addr[2];
	}
}

static void app_xiaoai_SetAdv_LRBdAddr()
{
	BD_ADDR_T *pBdAddr = (BD_ADDR_T *)xiaoai_nvkey_get_edr_bdaddr();//(BD_ADDR_T *)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_CON_BR_EDR_BDADDR);
	BD_ADDR_T BdAddr;

	if(pBdAddr)
	{	
		BdAddr.addr[0] = pBdAddr->addr[4];
		BdAddr.addr[1] = pBdAddr->addr[5];
		BdAddr.addr[2] = pBdAddr->addr[3];
		BdAddr.addr[3] = pBdAddr->addr[0];
		BdAddr.addr[4] = pBdAddr->addr[1];
		BdAddr.addr[5] = pBdAddr->addr[2];
		
		memcpy(&gXiaoaiAdv[18], &(BdAddr.addr[0]), 6);
		memcpy(&gXiaoaiAdv[25], &(BdAddr.addr[0]), 6);
	}
}

static void app_xiaoai_SetAdvCount()
{
	++gXiaoaiAdv[24];
}

static void app_Xiaoai_SetAdvData(void)
{	
	app_XiaoaiSetAdv_FixedByte();
	app_XiaoaiSetAdv_LRStatus();
	app_XiaoaiSetAdv_EarBudsStatus();
	app_xiaoai_SetAdv_LRCaseBattery();
	app_xiaoai_SetAdv_LastAGBda();
	app_xiaoai_SetAdv_LRBdAddr();
	//app_xiaoai_SetAdvCount();
	//app_XiaoaiDumpAdvData();	
	//xiaoai_set_adv_data_req(ADV_MAX_LENGHT, gXiaoaiAdv);
}

static U32 app_XiaoaiAdv_Handler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);
	
	switch(id)
	{
		case XIAOAI_ADV_UPDATE_TIMEOUT_IND:
			{
				U8 olaAdvData[31];
				FW_Memcpy(olaAdvData, gXiaoaiAdv, 31);
				app_Xiaoai_SetAdvData();
				
				if(FW_Memcmp(olaAdvData, gXiaoaiAdv, 31))//update adv /  not update adv counter 
 				{
					DBG_LOG_APP_XIAOAI("[APP_XIAOAI] xiaoai update adv !!" , 0);

					xiaoai_set_adv_enable_req(FALSE);
					xiaoai_set_adv_data_req(ADV_MAX_LENGHT, gXiaoaiAdv);
					xiaoai_set_adv_enable_req(TRUE);					
				}
				MSG_MessageSendLater(&gAppXiaoaiAdvHandle, XIAOAI_ADV_UPDATE_TIMEOUT_IND, NULL, XIAOI_ADV_UPDATE_TIME);
			}
			break;

		default:
			break;
	}
	return 0;
}

BOOL APP_XiaoaiEnableAdv(void)
{

	if(!BtAwsMce_IsDefaultRoleAgent())
		return FALSE;
	
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] APP_XiaoaiEnableAdv !!" , 0);
	MSG_MessageCancelAll(&gAppXiaoaiAdvHandle, XIAOAI_ADV_UPDATE_TIMEOUT_IND);

	xiaoai_set_adv_enable_req(FALSE);
	app_Xiaoai_SetAdvData();
	app_xiaoai_SetAdvCount();
	xiaoai_set_adv_data_req(ADV_MAX_LENGHT, gXiaoaiAdv);
	xiaoai_set_adv_enable_req(TRUE);

	MSG_MessageSendLater(&gAppXiaoaiAdvHandle, XIAOAI_ADV_UPDATE_TIMEOUT_IND, NULL, XIAOI_ADV_UPDATE_TIME);
	
	if(!gIsXiaoaiAdvEnable)
	{
		gIsXiaoaiAdvEnable = TRUE;
	}
	return TRUE;
}

BOOL APP_XiaoaiDisableAdv(void)
{
	if( (!BtAwsMce_IsDefaultRoleAgent()) || (!gIsXiaoaiAdvEnable) )
		return FALSE;

	
	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] APP_XiaoaiDisableAdv !!" , 0);

	MSG_MessageCancelAll(&gAppXiaoaiAdvHandle, XIAOAI_ADV_UPDATE_TIMEOUT_IND);
	xiaoai_set_adv_enable_req(FALSE);


	gIsXiaoaiAdvEnable = FALSE;
    return TRUE;
}
BOOL APP_Xiaoai_IsAdvEnable(void)
{
	return gIsXiaoaiAdvEnable;
}

void APP_XiaoaiResetAdv(void)
{
	APP_Ble_DisableAdv(BLE_ADV_XIAOAI_MASK);
	xiaoai_set_adv_interval(0x64, 0x64);
	APP_Ble_EnableAdv(BLE_ADV_XIAOAI_MASK);
}
#endif

