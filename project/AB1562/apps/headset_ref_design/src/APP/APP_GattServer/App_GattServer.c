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
<<<<<<< HEAD
=======
#include "PM_Nvkey.h"
>>>>>>> db20e11 (second commit)

#define APP_GATT_LINK_STATE_NONE        0
#define APP_GATT_LINK_STATE_CONNECTED   1

#define APP_GATT_CCCD_LEN               2
#define APP_GATT_INVALID_LINK_IDX       0xFF
#define APP_GATT_DEVICE_NAME_LEN        30
#define APP_GATT_APPEARANCE_LEN         2

#define APP_GATT_DATA_TYPE_DEVICE_NAME  0x09
#define APP_GATT_DATA_TYPE_APPEARANCE   0x19

/**************************************************************************************************
* Type define
**************************************************************************************************/
typedef struct
{
    U8 len;         // value is exclude len field
    U8 dataType;
    U8 data[1];
} PACKED APP_GATT_SERVER_ADV_T;

typedef struct
{
    BD_ADDR_T	bdAddr;
    U16         cccd;
    U8          linkState;
} APP_GATT_SERVER_LINK_INFO_T;

typedef struct
{
    U8 deviceName[APP_GATT_DEVICE_NAME_LEN];
    U8 appearance[APP_GATT_APPEARANCE_LEN];
    U16 serviceHdl;
    U16 serviceChangedHdl;
    U16 serviceChangedDescHdl;
    APP_GATT_SERVER_LINK_INFO_T linkInfo[FW_MAX_LINK_IDX];
} APP_GATT_SERVER_CTL_T; 
/**************************************************************************************************
* Prototype
**************************************************************************************************/
static U32 app_GattServerHandler(Handler handler, U16 id, void *pMsg, U32 handlerId);

/**************************************************************************************************
* Static Variables
**************************************************************************************************/
static const HandlerData gAppGattServerHandle = { app_GattServerHandler };

static APP_GATT_SERVER_CTL_T gAppGattServer =
{
    .deviceName = "Airoha",
    .appearance = {0xC0, 0x03},
};

/**************************************************************************************************
* Const Variable
**************************************************************************************************/
static const BT_GATT_ADD_SERVICE_T gAppGattServicePara =
{
    .serviceType = PRIMARY_SERVICE,
    .uuidType = UUID_TYPE_16,
    .uuid = {0x01,0x18},
    .registerLinkStatusEvt = 1,
};

static const BT_GATT_ADD_CHARACTERISTIC_T gAppGattServiceChanged =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x05,0x2A},
    .property = (C_INDICATE),
    .permission = BT_ATT_RB_WB,
    .data_initial_length = 4,
    .data_maximum_length = 0,
    .char_data = NULL,
};

static const BT_GATT_ADD_DESCRIPTOR_T gAppGattServiceChangedDesc =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x02,0x29},
    .permission = BT_ATT_RP_WP,
    .data_initial_length = 2,
    .data_maximum_length = 0,
    .desc_data = NULL,
};

static const BT_GATT_ADD_SERVICE_T gAppGapServicePara =
{
    .serviceType = PRIMARY_SERVICE,
    .uuidType = UUID_TYPE_16,
    .uuid = {0x00,0x18},
};

static const BT_GATT_ADD_CHARACTERISTIC_T gAppGapDeviceName =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x00,0x2A},
    .property = (C_READ),
    .permission = BT_ATT_RP_WB,
    .data_initial_length = APP_GATT_DEVICE_NAME_LEN,
    .data_maximum_length = APP_GATT_DEVICE_NAME_LEN,
    .char_data = gAppGattServer.deviceName,
};

static const BT_GATT_ADD_CHARACTERISTIC_T gAppGapAppearance =
{
    .uuidType = UUID_TYPE_16,
    .uuid = {0x01,0x2A},
    .property = (C_READ),
    .permission = BT_ATT_RP_WB,
    .data_initial_length = 2,
    .data_maximum_length = 0,
    .char_data = gAppGattServer.appearance,
};

static const U8 gAppAttrHandleRange[4] = {0x00, 0x00, 0xff, 0xff};

/**************************************************************************************************
* Static function
**************************************************************************************************/
static U8 app_GattServer_GetLinkIdx(BD_ADDR_T *pBdAddr)
{
    U8 i;

	for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
	    if (!FW_Memcmp(pBdAddr, &gAppGattServer.linkInfo[i].bdAddr, sizeof(BD_ADDR_T)))
	    {
            return i;
	    }
	}

    for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
		if (FW_IsBdAddrZero(&gAppGattServer.linkInfo[i].bdAddr))
		{
            return i;
		}
	}
    return APP_GATT_INVALID_LINK_IDX;
}

static void app_GattServer_ClearLinkInfo(BD_ADDR_T *pBdAddr)
{
	U8 i;

	for (i = 0; i < FW_MAX_LINK_IDX; i++)
	{
	    if (!FW_Memcmp(pBdAddr, &gAppGattServer.linkInfo[i].bdAddr, sizeof(BD_ADDR_T)))
	    {
			FW_Memset(&gAppGattServer.linkInfo[i], 0, sizeof(APP_GATT_SERVER_LINK_INFO_T));
	    }
	}
}

static void app_GattServer_UpdateLinkStatus(BD_ADDR_T *pBdAddr, BT_ATT_LINK_STATUS_E status)
{
    U8 i;
    
    if (ATT_LINK_CONNECTED == status)
    {
        i = app_GattServer_GetLinkIdx(pBdAddr);
        if (i < FW_MAX_LINK_IDX)
        {
            gAppGattServer.linkInfo[i].linkState = APP_GATT_LINK_STATE_CONNECTED;
        }
    }
    else if (ATT_LINK_CLOSED == status)
    {
        app_GattServer_ClearLinkInfo(pBdAddr);
    }
}

static U32 app_GattServerHandler(Handler handler, U16 id, void *pMsg, U32 handlerId)
{
    UNUSED(handler);
    UNUSED(handlerId);

    switch(id)
    {
        case BT_ATT_LINK_STATUS_IND:
        {
            BT_ATT_LINK_STATUS_IND_T *ind = (BT_ATT_LINK_STATUS_IND_T *)pMsg;

            if (ATT_LINK_CONNECTED == ind->status)
            {
                BtAttIndicationRequest(ind->bdAddr, gAppGattServer.serviceChangedHdl, 4, (U8 *)gAppAttrHandleRange);
            }
            
            app_GattServer_UpdateLinkStatus((BD_ADDR_T *)ind->bdAddr, ind->status);
            break;
        }

        case BT_ATT_ACCESS_IND:
        {
            BT_ATT_ACCESS_IND_T *ind = (BT_ATT_ACCESS_IND_T *)pMsg;
            
            if (gAppGattServer.serviceChangedDescHdl == ind->handle)
            {
                U8 i;

                i = app_GattServer_GetLinkIdx((BD_ADDR_T *)ind->bdAddr);
                if (i >= FW_MAX_LINK_IDX)
                {
                    BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, 0, NULL);
                    return 0;
                }
                
                if (ATT_ACCESS_READ == ind->flags)
                {
                    BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, ind->valueLen, (U8*)&gAppGattServer.linkInfo[i].cccd);
                    
                }
                else if (ATT_ACCESS_WRITE == ind->flags)
                {
                    BtAttAccessResponse(ind->bdAddr, BT_ATT_NOERR, ind->handle, 0, NULL);
                    if (APP_GATT_CCCD_LEN == ind->valueLen)
                    {
                        FW_Memcpy((U8*)&gAppGattServer.linkInfo[i].cccd, ind->value, ind->valueLen);

                        if (ind->value[0] == 2)
                        {
                            //BtAttIndicationRequest(ind->bdAddr, gAppGattServer.serviceChangedHdl, 4, (U8 *)gAppAttrHandleRange);
                        }
                    }
                }
            }
            break;
        }
            
        default:
            break;
    }

    return 0;
}

static void app_GattServer_AttrInit(void)
{
    APP_GATT_SERVER_ADV_T *pAdv;
    U8 *ptr;
    U8 advSize;
    U8 nameLen;
    
<<<<<<< HEAD
    advSize = NVKEY_GetPayloadLength(NVKEYID_BT_BLE_BLE_ADV_DATA);
    ptr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_BT_BLE_BLE_ADV_DATA);
    pAdv = (APP_GATT_SERVER_ADV_T *)ptr;

=======
    advSize = PM_Nvkey_GetBleAdvDataLength();
    ptr = PM_Nvkey_GetBleAdvData();
    pAdv = (APP_GATT_SERVER_ADV_T *)ptr;
    
>>>>>>> db20e11 (second commit)
    while (pAdv && (advSize > pAdv->len))
    {
        if (APP_GATT_DATA_TYPE_DEVICE_NAME == pAdv->dataType)
        {
            if (APP_GATT_DEVICE_NAME_LEN < (pAdv->len - 1))
            {
                nameLen = APP_GATT_DEVICE_NAME_LEN;
            }
            else
            {
                nameLen = pAdv->len - 1;
            }
            FW_Memset(gAppGattServer.deviceName, 0, APP_GATT_DEVICE_NAME_LEN);
            FW_Memcpy(gAppGattServer.deviceName, pAdv->data, nameLen);
        }
        else if (APP_GATT_DATA_TYPE_APPEARANCE == pAdv->dataType)
        {
            FW_Memcpy(gAppGattServer.appearance, pAdv->data, APP_GATT_APPEARANCE_LEN);
        }
        advSize -= (pAdv->len + 1);
        ptr += (pAdv->len + 1);
        pAdv = (APP_GATT_SERVER_ADV_T *)ptr;
    }
}

/**************************************************************************************************
* Public API
**************************************************************************************************/
void APP_GattServer_Init(void)
{
    // GATT Service
    gAppGattServer.serviceHdl= BtGattAddService((Handler)&gAppGattServerHandle, &gAppGattServicePara);

    // !!!ALWAYS THE FIRST CHARACTERISTIC IN SERVER!!!
    gAppGattServer.serviceChangedHdl = BtGattAddCharacteristic(&gAppGattServiceChanged);
    gAppGattServer.serviceChangedDescHdl = BtGattAddDescriptor(&gAppGattServiceChangedDesc);

    app_GattServer_AttrInit();
    
    // GAP Service
    BtGattAddService((Handler)&gAppGattServerHandle, &gAppGapServicePara);
    BtGattAddCharacteristic(&gAppGapDeviceName);
    BtGattAddCharacteristic(&gAppGapAppearance);
}

