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


#include "bt.h"
#include "App_CustomerTimer.h"
#ifdef AIROHA_BT_LE_ENABLE
#include "APP_CustApp_LE.h"
#endif


/**************************************************************************************************
* Variable
**************************************************************************************************/
static U32 app_CustomerTimerHandler(Handler handler, U16 id, void *msg, U32 handler_id);
static const HandlerData gAppCustomerTimerHandle = { app_CustomerTimerHandler };


/**************************************************************************************************
* Static Function (Handler)
**************************************************************************************************/
static U32 app_CustomerTimerHandler(Handler handler, U16 id, void *msg, U32 handler_id)
{
	UNUSED(handler);
	UNUSED(msg);
	UNUSED(handler_id);

	switch(id)
	{
		case CUSTOMER_TIMER_ID_0:
			break;

		case CUSTOMER_TIMER_ID_1:
			break;

		case CUSTOMER_TIMER_ID_2:
			break;

		case CUSTOMER_TIMER_ID_3:
#ifdef AIROHA_BT_LE_ENABLE
            APP_CustApp_LE_SendNotification();
#endif
			break;

		case CUSTOMER_TIMER_ID_4:
			break;

		case CUSTOMER_TIMER_ID_5:
			break;

		case CUSTOMER_TIMER_ID_6:
			break;

		case CUSTOMER_TIMER_ID_7:
			break;

		case CUSTOMER_TIMER_ID_8:
			break;

		case CUSTOMER_TIMER_ID_9:
			break;
	}

	return 0;
}

/**************************************************************************************************
* Public Function
**************************************************************************************************/
void APP_CustomerSetTimer(U16 id, void *msg, U32 handler_id, U32 delay)
{
	FW_SetTimer((Handler)&gAppCustomerTimerHandle, id, msg, handler_id, delay);
}

void APP_CustomerReleaseTimer(U16 id, U32 handler_id)
{
    FW_ReleaseTimer((Handler)&gAppCustomerTimerHandle, id, handler_id);
}

