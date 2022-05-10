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

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

 /**
 * @addtogroup Message
 * @{
 */

#include "types.h"

//////////////////////////////////////////////////////////////////////////////////
// Message Base Definition for Modules                                          //
//////////////////////////////////////////////////////////////////////////////////
// For System message
#define MSG_MESSGE_SYS_BASE             0x8000
// For PIO status monitoring task.
#define MSG_MESSAGE_PIO_BASE	0x0300
// For USB task.
#define MSG_MESSAGE_USB_BASE	0x0400
// For CDC task.
#define MSG_MESSAGE_CDC_BASE	0x0480
// For SDC(SD card controller) task.
#define MSG_MESSAGE_SDC_BASE	0x0500
// For EMMC(EMMC controller) task.
#define MSG_MESSAGE_EMMC_BASE	0x0580
// For BT libraries
#define MSG_MESSAGE_BT_DATA_BASE        0x0600
#define MSG_MESSAGE_BT_BASE	            0x0605
#define MSG_MESSAGE_BT_INTERNAL_BASE	0x0700
// For ADC module.
#define MESSAGE_ADC_BASE 0x0800
// For NVKEY module.
#define MESSAGE_NVKEY_BASE 0x0900

// For KEYPAD status monitoring task.
#define MSG_MESSAGE_KEYPAD_BASE	    0x0A00
#define MSG_MESSAGE_TOUCHKEY_BASE	0x0B00

// For Charger modual.
#define MSG_MESSAGE_CHARGER_BASE	    0x0C00

// For SW Protection.
#define MSG_MESSAGE_SW_PROTECTION	    0x0D00


// For BT Profiles
#define MSG_MESSAGE_BT_PROFILE_BASE    0x1000

// For user application task. Use this definition for the message index base.
#define MSG_MESSAGE_USER_BASE	0x4000
//////////////////////////////////////////////////////////////////////////////////
#define MSG_VOICE_COMMAND_DELAY_HANDLER_ID_BASE 0x5000


//////////////////////////////////////////////////////////////////////////////////
// Time Unit Definition                                                         //
//////////////////////////////////////////////////////////////////////////////////
#define MILLI_SEC_100         (100L)
#define HALF_SEC			  (5 * MILLI_SEC_100)
#define ONE_SEC				  (10 * MILLI_SEC_100)
#define ONE_MIN				  (60 * ONE_SEC)
#define HALF_MIN			  (30 * ONE_SEC)


typedef struct HandlerData *Handler;
/*
The handler function's return value is meaningful for whether the internal system will free the memory
(pointed by the 3rd pamameter) or not. If it returns 0, we'll free it; if it returns 1, we won't free it and
the user can re-use the memory, if it returns 2, the message will be put into the end of the queue. By default, it should returns 0.
*/
typedef struct HandlerData { U32 (*handler)(Handler, U16, void*, U32); } HandlerData;
typedef struct
{
    U16 id;
    void (*func)(void *m);
}MessageFuncEntry;

/*!
  @brief Send a message to the specified handler after the specified delay in milliseconds.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message A pointer to the message data. 0 means no data. The user should dynamically allocate the memory for the message data, and pass through this parameter.
  @param delay The delay in milliseconds before the message will be sent.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendLater(Handler handler, U16 id, void *message, U32 delay);

/*!
  @brief Send a message to the specified handler after the specified delay in milliseconds.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message A pointer to the message data. 0 means no data. The user should dynamically allocate the memory for the message data, and pass through this parameter.
  @param id_ext The user can use this parameter to pass more information.
  @param delay The delay in milliseconds before the message will be sent.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendLaterEx(Handler handler, U16 id, void *message, U32 id_ext, U32 delay);

/*!
  @brief Cancel the first queued message with the specified task and message id. Can't be called in ISR.

  @param handler The handler whose messages will be looked for.
  @param id The message identifier to look for.
  @return 1 if such a message was found and canceled. Else, 0 is returned.
*/
U16 MSG_MessageCancelFirst(Handler handler, U16 id);

/*!
  @brief Cancel the first queued message with the specified task and message id. The cancelled message's payload will be
  returned and should be freed by the caller to avoid memory leakage. Can't be called in ISR.

  @param handler The handler whose messages will be looked for.
  @param id The message identifier to look for.
  @param message After return, this parameter will contain the pointer to the cancelled message's payload.
  @return 1 if such a message was found and canceled. Else, 0 is returned.
*/
U16 MSG_MessageCancelFirstEx(Handler handler, U16 id, void **message);

/*!
  @brief Cancel the first queued message with the specified task, message id, and extended id. Can't be called in ISR.

  @param handler The handler whose messages will be looked for.
  @param id The message identifier to look for.
  @param id_ext The message extended identifier to look for.
  @return 1 if such a message was found and canceled. Else, 0 is returned.
*/
U16 MSG_MessageCancelFirstEx2(Handler handler, U16 id, U32 id_ext);


/*!
  @brief Cancel the first queued message with the specified task, message id, and extended id. Can't be called in ISR.

  @param handler The handler whose messages will be looked for.
  @param id_ext The message extended identifier to look for.
  @return 1 if such a message was found and canceled. Else, 0 is returned.
*/
U16 MSG_MessageCancelEx2(Handler handler, U32 id_ext);

/*!
  @brief Send a message to be be delivered when the corresponding U16 is zero.

  @param handler The handler to deliver the message to.
  @param id The message identifier.
  @param m The message data.
  @param c The condition that must be zero for the message to be delivered.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendConditionally(Handler handler, U16 id, void *m, U16 *c);

/*!
  @brief Send a message to be be delivered when the corresponding U16 is zero.

  @param handler The handler to deliver the message to.
  @param id The message identifier.
  @param m The message data.
  @param c The condition that must be zero for the message to be delivered.
  @param id_ext The user can use this parameter to pass more information.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendConditionallyEx(Handler handler, U16 id, void *m, U16 *c, U32 id_ext);

/*!
  @brief Cancel all queued messages of the specified handler.
  Normally used as part of the process of freeing a handler. Can't be called in ISR.

  @param handler The handler whose all messages will be canceled.
  @return The number of messages canceled.
*/
U16 MSG_MessageFlushHandler(Handler handler);

/*!
  @brief Register a handler to handle PIO changes.

  @param handler This handler will receive PIO related messages.
  @return The old handler (or zero).
*/
Handler MSG_MessagePioHandler(Handler handler);

/*!
  @brief Register a handler to handle USB information messages.

  @param handler This handler will receive USB related messages such as
  a data transfer is completed.
  @return The old handler (or zero).
*/
Handler MSG_MessageUsbHandler(Handler handler);

/*!
  @brief Register a handler to handle SDC(SD card controller) information messages.

  @param handler This handler will receive USB related messages such as
  a data transfer is completed.
  @return The old handler (or zero).
*/
Handler MSG_MessageSDCHandler(Handler handler);

/*!
  @brief Register a handler to handle key pad information messages.

  @param handler This handler will receive key pad related messages such as DRV_KEYPAD_MSG_NORMAL.
  @return The old handler (or zero).
*/
Handler MSG_MessageKeyPadHandler(Handler handler);

/*!
  @brief Register a handler to handle touch key information messages.

  @param handler This handler will receive touch key related messages such as DRV_TOUCH_MSG_NORMAL.
  @return The old handler (or zero).
*/
Handler MSG_MessageTouchKeyHandler(Handler handler);

/*!
  @brief Register a handler to handle Charger in information messages.

  @param handler This handler will receive charger plug related messages such as DRV_CHARGER_MSG_CHARGERIN.
  @return The old handler (or zero).
*/

Handler MSG_MessageForceTouchHandler(Handler handler);

Handler MSG_MessageChargerINHandler(Handler handler);


/*!
  @brief Register a handler to handle Charger in information messages.

  @param handler This handler will receive charger unplug related messages such as DRV_CHARGER_MSG_CHARGEROUT.
  @return The old handler (or zero).
*/
Handler MSG_MessageChargerOUTHandler(Handler handler);


/*!
  @brief Register a handler to handle key pad information messages.

  @param handler This handler will receive charger complete related messages such as DRV_CHARGER_MSG_CHARGERCPL.
  @return The old handler (or zero).
*/
Handler MSG_MessageChargerCPLHandler(Handler handler);


/*!
  @brief Register a handler to handle key pad information messages.

  @param handler This handler will receive Recharger related messages such as DRV_CHARGER_MSG_RECHARGER.
  @return The old handler (or zero).
*/
Handler MSG_MessageReChargerHandler(Handler handler);

Handler MSG_MessageGPIOYHandler(Handler handler);

/*!
  @brief Register a handler to handle key pad information messages.

  @param handler This handler will receive Sensor related messages.
  @return The old handler (or zero).
*/
Handler MSG_MessageSensorHandler(Handler handler);

Handler MSG_MessageUartStreamHandler(Handler handler);

/*!
  @brief Send a message to the corresponding handler immediately.
  The message will be passed to free after delivery.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message The pointer to the message data. 0 means no data.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSend(Handler handler, U16 id, void *message);

/*!
  @brief Send a message to the corresponding handler immediately.
  The message will be passed to free after delivery. This function can be called in ISR.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message The pointer to the message data. 0 means no data.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendFromISR(Handler handler, U16 id, void *message);

/*!
  @brief Send a message to the corresponding handler immediately.
  The message will be passed to free after delivery.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message The pointer to the message data. 0 means no data.
  @param id_ext The user can use this parameter to pass more information.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendEx(Handler handler, U16 id, void *message, U32 id_ext);

/*!
  @brief Send a message to the corresponding handler immediately.
  The message will be passed to free after delivery. This function can be called in ISR.

  @param handler The handler which will receive the message.
  @param id The message identifier.
  @param message The pointer to the message data. 0 means no data.
  @param id_ext The user can use this parameter to pass more information.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendExFromISR(Handler handler, U16 id, void *message, U32 id_ext);

/*!
  @brief Cancel all queued messages with the specified handler and message id. Can't be called in ISR.

  @param handler The handler to cancel message for.
  @param id The message identifier to look for.
  @return A count of how many such messages were canceled.
*/
U16 MSG_MessageCancelAll(Handler handler, U16 id);

/*!
  @brief Cancel all queued messages with the specified handler, message id and extended id. Can't be called in ISR.

  @param handler The handler to cancel message for.
  @param id The message identifier to look for.
  @param id_ext The message extended identifier to look for.
  @return A count of how many such messages were canceled.
*/
U16 MSG_MessageCancelAllEx(Handler handler, U16 id, U32 id_ext);

/*!
  @brief Cancel all queued messages with the specified handler, and extended id. Can't be called in ISR.

  @param handler The handler to cancel message for.
  @param id_ext The message extended identifier to look for.
  @return A count of how many such messages were canceled.
*/
U16 MSG_MessageCancelAllEx2(Handler handler, U32 id_ext);

/*!
  @brief Cancel all queued messages with the specified handler. Can't be called in ISR.

  @param handler The handler to cancel message for.
  @return A count of how many such messages were canceled.
*/
U16 MSG_MessageCancelAllHandler(Handler handler);

/*!
  @brief Search all queued messages with the specified handler, message id and extended id. Can't be called in ISR.

  @param handler The handler to search message for.
  @param id The message identifier to look for.
  @param id_ext The message extended identifier to look for.
  @return A count of how many such messages were found.
*/
U16 MSG_MessageSearchAllEx(Handler handler, U16 id, U32 id_ext);

/*!
  @brief Search all queued messages with the specified handler, and extended id. Can't be called in ISR.

  @param handler The handler to search message for.
  @param id_ext The message extended identifier to look for.
  @return A count of how many such messages were found.
*/
U16 MSG_MessageSearchAllEx2(Handler handler, U32 id_ext);

/*!
  @brief Search all queued messages with the specified handler, and id. Can't be called in ISR.

  @param handler The handler to search message for.
  @param id The message identifier to look for.
  @return A count of how many such messages were found.
*/
U16 MSG_MessageSearchAllEx3(Handler handler, U16 id);

/*!
  @brief Search all queued messages with the specified handler. Can't be called in ISR.

  @param handler The handler to search message for.
  @param id The message identifier to look for.
  @param id_ext The message extended identifier to look for.
  @return A count of how many such messages were found.
*/
U16 MSG_MessageSearchAllHandler(Handler handler);

/*!
  @brief Send a message to be be delivered when the corresponding handler is zero.

  @param handler The handler to deliver the message to.
  @param id The message identifier.
  @param m The pointer to the message data.
  @param c The handler that must be zero for the message to be delivered.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendConditionallyOnHandler(Handler handler, U16 id, void *m, Handler *c);

/*!
  @brief Send a message to be be delivered when the corresponding handler is zero.

  @param handler The handler to deliver the message to.
  @param id The message identifier.
  @param m The pointer to the message data.
  @param c The handler that must be zero for the message to be delivered.
  @param id_ext The user can use this parameter to pass more information.
  @return Return 0 on success, 1 for message allocation failure, 2 for too many pending messages, and 3 for null message handler.
*/
U16 MSG_MessageSendConditionallyOnHandlerEx(Handler handler, U16 id, void *m, Handler *c, U32 id_ext);

/*!
  @brief Set the specified conditional variable's value. This API is used to work with MSG_MessageSendConditionally() & MSG_MessageSendConditionallyEx().

  @param c The address of the conditonal variable.
  @param val The new value of the conditional variable.
  @return Return 0 on success, 1 for wrong parameters.
*/
U16 MSG_MessageSetConditionalVar(U16 *c, U16 val);

/*!
  @brief Set the specified conditional handler's value. This API is used to work with MSG_MessageSendConditionallyOnHandler() & MSG_MessageSendConditionallyOnHandlerEx().

  @param c The address of the conditonal handler.
  @param val The new value of the conditional handler.
  @return Return 0 on success, 1 for wrong parameters.
*/
U16 MSG_MessageSetConditionalHandler(Handler *c, Handler val);

/*
For internal files used. For example, when there's something about USB needed to inform the
upper application, we can use MSG_MessageSend() to the handler named gHandlerRegisteredForUSB, since
it is the handler registered to receive USB related messages.
*/
extern Handler gMSG_HandlerRegisteredForUSB;
extern Handler gMSG_HandlerRegisteredForSDC;
extern Handler gMSG_HandlerRegisteredForSensor;
extern Handler gMSG_HandlerRegisteredForUartStream;

/**
 * @}
*/

#endif

