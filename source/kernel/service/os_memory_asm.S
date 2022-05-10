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
#include <xtensa/coreasm.h>

#if 0
    FEA_OSMEM_SPEEDUP = 1
  // ***************************************************************************
  // * Global symbol
  // ***************************************************************************
     .global OSMQ_Put
     .global OSMQ_Get
     //.global OSMEM_Get
     //.global OSMEM_Put
     .global OSMQ_PutFront
     .global OS_malloc
     //.global OS_SW_INTR
  // ***************************************************************************
  // * External symbol
  // ***************************************************************************
     .extern OSMEM_MONITOR1
     .extern osTaskNotifyKernel
     .extern osTaskSignalList
     //.extern OSMEM0;
     //.extern OSMEM1;
     //.extern OSMEM_User;
     //.extern OSMEM2_small_TX;
     //.extern OSMEM2_tiny_RX;
     //.extern OSMEM2_small_RX;
     //.extern OSMEM2_large_RX;
     .extern _OS_TRAP
     .extern dbg_printf
     //.extern OSMEM_printf;
     .extern gSysMemoryEntries

  // ***************************************************************************
  // * Data structure definitions
  // ***************************************************************************
    OSMEM_OFFSET_HEAD     = 0
    OSMEM_OFFSET_noFB     = 8
    OSMEM_OFFSET_WAITHEAD = 12

    OSMEM_BLKHDR_OFFSET_LIST      = 0
    OSMEM_BLKHDR_OFFSET_EOB_PTR   = 8
    OSMEM_BLKHDR_OFFSET_HEAD_PTR  = 12
    OSMEM_BLKHDR_OFFSET_DBG_PTR   = 16

    OSMQ_OFFSET_HEAD     = 0
    OSMQ_OFFSET_NOB      = 8
    OSMQ_OFFSET_WAITHEAD = 12

    OSMEM_BLK_OFFSET_HEADER = 0
    OSMEM_BLK_OFFSET_DATASPACE = 20

    OSMEM_MONITOR_OFFSET_NOB = 8

    SYS_MEMORY_ENTRY_OFFSET_OSMEM0          = 3
    SYS_MEMORY_ENTRY_OFFSET_OSMEM1          = 5
    SYS_MEMORY_ENTRY_OFFSET_OSMEM2_SMALL_TX = 8
    SYS_MEMORY_ENTRY_OFFSET_OSMEM2_TINY_RX  = 12
    SYS_MEMORY_ENTRY_OFFSET_OSMEM2_SMALL_RX = 16
    SYS_MEMORY_ENTRY_OFFSET_OSMEM2_LARGE_RX = 20

////////////////////////////////////////////////////////////////////////////////
// macro ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
.macro dlist_append ax , ay
                l32i    a4 , \ay , 0
                s32i    \ay, \ax , 4
                s32i    a4 , \ax , 0
                s32i    \ax, a4  , 4
                s32i    \ax, \ay , 0
.endm


.macro dlist_prepend ax , ay
                s32i.n	\ay, \ax, 0
                l32i.n	 a4, \ay, 4
                s32i.n	 a4, \ax, 4
                s32i.n	\ax, \ay, 4
                s32i.n	\ax,  a4, 0
.endm


.macro dlist_remove ax
                l32i	a3 , \ax, 0
                l32i	a5 , \ax, 4
                s32i	a5 , a3 , 4
                l32i	a4 , \ax, 4
                s32i	a3 , a4 , 0
                s32i	\ax, \ax, 0
                s32i	\ax, \ax, 4
.endm

//***************************************************************************
// VOID OSMQ_Put(
//         OSMQ_PTR mq_ptr,
//         OS_DATA_PTR ptr)
//***************************************************************************
#if (1)
.section ".iram.OSMQ_Put", "ax"
.literal_position
.align 4
OSMQ_Put:
                addi     a1 , a1 , -16
                s32i     a0 , a1 , 0
                s32i     a2 , a1 , 4    //mq_ptr
                s32i     a3 , a1 , 8    //ptr

                //OS_ASSERT(mq_ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMQ_Put_dead1

                //OS_ASSERT(ptr != OS_NULL_DATA_PTR);
                beqz     a3 , OSMQ_Put_dead2

                //blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);
                addi     a8 , a3 , -OSMEM_BLK_OFFSET_DATASPACE
                s32i     a8 , a1 , 12   //blk_ptr

                //OS_ASSERT(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);
                l32i     a9 , a8 , OSMEM_BLKHDR_OFFSET_HEAD_PTR
                movi     a4, OSMEM_MONITOR1
                bne      a9 , a4, OSMQ_Put_dead3_DoublePut

                //blk_ptr->header.head_ptr = &mq_ptr->head;
                s32i     a2 , a8 , OSMEM_BLKHDR_OFFSET_HEAD_PTR

                //OS_ASSERT(blk_ptr == *blk_ptr->header.eob_ptr);
                l32i     a10, a8 , OSMEM_BLKHDR_OFFSET_EOB_PTR
                l32i     a10, a10, 0
                bne      a8 , a10, OSMQ_Put_dead4_MemoryOverFlow

                //blk_ptr->header.dbg_getter_addr = 0;
                s32i     a0 , a8,  OSMEM_BLKHDR_OFFSET_DBG_PTR

                /* os enter critical */
                rsil     a11 , 4

                //OSMEM_MONITOR1.noB--;
                l32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB
                addi     a10, a10, -1
                s32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB

                //OS_dlist_remove(&blk_ptr->header.list);
                l32i     a2 , a1 , 12
                //call0    OS_dlist_remove
                dlist_remove a2


                //OS_dlist_append(&blk_ptr->header.list,&mq_ptr->head);
                l32i     a2 , a1 , 12
                l32i     a3 , a1 , 4
                //call0    OS_dlist_append
                dlist_append a2 , a3

                //mq_ptr->noB++;
                l32i     a8 , a1 , 4
                l32i     a9 , a8 , OSMQ_OFFSET_NOB
                addi     a9 , a9 , 1
                s32i     a9 , a8 , OSMQ_OFFSET_NOB

                /* os exit critical */
                wsr      a11, PS
                rsync

                //osTaskSignalList(&mem_ptr->waitHead);
#if(AIROHAOS)
                l32i     a8 , a1 , 4
                l32i     a2 , a8 , OSMQ_OFFSET_WAITHEAD
                call0    osTaskSignalList
#endif
OSMQ_Put1:
                l32i     a0 , a1 , 0
                addi     a1 , a1 ,16
                ret
/* mq_ptr is not exist */
OSMQ_Put_dead1:
                call0    _OS_TRAP
OSMQ_Put_dead1_:
                j        OSMQ_Put_dead1_

/* ptr is NULL*/
OSMQ_Put_dead2:
                call0    _OS_TRAP
OSMQ_Put_dead2_:
                j        OSMQ_Put_dead2_

/* double put */
OSMQ_Put_dead3_DoublePut:
                mov      a2 , a8
                call0    OSMEM_Dump
OSMQ_Put_dead3_DoublePut1:
                j        OSMQ_Put_dead3_DoublePut1

/* memory over flow */
OSMQ_Put_dead4_MemoryOverFlow:
                mov      a2 , a8
                call0 OSMEM_Dump
OSMQ_Put_dead4_MemoryOverFlow1:
                j        OSMQ_Put_dead4_MemoryOverFlow1
#endif

//***************************************************************************
//** OS_DATA_PTR OSMQ_Get(
//**   IN      OSMQ_PTR mq_ptr)
//***************************************************************************
#if (1)
.section ".iram.OSMQ_Get", "ax"
.literal_position
.align 4
OSMQ_Get:
                //LIST_STRU_PTR list_ptr;
                //OSMEM_BLK_PTR blk_ptr;

                addi     a1 , a1 , -16
                s32i     a0 , a1 , 0
                s32i     a2 , a1 , 4    //mq_ptr


                //OS_ASSERT(mq_ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMQ_Get_dead1

                /* os enter critical */
                rsil     a11, 4

                //if (mq_ptr->noB > 0)
                l32i     a9 , a2 , OSMQ_OFFSET_NOB
                mov      a8 , a2
                movi     a2 , 0                     //do list_ptr = OS_NULL_DATA_PTR;  here
                beqz     a9 , OSMQ_Get1

                //mq_ptr->noB--;
                addi     a9 , a9 , -1
                s32i     a9 , a8 , OSMQ_OFFSET_NOB

                //list_ptr = OS_list_getfront(&mq_ptr->head);
                l32i     a2 , a1 , 4
                call0    OS_dlist_getfront
                s32i     a2 , a1 , 8

                //OS_ASSERT(list_ptr != OS_NULL_DATA_PTR);
                l32i     a2 , a1 , 8
                beqz     a2 , OSMQ_Get_dead2
OSMQ_Get1:
                beqz     a2 , OSMQ_Get2
                //blk_ptr = OS_CONTAINER_OF(OS_list_entry(list_ptr,OSMEM_BLKHDR,list),OSMEM_BLK,header);
                //blk_ptr = list_ptr actually

                //OS_ASSERT(blk_ptr == *blk_ptr->header.eob_ptr);
                l32i     a10, a2 , OSMEM_BLKHDR_OFFSET_EOB_PTR
                l32i     a10, a10, 0
                bne      a2 , a10, OSMQ_Get_dead3_MemoryOverFlow

                //blk_ptr->header.head_ptr = &OSMEM_MONITOR1.head;
                movi     a3 , OSMEM_MONITOR1
                s32i     a3 , a2 , OSMEM_BLKHDR_OFFSET_HEAD_PTR

                //OSMEM_MONITOR1.noB++;
                l32i     a9 , a3 , OSMEM_MONITOR_OFFSET_NOB
                addi     a9 , a9 , 1
                s32i     a9 , a3 , OSMEM_MONITOR_OFFSET_NOB

                //OS_dlist_append(list_ptr,&OSMEM_MONITOR1.head);
                //call0 OS_dlist_append
                dlist_append a2 , a3

                //blk_ptr->header.dbg_getter_addr = 0;
                l32i     a8 , a1 , 0
                l32i     a2 , a1 , 8
                s32i     a8 , a2 , OSMEM_BLKHDR_OFFSET_DBG_PTR

                //return blk_ptr->data_space;
                addi     a2 , a2 , OSMEM_BLK_OFFSET_DATASPACE
OSMQ_Get2:
                /* os exit critical */
                wsr      a11, PS
                rsync

                l32i     a0 , a1 , 0
                addi     a1 , a1 , 16
                ret
/* mq_ptr = NULL */
OSMQ_Get_dead1:
                call0    _OS_TRAP
OSMQ_Get_dead1_:
                j        OSMQ_Get_dead1_

/* get a invalid memory from queue*/
OSMQ_Get_dead2:
                call0    _OS_TRAP
OSMQ_Get_dead2_:
                j        OSMQ_Get_dead2_

/* data over flow*/
OSMQ_Get_dead3_MemoryOverFlow :
                mov      a2 , a8
                call0    OSMEM_Dump
OSMQ_Get_dead3_loop :
                j        OSMQ_Get_dead3_loop
#endif

//***************************************************************************
//** OS_DATA_PTR OSMEM_Get(
//**   IN      OSMEM_PTR mem_ptr)
//***************************************************************************
#if (0)
.section ".iram.OSMEM_Get", "ax"
.literal_position
.align 4
OSMEM_Get:
                //mem_ptr           a2
                addi     a1 , a1 , -16
                s32i     a0 , a1 , 0
                s32i     a2 , a1 , 4

                //LIST_STRU_PTR     list_ptr;
                //OSMEM_BLK_PTR     blk_ptr;

                //OS_ASSERT(mem_ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMMEM_Get_dead1

                /* os enter critical */
                rsil     a11, 4

                //if (mem_ptr->noFB > 0)
                l8ui     a8 , a2 , OSMEM_OFFSET_noFB
                beqz     a8 , OSMEM_Get4

                //mem_ptr->noFB--;
                addi     a8 , a8 , -1
                s8i      a8 , a2 , OSMEM_OFFSET_noFB

                //list_ptr = OS_dlist_getfront(&mem_ptr->head);
                call0    OS_dlist_getfront

                //OS_ASSERT(list_ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMMEM_Get_dead2
OSMEM_Get2:
                beqz     a2 , OSMEM_Get1
                //blk_ptr = OS_CONTAINER_OF(OS_list_entry(list_ptr,OSMEM_BLKHDR,list),OSMEM_BLK,header);
                //blk_ptr = list_ptr actually

                //OS_ASSERT(blk_ptr == *blk_ptr->header.eob_ptr);
                l32i     a10, a2 , OSMEM_BLKHDR_OFFSET_EOB_PTR
                l32i     a10, a10, 0
                bne      a2 , a10, OSMMEM_Get_dead3_MemoryOverFlow

                //blk_ptr->header.head_ptr = &OSMEM_MONITOR1.head;
                movi     a3 , OSMEM_MONITOR1
                s32i     a3 , a2 , OSMEM_BLKHDR_OFFSET_HEAD_PTR

                //OSMEM_MONITOR1.noB++;
                l32i     a9 , a3 , OSMEM_MONITOR_OFFSET_NOB
                addi     a9 , a9 , 1
                s32i     a9 , a3 , OSMEM_MONITOR_OFFSET_NOB

                //OS_dlist_append(list_ptr,&OSMEM_MONITOR1.head);
                s32i     a2 , a1 , 8
                //call0    OS_dlist_append
                dlist_append a2 , a3

                //blk_ptr->header.dbg_getter_addr = 0;
                l32i     a0 , a1 , 0
                l32i     a2 , a1 , 8
                s32i     a0 , a2 , OSMEM_BLKHDR_OFFSET_DBG_PTR

                //return blk_ptr->data_space;
                addi     a2 , a2 , OSMEM_BLK_OFFSET_DATASPACE
OSMEM_Get1:
                /* os exit critical */
                wsr      a11, PS
                rsync

                l32i     a0 , a1 , 0
                addi     a1 , a1 , 16
                ret
OSMEM_Get4:
                /* os exit critical */
                wsr      a11, PS
                rsync
#if(0)
                movi     a3 , OSMEM1
                beq      a2 , a3, OSMEM_Get5
                movi     a3 , OSMEM_User
                beq      a2 , a3, OSMEM_Get5

                movi     a3 , OSMEM_printf
                mov      a4 , a2
                movi     a2 , 1
                call0    dbg_printf
#endif
OSMEM_Get5:
                l32i     a0 , a1 , 0
                addi     a1 , a1 , 16
                movi     a2 , 0
                ret

/* mem_ptr = NULL */
OSMMEM_Get_dead1:
                call0 _OS_TRAP
OSMMEM_Get_dead1_:
                j     OSMMEM_Get_dead1_

/* get invalid memory from pool */
OSMMEM_Get_dead2:
                call0 _OS_TRAP
OSMMEM_Get_dead2_:
                j     OSMMEM_Get_dead2_

/* memory over flow */
OSMMEM_Get_dead3_MemoryOverFlow:
                call0 OSMEM_Dump
OSMMEM_Get_dead3_MemoryOverFlow1 :
                j     OSMMEM_Get_dead3_MemoryOverFlow1
#endif

//***************************************************************************
//** VOID OSMEM_Put(
//**   IN      OS_DATA_PTR ptr)
//***************************************************************************
#if (0)
.section ".iram.OSMEM_Put", "ax"
.literal_position
.align 4
OSMEM_Put:
                //a2       ptr
                addi     a1 , a1 , -16
                s32i     a0 , a1 , 0
                s32i     a2 , a1 , 4

                //OS_ASSERT(ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMEM_Put_dead1

                //blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);
                addi     a2 , a2 , -OSMEM_BLK_OFFSET_DATASPACE
                //s32i     a8 , a1 , 12   //blk_ptr

                //OS_ASSERT(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);
                l32i     a9 , a2 , OSMEM_BLKHDR_OFFSET_HEAD_PTR
                movi     a4 , OSMEM_MONITOR1
                bne      a9 , a4 , OSMEM_Put_dead2_DoublePut

                //OS_ASSERT(blk_ptr == *blk_ptr->header.eob_ptr);
                l32i     a10, a2 , OSMEM_BLKHDR_OFFSET_EOB_PTR
                l32i     a10, a10, 0
                bne      a2 , a10, OSMEM_Put_dead3_OverFlow

                //blk_ptr->header.dbg_getter_addr= 0;
                s32i     a0 , a2,  OSMEM_BLKHDR_OFFSET_DBG_PTR
                movi     a10, 0
                s32i     a10, a2,  OSMEM_BLKHDR_OFFSET_DBG_COUNT

                /* os enter critical */
                rsil     a11, 4

                //OSMEM_MONITOR1.noB--;
                l32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB
                addi     a10, a10, -1
                s32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB

                //OS_dlist_remove(&blk_ptr->header.list);
                //call0 OS_dlist_remove
                dlist_remove a2

                //mem_ptr = blk_ptr->header.mem_ptr;
                l32i     a3 , a2 , OSMEM_BLKHDR_OFFSET_OSMEM_PTR
                s32i     a3 , a1 , 8

                //blk_ptr->header.head_ptr = &blk_ptr->header.mem_ptr->head;
                s32i     a3 , a2 , OSMEM_BLKHDR_OFFSET_HEAD_PTR

                //mem_ptr->noFB++;
                l8ui     a10 ,a3 , OSMEM_OFFSET_noFB
                addi     a10 ,a10, 1
                s8i      a10 ,a3 , OSMEM_OFFSET_noFB

                //OS_dlist_append(&blk_ptr->header.list,&mem_ptr->head);
                //call0    OS_dlist_append
                dlist_append a2 , a3

                /* os exit critical */
                wsr      a11, PS
                rsync

#if(AIROHAOS)
                //osTaskSignalList(&mem_ptr->waitHead);
                l32i     a8 , a1 , 8
                l32i     a2 , a8 , OSMEM_OFFSET_WAITHEAD
                call0    osTaskSignalList
#endif
OSMEM_Put_exit:
                l32i     a0 , a1 , 0
                addi     a1 , a1 , 16
                ret

/* ptr = NULL */
OSMEM_Put_dead1:
                call0    _OS_TRAP
OSMEM_Put_dead1_:
                j        OSMEM_Put_dead1_

/* memory already in pool or queue */
OSMEM_Put_dead2_DoublePut:
                call0    OSMEM_Dump
OSMEM_Put_dead2_DoublePut1:
                j        OSMEM_Put_dead2_DoublePut1

/* memory over flow */
OSMEM_Put_dead3_OverFlow :
                call0    OSMEM_Dump
OSMEM_Put_dead3_OverFlow1:
                j        OSMEM_Put_dead3_OverFlow1

#endif

//***************************************************************************
//** VOID OSMQ_PutFront(
//**   IN      OSMQ_PTR mq_ptr,
//**   IN      OS_DATA_PTR ptr)
//***************************************************************************
#if (1)
.section ".iram.OSMQ_PutFront", "ax"
.literal_position
.align 4
OSMQ_PutFront:
                addi     a1 , a1 , -16
                s32i     a0 , a1 , 0
                s32i     a2 , a1 , 4    //mq_ptr
                s32i     a3 , a1 , 8    //ptr

                //OS_ASSERT(mq_ptr != OS_NULL_DATA_PTR);
                beqz     a2 , OSMQ_PutFront_dead1

                //OS_ASSERT(ptr != OS_NULL_DATA_PTR);
                beqz     a3 , OSMQ_PutFront_dead2

                //blk_ptr = OS_CONTAINER_OF(ptr,OSMEM_BLK,data_space);
                addi     a8 , a3 , -OSMEM_BLK_OFFSET_DATASPACE
                s32i     a8 , a1 , 12   //blk_ptr

                //OS_ASSERT(blk_ptr->header.head_ptr == &OSMEM_MONITOR1.head);
                l32i     a9 , a8 , OSMEM_BLKHDR_OFFSET_HEAD_PTR
                movi     a4, OSMEM_MONITOR1
                bne      a9 , a4, OSMQ_PutFront_dead3_DoublePut

                //blk_ptr->header.head_ptr = &mq_ptr->head;
                s32i     a2 , a8 , OSMEM_BLKHDR_OFFSET_HEAD_PTR

                //OS_ASSERT(blk_ptr == *blk_ptr->header.eob_ptr);
                l32i     a10, a8 , OSMEM_BLKHDR_OFFSET_EOB_PTR
                l32i     a10, a10, 0
                bne      a8 , a10, OSMQ_PutFront_dead4_MemoryOverFlow

                //blk_ptr->header.dbg_getter_addr = 0;
                //movi     a10, 0
                s32i     a0 , a8,  OSMEM_BLKHDR_OFFSET_DBG_PTR

                /* os enter critical */
                rsil     a11, 4

                ////OSMEM_MONITOR1.noB--;
                l32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB
                addi     a10, a10, -1
                s32i     a10, a4, OSMEM_MONITOR_OFFSET_NOB

                //OS_dlist_remove(&blk_ptr->header.list);
                l32i     a2 , a1 , 12
                //call0 OS_dlist_remove
                dlist_remove a2

                //OS_dlist_append(&blk_ptr->header.list,&mq_ptr->head);
                l32i     a2 , a1 , 12
                l32i     a3 , a1 , 4
                //call0    OS_dlist_prepend
                dlist_prepend a2 , a3

                //mq_ptr->noB++;
                l32i     a8 , a1 , 4
                l32i     a9 , a8 , OSMQ_OFFSET_NOB
                addi     a9 , a9 , 1
                s32i     a9 , a8 , OSMQ_OFFSET_NOB

#if(AIROHAOS)
                //osTaskSignalList(&mem_ptr->waitHead);
                l32i     a8 , a1 , 4
                l32i     a2 , a8 , OSMQ_OFFSET_WAITHEAD
                call0    osTaskSignalList
#endif
OSMQ_PutFront1:
                /* os exit critical */
                wsr      a11, PS
                rsync

                l32i     a0 , a1 , 0
                addi     a1 , a1 ,16
                ret

/* mq_ptr is not exist */
OSMQ_PutFront_dead1:
                call0    _OS_TRAP
OSMQ_PutFront_dead1_:
                j        OSMQ_PutFront_dead1_

/* ptr is NULL*/
OSMQ_PutFront_dead2:
                call0    _OS_TRAP
OSMQ_PutFront_dead2_:
                j        OSMQ_PutFront_dead2_

/* memory already in pool or queue */
OSMQ_PutFront_dead3_DoublePut :
                mov      a2 , a8
                call0    OSMEM_Dump
OSMQ_PutFront_dead3_DoublePut1:
                j        OSMQ_PutFront_dead3_DoublePut1

/* memory over flow */
OSMQ_PutFront_dead4_MemoryOverFlow :
                mov      a2 , a8
                call0    OSMEM_Dump
OSMQ_PutFront_dead4_MemoryOverFlow1:
                j        OSMQ_PutFront_dead4_MemoryOverFlow1
#endif

//***************************************************************************
//OS_DATA_PTR OS_malloc(
//    IN      SIZE Size)
//***************************************************************************
#if (0)
.section ".iram.OS_malloc", "ax"
.literal_position
.align 4
OS_malloc:
                //movi     a4 , OSMEM_Get
                movi     a3 , gSysMemoryEntries
                l8ui     a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM0
                bltu     a5 , a2 , OS_malloc_ptr1
OS_malloc_ptr0:
                movi     a2 , OSMEM0
                j        OSMEM_Get

OS_malloc_ptr1:
                l8ui     a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM1
                bltu     a5 , a2 , OS_malloc_ptr2
                movi     a2 , OSMEM1
                j        OSMEM_Get
OS_malloc_ptr2:
                l16ui    a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM2_SMALL_TX
                bltu     a5 , a2 , OS_malloc_ptr3
                movi     a2 , OSMEM2_small_TX
                j        OSMEM_Get
OS_malloc_ptr3:
                l16ui    a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM2_TINY_RX
                bltu     a5 , a2 , OS_malloc_ptr4
                movi     a2 , OSMEM2_tiny_RX
                j        OSMEM_Get
OS_malloc_ptr4:
                l16ui    a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM2_SMALL_RX
                bltu     a5 , a2 , OS_malloc_ptr5
                movi     a2 , OSMEM2_small_RX
                j        OSMEM_Get
OS_malloc_ptr5:
                l16ui    a5 , a3 , SYS_MEMORY_ENTRY_OFFSET_OSMEM2_LARGE_RX
                bltu     a5 , a2 , OS_malloc_null
                movi     a2 , OSMEM2_large_RX
                j        OSMEM_Get
OS_malloc_null:

                movi     a2 , 0
                ret
#endif

.section ".iram.OSMEM_Dump", "ax"
.literal_position
.align 4
OSMEM_Dump:
                l32i     a15, a2 , OSMEM_BLKHDR_OFFSET_DBG_PTR
                s32i     a15, a1 , 4
                j        _OS_TRAP

                //end
#endif

