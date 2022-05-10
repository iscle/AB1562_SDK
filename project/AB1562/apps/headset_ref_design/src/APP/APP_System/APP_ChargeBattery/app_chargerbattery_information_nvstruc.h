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
 
 
#ifndef _CHARGER_BATTERY_INFO_H_ 
#define	_CHARGER_BATTERY_INFO_H_

/**************************************************************************************************
* Define
**************************************************************************************************/
#define APP_CHARGER_OUT_THEN_POWER_ON_FEAT		0x0001
#define APP_INIT_BAT_CHECK_ON_POWER_UP_FEAT		0x0002
#define APP_POWER_OFF_WHEN_CHARGING_TO_FEAT		0x0004
#define APP_POWER_OFF_WHEN_CHARGING_IN_INTR		0x0008

/**************************************************************************************************
* Enum
**************************************************************************************************/
enum
{
	ACTION_NORMAL,
	ACTION_POWER_ON_PROHIBITED,
	ACTION_POWER_ON_NO_CONNECTIVITY_UNTIL_BAT_OK,
	ACTION_POWER_ON_NO_VALID_KEYS_UNTIL_BAT_OK,
	ACTION_POWER_ON_NO_CONNECTIVITY_NO_VALID_KEYS_UNTIL_BAT_OK,
};

enum
{
	TIMER_ID_AUTO_SWITCH_OFF_IN_LOW_BAT,	//autoSwitchOffInLowBatTimer			
	TIMER_ID_DELAY_CHG_MEDIA_EVT,			//delayChgMediaEvtTimer;				
	TIMER_ID_CHECK_CHARGING_TIME,			//checkChargingTimer	
	TIMER_ID_BATTERY_MONITOR_PERIOD			//batteryMonitoringPeriod	
};

/**************************************************************************************************
* Structure
**************************************************************************************************/
/**
 *
 * @KeyID 0xF21F
 * 
 */
 
typedef struct
{
	U16 checkPoint1;
	U16 checkPoint2;
	U16 checkPoint3;
}BATTERY_CHECK_POINT_STRU;
 
typedef struct
{
	struct {
		U8 level0;
		U8 level1;
		U8 level2;
		U8 level3;
	} ledDuration;
	U8 lowPercentLEDTimer;
	U8 lowPercentRingTimer;
	BATTERY_CHECK_POINT_STRU checkPoint;
}BATTERY_PARA_STRU;//BatteryPara

typedef struct
{
	U8 pwrOnLowPercent;
	U8 pwrOnLowPercentChgIn;
}APP_PWR_ON_LOW_PERCENT_CTL_STRU;//MMIPowerOnInLowBatCtlType

typedef struct
{
	U8 autoSwitchOffInLowBatTimer;			//unit: 1min
	U8 delayChgMediaEvtTimer;				//unit: 10ms		
	U8 checkChargingTimer;	      			//unit: 0.5 hour (30 mins)    
	U8 batteryMonitoringPeriod;				//unit: 1sec
}APP_BAT_TIMER_CTL_STRU;

typedef struct
{
	U16 initPercent;					
	U16 shutdownPercent;
	U16 lowPercent;
}APP_BAT_THRESHOLD_STRU;

/*NvkeyDefine NVKEYID_APP_BAT_CHG_INFORMATION*/
typedef struct
{
	BATTERY_PARA_STRU batteryPara;
	APP_PWR_ON_LOW_PERCENT_CTL_STRU pwrOnLowPercentAct;
	APP_BAT_TIMER_CTL_STRU timer;
	APP_BAT_THRESHOLD_STRU percentThld;
	U16 supportedFeature;
}PACKED APP_CHGBAT_INFO_STRU;

/*NvkeyDefine NVKEYID_APP_CHGCASE*/
typedef struct APP_CASE_INFO_STRU
{
	U8		chgCaseEnable;
	U8		inputGPIO;
	U8		outputGPIO;
	BOOL	mmiTimerEnable;
	U16		mmiTime;
}PACKED APP_CASE_INFO_STRU;
 
/**************************************************************************************************
* Variable
**************************************************************************************************/
/*
APP_CHGBAT_INFO_STRU ChgBat_Info = 
{
	{//batteryPara
		{//ledDuration
			10, 10, 9, 9		
		},
		20, //Low Percent LED Timer
		20,	//Low Percent Ring Timer
	
		{//checkPoint
			250, 500, 750
		},
	},
	
	{//pwrOnLowPercentAct
		ACTION_NORMAL, //pwrOnLowPercent
		ACTION_NORMAL, //pwrOnLowPercentChgIn
	},
	
	{//timer
		10,			//U8 autoSwitchOffInLowBatTimer 				
		3,			//U8 delayChgMediaEvtTimer: 					
		0,			//U8 checkChargingTimer: 							
		30			//U8 batteryMonitoringPeriod 					
	},
	
	{//Threshold
		150,		//initPercent
		70,			//shutdownPercent
		200,		//lowPercent
	},
	
	7				//supportedFeature
};
*/


#endif
