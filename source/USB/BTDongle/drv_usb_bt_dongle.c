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
#include "feature_config.h"
#if (FEATURE_USB_OVERALL)

#include "types.h"
#include "drv_usb.h"
#include "drv_usb_bt_dongle.h"
#include "stdlib.h"


#define USB_BT_VENDOR_ID                   0x7777
#define USB_BT_PRODUCT_ID                  0x6661
#define USB_BT_RELEASE_ID                  0x0100


static U8 BT_Manufacturer[] =
{
    14,
    0x03,
    'A', 0x00, 'i', 0x00, 'r', 0x00, 'o', 0x00, 'h', 0x00, 'a', 0x00
};

static U8 BT_Production[] =
{
    20,
    0x03,
    'B', 0x00, 'T', 0x00, ' ', 0x00, 'D', 0x00, 'o', 0x00, 'n', 0x00,  'g', 0x00, 'l', 0x00, 'e', 0x00
};

static U8 BT_SerialNumber[] =
{
    10,
    0x03,
    '1', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, ' ', 0x00, ' ', 0x00
};

static U8 u8USB_BT_HSDeviceDescriptor[] =
{
    0x12,                               /* 0 bLength */
    DT_DEVICE,                          /* 1 bdescriptorType */
    0x00,                               /* 2 bcdUSB version */
    0x02,                               /* 3 bcdUSB version */
    0xE0,                               /* 4 bDeviceClass */
    0x01,                               /* 5 bDeviceSubClass */
    0x01,                               /* 6 bDeviceProtocol */
    0x40,                               /* 7 bMaxPacketSize for EP0 - max = 64 */
    (USB_BT_VENDOR_ID & 0xFF),       /* 8 idVendor */
    (USB_BT_VENDOR_ID >> 8),         /* 9 idVendor */
    (USB_BT_PRODUCT_ID & 0xFF),      /* 10 idProduct */
    (USB_BT_PRODUCT_ID >> 8),        /* 11 idProduct */
    (USB_BT_RELEASE_ID & 0xFF),      /* 12 bcdDevice */
    (USB_BT_RELEASE_ID >> 8),        /* 13 bcdDevice */
    0x01,                               /* 14 iManufacturer - index of string*/
    0x02,                               /* 15 iProduct - index of string*/
    0x03,                               /* 16 iSerialNumber - index of string*/
    0x01                                /* 17 bNumConfigurations */
};

static U8 u8USB_BT_HSConfigOTGDescriptor[] =
{
    0x09,                               /* 0 bLength */
    DT_CONFIGURATION,                   /* 1 bDescriptortype - Configuration*/
    0xC1, 0x00,
    0x02,
    0x01,                               /* 5 bConfigurationValue */
    0x00,                               /* 6 iConfiguration */
    0xC0,                               /* 7 bmAttributes */
    0x00,                               /* 8 bMaxPower */

    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x00,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x03,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x81,                               /* 2 bEndpointAddress */
    0x03,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x02,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x02,                               /* 2 bEndpointAddress */
    0x02,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x02,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x82,                               /* 2 bEndpointAddress */
    0x02,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x02,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 0
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 1
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x01,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x09,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x09,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 2
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x02,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x11,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x11,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 3
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x03,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x19,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x19,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 4
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x04,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x21,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x21,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    // ISO Interface setting 5
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x05,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0xE0,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x01,                               /* 7 bInterfaceProtocol */
    0x01,                               /* 8 iInterface */

    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x03,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x31,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */

    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x01,                               /* 3 bmAttributes */
    0x31,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */
};

U32 g_u32USBBTInitFinish = 0;

typedef struct
{
    U8  *pu8BTBuff;
    U32 Status;
    U32 DataLen;
    U32 CurLen;
}USB_BT_T, *LPUSB_BT_BUFF_T;

USB_BT_T gUSBBTCMDBuff[USB_BT_CMD_BUFFER_NUM];
USB_BT_T gUSBBTEVTBuff[USB_BT_EVT_BUFFER_NUM];
USB_BT_T gUSBBTACLTC[USB_BT_ACL_BUFFER_NUM];
USB_BT_T gUSBBTACLFC[USB_BT_ACL_BUFFER_NUM];

volatile U32 gCmdWriteflg = 0;
volatile U32 gCmdReadflg = 0;
volatile U32 gEvtWriteflg = 0;
volatile U32 gEvtReadflg = 0;

volatile U32 gAclWriteflg = 0;
volatile U32 gAclReadflg = 0;

U32 DRV_USB_BT_GetCmd(U8 *pBuffer)
{
    if(g_u32USBBTInitFinish < 1)
        return 0;

    if(pBuffer == NULL)
    {
        if(gUSBBTCMDBuff[gCmdReadflg].Status == 2)
            return gUSBBTCMDBuff[gCmdReadflg].DataLen;
        else
            return 0;
    }

    if(gUSBBTCMDBuff[gCmdReadflg].Status == 2)
    {
        gUSBBTCMDBuff[gCmdReadflg].Status = 3;

        U32 DataLen =  gUSBBTCMDBuff[gCmdReadflg].DataLen;
        memcpy(pBuffer, gUSBBTCMDBuff[gCmdReadflg].pu8BTBuff, gUSBBTCMDBuff[gCmdReadflg].DataLen);

        gUSBBTCMDBuff[gCmdReadflg].Status = 0;
        if( gCmdReadflg == (USB_BT_CMD_BUFFER_NUM - 1))
        {
            gCmdReadflg = 0;
        }
        else
        {
            gCmdReadflg += 1;
        }

        return DataLen;
    }

    return 0;
}

U32 DRV_USB_BT_GetAcl(U8 *pBuffer)
{
    if(g_u32USBBTInitFinish < 1)
         return 0;

    if(pBuffer == NULL)
    {
        if(gUSBBTACLFC[gAclReadflg].Status == 2)
            return gUSBBTACLFC[gAclReadflg].DataLen;
        else
            return 0;
    }

    if(gUSBBTACLFC[gAclReadflg].Status == 2)
    {
    	gUSBBTACLFC[gAclReadflg].Status = 3;

        U32 DataLen =  gUSBBTACLFC[gAclReadflg].DataLen;
        memcpy(pBuffer, gUSBBTACLFC[gAclReadflg].pu8BTBuff, gUSBBTACLFC[gAclReadflg].DataLen);

        gUSBBTACLFC[gAclReadflg].Status = 0;
        if( gAclReadflg == (USB_BT_ACL_BUFFER_NUM - 1))
        {
            gAclReadflg = 0;
        }
        else
        {
            gAclReadflg += 1;
        }

        return DataLen;
    }

    return 0;
}

VOID DRV_USB_BT_SendDeviceDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_BT_HSDeviceDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_BT_HSDeviceDescriptor, dataLen);
}

VOID DRV_USB_BT_SendConfigureDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_BT_HSConfigOTGDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_BT_HSConfigOTGDescriptor, dataLen);
}

BOOL DRV_USB_BT_OTGdescriptor(void)
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();
    U32 RequestLen = 0;
    U16 mySize = 0;
    U8  *pData;
    switch ((U8)(pSetupPacket->Value >> 8))
    {
        case 1:
            {
                DRV_USB_BT_SendDeviceDescription(pSetupPacket->Length);
            }
            break;
        case 2:
            switch ((U8)pSetupPacket->Value)
            {
                case 0x00:
                        mySize = sizeof(u8USB_BT_HSConfigOTGDescriptor);
                        u8USB_BT_HSConfigOTGDescriptor[2] = mySize;
                        u8USB_BT_HSConfigOTGDescriptor[3] = (mySize >> 8);
                        DRV_USB_BT_SendConfigureDescription(pSetupPacket->Length);
                        break;
                default:
                        return FALSE;
            }
            break;
        case 3:
            switch ((U8)pSetupPacket->Value)
            {
                case 0x00:
                	   DRV_USB_SetLanguage(pSetupPacket->Length);
                    return TRUE;
                case 0x01:
                       pData = BT_Manufacturer;
                       RequestLen =  BT_Manufacturer[0];
                    break;
                case 0x02:
                       pData = BT_Production;
                       RequestLen =  BT_Production[0];
                    break;
                case 0x03:
                       pData = BT_SerialNumber;
                       RequestLen = BT_SerialNumber[0];
                    break;
                default:
                    return FALSE;
            }
            if (RequestLen > pSetupPacket->Length)
                RequestLen = pSetupPacket->Length;

            DRV_USB_CX_VDMAWrite2(pData, RequestLen);
            break;
        case 4:
                break;
        // Interface descriptor
        case 5:
                break;
        // Device_Qualifier descritor
        case 6:
            {
                // If Current is HS. Then return FS
                // If Current is FS. Then return HS
                DRV_USB_CX_DONE();
            }
            break;
        // Other_Speed_Configuration
        case 7:
            {
                // If Current is HS. Then return FS
                // If Current is FS. Then return HS
                // It includes Configuration, Interface and Endpoint Table
                DRV_USB_CX_DONE();
            }
                break;
        default:
                return FALSE;
    }
    return TRUE;
}

BOOL DRV_USB_BT_StandardCommand()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch (pSetupPacket->Request)
    {
        case 6:
                // get descriptor
                DRV_USB_BT_OTGdescriptor();
                return TRUE;
        case 7:
                // We just have 1 descriptor do nothing
                DRV_USB_CX_DONE();
                return TRUE;
        case 8:
                // get configuration
                DRV_USB_CX_DONE();
                return TRUE;
        case 9:
                // set configuration
                DRV_USB_SetConfigureDone();
                //DRV_USB_CX_DONE();
                return TRUE;
        case 10:
                // get interface
                DRV_USB_CX_DONE();
                return TRUE;
        case 11:
                // set interface
                g_u32USBBTInitFinish = 2;
                DRV_USB_CX_DONE();
                return TRUE;
        case 12:
                // synch frame
                DRV_USB_CX_DONE();
                return TRUE;
        default:
                DRV_USB_CX_DONE();
                break;
    }

    return FALSE;
}

VOID DRV_USB_BT_ClassCmd(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(pData);
    UNUSED(id_ext);

    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    if( gUSBBTCMDBuff[gCmdWriteflg].Status == 0 )
    {// get data from hardware to queue
        gUSBBTCMDBuff[gCmdWriteflg].Status = 1;
        gUSBBTCMDBuff[gCmdWriteflg].DataLen = pSetupPacket->Length;
        if(gUSBBTCMDBuff[gCmdWriteflg].DataLen > 0)
            DRV_USB_CX_VDMARead2(gUSBBTCMDBuff[gCmdWriteflg].pu8BTBuff, gUSBBTCMDBuff[gCmdWriteflg].DataLen);
        else
        	gUSBBTCMDBuff[gCmdWriteflg].Status = 0;
    }
    else
    {// No queue to store
        MSG_MessageSend(handle, MsgId, NULL);
    }

    DRV_USB_CX_DONE();
}

VOID DRV_USB_BT_CX_OUT(U32 DataLen)
{
	UNUSED(DataLen);

}

VOID DRV_USB_BT_CX_TRANS_CMPL()
{
	if(gUSBBTCMDBuff[gCmdWriteflg].Status == 1)
	{
		gUSBBTCMDBuff[gCmdWriteflg].Status = 2;
		if( gCmdWriteflg == (USB_BT_CMD_BUFFER_NUM - 1))
			gCmdWriteflg = 0;
		else
			gCmdWriteflg += 1;
	}

    DRV_USB_CX_DONE();
}

U32 DRV_USB_BT_SendAcl(U8 *pBuffer, U32 DataLen)
{
	UNUSED(pBuffer);
	UNUSED(DataLen);

    if(g_u32USBBTInitFinish < 1)
        return 0;
	else
        return 1;
}
VOID DRV_USB_BT_AclCMPL(void)
{
    DRV_USB_EP_DONE(3);

    if(gUSBBTACLTC[gAclReadflg].Status == 3)
    {
        gUSBBTACLTC[gAclReadflg].CurLen = 0;
        gUSBBTACLTC[gAclReadflg].Status = 0;
        if( gAclReadflg == (USB_BT_ACL_BUFFER_NUM - 1))
        {
            gAclReadflg = 0;
        }
        else
        {
            gAclReadflg += 1;
        }
    }
    else if(gUSBBTACLTC[gAclReadflg].Status == 2)
    {
        gUSBBTACLTC[gAclReadflg].Status = 3;
        DRV_USB_EP_VDMAWrite2(2, gUSBBTACLTC[gAclReadflg].pu8BTBuff, gUSBBTACLTC[gAclReadflg].DataLen, 0);
    }
    else if( gUSBBTACLTC[gAclReadflg].Status == 1)
    {
        //MSG_MessageSend(gUSBBTACLTC, DRV_USB_MSG_EP2_TRANS_CMPL, 0);  //yugo warring
    }

    DRV_USB_CX_DONE();
}


U32 DRV_USB_BT_SendEvt(U8 *pBuffer, U32 DataLen)
{
    if(g_u32USBBTInitFinish < 1)
        return 0;

    if(gUSBBTEVTBuff[gEvtWriteflg].Status == 0)
    {
        gUSBBTEVTBuff[gEvtWriteflg].Status = 1;
        gUSBBTEVTBuff[gEvtWriteflg].DataLen = DataLen;
        gUSBBTEVTBuff[gEvtWriteflg].CurLen = 0;
        memcpy(gUSBBTEVTBuff[gEvtWriteflg].pu8BTBuff, pBuffer, gUSBBTEVTBuff[gEvtWriteflg].DataLen);

        gUSBBTEVTBuff[gEvtWriteflg].Status = 2;

        if( gEvtWriteflg == (USB_BT_EVT_BUFFER_NUM - 1))
        {
            gEvtWriteflg = 0;
        }
        else
        {
            gEvtWriteflg += 1;
        }
    }

    MSG_MessageSend(gMSG_HandlerRegisteredForUSB, DRV_USB_MSG_EP1_TRANS_CMPL, 0);

    return DataLen;
}
VOID DRV_USB_BT_EVTCMPL(void)
{
    DRV_USB_EP_DONE(1);

    if(gUSBBTEVTBuff[gEvtReadflg].Status == 3)
    {
        gUSBBTEVTBuff[gEvtReadflg].CurLen = 0;
        gUSBBTEVTBuff[gEvtReadflg].Status = 0;
        if( gEvtReadflg == (USB_BT_EVT_BUFFER_NUM - 1))
        {
            gEvtReadflg = 0;
        }
        else
        {
            gEvtReadflg += 1;
        }
        MSG_MessageSend(gMSG_HandlerRegisteredForUSB, DRV_USB_MSG_EP1_TRANS_CMPL, 0);
    }
    else if(gUSBBTEVTBuff[gEvtReadflg].Status == 2)
    {
        gUSBBTEVTBuff[gEvtReadflg].Status = 3;
        DRV_USB_EP_VDMAWrite2(1, gUSBBTEVTBuff[gEvtReadflg].pu8BTBuff, gUSBBTEVTBuff[gEvtReadflg].DataLen, 0);
    }
    else if( gUSBBTEVTBuff[gEvtReadflg].Status == 1)
    {
        MSG_MessageSend(gMSG_HandlerRegisteredForUSB, DRV_USB_MSG_EP1_TRANS_CMPL, 0);
    }
}

U32 DRV_USB_BT_READ(U32 DataLen)
{
	if(DataLen == 0)
    {
        return 1;
    }

    // if( g_USBAudioBuff[g_writeflg].Status == 0 )
    // {
    // 	g_USBAudioBuff[g_writeflg].Status = 1;
    //     DRV_USB_EP_VDMAReadBlocking(3, g_USBAudioBuff[g_writeflg].pu8AudioBuff, DataLen);
    //     g_USBAudioBuff[g_writeflg].Status = 2;
    //     if( g_writeflg == (MAX_BUFF - 1))
    //     {
    //         g_writeflg = 0;
    //         g_start = 1;
    //     }
    //     else
    //         g_writeflg += 1;
    // }

    return 0;
}

NO_OPT U32 DRV_USB_BT_MSG_Handler(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(handle);
    UNUSED(pData);

    switch(MsgId)
    {
        case DRV_USB_STANDARD_CMD:          { DRV_USB_BT_StandardCommand();}break;
        case DRV_USB_CLASS_CMD:             { DRV_USB_BT_ClassCmd(handle, MsgId, pData, id_ext);}break;

        case DRV_USB_MSG_CX_OUT:            { DRV_USB_BT_CX_OUT(id_ext); } break;
        case DRV_USB_MSG_CX_TRANS_CMPL:     { DRV_USB_BT_CX_TRANS_CMPL(); } break;

        // Event
        case DRV_USB_MSG_EP1_TRANS_CMPL:    { DRV_USB_BT_EVTCMPL(); } break;


        // ACL R/W
        case DRV_USB_MSG_EP2_OUT :
            {
                DRV_USB_BT_READ(id_ext);
            }
            break;
        case DRV_USB_MSG_EP2_TRANS_CMPL:
            {
                DRV_USB_BT_AclCMPL();
            }
            break;

        // We wont use EP3(ISO) on this demo program.
        case DRV_USB_MSG_EP3_OUT : {} break;
        case DRV_USB_MSG_EP3_TRANS_CMPL: {}break;

        default:
            break;
    }

    return 0;
}
HandlerData  g_BTClassMSG  = {DRV_USB_BT_MSG_Handler};

VOID DRV_USB_BT_Release()
{
    g_u32USBBTInitFinish = 0;

    for(int x = 0; x < USB_BT_CMD_BUFFER_NUM; x++)
    {
        free(gUSBBTCMDBuff[x].pu8BTBuff);
    }
    for(int x = 0; x < USB_BT_ACL_BUFFER_NUM; x++)
    {
        free(gUSBBTACLTC[x].pu8BTBuff);
    }
    for(int x = 0; x < USB_BT_ACL_BUFFER_NUM; x++)
    {
        free(gUSBBTACLFC[x].pu8BTBuff);
    }
    for(int x = 0; x < USB_BT_EVT_BUFFER_NUM; x++)
    {
        free(gUSBBTEVTBuff[x].pu8BTBuff);
    }
}

extern VOID DRV_USB_Configure_BT();

VOID DRV_USB_BT_Init(void)
{
    if(g_u32USBBTInitFinish > 0 )
        return;

    g_u32USBBTInitFinish = 1;

    DRV_USB_InitV2();

    MSG_MessageUsbHandler(&g_BTClassMSG);

    // DRV_USB_Configure2_EP1(EP_TYPE_BULK, EP_DIRECTION_IN);
    // DRV_USB_Configure2_EP2(EP_TYPE_BULK, EP_DIRECTION_BI);
    // DRV_USB_Configure2_EP3(EP_TYPE_ISO, EP_DIRECTION_BI);

    // DRV_USB_EP_Configure_Len(1, 512);
    // DRV_USB_EP_Configure_Len(2, 512);
    // DRV_USB_EP_Configure_Len(3, 512);

    DRV_USB_Configure_BT();

    for(int x = 0; x < USB_BT_CMD_BUFFER_NUM; x++)
    {
        gUSBBTCMDBuff[x].pu8BTBuff = (U8 *)malloc(USB_BT_CMD_BUFFER_SIZE);
        gUSBBTCMDBuff[x].Status = 0;
        gUSBBTCMDBuff[x].DataLen = 0;
    }
    for(int x = 0; x < USB_BT_ACL_BUFFER_NUM; x++)
    {
        gUSBBTACLTC[x].pu8BTBuff = (U8 *)malloc(USB_BT_ACL_BUFFER_SIZE);
        gUSBBTACLTC[x].Status = 0;
        gUSBBTACLTC[x].DataLen = 0;
    }
    for(int x = 0; x < USB_BT_ACL_BUFFER_NUM; x++)
    {
        gUSBBTACLFC[x].pu8BTBuff = (U8 *)malloc(USB_BT_ACL_BUFFER_SIZE);
        gUSBBTACLFC[x].Status = 0;
        gUSBBTACLFC[x].DataLen = 0;
    }
    for(int x = 0; x < USB_BT_EVT_BUFFER_NUM; x++)
    {
        gUSBBTEVTBuff[x].pu8BTBuff = (U8 *)malloc(USB_BT_EVT_BUFFER_SIZE);
        gUSBBTEVTBuff[x].Status = 0;
        gUSBBTEVTBuff[x].DataLen = 0;
        gUSBBTEVTBuff[x].CurLen = 0;
    }
}
#endif

