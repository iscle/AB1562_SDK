#ifndef _OS_LIST_H_
#define _OS_LIST_H_
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

////////////////////////////////////////////////////////////////////////////////
// Type Definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief double link list structure
 */
typedef struct dlist_stru
{
    struct dlist_stru * prev;
    struct dlist_stru * next;

} DLIST_STRU, * DLIST_STRU_PTR;

/*
 * DLIST_HEAD - list head of double link list structure.
 * This structure is a structure of double link list.
 */
typedef DLIST_STRU DLIST_HEAD;
typedef DLIST_STRU_PTR DLIST_HEAD_PTR;



////////////////////////////////////////////////////////////////////////////////
// Function Prototypes /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief       get the container structure of given node
 *
 * @param ptr        the linkin list pointer.
 * @param type       the data structure of container
 * @param member     member of container structure
 * @return           pointer of the contanier structure
 */
#define OS_list_entry(ptr,type,member) OS_CONTAINER_OF(ptr,type,member)

/**
 * @brief       get the container structure of given node
 *
 * @param ptr        the linkin list pointer.
 * @param type       the data structure of container
 * @param member     member of container structure
 * @return           pointer of the contanier structure
 */
#define OS_dlist_entry(ptr,type,member) OS_CONTAINER_OF(ptr,type,member)

/**
 * @breif init a list entry
 *
 * @param list_ptr  list need to initialize
 */
static INLINE VOID OS_dlist_init(
          DLIST_STRU_PTR plist)
{
    plist->next = plist->prev = plist;
}


/**
 * @brief init a list head
 *
 * @param phead head need to initialize
 */
static INLINE VOID OS_dlist_head_init(
            DLIST_HEAD_PTR phead)
{
    OS_dlist_init(phead);
}

/**
 * @brief check whether a list is empty
 *
 * @param plist pointer of list
 * @return TURE means empty , FALSE means not empty
 */
static INLINE BOOL OS_dlist_is_empty(
            DLIST_STRU_PTR plist)
{
    return (plist == plist->next);
}

/**
 * @brief insert a node into a list
 *
 * @param list            pointer of list
 * @param prev_list       pointer of prev list
 * @param next_list       pointer of next list
 */
static INLINE VOID OS_dlist_insert(
                DLIST_STRU_PTR list,
                DLIST_STRU_PTR prev_list,
                DLIST_STRU_PTR next_list)
{
        list->prev = prev_list;
        list->next = next_list;
        prev_list->next = list;
        next_list->prev = list;
}

/**
 * @brief remove node from a list
 *
 * @param  list pointer of list
 */
static INLINE VOID OS_dlist_remove(
          DLIST_STRU_PTR list)
{
    list->prev->next = list->next;
    list->next->prev = list->prev;
    OS_dlist_init(list);
}

/**
 * @brief add a node into list front
 * @param newptr             new node to be added
 * @param head            list head to add it after
 */
static INLINE VOID OS_dlist_prepend(
          DLIST_STRU_PTR newptr,
          DLIST_HEAD_PTR head)
{
    OS_dlist_insert(newptr,head,head->next);    
}

/**
 * @brief  add a node into list back
 * @param newptr            new node to be added
 * @param head           list head to add it before
 *
 * Insert a new node just before the specified head.
 */
static INLINE VOID OS_dlist_append(
          DLIST_STRU_PTR newptr,
          DLIST_HEAD_PTR head)
{
    OS_dlist_insert(newptr,head->prev,head);
}

/**
 * @brief get first node from double link list
 *
 * @param head   head of a double list.
 * @return first node of list , null if no node in list
 */
EXTERN DLIST_STRU_PTR OS_dlist_getfront(
    IN      DLIST_HEAD_PTR head);

#endif /* _OS_LIST_H_ */

