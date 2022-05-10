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
#pragma once

/**
 * @ingroup PAGE_USB_CDC_DS
 * @{
 * This section introduces the USB CDC ACM Class APIs
 *
 */

// ping-pong buffer settings. frame base. NUM > 2 needed
#define CDC_BUFFER_NUM  2
#define CDC_BUFFER_SIZE 512
#define CDC_CBUFFER_SIZE 2048


#define DRV_CDC_MSG_INIT            MSG_MESSAGE_CDC_BASE + 0
#define DRV_CDC_MSG_DATA            MSG_MESSAGE_CDC_BASE + 1
#define DRV_CDC_MSG_DEINIT          MSG_MESSAGE_CDC_BASE + 2


typedef VOID (*fDRV_USB_CDC_HDLR)(VOID);
typedef U8 USB_CDC_HDLR_ID;
#define USB_CDC_HDLR_ID_TX_CPL_CALLBACK    ((USB_CDC_HDLR_ID)0)
#define USB_CDC_HDLR_ID_RX_GET_CALLBACK    ((USB_CDC_HDLR_ID)1)
#define USB_CDC_HDLR_ID_RX_CPL_CALLBACK    ((USB_CDC_HDLR_ID)2)

enum
{
    USB_CDC_INIT_NONE,
    USB_CDC_INIT_DONE,
};

/*!
   @}
*/

typedef struct
{
    U8  *pu8CDCBuff;
    volatile U32 Status;
    volatile U32 DataLen;
}USB_CDC_BUFF_T, *LPUSB_CDC_BUFF_T;

EXTERN volatile U32 gCDCInitFlg;
EXTERN volatile U32 gWriteOffset;
EXTERN volatile U32 gReadOffset;
EXTERN USB_CDC_BUFF_T gUSBCDCOUTBuffC;
EXTERN BOOL _gDrvUsbBusBusyToSend;


/**
 * @ingroup PAGE_USB_CDC_FN
 * @{
 * This section introduces the USB CDC ACM Class APIs
 *
 */


EXTERN VOID DRV_USB_CDC_ACM_MEM_Init(VOID);

/*!
  @brief Init USB_CDC_ACM memory & flags

  @return void:
*/
EXTERN VOID DRV_USB_CDC_ACM_Init(VOID);

/*!
  @brief De-Init USB_CDC_ACM memory & flags

  @return void:
*/
EXTERN VOID DRV_USB_CDC_ACM_DeInit(VOID);


/*!
  @brief Release USB_CDC_ACM memory & flags

  @return void:
*/
EXTERN VOID DRV_USB_CDC_ACM_Relese();


/*!
  @brief Get 1st data length on the queue

  @return 0: no data to get
        none-zero  data length be read.
*/
EXTERN U32 DRV_USB_CDC_ACM1_GetDataLen();

/*!
  @brief Get 1st data on the queue

  @param pBuffer Buffer pointer
  @param DataLen Data length to send

  @return 0: no data to get
        none-zero  data length be read.
*/
EXTERN U32 DRV_USB_CDC_ACM1_Get(U8 *pBuffer, U32 RequestLen);

EXTERN U32 DRV_USB_CDC_ACM1_Get_CBUF(U8 *pCBuffer, U32 RequestLen, VOID* DestCBufStart, U16 DestCBufSize);


/*!
  @brief Send data out

  @param pBuffer Buffer pointer
  @param DataLen Data length to send

  @return 0: Hardware busy. need resend.
        none-zero  data length be send
*/
EXTERN U32 DRV_USB_CDC_ACM1_Send(U8 *pBuffer, U32 DataLen);
EXTERN U32 DRV_USB_CDC_ACM1_Send_Fifo(U8 *pBuffer, U32 DataLen);

EXTERN U8 DRV_USB_CDC_C2D_BufferCopy(VOID* pDesBuf,
                        U8* pSrcBuf,
                        U8* pSrcBufHead,
                        U16 CopySize,
                        U16 SrcBufTotalSize);

EXTERN VOID DRV_USB_CDC_RegisterHdlr(
    IN      USB_CDC_HDLR_ID HdlrId,
    IN      fDRV_USB_CDC_HDLR fCallback);

/**
* @}
*/
#endif
