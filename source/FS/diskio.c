/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "feature_config.h"
#ifdef FEATURE_FILE_SYSTEM

#define ENABLE_INTERNAL_FLASH 0

#include "types.h"
#include "diskio.h"
#include "drv_sdc.h"
#include "drv_usb_host_msc.h"
#include "drv_usb_host.h"
#include "message.h"
#if ENABLE_INTERNAL_FLASH
#include "nvram.h"
#endif
#include "string.h"

#define MMC	    0	/* Example: Map MMC card to physical drive 0 */
#if ENABLE_INTERNAL_FLASH
#define FLASH   1	/* Example: Map internal flash to physical drive 1 */
#endif
#define USB     2	/* Example: Map USB hard disk to physical drive 2 */
#define SDCARD  3	/* Example: Map SD card to physical drive 3 */


#if ENABLE_INTERNAL_FLASH
static U8 ALIGN(16) g_ucBlockBuffer[256];
#endif

#if (FEATURE_USB_OVERALL)
static U32 gFSInitUSBFlg = 0;

static U32 USB_disk_initialize()
{
	if(gFSInitUSBFlg == 0)
	{
		gFSInitUSBFlg = 1;
		DRV_USB_Host_Init();
	}

	return 0;
}

static U32 USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRV_USB_Host_MSC_Write((U8*)buff, sector, count);

	return 0;
}

static U32 USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRV_USB_Host_MSC_Read(buff, sector, count);

	return 0;
}
#endif

static U32 MMC_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRV_SDC_WriteF(sector, (U8*)buff, (U32)count * 512);

	return 0;
}

static U32 MMC_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
#if 0
	return DRV_SDC_ReadF(sector, buff, count * 512);
#else
	U8 rc;

	rc=DRV_SDC_ReadF(sector, buff, count * 512);

	return rc;
#endif
}

#if ENABLE_INTERNAL_FLASH
static U32 FLASH_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	U32 DynaOffset = 0;
    DynaOffset += NVRam_FSystemAddress();

	DynaOffset += sector * count * 512;
	NVRam_PageRead(DynaOffset, g_ucBlockBuffer);
	memcpy(buff, g_ucBlockBuffer, 256);

	DynaOffset += 256;
	NVRam_PageRead(DynaOffset, g_ucBlockBuffer);
	memcpy(buff + 256, g_ucBlockBuffer, 256);

	return 0;
}
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
	switch (pdrv) {
	case USB :
	case MMC :
	case SDCARD :
#if ENABLE_INTERNAL_FLASH
	case FLASH :
#endif
			return 0;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
	#if (FEATURE_USB_OVERALL)
	int result;
	#endif

	switch (pdrv) {
	case USB :
		#if (FEATURE_USB_OVERALL)
		result = USB_disk_initialize();
		if(result == 0)
			return 0;
		else
		#endif
			return STA_NOINIT;
	case MMC :
    case SDCARD :

#if ENABLE_INTERNAL_FLASH
	case FLASH :
#endif
			return 0;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int result;


	switch (pdrv) {
	case USB :
#if (FEATURE_USB_OVERALL)
		result = USB_disk_read(buff, sector, count);
		if(result == 0)
			return RES_OK;
		else
#endif
			return RES_ERROR;

	case MMC :
	case SDCARD :
		result = MMC_disk_read(buff, sector, count);
		if(result == 0)
			return RES_OK;
		else
			return RES_ERROR;

#if ENABLE_INTERNAL_FLASH
	case FLASH :
		result = FLASH_disk_read(buff, sector, count);
		if(result == 0)
			return RES_OK;
		else
			return RES_ERROR;
#endif
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive number to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int result;

	switch (pdrv) {
	case USB :
#if (FEATURE_USB_OVERALL)
		result = USB_disk_write(buff, sector, count);
		if(result == 0)
			return 0;
		else
#endif
			return STA_NOINIT;

	case MMC :
	case SDCARD :
		result = MMC_disk_write(buff, sector, count);
		if(result == 0)
			return 0;
		else
			return STA_NOINIT;

#if ENABLE_INTERNAL_FLASH
	case FLASH :
		return STA_PROTECT;
#endif
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	UNUSED(pdrv);
	UNUSED(cmd);
	UNUSED(buff);

	return RES_PARERR;
}

#endif
