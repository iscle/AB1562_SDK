#ifndef _FOTA_H_
#define _FOTA_H_

/**************************************************************************************************
* Definition
**************************************************************************************************/

typedef enum
{
	InternalFlash = 0,
	ExternalFlash = 1,

	Invalid = 0xFF
} StorageType;

typedef enum
{
	FOTA_STATE_LOADER_START_DATA_MOVING = 0x0100,
	FOTA_STATE_LOADER_COMPLETE_DATA_MOVING = 0x0101,
	FOTA_STATE_LOADER_DATA_MOVING_FAIL = 0x0102,
} FotaState;

typedef enum
{
	FOTA_ERRCODE_SUCCESS = 0,
	FOTA_ERRCODE_READ_FOTA_HEADER_FAIL = 1,
	FOTA_ERRCODE_READ_FOTA_DATA_FAIL = 2,
	FOTA_ERRCODE_CHECK_INTEGRITY_FAIL = 3,
	FOTA_ERRCODE_UNKNOWN_STORAGE_TYPE = 4,
	FOTA_ERRCODE_UNKNOWN_INTEGRITY_CHECK_TYPE = 5,
	FOTA_ERRCODE_SHA256_IS_NOT_SUPPORTED = 6,
	FOTA_ERRCODE_COMMIT_FAIL_DUE_TO_INTEGRITY_NOT_CHECKED = 7,
	FOTA_ERRCODE_UNKNOWN_PARTITION_ID = 8,
	FOTA_ERRCODE_UNSUPPORTED_PARTITION_ID = 9,

	FOTA_ERRCODE_INVALID_FOTA_HEADER_TLV = 0x14,
	FOTA_ERRCODE_COMMIT_FAIL_DUE_TO_LOW_BATTERY = 0x15,
	
	FOTA_ERRCODE_FOTA_READ_STATE_FAIL = 0x20,
	FOTA_ERRCODE_FOTA_ERASE_WORKING_SECTOR_FAIL = 0x21,
	FOTA_ERRCODE_FOTA_WRITE_STATE_FAIL = 0x22,
	FOTA_ERRCODE_FOTA_WRITE_STATE_FULL = 0x23,
	FOTA_ERRCODE_FOTA_PACKAGE_MAGIC_WORD_READ_FAIL = 0x24,
	FOTA_ERRCODE_FOTA_PACKAGE_MAGIC_WORD_WRONG = 0x25,
	FOTA_ERRCODE_FOTA_WRITE_MAGIC_WORD_FAIL = 0x26,
	FOTA_ERRCODE_EXTERNAL_SHA256_FAIL = 0x30, 
	
	FOTA_ERRCODE_DSP_OPEN = 0xD0,
	FOTA_ERRCODE_BT_THROUGHPUT_BUSY = 0xD1,

	FOTA_ERRCODE_INVALID_PARAMETER = 0xfc,
	FOTA_ERRCODE_UNINITIALIZED = 0xfd,
	FOTA_ERRCODE_UNSUPPORTED = 0xfe,
	FOTA_ERRCODE_FAIL = 0xff,
} FOTA_ERRCODE;

/**************************************************************************************************
* Public Function
**************************************************************************************************/
FOTA_ERRCODE getFotaInfo(StorageType * storage_type, U32 * fota_address, U32 * length);
FOTA_ERRCODE readFotaState(U16 * state);
FOTA_ERRCODE checkIntegrity_InternalFlash();
void fotaReset();

#endif //_FOTA_H_
