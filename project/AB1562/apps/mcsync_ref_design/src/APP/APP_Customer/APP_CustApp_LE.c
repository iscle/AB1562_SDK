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
#include "bt.h"

#if AIROHA_BT_LE_ENABLE

#include "APP_CustApp_LE.h"
#include "App_CustomerTimer.h"

log_create_module(CUSTAPP, PRINT_LEVEL_DEBUG);

/**************************************************************************************************
* Define
**************************************************************************************************/
#define CUSTOMER_TX_CHARACTERISTIC_HANDLE   2
#define CUSTOMER_TX_DESCRIPTOR_HANDLE       3

#define CUSTAPP_LE_MSGID_I(fmt,cnt,arg...)  LOG_MSGID_I(CUSTAPP,fmt,cnt,##arg)

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 APP_CustApp_LE_Handler(Handler handler, U16 id, void* msg, U32 handler_id);

/**************************************************************************************************
* Variable
**************************************************************************************************/
const BT_GATT_ADD_SERVICE_T customer_service_param =
{
    .serviceType = PRIMARY_SERVICE,
    .uuidType = UUID_TYPE_16,
    .uuid = {0x45, 0x4D},
    .registerLinkStatusEvt = 1,
};

const BT_GATT_ADD_CHARACTERISTIC_T customer_tx_charc_param =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x45, 0x4C},
    .property = (C_NOTIFY|C_READ|C_WRITE),
    .permission = BT_ATT_RP_WP,
    .data_initial_length = 6,
    .data_maximum_length = 0,
    .char_data = NULL,
};

const BT_GATT_ADD_DESCRIPTOR_T customer_tx_descrip_param =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x02,0x29},
    .permission = BT_ATT_RP_WP,
    .data_initial_length = 2,
    .data_maximum_length = 0,
    .desc_data = NULL,
};

static U16 g_customer_service_handle;
static U16 g_customer_tx_cccd = 0;
static U8 g_customer_tx[6] = {0};

HandlerData APP_CustApp_LE_Callback = {APP_CustApp_LE_Handler};

/**************************************************************************************************
* Static Function (Handler)
**************************************************************************************************/
static void APP_CustApp_LE_HandleLinkStatusInd(BT_ATT_LINK_STATUS_IND_T *ind)
{
	if(ind->status == ATT_LINK_CONNECTED)
    {
        CUSTAPP_LE_MSGID_I("[APP_CustLE] Connected", 0);
    }
    else
    {
        CUSTAPP_LE_MSGID_I("[APP_CustLE] Disconnected", 0);
    }

}

static BOOL APP_CustApp_LE_HandleAccessInd(BT_ATT_ACCESS_IND_T *ind)
{
	U16 attHdl;
	U8 i;

	attHdl = (ind->handle-g_customer_service_handle);

    CUSTAPP_LE_MSGID_I("[APP_CustLE] HandleAccessInd %x %x", 2, (U8)attHdl, (U8)ind->handle);

	switch(attHdl)
	{
		case CUSTOMER_TX_CHARACTERISTIC_HANDLE:
		{
			if(ind->flags == ATT_ACCESS_READ)
			{
                /* Handle read request */
                CUSTAPP_LE_MSGID_I("[APP_CustLE] Read charc", 0);

                /* For example: the characteristic value represent peer bd_address */
                FW_Memcpy(g_customer_tx, ind->bdAddr, 6);
				BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, 6, g_customer_tx);
			}
			else
			{
                /* Handle write request */
                /* Note: check if valueLen valid */
				//FW_Memcpy((U8*)&g_customer_tx, ind->value, ind->valueLen);

				if(ind->flags == ATT_ACCESS_WRITE)
				{
                    CUSTAPP_LE_MSGID_I("[APP_CustLE] Write charc, Length:%x", 1, (U8)ind->valueLen);
					for(i=0; i<ind->valueLen; i++)
					{
                        CUSTAPP_LE_MSGID_I("[APP_CustLE] Write%x = %x", 2, (U8)i, (U8)ind->value[i]);
					}
					BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, 0, NULL);
				}
			}
		}
		break;

		case CUSTOMER_TX_DESCRIPTOR_HANDLE:
		{
			if(ind->flags == ATT_ACCESS_READ)
			{
                BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, ind->valueLen, (U8*)&g_customer_tx_cccd);
			}
            else
            {
                /* For example: when CCCD enable, device will notify peer bd_address each second. */
				FW_Memcpy(g_customer_tx, ind->bdAddr, 6);
                CUSTAPP_LE_MSGID_I("[APP_CustLE] Write CCCD (%x %x %x %x %x %x)", 6, (U8)ind->bdAddr[0], (U8)ind->bdAddr[1], (U8)ind->bdAddr[2], (U8)ind->bdAddr[3], (U8)ind->bdAddr[4], (U8)ind->bdAddr[5]);

                if(ind->flags == ATT_ACCESS_WRITE)
                    BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, 0, NULL);

                CUSTAPP_LE_MSGID_I("[APP_CustLE] CCCD %x %x", 2, (U8)ind->value[0], ind->value[1]);

				if(ind->value[0] == 1)
				{
                    CUSTAPP_LE_MSGID_I("[APP_CustLE] Enable notify", 0);
                    /* Enable Timer */
					APP_CustomerSetTimer(CUSTOMER_TIMER_ID_3, NULL, 0, 1000);
				}
				else
				{
                    CUSTAPP_LE_MSGID_I("[APP_CustLE] Disable notify", 0);
                    /* Disable Timer */
					APP_CustomerReleaseTimer(CUSTOMER_TIMER_ID_3, 0);
				}
            }
		}
		break;

	}

	return 0;
}

static U32 APP_CustApp_LE_Handler(Handler handler, U16 id, void* msg, U32 handler_id)
{
    UNUSED(handler);
    UNUSED(handler_id);

    switch(id)
    {
        case BT_ATT_LINK_STATUS_IND:
            APP_CustApp_LE_HandleLinkStatusInd((BT_ATT_LINK_STATUS_IND_T*)msg);
            break;

        case BT_ATT_ACCESS_IND:
            return APP_CustApp_LE_HandleAccessInd((BT_ATT_ACCESS_IND_T*)msg);

        default:
        break;

    }

    return 0;
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_CustApp_LE_SendNotification(void)
{
    CUSTAPP_LE_MSGID_I("[APP_CustLE] Notification", 0);

    /* For example: the value of the notification is peer bd_address */
    BtAttNotificationRequest(g_customer_tx, (g_customer_service_handle+CUSTOMER_TX_CHARACTERISTIC_HANDLE), 6, g_customer_tx);
    APP_CustomerSetTimer(CUSTOMER_TIMER_ID_3, NULL, 0, 1000);
}

void APP_CustApp_LE_Init(void)
{
    g_customer_service_handle = BtGattAddService(&APP_CustApp_LE_Callback, &customer_service_param);
    //printf("AppCustAppLE: service_handle:%x\n", service_handle);

    BtGattAddCharacteristic(&customer_tx_charc_param);
    BtGattAddDescriptor(&customer_tx_descrip_param);
}

#endif /* AIROHA_BT_LE_ENABLE */

