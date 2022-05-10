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
 
 
#ifndef __LED_NVKEY_ID_PATTERN__
#define __LED_NVKEY_ID_PATTERN__

/*
LED Number Define
*/
enum
{
	LED0,
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
	LED_NUM,
};

/*NvkeyDefine NVKEYID_APP_LED_BG_PATTERN */
/*NvkeyDefine NVKEYID_APP_LED_FG_PATTERN */
typedef struct
{

	U8 ledtimeUnit:7; //1ms ~ 64 ms
	U8 ledRepeatAlways:1; //Enable = 1, Disable = 0,
	U8 reserved:3;
    U8 noSync:1;
	U8 ledFollowIndex:3; //LED0 ~ LED5
	U8 ledInversEnable:1;
	U8 t0; //unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led T0 Setting */
	U8 t1; //unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led T1 Setting */
	U8 t2; //unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led T2 Setting */
	U8 t3; //unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led T3 Setting */
	U8 t1t2_repeat_cnt; //unit:times, Val:0, Range:0-0x0F		/**< @Value 0x00	@Desc Led T1T2 repeat times Setting */
	U8 pwmDuration; ///unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc PWM duration Setting
//////////////Sw Setting////////////////////
	U8 onOff; /**< @Value 0x00	@Desc Led OnOff Type Setting */
	U8 t4; //unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led T4 for ALT Type Setting */
	U8 loopForExtendRepeat; //unit:times, Val:0, Range:0-0x0F		/**< @Value 0x00	@Desc Led Total repeat times Setting */
	U8 pwmOnStep; ///unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc PWM On Step Setting
	U8 pwmOffStep; ///unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc PWM Off Step Setting
	U8 pwmPeriod; ///unit:count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc PWM Denominator of Step Setting
}LED_SETTING_STRU;

typedef struct
{
	U8 isMultiPly;	/**< @Value 0x00	@Desc Led Filter Multi Enable */
	U8 isDivide;	/**< @Value 0x00	@Desc Led Filter Divide Enable */
	U8 t0;			//unit:rate, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led filter t0 rate */
	U8 t1;			//unit:rate, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led filter t1 rate */
	U8 t2;			//unit:rate, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led filter t2 rate */
	U8 t3;			//unit:rate, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led filter t3 rate */
	U8 t12Rpt;		//unit:rate, Val:0, Range:0-0x0F		/**< @Value 0x00	@Desc Led T12 repeat rate */
	U8 t4;			//unit:rate, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led filter t3 rate */
}LED_FILTER_MULTI_DIVIDE_STRU;

typedef struct
{
	U8 isSwap;		/**< @Value 0x00	@Desc Led Filter Swap Enable */
	U8 swapTag1;	//unit:Led Index, Val:0, Range:0-0x05		/**< @Value 0x00	@Desc Led Filter Swap target 1 */
	U8 swapTag2;	//unit:Led Index, Val:0, Range:0-0x05		/**< @Value 0x00	@Desc Led Filter Swap target 2 */
	U8 isChange;	/**< @Value 0x00	@Desc Led Filter Change Enable */
	U8 changeSrc;	//unit:Led Index, Val:0, Range:0-0x05		/**< @Value 0x00	@Desc Led Filter Change Source */
	U8 changeTag;	//unit:Led Index, Val:0, Range:0-0x05		/**< @Value 0x00	@Desc Led Filter Change Target */
}LED_FILTER_SWAP_CHANGE_STRU;

typedef struct
{
	U8 tagLedForAdd;	//unit:Led Index, Val:0, Range:0-0x05		/**< @Value 0x00	@Desc Led Filter Add target */
	U8 gapWithAdd;		//unit:Count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led Filter Add Gap */
	U8 t1;				//unit:Count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led Filter Add T1 */
	U8 t2;				//unit:Count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led Filter Add T2 */
	U8 t12Rpt;			//unit:Times, Val:0, Range:0-0x0F		/**< @Value 0x00	@Desc Led Filter Add T1T2 repeat times */
	U8 pwmDuration;		//unit:Count, Val:0, Range:0-0xFF		/**< @Value 0x00	@Desc Led Filter Add PWM duration */
}LED_FILTER_ADD_STRU;

typedef struct
{
	LED_SETTING_STRU 	ledSettings[LED_NUM];
}LED_PATTERN_STRU;

/*NvkeyDefine NVKEYID_APP_LED_FILTER_PATTERN */
typedef struct
{
	U8					filterType;
	union
	{
		LED_SETTING_STRU 				ledSettings[LED_NUM];
		LED_FILTER_MULTI_DIVIDE_STRU	ledMultiDivide[LED_NUM];
		LED_FILTER_SWAP_CHANGE_STRU		ledSwapChange;
		LED_FILTER_ADD_STRU				ledFilterAdd;
	};
}PACKED LED_FILTER_PATTERN_STRUC; //__LED_NVKEY_ID_PATTERN__

#endif

