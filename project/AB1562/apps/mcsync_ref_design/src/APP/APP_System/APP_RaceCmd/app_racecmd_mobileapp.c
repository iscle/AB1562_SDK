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

#include "race_cmd.h"
#include "App_InEarDetection.h"
#include "types.h"
#include "bt_race_command.h"

/**************************************************************************************************
* Define
**************************************************************************************************/


/**************************************************************************************************
* Variable
**************************************************************************************************/
static U32 app_racecmd_mobileappcmdhandler(Handler app, U16 id, void *msg, U32 id_ext);
HandlerData gAppRaceCmd_MobileAppHandle = { app_racecmd_mobileappcmdhandler };


/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static VOID race_app_send(U8 RaceType, U16 Raceid, U8 *ptr, U16 len, U8 channelId);

static U32 app_racecmd_get_in_ear_status(MSG_ID_RACE_TO_APP_GET_EAR_STATUS_CMD_T* pCmd)
{
    U8 status = RACE_ERRCODE_SUCCESS;

    typedef struct
    {
        U8 status;
        U8 role;
        U8 inearstatus;
    }RACECMD_GET_IN_EAR_STATUS_STRU;

    RACECMD_GET_IN_EAR_STATUS_STRU rsp;
    rsp.role = pCmd->role;

    if(rsp.role == 0)
        rsp.inearstatus = APP_InEarDetection_GetStatus();
    else if(rsp.role == 1)
        rsp.inearstatus = APP_InEarDetection_GetPartnerStatus();

    rsp.status = RACE_ERRCODE_SUCCESS;

    race_app_send(RACE_TYPE_RESPONSE, pCmd->keyCode, &status, sizeof(status), pCmd->channelId);
    race_app_send(RACE_TYPE_NOTIFICATION, pCmd->keyCode, (U8*)&rsp, sizeof(RACECMD_GET_IN_EAR_STATUS_STRU), pCmd->channelId);
    return RACE_ERRCODE_SUCCESS;
}

static U32 app_racecmd_set_eardetect_on_off(MSG_ID_RACE_TO_APP_SET_EARDETECT_ON_OFF_CMD_T* pCmd)
{
    U8 status = RACE_ERRCODE_SUCCESS;
    APP_InEarDetection_SetFeature(pCmd->isOn);
    APP_InEarDetection_SetDebounceTimer();//check current in ear status
    race_app_send(RACE_TYPE_RESPONSE, pCmd->keycode, &status, sizeof(status), pCmd->channelId);
    return RACE_ERRCODE_SUCCESS;
}

static U32 app_racecmd_get_eardetect_on_off(MSG_ID_RACE_TO_APP_GET_EARDETECT_ON_OFF_CMD_T* pCmd)
{
    U8 isOn;
    isOn = APP_InEarDetection_IsEnable();
    race_app_send(RACE_TYPE_RESPONSE, pCmd->keycode, &isOn, sizeof(isOn), pCmd->channelId);
    return RACE_ERRCODE_SUCCESS;
}

static U32 app_racecmd_mobileappcmdhandler(Handler app, U16 id, void *msg, U32 id_ext)
{
    UNUSED(app);
    UNUSED(id_ext);
    switch(id)
    {
        case MSG_ID_RACE_TO_APP_GET_EAR_STATUS_CMD:
            app_racecmd_get_in_ear_status((MSG_ID_RACE_TO_APP_GET_EAR_STATUS_CMD_T*)msg);
            break;
        case MSG_ID_RACE_TO_APP_SET_EARDETECT_ON_OFF_CMD:
            app_racecmd_set_eardetect_on_off((MSG_ID_RACE_TO_APP_SET_EARDETECT_ON_OFF_CMD_T*)msg);
            break;
        case MSG_ID_RACE_TO_APP_GET_EARDETECT_ON_OFF_CMD:
            app_racecmd_get_eardetect_on_off((MSG_ID_RACE_TO_APP_GET_EARDETECT_ON_OFF_CMD_T*)msg);
            break;
    }

    return 0;
}

static VOID race_app_send(U8 RaceType, U16 Raceid, U8 *ptr, U16 len, U8 channelId)
{
    U8 *pRacePkt = (U8 *)RACE_ClaimPacket(RaceType, Raceid, len, channelId);

    if(pRacePkt)
    {
        memcpy((U8 *)pRacePkt, ptr, len);
        RACE_FlushPacket(pRacePkt, len);
    }
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
void app_racecmd_mobileapp_init(void)
{
    RACE_APP_RegisterMobileAppHandler(&gAppRaceCmd_MobileAppHandle);
}
