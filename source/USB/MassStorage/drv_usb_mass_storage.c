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
#include "types.h"
#include "drv_usb.h"
#include "drv_sdc.h"
#include "drv_usb_mass_storage.h"
#include "message.h"
#include "message_system.h"
#include "stdlib.h"
#include "types.h"
#include "drv_charger.h"
#include "dvfs.h"
#include "log.h"
#include "nvkey.h"
#include "nvkey_list.h"


#define USB_MSS_VENDOR_ID                   0x7777
#define USB_MSS_PRODUCT_ID                  0x6662
#define USB_MSS_RELEASE_ID                  0x0100

#define EMMC_SDC_INIT_RDY_TIME_MS           50
#define EMMC_SDC_INIT_CHK_RETRY_TIMES       20
#define EMMC_SDC_REINIT_DELAY_TIME_MS       400
#define EMMC_SDC_REINIT_TIMES               2

static U8 MSS_Manufacturer[] =
{
    14,
    0x03,
    'A', 0x00, 'i', 0x00, 'r', 0x00, 'o', 0x00, 'h', 0x00, 'a', 0x00
};

static U8 MSS_Production[] =
{
    24,
    0x03,
    'A', 0x00, 'i', 0x00, 'r', 0x00, 'o', 0x00, 'h', 0x00, 'a', 0x00, '-', 0x00, 'D', 0x00, 'i', 0x00, 's', 0x00, 'k', 0x00
};

static U8 MSS_SerialNumber[] =
{
    10,
    0x03,
    '1', 0x00, '1', 0x00, '0', 0x00, '0', 0x00
};

static U8 Disk_Info[] =
{
    // Device Type
    0x00,
    // Removeable Meida
    0x80,
    // ANSI / ECMA / ISO Version
    0x02,
    // Response Data Format
    0x02,
    // Additional Length
    0x1F,
    // Dummy
    0x00, 0x00, 0x00,
    // Vendor Information ( Always 8 bytes no way to change SCSI spec)
    'A', 'i', 'r', 'o', 'h', 'a', ' ', ' ',
    // Product Identification ( Always 16 bytes no way to change SCSI spec)
    0x32, 0x30, 0x30, 0x33, 0x2D, 0x31, 0x30, 0x2D, 0x30, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    // Product Revision (Always 4 bytes no way to change SCSI spec)
    0x32, 0x2E, 0x30, 0x30
};

//static U8 ILL_REQ[]
//    = { 0x70, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0xC0, 0x00, 0x07 };
static U8 MEDIA_NOT_PRESENT[]
    = { 0xF0, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0xAA, 0x55, 0x40, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00 };
static U8 MEDIA_PRESENT[]
    = { 0xF0, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0xAA, 0x55, 0x41, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00 };
//static U8 MEDIA_NOT_READY_TO_TRANSIT[]
//    = { 0xF0, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0xAA, 0x55, 0x41, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00 };

// We must prepare two speed descriptor for backward compatible. HS & FS
// High speed Configuration
U8 u8USB_MSS_HSDeviceDescriptor[] =
{
    0x12,                  // bLength
    DT_DEVICE,                      // bDescriptorType
    0x00,                           // bcdUSB
    0x02,
    USB_CLASS_CODE_INTERFACE,       // bDeviceClass
    USB_MASS_SUB_CLASS_SCSI,        // bDeviceSubClass
    USB_MASS_PROTOCOL_CBI_NOCOMMAND,// bDeviceProtocol
    0x40,                           // bMaxPacketSize0
    (USB_MSS_VENDOR_ID & 0xFF),     /* 8 idVendor */
    (USB_MSS_VENDOR_ID >> 8),       /* 9 idVendor */
    (USB_MSS_PRODUCT_ID & 0xFF),    /* 10 idProduct */
    (USB_MSS_PRODUCT_ID >> 8),      /* 11 idProduct */
    (USB_MSS_RELEASE_ID & 0xFF),    /* 12 bcdDevice */
    (USB_MSS_RELEASE_ID >> 8),      /* 13 bcdDevice */
    0x01,                           // iManufacturer
    0x02,                           // iProduct
    0x03,                           // iSerialNumber
    0x01,                           // bNumConfigurations
};

// FUll speed Configuration
U8 u8USB_MSS_FSDeviceDescriptor[] =
{
    0x12,                           // bLength
    DT_DEVICE,                      // bDescriptorType
    0x00,                           // bcdUSB
    0x02,
    USB_CLASS_CODE_INTERFACE,       // bDeviceClass
    USB_MASS_SUB_CLASS_SCSI,        // bDeviceSubClass
    USB_MASS_PROTOCOL_CBI_NOCOMMAND,// bDeviceProtocol
    0x40,                           // bMaxPacketSize0
    (USB_MSS_VENDOR_ID & 0xFF),     /* 8 idVendor */
    (USB_MSS_VENDOR_ID >> 8),       /* 9 idVendor */
    (USB_MSS_PRODUCT_ID & 0xFF),    /* 10 idProduct */
    (USB_MSS_PRODUCT_ID >> 8),      /* 11 idProduct */
    (USB_MSS_RELEASE_ID & 0xFF),    /* 12 bcdDevice */
    (USB_MSS_RELEASE_ID >> 8),      /* 13 bcdDevice */
    0x01,                           // iManufacturer
    0x02,                           // iProduct
    0x03,                           // iSerialNumber
    0x01,                           // bNumConfigurations
};

U8 u8USB_MSS_HSConfigOTGDescriptor01[] =
{
    //  CONFIGURATION descriptor
    0x09,                  // bLength
    DT_CONFIGURATION,               // bDescriptorType CONFIGURATION
    0x23,
    0x00,
    0x01,
    0x01,
    0x00,
    0xC0,
    0x00,

    // Alternate Setting 0
    0x09,
    DT_INTERFACE,
    0x00,
    0x00,
    0x02,
    0x08,
    0x06,
    0x50,
    0x00,

    // EP1
    0x07,
    DT_ENDPOINT,
    0x81,
    0x02,
    0x00,
    0x02,
    0x01,

    // EP2
    0x07,
    DT_ENDPOINT,
    0x03,
    0x02,
    0x00,
    0x02,
    0x01,

    0x03,
    0x09,
    0x00,
};

U8 u8USB_MSS_FSConfigOTGDescriptor01[] =
{
    0x09,
    DT_CONFIGURATION,               // bDescriptorType CONFIGURATION
    0x23,
    0x00,
    0x01,
    0x01,
    0x00,
    0xC0,
    0x00,

    // Alternate Setting 0
    0x09,
    DT_INTERFACE,
    0x00,
    0x00,
    0x02,
    0x08,
    0x06,
    0x50,
    0x00,

    // EP1
    0x07,
    DT_ENDPOINT,
    0x81,
    0x02,
    0x00,
    0x02,
    0x01,

    // EP2
    0x07,
    DT_ENDPOINT,
    0x03,
    0x02,
    0x00,
    0x02,
    0x01,

    0x03,
    0x09,
    0x00,
};

typedef struct
{
    U32 Offset_Blk;
    U32 NumBlks_for_transfer;
    U32 NumBlks_Left_SD;
    U32 NumBlks_Left_USB;
}TransferInfo, *lpTransferInfo;
TransferInfo g_TransferInfo;

#define CBW_SIGNATE                             0x43425355
#define CSW_SIGNATE                             0x53425355
#define CSW_STATUS_CMD_PASS                     0x00
#define CSW_STATUS_CMD_FAIL                     0x01
#define CSW_STATUS_PHASE_ERROR                  0x02

// SCSI Group Code
#define SCSI_Group_0                            0x00
#define SCSI_Group_1                            0x20
#define SCSI_Group_2                            0x40
#define SCSI_Group_4                            0x80
#define SCSI_Group_5                            0xA0
// --------------- Group 0 (000xxxxx) - six-byte commands ----------------
#define SCSI_CMD_TEST_UNIT_READY                0x00
#define SCSI_CMD_REZERO_UNIT                    0x01
#define SCSI_CMD_REQUEST_SENSE                  0x03
#define SCSI_CMD_FORMAT_UNIT                    0x04
#define SCSI_CMD_READ_BLOCK_LIMITS              0x05
#define SCSI_CMD_REASSIGN_BLOCKS                0x07
#define SCSI_CMD_READ6                          0x08
#define SCSI_CMD_WRITE6                         0x0a
#define SCSI_CMD_SEEK6                          0x0b
#define SCSI_CMD_READ_REVERSE                   0x0f
#define SCSI_CMD_WRITE_FILEMARKS                0x10
#define SCSI_CMD_SPACE                          0x11
#define SCSI_CMD_INQUIRY                        0x12
#define SCSI_CMD_VERIFY6                        0x13
#define SCSI_CMD_RECOVER_BUFFERED_DATA          0x14
#define SCSI_CMD_MODE_SELECT6                   0x15
#define SCSI_CMD_RESERVE                        0x16
#define SCSI_CMD_RELEASE                        0x17
#define SCSI_CMD_COPY                           0x18
#define SCSI_CMD_MODE_SENSE6                    0x1a
#define SCSI_CMD_START_STOP_UNIT                0x1b
#define SCSI_CMD_RECEIVE_DIAGNOSTIC_RESULTS     0x1c
#define SCSI_CMD_SEND_DIAGNOSTIC                0x1d
#define SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL   0x1e

// ---------------- Group 1 (001xxxxx) - ten-byte commands ----------------
#define SCSI_CMD_READ_FORMAT_CAPACITIES         0x23
#define SCSI_CMD_SET_WINDOW                     0x24
#define SCSI_CMD_READ_CAPACITY                  0x25
#define SCSI_CMD_READ10                         0x28
#define SCSI_CMD_READ_GENERATION                0x29
#define SCSI_CMD_WRITE10                        0x2a
#define SCSI_CMD_SEEK10                         0x2b
#define SCSI_CMD_ERASE10                        0x2c
#define SCSI_CMD_READ_UPDATED_BLOCK             0x2d
#define SCSI_CMD_WRITE_AND_VERIFY10             0x2e
#define SCSI_CMD_VERIFY10                       0x2f
#define SCSI_CMD_SEARCH_DATA_HIGH10             0x30
#define SCSI_CMD_SEARCH_DATA_EQUAL10            0x31
#define SCSI_CMD_SEARCH_DATA_LOW10              0x32
#define SCSI_CMD_SET_LIMITS10                   0x33
#define SCSI_CMD_PREFETCH                       0x34
#define SCSI_CMD_SYNCHRONIZE_CACHE              0x35
#define SCSI_CMD_LOCK_UNLOCK_CACHE              0x36
#define SCSI_CMD_READ_DEFECT_DATA10             0x37
#define SCSI_CMD_MEDIUM_SCAN                    0x38
#define SCSI_CMD_COMPARE                        0x39
#define SCSI_CMD_COPY_AND_VERIFY                0x3a
#define SCSI_CMD_WRITE_BUFFER                   0x3b
#define SCSI_CMD_READ_BUFFER                    0x3c
#define SCSI_CMD_UPDATE_BLOCK                   0x3d
#define SCSI_CMD_READ_LONG                      0x3e
#define SCSI_CMD_WRITE_LONG                     0x3f

// ---------------- Group 2 (010xxxxx) - ten-byte commands ----------------
#define SCSI_CMD_CHANGE_DEFINITION              0x40
#define SCSI_CMD_WRITE_SAME                     0x41
#define SCSI_CMD_READ_SUBCHANNEL                0x42
#define SCSI_CMD_READ_TOC                       0x43
#define SCSI_CMD_READ_HEADER                    0x44
#define SCSI_CMD_PLAY_AUDIO10                   0x45
#define SCSI_CMD_PLAY_AUDIO_MSF                 0x47
#define SCSI_CMD_PLAY_AUDIO_TRACK_INDEX         0x48
#define SCSI_CMD_PLAY_TRACK_RELATIVE            0x49
#define SCSI_CMD_PAUSE_RESUME                   0x4b
#define SCSI_CMD_LOG_SELECT                     0x4c
#define SCSI_CMD_LOG_SENSE                      0x4d
#define SCSI_CMD_MODE_SELECT10                  0x55
#define SCSI_CMD_MODE_SENSE10                   0x5a

// ---------------- Group 5 (101xxxxx) - twelve-byte commands ----------------
#define SCSI_CMD_READ12                         0xa8
#define SCSI_CMD_WRITE12                        0xaa

// Control byte of CDB
#define SCSI_CDB_CONTROL_LINK                   0x01
#define SCSI_CDB_CONTROL_FLAG                   0x02

typedef enum
{
    STATE_KEEPIDLE,
    STATE_CBW,
    STATE_CB_DATA_IN,
    STATE_CB_DATA_OUT,
    STATE_SDC_READ,
    STATE_SDC_WRITE,
    STATE_CSW,
    STATE_WAIT_CSW_END,
} MassStorageState;

typedef struct CommandBlockWrapper
{
    U32 u32Signature;
    U32 u32Tag;
    U32 u32DataTransferLength;
    U8  u8Flags;
    U8  u8LUN;
    U8  u8CBLength;
    U8  u8CB[16];
} CBW, *LPCBW;

typedef struct CommandStatusWrapper
{
    U32 u32Signature;
    U32 u32Tag;
    U32 u32DataResidue;
    U8  u8Status;
} CWS, *LPCWS;

LPCBW  g_pCBW;
CWS    g_CSW;


typedef struct
{
    U8  *pu8MSSBuff;
    U32 Status;
    U32 DataLen;
}USB_MSS_BUFF_T, *LPUSB_MSS_BUFF_T;
USB_MSS_BUFF_T g_USBMSSBuff[USB_BUFFER_NUM];

enum {
    DoCmd = 0,
    DoRead = 1,
    DoWrite = 2,
    DoCmdEnd = 3,
};

U32 g_MSSProcess = DoCmd;
U32 g_USBRWFlag = 0;
U32 g_SDRWFlag = 0;
U8 gEMMC_SDC_Init_Status = EMMC_SDC_INIT_NONE;
U8 gMP3_Play_Status = MP3_PLAY_NONE;
U16 gUSB_Buffer_Size = 0;
U16 gUSB_Buffer_Sect = 0;


VOID DRV_USB_MSS_SendCSW(U8 EPNo, U8 *pData)
{
    DRV_USB_EP_DONE(3);
    DRV_USB_EP_VDMAWrite2(EPNo, pData, 13, 0);
}

VOID DRV_MSS_USBCXFromHost()
{

}

VOID DRV_MSS_USBCXFromHostCmpl()
{

}

VOID DRV_MSS_USBToHost()
{

}

VOID DRV_MSS_USBToHostCmpl()
{

}

VOID DRV_MSS_USBFromHost()
{

}

VOID DRV_MSS_USBFromHostCmpl()
{

}


VOID DRV_USB_MSS_SendDeviceDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_MSS_HSDeviceDescriptor);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_MSS_HSDeviceDescriptor, dataLen);
    //DRV_USB_CX_VDMAWriteBlocking(u8USB_MSS_HSDeviceDescriptor, dataLen);
}

VOID DRV_USB_MSS_SendConfigureDescription(U32 RequestLen)
{
    U32 dataLen = 0;

    dataLen = sizeof(u8USB_MSS_HSConfigOTGDescriptor01);
    if(dataLen > RequestLen)
        dataLen = RequestLen;

    DRV_USB_CX_VDMAWrite2(u8USB_MSS_HSConfigOTGDescriptor01, dataLen);
    //DRV_USB_CX_VDMAWriteBlocking(u8USB_MSS_HSConfigOTGDescriptor01, dataLen);
}

BOOL DRV_USB_MSS_OTGdescriptor(void)
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();
    U32 RequestLen = 0;
    U8  *pData;
    switch ((U8)(pSetupPacket->Value >> 8))
    {
        case USB_DEVICE_DESCRIPTOR:
            DRV_USB_MSS_SendDeviceDescription(pSetupPacket->Length);
            break;
        case USB_CONFIGURATION_DESCRIPTOR:
            DRV_USB_MSS_SendConfigureDescription(pSetupPacket->Length);
            break;
        case USB_STRING_DESCRIPTOR:
            switch ((U8)pSetupPacket->Value)
            {
                case 0x00:
                       DRV_USB_SetLanguage(pSetupPacket->Length);
                    return TRUE;
                case 0x01:
                       pData = MSS_Manufacturer;
                       RequestLen =  MSS_Manufacturer[0];
                    break;
                case 0x02:
                       pData = MSS_Production;
                       RequestLen =  MSS_Production[0];
                    break;
                case 0x03:
                       pData = MSS_SerialNumber;
                       RequestLen = MSS_SerialNumber[0];
                    break;
                default:
                    return FALSE;
            }
            if (RequestLen > pSetupPacket->Length)
                RequestLen = pSetupPacket->Length;

            DRV_USB_CX_VDMAWrite2(pData, RequestLen);
            //DRV_USB_CX_VDMAWriteBlocking(pData, RequestLen);
            break;
        case USB_INTERFACE_DESCRIPTOR:
            break;
        case USB_ENDPOINT_DESCRIPTOR:
            break;
        case USB_DEVICE_QUALIFIER_DESCRIPTOR:
            {
                // If Current is HS. Then return FS
                // If Current is FS. Then return HS
                DRV_USB_CX_DONE();
            }
            break;
        case USB_OTHER_SPEED_DESCRIPTOR:
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

BOOL DRV_USB_MSS_StandardCommand()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch (pSetupPacket->Request)
    {
        case USB_STANDARD_REQUEST_GET_DESCRIPTOR:
            DRV_USB_MSS_OTGdescriptor();
            return TRUE;
        // We just have 1 descriptor do nothing
        case USB_STANDARD_REQUEST_SET_DESCRIPTOR:
            DRV_USB_CX_DONE();
            return TRUE;
        case USB_STANDARD_REQUEST_GET_CONFIGURATION:
            DRV_USB_CX_DONE();
            return TRUE;
        case USB_STANDARD_REQUEST_SET_CONFIGURATION:
            DRV_USB_SetConfigureDone();
            g_MSSProcess = DoCmd;
            return TRUE;
        case USB_STANDARD_REQUEST_SET_GET_INTERFACE:
        case USB_STANDARD_REQUEST_SET_INTERFACE:
        case USB_STANDARD_REQUEST_SYNCH_FRAME:
            DRV_USB_CX_DONE();
            return TRUE;
        default:
            DRV_USB_CX_DONE();
            break;
    }
    return FALSE;
}

VOID DRV_USB_MSS_ClassCmd()
{
    lpSetupPacket pSetupPacket = DRV_USB_GetSetupPacket();

    switch(pSetupPacket->Request)
    {
        case USB_MASS_REQUEST_GET_MAX_LUN:
            {
                U8 LUN = 0;
                DRV_USB_CX_VDMAWrite2(&LUN, 1);
            }
        default:
            {
                DRV_USB_CX_DONE();
            }
    }
}

static inline VOID DRV_USB_MSS_ProcessCDBGr0(U8 *pData)
{
    U8 Cmd = pData[0];

    switch(Cmd)
    {
        case SCSI_CMD_TEST_UNIT_READY:
        {
            if( DRV_SDC_GetCurStatus() < 2 || f_Eject)
            //if( DRV_SDC_GetCurStatus() < 2 )
            {
                g_MSSProcess = DoCmd;
                g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
                DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
            }
            else
            {
                g_MSSProcess = DoCmd;
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
            }
        }
        break;
        case SCSI_CMD_INQUIRY:
        {
            g_MSSProcess = DoCmdEnd;
            g_CSW.u8Status = CSW_STATUS_CMD_PASS;
            DRV_USB_EP_VDMAWrite2(1, Disk_Info, sizeof(Disk_Info), 0);
        }
        break;
        case SCSI_CMD_MODE_SENSE6:
        {
            if( DRV_SDC_GetCurStatus() < 2 || f_Eject)
            {
                U8 Temp[4] = {0x03, 0x00, 0x00, 0x00};
                g_MSSProcess = DoCmdEnd;
                g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
                DRV_USB_EP_VDMAWrite2(1, Temp, sizeof(Temp), 0);
            }
            else
            {
                U8 Temp[4] = {0x03, 0x00, 0x00, 0x00};
                g_MSSProcess = DoCmdEnd;
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                DRV_USB_EP_VDMAWrite2(1, Temp, sizeof(Temp), 0);
            }

        }
        break;
        case SCSI_CMD_START_STOP_UNIT:
        {
            if(pData[4]& 0x02) //LOEJ
            {
                // eject happened.........
                 f_Eject = 1;
            }

            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
        case SCSI_CMD_REQUEST_SENSE:
        {
            if( DRV_SDC_GetCurStatus() < 2 || f_Eject)
            {
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                g_MSSProcess = DoCmdEnd;
                DRV_USB_EP_VDMAWrite2(1, MEDIA_NOT_PRESENT, sizeof(MEDIA_NOT_PRESENT), 0);
            }
            else
            {
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                g_MSSProcess = DoCmdEnd;
                DRV_USB_EP_VDMAWrite2(1, MEDIA_PRESENT, sizeof(MEDIA_PRESENT), 0);
            }
        }
        break;
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        {
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
        default:
        {
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
    }
}

static inline VOID DRV_USB_MSS_ProcessCDBGr1(U8 *pData)
{
    U8 Cmd = pData[0];

    switch(Cmd)
    { //
        case SCSI_CMD_READ_CAPACITY:
        {
            if( DRV_SDC_GetCurStatus() < 2 )
            {
                U8 Capacity[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};
                g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
                g_MSSProcess = DoCmdEnd;
                DRV_USB_EP_VDMAWrite2(1, Capacity, sizeof(Capacity), 0);
            }
            else
            {
                U8 Capacity[] = {0x22, 0x00, 0x22, 0x00, 0x00, 0x00, 0x02, 0x00};
                U32 Size = DRV_SDC_GetCapacity() - 3;
                U8 *p = (U8 *)&Size;
                Capacity[0] = p[3];
                Capacity[1] = p[2];
                Capacity[2] = p[1];
                Capacity[3] = p[0];
                g_CSW.u8Status = CSW_STATUS_CMD_PASS;
                g_MSSProcess = DoCmdEnd;
                DRV_USB_EP_VDMAWrite2(1, Capacity, sizeof(Capacity), 0);
            }

        }
        break;
        case SCSI_CMD_READ10:
        {
            if( DRV_SDC_GetCurStatus() < 2 )
            {
                g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
                DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
            }
            else
            {   // Left
                // Caculate the
                g_TransferInfo.Offset_Blk           = pData[2] << 24 | pData[3] << 16 | pData[4] << 8| pData[5];
                g_TransferInfo.NumBlks_for_transfer = pData[7] << 8 | pData[8];
                // Start Transfer here
                g_MSSProcess = DoRead;
                int x = 0;
                for(x = 0; x < USB_BUFFER_NUM; x++)
                {
                    g_USBMSSBuff[x].Status = 0;
                    g_USBMSSBuff[x].DataLen = 0;
                }
                g_SDRWFlag = 0;
                g_USBRWFlag = 0;
                g_TransferInfo.NumBlks_Left_USB = g_TransferInfo.NumBlks_for_transfer;
                if(g_TransferInfo.NumBlks_for_transfer >= gUSB_Buffer_Sect)
                {
                    g_USBMSSBuff[g_SDRWFlag].Status = 1;
                    g_USBMSSBuff[g_SDRWFlag].DataLen = gUSB_Buffer_Sect;
                    g_TransferInfo.NumBlks_Left_SD = g_TransferInfo.NumBlks_for_transfer - gUSB_Buffer_Sect;
                    U32 OffSet = g_TransferInfo.Offset_Blk;
                    g_TransferInfo.Offset_Blk += gUSB_Buffer_Sect;
                    DRV_SDC_Read(OffSet, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, gUSB_Buffer_Size);
                }
                else
                {
                    g_USBMSSBuff[g_SDRWFlag].Status = 1;
                    g_USBMSSBuff[g_SDRWFlag].DataLen = g_TransferInfo.NumBlks_for_transfer;
                    g_TransferInfo.NumBlks_Left_SD = 0;
                    DRV_SDC_Read(g_TransferInfo.Offset_Blk, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, 512 * g_TransferInfo.NumBlks_for_transfer);
                }
                MSG_MessageSend(gMSG_HandlerRegisteredForUSB, DRV_USB_MSG_EP1_IN, NULL);
            }
        }
        break;
        case SCSI_CMD_WRITE10:
        {//
            if( DRV_SDC_GetCurStatus() < 2 )
            {
                g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
                DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
            }
            else
            {
                // DRV_GPIO_SetIOStatus(17, 1, 1);
                // DRV_GPIO_SetIOStatus(17, 1, 0);
                g_MSSProcess = DoWrite;
                g_TransferInfo.Offset_Blk           = pData[2] << 24 | pData[3] << 16 | pData[4] << 8| pData[5];
                g_TransferInfo.NumBlks_for_transfer = pData[7] << 8 | pData[8];
                int x = 0;
                for(x = 0; x < USB_BUFFER_NUM; x++)
                {
                    g_USBMSSBuff[x].Status = 0;
                    g_USBMSSBuff[x].DataLen = 0;
                }
                g_SDRWFlag = 0;
                g_USBRWFlag = 0;
                g_TransferInfo.NumBlks_Left_USB = g_TransferInfo.NumBlks_for_transfer;
                g_TransferInfo.NumBlks_Left_SD = g_TransferInfo.NumBlks_for_transfer;
                DRV_USB_EP_DONE(3);
                MSG_MessageSend(gMSG_HandlerRegisteredForSDC, DRV_SDC_MSG_TRANS_CMPL, NULL);
            }
        }
        break;
        case SCSI_CMD_READ_FORMAT_CAPACITIES:
        {
            U8 Temp[4];
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            g_MSSProcess = DoCmdEnd;
            DRV_USB_EP_VDMAWrite2(1, Temp, sizeof(Temp), 0);
        }
        break;
        default:
        {
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
    }
}

static inline VOID DRV_USB_MSS_ProcessCDBGr2(U8 *pData)
{
    U8 Cmd = pData[0];

    switch(Cmd)
    { //
        case SCSI_CMD_MODE_SENSE10:
        {
            U8 Temp[4];
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_EP_VDMAWrite2(1, Temp, sizeof(Temp), 0);
        }
        break;
        default:
        {
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
    }
}

static inline VOID DRV_USB_MSS_ProcessCDBGr4(U8 *pData)
{
    U8 Cmd = pData[0];
    switch(Cmd)
    { //
        default:
        {
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
    }
}

static inline VOID DRV_USB_MSS_ProcessCDBGr5(U8 *pData)
{
    U8 Cmd = pData[0];
    switch(Cmd)
    { //
        default:
        {
            g_CSW.u8Status = CSW_STATUS_CMD_FAIL;
            DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
        }
        break;
    }
}

U8 DRV_USB_MSS_ParsingSCSICmd(U8 *pData)
{
    U8 CDBGr = pData[0] & 0xE0;

    if(CDBGr == SCSI_Group_0)
    {
        DRV_USB_MSS_ProcessCDBGr0(pData);
    }
    else if(CDBGr == SCSI_Group_1)
    {
        DRV_USB_MSS_ProcessCDBGr1(pData);
    }
    else if(CDBGr == SCSI_Group_2)
    {
        DRV_USB_MSS_ProcessCDBGr2(pData);
    }
    else if(CDBGr == SCSI_Group_4)
    {
        DRV_USB_MSS_ProcessCDBGr4(pData);
    }
    else if(CDBGr == SCSI_Group_5)
    {
        DRV_USB_MSS_ProcessCDBGr5(pData);
    }

    return 0;
}


U8 DRV_USB_MSS_ParsingCBW(U8 *pData)
{
    g_pCBW = (LPCBW)pData;

    // Parsing the command and prepare the data.
    if(g_pCBW->u32Signature != CBW_SIGNATE)
    {
        return -1;
    }
    else
    {
        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // Step 1. Fill CSW for later use.
        g_CSW.u32Signature = CSW_SIGNATE;
        // pass u32DataTransferLength to u32DataResidue
        g_CSW.u32DataResidue = 0;//g_pCBW->u32DataTransferLength;
        // pass Tag from CBW to CSW
        g_CSW.u32Tag = g_pCBW->u32Tag;
        // Assume Status is CMD_PASS
        g_CSW.u8Status = CSW_STATUS_CMD_PASS;

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        // Step 2. Fill CSW for later use.
        if (g_pCBW->u32DataTransferLength == 0)
        {// Send CSW

            if (g_pCBW->u8Flags == 0x00)
            {// Host to Device & Send CSW
                DRV_USB_MSS_ParsingSCSICmd(pData + 15);
            }
            else
            {// Device to Host & Send CSW
                DRV_USB_MSS_ParsingSCSICmd(pData + 15);
            }

        }
        else
        {
            if (g_pCBW->u8Flags == 0x00)
            {// Host to Device & Send CSW
                DRV_USB_MSS_ParsingSCSICmd(pData + 15);
            }
            else
            {// Device to Host & Send CSW
                DRV_USB_MSS_ParsingSCSICmd(pData + 15);
            }
        }
    }

    return 0;
}

static inline VOID DRV_USB_MSS_DoPPToUSB1(Handler handle)
{
     UNUSED(handle);

     if(g_USBMSSBuff[g_USBRWFlag].Status == 2)
     {
        g_USBMSSBuff[g_USBRWFlag].Status = 3;
        DRV_USB_EP_VDMAWrite2(1, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, g_USBMSSBuff[g_USBRWFlag].DataLen * 512, 0);
     }
}

static inline VOID DRV_USB_MSS_DoPPToUSB2(Handler handle)
{
     UNUSED(handle);

     if(g_USBMSSBuff[g_USBRWFlag].Status == 3)
     {
        g_TransferInfo.NumBlks_Left_USB -= g_USBMSSBuff[g_USBRWFlag].DataLen;
        g_USBMSSBuff[g_USBRWFlag].Status = 0;
        if(g_USBRWFlag == (USB_BUFFER_NUM - 1))
            g_USBRWFlag = 0;
        else
            g_USBRWFlag += 1;
     }

     if(g_TransferInfo.NumBlks_Left_USB == 0)
     {
        g_MSSProcess = DoCmd;
        DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
     }
}

static inline VOID DRV_USB_MSS_DoUSBToPP1(Handler handle)
{
    if(g_TransferInfo.NumBlks_Left_USB >= gUSB_Buffer_Sect)
    {
        if(g_USBMSSBuff[g_USBRWFlag].Status == 0)
        {
            g_USBMSSBuff[g_USBRWFlag].Status = 1;
            g_USBMSSBuff[g_USBRWFlag].DataLen = gUSB_Buffer_Sect;
            g_TransferInfo.NumBlks_Left_USB -= gUSB_Buffer_Sect;
            // DRV_GPIO_SetIOStatus(17, 1, 1);
            // DRV_GPIO_SetIOStatus(17, 1, 0);
            DRV_USB_EP_VDMARead2(3, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, gUSB_Buffer_Size);
            DRV_USB_EP_DONE(3);
        }
        else
        {
            MSG_MessageSend(handle, DRV_USB_MSG_EP3_OUT, NULL);
        }
    }
    else if( g_TransferInfo.NumBlks_Left_USB > 0 )
    {
        if(g_USBMSSBuff[g_USBRWFlag].Status == 0)
        {
            g_USBMSSBuff[g_USBRWFlag].Status = 1;
            g_USBMSSBuff[g_USBRWFlag].DataLen = g_TransferInfo.NumBlks_Left_USB;
            g_TransferInfo.NumBlks_Left_USB = 0;
            // DRV_GPIO_SetIOStatus(17, 1, 1);
            // DRV_GPIO_SetIOStatus(17, 1, 0);
            DRV_USB_EP_VDMARead2(3, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, g_USBMSSBuff[g_USBRWFlag].DataLen * 512);
        }
        else
        {
            MSG_MessageSend(handle, DRV_USB_MSG_EP3_OUT, NULL);
        }
    }
}

static inline VOID DRV_USB_MSS_DoUSBToPP2(Handler handle)
{
    if(g_USBMSSBuff[g_USBRWFlag].Status == 1)
    {
        g_USBMSSBuff[g_USBRWFlag].Status = 2;
        if(g_USBRWFlag == (USB_BUFFER_NUM - 1))
            g_USBRWFlag = 0;
        else
            g_USBRWFlag += 1;

        //MSG_MessageSend(handle, DRV_USB_MSG_EP2_OUT, NULL);
    }
    //DRV_USB_EP_DONE(3);
    DRV_USB_MSS_DoUSBToPP1(handle);
}

VOID DRV_USB_MSS_CX_TRANS_CMPL()
{
    DRV_USB_CX_DONE();
}

U32 DRV_MSS_USBMSG_HandlerV2(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(pData);
    UNUSED(id_ext);

    if (gChargerStatus == CHARGER_NONE)
        return 0;

    switch(MsgId)
    {
        case DRV_USB_STANDARD_CMD:
        {
            DRV_USB_MSS_StandardCommand();

        }
        break;

        case DRV_USB_CLASS_CMD:
        {
            DRV_USB_MSS_ClassCmd();
        }
        break;

        case DRV_USB_MSG_CX_TRANS_CMPL:
        {
            DRV_USB_MSS_CX_TRANS_CMPL();
        }
        break;

        case DRV_USB_MSG_EP1_IN:
            {
                if(g_MSSProcess == DoRead)
                    MSG_MessageSend(handle, DRV_USB_MSG_EP1_IN, NULL);
                DRV_USB_MSS_DoPPToUSB1(handle);
            }
            break;
        case DRV_USB_MSG_EP1_TRANS_CMPL:
            {
                if (gChargerStatus != 0)
                {
                    if(g_MSSProcess == DoCmdEnd)
                    {
                        g_MSSProcess = DoCmd;
                        DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
                    }
                    else if(g_MSSProcess == DoRead)
                    {
                        DRV_USB_MSS_DoPPToUSB2(handle);
                    }
                }
            }
            break;
        case DRV_USB_MSG_EP3_OUT :
            {// if there still buffer read data
                if(g_MSSProcess == DoCmd)
                {
                    DRV_USB_EP_VDMARead2(3, g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff, id_ext);
                }
                else
                {
                    DRV_USB_MSS_DoUSBToPP1(handle);
                }
            }
            break;
        case DRV_USB_MSG_EP3_TRANS_CMPL:
            {// Epoint 2 only have command & data
                if(g_MSSProcess == DoCmd)
                {
                    DRV_USB_MSS_ParsingCBW(g_USBMSSBuff[g_USBRWFlag].pu8MSSBuff);
                }
                else if(g_MSSProcess == DoWrite)
                {
                    DRV_USB_MSS_DoUSBToPP2(handle);
                }
            }
            break;
    }

    return 0;
}

static inline VOID DRV_USB_MSS_DoSDToPP1(Handler handle)
{
    // Set this block to fill data status
    if(g_USBMSSBuff[g_SDRWFlag].Status == 1)
    {
        g_USBMSSBuff[g_SDRWFlag].Status = 2;
        if(g_SDRWFlag == (USB_BUFFER_NUM - 1))
            g_SDRWFlag = 0;
        else
            g_SDRWFlag += 1;
    }

    // if any empty block & data need read
    if(g_TransferInfo.NumBlks_Left_SD >= gUSB_Buffer_Sect)
    {
        if(g_USBMSSBuff[g_SDRWFlag].Status == 0)
        {
            g_USBMSSBuff[g_SDRWFlag].Status = 1;
            g_USBMSSBuff[g_SDRWFlag].DataLen = gUSB_Buffer_Sect;
            g_TransferInfo.NumBlks_Left_SD -= gUSB_Buffer_Sect;
            U32 OffSet = g_TransferInfo.Offset_Blk;
            g_TransferInfo.Offset_Blk += gUSB_Buffer_Sect;
            DRV_SDC_Read(OffSet, g_USBMSSBuff[g_SDRWFlag].pu8MSSBuff, gUSB_Buffer_Size);
        }
        else
            MSG_MessageSend(handle, DRV_SDC_MSG_TRANS_CMPL, NULL);
    }
    else if(g_TransferInfo.NumBlks_Left_SD > 0)
    {
        if(g_USBMSSBuff[g_SDRWFlag].Status == 0)
        {
            g_USBMSSBuff[g_SDRWFlag].Status = 1;
            g_USBMSSBuff[g_SDRWFlag].DataLen = g_TransferInfo.NumBlks_Left_SD;
            g_TransferInfo.NumBlks_Left_SD = 0;
            DRV_SDC_Read(g_TransferInfo.Offset_Blk,  g_USBMSSBuff[g_SDRWFlag].pu8MSSBuff, 512 * g_USBMSSBuff[g_SDRWFlag].DataLen);
        }
        else
            MSG_MessageSend(handle, DRV_SDC_MSG_TRANS_CMPL, NULL);
    }
}

static inline VOID DRV_USB_MSS_DoPPToSD1(Handler handle)
{
    if(g_USBMSSBuff[g_SDRWFlag].Status == 3)
    {
        g_USBMSSBuff[g_SDRWFlag].Status = 0;
        if(g_SDRWFlag == (USB_BUFFER_NUM - 1))
            g_SDRWFlag = 0;
        else
            g_SDRWFlag += 1;
    }

    // if any empty block & data need read
    if(g_TransferInfo.NumBlks_Left_SD > gUSB_Buffer_Sect)
    {
        if(g_USBMSSBuff[g_SDRWFlag].Status == 2)
        {
            g_USBMSSBuff[g_SDRWFlag].Status = 3;
            g_USBMSSBuff[g_SDRWFlag].DataLen = gUSB_Buffer_Sect;
            g_TransferInfo.NumBlks_Left_SD -= gUSB_Buffer_Sect;
            U32 OffSet = g_TransferInfo.Offset_Blk;
            g_TransferInfo.Offset_Blk += gUSB_Buffer_Sect;
            DRV_SDC_Write(OffSet, g_USBMSSBuff[g_SDRWFlag].pu8MSSBuff, gUSB_Buffer_Size);
        }
        else
            MSG_MessageSend(handle, DRV_SDC_MSG_TRANS_CMPL, NULL);
    }
    else if(g_TransferInfo.NumBlks_Left_SD > 0)
    {
        if(g_USBMSSBuff[g_SDRWFlag].Status == 2)
        {
            g_USBMSSBuff[g_SDRWFlag].Status = 3;
            g_USBMSSBuff[g_SDRWFlag].DataLen = g_TransferInfo.NumBlks_Left_SD;
            g_TransferInfo.NumBlks_Left_SD = 0;
            U32 OffSet = g_TransferInfo.Offset_Blk;
            g_TransferInfo.Offset_Blk += gUSB_Buffer_Sect;
            DRV_SDC_Write(OffSet,  g_USBMSSBuff[g_SDRWFlag].pu8MSSBuff, 512 * g_USBMSSBuff[g_SDRWFlag].DataLen);
        }
        else
            MSG_MessageSend(handle, DRV_SDC_MSG_TRANS_CMPL, NULL);
    }
    else
    {
        g_MSSProcess = DoCmd;
        DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
    }
}

VOID DRV_MSS_SDCMSG_ClearStatus(VOID)
{
    g_USBMSSBuff[0].Status = 0;
    g_USBMSSBuff[0].DataLen= 0;
    g_USBMSSBuff[1].Status = 0;
    g_USBMSSBuff[1].DataLen = 0;
    g_TransferInfo.NumBlks_Left_SD = 0;
    g_TransferInfo.NumBlks_Left_USB = 0;
    g_TransferInfo.NumBlks_for_transfer= 0;
    g_TransferInfo.Offset_Blk= 0;

    g_MSSProcess = DoCmd;
    g_CSW.u8Status = CSW_STATUS_PHASE_ERROR;
    DRV_USB_MSS_SendCSW(1, (U8 *)&g_CSW);
    DRV_USB_CX_DONE();
}

U32 DRV_MSS_SDCMSG_HandlerV2(Handler handle, U16 MsgId, VOID *pData, U32 id_ext)
{
    UNUSED(pData);
    UNUSED(id_ext);

    if (gChargerStatus == CHARGER_NONE)
        return 0;

    switch(MsgId)
    {
        case DRV_SDC_MSG_TRANS_CMPL:
            {
                if(g_MSSProcess == DoRead)
                {
                    DRV_USB_MSS_DoSDToPP1(handle);
                }
                else if (g_MSSProcess == DoWrite)
                {
                    //DRV_GPIO_SetIOStatus(16, 1, 1);
                    //DRV_GPIO_SetIOStatus(16, 1, 0);
                    DRV_USB_MSS_DoPPToSD1(handle);
                }
            }
            break;
        case DRV_SDC_MSG_CARD_REMOVE:   {} break;
        case DRV_SDC_MSG_CARD_INSERT:   {} break;
        default:
            break;
    }

    return 0;
}

HandlerData  gMSS_StorageMSGUSBV2  = {DRV_MSS_USBMSG_HandlerV2};
HandlerData  gMSS_StorageMSGSDCV2  = {DRV_MSS_SDCMSG_HandlerV2};

VOID DRV_USB_MASS_Release()
{
    int x = 0;
    for(x = 0; x < USB_BUFFER_NUM; x++)
    {
        if (g_USBMSSBuff[x].pu8MSSBuff != 0)
        {
            free(g_USBMSSBuff[x].pu8MSSBuff);
            g_USBMSSBuff[x].Status = 0;
            g_USBMSSBuff[x].DataLen = 0;
            g_USBMSSBuff[x].pu8MSSBuff = NULL;
        }
    }
}

U32 DRV_USB_MSSClass_InitMem()
{
    int x = 0;
    U32 status = 0;

    gUSB_Buffer_Size = USB_BUFFER_SIZE;

    do{
        for(x = 0; x < USB_BUFFER_NUM; x++)
        {
            if (g_USBMSSBuff[x].pu8MSSBuff == NULL)
            {
                g_USBMSSBuff[x].pu8MSSBuff = (U8 *)malloc(gUSB_Buffer_Size);
                if(g_USBMSSBuff[x].pu8MSSBuff == NULL)
                {
                    free(g_USBMSSBuff[x].pu8MSSBuff);
                    status = 1;
                }
                g_USBMSSBuff[x].Status = 0;
                g_USBMSSBuff[x].DataLen = 0;
            }
            else
            {
                status = 1;
                break;
            }
        }

        if (gUSB_Buffer_Size != USB_BUFFER_SIZE)
            break;

        if (status == 1)
        {
            DRV_USB_MASS_Release();
            gUSB_Buffer_Size = (USB_BUFFER_SIZE >> 1);
        }
    }while (status == 1);

    if(status)
    {
        DRV_USB_MASS_Release();
        return FALSE;
    }
    else
    {
        gUSB_Buffer_Sect = gUSB_Buffer_Size/512;
        return TRUE;
    }
}

U32 DRV_USB_Check_MsgHdlr(Handler handler, U16 id, void *payload, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(id);
    UNUSED(payload);
    UNUSED(ext_id);

    U8 USB_Mass_Type;
    U16 rc;

    MSG_MessageCancelAllHandler(&gUSB_Check_MsgHandler);
    rc = NVKEY_ReadFullKey(DRV_CHARGER_MP3_SELECT_eMMC_SD, &USB_Mass_Type, 1);

    if(rc == 0)
        return 0;

    if (gUSB_Check_Status == USB_NONE)
    {
        if (gMP3_Play_Status == MP3_PLAY_NONE)
        {
            if(USB_Mass_Type == DRV_CHARGER_USB_SELECT_eMMC)
            {
                DRV_eMMC_deInit(eUSB_MASS_STORAGE);
            }
            else if(USB_Mass_Type == DRV_CHARGER_USB_SELECT_SD)
            {
                DRV_SDC_deInit(eUSB_MASS_STORAGE);
            }
        }
        if (gUSB_Init_Status == USB_INIT_DONE)
        {
            DRV_USB_DeInitV2();
            DRV_USB_MASS_Release();
        }
        DRV_USB_Set_Sleep();
    }

    return 0;
}

HandlerData gUSB_Check_MsgHandler = {DRV_USB_Check_MsgHdlr};

U32 DRV_USB_MASS_Storage_Init()
{
    U32 timeout = 0;

    if (DRV_USB_MSSClass_InitMem() == TRUE)
    {
        timeout = DRV_USB_Check_Timeout();
        DRV_USB_InitV2();
        MSG_MessageSendLater(&gUSB_Check_MsgHandler, DRV_USB_CHECK_STATUS, NULL, timeout);

        DRV_USB_Configure2Ex_EP1(EP_TYPE_BULK, EP_DIRECTION_IN);
        DRV_USB_Configure2Ex_EP3(EP_TYPE_BULK, EP_DIRECTION_OUT);

        MSG_MessageUsbHandler(&gMSS_StorageMSGUSBV2);
        MSG_MessageSDCHandler(&gMSS_StorageMSGSDCV2);

        return TRUE;
    }

    return FALSE;
}

VOID DRV_USB_MASS_Storage_InitHdlr(U8 device)
{
    if (gChargerStatus == CHARGER_EXIST && gUSB_Init_Status == USB_INIT_NONE)
    {
        if(DRV_USB_MASS_Storage_Init() == FALSE)
        {
            logPrint(LOG_OS, PRINT_LEVEL_DEBUG , USB_MassStorageInitFailLogString, 0);
            if (gMP3_Play_Status == MP3_PLAY_NONE)
            {
                if (device == DEVICE_EMMC)
                    DRV_eMMC_deInit(eUSB_MASS_STORAGE);
                else if (device == DEVICE_SDC)
                    DRV_SDC_deInit(eUSB_MASS_STORAGE);
            }
            else
            {
                //re-init USB mass storage
            }
        }
    }
}

U8 gEMMC_SDC_InitChkCnt = 0;
U8 gEMMC_SDC_ReInitCnt = 0;

U32 DRV_EMMC_ReInit_MsgHdlr(Handler handler, U16 id, void *payload, U32 ext_id);
U32 DRV_EMMC_InitRdyChk_MsgHdlr(Handler handler, U16 id, void *payload, U32 ext_id);
U32 DRV_SDC_ReInit_MsgHdlr(Handler handler, U16 id, void *payload, U32 ext_id);
U32 DRV_SDC_InitRdyChk_MsgHdlr(Handler handler, U16 id, void *payload, U32 ext_id);

HandlerData gEMMC_ReInit_MsgHandler = {DRV_EMMC_ReInit_MsgHdlr};
HandlerData gEMMC_InitRdyChk_MsgHandler = {DRV_EMMC_InitRdyChk_MsgHdlr};
HandlerData gSDC_ReInit_MsgHandler = {DRV_SDC_ReInit_MsgHdlr};
HandlerData gSDC_InitRdyChk_MsgHandler = {DRV_SDC_InitRdyChk_MsgHdlr};

U32 DRV_EMMC_InitRdyChk_MsgHdlr (Handler handler, U16 id, void *payload, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(id);
    UNUSED(payload);

    gEMMC_SDC_Init_Status = EMMC_SDC_INIT_DONE;
    if (g_SDCStatus.CardState == Ready)
    {
        if (ext_id == eAPP_MP3_PLAY)
            gMP3_Play_Status = MP3_PLAY_EXIST;
        gEMMC_SDC_ReInitCnt = 0;
        gEMMC_SDC_InitChkCnt = 0;
    }
    else
    {
        logPrint(LOG_OS, PRINT_LEVEL_DEBUG , USB_EmmcInitFailLogString, 0);
        gEMMC_SDC_InitChkCnt++;
        if (gEMMC_SDC_InitChkCnt < EMMC_SDC_INIT_CHK_RETRY_TIMES)
        {
            MSG_MessageSendLaterEx(&gEMMC_InitRdyChk_MsgHandler, DRV_EMMC_MSG_INIT_RDY_CHK, NULL, ext_id, EMMC_SDC_INIT_RDY_TIME_MS);
        }
        else
        {
            gEMMC_SDC_ReInitCnt++;

            DRV_eMMC_deInit((U8)ext_id);
            if (gEMMC_SDC_ReInitCnt < EMMC_SDC_REINIT_TIMES)
            {
                MSG_MessageSendLaterEx(&gEMMC_ReInit_MsgHandler, DRV_EMMC_MSG_REINIT, NULL, ext_id, EMMC_SDC_REINIT_DELAY_TIME_MS);
            }
            else
            {
                //send emmc init fail msg to APP
                gEMMC_SDC_ReInitCnt = 0;
                gEMMC_SDC_InitChkCnt = 0;
            }
        }
    }

    return 0;
}

U32 DRV_EMMC_ReInit_MsgHdlr (Handler handler, U16 id, void *payload, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(id);
    UNUSED(payload);

    DRV_USB_eMMC_Init((U8) ext_id);

    return 0;
}

U32 DRV_SDC_InitRdyChk_MsgHdlr (Handler handler, U16 id, void *payload, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(id);
    UNUSED(payload);

    gEMMC_SDC_Init_Status = EMMC_SDC_INIT_DONE;
    if (g_SDCStatus.CardState == Ready)
    {
        logPrint(LOG_OS, PRINT_LEVEL_DEBUG , USB_SdcInitPassLogString, 0);

        if (ext_id == eAPP_MP3_PLAY)
            gMP3_Play_Status = MP3_PLAY_EXIST;
        gEMMC_SDC_ReInitCnt = 0;
        gEMMC_SDC_InitChkCnt = 0;
    }
    else
    {
        logPrint(LOG_OS, PRINT_LEVEL_DEBUG , USB_SdcInitFailLogString, 0);
        gEMMC_SDC_InitChkCnt++;
        if (gEMMC_SDC_InitChkCnt < EMMC_SDC_INIT_CHK_RETRY_TIMES)
        {
            MSG_MessageSendLaterEx(&gSDC_InitRdyChk_MsgHandler, DRV_SDC_MSG_INIT_RDY_CHK, NULL, ext_id, EMMC_SDC_INIT_RDY_TIME_MS);
        }
        else
        {
            gEMMC_SDC_ReInitCnt++;
            DRV_SDC_deInit((U8)ext_id);
            if (gEMMC_SDC_ReInitCnt < EMMC_SDC_REINIT_TIMES)
            {
                MSG_MessageSendLaterEx(&gSDC_ReInit_MsgHandler, DRV_SDC_MSG_REINIT, NULL, ext_id, EMMC_SDC_REINIT_DELAY_TIME_MS);
            }
            else
            {
                //send emmc init fail msg to APP
                gEMMC_SDC_ReInitCnt = 0;
                gEMMC_SDC_InitChkCnt = 0;
            }
        }
    }

    return 0;
}

U32 DRV_SDC_ReInit_MsgHdlr (Handler handler, U16 id, void *payload, U32 ext_id)
{
    UNUSED(handler);
    UNUSED(id);
    UNUSED(payload);

    DRV_USB_SDC_Init((U8) ext_id);

    return 0;
}

U8 DRV_USB_eMMC_Init(U8 path)
{
    gEMMC_SDC_InitChkCnt = 0;

    if (gEMMC_SDC_Init_Status == EMMC_SDC_INIT_NONE)
    {
        DRV_SDC_Init_eMMC(4);
        MSG_MessageCancelAllHandler(&gEMMC_InitRdyChk_MsgHandler);
        MSG_MessageCancelAllHandler(&gEMMC_ReInit_MsgHandler);
        MSG_MessageSendLaterEx(&gEMMC_InitRdyChk_MsgHandler, DRV_EMMC_MSG_INIT_RDY_CHK, NULL, (U32)path, EMMC_SDC_INIT_RDY_TIME_MS);

        return EMMC_SDC_INIT_DONE;
    }
    else
    {
        if (path == eAPP_MP3_PLAY)
            gMP3_Play_Status = MP3_PLAY_EXIST;
    }

    return EMMC_SDC_INIT_EXIST;
}

U8 DRV_USB_SDC_Init(U8 path)
{
    gEMMC_SDC_InitChkCnt = 0;

    if (gEMMC_SDC_Init_Status == EMMC_SDC_INIT_NONE)
    {
        DRV_SDC_Init_SDCard();
        MSG_MessageCancelAllHandler(&gSDC_InitRdyChk_MsgHandler);
        MSG_MessageCancelAllHandler(&gSDC_ReInit_MsgHandler);
        MSG_MessageSendLaterEx(&gSDC_InitRdyChk_MsgHandler, DRV_SDC_MSG_INIT_RDY_CHK, NULL, (U32)path, EMMC_SDC_INIT_RDY_TIME_MS);

        return EMMC_SDC_INIT_DONE;
    }
    else
    {
        if (path == eAPP_MP3_PLAY)
            gMP3_Play_Status = MP3_PLAY_EXIST;
    }

    return EMMC_SDC_INIT_EXIST;
}

VOID DRV_USB_MASS_eMMC_Init()
{
    DRV_SDC_Init_eMMC(4);

    DRV_USB_MSSClass_InitMem();
    DRV_USB_InitV2();

    DRV_USB_Configure2Ex_EP1(EP_TYPE_BULK, EP_DIRECTION_IN);
    DRV_USB_Configure2Ex_EP3(EP_TYPE_BULK, EP_DIRECTION_OUT);

    MSG_MessageUsbHandler(&gMSS_StorageMSGUSBV2);
    MSG_MessageSDCHandler(&gMSS_StorageMSGSDCV2);
}

VOID DRV_USB_MASS_SDMem_Init()
{
    DRV_SDC_Init_SDCard();

    DRV_USB_MSSClass_InitMem();
    DRV_USB_InitV2();

    DRV_USB_Configure2Ex_EP1(EP_TYPE_BULK, EP_DIRECTION_IN);
    DRV_USB_Configure2Ex_EP3(EP_TYPE_BULK, EP_DIRECTION_OUT);

    MSG_MessageUsbHandler(&gMSS_StorageMSGUSBV2);
    MSG_MessageSDCHandler(&gMSS_StorageMSGSDCV2);
}

U32 MassStorage_msghanlder_func(Handler handler, U16 id, void *payload, U32 ext_id)
{

    UNUSED(payload);
    UNUSED(ext_id);
    UNUSED(handler);

    if(id == 0x0505)
    {
        DRV_USB_MASS_SDMem_Init();
    }
    else if(id == 0x0510)
    {
        DRV_USB_MASS_eMMC_Init(4);
    }
    else
        return 0;
    return 0;
}

HandlerData MassStorage_MSGHandler = {MassStorage_msghanlder_func};
//#endif

