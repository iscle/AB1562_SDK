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
 * @ingroup PAGE_USB_MASS_STORAGE_DS
 * @{
 * This section introduces the USB mass-storage Class APIs
 *
 */

// ping-pong buffer settings. frame base. NUM > 2 needed
#define USB_BUFFER_NUM  2
// ping-pong buffer size (for usb), smaller lead slow speed.
#define USB_BUFFER_SIZE 1024 * 2
// ping-pong buffer size (for sdc/emmc), smaller lead slow spped
//#define USB_BUFFER_SECT USB_BUFFER_SIZE/512


#define DRV_EMMC_MSG_INIT_RDY_CHK			MSG_MESSAGE_EMMC_BASE + 0
#define DRV_EMMC_MSG_REINIT					MSG_MESSAGE_EMMC_BASE + 1

enum
{
    MP3_PLAY_NONE,
    MP3_PLAY_EXIST,
};

enum
{
    EMMC_SDC_INIT_NONE,
    EMMC_SDC_INIT_DONE,
    EMMC_SDC_INIT_EXIST,
};

enum
{
	eReserved,
	eAPP_MP3_PLAY,
	eUSB_MASS_STORAGE,
};

enum
{
    USB_SUCCESS,
    USB_FAIL,
    USB_RESERVED,
};

enum
{
    DEVICE_NONE,
    DEVICE_EMMC,
    DEVICE_SDC,
};

/**
 * @}
 */
EXTERN HandlerData MassStorage_MSGHandler;
EXTERN HandlerData gUSB_Check_MsgHandler;
EXTERN U8 gEMMC_SDC_Init_Status;
EXTERN U8 gMP3_Play_Status;


/**
* @ingroup PAGE_USB_MASS_STORAGE_FN
* @{
* This section introduces the USB mass-storage Class APIs
*
*/
/*!
  @brief using SD memory

  @return void: no return value
*/
EXTERN VOID DRV_USB_MASS_SDMem_Init();
/*!
  @brief using eMMC

  @return void: no return value
*/
EXTERN VOID DRV_USB_MASS_eMMC_Init();
EXTERN U32 DRV_USB_MASS_Storage_Init();
EXTERN U8 DRV_USB_eMMC_Init(U8 path);
EXTERN U8 DRV_USB_SDC_Init(U8 path);
EXTERN VOID DRV_MSS_SDCMSG_ClearStatus(VOID);
EXTERN VOID DRV_USB_MASS_Release();
EXTERN U32 DRV_USB_MSSClass_InitMem();

/**
 * @}
 */
//#endif

