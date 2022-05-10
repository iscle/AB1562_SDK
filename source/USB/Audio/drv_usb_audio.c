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
#include "drv_usb_audio.h"
#include "stdlib.h"
#include "types.h"


#define USB_AUDIO_VENDOR_ID                   0x7777
#define USB_AUDIO_PRODUCT_ID                  0x6660
#define USB_ADUIO_RELEASE_ID                  0x0100


typedef struct
{
    U8  *pu8AudioBuff;
    U32 Status;
}USB_AUDIO_BUFF_T, *LPUSB_AUDIO_BUFF_T;
USB_AUDIO_BUFF_T g_USBAudioBuff[USB_AUDIO_BUFFER_NUM];

volatile U32 g_writeflg = 0;
volatile U32 g_readflg = 0;
volatile U32 g_start = 0;
volatile U32 g_u32Ready = 0;
volatile U32 g_u32InitFinish = 0;

static U8 Audio_Manufacturer[] =
{
    14,
    0x03,
    'A', 0x00, 'i', 0x00, 'r', 0x00, 'o', 0x00, 'h', 0x00, 'a', 0x00
};

static U8 Audio_Production[] =
{
    16,
    0x03,
    'A', 0x00, 'U', 0x00, 'D', 0x00, 'I', 0x00, 'O', 0x00,  ' ', 0x00, ' ', 0x00
};

static U8 Audio_SerialNumber[] =
{
    10,
    0x03,
    '1', 0x00, '0', 0x00, '0', 0x00, '0', 0x00, ' ', 0x00, ' ', 0x00
};

static U8 u8USB_AUDIO_HSDeviceDescriptor[] =
{
    0x12,                               /* 0 bLength */
    DT_DEVICE,                          /* 1 bdescriptorType */
    0x00,                               /* 2 bcdUSB version */
    0x02,                               /* 3 bcdUSB version */
    0x00,                               /* 4 bDeviceClass */
    0x00,                               /* 5 bDeviceSubClass */
    0x00,                               /* 6 bDeviceProtocol */
    0x40,                               /* 7 bMaxPacketSize for EP0 - max = 64 */
    (USB_AUDIO_VENDOR_ID & 0xFF),       /* 8 idVendor */
    (USB_AUDIO_VENDOR_ID >> 8),         /* 9 idVendor */
    (USB_AUDIO_PRODUCT_ID & 0xFF),      /* 10 idProduct */
    (USB_AUDIO_PRODUCT_ID >> 8),        /* 11 idProduct */
    (USB_ADUIO_RELEASE_ID & 0xFF),      /* 12 bcdDevice */
    (USB_ADUIO_RELEASE_ID >> 8),        /* 13 bcdDevice */
    0x01,                               /* 14 iManufacturer - index of string*/
    0x02,                               /* 15 iProduct - index of string*/
    0x03,                               /* 16 iSerialNumber - index of string*/
    0x01                                /* 17 bNumConfigurations */
};

// 3 Interface
// Interface 0 Audio Control
// Interface 1 Audio Stream speaker
// interface 2 Audio Stream microphone

//#define EnableHID
#define Enable48K
//#define Enable48K_96K
//#define Enable48K_96K_192K
//#define EnableMIC48K

static U8 u8USB_AUDIO_HSConfigOTGDescriptor[] =
{
    0x09,                               /* 0 bLength */
    DT_CONFIGURATION,                   /* 1 bDescriptortype - Configuration*/
    0xC6, 0x00,
    0x03,
    0x01,                               /* 5 bConfigurationValue */
    0x00,                               /* 6 iConfiguration */
    0xC0,                               /* 7 bmAttributes */
    0x00,                               /* 8 bMaxPower */

    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x00,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x00,                               /* 4 bNumEndpoints */
    0x01,                               /* 5 bInterfaceClass */
    0x01,                               /* 6 bInterfaceSubClass - Audio control */
    0x00,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    /* Class Specific Header */
    0x0A,
    0x24,                               /* bDescriptorType = CS_INTERFACE */
    0x01,                               /* bDescriptorSubType = HEADER */
    0x00, 0x01,                         /* bcdADC = Audio Device Class v1.00 */
    0x3E,
    0x00,                               /* wTotalLength MSB */
    0x02,
    0x01,                               /* baInterfaceNr(1) - AS#1 id */
    0x02,

    // Aduio Control 1
    /* Speaker IT */
    0x0c,           /* bLength */
    0x24,           /* bDescriptorType = CS_INTERFACE */
    0x02,           /* bDescriptorSubType = INPUT_TERMINAL */
    0x01,           /* bTerminalID */
    0x01, 0x01,     /* wTerminalType = USB streaming */
    0x00,           /* bAssocTerminal = none */
    0x02,           /* bNrChannels = 2 */
    0x03, 0x00,     /* wChannelConfig = left front and right front */
    0x00,           /* iChannelName = no string */
    0x00,           /* iTerminal = same as USB product string */

    /* Speaker Features */
    0x0d,                               /*bLength*/
    0x24,                               /*bDescriptorType = CS_INTERFACE */
    0x06,                               /*bDescriptorSubType = FEATURE_UNIT*/
    0x02,                               /*bUnitId*/
    0x01,                               /*bSourceId - Speaker IT*/
    0x02,                               /**/
    0x01, 0x00,                         /*bmaControls[0] = 0001 (Mute on Master Channel)*/
    0x02, 0x00,                         /*bmaControls[1] = 0002 (Vol on Left Front)*/
    0x02, 0x00,                         /*bmaControls[1] = 0002 (Vol on Right Front)*/
    0x00,                               /*iFeature = same as USB product string*/

    0x09,                               /* bLength */
    0x24,                               /* bDescriptorType = CS_INTERFACE */
    0x03,                               /* bDescriptorSubType = OUTPUT_TERMINAL */
    0x03,                               /* bTerminalID */
    0x01, 0x03,                         /* wTerminalType = Speaker */
    0x00,                               /* bAssocTerminal = none */
    0x02,                               /* bSourceID - Speaker Features*/
    0x00,                               /* iTerminal = same as USB product string */


    /* MIC IT */
    0x0c,                               /* bLength */
    0x24,                               /* bDescriptorType = CS_INTERFACE */
    0x02,                               /* bDescriptorSubType = OUT_TERMINAL */
    0x04,                               /* bTerminalID */
    0x01, 0x02,                         /* wTerminalType = USB streaming */
    0x00,                               /* bAssocTerminal = none */
    0x01,                               /* bNrChannels = */
    0x01, 0x00,                         /* wChannelConfig =  */
    0x00,                               /* iChannelName = no string */
    0x00,                               /* iTerminal = same as USB product string */

    0x09,                               /* bLength */
    0x24,                               /* bDescriptorType = CS_INTERFACE */
    0x03,                               /* bDescriptorSubType = INPUT_TERMINAL */
    0x05,                               /* bTerminalID */
    0x01, 0x01,                         /* wTerminalType = Speaker */
    0x00,                               /* bAssocTerminal = none */
    0x04,                               /* bSourceID - Speaker Features*/
    0x00,                               /* iTerminal = same as USB product string */

    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x00,                               /* 4 bNumEndpoints */
    0x01,                               /* 5 bInterfaceClass */
    0x02,                               /* 6 bInterfaceSubClass - Audio Streaming */
    0x00,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x01,                               /* 2 bInterfaceNumber - Interface 1 */
    0x01,                               /* 3 bAlternateSetting */
    0x01,                               /* 4 bNumEndpoints */
    0x01,                               /* 5 bInterfaceClass */
    0x02,                               /* 6 bInterfaceSubClass - Audio Streaming */
    0x00,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    0x07,                               // bLength (7)
    0x24,                               // bDescriptorType (CS_INTERFACE)
    0x01,                               // bDescriptorSubtype (AS_GENERAL)
    0x01,                               // bTerminalLink (terminal 3)
    0x00,                               // bDelay (none)
    0x01, 0x00,                         // wFormatTag (PCM format)

#ifdef Enable48K_96K_192K
    0x11,
#endif

#ifdef Enable48K_96K
    0x0E,
#endif

#ifdef Enable48K
    0x0B,                               // bLength (11)
#endif

    0x24,                               // bDescriptorType (CS_INTERFACE)
    0x02,                               // bDescriptorSubtype (FORMAT_TYPE)
    0x01,                               // bFormatType (TYPE_I)
    0x02,                               // bNrChannels (2)
    0x03,
    0x18,                               // bBitResolution (24)

#ifdef Enable48K_96K_192K
    0x03,                               // bSamFreqType (1 sampling frequency)
    0x80,                               // 48,000 Hz (byte 0)
    0xBB,                               // 48,000 Hz (byte 1)
    0x00,                               // 48,000 Hz (byte 2)
    0x00,                               // 96,000 Hz (byte 0)
    0x77,                               // 96,000 Hz (byte 1)
    0x01,                               // 96,000 Hz (byte 2)
    0x00,                               // 192,000 Hz (byte 0)
    0xEE,                               // 192,000 Hz (byte 1)
    0x02,                               // 192,000 Hz (byte 2)
#endif
#ifdef Enable48K_96K
    0x02,                               // bSamFreqType (1 sampling frequency)
    0x80,                               // 48,000 Hz (byte 0)
    0xBB,                               // 48,000 Hz (byte 1)
    0x00,                               // 48,000 Hz (byte 2)
    0x00,                               // 96,000 Hz (byte 0)
    0x77,                               // 96,000 Hz (byte 1)
    0x01,                               // 96,000 Hz (byte 2)
#endif
#ifdef Enable48K
    0x01,                               // bSamFreqType (1 sampling frequency)
    0x80,                               // 48,000 Hz (byte 0)
    0xBB,                               // 48,000 Hz (byte 1)
    0x00,                               // 48,000 Hz (byte 2)
#endif

    0x07,                               //
    0x25,                               //
    0x01,                               //
    0x80,                               //
    0x02,                               //
    0x20, 0x01,                         //

    0x09,                               // bLength (9)
    0x05,                               // bDescriptorType (ENDPOINT)
    0x03,                               // bEndpointAddress (EP2 out)
    0x01,                               // bmAttributes (asynchronous)
#ifdef Enable48K_96K
    0x40, 0x02,                         // wMaxPacketSize (388)
#endif
#ifdef Enable48K
    0x20, 0x01,
#endif

#ifdef Enable48K_96K_192K
    0x04,                               // bInterval (1000 us)
#endif
#ifdef Enable48K_96K
    0x04,                               // bInterval (1000 us)
#endif
#ifdef Enable48K
    0x04,                               // bInterval (1000 us)
#endif

    0x00,                               // bRefresh (0)
    0x00,                               // bSynchAddress (no synchronization)


    // Aduio Control 2
    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x02,                               /* 2 bInterfaceNumber - Interface 0 */
    0x00,                               /* 3 bAlternateSetting */
    0x00,                               /* 4 bNumEndpoints */
    0x01,                               /* 5 bInterfaceClass */
    0x02,                               /* 6 bInterfaceSubClass - Audio Streaming */
    0x00,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    0x09,                               /* 0 bLength */
    DT_INTERFACE,                       /* 1 bDescriptorType - Interface */
    0x02,                               /* 2 bInterfaceNumber - Interface 1 */
    0x01,                               /* 3 bAlternateSetting */
    0x01,                               /* 4 bNumEndpoints */
    0x01,                               /* 5 bInterfaceClass */
    0x02,                               /* 6 bInterfaceSubClass - Audio Streaming */
    0x00,                               /* 7 bInterfaceProtocol */
    0x00,                               /* 8 iInterface */

    0x07,                               // bLength (7)
    0x24,                               // bDescriptorType (CS_INTERFACE)
    0x01,                               // bDescriptorSubtype (AS_GENERAL)
    0x05,                               // bTerminalLink (terminal 4)
    0x00,                               // bDelay (none)
    0x01, 0x00,                         // wFormatTag (PCM format)

    0x0B,                               // bLength (11)
    0x24,                               // bDescriptorType (CS_INTERFACE)
    0x02,                               // bDescriptorSubtype (FORMAT_TYPE)
    0x01,                               // bFormatType (TYPE_I)
    0x02,                               // bNrChannels (1)
    0x03,
    0x18,                               // bBitResolution (24)
    0x01,                               // bSamFreqType (1 sampling frequency)


#ifdef EnableMIC48K
    0x80,                               // 48,000 Hz (byte 0)
    0xBB,                               // 48,000 Hz (byte 1)
    0x00,                               // 48,000 Hz (byte 2)
#else
    0x80,                               // 16,000 Hz (byte 0)
    0x3E,                               // 16,000 Hz (byte 1)
    0x00,                               // 16,000 Hz (byte 2)
#endif

    0x07,                               //
    0x25,                               //
    0x01,                               //
    0x80,                               //
    0x02,                               //
    0x00, 0x00,                         //

    0x09,                               // bLength (9)
    0x05,                               // bDescriptorType (ENDPOINT)
    0x81,                               // bEndpointAddress (EP1 in)
    0x01,                               // bmAttributes (asynchronous)

#ifdef EnableMIC48K
    //0x40, 0x02,                         // wMaxPacketSize (596)
    0x20, 0x01,                         // wMaxPacketSize (288)
    //0x90, 0x00,                       // wMaxPacketSize (144)
#else
    0x60, 0x00,                         // wMaxPacketSize (96)
#endif

    //0x05,                               // bInterval (2000 us) ( 2^Intval-1 ) * 125us
    0x04,                               // bInterval (2000 us) ( 2^Intval-1 ) * 125us
    //0x03,                               // bInterval (500 us) ( 2^Intval-1 ) * 125us
    0x00,                               // bRefresh (0)
    0x00,                               // bSynchAddress (no synchronization)

#ifdef EnableHID
// HID

#endif
};

U32 DRV_USB_Audio_GetDataLen()
{
    if(g_USBAudioBuff[g_readflg].Status == 2 && g_start == 1)
        return USB_AUDIO_BUFFER_SIZE;
    else
    {
        return 0;
    }
}

U32 DRV_USB_Audio_Get(U8 *pBuffer, U32 RequestLen)
{
    //U32 DataLen = 0;

	UNUSED(RequestLen);

    if(g_USBAudioBuff[g_readflg].Status == 2 && g_start == 1)
    {
        g_USBAudioBuff[g_readflg].Status = 3;
        memcpy(pBuffer, g_USBAudioBuff[g_readflg].pu8AudioBuff, USB_AUDIO_BUFFER_SIZE);
        g_USBAudioBuff[g_readflg].Status= 0;
        if(g_readflg == (USB_AUDIO_BUFFER_NUM - 1))
            g_readflg = 0;
        else
            g_readflg += 1;
    }

    return 0;
}

U32 DRV_USB_AudioSendDataLen()
{
    if(g_u32Ready == 0)
        return 0;

    return 0;
}

U32 DRV_USB_Audio_Send(U8 *pBuffer, U32 DataLen)
{
    //if(g_u32Ready != 0  || g_u32InitFinish < 1 || DataLen == 0)
    //    return 0;


    if(g_u32InitFinish < 1 || DataLen == 0)
          return 0;

    if(DataLen > USB_AUDIO_BUFFER_SIZE)
        DRV_USB_EP_VDMAWrite3(1, pBuffer, USB_AUDIO_BUFFER_SIZE, 0);
    else
        DRV_USB_EP_VDMAWrite3(1, pBuffer, DataLen, 0);

    return DataLen;
}

VOID DRV_USB_AUDIO_SendDeviceDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_AUDIO_HSDeviceDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_AUDIO_HSDeviceDescriptor, dataLen);
}

VOID DRV_USB_AUDIO_SendConfigureDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_AUDIO_HSConfigOTGDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_AUDIO_HSConfigOTGDescriptor, dataLen);
}

BOOL DRV_USB_AUDIO_OTGdescriptor(void)
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();
    U32 RequestLen = 0;
    U16 mySize = 0;
    U8  *pData;
    switch ((U8)(pSetupPacket->Value >> 8))
    {
        case 1:
            {
                DRV_USB_AUDIO_SendDeviceDescription(pSetupPacket->Length);
            }
            break;
        case 2:
            switch ((U8)pSetupPacket->Value)
            {
                case 0x00:
                        mySize = sizeof(u8USB_AUDIO_HSConfigOTGDescriptor);
                        u8USB_AUDIO_HSConfigOTGDescriptor[2] = mySize;
                        u8USB_AUDIO_HSConfigOTGDescriptor[3] = (mySize >> 8);
                        DRV_USB_AUDIO_SendConfigureDescription(pSetupPacket->Length);
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
                       pData = Audio_Manufacturer;
                       RequestLen =  Audio_Manufacturer[0];
                    break;
                case 0x02:
                       pData = Audio_Production;
                       RequestLen =  Audio_Production[0];
                    break;
                case 0x03:
                       pData = Audio_SerialNumber;
                       RequestLen = Audio_SerialNumber[0];
                    break;
                default:
                    return FALSE;
            }
            if (RequestLen > pSetupPacket->Length)
                RequestLen = pSetupPacket->Length;

            DRV_USB_CX_VDMAWrite2(pData, RequestLen);
            break;
        case 4:
            DRV_USB_CX_DONE();
            break;
        // Interface descriptor
        case 5:
            DRV_USB_CX_DONE();
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

//U8 QTemp[288*2];

BOOL DRV_USB_AUDIO_StandardCommand()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch (pSetupPacket->Request)
    {
        case 6:
                // get descriptor
                DRV_USB_AUDIO_OTGdescriptor();
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
                g_u32InitFinish = 2;
                DRV_USB_SetConfigureDone();
                    // DRV_USB_EP_Configure_Len(1, USB_AUDIO_BUFFER_SIZE);
                    // DRV_USB_EP_Configure_Len(3, USB_AUDIO_BUFFER_SIZE);
                    // QTemp[0] = 0x55;
                    // QTemp[288] = 0xAA;
                    // DRV_USB_EP_VDMAWrite2(1, QTemp, USB_AUDIO_BUFFER_SIZE*2, 0);
                //DRV_USB_CX_DONE();
                return TRUE;
        case 10:
                // get interface
                DRV_USB_CX_DONE();
                return TRUE;
        case 11:
                // set interface
                DRV_USB_CX_DONE();
                    //DRV_USB_EP_IN_ENABLE(1);
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

VOID DRV_USB_AUDIO_ClassCmd()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch(pSetupPacket->Request)
    {
        // case 0x01:
        //     {// Set Cur
        //         //william c
        //         U8 Temp[64];
        //         if(pSetupPacket->Length > 0)
        //         {
        //             DRV_USB_CX_VDMARead2(Temp, pSetupPacket->Length);
        //         }
        //     }
        //     break;
        case 0x81:
            {
                if(pSetupPacket->Value == 0x0100)
                {
                    U8 Temp[] = {0x00};
                    DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
                }
                else if(pSetupPacket->Value == 0x0201)
                {
                    U8 Temp[] = {0x00, 0xEC};
                    DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
                }
                else if(pSetupPacket->Value == 0x0202)
                {
                    U8 Temp[] = {0x00, 0xEC};
                    DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
                }
            }
            break;
        case 0x82:
            {// Get Min
                U8 Temp[] = {0x00, 0xC4};
                DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
            }
            break;
        case 0x83:
            {// Get Max
                U8 Temp[] = {0x00, 0x00};
                DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
            }
            break;
        case 0x84:
            {// Get Res
                U8 Temp[] = {0x00, 0x01};
                DRV_USB_CX_VDMAWrite2(Temp, sizeof(Temp));
            }
            break;
        default:
            {
                DRV_USB_CX_DONE();
            }
    }
}

VOID DRV_USB_AUDIO_CX_OUT(U32 DataLen)
{
	UNUSED(DataLen);

}

VOID DRV_USB_AUDIO_CX_TRANS_CMPL(void)
{
    DRV_USB_CX_DONE();
}

VOID DRV_USB_AUDIO_WRITECMPL(void)
{
    g_u32Ready = 0;
}

U32 DRV_USB_AUDIO_READ(U32 DataLen)
{
    if(DataLen == 0 || g_u32InitFinish < 1 )
    {
        return 1;
    }

    if( g_USBAudioBuff[g_writeflg].Status == 0 )
    {
        g_USBAudioBuff[g_writeflg].Status = 1;
        DRV_USB_EP_VDMAReadBlocking(3, g_USBAudioBuff[g_writeflg].pu8AudioBuff, DataLen);
        g_USBAudioBuff[g_writeflg].Status = 2;
        if( g_writeflg == (USB_AUDIO_BUFFER_NUM - 1))
        {
            g_writeflg = 0;
            g_start = 1;
        }
        else
            g_writeflg += 1;
    }

    return 0;
}

U32 DRV_USB_AUDIO_MSG_Handler(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(handle);
    UNUSED(pData);

    switch(MsgId)
    {
        case DRV_USB_STANDARD_CMD:          { DRV_USB_AUDIO_StandardCommand();}break;
        case DRV_USB_CLASS_CMD:             { DRV_USB_AUDIO_ClassCmd();}break;

        case DRV_USB_MSG_CX_OUT:            { DRV_USB_AUDIO_CX_OUT(id_ext); } break;
        case DRV_USB_MSG_CX_TRANS_CMPL:     { DRV_USB_AUDIO_CX_TRANS_CMPL(); } break;

        case DRV_USB_MSG_EP1_TRANS_CMPL:    { DRV_USB_AUDIO_WRITECMPL(); } break;
        case DRV_USB_MSG_EP3_OUT :
            {
                DRV_USB_AUDIO_READ(id_ext);
                DRV_USB_EP_DONE(3);
            }
            break;
        default:
            break;
    }

    return 0;
}
HandlerData  g_AudioClassMSG  = {DRV_USB_AUDIO_MSG_Handler};

VOID DRV_USB_Audio_Release()
{
    for( int x = 0; x < USB_AUDIO_BUFFER_NUM; x++)
    {
        g_USBAudioBuff[x].Status = 0;
        free(g_USBAudioBuff[x].pu8AudioBuff);
    }
}

VOID DRV_USB_Audio_Init()
{
    if(g_u32InitFinish > 0)
        return;

    //DRV_USB_InitV2();
    DRV_USB_InitFullSpeed();

    for( int x = 0; x < USB_AUDIO_BUFFER_NUM; x++)
    {
        g_USBAudioBuff[x].Status = 0;
        g_USBAudioBuff[x].pu8AudioBuff = (U8 *)malloc(USB_AUDIO_BUFFER_SIZE);
    }

    DRV_USB_Configure2Ex_EP1(EP_TYPE_ISO, EP_DIRECTION_IN);
    DRV_USB_Configure2Ex_EP3(EP_TYPE_ISO, EP_DIRECTION_OUT);

    DRV_USB_EP_Configure_Len(1, 288);
    DRV_USB_EP_Configure_Len(3, 288);

    g_u32InitFinish = 1;

    MSG_MessageUsbHandler(&g_AudioClassMSG);
}
#endif

