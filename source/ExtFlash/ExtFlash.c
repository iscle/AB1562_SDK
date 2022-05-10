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
#include "ExtFlash.h"
#include "drv_spiflash.h"

#ifdef FEATURE_EXT_SPIFLASH

U32 ExtFlashInit()
{
    DRV_SPIFLH_Initial();

    return 0;
}
U32 ExtFlashPageRead(U32 Offset, U8 *Buffer)
{
    return DRV_SPIFLH_PageRead(Offset, Buffer);
}
U32 ExtFlashPageWrite(U32 Offset, U8 *Buffer)
{
    return DRV_SPIFLH_PageProgram(Offset, Buffer, NULL);
}
U32 ExtFlashByteRead(U32 Offset, U8 *Buffer, U32 BufferLength)
{
    return DRV_SPIFLH_ByteRead(Offset, Buffer, BufferLength);
}
U32 ExtFlashByteWrite(U32 Offset, U8 *Buffer, U32 BufferLength)
{
    return DRV_SPIFLH_ByteProgram(Offset, Buffer, BufferLength);
}
U32 ExtFlashChipErase()
{
    return DRV_SPIFLH_ChipErase();
}
U32 ExtFlash4KErase(U32 offset)
{
    return DRV_SPIFLH_SectorErase(offset);
}
U32 ExtFlash32KErase(U32 offset)
{
    return DRV_SPIFLH_Block_32KB_Erase(offset);
}
U32 ExtFlash64KErase(U32 offset)
{
    return DRV_SPIFLH_Block_64KB_Erase(offset);
}
U32 ExtFlashGetFlashSize()
{
    return DRV_SPIFLH_GetFlashSize();
}
U32 ExtFlashLock(U8 LockId)
{
    return DRV_SPIFLH_Protection(LockId);
}
U32 ExtFlashUnLock()
{
    return DRV_SPIFLH_GetLockBitUnLock();
}  
#endif
