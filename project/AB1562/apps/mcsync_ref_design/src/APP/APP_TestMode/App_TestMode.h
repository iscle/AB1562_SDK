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
<<<<<<< HEAD
 
 
=======


>>>>>>> db20e11 (second commit)
#ifndef _APP_TEST_MODE_H_
#define _APP_TEST_MODE_H_

#if 0 /* Unused LOG_PRINT_156X */
<<<<<<< HEAD
#define DBG_LOG_APP_TEST_MODE(_message, arg_cnt, ...) 
=======
#define DBG_LOG_APP_TEST_MODE(_message, arg_cnt, ...)
>>>>>>> db20e11 (second commit)
#else
#define DBG_LOG_APP_TEST_MODE(_message, arg_cnt, ...)   LOG_MSGID_I(APP_TEST_MODE, _message, arg_cnt, ##__VA_ARGS__)
#endif

typedef U8 HCI_CMD_OGF;
typedef U16 HCI_OPCODE;
typedef U8 HCI_VCMD_GRP;

#define HCI_VCMD_GRP_TESTING                                (HCI_VCMD_GRP)0x01

#define HCI_CMD_OGF_AIROHA_VENDOR                           (HCI_CMD_OGF)0x3F
#define HCI_COMBINED_OCF_GRP1(vocf) (((HCI_OPCODE)(HCI_VCMD_GRP_TESTING)<<6)|(HCI_OPCODE)(vocf))
#define HCI_COMBINED_OPCODE(ogf,ocf) (((HCI_OPCODE)(ogf)<<10)|(HCI_OPCODE)(ocf))
#define tx_gfsk_max_gain 50
#define tx_default_gfsk_gc1 0x0D
#define tx_default_gfsk_gc2 0x79
#define tx_default_gfsk_bias2_1 0xBC
#define tx_default_gfsk_bias2_2 0xA8

/* HCI Vendor Command OCF of TESTING Group */
#define HCI_VCMD_OCF_LABTEST_TEST_END                           	HCI_COMBINED_OCF_GRP1(0x01)
#define HCI_VCMD_OCF_LABTEST_CTX_DATA                           	HCI_COMBINED_OCF_GRP1(0x02)
#define HCI_VCMD_OCF_LABTEST_CRX_DATA                           	HCI_COMBINED_OCF_GRP1(0x03)
#define HCI_VCMD_OCF_LABTEST_BT3_BTX_PACKET                     	HCI_COMBINED_OCF_GRP1(0x04)
#define HCI_VCMD_OCF_LABTEST_BT3_BRX_PACKET                     	HCI_COMBINED_OCF_GRP1(0x05)
#define HCI_VCMD_OCF_LABTEST_BLE_BTX_PACKET                     	HCI_COMBINED_OCF_GRP1(0x06)
#define HCI_VCMD_OCF_LABTEST_BLE_BRX_PACKET                     	HCI_COMBINED_OCF_GRP1(0x07)
#define HCI_VCMD_OCF_LABTEST_SET_TEMPERATURE_COMPENSATION_ENABLE    HCI_COMBINED_OCF_GRP1(0x08)

/* HCI Testing Command OpCode */
#define HCI_VCMD_LABTEST_TEST_END                               	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_TEST_END)
#define HCI_VCMD_LABTEST_CTX_DATA                               	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_CTX_DATA)
#define HCI_VCMD_LABTEST_CRX_DATA                              		HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_CRX_DATA)
#define HCI_VCMD_LABTEST_BT3_BTX_PACKET                         	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_BT3_BTX_PACKET)
#define HCI_VCMD_LABTEST_BT3_BRX_PACKET                         	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_BT3_BRX_PACKET)
#define HCI_VCMD_LABTEST_BLE_BTX_PACKET                         	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_BLE_BTX_PACKET)
#define HCI_VCMD_LABTEST_BLE_BRX_PACKET                         	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_BLE_BRX_PACKET)
#define HCI_VCMD_LABTEST_SET_TEMPERATURE_COMPENSATION_ENABLE    	HCI_COMBINED_OPCODE(HCI_CMD_OGF_AIROHA_VENDOR,HCI_VCMD_OCF_LABTEST_SET_TEMPERATURE_COMPENSATION_ENABLE)

//========================================== packet type defination

#define BT_PKT_NULL                     0x0
#define BT_PKT_POLL                     0x1
#define BT_PKT_FHS                      0x2
#define BT_PKT_DM1                      0x3
#define BT_PKT_DH1                      0x4
#define BT_PKT_2DH1                     0x4
#define BT_PKT_HV1                      0x5
#define BT_PKT_HV2                      0x6
#define BT_PKT_2EV3                     0x6
#define BT_PKT_HV3                      0x7
#define BT_PKT_EV3                      0x7
#define BT_PKT_3EV3                     0x7
#define BT_PKT_DV                       0x8
#define BT_PKT_3DH1                     0x8
#define BT_PKT_AUX1                     0x9
#define BT_PKT_DM3                      0xA
#define BT_PKT_2DH3                     0xA
#define BT_PKT_DH3                      0xB
#define BT_PKT_3DH3                     0xB
#define BT_PKT_EV4                      0xC
#define BT_PKT_2EV5                     0xC
#define BT_PKT_EV5                      0xD
#define BT_PKT_3EV5                     0xD
#define BT_PKT_DM5                      0xE
#define BT_PKT_2DH5                     0xE
#define BT_PKT_DH5                      0xF
#define BT_PKT_3DH5                     0xF
#define BT_CRC_OK                       0x10
#define BT_HEC_OK                       0x08

typedef U8 LMP_VERSION;
#define LMP_VERSION_OF_BT_CORE_SPEC_10B     ((LMP_VERSION)(0))
#define LMP_VERSION_OF_BT_CORE_SPEC_11      ((LMP_VERSION)(1))
#define LMP_VERSION_OF_BT_CORE_SPEC_12      ((LMP_VERSION)(2))
#define LMP_VERSION_OF_BT_CORE_SPEC_20EDR   ((LMP_VERSION)(3))
#define LMP_VERSION_OF_BT_CORE_SPEC_21EDR   ((LMP_VERSION)(4))
#define LMP_VERSION_OF_BT_CORE_SPEC_30HS    ((LMP_VERSION)(5))
#define LMP_VERSION_OF_BT_CORE_SPEC_40      ((LMP_VERSION)(6))
#define LMP_VERSION_OF_BT_CORE_SPEC_41      ((LMP_VERSION)(7))
#define LMP_VERSION_OF_BT_CORE_SPEC_42      ((LMP_VERSION)(8))
#define LMP_VERSION_OF_BT_CORE_SPEC_50      ((LMP_VERSION)(9))
<<<<<<< HEAD
=======
#define LMP_VERSION_OF_BT_CORE_SPEC_51      ((LMP_VERSION)(10))
#define LMP_VERSION_OF_BT_CORE_SPEC_52      ((LMP_VERSION)(11))

>>>>>>> db20e11 (second commit)

typedef U8 HCI_VERSION;
#define HCI_VERSION_OF_BT_CORE_SPEC_10B     ((HCI_VERSION)(0))
#define HCI_VERSION_OF_BT_CORE_SPEC_11      ((HCI_VERSION)(1))
#define HCI_VERSION_OF_BT_CORE_SPEC_12      ((HCI_VERSION)(2))
#define HCI_VERSION_OF_BT_CORE_SPEC_20EDR   ((HCI_VERSION)(3))
#define HCI_VERSION_OF_BT_CORE_SPEC_21EDR   ((HCI_VERSION)(4))
#define HCI_VERSION_OF_BT_CORE_SPEC_30HS    ((HCI_VERSION)(5))
#define HCI_VERSION_OF_BT_CORE_SPEC_40      ((HCI_VERSION)(6))
#define HCI_VERSION_OF_BT_CORE_SPEC_41      ((HCI_VERSION)(7))
#define HCI_VERSION_OF_BT_CORE_SPEC_42      ((HCI_VERSION)(8))
#define HCI_VERSION_OF_BT_CORE_SPEC_50      ((HCI_VERSION)(9))

typedef U8 BT_PKT_CODE;
#define BT_PKT_CODE_NULL                     ((BT_PKT_CODE)0x0)
#define BT_PKT_CODE_POLL                     ((BT_PKT_CODE)0x1)
#define BT_PKT_CODE_FHS                      ((BT_PKT_CODE)0x2)
#define BT_PKT_CODE_DM1                      ((BT_PKT_CODE)0x3)
#define BT_PKT_CODE_DH1                      ((BT_PKT_CODE)0x4)
#define BT_PKT_CODE_2DH1                     ((BT_PKT_CODE)0x4)
#define BT_PKT_CODE_HV1                      ((BT_PKT_CODE)0x5)
#define BT_PKT_CODE_HV2                      ((BT_PKT_CODE)0x6)
#define BT_PKT_CODE_2EV3                     ((BT_PKT_CODE)0x6)
#define BT_PKT_CODE_HV3                      ((BT_PKT_CODE)0x7)
#define BT_PKT_CODE_EV3                      ((BT_PKT_CODE)0x7)
#define BT_PKT_CODE_3EV3                     ((BT_PKT_CODE)0x7)
#define BT_PKT_CODE_DV                       ((BT_PKT_CODE)0x8)
#define BT_PKT_CODE_3DH1                     ((BT_PKT_CODE)0x8)
#define BT_PKT_CODE_AUX1                     ((BT_PKT_CODE)0x9)
#define BT_PKT_CODE_DM3                      ((BT_PKT_CODE)0xA)
#define BT_PKT_CODE_2DH3                     ((BT_PKT_CODE)0xA)
#define BT_PKT_CODE_DH3                      ((BT_PKT_CODE)0xB)
#define BT_PKT_CODE_3DH3                     ((BT_PKT_CODE)0xB)
#define BT_PKT_CODE_EV4                      ((BT_PKT_CODE)0xC)
#define BT_PKT_CODE_2EV5                     ((BT_PKT_CODE)0xC)
#define BT_PKT_CODE_EV5                      ((BT_PKT_CODE)0xD)
#define BT_PKT_CODE_3EV5                     ((BT_PKT_CODE)0xD)
#define BT_PKT_CODE_DM5                      ((BT_PKT_CODE)0xE)
#define BT_PKT_CODE_2DH5                     ((BT_PKT_CODE)0xE)
#define BT_PKT_CODE_DH5                      ((BT_PKT_CODE)0xF)
#define BT_PKT_CODE_3DH5                     ((BT_PKT_CODE)0xF)

typedef U8 BT_LOGICAL_TRANSPORT;
#define BT_LOGICAL_TRANSPORT_ACL_SCO         ((BT_LOGICAL_TRANSPORT)0)
#define BT_LOGICAL_TRANSPORT_ESCO            ((BT_LOGICAL_TRANSPORT)1)

typedef U8 BT_DATA_RATE;
#define BT_DATA_RATE_BR                      ((BT_DATA_RATE)0)
#define BT_DATA_RATE_EDR                     ((BT_DATA_RATE)1)

typedef U8 BT_PKT_TYPE;
#define BT_COMBINED_PKT_TYPE(code,lt,rate)   (((BT_PKT_TYPE)(rate)<<5)|((BT_PKT_TYPE)(lt)<<4)|(BT_PKT_TYPE)code)



#define BT_PKT_TYPE_NULL                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_NULL,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_POLL                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_POLL,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_FHS                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_FHS,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_DM1                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DM1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_DH1                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DH1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_2DH1                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_2DH1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_HV1                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_HV1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_HV2                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_HV2,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_2EV3                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_2EV3,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_HV3                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_HV3,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_EV3                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_EV3,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_3EV3                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_3EV3,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_DV                       BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DV,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_3DH1                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_3DH1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_AUX1                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_AUX1,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_DM3                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DM3,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_2DH3                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_2DH3,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_DH3                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DH3,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_3DH3                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_3DH3,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_EV4                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_EV4,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_2EV5                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_2EV5,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_EV5                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_EV5,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_3EV5                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_3EV5,BT_LOGICAL_TRANSPORT_ESCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_DM5                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DM5,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_2DH5                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_2DH5,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)
#define BT_PKT_TYPE_DH5                      BT_COMBINED_PKT_TYPE(BT_PKT_CODE_DH5,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_BR)
#define BT_PKT_TYPE_3DH5                     BT_COMBINED_PKT_TYPE(BT_PKT_CODE_3DH5,BT_LOGICAL_TRANSPORT_ACL_SCO,BT_DATA_RATE_EDR)

#define BT_CRC_OK                       0x10
#define BT_HEC_OK                       0x08

#define BT_MAX_LENGTH_DM1               17
#define BT_MAX_LENGTH_DH1               27
#define BT_MAX_LENGTH_DM3               121
#define BT_MAX_LENGTH_DH3               183
#define BT_MAX_LENGTH_DM5               224
#define BT_MAX_LENGTH_DH5               339
#define BT_MAX_LENGTH_AUX1              29

#define BT_MAX_LENGTH_2DH1              54
#define BT_MAX_LENGTH_2DH3              367
#define BT_MAX_LENGTH_2DH5              679
#define BT_MAX_LENGTH_3DH1              83
#define BT_MAX_LENGTH_3DH3              552
#define BT_MAX_LENGTH_3DH5              1021

#define BT_MAX_LENGTH_HV1           10
#define BT_MAX_LENGTH_HV2           20
#define BT_MAX_LENGTH_HV3           30
#define BT_MAX_LENGTH_DV_VOICE      10
#define BT_MAX_LENGTH_DV_DATA       9
#define BT_MAX_LENGTH_DV_TOTAL      19
#define BT_MAX_LENGTH_EV3           30
#define BT_MAX_LENGTH_EV4           120
#define BT_MAX_LENGTH_EV5           180
#define BT_MAX_LENGTH_2EV3          60
#define BT_MAX_LENGTH_2EV5          360
#define BT_MAX_LENGTH_3EV3          90
#define BT_MAX_LENGTH_3EV5          540


typedef struct stru_hci_vcmd_para_labtest_ctx_data
{
    U8 Channel;
    U8 ModType;
    U8 PatternType;
    U16 TxGc;

}PACKED HCI_VCMD_PARA_LABTEST_CTX_DATA_STRU;

typedef struct stru_hci_vcmd_para_labtest_crx_data
{
    U8 Channel;

}PACKED HCI_VCMD_PARA_LABTEST_CRX_DATA_STRU;

typedef struct stru_hci_vcmd_para_labtest_bt3_btx_pkt
{
    U8 StartChannel;
    U8 EndChannel;
    U8 PktType;
    U16 PayloadLen;
    U8 PatternType;
    U16 TxGc;
    U8 BdAddr[6];

} PACKED HCI_VCMD_PARA_LABTEST_BT3_BTX_PKT_STRU;

typedef struct stru_hci_vcmd_para_labtest_bt3_brx_pkt
{
    U8 Channel;
    U8 ModType;
    U8 BdAddr[6];

} HCI_VCMD_PARA_LABTEST_BT3_BRX_PKT_STRU;

typedef struct stru_hci_vcmd_para_labtest_ble_btx_pkt
{
    U8 StartChannel;
    U8 EndChannel;
    U8 ModType;
    U8 PayloadLen;
    U8 PatternType;
    U16 TxGc;
    ADDR32 AccessAddr;
    U8 CrcInit[3];

} PACKED HCI_VCMD_PARA_LABTEST_BLE_BTX_PKT_STRU;

typedef struct stru_hci_vcmd_para_labtest_ble_brx_pkt
{
    U8 Channel;
    U8 ModType;
    ADDR32 AccessAddr;
    U8 CrcInit[3];

} PACKED HCI_VCMD_PARA_LABTEST_BLE_BRX_PKT_STRU;

//
	enum
	{
		APP_STX,
		APP_CTX,
		APP_BTX,
		APP_HBTX,	//burst TX with hopping
		APP_RX,
		TOTAL_TESTMODE_STATE_NO,
	};

	enum
	{
		APP_DH5,
		APP_2DH5,
		APP_3DH5,
		TOTAL_TESTMODE_PACKET_TYPE_NO
	};
<<<<<<< HEAD
	
=======

>>>>>>> db20e11 (second commit)
	typedef struct
	{
		U8 state;
		U8 channelIndex[TOTAL_TESTMODE_STATE_NO];
		U8 powerIndex[TOTAL_TESTMODE_STATE_NO];
		U8 packetTypeIndex[TOTAL_TESTMODE_STATE_NO];
	}AppTestModeInfo;
<<<<<<< HEAD
	
=======

>>>>>>> db20e11 (second commit)
	typedef struct
	{
		void (*TestModeHandler)(U8 channel, U16 power);
	}AppTestModeHandlerSetType;
<<<<<<< HEAD
	
=======

>>>>>>> db20e11 (second commit)

    void APP_TestMode_ProcessKeyEvent(BD_ADDR_T *pBdAddr, U8 index);
    void APP_TestMode_KeyEnterTestMode(BD_ADDR_T *pBdAddr);
    void APP_TestMode_KeyTestModeChannel(BD_ADDR_T *pBdAddr);
    void APP_TestMode_KeyTestModePower(BD_ADDR_T *pBdAddr);
    void APP_TestMode_KeyTestModeState(BD_ADDR_T *pBdAddr);
    void APP_TestMode_KeyTestModeTxPktType(BD_ADDR_T *pBdAddr);


    U8 APP_TestMode_GetMsgNumInQueue(void);

<<<<<<< HEAD
	
=======

>>>>>>> db20e11 (second commit)
#endif //_APP_TEST_MODE_H_
