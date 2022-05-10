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

#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#if defined(MT2811A) && defined(CORE_DSP0)
extern uint32_t _ram_start[];
extern uint32_t _ram_length[];
extern uint32_t _vram_start[];
extern uint32_t _vram_length[];
#endif
#ifdef CORE_DSP0
extern uint32_t _sysram_start[];
extern uint32_t _sysram_length[];
extern uint32_t _vsysram_start[];
extern uint32_t _vsysram_length[];
#endif
extern uint32_t _iram_start[];
extern uint32_t _iram_length[];
extern uint32_t _dram_start[];
extern uint32_t _dram_length[];

typedef struct {
    uint32_t BinaryId;
    uint32_t PartitionId;
    uint32_t LoadAddressHigh;
    uint32_t LoadAddressLow;
    uint32_t BinaryLengthHigh;
    uint32_t BinaryLengthLow;
    uint32_t ExecutionAddress;
    uint32_t ReservedItem0;
    uint32_t ReservedItem1;
    uint32_t ReservedItem2;
    uint32_t ReservedItem3;
    uint32_t ReservedItem4;
} PartitionTableItem_T;

typedef struct {
    PartitionTableItem_T SEC_HEADER;
    PartitionTableItem_T BL;
    PartitionTableItem_T N9;
    PartitionTableItem_T CM4;
    PartitionTableItem_T DSP0;
    PartitionTableItem_T DSP1;
    PartitionTableItem_T FOTA;
    PartitionTableItem_T NVDM;
} PartitionTable_T;

#define ROM_BASE    0x08000000
#define ROM_LENGTH  0x400000
#define PARTITION_TABLE  ((PartitionTable_T *)ROM_BASE)

#define SEC_HEADER_BASE     PARTITION_TABLE->SEC_HEADER.LoadAddressLow
#define SEC_HEADER_LENGTH   PARTITION_TABLE->SEC_HEADER.BinaryLengthLow

#define BL_BASE     PARTITION_TABLE->BL.LoadAddressLow
#define BL_LENGTH   PARTITION_TABLE->BL.BinaryLengthLow

#define N9_BASE     PARTITION_TABLE->N9.LoadAddressLow
#define N9_LENGTH   PARTITION_TABLE->N9.BinaryLengthLow

#define CM4_BASE    PARTITION_TABLE->CM4.LoadAddressLow
#define CM4_LENGTH  PARTITION_TABLE->CM4.BinaryLengthLow

#define DSP0_BASE   PARTITION_TABLE->DSP0.LoadAddressLow
#define DSP0_LENGTH PARTITION_TABLE->DSP0.BinaryLengthLow

#define DSP1_BASE   PARTITION_TABLE->DSP1.LoadAddressLow
#define DSP1_LENGTH PARTITION_TABLE->DSP1.BinaryLengthLow

#define FOTA_RESERVED_BASE    PARTITION_TABLE->FOTA.LoadAddressLow
#define FOTA_RESERVED_LENGTH  PARTITION_TABLE->FOTA.BinaryLengthLow

#define ROM_NVDM_BASE    PARTITION_TABLE->NVDM.LoadAddressLow
#define ROM_NVDM_LENGTH  PARTITION_TABLE->NVDM.BinaryLengthLow

#if defined(MT2811A) && defined(CORE_DSP0)
#define RAM_BASE       (uint32_t)_ram_start
#define RAM_LENGTH     (uint32_t)_ram_length

#define VRAM_BASE      (uint32_t)_vram_start
#define VRAM_LENGTH    (uint32_t)_vram_length
#endif

#ifdef CORE_DSP0
#define SYSRAM_BASE    (uint32_t)_sysram_start
#define SYSRAM_LENGTH  (uint32_t)_sysram_length

#define VSYSRAM_BASE   (uint32_t)_vsysram_start
#define VSYSRAM_LENGTH (uint32_t)_vsysram_length
#endif

#define IRAM_BASE      (uint32_t)_iram_start
#define IRAM_LENGTH    (uint32_t)_iram_length
#define DRAM_BASE      (uint32_t)_dram_start
#define DRAM_LENGTH    (uint32_t)_dram_length

#endif

