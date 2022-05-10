#ifndef _STREAM_H
#define _STREAM_H
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

//-
#include "source.h"
#include "sink.h"
#include "transform.h"
#include "uart_config.h"
#include "audio_config.h"
#include "syslog.h"
#define STREAM_LOG_I(fmt,arg...)              LOG_I(STREAM,fmt,##arg)
#define STREAM_LOG_W(fmt,arg...)              LOG_W(STREAM,fmt,##arg)
#define STREAM_LOG_E(fmt,arg...)              LOG_E(STREAM,fmt,##arg)
#define STREAM_MSGID_I(fmt,cnt,arg...)        LOG_MSGID_I(STREAM,fmt,cnt,##arg)
#define STREAM_MSGID_W(fmt,cnt,arg...)        LOG_MSGID_W(STREAM,fmt,cnt,##arg)
#define STREAM_MSGID_E(fmt,cnt,arg...)        LOG_MSGID_E(STREAM,fmt,cnt,##arg)

////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#define Ignore_Unused_stream
//#define STREAM_AUDIO_QUEUE

typedef struct
{
    SOURCE source; /*!< The source which has more data. */
    U32 tick_count;
} MESSAGE_MORE_DATA_T;

typedef struct
{
    SINK sink;    /*!< The sink which has more space. */
} MESSAGE_MORE_SPACE_T;
typedef enum
{
    InstantCloseMode,
    DspSoftCloeMode,
}CLOSE_MODE_enum_s;


////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function Declarations ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @breif init stream system
 */
VOID Stream_Init(VOID);

/**
 * @brief Move the specified number of bytes from source to sink.
 *
 * @param sink   The Sink to move data to.
 * @param source The Source to move data from.
 * @param count  The number of bytes to move.
 * @return Zero on failure and the count on success.
 */
U32 StreamMove(SINK sink, SOURCE source, U32 count);

/**
 * @brief Make an automatic connection between a source and sink
 *
 * @param source The Source data will be taken from.
 * @param sink   The Sink data will be written to.
 *
 * @return a transform on success, or zero on failure.
 */
TRANSFORM StreamConnect(SOURCE source, SINK sink);

/**
 * @brief Break transform of source and sink
 *
 * @param transform the transform to break
 */
EXTERN VOID StreamDisconnect(TRANSFORM transform);

/**
 * @brief Find the Sink associated with the raw UART.
 *
 * Returns zero if it is unavailable (for example the appropriate
 * transport has not been configured.)
 */
SINK StreamUartSink(VOID);
SINK StreamUart2Sink(VOID);
SINK Stream1WireSink(VOID);

/**
 * @brief Find the source associated with the raw UART.
 *
 *  Returns zero if it is unavailable (for example the appropriate
 * transport has not been configured.)
 */
SOURCE StreamUartSource(VOID);
SOURCE StreamUart2Source(VOID);
SOURCE Stream1WireSource(VOID);

/**
 * @brief Request to create an audio source
 * @param hardware The audio hardware which would be reserved as a source
 * @param instance The audio hardware instance (meaning depends on \e hardware)
 * @param channel The audio channel (meaning depends on \e hardware)
 *
 * @return The Source ID associated with the audio hardware.
 */
EXTERN SOURCE StreamAudioSource(audio_hardware hardware,audio_instance instance, audio_channel channel);

/**
 * @brief Request to create an audio sink
 * @param hardware The audio hardware which would be reserved as a sink
 * @param instance The audio hardware instance (meaning depends on \e hardware)
 * @param channel The audio channel (meaning depends on \e hardware)
 *
 * @return The Sink ID associated with the audio hardware.
 */
EXTERN SINK StreamAudioSink(audio_hardware hardware, audio_instance instance, audio_channel channel);
SINK StreamVPPathSink(audio_hardware hardware , audio_instance instance);


/**
 * @brief Request to create a sink which is joint to transform
 * @param transform The audio transform to connect
 * @param channel The audio channel (meaning depends on \e hardware)
 *
 * @return The Sink ID associated with the audio hardware.
 */
EXTERN SINK StreamJointSink(TRANSFORM transform, audio_channel channel);


/**
 * @brief Request to create a source which is branched from transform
 * @param transform The audio transform to connect
 * @param instance The audio hardware instance (meaning depends on \e hardware)
 * @param channel The audio channel (meaning depends on \e hardware)
 *
 * @return The Source ID associated with the audio hardware.
 */
EXTERN SOURCE StreamBranchSource(TRANSFORM transform, audio_channel channel);

/**
 * @breif Get source from sink
 * @param sink The sink.
 * @return The source.
 */
SOURCE StreamSourceFromSink(SINK sink);

/**
 * @breif Get sink from source
 * @param source The source.
 * @return The sink.
 */
SINK StreamSinkFromSource(SOURCE source);


/*!
@brief Return the USB Class Request Source associated with 'interface'.
@param interface The USB interface (returned by UsbAddInterface) to fetch the Source for.
*/
EXTERN SOURCE StreamUsbAudioClassSource(U32 inter_face);

/*!
   @brief Return the USB Class Request Sink.
   @param interface The USB interface (returned by UsbAddInterface) to fetch the Sink for.
*/
EXTERN SINK StreamUsbAudioClassSink(U32 inter_face);

/*!
@brief Returns a Source from the SCO stream passed.
@param handle The SCO stream from which to fetch the Source.
*/
EXTERN SOURCE StreamScoSource(SCO_CODEC codec, U16 ConnHandle);


/*!
   @brief Returns a Sink from the SCO stream passed.
   @param handle The SCO stream from which to fetch the Sink.
*/
EXTERN SINK StreamScoSink(SCO_CODEC codec, U16 ConnHandle);


/*!
   @brief Request to create an A2DP source.
*/
EXTERN SOURCE StreamA2dpSource(U8 codec, U8 scms, U8 role);

SOURCE StreamFileSource();


/*!
  @brief Find the Sink corresponding to an L2CAP connection

  @param cid The connection ID to fetch the Sink for.
*/
SINK StreamL2capSink(U8 bdaddr[6],U32 cid);



#ifdef Ignore_Unused_stream


/*!
  @brief Create a source from a region of memory.

  @param data The memory that the source will be created from.
  @param length The size of the memory region.

  This function allows a region of memory to be treated as a source.
  This is useful when there is a requirement to handle data (held in a
  known region of memory) using functions that expect a source, e.g.
  StreamConnect(), in order to efficiently transfer the data without
  having to copy it.

  It is important that the memory being treated as a source persists
  long enough for the stream operation to complete, i.e., long enough
  for the source to be read. The source created using this function
  only exists while the data is being read. However, the memory block
  being treated as a source is not freed by the stream subsystem once
  the data has been read. It remains the caller's responsibility to
  manage the memory and free it when it is appropriate to do so.

  If length is zero then 0 is returned.
*/
SOURCE StreamRegionSource(const U32 *data, U32 length);




/*!
@brief Find the Source corresponding to an RFCOMM connection.
*/
SOURCE StreamRfcommSource(U32 conn_id);

/*!
@brief Find the Source corresponding to an L2CAP connection
**
**
@param cid The connection ID to fetch the Source for.
*/
SOURCE StreamL2capSource(U32 cid);


/*!
@brief The Source connected to the port passed on Kalimba.
@param port In the range 0..3 (BC3-MM) or 0..7 (BC5-MM)
*/
SOURCE StreamDspSource(U32 port);

/*!
@brief Return the USB Request Source associated with the USB transport.
@param end_point The USB endpoint (bEndPointAddress field in EndPointInfo structure) to fetch the Source for.
*/
SOURCE StreamUsbEndPointSource(U32 end_point);

/*!
@brief Return the USB Vendor Source associated with the USB transport.
*/
SOURCE StreamUsbVendorSource(VOID);


/*!
  @brief Dynamically configure the UART settings.

  @param rate The UART rate to use.
*/
VOID StreamUartConfigure(stream_uart_baudrate rate);

/*!
  @brief Returns a source for a synthesised sequence of notes.

  @param ringtone This must be a pointer to an array of ringtone notes.

  If the ringtone_note* passed is invalid, the function returns 0.

  See \ref playing_ringtones for details of how to construct
  the \e ringtone argument.
*/
SOURCE StreamRingtoneSource(const ringtone_note *ringtone);


/*!
  @brief Find the Sink corresponding to an RFCOMM connection.
*/
SINK StreamRfcommSink(U32 conn_id);

/*!
  @brief Find the Sink corresponding to an L2CAP connection

  @param cid The connection ID to fetch the Sink for.
*/
SINK StreamL2capSink(U32 cid);


/*!
  @brief Return a source with the contents of the specified file.

  @param index the file whose contents are requested

  @return 0 if index is #FILE_NONE, or does not correspond to a narrow file.
*/
SOURCE StreamFileSource(FILE_INDEX index);

/*!
  @brief The Sink connected to the port passed on Kalimba.
  @param port In the range 0..3 (BC3-MM) or 0..7 (BC5-MM)
*/
SINK StreamKalimbaSink(U32 port);

/*!
  @brief Return a source with the contents of the specified I2C address.
  @param slave_addr The slave address of the device to read data from.
  @param array_addr The array address to read data from.
  @param size The amount of data (in bytes) to read.

  @return The source associated with the I2C stream.
*/
SOURCE StreamI2cSource(U32 slave_addr, U32 array_addr, U32 size);



/*!
   @brief Return the USB Request Sink associated with the USB transport.
   @param end_point The USB endpoint (bEndPointAddress field in EndPointInfo structure) to fetch the Sink for.
*/
SINK StreamUsbEndPointSink(U32 end_point);

/*!
   @brief Return the USB Vendor Sink associated with the USB transport.
*/
SINK StreamUsbVendorSink(VOID);


/*!
  @brief Find the Source corresponding to an ATT connection with a specific
         connection id and attribute handle.
  @param cid The channel id to get the connection source id for.
  @param handle The attribute handle to get the connection source id for.

  @return Source on success or zero on failure.
*/
SOURCE StreamAttSource(U32 cid, U32 handle);
#endif
VOID StreamTransformClose(VOID* pTransform);


VOID StreamCloseAll(TRANSFORM transform, CLOSE_MODE_enum_s mode);

#ifdef STREAM_AUDIO_QUEUE
EXTERN SOURCE StreamAudioQSource(VOID* OSMQ_ptr);
EXTERN SINK StreamAudioQSink(VOID* OSMQ_ptr);
#endif
EXTERN SINK StreamVirtualSink(VOID* entry, U32 report_length);
EXTERN SOURCE StreamUsbCDCSource(void);
EXTERN SINK StreamUsbCDCSink(void);
EXTERN SOURCE StreamMemorySource(U8* Memory_addr, U32 Memory_len);

#endif
