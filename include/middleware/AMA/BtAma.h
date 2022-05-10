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

#ifndef _BTAMA_H_
#define _BTAMA_H_

#include "Ama_Interface.h"

/**************************************************************************************************
* API (Connection Relative)
**************************************************************************************************/

/**
 * @brief       Check whether the AMA is enabled according to the NVKEY configuration.
 *
 * @return      If the NVKEY configured the AMA enabled, return true; otherwise return false.
 */
BOOL AMA_Target_IsEnable(void);

/**
 * @brief       Start the AMA service with the maximum link count.
 *
 * @param[in]   app             A #AMA_HandlerData structure pointer which used to handle the AMA core event.
 * @param[in]   maxLinkNum      The maximum link count of the AMA. Typically should be 1.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 *              When the start process is complete, the event #AMA_START_SERVICE_CFM should be received in the #app handler.
 */
AMAStatus AMA_Target_StartService(Handler app, U8 maxLinkNum);

/**
 * @brief       Stop the AMA service.
 *
 * @param[in]   app             A #AMA_HandlerData structure pointer which used to handle the AMA core event.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 *              When the stop process is complete, the event #AMA_STOP_SERVICE_CFM should be received in the #app handler.
 */
AMAStatus AMA_Target_StopService(Handler app);

/**
 * @brief       Start the BLE advertisement which contains the advertise data that is required in the AMA spec.
 *
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_EnableAdv(void);

/**
 * @brief       Stop the BLE advertisement.
 *
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_DisableAdv(void);

/**
 * @brief       Disconnect the connection according to the BT address.
 *
 * @param[in]   pBdAddr         The BT address which is to be disconnected.
 *              If the BT address is a BLE random address, disconnect the BLE link.
 *              If the BT address is a EDR address, disconnect the SPP link.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_DisconnectRequest(BD_ADDR_T *pBdAddr);

/**
 * @brief       Send the reset-connection message to the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address which connection should be reset.
 * @param[in]   timeout         The reset connection timeout parameter (in seconds). A value of 0 indicates an indefinite timeout.
 * @param[in]   forceDisconnect A boolean flag indicating if the recipient of the message should close the session.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_ResetConnection(BD_ADDR_T *pBdAddr, U32 timeout, BOOL forceDisconnect);

/**************************************************************************************************
* API (SPEECH)
**************************************************************************************************/

/**
 * @brief       Send the start speech message to the mobile phone to initiate a SpeechRecognizer.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   param           The start speech parameter configuration.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_StartSpeech(BD_ADDR_T *pBdAddr, AMA_START_SPEECH_T *param);

/**
 * @brief       Response the ProvideSpeech message to the mobile phone to indicate the ProvideSpeech execute result.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   param           The ProvideSpeech parameter configuration.
 * @param[in]   errorCode       The executed result of the ProvideSpeech response.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_ProvideSpeechResponse(BD_ADDR_T *pBdAddr, AMA_PROVIDE_SPEECH_T *param, AMA_ERROR_CODE_E errorCode);

/**
 * @brief       Send the StopSpeech message to the mobile phone to stop the SpeechRecognizer.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_StopSpeech(BD_ADDR_T *pBdAddr);

/**
 * @brief       Response the StopSpeech message to the mobile phone to indicate the StopSpeech executed result.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_StopSpeechResponse(BD_ADDR_T *pBdAddr);

/**
 * @brief       Send the EndPointSpeech message to the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_EndpointSpeech(BD_ADDR_T *pBdAddr);

/**
 * @brief       Response the EndPointSpeech message to the mobile phone to indicate the EndPointSpeech executed result.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_EndpointSpeechResponse(BD_ADDR_T *pBdAddr);

/**
 * @brief       Get the current speech state.
 *
 * @param[in]   pBdAddr         The BT address according to.
 * @return U8   Return the current speech state.
 *              According to the #AMA_SPEECH_STATE_E Enumuration.
 */
U8 AMA_Target_GetSpeechState(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* API (STATE)
**************************************************************************************************/

/**
 * @brief       Send the GetState message to the mobile phone to retrieve the state.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   feature         The state of which feature to retrieve.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_GetState(BD_ADDR_T *pBdAddr, AMA_STATE_FEATURE_E feature);

/**
 * @brief       Response the GetState message that is sent from the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   state           The corresponding state values.
 * @param[in]   errorCode       The executed result of the GetState message.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_GetStateResponse(BD_ADDR_T *pBdAddr, AMA_STATE_T *state, AMA_ERROR_CODE_E errorCode);

/**
 * @brief       Response the SetState message that is sent from the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   errorCode       The executed result of the SetState message.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_SetStateResponse(BD_ADDR_T *pBdAddr, AMA_ERROR_CODE_E errorCode);

/**
 * @brief       Send the SynchronizeState message to the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   state           The SynchronizeState parameter configuration.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_SynchronizeState(BD_ADDR_T *pBdAddr, AMA_STATE_T *state);

/**
 * @brief       Response the SynchronizeState message from the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_SynchronizeStateResponse(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* API (MEDIA)
**************************************************************************************************/

/**
 * @brief       Send the IssueMediaControl message to the mobile phone to control music being played through Alexa streaming if the media playback on the phone doesn't respond to regular Bluetooth link commands.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   control         The control parameter to indicate the Playback to execute.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_IssueMediaControl(BD_ADDR_T *pBdAddr, AMA_MEDIA_CONTROL_E control);

/**
 * @brief       Response the IssueMediaControl message that is sent from the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   errorCode       The executed result of the IssueMediaControl message.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_IssueMediaControlResponse(BD_ADDR_T *pBdAddr, AMA_ERROR_CODE_E errorCode);

/**************************************************************************************************
* API (Cellular Calling)
**************************************************************************************************/

/**
 * @brief       Response the ForwardATCommand message that is sent from the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   errorCode       The executed result of the ForwardATCommand message.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_ForwardATCommandResponse(BD_ADDR_T *pBdAddr, AMA_ERROR_CODE_E errorCode);

/**
 * @brief       Report the IncomingCall status to mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   callerNumber    The incoming call phone number.
 * @param[in]   length          The length of the incoming call phone number.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_IncommingCall(BD_ADDR_T *pBdAddr, U8 *callerNumber, U8 length);

/**************************************************************************************************
* API (MISC)
**************************************************************************************************/

/**
 * @brief       Reset the AMA core status, including reset the NVKEY configuration.
 *
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_FactoryReset(void);

/**
 * @brief       Report the voice assistant has been override by others.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_OverrideAssistant(BD_ADDR_T *pBdAddr);

/**
 * @brief       Send the GetCentralInformation message to mobile phone to get central information.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_GetCentralInformation(BD_ADDR_T *pBdAddr);

/**
 * @brief       Send the OverrideAssistant and Setup configuration to the mobile phone.
 *
 * @param[in]   pBdAddr         The BT address to which message should be sent.
 * @param[in]   needsAssistantOverride  The OverrideAssistant value.
 * @param[in]   needsSetup      The SetUp value.
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_SetDeviceConfiguration(BD_ADDR_T *pBdAddr, BOOL needsAssistantOverride, BOOL needsSetup);

/**
 * @brief       Get the current connection Maximum Transfer Unit(MTU) value.
 *
 * @param[in]   pBdAddr         The BT address from which to retrieve.
 * @return      Return the current connection MTU value.
 */
U16 AMA_Target_GetMtu(BD_ADDR_T *pBdAddr);

/**************************************************************************************************
* API (Stream)
**************************************************************************************************/

/**
 * @brief       Claim a voice stream buffer according to the MTU value. The claimed buffer is used to fill the voice data.
 *
 * @param[in]   size            The length to try to claim from the system.
 * @return      The claimed buffer from the system.
 */
U8 *AMA_Target_VoiceStreamSinkClaim(U32 size);

/**
 * @brief       Flush the voice stream data to the mobile phone which is stored in the claim buffer.
 *
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_VoiceStreamSinkFlush(void);

/**
 * @brief       Free the claim buffer which is allocated from the #AMA_Target_VoiceStreamSinkClaim
 *
 * @return      If the operation is successful, return #AMA_STATUS_OK; Otherwise, return failed.
 */
AMAStatus AMA_Target_VoiceStreamSinkFree(void);

/**************************************************************************************************
* API (LOCALE)
**************************************************************************************************/

/**
 * @brief Response the SetLocale message with error code
 *
 * @param pBdAddr The device which send to
 * @param error The response error code
 * @return AMAStatus If response succeed, return AMA_STATUS_OK, others failed.
 */
AMAStatus AMA_Target_SetLocaleResponse(BD_ADDR_T *pBdAddr, AMA_ERROR_CODE_E error);


#define AMA_INVALID_LOCALE_ID               0xFF
/**
 * @brief The locale information
 *
 */
typedef struct {
    unsigned char           locale_id; /* The locale ID */
} AMA_LocaleInfo_T;

/**
 * @brief The GetLocale response
 *
 */
typedef struct {
    AMA_LocaleInfo_T        *supported_locale_info; /* The list of the supported locales */
    AMA_LocaleInfo_T        current_locale;        /* The current locale */
    unsigned char           supported_locale_count; /* The supported locale count */
} AMA_GetLocaleInfoRsp_T;

/**
 * @brief Response the GetLocale message
 *
 * @param pBdAddr The device which send to
 * @param locale_info The response locale info
 *                    If the error code is not supported, and the locale info, return error.
 *                    If the supported locale info in locale_info is NULL or length is 0, return error.
 * @param error The error code
 * @return AMAStatus If response succeed, return AMA_STATUS_OK, others failed.
 */
AMAStatus AMA_Target_GetLocalesResponse(BD_ADDR_T *pBdAddr, AMA_GetLocaleInfoRsp_T *locale_info, AMA_ERROR_CODE_E error);


/**************************************************************************************************
* API (BtAma)
**************************************************************************************************/
void 	AMA_Target_RegisterTransform(TRANSFORM transform, U32 bitRate);
void 	AMA_Target_ClearCtlInfo(void);
void 	AMA_Target_SourceConfigure(SOURCE source, stream_config_type type, U32 value);
void 	AMA_Target_ControlEncodeStream(SINK sink);

/**************************************************************************************************
* API (RHO)
**************************************************************************************************/
BOOL BtAmaLock();
BOOL BtAmaUnlock();
BOOL BtAmaIsAllow();
U16 BtAmaGetRhoDataLength();
BOOL BtAmaGetRhoData(U8 *pBuffer);
BOOL BtAmaApplyAgentRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);
BOOL BtAmaApplyPartnerRhoData(BD_ADDR_T *pBdAddr, U8 *pAwsMceData, U16 length);

AMAStatus AMA_Target_AddDataApplyAgent(U8 *pPartner, U16 length, U8 *pKeyId, U8 numOfKeyId);
AMAStatus AMA_Target_AddDataApplyPartner(U8 *pAgent, U16 length, U8 *pKeyId, U8 numOfKeyId);
AMAStatus AMA_Target_GetData(void);

#endif	/* _BTAMA_H_ */

