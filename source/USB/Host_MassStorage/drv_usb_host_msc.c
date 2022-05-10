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
#include "drv_usb_host_msc.h"
#include "drv_usb_host.h"
#include "message.h"
#include "message_system.h"


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

CBW  g_CBW;
LPCWS  g_pCSW;

#define CBW_SIGNATE                             0x43425355
#define CBW_Data_OUT                            0x00
#define CBW_Data_IN                             0x80
#define CSW_SIGNATE                             0x53425355
#define CSW_STATUS_CMD_PASS                     0x00
#define CSW_STATUS_CMD_FAIL                     0x01
#define CSW_STATUS_PHASE_ERROR                  0x02



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


U8 *MakeCBW_MSC_Reset()
{



    return NULL;
}

U8 *MakeCBW_MSC_Inquery()
{
    g_CBW.u32Tag = 0x00010209;
    g_CBW.u32Signature = CBW_SIGNATE;
    g_CBW.u8Flags = CBW_Data_IN;
    g_CBW.u32DataTransferLength = 36;
    g_CBW.u8LUN = 0;
    g_CBW.u8CBLength = 6;

    g_CBW.u8CB[0] = 0x12; // READ_CMD(10)
    g_CBW.u8CB[1] = 0x00;
    g_CBW.u8CB[2] = 0x00;
    g_CBW.u8CB[3] = 0x00;

    g_CBW.u8CB[4] = 0x24;
    g_CBW.u8CB[5] = 0x00;

    return (U8 *)&g_CBW;
}

U8 *MakeCBW_MSC_Read_CMD10(U32 Offset, U16 Blocks)
{
    g_CBW.u32Signature = CBW_SIGNATE;
    g_CBW.u8Flags = CBW_Data_IN;
    g_CBW.u32DataTransferLength = 512;
    g_CBW.u8LUN = 0;
    g_CBW.u8CBLength = 10;

    g_CBW.u8CB[0] = SCSI_CMD_READ10;
    g_CBW.u8CB[1] = 0x00;

    U8 *pT1 = (U8 *)&Offset;

    g_CBW.u8CB[2] = pT1[3];
    g_CBW.u8CB[3] = pT1[2];
    g_CBW.u8CB[4] = pT1[1];
    g_CBW.u8CB[5] = pT1[0];

    g_CBW.u8CB[6] = 0x00;

    U8 *pT2 = (U8 *)&Blocks;

    g_CBW.u8CB[7] = pT2[1];
    g_CBW.u8CB[8] = pT2[0];

    return (U8 *)&g_CBW;
}

U8 *MakeCBW_MSC_Write_CMD10(U32 Offset, U16 Blocks)
{
    g_CBW.u32Signature = CBW_SIGNATE;
    g_CBW.u8Flags = CBW_Data_OUT;
    g_CBW.u32DataTransferLength = 512;
    g_CBW.u8LUN = 0;
    g_CBW.u8CBLength = 10;

    g_CBW.u8CB[0] = SCSI_CMD_WRITE10;
    g_CBW.u8CB[1] = 0x00;

    U8 *pT1 = (U8 *)&Offset;

    g_CBW.u8CB[2] = pT1[3];
    g_CBW.u8CB[3] = pT1[2];
    g_CBW.u8CB[4] = pT1[1];
    g_CBW.u8CB[5] = pT1[0];

    g_CBW.u8CB[6] = 0x00;

    U8 *pT2 = (U8 *)&Blocks;

    g_CBW.u8CB[7] = pT2[1];
    g_CBW.u8CB[8] = pT2[0];

    return (U8 *)&g_CBW;
}

U8 *MakeCBW_MSC_WRITE_TESTUNIT()
{
    g_CBW.u32Signature = CBW_SIGNATE;
    g_CBW.u8Flags = CBW_Data_OUT;
    g_CBW.u32DataTransferLength = 0;
    g_CBW.u8LUN = 0;
    g_CBW.u8CBLength = 0;

    return NULL;
}

U32 DRV_USB_Host_MSC_Write(U8 *buff,  U32 sector,  U16 count)
{
    // Make CBW
    U8 *pCmd = MakeCBW_MSC_Write_CMD10(sector, count);

    // get Data
    DRV_USB_Host_MSCWrite1(pCmd, 31, buff, count * 512);

    // Parsing CSW
    DRV_USB_Host_MSCRead1(pCmd, 31, buff, count * 512);

    return 0;
}

U32 DRV_USB_Host_MSC_Read(U8 *buff,  U32 sector,  U16 count)
{
	U8 *pCmd = NULL;

    // pCmd = MakeCBW_MSC_Inquery();
    // // get Data
    // DRV_USB_Host_MSCWrite(pCmd, 31, buff, 36);

    // DRV_USB_Host_MSCRead(pCmd, 31, buff, 36);

    // Make CBW
    pCmd = MakeCBW_MSC_Read_CMD10(sector, count);
    // get Data
    DRV_USB_Host_MSCWrite2(pCmd, 31, buff, count * 512);

    DRV_USB_Host_MSCRead2(pCmd, 31, buff, count * 512);
    // Parsing CSW

    return 0;
}
#endif

