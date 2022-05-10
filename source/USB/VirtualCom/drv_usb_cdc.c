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
#if 0
#include "drv_usb.h"
#include "drv_usb_cdc.h"
#include "stdlib.h"
#include "log.h"
#include "sink.h"


#define SEND_ENCAPSULATED_COMMAND               0x00
#define GET_ENCAPSULATED_RESPONSE               0x01
#define SET_COMM_FEATURE                        0x02
#define GET_COMM_FEATURE                        0x03
#define CLEAR_COMM_FEATURE                      0x04
#define SET_LINE_CODING                         0x20
#define GET_LINE_CODING                         0x21
#define SET_CONRTROL_LINE_STATE                 0x22
#define SET_LINE_PARMS                          0x34
#define GET_LINE_PARMS                          0x35

#define USB_CDC_ACM_VENDOR_ID                   0x7777
#define USB_CDC_ACM_PRODUCT_ID                  0x6663
#define USB_CDC_ACM_RELEASE_ID                  0x0100

static U8 ACM_Manufacturer[] =
{
    14,
    0x03,
    'A', 0x00, 'i', 0x00, 'r', 0x00, 'o', 0x00, 'h', 0x00, 'a', 0x00
};

static U8 ACM_Production[] =
{
    16,
    0x03,
    'C', 0x00, 'D', 0x00, 'C', 0x00, '-', 0x00, 'A', 0x00, 'C', 0x00, 'M', 0x00
};

static U8 ACM_SerialNumber[] =
{
    10,
    0x03,
    '1', 0x00, '1', 0x00, '0', 0x00, '0', 0x00, 0x00, 0x00, 0x00, 0x00
};

static U8 u8USB_CDC_ACM_HSDeviceDescriptor[] =
{
    0x12,                               /* 0 bLength */
    DT_DEVICE,                          /* 1 bdescriptorType */
    0x00,                               /* 2 bcdUSB version */
    0x02,                               /* 3 bcdUSB version */
    0xEF,                               /* 4 bDeviceClass */
    0x02,                               /* 5 bDeviceSubClass */
    0x01,                               /* 6 bDeviceProtocol */
    0x40,                               /* 7 bMaxPacketSize for EP0 - max = 64 */
    (USB_CDC_ACM_VENDOR_ID & 0xFF),     /* 8 idVendor */
    (USB_CDC_ACM_VENDOR_ID >> 8),       /* 9 idVendor */
    (USB_CDC_ACM_PRODUCT_ID & 0xFF),    /* 10 idProduct */
    (USB_CDC_ACM_PRODUCT_ID >> 8),      /* 11 idProduct */
    (USB_CDC_ACM_RELEASE_ID & 0xFF),    /* 12 bcdDevice */
    (USB_CDC_ACM_RELEASE_ID >> 8),      /* 13 bcdDevice */
    0x01,                               /* 14 iManufacturer - index of string*/
    0x02,                               /* 15 iProduct - index of string*/
    0x03,                               /* 16 iSerialNumber - index of string*/
    0x01                                /* 17 bNumConfigurations */
};

static U8 u8USB_CDC_ACM_HSConfigOTGDescriptor[] =
{
    0x09,                               /* 0 bLength */
    DT_CONFIGURATION,                   /* 1 bDescriptortype - Configuration*/
    0x72, 0x00,                         /* 2 wTotalLength */
    0x02,                               /* 4 bNumInterfaces */
    0x01,                               /* 5 bConfigurationValue */
    0x00,                               /* 6 iConfiguration */
    0xC0,                               /* 7 bmAttributes */
    0x00,                               /* 8 bMaxPower */

    /* ACM Group 1 */
    0x08,                               /* bLength: Interface Descriptor size */
    0x0B,                               /* bDescriptorType */
    0x00,                               /* bFirstInterface */
    0x02,                               /* bInterfaceCount */
    0x02,                               /* bFunctionClass */
    0x02,                               /* bFunctionSubClass */
    0x00,                               /* bFunctionProtocol */
    0x02,                               /* iFunction */

    /* CDC Communication interface */
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x00,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x01,                               /* 4 bNumEndpoints */
    0x02,                               /* 5 bInterfaceClass */
    0x02,                               /* 6 bInterfaceSubClass - Abstract Control Model */
    0x00,                               /* 7 bInterfaceProtocol - AT Command V.250 protocol */
    0x00,                               /* 8 iInterface */

    /* Header Functional descriptor */
    0x05,                               /* 0 bLength */
    0x24,                               /* 1 bDescriptortype, CS_INTERFACE */
    0x00,                               /* 2 bDescriptorsubtype */
    0x10, 0x01,                         /* 3 bcdCDC */

    /* Call Management Functional descriptor */
    0x05,                               /* 0 bLength */
    0x24,                               /* 1 bDescriptortype, CS_INTERFACE */
    0x01,                               /* 2 bDescriptorsubtype */
    0x01,                               /* 3 bmCapabilities */
    0x01,                               /* 4 bDataInterface */

    /* ACM Functional descriptor */
    0x04,                               /* 0 bLength */
    0x24,                               /* 1 bDescriptortype, CS_INTERFACE */
    0x02,                               /* 2 bDescriptorsubtype */
    0x02,                               /* 3 bmCapabilities */


    /* Union Functional descriptor */
    0x05,                               /* 0 bLength */
    0x24,                               /* 1 bDescriptortype, CS_INTERFACE */
    0x06,                               /* 2 bDescriptorsubtype */
    0x00,                               /* 3 bControlInterface */
    0x01,                               /* 4 bSubordinateInterface0 */


    /* Notification Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x83,                               /* 2 bEndpointAddress */
    0x03,                               /* 3 bmAttributes */
    0x40,                               /* 4 wMaxPacketSize - Low */
    0x00,                               /* 5 wMaxPacketSize - High */
    0x04,                               /* 6 bInterval */


    /* CDC Data interface */
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType */
    0x01,                               /* 2 bInterfacecNumber */
    0x00,                               /* 3 bAlternateSetting */
    0x02,                               /* 4 bNumEndpoints */
    0x0A,                               /* 5 bInterfaceClass */
    0x00,                               /* 6 bInterfaceSubClass */
    0x00,                               /* 7 bInterfaceProtocol*/
    0x00,                               /* 8 iInterface */


    /* Data OUT Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x81,                               /* 2 bEndpointAddress */
    0x02,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low */
    0x02,                               /* 5 wMaxPacketSize - High */
    0x00,                               /* 6 bInterval */


    /* Data IN Endpoint descriptor */
    0x07,                               /* 0 bLength */
    DT_ENDPOINT,                        /* 1 bDescriptorType */
    0x02,                               /* 2 bEndpointAddress */
    0x02,                               /* 3 bmAttributes */
    0x00,                               /* 4 wMaxPacketSize - Low byte */
    0x02,                               /* 5 wMaxPacketSize - High byte */
    0x00,                               /* 6 bInterval */

};

static U8 LINE_CODING_1[] =
{
    0x80, 0x25, 0x00, 0x00, //0xC0, 0xC6, 0x2D, 0x00, // Baudrate	//0x80, 0x25, 0x00, 0x00, // Baudrate
    0x00,                   // stop bit
    0x00,                   // parity check
    0x08                    // data bit
};

static fDRV_USB_CDC_HDLR _DrvUsbCdcHdlr[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

volatile U32 gCDCInitFlg = USB_CDC_INIT_NONE;
volatile U32 gVCOMStatus = 0;

// for OUT endpoint queue control
//volatile U32 gOUTWriteflg = 0;
//volatile U32 gOUTReadflg = 0;

//volatile U32 gINWriteflg = 0;
//volatile U32 gINReadflg = 0;

// for IN endpoint hardware status
//volatile U32 gHardwareReady = 1;

USB_CDC_BUFF_T gUSBCDCOUTBuff[CDC_BUFFER_NUM];
USB_CDC_BUFF_T gUSBCDCINBuff[CDC_BUFFER_NUM];
USB_CDC_BUFF_T gUSBCDCOUTBuffC;
volatile U32 gWriteOffset = 0;
volatile U32 gReadOffset = 0;
BOOL _gDrvUsbBusBusyToSend = FALSE;

//
// -------------------------------------------------------------------------------------------------------------
// USB Handle

VOID DRV_USB_CDC_ACM_SetLineCoding1()
{
    DRV_USB_CX_VDMARead2(LINE_CODING_1, sizeof(LINE_CODING_1));
}
VOID DRV_USB_CDC_ACM_GetLineCoding1()
{
    DRV_USB_CX_VDMAWrite2(LINE_CODING_1, sizeof(LINE_CODING_1));
}

// Descriptor function
VOID DRV_USB_CDC_SendDeviceDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_CDC_ACM_HSDeviceDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_CDC_ACM_HSDeviceDescriptor, dataLen);
}
VOID DRV_USB_CDC_SendConfigureDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_CDC_ACM_HSConfigOTGDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_CDC_ACM_HSConfigOTGDescriptor, dataLen);
}
BOOL DRV_USB_CDC_OTGdescriptor(void)
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();
    U32 RequestLen = 0;
    U16 mySize = 0;
    U8  *pData;
    switch ((U8)(pSetupPacket->Value >> 8))
    {
        case 1:
            {
                DRV_USB_CDC_SendDeviceDescription(pSetupPacket->Length);
            }
            break;
        case 2:
            switch ((U8)pSetupPacket->Value)
            {
                case 0x00:
                        mySize = sizeof(u8USB_CDC_ACM_HSConfigOTGDescriptor);
                        u8USB_CDC_ACM_HSConfigOTGDescriptor[2] = mySize;
                        u8USB_CDC_ACM_HSConfigOTGDescriptor[3] = (mySize >> 8);
                        DRV_USB_CDC_SendConfigureDescription(pSetupPacket->Length);
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
                       pData = ACM_Manufacturer;
                       RequestLen =  ACM_Manufacturer[0];
                    break;
                case 0x02:
                       pData = ACM_Production;
                       RequestLen =  ACM_Production[0];
                    break;
                case 0x03:
                       pData = ACM_SerialNumber;
                       RequestLen = ACM_SerialNumber[0];
                    break;
                default:
                    return FALSE;
            }
            if (RequestLen > pSetupPacket->Length)
            	RequestLen = pSetupPacket->Length;

            DRV_USB_CX_VDMAWrite2(pData, RequestLen);
            //DRV_USB_CX_VDMAWriteBlocking(pData, RequestLen);
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
// Message Handle function
BOOL DRV_USB_CDC_ACM_StandardCommand()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch (pSetupPacket->Request)
    {
        case 6:     // get descriptor
                DRV_USB_CDC_OTGdescriptor();
                //DRV_USB_CX_DONE();
                return TRUE;
        case 7:     // We just have 1 configuration no need change
                //return(bSet_OTGdescriptor2());
                DRV_USB_CX_DONE();
                return TRUE;
        case 8:     // get configuration
                DRV_USB_CX_DONE();
                return TRUE;
        case 9: // set configuration
                DRV_USB_SetConfigureDone();
                return TRUE;
        case 10:// get interface
                DRV_USB_CX_DONE();
                return TRUE;
        case 11:// set interface
                DRV_USB_CX_DONE();
                return TRUE;
        case 12:    // synch frame
                DRV_USB_CX_DONE();
                return TRUE;
        default:
                break;
    }

    return FALSE;
}
VOID DRV_USB_CDC_ACM_ClassCmd()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch(pSetupPacket->Request)
    {
        case SEND_ENCAPSULATED_COMMAND:
            {

            }
            break;
        case GET_ENCAPSULATED_RESPONSE:
            {

            }
            break;
        case SET_COMM_FEATURE:
            {

            }
            break;
        case GET_COMM_FEATURE:
            {

            }
            break;
        case CLEAR_COMM_FEATURE:
            {

            }
            break;
        case GET_LINE_CODING:
            {
                if(pSetupPacket->Object == 1 && pSetupPacket->Index == 0)
                {
                    DRV_USB_CDC_ACM_GetLineCoding1();
                }
            }
            break;
        case SET_LINE_CODING:
            {
                if(pSetupPacket->Object == 1 && pSetupPacket->Index == 0)
                {
                    DRV_USB_CDC_ACM_SetLineCoding1();
                }
            }
            break;
        case SET_CONRTROL_LINE_STATE:
            {
                if(pSetupPacket->Object == 1 && pSetupPacket->Index == 0)
                {
                    if((pSetupPacket->Value & 0x0003) != 0)
                    {
                        if((gVCOMStatus & 0x00000001) == 0)
                        {
                            gVCOMStatus |= 0x00000001;
                        }
                    }
                    DRV_USB_CX_DONE();
                }
            }
            break;
    }
}
VOID DRV_USB_CDC_ACM_CX_OUT(U32 Len)
{
    UNUSED(Len);

}
VOID DRV_USB_CDC_ACM_CX_TRANS_CMPL(void)
{
    DRV_USB_CX_DONE();
}


VOID DRV_USB_CDC_ACM1_READ(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(MsgId);
    UNUSED(pData);
    UNUSED(id_ext);

    U16 UnwrapSize;
    U16 WrapSize;
    U8* tmpBuf;

    if((gVCOMStatus & 0x00000001) == 0)
        return;

    if( gUSBCDCOUTBuffC.Status == 0 )
    {// get data from hardware to queue
        gUSBCDCOUTBuffC.Status = 1;
        gUSBCDCOUTBuffC.DataLen = DRV_USB_EP_DataLen(2);

        if(gUSBCDCOUTBuffC.DataLen > 0)
        {
            tmpBuf = malloc(CDC_BUFFER_SIZE);
            DRV_USB_EP_VDMARead2(2, tmpBuf, gUSBCDCOUTBuffC.DataLen);
            if (gWriteOffset + gUSBCDCOUTBuffC.DataLen <= CDC_CBUFFER_SIZE)
            {
                memcpy(gUSBCDCOUTBuffC.pu8CDCBuff + gWriteOffset, tmpBuf, gUSBCDCOUTBuffC.DataLen);
                gWriteOffset += gUSBCDCOUTBuffC.DataLen;
                if (gWriteOffset > CDC_CBUFFER_SIZE)
                    gWriteOffset -= CDC_CBUFFER_SIZE;
            }
            else
            {
                UnwrapSize = CDC_CBUFFER_SIZE - gWriteOffset;
                WrapSize = gUSBCDCOUTBuffC.DataLen - UnwrapSize;
                memcpy(gUSBCDCOUTBuffC.pu8CDCBuff + gWriteOffset, tmpBuf, UnwrapSize);
                memcpy(gUSBCDCOUTBuffC.pu8CDCBuff, tmpBuf + UnwrapSize, WrapSize);
                gWriteOffset = WrapSize;
            }
            free(tmpBuf);
        }
        else
            gUSBCDCOUTBuffC.Status = 0;
    }
    else
    {// No queue to store
        MSG_MessageSend(handle, MsgId, NULL);
    }
}

VOID DRV_USB_CDC_ACM1_READCMPL()
{

    gUSBCDCOUTBuffC.Status = 0;//2; //already transfer to CBuff

    DRV_USB_EP_DONE(2);
}

U32 DRV_USBCDCACM_MSG_Handler(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(handle);
    UNUSED(pData);
    UNUSED(id_ext);

    switch(MsgId)
    {
        case DRV_USB_STANDARD_CMD:          { DRV_USB_CDC_ACM_StandardCommand();}break;
        case DRV_USB_CLASS_CMD:             { DRV_USB_CDC_ACM_ClassCmd();}break;
        case DRV_USB_MSG_CX_OUT:            { DRV_USB_CDC_ACM_CX_OUT(id_ext); } break;
        case DRV_USB_MSG_CX_TRANS_CMPL:     { DRV_USB_CDC_ACM_CX_TRANS_CMPL(); } break;

        case DRV_USB_MSG_EP1_TRANS_CMPL:    { _DrvUsbCdcHdlr[USB_CDC_HDLR_ID_TX_CPL_CALLBACK](); } break;
        case DRV_USB_MSG_EP2_OUT :          { DRV_USB_CDC_ACM1_READ(handle, MsgId, pData, id_ext); } break;
        case DRV_USB_MSG_EP2_TRANS_CMPL:    { DRV_USB_CDC_ACM1_READCMPL(); } break;
        default:
            break;
    }
    return 0;
}
HandlerData  gUSBCDC_MSGUSB  = {DRV_USBCDCACM_MSG_Handler};


//
// -------------------------------------------------------------------------------------------------------------
// API

U32 DRV_USB_CDC_ACM1_GetDataLen()
{
#if 0
/*    if(gCDCInitFlg == USB_CDC_INIT_NONE)
        return 0;

    if(gUSBCDCOUTBuffC.Status == 2)
    {
        return gUSBCDCOUTBuffC.DataLen;
    }

    return 0;*/
    STREAM_USB_TX_HEADER_STRU_PTR buffer;
    U32 RealRequiredSize = extra + sizeof(STREAM_USB_TX_HEADER_STRU);
    U8  type;

    /* if last claim do not flush , clear last claim */
    if( (buffer = (STREAM_USB_TX_HEADER_STRU_PTR)sink->streamBuffer.QueueInfo.memblk.addr) != NULL)
    {
        if(buffer->txType == USB_TX_TYPE_QUEUE)
        {
            OSMEM_Put(buffer);
        }
        else if(buffer->txType == USB_TX_TYPE_HEAP)
        {
            /* its heap actually */
            UsbHeapBuffer.WriteOffset -= (buffer->length + sizeof(STREAM_USB_TX_HEADER_STRU));
            if(UsbHeapBuffer.WriteOffset == UsbHeapBuffer.ReadOffset)
            {
                UsbHeapBuffer.WriteOffset = 0;
                UsbHeapBuffer.ReadOffset = 0;
            }
        }
    }

    /* use heap if extra size large than osmem block */
    if( RealRequiredSize > gSysMemoryEntries.OSMEM2SmallTxEntrySize)
    {
        type = USB_TX_TYPE_HEAP;
        /* get from heap */
        if(bUsbHeapBuffer)
        {
            buffer = (STREAM_USB_TX_HEADER_STRU_PTR)(UsbHeapBuffer.startaddr[0] + UsbHeapBuffer.WriteOffset);
            UsbHeapBuffer.WriteOffset += RealRequiredSize;
        }
        else
        {
            bUsbHeapBuffer = TRUE;
            UsbHeapBuffer.length = 2048;
            UsbHeapBuffer.startaddr[0] = OSHEAP_malloc(UsbHeapBuffer.length + sizeof(STREAM_USB_TX_HEADER_STRU));
            UsbHeapBuffer.ReadOffset   = 0;
            UsbHeapBuffer.WriteOffset  = RealRequiredSize;
            buffer = (STREAM_USB_TX_HEADER_STRU_PTR)UsbHeapBuffer.startaddr[0];
        }
    }
    else
    {
        type = USB_TX_TYPE_QUEUE;
        buffer = OS_malloc(RealRequiredSize);
    }

    if(buffer)
    {
        buffer->offset = sizeof(STREAM_USB_TX_HEADER_STRU);
        buffer->length = extra;
        buffer->txType = type;

        sink->streamBuffer.QueueInfo.memblk.addr   = (U8*)buffer;
        sink->streamBuffer.QueueInfo.memblk.length = extra;

        return buffer->offset;
    }
    else
    {
        return SINK_INVALID_CLAIM;
    }
#endif
return 0;

}

U32 DRV_USB_CDC_ACM1_Send(U8 *pBuffer, U32 DataLen)
{
    UNUSED(pBuffer);
    UNUSED(DataLen);

    if(gCDCInitFlg == 0)
        return 0;
    // if((gVCOMStatus & 0x00000001) != 0 && gHardwareReady == 1)
    // {
    //     gHardwareReady = 0;
    //     if(DataLen == 512)
    //         DRV_USB_EP_VDMAWrite2(1, pBuffer, DataLen, 1);
    //     else
    //         DRV_USB_EP_VDMAWrite2(1, pBuffer, DataLen, 0);
    //     return DataLen;
    // }
    if((gVCOMStatus & 0x00000001) == 0)
        return 0;

    /*if( gUSBCDCINBuff[gINWriteflg].Status == 0 )
    {// get data from hardware to queue
        gUSBCDCINBuff[gINWriteflg].Status = 1;
        gUSBCDCINBuff[gINWriteflg].DataLen = DataLen;
        memcpy(gUSBCDCINBuff[gINWriteflg].pu8CDCBuff, pBuffer, DataLen);
        gUSBCDCINBuff[gINWriteflg].Status = 2;

        if( gINWriteflg == (CDC_BUFFER_NUM - 1))
        {
            gINWriteflg = 0;
        }
        else
        {
            gINWriteflg += 1;
        }

        MSG_MessageSend(gMSG_HandlerRegisteredForUSB, DRV_USB_MSG_EP1_TRANS_CMPL, 0);

        return DataLen;
    }*/

    return 0;
}
U32 DRV_USB_CDC_ACM1_Send_Fifo(U8 *pBuffer, U32 DataLen)
{
    if(gCDCInitFlg == 0 || _gDrvUsbBusBusyToSend)
        return 0;

    if((gVCOMStatus & 0x00000001) == 0)
        return 0;

    _gDrvUsbBusBusyToSend = TRUE;

    if(DataLen % 512 == 0)
        DRV_USB_EP_VDMAWrite2(1, pBuffer, DataLen, 1);
    else
        DRV_USB_EP_VDMAWrite2(1, pBuffer, DataLen, 0);

    return DataLen;
}

U32 DRV_USB_CDC_ACM1_Get(U8 *pBuffer, U32 RequestLen)
{
    UNUSED(pBuffer);
    UNUSED(RequestLen);

/*    if(gCDCInitFlg == 0)
        return 0;

    if(gUSBCDCOUTBuff[gOUTReadflg].Status == 2)
    {
        gUSBCDCOUTBuff[gOUTReadflg].Status = 3;
        int tempflg = gOUTReadflg;
        U32 DataLen =  gUSBCDCOUTBuff[gOUTReadflg].DataLen;
        memcpy(pBuffer, gUSBCDCOUTBuff[gOUTReadflg].pu8CDCBuff, RequestLen);

        if( gOUTReadflg == (CDC_BUFFER_NUM - 1))
            gOUTReadflg = 0;
        else
            gOUTReadflg += 1;

        gUSBCDCOUTBuff[tempflg].Status = 0;

        return DataLen;
    }*/

    return 0;
}


/**
 * DSP_D2C_BufferCopy
 *
 * @Author : Yoyo <SYChiu@airoha.com.tw>
 */
U8 DRV_USB_CDC_D2C_BufferCopy(VOID* DestBuf,
                        VOID* SrcBuf,
                        U16 CopySize,
                        VOID* DestCBufStart,
                        U16 DestCBufSize)
{
    U8* DestCBufEnd = (U8*)((U8*)DestCBufStart + DestCBufSize);
    U16 UnwrapSize = (U8*)DestCBufEnd - (U8*)DestBuf; /* Remove + 1 to sync more common usage */
    S16 WrapSize  = CopySize - UnwrapSize;

    //OS_ASSERT((DestCBufEnd >= (U8*)DestBuf) && (DestBuf >= DestCBufStart));
    if(!((DestCBufEnd >= (U8*)DestBuf) && (DestBuf >= DestCBufStart)))
    return 0;

    if (WrapSize > 0)
    {
        memcpy(DestBuf, SrcBuf, UnwrapSize);

        while (WrapSize > DestCBufSize)
        {
            memcpy(DestCBufStart, (U8*)SrcBuf + UnwrapSize, DestCBufSize);
            WrapSize -= DestCBufSize;
        }

        memcpy(DestCBufStart, (U8*)SrcBuf + UnwrapSize, WrapSize);
    }
    else
    {
        memcpy(DestBuf, SrcBuf, CopySize);
    }
    return 1;
}

/**
 * DRV_USB_CDC_C2D_BufferCopy
 *
 */
U8 DRV_USB_CDC_C2D_BufferCopy(VOID* pDesBuf,
                        U8* pSrcBuf,
                        U8* pSrcBufHead,
                        U16 CopySize,
                        U16 SrcBufTotalSize)
{
    U16 UnwrapSize;
    U16 WrapSize;
    U8* pSrcBufEnd = (U8*)(pSrcBufHead + SrcBufTotalSize);

    if (pSrcBuf + CopySize <= pSrcBufEnd)
    {
        memcpy(pDesBuf, pSrcBuf, CopySize);
    }
    else
    {
        UnwrapSize = pSrcBufEnd - pSrcBuf;
        WrapSize = CopySize - UnwrapSize;

        memcpy(pDesBuf, pSrcBuf, UnwrapSize);
        memcpy(pDesBuf + UnwrapSize, (U8*)pSrcBufHead, WrapSize);
    }

    return 1;
}

/*U32 DRV_USB_CDC_ACM1_Get_CBUF(U8 *pCBuffer, U32 RequestLen, VOID* DestCBufStart, U16 DestCBufSize)
{
    U32 DataLength;
    if(gUSBCDCOUTBuffC.Status == 2)
    {
        gUSBCDCOUTBuffC.Status = 3;
        int tempflg = gOUTReadflg;

        U8 status =DRV_USB_CDC_D2C_BufferCopy(pCBuffer,
        gUSBCDCOUTBuffC.pu8CDCBuff,
        RequestLen,
        DestCBufStart,
        DestCBufSize);
        if(status ==0)
            return 0;

        DataLength = gUSBCDCOUTBuffC.DataLen;

        gUSBCDCOUTBuffC.Status = 0;

        return DataLength;//RequestLen;
    }

    return 0;
}*/


VOID DRV_USB_CDC_ACM_Relese()
{
    gVCOMStatus = 0;
    gWriteOffset = 0;
    gReadOffset = 0;

    gUSBCDCOUTBuffC.Status = 0;
    gUSBCDCOUTBuffC.DataLen = 0;
    //free(gUSBCDCOUTBuffC.pu8CDCBuff);
}

// Init Function
VOID DRV_USB_CDC_ACM_MEM_Init(VOID)
{
    gUSBCDCOUTBuffC.Status = 0;
    gUSBCDCOUTBuffC.DataLen = 0;
    gUSBCDCOUTBuffC.pu8CDCBuff = malloc(CDC_CBUFFER_SIZE);
}

VOID DRV_USB_CDC_ACM_Init(VOID)
{

    if(gCDCInitFlg == USB_CDC_INIT_DONE)
        return;

    MSG_MessageUsbHandler(&gUSBCDC_MSGUSB);

    DRV_USB_InitV2();
    DRV_USB_Configure2_EP1(EP_TYPE_BULK, EP_DIRECTION_IN);
    DRV_USB_Configure2_EP2(EP_TYPE_BULK, EP_DIRECTION_OUT);

    DRV_USB_EP_Configure_Len(1, CDC_BUFFER_SIZE);
    DRV_USB_EP_Configure_Len(2, CDC_BUFFER_SIZE);

    gVCOMStatus = 0;
    _gDrvUsbBusBusyToSend = FALSE;
    //gOUTWriteflg = 0;
    //gOUTReadflg = 0;
    //gINWriteflg = 0;
    //gINReadflg = 0;
    //gHardwareReady = 1;

    gCDCInitFlg = USB_CDC_INIT_DONE;
}

VOID DRV_USB_CDC_ACM_DeInit(VOID)
{
    if(gCDCInitFlg == USB_CDC_INIT_NONE)
        return;

    MSG_MessageUsbHandler(NULL);
    DRV_USB_DeInitV2();

    //DRV_USB_EP_Reset(1);
    //DRV_USB_EP_Reset(2);

    gVCOMStatus = 0;
    //gOUTWriteflg = 0;
    //gOUTReadflg = 0;
    //gINWriteflg = 0;
    //gINReadflg = 0;
    //gHardwareReady = 1;
    gCDCInitFlg = USB_CDC_INIT_NONE;
    _gDrvUsbBusBusyToSend = FALSE;

    DRV_USB_CDC_ACM_Relese();
}

/**
 * DRV_USB_CDC_RegisterHdlr
 *
 * A function is used to register USB CDC handlers
 *
 * @param[in] fCallback The function entry of callback function
 */

VOID DRV_USB_CDC_RegisterHdlr(
         USB_CDC_HDLR_ID HdlrId,
         fDRV_USB_CDC_HDLR fCallback)
{
    if(fCallback != NULL)
        _DrvUsbCdcHdlr[HdlrId] = fCallback;
}

#endif
