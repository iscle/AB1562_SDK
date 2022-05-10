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
#pragma once

/**
 * @defgroup PAGE_USB_HOST USB_Host Libraries (Host Mass-Storage)
   @{
       This section introduces the USB Host Library APIs including terms and acronyms,
       supported features, software architecture, details on how to use the Message, enums, structures, typedefs and functions.

       @defgroup PAGE_USB_HOST_CORE Host-Core
       @{
            @defgroup PAGE_USB_HOST_CORE_DS Data Structures
            @defgroup PAGE_USB_HOST_CORE_FN Functions
       @}

       @defgroup PAGE_USB_HOST_MASS_STORAGE Host-Mass-Storage
       @{
            @defgroup PAGE_USB_HOST_MASS_STORAGE_DS Data Structures
            @defgroup PAGE_USB_HOST_MASS_STORAGE_FN Functions
       @}
   @}
 */

/**
 * @ingroup PAGE_USB_HOST_CORE_DS
 * @{
 * This section introduces the USB mass-storage Class APIs 
 *
 */


typedef struct
{
  U8 bDEVICE_LENGTH;         // bLength
  U8 bDT_DEVICE;           // bDescriptorType
  U8 bVerLowByte;                  // bcdUSB
  U8 bVerHighByte;

  U8 bDeviceClass;                 // bDeviceClass
  U8 bDeviceSubClass;              // bDeviceSubClas;
  U8 bDeviceProtocol;              // bDeviceProtocol
  U8 bEP0MAXPACKETSIZE;        // bMaxPacketSize0

  U8 bVIDLowByte;                  // idVendor
  U8 bVIDHighByte;
  U8 bPIDLowByte;                  // idProduct
  U8 bPIDHighByte;
  U8 bRNumLowByte;                     // bcdDeviceReleaseNumber
  U8 bRNumHighByte;

  U8 bManufacturer;              // iManufacturer
  U8 bProduct;                   // iProduct
  U8 bSerialNumber;              // iSerialNumber
  U8 bCONFIGURATION_NUMBER;      // bNumConfigurations
}OTGH_Descriptor_Device_Struct;

typedef struct
{
//<1>.Basic Information
  U8                                   bDeviceOnHub;
  U8                                   bOnHubPortNumber;
  U8                                   bAdd;
  U8                                   bConnectStatus;
  U8                                   bPortEnableDisableStatus;
  U8                                   bSpeed;  //0=>Low Speed / 1=>Full Speed / 2 => High Speed
  U8                                   bPortReset;
  U8                                   bSuspend;
  U8                                   bRemoteWakeUpDetected;
  U8                                   bSendOK;
  U8                                   bSendStatusError;

  U8                                   *bDataBuffer;
//<2>.Descriptor Information

  //OTGH_Descriptor_Device_Struct               sDD;
  //OTGH_Descriptor_Configuration_Only_Struct   saCD[HOST20_CONFIGURATION_NUM_MAX];
  //OTGH_Descriptor_OTG_Struct                  sOTG;

  U8                                   bReportDescriptor[0x74];
  U8                                   bStringLanguage[10];
  U8                                   bStringManufacture[0xFF];
  U8                                   bStringProduct[0xFF];
  U8                                   bStringSerialN[0xFF];
  //<3>.For ISO Information
  U8                                   bISOTransferEnable;
  U32                                  wISOiTDAddress[1024];

}Host20_Attach_Device_Structure;



typedef struct   {

   //<1>.Next_Link_Pointer Word
   U32   bTerminal:1;             //Bit11~0
   U32   bType:2;             //Bit11~0
   U32   bReserved:2;          //Bit14~12
   U32   bLinkPointer:27; //Bit15

} Periodic_Frame_List_Cell_Structure;

typedef struct  {

   Periodic_Frame_List_Cell_Structure   sCell[1024];

} Periodic_Frame_List_Structure;

//<3.2>qTD Structure Definition****************************************


 typedef struct _qTD {

     //<1>.Next_qTD_Pointer Word
      U32   bTerminate:1;
      U32   bReserve_1:4;
      U32   bNextQTDPointer:27;

     //<2>.Alternate Next qTD Word
      U32   bAlternateTerminate:1;
      U32   bReserve_2:4;
      U32   bAlternateQTDPointer:27;

     //<3>.Status Word
      U32   bStatus_PingState:1;
      U32   bStatus_SplitState:1;
      U32   bStatus_MissMicroFrame:1;
      U32   bStatus_Transaction_Err:1;
      U32   bStatus_Babble:1;
      U32   bStatus_Buffer_Err:1;
      U32   bStatus_Halted:1;
      U32   bStatus_Active:1;

      U32   bPID:2;
      U32   bErrorCounter:2;
      U32   CurrentPage:3;
      U32   bInterruptOnComplete:1;
      U32   bTotalBytes:15;
      U32   bDataToggle:1;


     //<4>.Buffer Pointer Word Array
     U32   ArrayBufferPointer_Word[5];

 } qTD_Structure;

    #define HOST20_qTD_PID_OUT                  0x00
    #define HOST20_qTD_PID_IN                   0x01
    #define HOST20_qTD_PID_SETUP                0x02


    #define HOST20_qTD_STATUS_Active            0x80
    #define HOST20_qTD_STATUS_Halted            0x40
    #define HOST20_qTD_STATUS_BufferError       0x20
    #define HOST20_qTD_STATUS_Babble            0x10
    #define HOST20_qTD_STATUS_TransactionError  0x08
    #define HOST20_qTD_STATUS_MissMicroFrame    0x04
    #define HOST20_qTD_STATUS_Split             0x02
    #define HOST20_qTD_STATUS_Ping              0x01


//<3.3>qHD Structure Definition****************************************

 typedef struct _qHD {

     //<1>.Next_qHD_Pointer Word
      U32   bTerminate:1;
      U32   bType:2;
      U32   bReserve_1:2;
      U32   bNextQHDPointer:27;

     //<2>.qHD_2 Word
      U32   bDeviceAddress:7;
      U32   bInactiveOnNextTransaction:1;
      U32   bEdNumber:4;
      U32   bEdSpeed:2;
      U32   bDataToggleControl:1;
      U32   bHeadOfReclamationListFlag:1;
      U32   bMaxPacketSize:11;
      U32   bControlEdFlag:1;
      U32   bNakCounter:4;

     //<3>.qHD_3 Word
      U32   bInterruptScheduleMask:8;
      U32   bSplitTransactionMask:8;
      U32   bHubAddr:7;
      U32   bPortNumber:7;
      U32   bHighBandwidth:2;

     //<4>.Overlay_CurrentqTD
      U32   bOverlay_CurrentqTD;

     //<5>.Overlay_NextqTD
      U32   bOverlay_NextTerminate:1;
      U32   bOverlay_Reserve2:4;
      U32   bOverlay_NextqTD:27;

     //<6>.Overlay_AlternateNextqTD
      U32   bOverlay_AlternateNextTerminate:1;
      U32   bOverlay_NanCnt:4;
      U32   bOverlay_AlternateqTD:27;

     //<7>.Overlay_TotalBytes
      U32   bOverlay_Status:8;
      U32   bOverlay_PID:2;
      U32   bOverlay_ErrorCounter:2;
      U32   bOverlay_C_Page:3;
      U32   bOverlay_InterruptOnComplete:1;
      U32   bOverlay_TotalBytes:15;
      U32   bOverlay_Direction:1;

     //<8>.Overlay_BufferPointer0
      U32   bOverlay_CurrentOffset:12;
      U32   bOverlay_BufferPointer_0:20;

     //<9>.Overlay_BufferPointer1
      U32   bOverlay_C_Prog_Mask:8;
      U32   bOverlay_Reserve3:4;
      U32   bOverlay_BufferPointer_1:20;

     //<10>.Overlay_BufferPointer2
      U32   bOverlay_FrameTag:5;
      U32   bOverlay_S_Bytes:7;
      U32   bOverlay_BufferPointer_2:20;

     //<11>.Overlay_BufferPointer3
      U32   bOverlay_Reserve4:12;
      U32   bOverlay_BufferPointer_3:20;

     //<12>.Overlay_BufferPointer4
      U32   bOverlay_Reserve5:12;
      U32   bOverlay_BufferPointer_4:20;

 } qHD_Structure;


#define qTD_PID_OUT                  0x00
#define qTD_PID_IN                   0x01
#define qTD_PID_SETUP                0x02


#define qTD_STATUS_Active            0x80
#define qTD_STATUS_Halted            0x40
#define qTD_STATUS_BufferError       0x20
#define qTD_STATUS_Babble            0x10
#define qTD_STATUS_TransactionError  0x08
#define qTD_STATUS_MissMicroFrame    0x04
#define qTD_STATUS_Split             0x02
#define qTD_STATUS_Ping              0x01

#define HOST20_HD_Type_iTD                  0x00
#define HOST20_HD_Type_QH                   0x01
#define HOST20_HD_Type_siTD                 0x02
#define HOST20_HD_Type_FSTN                 0x03

typedef enum
{
    WaitConnect        = 0x10,
    PortReset1,
    PortReset1E,
    PortReset2,
    PortReset2E,
    Connected,
    GetDevDescriptor1,
    GetDevDescriptor2,
    UnderTestMode,
}USB_Host_State;

typedef enum
{
    EnumerateStep1  = 0x11, // GetDescriptor start
    EnumerateStep2  = 0x12, // GetDescriptor end
    EnumerateStep3  = 0x13, // SetAddress
    EnumerateStep4  = 0x14, // GetDescriptor start
    EnumerateStep5  = 0x15, // GetDescriptor end
    EnumerateStep6  = 0x16, // GetConfiguration
    EnumerateStep7  = 0x17, // Get Lang Id
    EnumerateStep8  = 0x18, // Get iManufacturer
    EnumerateStep9  = 0x19, // Get iProduct
    EnumerateStep10 = 0x1A, // Get iSerialNumber
}EnumarateState;

typedef enum
{
  TransferNone  = 0x50,
  TransferStart = 0x51,
  TransferEnd   = 0x52,
}TransferState;


typedef struct
{
    U8 bDeviceClass;                 // bDeviceClass
    U8 bDeviceSubClass;              // bDeviceSubClas;
    U8 bVIDLowByte;                  // idVendor
    U8 bVIDHighByte;
    U8 bPIDLowByte;                  // idProduct
    U8 bPIDHighByte;
    U8 bCONFIGURATION_NUMBER;        // bNumConfigurations
    qHD_Structure *pEP0;
    qHD_Structure *pEPA;
    qHD_Structure *pEPB;
}DeviceInfomation_Struct;


#define DRV_USB_HOST_ST_ENUMERATIONSHORT      0x4000
#define DRV_USB_HOST_ST_ENUMERATIONFULL       0x4001
#define MSG_DRV_USB_HOST_PORTRESETON          0x4002
#define MSG_DRV_USB_HOST_PORTRESETOFF         0x4003
#define MSG_DRV_USB_HOST_SENDSETUPPACKET      0x4004


#define MSG_DRV_USB_HOST_BASE                 0x0000
#define MSG_DRV_USB_HOST_GET_DEVICE_S               MSG_DRV_USB_HOST_BASE + 100
#define MSG_DRV_USB_HOST_GET_DEVICE_G               MSG_DRV_USB_HOST_BASE + 101
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_S        MSG_DRV_USB_HOST_BASE + 102
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_G        MSG_DRV_USB_HOST_BASE + 103
#define MSG_DRV_USB_HOST_SET_CONFIGURATION_S        MSG_DRV_USB_HOST_BASE + 104
#define MSG_DRV_USB_HOST_SET_CONFIGURATION_G        MSG_DRV_USB_HOST_BASE + 105
#define MSG_DRV_USB_HOST_SET_ADDRESS_S              MSG_DRV_USB_HOST_BASE + 106
#define MSG_DRV_USB_HOST_SET_ADDRESS_G              MSG_DRV_USB_HOST_BASE + 107

#define MSG_DRV_USB_HOST_GET_DEVICE_S1              MSG_DRV_USB_HOST_BASE + 110
#define MSG_DRV_USB_HOST_GET_DEVICE_S2              MSG_DRV_USB_HOST_BASE + 111
#define MSG_DRV_USB_HOST_GET_DEVICE_S3              MSG_DRV_USB_HOST_BASE + 112
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_SHORT_S1 MSG_DRV_USB_HOST_BASE + 113
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_SHORT_S2 MSG_DRV_USB_HOST_BASE + 114
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_FULL_S1  MSG_DRV_USB_HOST_BASE + 115
#define MSG_DRV_USB_HOST_GET_CONFIGURATION_FULL_S2  MSG_DRV_USB_HOST_BASE + 116

#define MSG_DRV_USB_HOST_GET_LANGIDS_S1             MSG_DRV_USB_HOST_BASE + 120
#define MSG_DRV_USB_HOST_GET_LANGIDS_S2             MSG_DRV_USB_HOST_BASE + 121
#define MSG_DRV_USB_HOST_GET_LANGIDS_S3             MSG_DRV_USB_HOST_BASE + 122
#define MSG_DRV_USB_HOST_GET_LANGIDS_S4             MSG_DRV_USB_HOST_BASE + 123
#define MSG_DRV_USB_HOST_GET_ISERIALNUMBER_S1       MSG_DRV_USB_HOST_BASE + 124
#define MSG_DRV_USB_HOST_GET_ISERIALNUMBER_S2       MSG_DRV_USB_HOST_BASE + 125
#define MSG_DRV_USB_HOST_GET_IPRODUCT_S1            MSG_DRV_USB_HOST_BASE + 126
#define MSG_DRV_USB_HOST_GET_IPRODUCT_S2            MSG_DRV_USB_HOST_BASE + 127


#define MSG_DRV_USB_HOST_RESUME_S1              MSG_DRV_USB_HOST_BASE + 9
#define MSG_DRV_USB_HOST_RESUME_S2              MSG_DRV_USB_HOST_BASE + 10
#define MSG_DRV_USB_HOST_SUSPEND                MSG_DRV_USB_HOST_BASE + 11

#define MSG_DRV_USB_HOST_PORT_RESETON_1         MSG_DRV_USB_HOST_BASE + 150
#define MSG_DRV_USB_HOST_PORT_RESETOFF_1        MSG_DRV_USB_HOST_BASE + 151
#define MSG_DRV_USB_HOST_PORT_RESETON_2         MSG_DRV_USB_HOST_BASE + 152
#define MSG_DRV_USB_HOST_PORT_RESETOFF_2        MSG_DRV_USB_HOST_BASE + 153

#define MSG_DRV_USB_Host_TurnOnReadWrite        MSG_DRV_USB_HOST_BASE + 200
#define MSG_DRV_USB_Host_TurnOffReadWrite       MSG_DRV_USB_HOST_BASE + 201
#define MSG_DRV_USB_Host_SecondReset_End        MSG_DRV_USB_HOST_BASE + 202
#define MSG_DRV_USB_HOST_GETMAXLUN              MSG_DRV_USB_HOST_BASE + 203
#define MSG_DRV_USB_HOST_MSCINQUERY             MSG_DRV_USB_HOST_BASE + 204

/**
* @}
*/


/**
 * @ingroup PAGE_USB_HOST_CORE_FN
 * @{
 * This section introduces the USB mass-storage Class APIs 
 *
 */


EXTERN U8 DRV_USB_Host_Issue_Control (U8 bEdNum,U8* pbCmd,U16 hwDataSize,U8* pbData);
EXTERN VOID DRV_USB_Host_Init(VOID);
EXTERN VOID DRV_USB_Host_TestMode(VOID);

EXTERN VOID DRV_USB_Host_RBulkData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_WBulkData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_RISOData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_WISOData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_RIntData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_WIntData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_RCtlData(U32 EPNum, U8 *pBuffer, U32 BufferLen);
EXTERN VOID DRV_USB_Host_WCtlData(U32 EPNum, U8 *pBuffer, U32 BufferLen);

EXTERN VOID DRV_USB_Host_SetupTestPacket();
EXTERN VOID DRV_USB_Host_SetupJState();
EXTERN VOID DRV_USB_Host_SetupKState();
EXTERN VOID DRV_USB_Host_SetupSE0NAK();
EXTERN U8 DRV_USB_Host_Resume();


EXTERN U32 DRV_USB_Host_MSCRead1(U8 *pCmd, U32 CmdLen, U8 *pData, U32 DataLen);
EXTERN U32 DRV_USB_Host_MSCRead2(U8 *pCmd, U32 CmdLen, U8 *pData, U32 DataLen);

EXTERN U32 DRV_USB_Host_MSCWrite1(U8 *pCmd, U32 CmdLen, U8 *pData, U32 DataLen); 
EXTERN U32 DRV_USB_Host_MSCWrite2(U8 *pCmd, U32 CmdLen, U8 *pData, U32 DataLen);

/**
* @}
*/
