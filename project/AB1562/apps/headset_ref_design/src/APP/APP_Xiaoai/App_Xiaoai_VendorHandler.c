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
#include "App_Xiaoai_VendorHandler.h"
#include "App_Xiaoai.h"


#ifdef XIAOAI_DEVELOPMENT

static U8 app_xiaoai_zimi_SetKeyFunction(U8 keyType, U8 keyOperation, U8 keyDefine)
{
	DBG_LOG_APP_XIAOAI("[APP XIAOAI]  zimi_SetKeyFunction : keyType 0x%x, keyOperation 0x%x, keyDefine 0x%x", 3, keyType, keyOperation, keyDefine);
	return XIAOAI_RCSP_STATUS_SUCCESS;
}

static U8 app_xiaoai_zimi_SetVolumeAutoCtl(U8 ctl)
{
	DBG_LOG_APP_XIAOAI("[APP XIAOAI] zimi_SetVolumeAutoCtl : 0x%x" , 1, ctl);
	return XIAOAI_RCSP_STATUS_SUCCESS;
}

static U8 app_xiaoai_zimi_SetEQMode(U8 mode)
{
	DBG_LOG_APP_XIAOAI("[APP XIAOAI] zimi_SetEQMode : 0x%x" , 1, mode);
	return XIAOAI_RCSP_STATUS_SUCCESS;
}

static U8 app_xiaoai_zimi_SetGameMode(U8 status)
{
	DBG_LOG_APP_XIAOAI("[APP XIAOAI] zimi_SetGameMode : 0x%x" , 1, status);
	return XIAOAI_RCSP_STATUS_SUCCESS;
}

void App_Xiaoai_VendorHandler(BD_ADDR_T *pBdAddr, U8 opCodeSN, U8 *ptr, U16 dataLen)
{
	U8 status = XIAOAI_RCSP_STATUS_FAIL;


	DBG_LOG_APP_XIAOAI("[APP_XIAOAI] App_Xiaoai_VendorHandler, opcode_sn :0x%x, datalen:%d", 2, opCodeSN, dataLen);
	
	//ZIMI vendor specific command
	if(dataLen >= 5)
	{
		U32 companyID = xiaoai_get_pkt_u32(ptr);
		U8 customOpCode = ptr[4];

		//check_companyID();
		DBG_LOG_APP_XIAOAI("[APP_XIAOAI] App_Xiaoai_VendorHandler,customOpCode 0x%x", 1, customOpCode);

		switch(customOpCode)
		{
			case ZIMI_TWS_SET_KEY_FUNCTION:
				if(dataLen != 8)
					break;
				status = app_xiaoai_zimi_SetKeyFunction(ptr[5], ptr[6], ptr[7]);
				break;
			case ZIMI_TWS_SET_VOLUME_AUTO_CONTROL:
				if(dataLen != 6)
					break;				
				status = app_xiaoai_zimi_SetVolumeAutoCtl(ptr[5]);
				break;
			case ZIMI_TWS_SET_EQ_MODE:
				if(dataLen != 6)
					break;				
				status = app_xiaoai_zimi_SetEQMode(ptr[5]);
				break;
			case ZIMI_TWS_SET_GAME_MODE:
				if(dataLen != 6)
					break;				
				status = app_xiaoai_zimi_SetGameMode(ptr[5]);
				break;
				
			default:
				DBG_LOG_APP_XIAOAI("[APP_XIAOAI] unknown customOpCode :0x%x", 1, customOpCode);
				break;
		}
		
		App_Xiaoai_SendZIMIRsp((xiaoai_bdaddr_t *)pBdAddr, status, opCodeSN, companyID, customOpCode);
		
	}
	else
	{
		DBG_LOG_APP_XIAOAI("[APP XIAOAI] App_Xiaoai_VendorHandler ,dataLen:%d" , 1, dataLen);
		xiaoai_vendor_send_rsp((xiaoai_bdaddr_t *)pBdAddr, XIAOAI_RCSP_STATUS_FAIL, NULL, 0, opCodeSN);
	}
}

xiaoai_status_e App_Xiaoai_SendZIMIRsp(xiaoai_bdaddr_t * pBdAddr, U8 status, U8 opCodeSN, U32 companyID, U8 customOpCode)
{
	U8 data[5];

	U32 pktCompanyID = xiaoai_transform_endian_u32(companyID);

	FW_Memcpy(data, &pktCompanyID, sizeof(U32));
	FW_Memcpy(data + 4, &customOpCode, sizeof(U8));
	
	return xiaoai_vendor_send_rsp(pBdAddr, status, data, 5, opCodeSN);
}

#endif

