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
 
 
#ifndef _APP_HFP_SCO_HANDLER_H_
#define _APP_HFP_SCO_HANDLER_H_

#define LINK_TYPE_SCO    (0x00)
#define LINK_TYPE_ACL    (0x01)
#define LINK_TYPE_eSCO   (0x02)
#define LINK_TYPE_SCO_ONLY_HV3	(0x03)

void APP_HfpSco_Register(void);
void APP_HfpSco_ConnectRequest(BD_ADDR_T *pBdAddr, BOOL isCodecMSBC,U8 linkType); //MMI_HCI_SendSetupESCO
void APP_HfpSco_DisconnectRequest(BD_ADDR_T *pBdAddr); //MMI_HCI_SendRelSco
void APP_HfpSco_DisconnectAllSco(void);
void APP_HfpSco_SendDSPCmd(BD_ADDR_T *pBdAddr); //APP_Driver_SendDSPCmd(pBdAddr, DSP_ENABLE_BY_SCO);
void APP_HfpSco_DisconnectHandleByBdAddress(BD_ADDR_T *pBdAddr);
U8 APP_HfpSco_IsSupportCodecWideBand(BD_ADDR_T* pBdAddr);
void APP_HfpSco_SetRetransmissionEnable(BD_ADDR_T *pBdAddr, U8 enable);	// 1:disable 0:enable

void APP_HfpSco_RegisterStopNotify(void);
void APP_HfpSco_DeRegisterStopNotify(void);
BOOL APP_HfpSco_IsRegisterStopNotify(void);

U8 APP_HfpSco_GetMsgNumInQueue(void);
void APP_HfpSco_SetEcnr(BOOL isEcnrOff);
void APP_HfpSco_EnableLightMonoMode(BD_ADDR_T *pBdAddr);
void APP_HfpSco_DisableLightMonoMode(BD_ADDR_T *pBdAddr);
	
#endif //_APP_HFP_SCO_HANDLER_H_
