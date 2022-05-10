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
#include "App_ResourceManager.h"
#include "App_State.h"
#include "App_EventOpcode.h"
#include "App_TestMode.h"
#include "App_Interface.h"
#include "App_LedControl.h"
#include "led_nvkey_pattern_nvstruc.h"
#include "App_TestMode.h"
#include "App_PowerOff.h"
#include "Power_NvkeyHandler.h"
#include "syslog.h"

log_create_module(APP_TEST_MODE, PRINT_LEVEL_INFO);

static U32 app_TestModeHandler(Handler t, U16 id, void* msg, U32 id_ext);


void APP_TestModeEntry(BD_ADDR_T *pBdAddr);
void APP_TestModeExit(BD_ADDR_T *pBdAddr);
void APP_TestMode_StartLabTest(void);
void APP_TxSingleTone(U8 channel, U16 power);
void APP_TxContinuePacket(U8 channel, U16 power);
void APP_TxBurstPacket(U8 channel, U16 power);
void APP_RxTest(U8 channel, U16 power);
U16 APP_LabTest_TransferPowerToGC1GC2 (U16 power);
void APP_TestMode_LedHandler(U8 type, U8 repeat);
void APP_TestMode_Execute(void);
void APP_TestMode_KeyEnterTestMode(BD_ADDR_T *pBdAddr);
void APP_TestMode_KeyTestModeChannel(BD_ADDR_T *pBdAddr);
void APP_TestMode_KeyTestModePower(BD_ADDR_T *pBdAddr);
void APP_TestMode_KeyTestModeState(BD_ADDR_T *pBdAddr);
void APP_TestMode_KeyTestModeTxPktType(BD_ADDR_T *pBdAddr);

U8 const TestModeChannelMap[] = {0, 39, 78};
U8 const TestModePowerDiffMap[] = {0, 1, 2, 3, 4, 7, 10, 13, 16, 19}; // {X,X-1,X-2,X-3,X-4,X-7,X-10,X-13,X-16,X-19}

U8 const defaultBdAddr[6] = { 0x7E,0x96,0xC6,0x6B,0x00,0x00 };

static const HandlerData appTestModeHandle = { app_TestModeHandler};

AppTestModeInfo  gAPP_TestMode_ctl;
static U32 app_TestModeHandler(Handler app, U16 id, void* msg, U32 id_ext)
{
	UNUSED(app);
	UNUSED(id_ext);
	switch(id)
	{
		case BT_APP_HCI_CFM:
			DBG_LOG_APP_TEST_MODE( "[APP_TestMode] status:[%d]", 1, ((BT_APP_HCI_CFM_T *)msg)->status);
			break;
		default:
			DBG_LOG_APP_TEST_MODE( "[APP_TestMode] app_TestModeHandler id:[%d]", 1, id);
			FW_Assert(FALSE);
			break;
	}
	return 0;
}

static void app_TestMode_Send_LabTest_End(void)
{
	BtHciCmdSend((Handler)&appTestModeHandle,HCI_VCMD_LABTEST_TEST_END,0,NULL);
}

static void app_TestMode_Send_LabTest_CTX_DATA(HCI_VCMD_PARA_LABTEST_CTX_DATA_STRU* pPara)
{
	BtHciCmdSend((Handler)&appTestModeHandle,HCI_VCMD_LABTEST_CTX_DATA,sizeof(HCI_VCMD_PARA_LABTEST_CTX_DATA_STRU),(U8*)pPara);
}

static void app_TestMode_Send_LabTest_CRX_DATA(HCI_VCMD_PARA_LABTEST_CRX_DATA_STRU* pPara)
{
	BtHciCmdSend((Handler)&appTestModeHandle,HCI_VCMD_LABTEST_CRX_DATA,sizeof(HCI_VCMD_PARA_LABTEST_CRX_DATA_STRU),(U8*)pPara);
}

static void app_TestMode_Send_LabTest_BT3_BTX_PACKET(HCI_VCMD_PARA_LABTEST_BT3_BTX_PKT_STRU* pPara)
{
	BtHciCmdSend((Handler)&appTestModeHandle,HCI_VCMD_LABTEST_BT3_BTX_PACKET,sizeof(HCI_VCMD_PARA_LABTEST_BT3_BTX_PKT_STRU),(U8*)pPara);
}


//static void app_TestMode_Send_LabTest_BT3_BRX_PACKET(HCI_VCMD_PARA_LABTEST_BLE_BRX_PKT_STRU* pPara)
//{
//	BtHciCmdSend((Handler)&appTestModeHandle,HCI_VCMD_LABTEST_BLE_BRX_PACKET,sizeof(HCI_VCMD_PARA_LABTEST_BLE_BRX_PKT_STRU),(U8*)pPara);
//}


static AppTestModeHandlerSetType const APP_TestModeHandlerSet[TOTAL_TESTMODE_STATE_NO] =
{
	{APP_TxSingleTone},
	{APP_TxContinuePacket},
	{APP_TxBurstPacket},
	{APP_TxBurstPacket},
	{APP_RxTest},
};

APP_STATE_HANDLER_SET_TYPE_STRU const gAppStateTestModeHandler =
{
	APP_TestModeEntry, NULL, APP_TestModeExit, //APP_TEST_MODE
};

PRIVATE void APP_TestModeEntry(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);

	#ifdef APP_LE_ENABLE
	APP_LE_DisableAdv();
	#endif

	//New Event - "MEDIA_EVT_ENTER_TESTMODE_STATE"
	APP_Media_PushMediaEvent(MEDIA_EVT_ENTER_TESTMODE_STATE);


	//((U8 *)&gHC_ctl.BdAddr)[0] = 0x7E;
	//((U8 *)&gHC_ctl.BdAddr)[1] = 0x96;
	//((U8 *)&gHC_ctl.BdAddr)[2] = 0xC6;
	//((U8 *)&gHC_ctl.BdAddr)[3] = 0x6B;
	//((U8 *)&gHC_ctl.BdAddr)[4] = 0x00;
	//((U8 *)&gHC_ctl.BdAddr)[5] = 0x00;

	gAPP_TestMode_ctl.state = TOTAL_TESTMODE_STATE_NO - 1;
	APP_TestMode_StartLabTest();

}

PRIVATE void APP_TestModeExit(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
}

PRIVATE void APP_TestMode_StartLabTest(void)
{
/*
	APP_HCI_SendVCmdLabTestSwitchMCUTo12M();

	APP_HCI_SendVCmdLabTestStart();
    OS_delay_ms(1000);


	//RX
	OS_ENTER_CRITICAL();
	//324B2A
	SYS_3WireWrite(0x13,gAPP_nvram.mmiFCCInfo.Reg32B1,gAPP_nvram.mmiFCCInfo.Reg32B2);
    OS_delay_us(1000);
	//350580
	SYS_3WireWrite(0x15,gAPP_nvram.mmiFCCInfo.Reg35B1,gAPP_nvram.mmiFCCInfo.Reg35B2);
    OS_delay_us(1000);
    OS_EXIT_CRITICAL();


	APP_HCI_SendVCmdLabTestUpdateUapLap((U8 XDATA_PTR)&gHC_ctl.BdAddr);
	APP_HCI_SendVCmdLabTestLTAddrInTxPkt();
	APP_HCI_SendVCmdLabTestReportRateSetup();
*/
}

PRIVATE void APP_TxSingleTone(U8 channel, U16 power)
{
	UNUSED(power);
	app_TestMode_Send_LabTest_End();
	HCI_VCMD_PARA_LABTEST_CTX_DATA_STRU para;
	para.Channel = channel;
	para.ModType = 255;//singletone
	para.PatternType = 255;//singletone
	para.TxGc = ((U16)tx_default_gfsk_bias2_1<<8) | ((U16)tx_default_gfsk_bias2_2);
	app_TestMode_Send_LabTest_CTX_DATA(&para);
/*
	APP_HCI_SendVCmdLabTestPause();
	APP_HCI_SendVCmdLabTestSingleToneSetup(	channel,
											APP_LabTest_TransferPowerToGC1GC2(power),
											((U16)gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_1<<8) | (U16)gLC_ctl.rf_ctl.tx_gain_ctl.tx_default_gfsk_bias2_2);
*/
}

PRIVATE void APP_TxContinuePacket(U8 channel, U16 power)
{
	app_TestMode_Send_LabTest_End();
	HCI_VCMD_PARA_LABTEST_CTX_DATA_STRU para;
	para.Channel = channel;
	para.ModType = 0;//gfsk
	para.PatternType = 6;//??
	para.TxGc = ((U16)power<<8) | ((U16)power);
	app_TestMode_Send_LabTest_CTX_DATA(&para);
}

PRIVATE void APP_TxBurstPacket(U8 channel, U16 power)
{
	app_TestMode_Send_LabTest_End();
	power = APP_LabTest_TransferPowerToGC1GC2 (power);
	HCI_VCMD_PARA_LABTEST_BT3_BTX_PKT_STRU para;

	memcpy(&para.BdAddr,&defaultBdAddr,6);
	para.EndChannel = channel;
	para.StartChannel = channel;
	para.TxGc = ((U16)power<<8) | ((U16)power);
	switch(gAPP_TestMode_ctl.packetTypeIndex[APP_BTX])
	{
		case APP_2DH5:
			para.PayloadLen = BT_MAX_LENGTH_2DH5;
			para.PktType = BT_PKT_TYPE_2DH5;
			para.PatternType = 6; //??
			break;
		case APP_3DH5:
			para.PayloadLen = BT_MAX_LENGTH_3DH5;
			para.PktType = BT_PKT_TYPE_3DH5;
			para.PatternType = 6; //??
			break;
		default:
			para.PayloadLen = BT_MAX_LENGTH_DH5;
			para.PktType = BT_PKT_TYPE_DH5;
			para.PatternType = 6; //??
			break;
	}
	app_TestMode_Send_LabTest_BT3_BTX_PACKET(&para);
}

PRIVATE void APP_RxTest(U8 channel, U16 power)
{
	UNUSED(power);
	app_TestMode_Send_LabTest_End();
	HCI_VCMD_PARA_LABTEST_CRX_DATA_STRU para;
	para.Channel = channel;
	app_TestMode_Send_LabTest_CRX_DATA(&para);
	// issue 4917
	//SYS_3WireWrite(0x13, 0xC9, 0x4A);
	//SYS_3WireWrite(0x15, 0x00, 0xF8);
	//APP_HCI_SendVCmdLabTestPause();
	//APP_HCI_SendVCmdLabTestTxModemManualOff();
	//APP_HCI_SendVCmdLabTestContinueRxStart(channel, 0, 0);
}

PRIVATE U16 APP_LabTest_TransferPowerToGC1GC2 (U16 power)
{
	U16 temp;

	temp = tx_default_gfsk_gc1 + ((U16)tx_default_gfsk_gc2 << 8);
	temp &= ~((U16)0x003F << 7);
	temp |= (power << 7);
	return (U16)((temp<<8)|(temp>>8));
}

#define LED_BRAMPFLASH	0
#define LED_BSFLASH		1
#define LED_BFFLASH		2
#define LED_RRAMPFLASH	3

PRIVATE void APP_TestMode_LedHandler(U8 type, U8 repeat)
{
    U8 ledIndex;
    LED_SETTING_STRU *LedCtrlPtr;

    LedCtrlPtr = FW_GetMemory(sizeof(LED_SETTING_STRU));

    if(!LedCtrlPtr)
    {
        FW_Assert(FALSE);
        return;
    }

    FW_Memset((U8 *)LedCtrlPtr, 0, sizeof(LED_SETTING_STRU));
    switch(type)
    {
        case LED_BRAMPFLASH:
            //BLUE
            LedCtrlPtr->onOff= LED_ON;
            LedCtrlPtr->ledFollowIndex= 0x01;
            LedCtrlPtr->pwmDuration = 0xF0 | repeat;
            LedCtrlPtr->t1 = 50;	//1s, unit:20ms
            LedCtrlPtr->t2 = 50;	//1s, unit:20ms
            LedCtrlPtr->t3 = 15;		//0.3s, unit:20ms
            ledIndex = 1;
            break;

        case LED_BSFLASH:
            //BLUE
            LedCtrlPtr->onOff  = LED_ON;
            LedCtrlPtr->ledFollowIndex = 0x01;
            LedCtrlPtr->pwmDuration = 0xF0 |repeat ;
            LedCtrlPtr->t1 = 50;	//1s, unit:20ms
            LedCtrlPtr->t2 = 50;	//1s, unit:20ms
            LedCtrlPtr->t3 = 15;		//0.3s, unit:20ms
            ledIndex = 1;
            break;

        case LED_BFFLASH:
            //BLUE
            LedCtrlPtr->onOff  = LED_ON;
            LedCtrlPtr->ledFollowIndex = 0x01;
            LedCtrlPtr->pwmDuration = 0xF0 | repeat;
            LedCtrlPtr->t1 = 5;	//0.1s, unit:20ms
            LedCtrlPtr->t2 = 5;	//0.1s, unit:20ms
            LedCtrlPtr->t3 = 15;		//0.3s, unit:20ms
            ledIndex = 1;
            break;

        case LED_RRAMPFLASH:
            //RED
            LedCtrlPtr->onOff  = LED_ON;
            LedCtrlPtr->ledFollowIndex = 0x01;
            LedCtrlPtr->pwmDuration = 0xF0 | repeat;
            LedCtrlPtr->t1 = 50;	//1s, unit:20ms
            LedCtrlPtr->t2 = 50;	//1s, unit:20ms
            LedCtrlPtr->t3 = 15;		//0.3s, unit:20ms
            ledIndex = 0;
            break;
        default:
            FW_FreeMemory((U8 *)LedCtrlPtr);
            return;
    }

    App_LED_SetPatternToHW(ledIndex,LedCtrlPtr);

    FW_FreeMemory((U8 *)LedCtrlPtr);
}


static 	U8 CODE TestModeLed[TOTAL_TESTMODE_STATE_NO] = {LED_BRAMPFLASH, LED_BSFLASH, LED_BFFLASH, LED_BFFLASH, LED_RRAMPFLASH};
void APP_TestMode_Execute(void)
{

	APP_TestModeHandlerSet[gAPP_TestMode_ctl.state].TestModeHandler(TestModeChannelMap[gAPP_TestMode_ctl.channelIndex[gAPP_TestMode_ctl.state]],
		tx_gfsk_max_gain - TestModePowerDiffMap[gAPP_TestMode_ctl.powerIndex[gAPP_TestMode_ctl.state]]);
	APP_TestMode_LedHandler(TestModeLed[gAPP_TestMode_ctl.state], gAPP_TestMode_ctl.channelIndex[gAPP_TestMode_ctl.state]);
}

void APP_TestMode_KeyEnterTestMode(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	APP_State_AddTopState(APP_EOF, APP_TEST_MODE);
	if(APP_State_CheckNestStateByLink(APP_EOF, APP_CONDISCABLE))
	{
		APP_State_DiscoverabilityHandle(CMD_SET_NON_DISCOVERABLE);
	}
}

void APP_TestMode_KeyTestModeChannel(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	gAPP_TestMode_ctl.channelIndex[gAPP_TestMode_ctl.state] =
		(gAPP_TestMode_ctl.channelIndex[gAPP_TestMode_ctl.state]+1)% sizeof(TestModeChannelMap);
	APP_TestMode_Execute();
}

void APP_TestMode_KeyTestModePower(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	gAPP_TestMode_ctl.powerIndex[gAPP_TestMode_ctl.state] =
		(gAPP_TestMode_ctl.powerIndex[gAPP_TestMode_ctl.state]+1)% sizeof(TestModePowerDiffMap);
	APP_TestMode_Execute();
}

void APP_TestMode_KeyTestModeState(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	//Clear Info
	gAPP_TestMode_ctl.channelIndex[gAPP_TestMode_ctl.state] = 0;
	gAPP_TestMode_ctl.powerIndex[gAPP_TestMode_ctl.state] = 0;

	gAPP_TestMode_ctl.state =
		(gAPP_TestMode_ctl.state+1)%TOTAL_TESTMODE_STATE_NO;
	APP_TestMode_Execute();
}

void APP_TestMode_KeyTestModeTxPktType(BD_ADDR_T *pBdAddr)
{
	UNUSED(pBdAddr);
	gAPP_TestMode_ctl.packetTypeIndex[gAPP_TestMode_ctl.state] =
		(gAPP_TestMode_ctl.packetTypeIndex[gAPP_TestMode_ctl.state]+1)%TOTAL_TESTMODE_PACKET_TYPE_NO;
	APP_TestMode_Execute();
}

