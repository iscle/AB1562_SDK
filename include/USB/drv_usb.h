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
#include "message.h"
#include "string.h"

#pragma once

/**
 * @defgroup PAGE_USB_DEVICE USB_Device Libraries (audio cdc-acm mass-storage bluetooth)
   @{
       This section introduces the USB Device Library APIs including terms and acronyms,
       supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.

       @defgroup PAGE_USB_CORE USB-Core
       @{
            @defgroup PAGE_USB_CORE_DS Data Structures
            @defgroup PAGE_USB_CORE_FN Functions
       @}

       @defgroup PAGE_USB_CDC CDC-ACM-Class
       @{
            @defgroup PAGE_USB_CDC_DS Data Structures
            @defgroup PAGE_USB_CDC_FN Functions
       @}

       @defgroup PAGE_USB_AUDIO Audio-Class
       @{
            @defgroup PAGE_USB_AUDIO_DS Data Structures
            @defgroup PAGE_USB_AUDIO_FN Functions
       @}

       @defgroup PAGE_USB_MASS_STORAGE Mass-Storage-Class
       @{
            @defgroup PAGE_USB_MASS_STORAGE_DS Data Structures
            @defgroup PAGE_USB_MASS_STORAGE_FN Functions
       @}

       @defgroup PAGE_USB_BLUETOOTH Bluetooth-Class
       @{
            @defgroup PAGE_USB_BLUETOOTH_DS Data Structures
            @defgroup PAGE_USB_BLUETOOTH_FN Functions
       @}
   @}
*/


/**
 * @ingroup PAGE_USB_CORE_DS
 * @{
 * This section introduces the USB Audio Class APIs
 *
 */


#define DRV_USB_MSG_EP1_IN              MSG_MESSAGE_USB_BASE + 0
#define DRV_USB_MSG_EP1_OUT             MSG_MESSAGE_USB_BASE + 1
#define DRV_USB_MSG_EP1_SPK             MSG_MESSAGE_USB_BASE + 2
#define DRV_USB_MSG_EP1_TRANS_CMPL      MSG_MESSAGE_USB_BASE + 3

#define DRV_USB_MSG_EP2_IN              MSG_MESSAGE_USB_BASE + 4
#define DRV_USB_MSG_EP2_OUT             MSG_MESSAGE_USB_BASE + 5
#define DRV_USB_MSG_EP2_SPK             MSG_MESSAGE_USB_BASE + 6
#define DRV_USB_MSG_EP2_TRANS_CMPL      MSG_MESSAGE_USB_BASE + 7

#define DRV_USB_MSG_EP3_IN              MSG_MESSAGE_USB_BASE + 8
#define DRV_USB_MSG_EP3_OUT             MSG_MESSAGE_USB_BASE + 9
#define DRV_USB_MSG_EP3_SPK             MSG_MESSAGE_USB_BASE + 10
#define DRV_USB_MSG_EP3_TRANS_CMPL      MSG_MESSAGE_USB_BASE + 11

#define DRV_USB_MSG_EP4_IN              MSG_MESSAGE_USB_BASE + 12
#define DRV_USB_MSG_EP4_OUT             MSG_MESSAGE_USB_BASE + 13
#define DRV_USB_MSG_EP4_SPK             MSG_MESSAGE_USB_BASE + 14
#define DRV_USB_MSG_EP4_TRANS_CMPL      MSG_MESSAGE_USB_BASE + 15

#define DRV_USB_MSG_CX_IN               MSG_MESSAGE_USB_BASE + 16
#define DRV_USB_MSG_CX_OUT              MSG_MESSAGE_USB_BASE + 17
#define DRV_USB_MSG_CX_TRANS_CMPL       MSG_MESSAGE_USB_BASE + 18

#define DRV_USB_STANDARD_CMD            MSG_MESSAGE_USB_BASE + 19
#define DRV_USB_CLASS_CMD               MSG_MESSAGE_USB_BASE + 20

#define DRV_USB_HOST_DEVICE_PLUG        MSG_MESSAGE_USB_BASE + 21
#define DRV_USB_HOST_DEVICE_UNPLUG      MSG_MESSAGE_USB_BASE + 22

#define DRV_USB_CHECK_STATUS            MSG_MESSAGE_USB_BASE + 23

#define EP_TYPE_ISO                         0X01
#define EP_TYPE_BULK                        0X02
#define EP_TYPE_INT                         0X03
#define EP_DIRECTION_IN                     0x01
#define EP_DIRECTION_OUT                    0x00
#define EP_DIRECTION_BI                     0x02

/*
 * Ch9 descriptor Type
 */
#define DT_DEVICE                           1
#define DT_CONFIGURATION                    2
#define DT_STRING                           3
#define DT_INTERFACE                        4
#define DT_ENDPOINT                         5
#define DT_DEVICE_QUALIFIER                 6
#define DT_OTHER_SPEED_CONFIGURATION        7
#define DT_INTERFACE_POWER                  8
#define DT_OTG                              9

/*
 * USB class code List
 */
#define USB_CLASS_CODE_INTERFACE            0x00
#define USB_CLASS_CODE_AUDIO                0x01
#define USB_CLASS_CODE_CDC                  0x02
#define USB_CLASS_CODE_HID                  0x03
#define USB_CLASS_CODE_PHYSICAL             0x05
#define USB_CLASS_CODE_IMAGE                0x06
#define USB_CLASS_CODE_PRINTER              0x07
#define USB_CLASS_CODE_MASS_STORAGE         0x08
#define USB_CLASS_CODE_HUB                  0x09
#define USB_CLASS_CODE_CDC_DATA             0x0A
#define USB_CLASS_CODE_SMART_CARD           0x0B
#define USB_CLASS_CODE_CONTENT_SECURITY     0x0D
#define USB_CLASS_CODE_VIDEO                0x0E
#define USB_CLASS_CODE_PERSONAL_HEALTH      0x0F
#define USB_CLASS_CODE_AUDIO_VIDEO          0x10
#define USB_CLASS_CODE_BILLBOARD            0x11
#define USB_CLASS_CODE_TYPEC                0x12
#define USB_CLASS_CODE_DIAGNOSTIC_DEVICE    0xDC
#define USB_CLASS_CODE_WIRELESS             0xE0
#define USB_CLASS_CODE_MISCELLANEOUS        0xEF
#define USB_CLASS_CODE_APPLICATION          0xFE
#define USB_CLASS_CODE_VENDOR_SPECIFIC      0xFF

/*
 * USB Mass Sorage SubClass Code list
 */
#define USB_MASS_SUB_CLASS_SCSI             0x00
#define USB_MASS_SUB_CLASS_SCSI_TRANSPARENT 0x06

/*
 * USB Mass Sorage Protocol Code List
 */
#define USB_MASS_PROTOCOL_CBI_NOCOMMAND     0x00
#define USB_MASS_PROTOCOL_CBI_COMMAND       0x01
#define USB_MASS_PROTOCOL_BBB               0x50

/*
 * USB Mass Sorage Request Code List
 */
#define USB_MASS_REQUEST_ADSC               0x00
#define USB_MASS_REQUEST_GET_REQUEST        0xFC
#define USB_MASS_REQUEST_PUT_REQUEST        0xFD
#define USB_MASS_REQUEST_GET_MAX_LUN        0xFE
#define USB_MASS_REQUEST_BULK_ONLY_RESET    0xFF

// Endpoint or FIFO direction define
#define DIRECTION_IN    0
#define DIRECTION_OUT   1

// Controller Mode
#define DRV_USB_DEVICE_MODE 210
#define DRV_USB_HOST_MODE   220
#define DRV_USB_HW_MODE     0

/*
 * USB Standard Request Codes.
 *     - CH9 defines this.
*/
#define USB_STANDARD_REQUEST_GET_STATUS         0
#define USB_STANDARD_REQUEST_CLEAR_FEATURE      1
#define USB_STANDARD_REQUEST_RESERVE_FUTURE     2
#define USB_STANDARD_REQUEST_SET_FEATURE        3
#define USB_STANDARD_REQUEST_RESERVE_FEATURE    4
#define USB_STANDARD_REQUEST_SET_ADDRESS        5
#define USB_STANDARD_REQUEST_GET_DESCRIPTOR     6
#define USB_STANDARD_REQUEST_SET_DESCRIPTOR     7
#define USB_STANDARD_REQUEST_GET_CONFIGURATION  8
#define USB_STANDARD_REQUEST_SET_CONFIGURATION  9
#define USB_STANDARD_REQUEST_SET_GET_INTERFACE  10
#define USB_STANDARD_REQUEST_SET_INTERFACE      11
#define USB_STANDARD_REQUEST_SYNCH_FRAME        12

/*
 * CH9 Table 9-5 Descriptor Type
 */
#define USB_DEVICE_DESCRIPTOR                   1
#define USB_CONFIGURATION_DESCRIPTOR            2
#define USB_STRING_DESCRIPTOR                   3
#define USB_INTERFACE_DESCRIPTOR                4
#define USB_ENDPOINT_DESCRIPTOR                 5
#define USB_DEVICE_QUALIFIER_DESCRIPTOR         6
#define USB_OTHER_SPEED_DESCRIPTOR              7
#define USB_INTERFACE_POWER_DESCRIPTOR          8


enum
{
    USB_INIT_NONE,
    USB_INIT_DONE,
};

enum
{
    USB_NONE,
    USB_EXIST,
};

typedef enum
{
   BULK_TRANS,
   ISO_TRANS,
   ASYN_TRANS,
   INT_TRANS,
} EPtype;

typedef struct EPConfig
{
    U32 EPType;
    U32 FIFONum;
    U32 FIFODirection;
    U32 FIFOBlkSize;
}EPConfiguration, *lpEPConfiguration;

typedef struct Setup_Packet
{
    U8 Direction;    /*Data transfer direction: IN, OUT*/
    U8 Type;         /*Request Type: Standard, Class, Vendor*/
    U8 Object;       /*Recipient: Device, Interface, Endpoint,other*/
    U16 Request;
    U16 Value;
    U16 Index;
    U16 Length;
} SetupPacket, *lpSetupPacket;

/*!
   @}
*/
EXTERN U8 gUSB_Init_Status;
EXTERN U8 gUSB_Check_Status;

/**
 * @ingroup PAGE_USB_CORE_FN
 * @{
 * This section introduces the USB Audio Class APIs
 *
 */

// We should use below function.
EXTERN U32 DRV_USB_Check_Timeout(VOID);

EXTERN U32 DRV_USB_IsDmaBusy(VOID);

EXTERN VOID DRV_USB_Set_WakeUp();
EXTERN VOID DRV_USB_Set_Sleep();

EXTERN VOID DRV_USB_Device_Init(VOID);

EXTERN VOID DRV_USB_InitV2(VOID);
EXTERN VOID DRV_USB_DeInitV2();
EXTERN VOID DRV_USB_InitFullSpeed(VOID);

EXTERN VOID DRV_USB_SetDeviceAddress(U32 Address);
EXTERN VOID DRV_USB_SetConfigureDone();
EXTERN VOID DRV_USB_SetLanguage(U32 DataLen);

EXTERN VOID DRV_USB_CX_DONE();
EXTERN VOID DRV_USB_EP_IN_ENABLE(U32 EPNo);
EXTERN VOID DRV_USB_EP_IN_DISABLE(U32 EPNo);

EXTERN lpSetupPacket DRV_USB_GetSetupPacket();

EXTERN VOID DRV_USB_Configure2_EP1(U8 Type, U8 Direction);
EXTERN VOID DRV_USB_Configure2_EP2(U8 Type, U8 Direction);
EXTERN VOID DRV_USB_Configure2_EP3(U8 Type, U8 Direction);
EXTERN VOID DRV_USB_Configure2_EP4(U8 Type, U8 Direction);

EXTERN VOID DRV_USB_CX_VDMARead(U8 *pBuff, U32 Len);
EXTERN VOID DRV_USB_CX_VDMAWrite2(U8 *pBuff, U32 Len);
EXTERN VOID DRV_USB_CX_VDMARead2(U8 *pBuff, U32 Len);

EXTERN VOID DRV_USB_FIFO_VDMARead2(U32 FIFONum, U8 *pBuff, U32 Len);
EXTERN VOID DRV_USB_FIFO_VDMAWrite2(U32 FIFONum, U8 *pBuff, U32 Len);

EXTERN VOID DRV_USB_EP_VDMARead2(U32 EPNo, U8 *pBuff, U32 Len);
EXTERN VOID DRV_USB_EP_VDMAWrite2(U32 EPNo, U8 *pBuff, U32 Len, U32 LastBlock);
EXTERN VOID DRV_USB_EP_VDMAWrite3(U32 EPNo, U8 *pBuff, U32 Len, U32 LastBlock);

EXTERN U32 DRV_USB_EP_DataLen(U32 EPNo);
EXTERN VOID DRV_USB_EP_Reset(U32 EPNo);
EXTERN VOID DRV_USB_EP_DONE(U32 EPNo);

EXTERN VOID DRV_USB_Configure2Ex_EP1(U8 TYPE, U8 Direction);
EXTERN VOID DRV_USB_Configure2Ex_EP3(U8 TYPE, U8 Direction);

EXTERN VOID DRV_USB_Configure2SEx_EP1(U8 Type, U8 Direction);
EXTERN VOID DRV_USB_Configure2SEx_EP2(U8 Type, U8 Direction);

EXTERN VOID DRV_USB_EP_VDMAReadBlocking(U32 EPNo, U8 *pBuff, U32 Len);
EXTERN VOID DRV_USB_EP_VDMAWriteBlocking(U32 EPNo, U8 *pBuff, U32 Len);

EXTERN U32 DRV_USB_EP_VDMAReadBlocking2(U32 EPNo, U8 *pBuff, U32 Len);
EXTERN U32 DRV_USB_EP_VDMAWriteBlocking2(U32 EPNo, U8 *pBuff, U32 Len);

EXTERN VOID DRV_USB_CX_VDMAReadBlocking(U8* pBuff, U32 Len);
EXTERN VOID DRV_USB_CX_VDMAWriteBlocking(U8* pBuff, U32 Len);

EXTERN VOID DRV_USB_EP_Configure_Len(U32 EPNo, U32 Length);

EXTERN U8 f_Eject;


/*!
   @}
*/
//#endif

