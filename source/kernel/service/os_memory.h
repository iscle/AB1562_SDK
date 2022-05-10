#ifndef _OS_MEMORY_H_
#define _OS_MEMORY_H_
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
#include "os_list.h"
#include "stdlib.h"

////////////////////////////////////////////////////////////////////////////////
// CONSTANT DEFINITIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define OS_MEMBLK_EOB_SIZE (sizeof(OSMEM_BLK *))
#define OS_ADDITIONAL_BYTES_FOR_MEMORY_ARRAY (sizeof(OSMEM_BLKHDR)+OS_MEMBLK_EOB_SIZE)


typedef VOID (*fCallerFunc)(VOID);

////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINITIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 *  @brief OSMEM is OS MEMORY control
 */
typedef struct osmem_stru
{
    /*!  8B, point to list head */
    DLIST_HEAD          head;
    /*!  4B, number of free block */
    U32                  noFB;
    /*!  8B, task wait list */
    DLIST_HEAD          waitHead;
} OSMEM, * OSMEM_PTR;

/**
 *  @brief OSMEM_BLKHDR is the header of MEMORY BLOCK
 */
typedef struct osmem_blk_hdr_stru
{
    /*! 8B, Link list */
    DLIST_STRU          list;

    /*! 4B, End of block memory */
    struct osmem_blk_stru** eob_ptr;

    /*! 4B, Relative list head */
    DLIST_HEAD_PTR      head_ptr;

    /*! 4B, debug use */
    fFunc               dbg_caller_addr;

} OSMEM_BLKHDR, * OSMEM_BLKHDR_PTR;

/**
 *@brief OSMEM_BLK is OS MEMORY BLOCK
 */
typedef struct osmem_blk_stru
{
    /*! 20B, Header of block memory */
    OSMEM_BLKHDR        header;
    /*! ?B, Free data space with unknown size */
    U8                  data_space[1];

} OSMEM_BLK, * OSMEM_BLK_PTR;

/** @brief OSMQ is OS MEMORY QUEUE HEAD */
typedef struct osmq_stru
{
    /*! 8B, point to list head */
    DLIST_HEAD          head;
    /*! 4B, number of memory blocks*/
    U32                 noB;
} OSMQ, * OSMQ_PTR;

/** @brief OSMEM_MONITOR is monitor of free memory block*/
typedef struct osmem_monitor_str
{
    /*! 8B, point to list head */
    DLIST_HEAD          head;
    /*! 1B, number of memory blocks */
    U32                  noB;

}OSMEM_MONITOR, * OSMEM_MONITOR_PTR;

////////////////////////////////////////////////////////////////////////////////
// External Variables //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//- memory monitor
EXTERN OSMEM_MONITOR OSMEM_MONITOR1;
////////////////////////////////////////////////////////////////////////////////
// Macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// OSMEM FUNCTION DECLARATIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Do OSMQ initialization
 *
 * @param mq_ptr  Pointer of OSMQ
 */
EXTERN VOID OSMQ_Init(
    IN      OSMQ_PTR mq_ptr);

/**
 * @brief Do OS Memory Initialization
 *
 * @param pOSMEM     Pointer of OSMEM
 * @param NumBlks    Number of Blocks
 * @param BlkSize    Memory Block Size
 */
EXTERN VOID* OSMEM_Init (
    IN      OSMEM_PTR pOSMEM,
    IN      U8 NumBlks,
    IN      U16 BlkSize);


/**
 * @brief Get free memory space
 *
 * @param mem_ptr Pointer of OSMEM
 * @return Pointer of free memory space
 */
EXTERN VOID* OSMEM_Get (
    IN      OSMEM_PTR mem_ptr);

/**
 * @brief Release memory space
 *
 * @param ptr Pointer of OSMEM
 */
//VOID OSMEM_Put (
//    IN      VOID* ptr);
#define OSMEM_Put(x) free(x)

/**
 * @brief Get number of free memory blocks
 *
 * @param mem_ptr Pointer of OSMEM
 * @return  Number of free memory blocks
 */
EXTERN U8 OSMEM_NFree (
    IN      OSMEM_PTR mem_ptr);



////////////////////////////////////////////////////////////////////////////////
// OSMQ FUNCTION DECLARATIONS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Dequeue from specified message queue
 *
 * @param mq_ptr Pointer of OSMQ
 * @return  Pointer of message
 */
EXTERN VOID* OSMQ_Get (
    IN      OSMQ_PTR mq_ptr);

/**
 * @brief Enqueue to specified message queue
 *
 * @param mq_ptr  Pointer of OSMQ
 * @param ptr     Pointer of message
 */
EXTERN VOID OSMQ_Put (
    IN      OSMQ_PTR mq_ptr,
    IN      VOID* ptr);

/**
 * @brief Enqueue to specified message queue from front
 *
 * @param mq_ptr  Pointer of OSMQ
 * @param ptr     Pointer of message
 */
EXTERN VOID OSMQ_PutFront (
    IN      OSMQ_PTR mq_ptr,
    IN      VOID* ptr);

/**
 * @brief Get number of memory blocks that have been queued in specified OSMQ
 *
 * @param mq_ptr  Pointer of OSMQ
 * @return        Number of memory blocks.
 */
EXTERN U8 OSMQ_Entries (
    IN      OSMQ_PTR mq_ptr);

/**
 * @brief Get number of memory blocks that have been queued in specified OSMQ
 *
 * @param mq_ptr  Pointer of OSMQ
 * @return        Number of memory blocks.
 */
EXTERN U8 OSMQ_Flush(
    IN      OSMQ_PTR mq_ptr);


/**
 * @brief Memory allocation
 *
 * @param Size  Number of bytes memory to allocate
 * @return      Pointer of allocated memory
 */
//#define OS_malloc port_OS_Malloc

EXTERN U32 OSMEM_BlkSize(VOID* ptr);


#endif /* _OS_MEMORY_H_ */

