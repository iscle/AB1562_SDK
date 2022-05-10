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

#ifndef _LOG_H_
#define _LOG_H_

#include "types.h"
#include "stdio.h"
#include "syslog.h"

#define MAX_LOG_STRING 48
//#define LOG_STRING __attribute__ ((section (".log.string")))
#define LOG_STRING const
#define LOG_STRING_NEW __attribute__ ((__section__(".log_string")))
#define LOG_STRING_SYS1 __attribute__ ((section (".log.system1.string")))
#define LOG_STRING_SYS2 __attribute__ ((section (".log.system2.string")))


typedef enum {
    PRINT_LEVEL_EMPTY,
} PRINTF_LEVEL;

typedef U32 LOG_PRINTF_GROUP;
/* user group start from 0 */
#define LOG_USER1			((LOG_PRINTF_GROUP)1<<0)
#define LOG_USER2			((LOG_PRINTF_GROUP)1<<1)
#define LOG_USER3			((LOG_PRINTF_GROUP)1<<2)
#define LOG_USER4			((LOG_PRINTF_GROUP)1<<3)
#define LOG_USER5			((LOG_PRINTF_GROUP)1<<4)

/* module group start from 5 */
//#define LOG_OS				((LOG_PRINTF_GROUP)1<<6)
#define LOG_DM2L			((LOG_PRINTF_GROUP)1<<6)
#define LOG_DSP				((LOG_PRINTF_GROUP)1<<7)
//#define LOG_GERNERAL        ((LOG_PRINTF_GROUP)1<<8)

/* app info start from 10*/
#define LOG_APP_Connection	((LOG_PRINTF_GROUP)1<<10)
#define LOG_APP_A2DP		((LOG_PRINTF_GROUP)1<<11)
#define LOG_APP_AVRCP		((LOG_PRINTF_GROUP)1<<12)
#define LOG_APP_HFP			((LOG_PRINTF_GROUP)1<<13)
#define LOG_APP_USR			((LOG_PRINTF_GROUP)1<<14)
#define LOG_APP_STATE		((LOG_PRINTF_GROUP)1<<15)
#define LOG_APP_DSP			((LOG_PRINTF_GROUP)1<<16)
#define LOG_APP_SYSTEM		((LOG_PRINTF_GROUP)1<<17)

/* Profile */
#define LOG_PROFILE_A2DP	((LOG_PRINTF_GROUP)1<<18)
#define LOG_PROFILE_AVRCP	((LOG_PRINTF_GROUP)1<<19)
#define LOG_PROFILE_HFP		((LOG_PRINTF_GROUP)1<<20)
#define LOG_PROFILE_OTHER   ((LOG_PRINTF_GROUP)1<<21)


/* BT LIB */
#define LOG_L2CAP           ((LOG_PRINTF_GROUP)1<<23)
#define LOG_RFCOMM          ((LOG_PRINTF_GROUP)1<<24)
#define LOG_SDP             ((LOG_PRINTF_GROUP)1<<25)
#define LOG_GAP             ((LOG_PRINTF_GROUP)1<<26)


/* system info start from 28 */
#define LOG_COREDUMP		((LOG_PRINTF_GROUP)1<<28)
#define LOG_PRINT			((LOG_PRINTF_GROUP)1<<29)
#define LOG_MEMDUMP			((LOG_PRINTF_GROUP)1<<30)
#define LOG_ASSERT			((LOG_PRINTF_GROUP)1<<31)

#define LOG_COMMON_MSGID_INFO(_message, arg_cnt,...)       LOG_MSGID_I(common, _message, arg_cnt,##__VA_ARGS__)
#define LOG_MPLOG_MSGID_W(_message, arg_cnt,...)           LOG_W(MPLOG, _message, ##__VA_ARGS__)

EXTERN LOG_STRING char exampleLogString[];
EXTERN LOG_STRING char testLogString[];
EXTERN LOG_STRING_NEW char exampleLogString1[];
EXTERN LOG_STRING_NEW char testString[2][MAX_LOG_STRING];

EXTERN LOG_STRING char SystemPowerOff[];
EXTERN LOG_STRING char SystemPowerOffFail[];
EXTERN LOG_STRING char SystemLowPowerMode[];
EXTERN LOG_STRING char SystemNormalPowerMode[];
EXTERN LOG_STRING char tickInfo[];
EXTERN LOG_STRING char isrInfo[];
EXTERN LOG_STRING char isrStackSize[];
EXTERN LOG_STRING char taskInfo0[];
EXTERN LOG_STRING char taskInfo1[];
EXTERN LOG_STRING char taskBackTrace[];
EXTERN LOG_STRING char taskNameString[][MAX_LOG_STRING];
EXTERN LOG_STRING char taskNameString2[][MAX_LOG_STRING];
EXTERN LOG_STRING char taskState[][MAX_LOG_STRING];
EXTERN LOG_STRING char memoryInfo[];
EXTERN LOG_STRING char heapInfo[];
EXTERN LOG_STRING char memoryInfoString[][MAX_LOG_STRING];
EXTERN LOG_STRING char wakeUpInfo[];
EXTERN LOG_STRING char heapBlockInfo[];
EXTERN LOG_STRING char heapFragInfo[];
EXTERN LOG_STRING char heapCrash[];
EXTERN LOG_STRING char airapptest[];
EXTERN LOG_STRING char heapDumpBegin[];
EXTERN LOG_STRING char heapDumpEnd[];
EXTERN LOG_STRING char heapSGBlockInfo[];
EXTERN LOG_STRING char systemDead[];
EXTERN LOG_STRING char System_IpcMsgDispatchString[];

EXTERN LOG_STRING_NEW char DSP_WarningString[][64];

EXTERN LOG_STRING char DSP_INFO_CreateCallbackStream[];
EXTERN LOG_STRING char DSP_INFO_CloseCallbackStream[];
EXTERN LOG_STRING char DSP_INFO_AnalogInputGainString[];
EXTERN LOG_STRING char DSP_INFO_AnalogOutputGainString[];
EXTERN LOG_STRING char DSP_INFO_DigitalInputGainString[];
EXTERN LOG_STRING char DSP_INFO_DigitalOutputGainString[];
EXTERN LOG_STRING char DSP_INFO_SourceConfiguration[];
EXTERN LOG_STRING char DSP_INFO_SinkConfiguration[];
EXTERN LOG_STRING char DSP_INFO_EnableSrc[];
EXTERN LOG_STRING char DSP_INFO_DisableSrcClk[];
EXTERN LOG_STRING char DSP_INFO_EnableAudioClock[];
EXTERN LOG_STRING char DSP_INFO_DisableAudioClock[];
EXTERN LOG_STRING char DSP_INFO_Audio3wireSetting[];
EXTERN LOG_STRING char DSP_INFO_EnableMceAudio[];
EXTERN LOG_STRING char DSP_INFO_DisableMceAudio[];
EXTERN LOG_STRING char DSP_INFO_ModifyAudioChannelSel[];
EXTERN LOG_STRING char DSP_INFO_UpdateFeatureCommand[] ;
EXTERN LOG_STRING char DSP_INFO_UpdateFeatureMips[];
EXTERN LOG_STRING char DSP_INFO_MceSyncParaPush[];
EXTERN LOG_STRING char DSP_INFO_MceSyncParaPop[];
EXTERN LOG_STRING char DSP_INFO_StreamDeinit[];
EXTERN LOG_STRING char DSP_INFO_ScoPacketCheck[];
EXTERN LOG_STRING char DSP_INFO_MicBiasCtrl[];
EXTERN LOG_STRING char DSP_INFO_AdioOutputDelayTime[];
EXTERN LOG_STRING char DSP_INFO_AudioMutePenalties[];
EXTERN LOG_STRING char DSP_INFO_AudioMutePenaltyClear[];
EXTERN LOG_STRING char DSP_INFO_AudioInputDevice[];
EXTERN LOG_STRING char DSP_INFO_AudioOutputDevice[];
EXTERN LOG_STRING char DSP_INFO_AudioI2SMclkConfigure[];
EXTERN LOG_STRING char DSP_INFO_DebugString[];


EXTERN LOG_STRING char dsp_hal_info_InterconnectionState[];
EXTERN LOG_STRING char dsp_hal_info_SrcParameters[];
EXTERN LOG_STRING char dsp_hal_info_SrcTrackingMode[];
EXTERN LOG_STRING char dsp_hal_info_SrcIIRCoefficient[];
EXTERN LOG_STRING char dsp_hal_info_AfeControl[];
EXTERN LOG_STRING char dsp_hal_info_MemoryOccupy[];
EXTERN LOG_STRING char dsp_hal_info_MemorySramAlloc[];
EXTERN LOG_STRING char dsp_hal_info_MemorySramFree[];
EXTERN LOG_STRING char dsp_hal_warning_UnsupportSrcIIR[];
EXTERN LOG_STRING char dsp_hal_warning_SrcTrackingBusy[];
EXTERN LOG_STRING char dsp_hal_warning_DigitalGainOccupancy[];
EXTERN LOG_STRING char dsp_hal_error_WrongDeviceAgent[];
EXTERN LOG_STRING char dsp_hal_error_WrongMemoryAgent[];
EXTERN LOG_STRING char dsp_hal_error_AudioAgentUserCnt[];
EXTERN LOG_STRING char dsp_hal_error_SramAllocFail[];
EXTERN LOG_STRING char dsp_hal_error_HalAudioControl[];
EXTERN LOG_STRING char dsp_hal_error_log[];

EXTERN LOG_STRING char DSP_MemoryFlush[];
EXTERN LOG_STRING char DSP_MemoryCorruptionLogString[];
EXTERN LOG_STRING char DSP_MemoryCorruptionHeaderLogString[];
EXTERN LOG_STRING char DSP_MemoryCorruptionEOBLogString[];
EXTERN LOG_STRING char DSP_MemoryCorruptionFeatureLogString[];

/* AMA */
EXTERN LOG_STRING char AMA_GetLinkIdxString[];
EXTERN LOG_STRING char AMA_UpdateUpgradeTransportDataString[];
EXTERN LOG_STRING char AMA_GetEmptyLinkInfoString[];
EXTERN LOG_STRING char AMA_ClearLinkInfoString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerGetDataLengthString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerGetDataNoAmaLinkString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerGetDataOkString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerApplyAgentFailString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerApplyAgentOkString[];
EXTERN LOG_STRING char AMA_RhoLinkManagerApplyPartnerOkString[];
EXTERN LOG_STRING char AMA_NvkeyGetVarDataString[];
EXTERN LOG_STRING char AMA_NvkeyUpdateVarDataString[];
EXTERN LOG_STRING char AMA_NvkeyVarDataOobString[];
EXTERN LOG_STRING char AMA_NvkeyVarDataBdAddrString[];
EXTERN LOG_STRING char AMA_RhoDataString[];
EXTERN LOG_STRING char AMA_RhoNoMemString[];
EXTERN LOG_STRING char AMA_RhoPushFailString[];
EXTERN LOG_STRING char AMA_RhoQlenString[];
EXTERN LOG_STRING char AMA_RhoApplyB4String[];
EXTERN LOG_STRING char AMA_RhoApplyAfterString[];

/* AMA BLE */
EXTERN LOG_STRING char AMA_BleEnableAdvBdAString[];
EXTERN LOG_STRING char AMA_BleAccessRxDescString[];
EXTERN LOG_STRING char	AMA_BleDeviceStateString[];
EXTERN LOG_STRING char	AMA_BleKeyIdString[];
EXTERN LOG_STRING char AMA_BleSetAdvEnableCfmString[];
EXTERN LOG_STRING char AMA_BleConnectedString[];
EXTERN LOG_STRING char	AMA_BleDeinitString[];
EXTERN LOG_STRING char AMA_BleDisableAdvString[];
EXTERN LOG_STRING char AMA_BleEnableAdvString[];
EXTERN LOG_STRING char	AMA_BleDisconnString[];
EXTERN LOG_STRING char AMA_BleTxStateErrorString[];
EXTERN LOG_STRING char AMA_BleTxString[];
EXTERN LOG_STRING char AMA_BleTxCccdString[];
EXTERN LOG_STRING char AMA_BleGetDataLengthString[];
EXTERN LOG_STRING char AMA_BleGetDataNoAmaLinkString[];
EXTERN LOG_STRING char AMA_BleGetDataNoGapLinkString[];
EXTERN LOG_STRING char AMA_BleGetDataOkString[];
EXTERN LOG_STRING char AMA_BleApplyAgentNoGapLinkString[];
EXTERN LOG_STRING char AMA_BleApplyAgentOkString[];
EXTERN LOG_STRING char AMA_BleApplyPartnerOkString[];

/* AMA SPP */
EXTERN LOG_STRING char AMA_SppNullSinkString[];
EXTERN LOG_STRING char AMA_SppStateSinkString[];
EXTERN LOG_STRING char AMA_SppNullLinkInfoString[];
EXTERN LOG_STRING char AMA_SppInvalidClaimString[];
EXTERN LOG_STRING char AMA_SppStatusString[];
EXTERN LOG_STRING char AMA_SppConnIndHandlerString[];
EXTERN LOG_STRING char AMA_SppGetDataLengthString[];
EXTERN LOG_STRING char AMA_SppGetDataNoAmaLinkString[];
EXTERN LOG_STRING char AMA_SppGetDataNoSppLinkString[];
EXTERN LOG_STRING char AMA_SppGetDataOkString[];
EXTERN LOG_STRING char AMA_SppApplyAgentNoEmptyLinkString[];
EXTERN LOG_STRING char AMA_SppApplyAgentOkString[];
EXTERN LOG_STRING char AMA_SppApplyPartnerOkString[];

/* TRSPX APP*/
EXTERN LOG_STRING char APP_TrspxBtTimerString[];
EXTERN LOG_STRING char APP_TrspxBtStartServiceString[];
EXTERN LOG_STRING char APP_TrspxBtConnCfm1String[];
EXTERN LOG_STRING char APP_TrspxBtConnCfm2String[];
EXTERN LOG_STRING char APP_TrspxBtDisconnCfmString[];
EXTERN LOG_STRING char APP_TrspxBtUartTxString[];
EXTERN LOG_STRING char APP_TrspxBtRxDoneString[];
EXTERN LOG_STRING char APP_TrspxBtRxDropDataString[];
EXTERN LOG_STRING char APP_TrspxBtRegAllProfileString[];
EXTERN LOG_STRING char APP_TrspxUartTxString[];
EXTERN LOG_STRING char APP_TrspxUartClaimFailString[];
EXTERN LOG_STRING char APP_TrspxUartBtTxString[];
EXTERN LOG_STRING char APP_TrspxUartBtTxTimeoutString[];
EXTERN LOG_STRING char APP_TrspxUartTimerString[];

/* TRSPX */
EXTERN LOG_STRING char TRSPX_BleConnectedString[];

EXTERN LOG_STRING char SM_AddAuthList1String[];
EXTERN LOG_STRING char SM_AddAuthList2String[];
EXTERN LOG_STRING char SM_AddAuthList3String[];
EXTERN LOG_STRING char SM_GetAuthListString[];

EXTERN LOG_STRING char SM_WriteLtk1String[];
EXTERN LOG_STRING char SM_WriteLtk2String[];
EXTERN LOG_STRING char SM_WriteLtk3String[];
EXTERN LOG_STRING char SM_ReadLtk1String[];
EXTERN LOG_STRING char SM_ReadLtk2String[];

EXTERN LOG_STRING_NEW char APP_KeyIndexLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_ActionLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_KeyEventLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_StateLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_MediaEvent1LogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_MediaEvent2LogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_MediaEvent3LogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_MediaEvent4LogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_VPLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_RTLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_VPRTLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_ProfileLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_BatStatusLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_VolComponentLogString[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char APP_PeqComponentLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_PeqPhaseLogString[][MAX_LOG_STRING];




EXTERN LOG_STRING_NEW char APP_CallHoldActiveLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_CallActiveLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_CallSetupLogString[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char APP_VoiceCommandLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_AudioTypeLogString[][MAX_LOG_STRING];

EXTERN LOG_STRING char MSG_CallbackTimeConsuming[];
EXTERN LOG_STRING char MSG_MessageSendInternalFromISR_Return3[];
EXTERN LOG_STRING char MSG_MessageSendInternalFromISR_Return2[];
EXTERN LOG_STRING char MSG_MessageSendInternalFromISR_Return1[];
EXTERN LOG_STRING char MSG_MessageSendInternalFromISR_Return1DPC[];
EXTERN LOG_STRING char MSG_MessageSendInternal_Return3[];
EXTERN LOG_STRING char MSG_MessageSendInternal_Return2[];
EXTERN LOG_STRING char MSG_MessageSendInternal_Return1[];
EXTERN LOG_STRING char MSG_MessageSendInternal_Return1DPC[];
EXTERN LOG_STRING char MSG_PendingStatus[];
EXTERN LOG_STRING char MSG_StartDelayMessageFromISR[];
EXTERN LOG_STRING char MSG_StartDelayMessage[];
EXTERN LOG_STRING char MSG_DPCCallback[];
EXTERN LOG_STRING char MSG_CallDelayedMessageHandler[];
EXTERN LOG_STRING char MSG_CancelDelayMessage[];
EXTERN LOG_STRING char MSG_MessageDumpPendingString[];
EXTERN LOG_STRING char MSG_MessageNullHandler[];
EXTERN LOG_STRING char MSG_MessageDPCCallbackEnter[];
EXTERN LOG_STRING char MSG_MessageDPCCallbackLeave[];

EXTERN LOG_STRING char NVKEY_LOG_RAMBufferedWriting[];
EXTERN LOG_STRING char NVKEY_LOG_RAMBufferedFlushed[];
EXTERN LOG_STRING char NVKEY_LOG_SectorSwapping[];
EXTERN LOG_STRING char NVKEY_LOG_Write[];
EXTERN LOG_STRING char NVKEY_LOG_CleanInvalidThreshold[];
EXTERN LOG_STRING char NVKEY_LOG_SectorStatusReport[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYNotFound[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYDecryptionCRCError[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYDecryptionLengthTooShort[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYDecryptionNoMemory[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYWriteError[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYWriteSameContents[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEYFlashAddressDoubleEnter[];
EXTERN LOG_STRING char NVKEY_LOG_AddNonExistedNVKEY_Non_FOTA[];
EXTERN LOG_STRING char NVKEY_LOG_AddNonExistedNVKEY_FOTA[];
EXTERN LOG_STRING char NVKEY_LOG_NVKEY_GetFlashAddress_Wrong_Tag[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_WriteFullKey[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_ReclaimCheck[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_ReadFullKey[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_Next[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_GetPayloadFlashAddress[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_GetPayloadLength[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_Flash_Access_Disable[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_Flash_Access_Enable[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_SetDecryptionKey[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_GetPayloadFlashAddress_Enter[];
EXTERN LOG_STRING char NVKEY_LOG_Enter_NVKEY_GetPayloadFlashAddress_Leave[];
EXTERN LOG_STRING char NVKEY_LOG_Semaphore_Take[];
EXTERN LOG_STRING char NVKEY_LOG_Semaphore_Release[];

EXTERN LOG_STRING char ROFS_LOG_BaseAddressNotFound[];
EXTERN LOG_STRING char ROFS_LOG_FileNotFound[];

EXTERN LOG_STRING char FLS_MetFileLengthZero[];
EXTERN LOG_STRING char FLS_SDCardReadInterval[];

/*APP System*/
EXTERN LOG_STRING_NEW char success_log_string[];
EXTERN LOG_STRING_NEW char fail_log_string[];

/*APP HFP*/
EXTERN LOG_STRING_NEW char APP_HFPMsg[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_HFPScoLinkType[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_HFPScoHandle[][MAX_LOG_STRING];

/*APP AVRCP*/
EXTERN LOG_STRING_NEW char APP_AvrcpRegNotifyRspEvtID[][64];
EXTERN LOG_STRING_NEW char APP_AvrcpIndID[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_AvrcpTimerID[][MAX_LOG_STRING];

/*USB*/
EXTERN LOG_STRING char USB_EmmcInitFailLogString[];
EXTERN LOG_STRING char USB_EmmcInitPassLogString[];
EXTERN LOG_STRING char USB_SdcInitFailLogString[];
EXTERN LOG_STRING char USB_SdcInitPassLogString[];
EXTERN LOG_STRING char USB_MassStorageInitFailLogString[];
EXTERN LOG_STRING_NEW char NTC_LevelChargerString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char NTC_LevelUNChargerString[][MAX_LOG_STRING];

/* BT Lib log */

/* Gap */

EXTERN LOG_STRING char BtUtilGetTicketString[];
EXTERN LOG_STRING char Gap_Link_SetAttr_SniffLockString[];
EXTERN LOG_STRING char Gap_Link_SetAttr_DetachingString[];
EXTERN LOG_STRING char Gap_Link_GetAttr_DetachingString[];
EXTERN LOG_STRING char Gap_Link_SetAttr_ClearString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_MODE_CHANGE_INDString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_QOS_SETUP_COMPLETE_INDString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CMD_SEND_CFMString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CMD_SEND_CFM1String[];
EXTERN LOG_STRING char BT_INTERNAL_SET_LINK_MODE_REQString[];
EXTERN LOG_STRING char BT_INTERNAL_QOS_SETUP_REQString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CMD_SEND_CFMLinkString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CMD_SEND_CFMLink1String[];
EXTERN LOG_STRING char BT_INTERNAL_GAP_PAGING_REQString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_LINK_KEY_REQUEST_INDString[];
EXTERN LOG_STRING char GAP_detach_LinkString[];
EXTERN LOG_STRING char GAP_detach_Link1String[];
EXTERN LOG_STRING char Security_DB_DeleteAllLinkKeyString[];
EXTERN LOG_STRING char Security_DB_DeleteLinkKeyString[];
EXTERN LOG_STRING char Security_DB_WriteLinkKeyString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CONNECT_COMPLETE_INDString[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CONNECT_COMPLETE_IND1String[];
EXTERN LOG_STRING char BT_INTERNAL_HCI_CONNECT_COMPLETE_IND2String[];
EXTERN LOG_STRING char Gap_Link_SetAttr_StateString[];
EXTERN LOG_STRING char Gap_BleDisableMaskString[];
EXTERN LOG_STRING char Gap_BleEnableMaskString[];
EXTERN LOG_STRING char Gap_EncryptionString[];

EXTERN LOG_STRING char Gap_RhoNotAllow1_String[];
EXTERN LOG_STRING char Gap_RhoNotAllow2_String[];
EXTERN LOG_STRING char Gap_RhoNotAllow3_String[];


EXTERN LOG_STRING_NEW char Rfcomm_Dlci_state_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char Rfcomm_Session_state_String[][MAX_LOG_STRING];


/*Profile AVRCP*/
EXTERN LOG_STRING_NEW char Avrcp_Opcode_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char AVRCP_TX_RESULT_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char AVRCP_TX_CMD_RSP_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char AVRCP_PDU_ID_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char AVRCP_OPID_STR[][MAX_LOG_STRING];

/*Profile HFP*/
EXTERN LOG_STRING_NEW char HFP_TIMER_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_SDP_CFM_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_RFCOMM_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_SCO_CODEC_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_PROFILE_STATE_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_CMD_STATE_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING char HFP_RESULT_CODE_LOG_STR[];
EXTERN LOG_STRING_NEW char HFP_RESULT_CODE_STR[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char HFP_PUSH_CMD_STR[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char PM_ConflictStateLogString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char PM_ConflictEventLogString[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char app_audio_bist_cp_mode_log_string[][MAX_LOG_STRING];


EXTERN LOG_STRING_NEW char Connect_String[];
EXTERN LOG_STRING_NEW char Disconnect_String[];
EXTERN LOG_STRING_NEW char MediaConnect_String[];
EXTERN LOG_STRING_NEW char Register_String[];
EXTERN LOG_STRING_NEW char UnRegister_String[];
EXTERN LOG_STRING_NEW char NoApp_String[];
EXTERN LOG_STRING_NEW char NoResource_String[];
EXTERN LOG_STRING_NEW char Success_String[];
EXTERN LOG_STRING_NEW char Conflict_String[];
EXTERN LOG_STRING_NEW char AppReject_String[];
EXTERN LOG_STRING_NEW char Unknown_ID_String[];
EXTERN LOG_STRING_NEW char Accept_ID_String[];
EXTERN LOG_STRING_NEW char AVDTP_SignalIDString[14][MAX_LOG_STRING];

/* MCSync */
EXTERN LOG_STRING_NEW char MCSync_IFType_String[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char MCSync_IF_AppOpcode_String[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char MCSync_RHO_STATUS_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char MCSync_RHO_REASON_String[][MAX_LOG_STRING];



/* APP RHO */
EXTERN LOG_STRING_NEW char APP_RHO_MODULE_String[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char APP_McsReconn_State_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_McsReconn_Action_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_McsReconn_Event_String[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_McsReconn_TimerId_String[][MAX_LOG_STRING];

/*Lib Charger*/

/*Smart Charger Case*/
EXTERN LOG_STRING_NEW char APP_SmartCase_DrvEvt_String[][MAX_LOG_STRING];

EXTERN LOG_STRING_NEW char LibDrv_SmartCase_StateString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char LibDrv_Charger_StateString[][MAX_LOG_STRING];

/* App Smart Case*/
EXTERN LOG_STRING_NEW char APP_McSyncLinkString[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char APP_LinkConnString[][MAX_LOG_STRING];

/* A2DP Codec */
EXTERN LOG_STRING_NEW char a2dp_codec_log_string[][MAX_LOG_STRING];
EXTERN LOG_STRING_NEW char a2dp_vendor_codec_log_string[];

/*xiaoai mcsync*/
EXTERN LOG_STRING_NEW char xiaoai_mcsync_cb_string[][MAX_LOG_STRING];

/*EXTERN LOG_STRING char DBG_HANG_01_String[];
EXTERN LOG_STRING char DBG_HANG_02_String[];
EXTERN LOG_STRING char DBG_HANG_03_String[];
EXTERN LOG_STRING char DBG_HANG_04_String[];
EXTERN LOG_STRING char DBG_HANG_05_String[];
EXTERN LOG_STRING char DBG_HANG_06_String[];
EXTERN LOG_STRING char DBG_HANG_07_String[];
EXTERN LOG_STRING char DBG_HANG_08_String[];
EXTERN LOG_STRING char DBG_HANG_09_String[];
EXTERN LOG_STRING char DBG_HANG_10_String[];
EXTERN LOG_STRING char DBG_HANG_11_String[];
EXTERN LOG_STRING char DBG_HANG_12_String[];
EXTERN LOG_STRING char DBG_HANG_13_String[];
EXTERN LOG_STRING char DBG_HANG_14_String[];*/


#define logPrint(x,y,z,j, ...)
#define logPrintBlocking(x,y,z,j, ...)


#endif /* _LOG_H_ */

