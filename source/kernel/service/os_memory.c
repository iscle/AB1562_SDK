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
#include "string.h"
#include "assert.h"
#include "os_memory.h"
#include "stdlib.h"
#include "FreeRTOS.h"
////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief monitor of memory not in queue or memory space
 */
OSMEM_MONITOR OSMEM_MONITOR1 = {
                                 {
                                    .prev = (DLIST_STRU_PTR)&OSMEM_MONITOR1,
                                    .next = (DLIST_STRU_PTR)&OSMEM_MONITOR1
                                 },
                                 .noB = 1
                               };

/**
 * @brief entries and size of each memory space
 */
////////////////////////////////////////////////////////////////////////////////
// OSMEM FUNCTION DECLARATIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Get free memory space
 *
 * @param mem_ptr Pointer of OSMEM
 * @return Pointer of free memory space
 */
#if 0
AT_IRAM VOID* OSMEM_Get(
          OSMEM_PTR mem_ptr)
{
    DLIST_STRU_PTR    list_ptr;

    OSMEM_BLK_PTR     blk_ptr;

    /* check input pointers */
    assert(mem_ptr != NULL);

    portENTER_CRITICAL();
    {
        if (mem_ptr->noFB > 0)
        {
            list_ptr = OS_dlist_getfront(&mem_ptr->head);
            assert(list_ptr != NULL);

            mem_ptr->noFB--;
        }
        else
        {
            OS_printf("OSMEM empty[0x%x]",mem_ptr);
            list_ptr = NULL;
        }
    }

    if (list_ptr != NULL)
    {
        blk_ptr = OS_CONTAINER_OF(OS_list_entry(list_ptr,OSMEM_BLKHDR,list),OSMEM_BLK,header);

        assert(blk_ptr == *blk_ptr->header.eob_ptr);

        blk_ptr->header.head_ptr = &OSMEM_MONITOR1.head;

        OS_dlist_append(list_ptr,&OSMEM_MONITOR1.head);

        OSMEM_MONITOR1.noB++;

        /* Save caller address */
        /* its hard to implement in c code , so do it at assmbly*/
        blk_ptr->header.dbg_caller_addr = __builtin_return_address(0);

        portEXIT_CRITICAL();

        return blk_ptr->data_space;
    }
    else
    {
        portEXIT_CRITICAL();
        return NULL;
    }
}
#endif



/**
 * @brief Release memory space
 *
 * @param ptr Pointer of OSMEM
 */
#if 0
AT_IRAM VOID OSMEM_Put(
          VOID* ptr)
{
    OSMEM_PTR mem_ptr;

    OSMEM_BLK_PTR blk_ptr;

    DLIST_STRU_PTR pList;

    /* check input pointer */
    assert(ptr != NULL);

    blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);

    /* check redundant put */
    assert(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);

    /* Anti memory corruption */
    assert(blk_ptr == *blk_ptr->header.eob_ptr);

    /* Clear getter address */
    blk_ptr->header.dbg_caller_addr= __builtin_return_address(0);

    portENTER_CRITICAL();
    {
        OS_dlist_remove(&blk_ptr->header.list);

        OSMEM_MONITOR1.noB--;

        /* Acquire OSMEM */
        mem_ptr = blk_ptr->header.mem_ptr;

        /* Relative head */
        blk_ptr->header.head_ptr = &blk_ptr->header.mem_ptr->head;

        /* Append to free queue */
        OS_dlist_append(&blk_ptr->header.list,&mem_ptr->head);

        mem_ptr->noFB++;
    }
    portEXIT_CRITICAL();

    osTaskSignalList(&mem_ptr->waitHead);

    return;
}
#endif



/**
 * @brief Get number of free memory blocks
 *
 * @param mem_ptr Pointer of OSMEM
 * @return  Number of free memory blocks
 */
//#if !(FEA_OSMEM_SPEEDUP)
AT_IRAM U8 OSMEM_NFree(
    IN      OSMEM_PTR mem_ptr)
{
    U8 i;

    /* check input pointer */
    assert(mem_ptr != NULL);

    i = mem_ptr->noFB;

    return i;
}
//#endif



////////////////////////////////////////////////////////////////////////////////
// OSMQ FUNCTION DECLARATIONS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Do OSMQ initialization
 *
 * @param mq_ptr  Pointer of OSMQ
 */
VOID OSMQ_Init(
          OSMQ_PTR mq_ptr)
{
    memset(mq_ptr,0,sizeof(*mq_ptr));
    OS_dlist_init(&mq_ptr->head);
}



/**
 * @brief Dequeue from specified message queue
 *
 * @param mq_ptr Pointer of OSMQ
 * @return  Pointer of message
 */
#if 1
AT_IRAM VOID* OSMQ_Get(
          OSMQ_PTR mq_ptr)
{
    DLIST_STRU_PTR list_ptr;

    OSMEM_BLK_PTR  blk_ptr;

    /* check input pointer */
    assert(mq_ptr != NULL);

    portENTER_CRITICAL();
    {
        if (mq_ptr->noB > 0)
        {
            mq_ptr->noB--;

            list_ptr = OS_dlist_getfront(&mq_ptr->head);

            /* anti fatal error */
            assert(list_ptr != NULL);
        }
        else
        {
            list_ptr = NULL;
        }
    }

    if (list_ptr != NULL)
    {
        blk_ptr = OS_CONTAINER_OF(OS_list_entry(list_ptr,OSMEM_BLKHDR,list),OSMEM_BLK,header);

        /* anti memory corruption */
        assert(blk_ptr == *blk_ptr->header.eob_ptr);

        blk_ptr->header.head_ptr = &OSMEM_MONITOR1.head;

        OS_dlist_append(list_ptr,&OSMEM_MONITOR1.head);

        OSMEM_MONITOR1.noB++;

        /* Save caller address */
        blk_ptr->header.dbg_caller_addr = __builtin_return_address(0);

        portEXIT_CRITICAL();

        return blk_ptr->data_space;
    }
    else
    {
        portEXIT_CRITICAL();
        return NULL;
    }
}
#endif



/**
 * @brief Enqueue to specified message queue
 *
 * @param mq_ptr  Pointer of OSMQ
 * @param ptr     Pointer of message
 */
#if 1
AT_IRAM VOID OSMQ_Put(
          OSMQ_PTR mq_ptr,
          VOID* ptr)
{
    OSMEM_BLK_PTR blk_ptr;

    /* check input pointers */
    assert(mq_ptr != NULL);
    assert(ptr != NULL);

    blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);

    /* check redundant put */
    assert(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);

    /* Relative head */
    blk_ptr->header.head_ptr = &mq_ptr->head;

    /* anti memory corruption */
    assert(blk_ptr == *blk_ptr->header.eob_ptr);

    blk_ptr->header.dbg_caller_addr = __builtin_return_address(0);

    portENTER_CRITICAL();
    {
        OS_dlist_remove(&blk_ptr->header.list);

        OSMEM_MONITOR1.noB--;

        /* append from queue tail */
        OS_dlist_append(&blk_ptr->header.list,&mq_ptr->head);

        mq_ptr->noB++;
    }
    portEXIT_CRITICAL();

    return;
}
#endif



/**
 * @brief Enqueue to specified message queue from front
 *
 * @param mq_ptr  Pointer of OSMQ
 * @param ptr     Pointer of message
 */
#if 1
AT_IRAM VOID OSMQ_PutFront(
          OSMQ_PTR mq_ptr,
          VOID* ptr)
{
    OSMEM_BLK_PTR blk_ptr;

    /* check input pointers */
    assert(mq_ptr != NULL);
    assert(ptr != NULL);

    blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);

    /* check redundant put */
    assert(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);

    /* Relative head */
    blk_ptr->header.head_ptr = &mq_ptr->head;

    /* anti memory corruption */
    assert(blk_ptr == *blk_ptr->header.eob_ptr);

    blk_ptr->header.dbg_caller_addr = __builtin_return_address(0);

    portENTER_CRITICAL();
    {
        OS_dlist_remove(&blk_ptr->header.list);

        OSMEM_MONITOR1.noB--;

        OS_dlist_prepend(&blk_ptr->header.list,&mq_ptr->head);

        mq_ptr->noB++;
    }
    portEXIT_CRITICAL();

    return;

}
#endif

/**
 * @brief Get number of memory blocks that have been queued in specified OSMEM
 *
 * @param mq_ptr  Pointer of OSMEM
 * @return        Number of memory blocks.
 */
AT_IRAM U8 OSMEM_Entries(
    IN      OSMEM_PTR mem_ptr)
{
    U8 i;

    /* check input pointer */
    assert(mem_ptr != NULL);

    i = mem_ptr->noFB;

    return i;
}


/**
 * @brief Get number of memory blocks that have been queued in specified OSMQ
 *
 * @param mq_ptr  Pointer of OSMQ
 * @return        Number of memory blocks.
 */
AT_IRAM U8 OSMQ_Entries(
    IN      OSMQ_PTR mq_ptr)
{
    U8 i;

    /* check input pointer */
    assert(mq_ptr != NULL);

    i = mq_ptr->noB;

    return i;
}



/**
 * @brief Flush specified OSMQ
 *
 * @param mq_ptr Pointer of specified message queue
 * @Return       Number of total flushed message
 */
AT_IRAM U8 OSMQ_Flush(
    IN      OSMQ_PTR mq_ptr)
{
    VOID* ptr;
    U8 count;

    assert(mq_ptr != NULL);

    count = 0;
    while ((ptr = OSMQ_Get(mq_ptr)) != NULL)
    {
        OSMEM_Put(ptr);
        count++;
    }

    return count;
}

