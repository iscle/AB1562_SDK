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
#include "types.h"
#include "stdio.h"
#include "message.h"

#include "App_ResourceManager.h"
#include "App_System.h"
#include "App_LedControl.h"
#include "App_MCSync_Message.h"
#include "App_Media.h"
#include "App_PowerOff.h"
#include "App_ChargeBattery.h"
#include "App_Battery.h"
#include "syslog.h"

log_create_module(APP_LED, PRINT_LEVEL_INFO);

enum
{
    LED_TYPE_FG,
    LED_TYPE_BG,
    LED_TYPE_FILTER,
    LED_TYPE_BG_RESUME,
};

static U32 app_LED_Handler(Handler handler, U16 id, void *payload, U32 id_ext);

static LED_BG_SETTING_STRU *pStateBGLed;
static LED_CTRL_STRU gLedCtrl;
static const HandlerData gLedHandler = {app_LED_Handler};

#define LED_BG_NUM			gLedCtrl.pLedNum.BGNum
#define LED_FG_NUM			gLedCtrl.pLedNum.FGNum
#define LED_FILTER_NUM		gLedCtrl.pLedNum.FilterNum

#define GLED_CTL_PTR(X)		((LED_SETTING_STRU *)gLedCtrl.ledMsgCtl[X].pattern)
#define GLED_FILTER_PTR		((LED_FILTER_PATTERN_STRUC *)gLedCtrl.pFilterPtn)

#define RESYNC_TIME         (12750)


static const U8 gLedActionMapTimer[MCSYNC_LED_SET_ACTION_NUM] =
{
	LED_HDL_ID_SYNC_BG_TIMER,
	LED_HDL_ID_SYNC_FG_TIMER,
	LED_HDL_ID_SYNC_RESUMEBG_TIMER,
};


static void app_LED_SetFgPattern(U8 ptnIndex, U8 overwriteTime, BOOL isOverWrite);


static void app_LED_PushAction(U8 action)
{
	gLedCtrl.action.actionQ[gLedCtrl.action.actionTail] = action;
	gLedCtrl.action.actionTail = (gLedCtrl.action.actionTail + 1) % MAX_ACTION_Q_NUM;
}

static U8 app_LED_PopAction(void)
{
	U8 action = gLedCtrl.action.actionQ[gLedCtrl.action.actionHead];

	gLedCtrl.action.actionHead = (gLedCtrl.action.actionHead + 1) % MAX_ACTION_Q_NUM;

	return action;
}

static void app_LED_SendSyncInfo(U32 timeToWait, U32 curPicoClock, U8 action)
{
	U32 clockToStart;
	U8 patternType[6], ledIndex;

	clockToStart = curPicoClock + (timeToWait * 1000) / HALF_SLOT;

	for(ledIndex = LED0; ledIndex < LED_NUM; ledIndex++)
	{
		if(gLedCtrl.ledMsgCtl[ledIndex].ptnType == LED_FILTER_DATA)
		{
			patternType[ledIndex] = LED_NUM_DATA;
		}
		else
		{
			patternType[ledIndex] = gLedCtrl.ledMsgCtl[ledIndex].ptnType;
		}
	}

	APP_MCSYNC_SendSyncLEDInfo(gLedCtrl.awsCtl[action].ptnIndex, patternType,
								gLedCtrl.awsCtl[action].overwriteTime, gLedCtrl.awsCtl[action].isOverWrite, clockToStart, action);
}

static void app_LED_StartFgSync(U8 ptnIndex, U8 overwriteTime, BOOL isOverWrite)
{
	BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	if(BtAwsMce_IsDefaultRolePartner())
	{
		if(!PM_IsProfileConnected(pServiceBdAddr, PROFILE_MCSYNC))
		{
			app_LED_SetFgPattern(ptnIndex, overwriteTime, isOverWrite);
		}

		return;
	}

	gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].ptnIndex = ptnIndex;
	gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].overwriteTime = overwriteTime;
	gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].isOverWrite = isOverWrite;

	if(BtAwsMce_IsWithPartner(pServiceBdAddr))
	{
		app_LED_PushAction(MCSYNC_LED_AGENT_SET_FG);
		BtAwsMce_GetCurrentPiconetBtClock(pServiceBdAddr, PICO_EVENT_LED);
	}
	else
	{
		app_LED_SetFgPattern(ptnIndex, overwriteTime, isOverWrite);
	}
}

static BOOL app_LED_IsFgSyncNeeded(U16 fgEvent)
{
	switch(fgEvent)
	{
		case MEDIA_EVT_BAT_LOW_LED:
		case MEDIA_EVT_POWER_ON:
		case MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT1:
		case MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT2:
		case MEDIA_EVT_POWER_ON_UNDER_CHECKPOINT3:
		case MEDIA_EVT_POWER_OFF:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT1:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT2:
		case MEDIA_EVT_POWER_OFF_UNDER_CHECKPOINT3:
		case MEDIA_EVT_POWER_OFF_IN_SHUTDOWN_LOW_BAT:
		case MEDIA_EVT_BAT_LOW:
		case MEDIA_EVT_BAT_FULL:
		case MEDIA_EVT_BAT_CHGCPL:
		case MEDIA_EVT_BAT_RECHG:
		case MEDIA_EVT_BAT_CHGROUT:
		case MEDIA_EVT_BAT_CHGRIN:
		case MEDIA_EVT_BAT_CHGTO:
		case MEDIA_EVT_SLC_CONNECTED:
			return FALSE;

		default:
			break;
	}

	return TRUE;
}

static void app_LED_ReadStateBGNvkey(void)
{
	U8* pAddr;
	U8 dataSize;

	pAddr = (U8*)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_STATE_BG_LED_SETTING);
	dataSize = NVKEY_GetPayloadLength(NVKEYID_APP_STATE_BG_LED_SETTING);

	if(pAddr == NULL || dataSize <= 0)
	{
		FW_Assert(FALSE);
	}
	else
	{
		pStateBGLed = (LED_BG_SETTING_STRU *)pAddr;
	}

	return ;
}

static BOOL app_LED_ReadNumber(void)
{
	U16 number;

	gLedCtrl.pLedNum.BGNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_LED_BG_PATTERN, NULL, 0)) > 0)
	{
		if((number % sizeof(LED_PATTERN_STRU)) == 0)
		{
			gLedCtrl.pLedNum.BGNum = (number / sizeof(LED_PATTERN_STRU));
		}
	}

	gLedCtrl.pLedNum.FGNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_LED_FG_PATTERN, NULL, 0)) > 0)
	{
		if((number % sizeof(LED_PATTERN_STRU)) == 0)
		{
			gLedCtrl.pLedNum.FGNum = (number / sizeof(LED_PATTERN_STRU));
		}
	}

	gLedCtrl.pLedNum.FilterNum = 0;
	if((number = NVKEY_ReadFullKey(NVKEYID_APP_LED_FILTER_PATTERN, NULL, 0)) > 0)
	{
		if((number % sizeof(LED_FILTER_PATTERN_STRUC)) == 0)
		{
			gLedCtrl.pLedNum.FilterNum = (number / sizeof(LED_FILTER_PATTERN_STRUC));
		}
	}

	return TRUE;
}

static BOOL app_LED_ReadDataIDNvkey(void)
{
	if((gLedCtrl.pLedBGHeader = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_LED_BG_PATTERN)) == NULL)
		return FALSE;

	if((gLedCtrl.pLedFGHeader = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_LED_FG_PATTERN)) == NULL)
		return FALSE;

	if((gLedCtrl.pFilterHeader = (U8 *)NVKEY_GetPayloadFlashAddress(NVKEYID_APP_LED_FILTER_PATTERN)) == NULL)
		return FALSE;

	return TRUE;
}

static void *app_LED_ReadPattern(U8 ptnType, U8 ptnIndex)
{
	void *pLedPattern;
	if(!gLedCtrl.isDataOk)
		return NULL;

	switch(ptnType)
	{
		case LED_BG_DATA:
			pLedPattern = (void *)(gLedCtrl.pLedBGHeader + ptnIndex *sizeof(LED_PATTERN_STRU));
			break;
		case LED_FG_DATA:
			pLedPattern = (void *)(gLedCtrl.pLedFGHeader + ptnIndex * sizeof(LED_PATTERN_STRU));
			break;
		case LED_FILTER_DATA:
			pLedPattern = (void *)(gLedCtrl.pFilterHeader + ptnIndex * sizeof(LED_FILTER_PATTERN_STRUC));
			break;
		default:
			pLedPattern = NULL;
			break;
	}
	return pLedPattern;
}

static U8 app_LED_GetBitPos(U8 index)
{
	U8 ledBit;

	switch(index)
	{
		case LED0:
			ledBit = 0x01;
			break;
		case LED1:
			ledBit = 0x02;
			break;
		case LED2:
			ledBit = 0x04;
			break;
		case LED3:
			ledBit = 0x08;
			break;
		case LED4:
			ledBit = 0x10;
			break;
		case LED5:
			ledBit = 0x20;
			break;
		default:
			ledBit = 0;
			break;
	}

	return ledBit;
}

static BOOL app_LED_IsPtnCanBeChanged(U8 oldType, U8 newType)
{
	return (newType >= oldType ? TRUE : FALSE);
}

void App_LED_SetPatternToHW(U8 ledIndex ,LED_SETTING_STRU *pLedPtr)
{
    drv_led_setting pSetting;

    if(GLED_FILTER_PTR != NULL && GLED_FILTER_PTR->filterType == FILTER_MULTI_DIVIDE)
    {
        if(GLED_FILTER_PTR->ledMultiDivide[ledIndex].isMultiPly)
        {
            if((0xFF / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t0) < GLED_CTL_PTR(ledIndex)->t0)
                goto DEFAULT_LED_SET;
            if((0xFF / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t1) < GLED_CTL_PTR(ledIndex)->t1)
                goto DEFAULT_LED_SET;
            if((0xFF / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t2) < GLED_CTL_PTR(ledIndex)->t2)
                goto DEFAULT_LED_SET;
            if((0xFF / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t3) < GLED_CTL_PTR(ledIndex)->t3)
                goto DEFAULT_LED_SET;

            if((0x0F / GLED_CTL_PTR(ledIndex)->t1t2_repeat_cnt) < GLED_FILTER_PTR->ledMultiDivide[ledIndex].t12Rpt)
                goto DEFAULT_LED_SET;

            pSetting.unit_ms = pLedPtr->ledtimeUnit;
            pSetting.duration_sel = pLedPtr->pwmDuration;
            pSetting.t0_cnt = (pLedPtr->t0 * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t0);
            pSetting.t1_cnt = (pLedPtr->t1 * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t1);
            pSetting.t2_cnt = (pLedPtr->t2 * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t2);
            pSetting.t3_cnt = (pLedPtr->t3 * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t3);
            pSetting.t1t2_repeat_cnt = (pLedPtr->t1t2_repeat_cnt * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t12Rpt);
            //pSetting.nostop = pLedPtr->ledRepeatAlways; //for HW workaround
            pSetting.nostop = ((pSetting.t2_cnt == 0 && pSetting.t3_cnt == 0) || pLedPtr->noSync)? pLedPtr->ledRepeatAlways : 0; //for HW workaround
            pSetting.pwm_en = (pLedPtr->pwmDuration != 0 ? TRUE : FALSE);
            pSetting.follow_idx= pLedPtr->ledFollowIndex;
            pSetting.inverse = pLedPtr->ledInversEnable;
            if(pLedPtr->loopForExtendRepeat)
            {
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_REPEAT_TIMES, NULL, LED_HDL_IDEXT_REPEAT_LED0 + ledIndex,
                            (pSetting.t0_cnt + (pSetting.t1_cnt + pSetting.t2_cnt) * pSetting.t1t2_repeat_cnt + pSetting.t3_cnt) * pSetting.unit_ms);
            }
            if(pLedPtr->onOff >= LED_ALT_FIRST && pLedPtr->onOff <= LED_ALT_SIXTH)
            {
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ALT_SCENARIO, NULL, LED_HDL_IDEXT_ALT_LED0 + ledIndex,
                            pLedPtr->t4 * GLED_FILTER_PTR->ledMultiDivide[ledIndex].t4 * pSetting.unit_ms);
            }
        }
        else if(GLED_FILTER_PTR->ledMultiDivide[ledIndex].isDivide)
        {
            pSetting.unit_ms = pLedPtr->ledtimeUnit;
            pSetting.duration_sel = pLedPtr->pwmDuration;
            pSetting.t0_cnt = (pLedPtr->t0 / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t0);
            pSetting.t1_cnt = (pLedPtr->t1 / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t1);
            pSetting.t2_cnt = (pLedPtr->t2 / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t2);
            pSetting.t3_cnt = (pLedPtr->t3 / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t3);
            pSetting.t1t2_repeat_cnt = (pLedPtr->t1t2_repeat_cnt / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t12Rpt);
            //pSetting.nostop = pLedPtr->ledRepeatAlways; //for HW workaround
            pSetting.nostop = ((pSetting.t2_cnt == 0 && pSetting.t3_cnt == 0) || pLedPtr->noSync) ? pLedPtr->ledRepeatAlways : 0; //for HW workaround
            pSetting.pwm_en = (pLedPtr->pwmDuration != 0 ? TRUE : FALSE);
            pSetting.follow_idx= pLedPtr->ledFollowIndex;
            pSetting.inverse = pLedPtr->ledInversEnable;
            if(pLedPtr->loopForExtendRepeat)
            {
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_REPEAT_TIMES, NULL, LED_HDL_IDEXT_REPEAT_LED0 + ledIndex,
                            (pSetting.t0_cnt + (pSetting.t1_cnt + pSetting.t2_cnt) * (pSetting.t1t2_repeat_cnt + 1)+ pSetting.t3_cnt) * pSetting.unit_ms);
            }
            if(pLedPtr->onOff >= LED_ALT_FIRST && pLedPtr->onOff <= LED_ALT_SIXTH)
            {
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ALT_SCENARIO, NULL, LED_HDL_IDEXT_ALT_LED0 + ledIndex,
                            pLedPtr->t4 * pSetting.unit_ms / GLED_FILTER_PTR->ledMultiDivide[ledIndex].t4);
            }
        }
    }
    else
    {
        DEFAULT_LED_SET:
        pSetting.unit_ms = pLedPtr->ledtimeUnit;
        pSetting.duration_sel = pLedPtr->pwmDuration;
        pSetting.t0_cnt = pLedPtr->t0;
        pSetting.t1_cnt = pLedPtr->t1;
        pSetting.t2_cnt = pLedPtr->t2;
        pSetting.t3_cnt = pLedPtr->t3;
        pSetting.t1t2_repeat_cnt = pLedPtr->t1t2_repeat_cnt;
        //pSetting.nostop = pLedPtr->ledRepeatAlways; //for HW workaround
        pSetting.nostop = ((pLedPtr->t2 == 0 && pLedPtr->t3 == 0) || pLedPtr->noSync) ? pLedPtr->ledRepeatAlways : 0; //for HW workaround
        pSetting.pwm_en = (pLedPtr->pwmDuration != 0 ? TRUE : FALSE);
        pSetting.follow_idx= pLedPtr->ledFollowIndex;
        pSetting.inverse = pLedPtr->ledInversEnable;
        pSetting.pwm_onstep = pLedPtr->pwmOnStep;
        pSetting.pwm_offstep = pLedPtr->pwmOffStep;
        pSetting.pwm_period = pLedPtr->pwmPeriod;
    }
    
    //if(pLedPtr->ledRepeatAlways)
    //{
    //    gLoopTime = (pSetting.t0_cnt + (pSetting.t1_cnt + pSetting.t2_cnt) * (pSetting.t1t2_repeat_cnt + 1)+ pSetting.t3_cnt) * pSetting.unit_ms
    //}
    DRV_LED_Setting(ledIndex , &pSetting);
}

static U32 app_LED_CalculateRepeatTime(LED_SETTING_STRU *ledSet)
{
	return (ledSet->t0 + (ledSet->t1 + ledSet->t2) * (ledSet->t1t2_repeat_cnt + 1)+ ledSet->t3) * ledSet->ledtimeUnit;
}

static void app_LED_SetAllPattern(U8 changeMap, U8 type)
{
	U8 index;
	U8 ledOnOff;
    U8 changeMapBefore = changeMap;
    
    if(changeMap == 0)
        return;

    if(type == LED_TYPE_BG_RESUME && (gLedCtrl.currentType == LED_TYPE_BG_RESUME || gLedCtrl.currentType == LED_TYPE_FILTER || gLedCtrl.currentType == LED_TYPE_BG))
    {
        for(index = LED0 ; index < LED_NUM ; index++)
        {
            LED_SETTING_STRU *pLedPtr = GLED_CTL_PTR(index);
            if(pLedPtr->onOff == LED_ON || (pLedPtr->onOff >= LED_ALT_FIRST && pLedPtr->onOff <= LED_ALT_FIFTH))
            {
                if(pLedPtr->t2 == 0 && pLedPtr->t3 == 0)
                {
                    DBG_LOG_APP_LED("[APP_LED]SetAllPattern: always on led:%d", 1, (U8) index);
                    changeMap &= ~app_LED_GetBitPos(index);
                }
            }
        }        
    }
    
    if(type == LED_TYPE_BG)
    {
        for(index = LED0 ; index < LED_NUM ; index++)
        {
            LED_SETTING_STRU *pLedPtr = GLED_CTL_PTR(index);
            if(pLedPtr->onOff == LED_ON || (pLedPtr->onOff >= LED_ALT_FIRST && pLedPtr->onOff <= LED_ALT_FIFTH))
            {
                if(gLedCtrl.ledMsgCtl[index].ptnType == LED_FILTER_DATA && (pLedPtr->t2 == 0 && pLedPtr->t3 == 0))
                {
                    DBG_LOG_APP_LED("[APP_LED]SetAllPattern: filter always on led:%d", 1, (U8) index);
                    changeMap &= ~app_LED_GetBitPos(index);
                }                
            }
        }
    }
    
	ledOnOff = DRV_LED_Read_State();
    
    DBG_LOG_APP_LED("[APP_LED]SetAllPattern: changeMap:0x%x-->0x%x, ledOnOff:0x%x, type:%d", 4, changeMapBefore, (U8) changeMap, ledOnOff, type);
    
    gLedCtrl.currentType = type;

	// 1. Turn off the related leds first and release alt leds timer
	for (index = LED0; index < LED_NUM; index++)
	{
		if((changeMap & app_LED_GetBitPos(index)))
		{
			if(GLED_CTL_PTR(index)->onOff >= LED_ALT_FIRST && GLED_CTL_PTR(index)->onOff <= LED_ALT_FIFTH)
			{
				MSG_MessageCancelAllEx((Handler)&gLedHandler ,LED_HDL_ID_ALT_SCENARIO, GLED_CTL_PTR(index)->onOff);
			}

			MSG_MessageCancelAllEx((Handler)&gLedHandler ,LED_HDL_ID_REPEAT_TIMES, (LED_HDL_IDEXT_REPEAT_LED0 + index));
		}
	}

	DRV_LED_MultiLED_Stop(changeMap);
    ledOnOff &= ~changeMap;

	// 2. SetParameters
	for (index = LED0; index < LED_NUM; index++)
	{
		if((changeMap & app_LED_GetBitPos(index)))
		{
			if(GLED_CTL_PTR(index)->onOff == LED_ON || (GLED_CTL_PTR(index)->onOff >= LED_ALT_FIRST && GLED_CTL_PTR(index)->onOff <= LED_ALT_FIFTH))
			{
				U32 loopExtTime = app_LED_CalculateRepeatTime(GLED_CTL_PTR(index));

				App_LED_SetPatternToHW(index, GLED_CTL_PTR(index));
				if(GLED_CTL_PTR(index)->loopForExtendRepeat && (GLED_CTL_PTR(index)->onOff == LED_ON || GLED_CTL_PTR(index)->onOff == LED_ALT_FIRST))
				{
					DBG_LOG_APP_LED("[APP_LED] Set Repeat:%d,%d", 2, (U8) index,(U32) loopExtTime);
					gLedCtrl.ledMsgCtl[index].loopTimes =  GLED_CTL_PTR(index)->loopForExtendRepeat;
					FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_REPEAT_TIMES, NULL, LED_HDL_IDEXT_REPEAT_LED0 + index, loopExtTime);
				}
			}

			if(GLED_CTL_PTR(index)->onOff == LED_ON || GLED_CTL_PTR(index)->onOff == LED_ALT_FIRST)
			{
				if(GLED_FILTER_PTR != NULL && GLED_FILTER_PTR->filterType == FILTER_ADD)
				{
					DBG_LOG_APP_LED("[APP_LED] Set Add Time:%d,%d", 2, (U8) index,(U32) (app_LED_CalculateRepeatTime(GLED_CTL_PTR(index)) + GLED_FILTER_PTR->ledFilterAdd.gapWithAdd * GLED_CTL_PTR(index)->ledtimeUnit));
					FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ADD_SET_TIMER, NULL, LED_HDL_IDEXT_ADD_SET_LED0 + index,
					(app_LED_CalculateRepeatTime(GLED_CTL_PTR(index)) + GLED_FILTER_PTR->ledFilterAdd.gapWithAdd * GLED_CTL_PTR(index)->ledtimeUnit));
				}
				ledOnOff |= app_LED_GetBitPos(index);
			}

			if(GLED_CTL_PTR(index)->onOff == LED_ALT_FIRST)
			{
				if(GLED_CTL_PTR(index)->t4)
				{
					DBG_LOG_APP_LED("[APP_LED] Set T4 Time:%d,%d", 2, (U8) index,(U32) (GLED_CTL_PTR(index)->t4 * GLED_CTL_PTR(index)->ledtimeUnit));
					FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ALT_SCENARIO, NULL, LED_HDL_IDEXT_ALT_LED0 + index, (GLED_CTL_PTR(index)->t4 * GLED_CTL_PTR(index)->ledtimeUnit));
				}
			}
		}
	}

	// 3. LEDs turn on
	DRV_LED_MultiLED_Start(ledOnOff);
}

static void app_LED_ResumeBG(void)
{
	U8 index;
	U8 ledOnOff = DRV_LED_Read_State();
    BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	if((!APP_PowerOff_IsPowerOn()) && (APP_ChgBat_GetBatStatus()< APP_BAT_CHARGER_IN))
	{
		return;
	}

	for(index = LED0 ; index < LED_NUM ; index++)
	{
		//if(gLedCtrl.ledMsgCtl[index].resumeBGPtn != NULL && (gLedCtrl.ledMsgCtl[index].ptnType == LED_FG_DATA || gLedCtrl.ledMsgCtl[index].ptnType == LED_FILTER_DATA)) //mark for BG can resume again
		{
			if(gLedCtrl.pFilterPtn && ((LED_FILTER_PATTERN_STRUC *)gLedCtrl.pFilterPtn)->filterType == FILTER_USER_DEF &&
				((LED_FILTER_PATTERN_STRUC *)gLedCtrl.pFilterPtn)->ledSettings[index].onOff != LED_DONT_CARE) //if it is filter, light the filter pattern
			{
				gLedCtrl.ledMsgCtl[index].ptnType = LED_FILTER_DATA;
				gLedCtrl.ledMsgCtl[index].pattern = (U8 *)&((LED_FILTER_PATTERN_STRUC *)gLedCtrl.pFilterPtn)->ledSettings[index];
				ledOnOff |= app_LED_GetBitPos(index);
			}
			else //if it is not filter, resume BG
			{
				gLedCtrl.ledMsgCtl[index].ptnType = LED_BG_DATA;
				gLedCtrl.ledMsgCtl[index].pattern = gLedCtrl.ledMsgCtl[index].resumeBGPtn;
				ledOnOff |= app_LED_GetBitPos(index);
			}
		}
	}
	DBG_LOG_APP_LED("[APP_LED] Resume: LedOnOff:0x%x", 1, ledOnOff);
	app_LED_SetAllPattern(ledOnOff, LED_TYPE_BG_RESUME);
    
    for(index = LED0 ; index < LED_NUM ; index++)
    {
        if(GLED_CTL_PTR(index)->onOff == LED_ON || GLED_CTL_PTR(index)->onOff == LED_ALT_FIRST)
        {
            if(!GLED_CTL_PTR(index)->noSync)
            {
                U32 loopTime = app_LED_CalculateRepeatTime(GLED_CTL_PTR(index));

                if(BtAwsMce_IsWithPartner(pServiceBdAddr))
                {
                    if(loopTime > LED_SYNC_TIME)
                        loopTime -= LED_SYNC_TIME;
                    else
                        loopTime = 0;
                }
                FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, 0);
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, NULL, 0, loopTime);
                DBG_LOG_APP_LED("[APP_LED] LED_HDL_ID_RESYNC_TIMER set 3 ======== %d", 1, loopTime);
                break;
            }
        }
    }
}

static void app_LED_ResumeBGHandler(void)
{
	BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	if(BtAwsMce_IsDefaultRolePartner())
	{
		if(!PM_IsProfileConnected(pServiceBdAddr, PROFILE_MCSYNC))
		{
			app_LED_ResumeBG();
		}

		return;
	}

	if(BtAwsMce_IsWithPartner(pServiceBdAddr))
	{
		app_LED_PushAction(MCSYNC_LED_AGENT_SET_RESUME_BG);
		BtAwsMce_GetCurrentPiconetBtClock(pServiceBdAddr, PICO_EVENT_LED);
	}
	else
	{
		app_LED_ResumeBG();
	}
}

static void app_LED_SetBgPattern(U8 ptnIndex)
{
	U8 ledIndex;
	U8 ledOnOff = 0;
	LED_SETTING_STRU *pLedSet;
    BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	if(!gLedCtrl.isDataOk || ptnIndex > LED_BG_NUM)
		return;

	if((pLedSet = (LED_SETTING_STRU *)app_LED_ReadPattern(LED_BG_DATA, ptnIndex)) == NULL)
		return;

	for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
	{
		if(pLedSet[ledIndex].onOff != LED_DONT_CARE)
		{
			if(app_LED_IsPtnCanBeChanged(gLedCtrl.ledMsgCtl[ledIndex].ptnType, LED_BG_DATA))
			{
				gLedCtrl.ledMsgCtl[ledIndex].ptnType = LED_BG_DATA;
				gLedCtrl.ledMsgCtl[ledIndex].pattern = (U8 *)(pLedSet + ledIndex);
				ledOnOff |= app_LED_GetBitPos(ledIndex);
			}

			gLedCtrl.ledMsgCtl[ledIndex].resumeBGPtn = (U8 *)(pLedSet + ledIndex); //for resuming BG again while in BG LED mode
		}
	}
	DBG_LOG_APP_LED("[APP_LED] BG: ptnIdx:%d, LedOnOff:0x%x", 2, ptnIndex, ledOnOff);
	app_LED_SetAllPattern(ledOnOff, LED_TYPE_BG);
    
    for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
    {
        if(GLED_CTL_PTR(ledIndex)->onOff == LED_ON || GLED_CTL_PTR(ledIndex)->onOff == LED_ALT_FIRST)
        {
            if(!GLED_CTL_PTR(ledIndex)->noSync)
            {
                U32 loopTime = app_LED_CalculateRepeatTime(GLED_CTL_PTR(ledIndex));

                if(BtAwsMce_IsWithPartner(pServiceBdAddr))
                {
                    if(loopTime > LED_SYNC_TIME)
                        loopTime -= LED_SYNC_TIME;
                    else
                        loopTime = 0;
                }
                FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, 0);
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, NULL, 0, loopTime);
                DBG_LOG_APP_LED("[APP_LED] LED_HDL_ID_RESYNC_TIMER set 1 ======== %d", 1, loopTime);
                break;
            }
        }
    }
}

static void app_LED_SetFgPattern(U8 ptnIndex, U8 overwriteTime, BOOL isOverWrite)
{
	U8 ledIndex;
	U8 ledOnOff = 0;
	LED_SETTING_STRU *pLedSet;

	if(!gLedCtrl.isDataOk || ptnIndex > LED_FG_NUM)
		return;

	if((pLedSet = (LED_SETTING_STRU *)app_LED_ReadPattern(LED_FG_DATA, ptnIndex)) == NULL)
		return;

	for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
	{
		if(pLedSet[ledIndex].onOff != LED_DONT_CARE )
		{
			if(app_LED_IsPtnCanBeChanged(gLedCtrl.ledMsgCtl[ledIndex].ptnType, LED_FG_DATA) || isOverWrite)
			{
				if(gLedCtrl.ledMsgCtl[ledIndex].ptnType == LED_BG_DATA)
					gLedCtrl.ledMsgCtl[ledIndex].resumeBGPtn = gLedCtrl.ledMsgCtl[ledIndex].pattern;

				gLedCtrl.ledMsgCtl[ledIndex].ptnType = LED_FG_DATA;
				gLedCtrl.ledMsgCtl[ledIndex].pattern = (U8 *)(pLedSet + ledIndex);
				ledOnOff |= app_LED_GetBitPos(ledIndex);
			}
		}
	}
	if(overwriteTime && ledOnOff && (BtAwsMce_IsDefaultRoleAgent() || (BtAwsMce_IsDefaultRolePartner() && !PM_IsProfileConnected(APP_GetServiceBdAddr(), PROFILE_MCSYNC))) )
    {
		FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_FG_TIMER, NULL, 0, overwriteTime * MILLI_SEC_100);
        FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, 0);
        DBG_LOG_APP_LED("[APP_LED] LED_HDL_ID_RESYNC_TIMER release ========", 0);
    }
	DBG_LOG_APP_LED("[APP_LED] FG: ptnIdx:%d, LedOnOff:0x%x", 2, ptnIndex, ledOnOff);
	app_LED_SetAllPattern(ledOnOff, LED_TYPE_FG);
}

static U32 app_LED_Handler(Handler handler, U16 id, void *payload, U32 id_ext)
{
	U8 ledIndex;
	U8 ledOnOff;
	U8 ledID = id_ext;
	U8 nextALT = LED_NUM;
	U32 loopExtTime;

	UNUSED(handler);
	UNUSED(payload);

	if(!gLedCtrl.isDataOk)
		return 0;

	DBG_LOG_APP_LED("[APP_LED] HDL:%d,%d", 2, (U8) id,(U8) id_ext);
	switch(id)
	{
		case LED_HDL_ID_REPEAT_TIMES:
			if(gLedCtrl.ledMsgCtl[ledID].loopTimes)
			{
				gLedCtrl.ledMsgCtl[ledID].loopTimes--;

				DBG_LOG_APP_LED("[APP_LED] Set Repeat Time3:%d,%d", 2, (U8) ledID,(U32) app_LED_CalculateRepeatTime(GLED_CTL_PTR(ledID)));
				FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_REPEAT_TIMES, NULL, ledID, app_LED_CalculateRepeatTime(GLED_CTL_PTR(ledID)));

				ledOnOff = DRV_LED_Read_State();
				DRV_LED_MultiLED_Stop(app_LED_GetBitPos(ledID));
				ledOnOff |= app_LED_GetBitPos(ledID);
				DRV_LED_MultiLED_Start(ledOnOff);
			}
			break;
		case LED_HDL_ID_ALT_SCENARIO:
            for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
            {
                if(GLED_CTL_PTR(ledIndex)->onOff == (GLED_CTL_PTR(ledID)->onOff + 1))
                    nextALT = ledIndex;
            }

            if(nextALT == LED_NUM)
                return 0;

            ledOnOff = DRV_LED_Read_State();
            DRV_LED_MultiLED_Stop(app_LED_GetBitPos(nextALT));
            ledOnOff |= app_LED_GetBitPos(nextALT);
            DRV_LED_MultiLED_Start(ledOnOff);

            if(GLED_CTL_PTR(nextALT)->t4 != 0)
            {
                DBG_LOG_APP_LED("[APP_LED] Set T4 Time2:%d,%d", 2, (U8) nextALT, (U32) (GLED_CTL_PTR(nextALT)->t4 * GLED_CTL_PTR(nextALT)->ledtimeUnit));
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ALT_SCENARIO, NULL, LED_HDL_IDEXT_ALT_LED0 + nextALT, (GLED_CTL_PTR(nextALT)->t4 * GLED_CTL_PTR(nextALT)->ledtimeUnit));
            }

            loopExtTime = app_LED_CalculateRepeatTime(GLED_CTL_PTR(nextALT));
            if(GLED_CTL_PTR(nextALT)->loopForExtendRepeat != 0)
            {
                DBG_LOG_APP_LED("[APP_LED] Set Repeat Time2:%d,%d", 2, (U8) nextALT, (U32) loopExtTime * GLED_CTL_PTR(nextALT)->loopForExtendRepeat);
                gLedCtrl.ledMsgCtl[nextALT].loopTimes = GLED_CTL_PTR(nextALT)->loopForExtendRepeat;
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_REPEAT_TIMES, NULL, LED_HDL_IDEXT_REPEAT_LED0 + nextALT, loopExtTime * GLED_CTL_PTR(nextALT)->loopForExtendRepeat);
            }
            else if(GLED_CTL_PTR(ledID)->ledRepeatAlways == 1)
            {
                DBG_LOG_APP_LED("[APP_LED] Set T4 Time3:%d,%d", 2, (U8) ledID, (U32) (GLED_CTL_PTR(ledID)->t4 * GLED_CTL_PTR(ledID)->ledtimeUnit));
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_ALT_SCENARIO, NULL, ledID, loopExtTime);
            }
			break;
		case LED_HDL_ID_FG_TIMER:
			DBG_LOG_APP_LED("[APP_LED] Resume BG=============", 0);
			if(BtAwsMce_IsDefaultRolePartner() && PM_IsProfileConnected(APP_GetServiceBdAddr(), PROFILE_MCSYNC))
			{
				APP_MCSYNC_SendSyncLEDInfo(0, NULL, 0, 0, 0, MCSYNC_LED_PARTNER_ASK_RESUME_BG);
			}
			else
			{
				app_LED_ResumeBGHandler();
			}
			break;
        case LED_HDL_ID_RESYNC_TIMER:
            app_LED_ResumeBGHandler();
            break;
        case LED_HDL_ID_ADD_SET_TIMER:
            {
                LED_SETTING_STRU ledCtl;
                ledCtl.t0 = 0;
                ledCtl.t1 = GLED_FILTER_PTR->ledFilterAdd.t1;
                ledCtl.t2 = GLED_FILTER_PTR->ledFilterAdd.t2;
                ledCtl.t1t2_repeat_cnt = GLED_FILTER_PTR->ledFilterAdd.t12Rpt;
                ledCtl.ledtimeUnit = GLED_CTL_PTR(ledID)->ledtimeUnit;
                ledCtl.pwmDuration = GLED_FILTER_PTR->ledFilterAdd.pwmDuration;
                ledCtl.t3 = 0;
                ledCtl.ledRepeatAlways = 0;
                ledCtl.ledFollowIndex = 0;
                ledCtl.ledInversEnable = 0;

                ledOnOff = DRV_LED_Read_State();
                DRV_LED_MultiLED_Stop(app_LED_GetBitPos(ledID));
                App_LED_SetPatternToHW(ledID, &ledCtl);
                ledOnOff |= app_LED_GetBitPos(ledID);
                DRV_LED_MultiLED_Start(ledOnOff);
            }
            break;

		case LED_HDL_ID_SYNC_BG_TIMER:
			app_LED_SetBgPattern(gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_BG].ptnIndex);
			break;
		case LED_HDL_ID_SYNC_FG_TIMER:
			app_LED_SetFgPattern(gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].ptnIndex, gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].overwriteTime, gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_FG].isOverWrite);
			break;
		case LED_HDL_ID_SYNC_RESUMEBG_TIMER:
			if(BtAwsMce_IsDefaultRolePartner())
			{
				for(ledIndex = LED0; ledIndex < LED_NUM; ledIndex++)
				{
					gLedCtrl.ledMsgCtl[ledIndex].ptnType = gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_RESUME_BG].ptnType[ledIndex];
				}
			}
			app_LED_ResumeBG();
			break;
	}

	return 0;
}

void APP_LED_SetBgPattern(U8 ptnIndex)
{
	BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

    DBG_LOG_APP_LED("[APP_LED] APP_LED_SetBgPattern:%x", 1, ptnIndex);
	if(BtAwsMce_IsDefaultRolePartner())
	{
		if(!PM_IsProfileConnected(pServiceBdAddr, PROFILE_MCSYNC))
		{
			app_LED_SetBgPattern(ptnIndex);
		}

		return;
	}

	gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_BG].ptnIndex = ptnIndex;

	if(BtAwsMce_IsWithPartner(pServiceBdAddr))
	{
		app_LED_PushAction(MCSYNC_LED_AGENT_SET_BG);
		BtAwsMce_GetCurrentPiconetBtClock(pServiceBdAddr, PICO_EVENT_LED);
	}
	else
	{
		app_LED_SetBgPattern(ptnIndex);
	}
}

void APP_LED_SetFgParameter(U16 fgEvent, U8 ptnIndex, U8 overwriteTime, BOOL isOverWrite)
{
	if(app_LED_IsFgSyncNeeded(fgEvent))
	{
		app_LED_StartFgSync(ptnIndex, overwriteTime, isOverWrite);
	}
	else
	{
		app_LED_SetFgPattern(ptnIndex, overwriteTime, isOverWrite);
	}
}

void APP_LED_SetFilterPattern(U8 ptnIndex)
{
	U8 filterIdx, ledIndex;
	U8 ledOnOff = 0;
	LED_SETTING_STRU *tempPtn;
    BD_ADDR_T * pServiceBdAddr = (BD_ADDR_T *)APP_GetServiceBdAddr();

	LED_FILTER_PATTERN_STRUC *pLedSet;

	if(!gLedCtrl.isDataOk || ptnIndex > LED_FILTER_NUM)
		return;

	if((pLedSet = (LED_FILTER_PATTERN_STRUC *)app_LED_ReadPattern(LED_FILTER_DATA, ptnIndex)) == NULL)
		return;

	gLedCtrl.pFilterPtn = (U8 *)pLedSet;

	for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
	{
		if(gLedCtrl.ledMsgCtl[ledIndex].ptnType == LED_BG_DATA)
		{
			gLedCtrl.ledMsgCtl[ledIndex].resumeBGPtn = gLedCtrl.ledMsgCtl[ledIndex].pattern;
		}
	}

	if(pLedSet->filterType == FILTER_SWAP_CHANGE)
	{
		if(pLedSet->ledSwapChange.isSwap)
		{
			tempPtn = GLED_CTL_PTR(pLedSet->ledSwapChange.swapTag1);
			gLedCtrl.ledMsgCtl[pLedSet->ledSwapChange.swapTag1].pattern = (U8 *)GLED_CTL_PTR(pLedSet->ledSwapChange.swapTag2);
			gLedCtrl.ledMsgCtl[pLedSet->ledSwapChange.swapTag2].pattern = (U8 *)tempPtn;
			ledOnOff |= app_LED_GetBitPos(pLedSet->ledSwapChange.swapTag1);
			ledOnOff |= app_LED_GetBitPos(pLedSet->ledSwapChange.swapTag2);
		}
		else if(pLedSet->ledSwapChange.isChange)
		{
			gLedCtrl.ledMsgCtl[pLedSet->ledSwapChange.changeTag].pattern = (U8 *)GLED_CTL_PTR(pLedSet->ledSwapChange.changeSrc);
			ledOnOff |= app_LED_GetBitPos(pLedSet->ledSwapChange.changeTag);
		}

	}
	else if(pLedSet->filterType == FILTER_USER_DEF || pLedSet->filterType == FILTER_MULTI_DIVIDE || pLedSet->filterType == FILTER_ADD)
	{
		for(filterIdx = LED0 ; filterIdx < LED_NUM ; filterIdx++)
		{
			if(pLedSet->filterType == FILTER_USER_DEF )
			{
				if(pLedSet->ledSettings[filterIdx].onOff != LED_DONT_CARE)
				{
					gLedCtrl.ledMsgCtl[filterIdx].ptnType = LED_FILTER_DATA;
					gLedCtrl.ledMsgCtl[filterIdx].pattern = (U8 *)&pLedSet->ledSettings[filterIdx];
					ledOnOff |= app_LED_GetBitPos(filterIdx);
				}
			}
			else
			{
				ledOnOff |= app_LED_GetBitPos(filterIdx);
			}
		}
	}
	DBG_LOG_APP_LED("[APP_LED] Filter: type:%d, ptnIdx:%d, LedOnOff:0x%x", 3, pLedSet->filterType, ptnIndex, ledOnOff);
	app_LED_SetAllPattern(ledOnOff, LED_TYPE_FILTER);
    
    for(ledIndex = LED0 ; ledIndex < LED_NUM ; ledIndex++)
    {
        if(GLED_CTL_PTR(ledIndex)->onOff == LED_ON || GLED_CTL_PTR(ledIndex)->onOff == LED_ALT_FIRST)
        {
            if(!GLED_CTL_PTR(ledIndex)->noSync)
            {
                U32 loopTime = app_LED_CalculateRepeatTime(GLED_CTL_PTR(ledIndex));

                if(BtAwsMce_IsWithPartner(pServiceBdAddr))
                {
                    if(loopTime > LED_SYNC_TIME)
                        loopTime -= LED_SYNC_TIME;
                    else
                        loopTime = 0;
                }
                FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, 0);
                FW_SetTimer((Handler)&gLedHandler, LED_HDL_ID_RESYNC_TIMER, NULL, 0, loopTime);
                DBG_LOG_APP_LED("[APP_LED] LED_HDL_ID_RESYNC_TIMER set 2 ======== %d", 1, loopTime);
                break;
            }
        }
    }
}

BOOL APP_LED_IsFGLedOn(void)
{
	return (MSG_MessageSearchAllEx((Handler)&gLedHandler, LED_HDL_ID_FG_TIMER, 0) > 0) ? TRUE : FALSE;
}

void APP_LED_Init(void)
{
	if(app_LED_ReadNumber() && app_LED_ReadDataIDNvkey())
	{
		gLedCtrl.isDataOk = TRUE;
		DBG_LOG_APP_LED("[APP_LED] LED:%d,%d,%d", 3, (U8) LED_BG_NUM, (U8) LED_FG_NUM, (U8) LED_FILTER_NUM);
	}
	else
		DBG_LOG_APP_LED("[APP_LED] Init Fail:%d,%d", 2, (U8) app_LED_ReadNumber(),(U8) app_LED_ReadDataIDNvkey());

	app_LED_ReadStateBGNvkey();
}

void APP_LED_CancelFilter(U8 ptnIndex)
{
	LED_FILTER_PATTERN_STRUC *pLedSet;
	U8 index;

	if(!gLedCtrl.isDataOk || ptnIndex > LED_FILTER_NUM)
		return;

	if((pLedSet = (LED_FILTER_PATTERN_STRUC *)app_LED_ReadPattern(LED_FILTER_DATA, ptnIndex)) == NULL)
		return;

	if(GLED_FILTER_PTR == pLedSet)
	{
		DBG_LOG_APP_LED("[APP_LED] Cancel Filter: ptnIdx:%d", 1, ptnIndex);
		gLedCtrl.pFilterPtn = NULL;
		for(index = LED0 ; index < LED_NUM ; index++)
		{
			gLedCtrl.ledMsgCtl[index].ptnType = LED_BG_DATA;
			gLedCtrl.ledMsgCtl[index].pattern = gLedCtrl.ledMsgCtl[index].resumeBGPtn;
		}

		app_LED_ResumeBGHandler();

		if(PM_IsProfileConnected((BD_ADDR_T *)APP_GetServiceBdAddr(), PROFILE_MCSYNC))
		{
			APP_MCSYNC_SendSyncLEDInfo(0, NULL, 0, 0, 0, MCSYNC_LED_PARTNER_ASK_RESUME_BG); //partner tell agent to resume BG again
		}
	}
}


void APP_LED_SyncParameter(U8 ptnIndex, U8 *ptnType, U8 overwriteTime, BOOL isOverWrite, U32 clockToStart, U8 action)
{
	U8 ledIndex;

	for(ledIndex = LED0; ledIndex < LED_NUM; ledIndex++)
	{
		if(ptnType[ledIndex] != LED_NUM_DATA)
		{
			gLedCtrl.awsCtl[action].ptnType[ledIndex] = ptnType[ledIndex];
		}
		else
		{
			gLedCtrl.awsCtl[action].ptnType[ledIndex] = gLedCtrl.ledMsgCtl[ledIndex].ptnType;
		}
	}

	gLedCtrl.awsCtl[action].ptnIndex = ptnIndex;
	gLedCtrl.awsCtl[action].overwriteTime = overwriteTime;
	gLedCtrl.awsCtl[action].isOverWrite = isOverWrite;
	gLedCtrl.awsCtl[action].clockToStart = clockToStart;
}

void APP_LED_SetSyncTimer(U32 curPicoClock)
{
	U32 timeToWait;
	U8 curAction = app_LED_PopAction();

	if(BtAwsMce_IsDefaultRolePartner())
	{
		if(!PM_IsProfileConnected((BD_ADDR_T *)APP_GetServiceBdAddr(), PROFILE_MCSYNC))
		{
			return;
		}

		timeToWait = ((gLedCtrl.awsCtl[curAction].clockToStart - curPicoClock) * HALF_SLOT) / 1000; //ms
		if(timeToWait > LED_SYNC_TIME)
		{
			timeToWait = 0;
		}
        DBG_LOG_APP_LED("[APP_LED] SetSyncTimer: curAction:%d, clockToStart:%d, curPicoClock:%d, timeToWait", 4, curAction, gLedCtrl.awsCtl[curAction].clockToStart, curPicoClock, timeToWait);
	}
	else
	{
		timeToWait = LED_SYNC_TIME;
		app_LED_SendSyncInfo(timeToWait, curPicoClock, curAction);
        DBG_LOG_APP_LED("[APP_LED] SetSyncTimer2: curAction:%d, curPicoClock:%d, timeToWait", 3, curAction, curPicoClock, timeToWait);
	}

	FW_SetTimer((Handler)&gLedHandler, gLedActionMapTimer[curAction], NULL, 0, timeToWait);
}

void APP_LED_PushAction(U8 action)
{
	app_LED_PushAction(action);
}

void APP_LED_SyncAllInfo(void)
{
	FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_SYNC_BG_TIMER, 0);
	APP_LED_SetBgPattern(gLedCtrl.awsCtl[MCSYNC_LED_AGENT_SET_BG].ptnIndex);
}

void APP_LED_ReleaseSyncTimer(void)
{
	FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_SYNC_FG_TIMER, 0);
	FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_SYNC_BG_TIMER, 0);
	FW_ReleaseTimer((Handler)&gLedHandler, LED_HDL_ID_SYNC_RESUMEBG_TIMER, 0);
}

U8 APP_LED_GetDefaultIndex(U8 state)
{
	return pStateBGLed->defaultLedSetting[state].entryLedDisplayIndex;
}

U8 APP_LED_GetMaxLinkIndex(void)
{
	return pStateBGLed->maxLinkLedSetting.entryLedDisplayIndex;
}

void APP_LED_ResumeBGHandler(void)
{
	app_LED_ResumeBGHandler();
}
