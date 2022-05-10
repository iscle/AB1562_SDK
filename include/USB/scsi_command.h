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

/** @brief SCSI Comment Group*/
/** @brief: subclass code: 06h SCSI transparent command set 
 **         define: UASP Bootability specification.         */
#define SCSI_Group_0                            0x00
#define SCSI_Group_1                            0x20
#define SCSI_Group_2                            0x40
#define SCSI_Group_4                            0x80
#define SCSI_Group_5                            0xA0

/* SCSI OPERATION CODE
 * 
 */
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
#define SCSI_CMD_READ12                         0xa8
#define SCSI_CMD_WRITE12                        0xaa

