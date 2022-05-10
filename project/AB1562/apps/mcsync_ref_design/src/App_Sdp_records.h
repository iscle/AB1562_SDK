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

#ifdef AIROHA_BT_HID_ENABLE

/*HID SDP*/
#define HID_COTROL_PSM 0x0011
#define EN_ENGLISH 0x656E
#define UTF8_ENCODE 0x006A
#define PL_BASEID 0x0100
#define VERSION_1V1 0x0101
#define HID_CORE_VER 0x0111
#define REMOTE_CONTROL 0x0C
#define MOUSE 0x80
#define COUNTRY_USA 0x21
#define COUNTRY_NONE 0x00
#define FEATURE_SUPPORTED 0x01
#define FEATURE_NOT_SUPPORTE 0x00
#define HID_REPORT_DESCRIPTOR 0x22
#define HID_PHYSICAL_DESCRIPTOR 0x23

#define ENGLISH_US 0x0409

#define ATTRID_HIDParseVersion 0x0201
#define ATTRID_HIDDeviceSubclass 0x0202
#define ATTRID_HIDCountryCode 0x0203
#define ATTRID_HIDVirtualCable 0x0204
#define ATTRID_HIDReconnectInitiate 0x0205
#define ATTRID_HIDDescriptorList 0x0206
#define ATTRID_HIDLANGIDBase 0x0207
#define ATTRID_HIDBatteryPower 0x0209
#define ATTRID_HIDRemoteWake 0x020A
#define ATTRID_HIDBootDevice 0x020E
#define ATTRID_HIDSupervsionTimeout 0x020C
#define ATTRID_HIDNormallyConnectable 0x020D
#define ATTRID_HIDSSRHostMaxLatency 0x020F
#define ATTRID_HIDSSRHostMinTimeout 0x0210

/*HID SDP Record  (gSPP_SERVICE_RECORD)*/
const U8 HIDServiceRecordHandle[]={         SERVICE_HANDLE32(0x0002)                                                         };
const U8 HIDServiceClassIdList[] = {        DEDEF_SEQ8(3), SERVICE_UUID16(SERVICE_UUID_HUMAN_INTERFACE_DEVICE)               };
const U8 HIDProtocolDescriptorList[] = {    DEDEF_SEQ8(13), 
	                                        DEDEF_SEQ8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),PSM_U16(HID_COTROL_PSM),  
                                            DEDEF_SEQ8(3), PROTOCOL_UUID16(PROTOCOL_UUID_HID)                                };
const U8 HIDLanguageBaseAttributeIDList[]={ DEDEF_SEQ8(9), DEDEF_U16(EN_ENGLISH), DEDEF_U16(UTF8_ENCODE),DEDEF_U16(PL_BASEID)};

const U8 HIDProfileDescriptorList[] = {     DEDEF_SEQ8(8), 
	                                        DEDEF_SEQ8(6), DEDEF_U16(PROFILE_UUID_HID), VERSION_U16(VERSION_1V1)             };
const U8 HIDAddiProtocolDescirptorList[]={  DEDEF_SEQ8(12),
	                                        DEDEF_SEQ8(10),
	                                        DEDEF_SEQ8(3), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP),
                                            DEDEF_SEQ8(3), PROTOCOL_UUID16(PROTOCOL_UUID_HID),                               };
const U8 HIDServiceName[] = {               DEDEF_TEXT8(8), 'H', 'I', 'D', '_', 'T', 'E', 'S', 'T'                           };
const U8 HIDServiceDescription[] = {        DEDEF_TEXT8(8), 'B', 'T', '-', 'M', 'O', 'U', 'S', 'E'                           };
const U8 HIDProviderName[] = {              DEDEF_TEXT8(6), 'A', 'I', 'R', 'O', 'H', 'A'                                     };


const U8 HIDParseVersion[]={                DEDEF_U16(HID_CORE_VER)                                                          };
const U8 HIDDeviceSubclass[]={              DEDEF_U8(MOUSE)                                                                  };
const U8 HIDCountryCode[]={                 DEDEF_U8(COUNTRY_NONE)                                                            };
const U8 HIDVirtualCable[]={                DEDEF_BOOL(FEATURE_SUPPORTED)                                                    };
const U8 HIDReconnectInit[]={               DEDEF_BOOL(FEATURE_SUPPORTED)                                                    };
/*
const U8 HIDDescriptorList[]={              
	                                        DEDEF_SEQ8(61),    //43
	                                        DEDEF_SEQ8(59),    //41
	                                        DEDEF_U8(HID_REPORT_DESCRIPTOR),
	                                        DEDEF_TEXT8(55),  //37
	                                        
	                                        0x05, 0x0C, 	   // Usage Page (Consumer)
  											0x09, 0x01, 	   // Usage (Consumer Control)
  											0xA1, 0x01, 	   // Collection (Application)
  											0x09, 0x42, 	   //	Usage (Menu Up)
  											0x09, 0x43, 	   //	Usage (Menu Down)
  											0x09, 0x44, 	   //	Usage (Menu Left)
  											0x09, 0x45, 	   //	Usage (Menu Right)
  											0x09, 0x41, 	   //	Usage (Menu Pick)
  											0x09, 0x40, 	   //	Usage (Menu)
  											0x0A, 0x23, 0x02,  //	Usage (AC Home)
  											0x0A, 0x24, 0x02,  //	Usage (AC Back)
  											0x15, 0x00, 	   //	Logical Minimum (0)
  											0x25, 0x01, 	   //	Logical Maximum (1)
  											0x75, 0x01, 	   //	Report Size (1)
  											0x95, 0x08, 	   //	Report Count (8)
  											0x85, 0x04, 	   //	Report ID (4)
  											0x81, 0x02, 	   //	Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  											0xC0,			   // End Collection
                                            // 37 bytes

                                            0x06, 0x01, 0xFF,  // Usage Page (Vendor Defined 0xFF01)
										    0x09, 0x02,        // Usage (tmp usage)
										    0xA1, 0x02,        //   Collection (Logical)

										    0x85, 0x05,        //   Report ID (5)
										    0x09, 0x04,        //   Usage (tmp usage)
										    0x75, 0x08,        //   Report Size (8)
										    0x95, 0x01,        //   Report Count (1)
										    0x91, 0x02,        //   Output

										    0xC0,              // End Collection
                                            
  											// 55  bytes 

  											                         };  */




const U8 HIDDescriptorList[]={ 
	                                        DEDEF_SEQ8(121),     
											DEDEF_SEQ8(119),    
											DEDEF_U8(HID_REPORT_DESCRIPTOR),
											DEDEF_TEXT8(115),  
							
											0x05, 0x01, // Usage Page (Generic Desktop)
											0x09, 0x02, // Usage (Mouse)
											0xA1, 0x01, // Collection (Application)
											
											0x85, 0x01, // Report ID (0x01)
											0x09, 0x01, // Usage (Pointer)
											0xA1, 0x00, // Collection (Physical)
											0x05, 0x09, // Usage Page (Buttons)
											0x19, 0x01, // Usage Minimum (1)
											0x29, 0x03, // Usage Maximum (3)
											0x15, 0x00, // Logical Minimum (0)
											0x25, 0x01, // Logical Maximum (1)
											0x75, 0x01, // Report Size (1)
											0x95, 0x03, // Report Count (3)
											0x81, 0x02, // Input (Data, Variable, Absolute)
											0x75, 0x05, // Report Size (5)
											0x95, 0x01, // Report Count (1)
											0x81, 0x01, // Input (Constant)
											0x05, 0x01, // Usage Page (Generic Desktop)
											0x09, 0x30, // Usage (X)
											0x09, 0x31, // Usage (Y)
											0x15, 0x81, // Logical Minimum (-127)
											0x25, 0x7F, // Logical Maximum (+127)
											0x75, 0x08, // Report Size (8)
											0x95, 0x02, // Report Count (2)
											0x81, 0x06, // Input (Data, Variable, Relative)
											0xC0, // End Collection (Physical)
											0xC0, // End Collection Application
                    

											
											0x06, 0xF0, 0xFF, // Usage Page (0xFFF0, Vendor Specific)
											0x09, 0x01, // Usage (0x01, Vendor Specific)
											0xA1, 0x01, // Collection (Application)
											
											// Report for testing
											
											0x85, 0x03, // Report ID (0x03)
											0x06, 0xF0, 0xFF, // Usage Page (0xFFF0, Vendor Specific)
											0x09, 0x04, // Usage (0x04, Vendor Specific)
											0x75, 0x08, // Report Size (8)
											0x95, 0x01, // Report Count (1)
											0x15, 0x00, // Logical Minimum (0)
											0x26, 0xFF, 0x00, // Logical Maximum (255)
											0xB1, 0x12, // Feature (Absolute, Data, No Pref, Variable)
											 // Feature Report for testing
											 
											0x85, 0x04, // Report ID (0x04)
											0x06, 0xF0, 0xFF, // Usage Page (0xFFF0, Vendor Specific)
											0x09, 0x05, // Usage (0x05, Vendor Specific)
											0x75, 0x08, // Report Size (8)
											0x95, 0x30, // Report Count (48)
											0x15, 0x00, // Logical Minimum (0)
											0x26, 0xFF, 0x00, // Logical Maximum (255)
											0xB1, 0x12, // Feature (Absolute, Data, No Pref, Variable)
											 // Output Report for testing
											 
											0x85, 0x05, // Report ID (0x05)
											0x06, 0xF0, 0xFF, // Usage Page (0xFFF0, Vendor Specific)
											0x09, 0x05, // Usage (0x05, Vendor Specific)
											0x75, 0x08, // Report Size (8)
											0x96, 0x9E, 0x02, // Report Count (670)
											0x15, 0x00, // Logical Minimum (0)
											0x26, 0xFF, 0x00, // Logical Maximum (255)
											0x91, 0x02, // Output (Data, Variable, Relative)
											0xC0 // End Collection (Application)   
};



     										                                                                             
const U8 HIDLANGINBaseList[]={              DEDEF_SEQ8(8),
	                                        DEDEF_SEQ8(6),DEDEF_U16(ENGLISH_US),DEDEF_U16(0x0100)                           };
const U8 HIDBatteryPower[]={                DEDEF_BOOL(FEATURE_SUPPORTED)                                                   };
const U8 HIDRemoteWake[]={                  DEDEF_BOOL(FEATURE_SUPPORTED)                                                   };
const U8 HIDSupervisionTimeout[]={          DEDEF_U16(0x0C80)                                                               };
const U8 HIDNormallyConnectable[]={         DEDEF_BOOL(FEATURE_NOT_SUPPORTE)                                                };
const U8 HIDBootDevice[]={                  DEDEF_BOOL(FEATURE_SUPPORTED)                                                   };
const U8 HIDSSRHostMaxLatency[]={           DEDEF_U16(0x0640)                                                               };
const U8 HIDSSRHostMinTimeout[]={           DEDEF_U16(0x0C80)                                                               };



const BT_SDP_ATTRIBUTE_T gHID_SERVICE_RECORD[] =
{
    { (ATTRID_ServiceRecordHandle), sizeof(HIDServiceRecordHandle), (U8*)HIDServiceRecordHandle },
    { (ATTRID_ServiceClassIdList), sizeof(HIDServiceClassIdList), (U8*)HIDServiceClassIdList },
    { (ATTRID_ProtocolDescriptorList), sizeof(HIDProtocolDescriptorList), (U8*)HIDProtocolDescriptorList },
    { (ATTRID_LangBaseAttributeIdList), sizeof(HIDLanguageBaseAttributeIDList), (U8*)HIDLanguageBaseAttributeIDList },
    { (ATTRID_ProfileDescriptorList), sizeof(HIDProfileDescriptorList), (U8*)HIDProfileDescriptorList },
    { (ATTRID_AddProtocolDescriptorLists), sizeof(HIDAddiProtocolDescirptorList), (U8*)HIDAddiProtocolDescirptorList },
    { (ATTRID_ServiceName), sizeof(HIDServiceName), (U8*)HIDServiceName },
	{ (ATTRID_ServiceDescription), sizeof(HIDServiceDescription), (U8*)HIDServiceDescription },
	{ (ATTRID_ProviderName), sizeof(HIDProviderName), (U8*)HIDProviderName },	
    { (ATTRID_HIDParseVersion), sizeof(HIDParseVersion), (U8*)HIDParseVersion },
	{ (ATTRID_HIDDeviceSubclass), sizeof(HIDDeviceSubclass), (U8*)HIDDeviceSubclass },
	{ (ATTRID_HIDCountryCode), sizeof(HIDCountryCode), (U8*)HIDCountryCode },
	{ (ATTRID_HIDVirtualCable), sizeof(HIDVirtualCable), (U8*)HIDVirtualCable },
	{ (ATTRID_HIDReconnectInitiate), sizeof(HIDReconnectInit), (U8*)HIDReconnectInit },
	{ (ATTRID_HIDDescriptorList), sizeof(HIDDescriptorList), (U8*)HIDDescriptorList },
    { (ATTRID_HIDLANGIDBase), sizeof(HIDLANGINBaseList), (U8*)HIDLANGINBaseList },
    { (ATTRID_HIDBatteryPower), sizeof(HIDBatteryPower), (U8*)HIDBatteryPower },
    { (ATTRID_HIDRemoteWake), sizeof(HIDRemoteWake), (U8*)HIDRemoteWake },
    { (ATTRID_HIDBootDevice), sizeof(HIDBootDevice), (U8*)HIDBootDevice },
	{ (ATTRID_HIDSupervsionTimeout), sizeof(HIDSupervisionTimeout), (U8*)HIDSupervisionTimeout },
	{ (ATTRID_HIDNormallyConnectable), sizeof(HIDNormallyConnectable), (U8*)HIDNormallyConnectable },
	{ (ATTRID_HIDSSRHostMaxLatency), sizeof(HIDSSRHostMaxLatency), (U8*)HIDSSRHostMaxLatency },
	{ (ATTRID_HIDSSRHostMinTimeout), sizeof(HIDSSRHostMinTimeout), (U8*)HIDSSRHostMinTimeout },
		
};

#endif


#define SERVICE_HANDLE_PNP                 (4)
#define AIROHA_COMPANY_ID					0x0094
#define VENDOR_ID_SOURCE_BLUETOOTH			0x0001

/***************************************
    PNP SDP Record  (gPNP_SERVICE_RECORD)
***************************************/
const U8 PNPServiceRecordHandle [] =		{SERVICE_HANDLE32(SERVICE_HANDLE_PNP)};
const U8 PNPServiceClassIdList [] = 		{DEDEF_SEQ8(3), SERVICE_UUID16(SERVICE_UUID_PNP_INFORMATION) };
const U8 PNPSpecifcationID [] = 		    {VERSION_U16(0x0103) };
const U8 PNPVendorID [] = 		            {DEDEF_U16(AIROHA_COMPANY_ID) };
const U8 PNPProductID [] = 		            {DEDEF_U16(0x0123) };
const U8 PNPProductVersion [] = 	    	{DEDEF_U16(0x1000) };
const U8 PNPPrimaryRecord [] =       		{DEDEF_BOOL(1) }; /*primary*/
const U8 PNPVendorIDSource [] = 	    	{DEDEF_U16(VENDOR_ID_SOURCE_BLUETOOTH) };
const U8 PNPProtocolDescriptorList [] = 	{DEDEF_SEQ8(13), DEDEF_SEQ8(6), PROTOCOL_UUID16(PROTOCOL_UUID_L2CAP), PSM_U16(PSM_SDP), DEDEF_SEQ8(3), PROTOCOL_UUID16(PSM_SDP) };

const BT_SDP_ATTRIBUTE_T gPNP_SERVICE_RECORD [] =
{
    { (ATTRID_ServiceRecordHandle), sizeof(PNPServiceRecordHandle), (U8*)PNPServiceRecordHandle },
    { (ATTRID_ServiceClassIdList), sizeof(PNPServiceClassIdList), (U8*)PNPServiceClassIdList },
    { (ATTRID_SpecifcationID), sizeof(PNPSpecifcationID), (U8*)PNPSpecifcationID },
    { (ATTRID_VendorID), sizeof(PNPVendorID), (U8*)PNPVendorID },
    { (ATTRID_ProductID), sizeof(PNPProductID), (U8*)PNPProductID },
    { (ATTRID_ProductVersion), sizeof(PNPProductVersion), (U8*)PNPProductVersion },
    { (ATTRID_PrimaryRecord), sizeof(PNPPrimaryRecord), (U8*)PNPPrimaryRecord },
    { (ATTRID_VendorIDSource), sizeof(PNPVendorIDSource), (U8*)PNPVendorIDSource },
    { (ATTRID_ProtocolDescriptorList), sizeof(PNPProtocolDescriptorList), (U8*)PNPProtocolDescriptorList },
};




