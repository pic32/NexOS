/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "FileSystemConfig.h"

#if (USING_FILE_SYSTEM_RTCC == 1)
    #include "RTCCLibrary.h"
#endif // end of #if (USING_FILE_SYSTEM_RTCC == 1)

#if (USING_SD_CARD_FILE_SYSTEM == 1)
    #include "SD-SPI.h"
#endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

#if (USING_USB_HOST_FILE_SYSTEM == 1)
    #include "usb.h"
    #include "usb_host_msd.h"
    #include "usb_host_msd_scsi.h"
#endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)

#if (USING_FLASH_FILE_SYSTEM == 1)
    #include "FlashController.h"
    #include "LUNFlashFileSystemInterface.h"
#endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)

#if (USING_SD_CARD_FILE_SYSTEM == 1 || USING_USB_HOST_FILE_SYSTEM == 1)
    #include "FSDefs.h"
#endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1 || USING_USB_HOST_FILE_SYSTEM == 1)

#include "GenericTypeDefs.h"

#if (USING_USB_HOST_FILE_SYSTEM == 1)
    static BOOL gUSBFlashDeviceConnected = FALSE;

    BOOL GetUSBFlashDeviceConnected(void)
    {
        return gUSBFlashDeviceConnected;
    }

    void SetUSBFlashDeviceAttached(BOOL Attached)
    {
        gUSBFlashDeviceConnected = Attached;
    }
#endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)
    
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
    switch (pdrv)
	{
        #if (USING_SD_CARD_FILE_SYSTEM == 1)
            case SD_MMC:
            {
                // is the media preset?
                if(MDD_SDSPI_MediaDetect() == TRUE)
                    return 0;
                else
                    return STA_NODISK;
            }
        #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

        #if (USING_USB_HOST_FILE_SYSTEM == 1)
            case USB:
            {
                if(USBHostMSDSCSIMediaDetect() == TRUE)
                    gUSBFlashDeviceConnected = TRUE;
                else
                    gUSBFlashDeviceConnected = FALSE;

                if(gUSBFlashDeviceConnected == TRUE)
                    return 0;
                else
                    return STA_NODISK;                       
            }
        #endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)
            
        #if (USING_FLASH_FILE_SYSTEM == 1)
            case SERIAL_FLASH:
            {
                return 0;
            }
        #endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)
	}

	return STA_NODISK;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
    #if (USING_SD_CARD_FILE_SYSTEM == 1 || USING_USB_HOST_FILE_SYSTEM == 1)
        MEDIA_INFORMATION *Media;
    #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1 || USING_USB_HOST_FILE_SYSTEM == 1)

	switch (pdrv)
	{
        #if (USING_SD_CARD_FILE_SYSTEM == 1)
            case SD_MMC:
            {
                // initialize the IO of the uC for the SD card
                MDD_SDSPI_InitIO();

                // is it present?
                if(MDD_SDSPI_MediaDetect() == FALSE)
                    return STA_NODISK;

                // is it write protected?
                if(MDD_SDSPI_WriteProtectState() == TRUE)
                    return STA_PROTECT;

                // initialize the media
                Media = MDD_SDSPI_MediaInitialize();

                // was there an error in the initialization of the media?
                if(Media->errorCode == MEDIA_NO_ERROR)
                    return 0;

                return STA_NOINIT;
            }
        #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

        #if (USING_USB_HOST_FILE_SYSTEM == 1)
            case USB:
            {
                // is the media even present?
                if(USBHostMSDSCSIMediaDetect() == TRUE)
                {
                    gUSBFlashDeviceConnected = TRUE;
                }
                else
                {
                    gUSBFlashDeviceConnected = FALSE;

                    return STA_NODISK;
                }

                // is the media write protected?
                if(USBHostMSDSCSIWriteProtectState() == TRUE)
                    return STA_PROTECT;

                // initialize the media
                Media = USBHostMSDSCSIMediaInitialize();

                // was there an error in the initialization of the media?
                if(Media->errorCode == MEDIA_NO_ERROR)
                    return 0;

                return STA_NOINIT;
            }
        #endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)
            
        #if (USING_FLASH_FILE_SYSTEM == 1)
            case SERIAL_FLASH:
            {
                if(InitFlashController(TRUE) == FLASH_CONTROLLER_OK)
                    return 0;
                else
                    return STA_NOINIT;
            }
        #endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)
    }

	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	switch (pdrv)
	{
        #if (USING_SD_CARD_FILE_SYSTEM == 1)
            case SD_MMC:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(MDD_SDSPI_SectorRead(sector + i, buff) == FALSE)
                        return RES_ERROR;

                    buff += SD_CARD_MEDIA_SECTOR_SIZE_IN_BYTES;
                }

                return RES_OK;
            }
        #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

        #if (USING_USB_HOST_FILE_SYSTEM == 1)
            case USB:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(USBHostMSDSCSISectorRead(sector + i, buff) == FALSE)
                        return RES_ERROR;
                    
                    buff += USB_MEDIA_SECTOR_SIZE_IN_BYTES;
                }
                
                return RES_OK;
            }
        #endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)
            
        #if (USING_FLASH_FILE_SYSTEM == 1)
            case SERIAL_FLASH:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(FlashControllerFileSystemReadSector(sector + i, buff) == FALSE)
                        return RES_ERROR;

                    buff += FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;
                }

                return RES_OK;
            }
        #endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)
    }

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Sector address in LBA */
    UINT count			/* Number of sectors to write */
)
{
    switch (pdrv)
    {
        #if (USING_SD_CARD_FILE_SYSTEM == 1)
            case SD_MMC:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(MDD_SDSPI_SectorWrite(sector + i, (BYTE*)buff, TRUE) == FALSE)
                        return RES_ERROR;

                    buff += SD_CARD_MEDIA_SECTOR_SIZE_IN_BYTES;
                }

                return RES_OK;
            }
        #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

        #if (USING_USB_HOST_FILE_SYSTEM == 1)
            case USB:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(USBHostMSDSCSISectorWrite(sector, (BYTE*)buff, TRUE) == FALSE)
                        return RES_ERROR;

                    buff += USB_MEDIA_SECTOR_SIZE_IN_BYTES;
                }

                return RES_OK;
            }
        #endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)
            
        #if (USING_FLASH_FILE_SYSTEM == 1)
            case SERIAL_FLASH:
            {
                UINT32 i;

                for(i = 0; i < count; i++)
                {
                    if(FlashControllerFileSystemWriteSector(sector + i, (BYTE*)buff, TRUE) == FALSE)
                        return RES_ERROR;

                    buff += FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;
                }

                return RES_OK;
            }
        #endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)
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
    switch (pdrv) 
    {
        #if (USING_SD_CARD_FILE_SYSTEM == 1)
            case SD_MMC:
            {
                // Process of the command for the MMC/SD card
                switch(cmd)
                {
                    // Complete pending write process
                    case CTRL_SYNC:
                    {
                        // Do not support this command, it makes things much much slower since
                        // our size is 65,536.  32,768 is the cut off for this feature.

                        break;
                    }

                    // Get media size
                    case GET_SECTOR_COUNT:
                    {
                        DWORD *d = (DWORD*)buff;
                        UINT32 i;
                        BYTE TempBuffer[SD_CARD_MEDIA_SECTOR_SIZE_IN_BYTES];

                        *d = 0;
                                
                        // our max sector is the (size of flash in bytes - flash FS offset in flash) / SECTOR_SIZE_IN_BYTES
                        //*d = MAX_FLASH_SECTOR;
                        //*d = (DWORD)(0x00003FFF - 8);
                        for(i = 0x80000000; i > 1024; i /= 2)
                        {
                            if(MDD_SDSPI_SectorRead(i - 1, TempBuffer) != FALSE)
                            {
                                *d = (DWORD)((i * 2) - 1);
                                    
                                return RES_OK;
                            }
                        }
                        
                        // error
                        return RES_PARERR;
                    }

                    // Get sector size
                    case GET_SECTOR_SIZE:
                    {
                        DWORD *d = (DWORD*)buff;

                        *d = SD_CARD_MEDIA_SECTOR_SIZE_IN_BYTES;

                        break;
                    }

                    // Get erase block size
                    case GET_BLOCK_SIZE:
                    {
                        DWORD *d = (DWORD*)buff;

                        // this tells the FatFs that
                        // we don't know how large our
                        // flash block size is.  32,768 is
                        // the cutoff, and we are 65,536.
                        // So we have to return a 1.
                        *d = SD_CARD_BLOCK_SIZE_IN_BYTES;

                        break;
                    }
                }

                return RES_OK;
            }
        #endif // end of #if (USING_SD_CARD_FILE_SYSTEM == 1)

        #if (USING_USB_HOST_FILE_SYSTEM == 1)
            case USB:
            {
                // Process of the command the USB drive

                return RES_OK;
            }
        #endif // end of #if (USING_USB_HOST_FILE_SYSTEM == 1)

        #if (USING_FLASH_FILE_SYSTEM == 1)
            case SERIAL_FLASH:
            {
                switch(cmd)
                {
                    // Complete pending write process
                    case CTRL_SYNC:
                    {
                        // Do not support this command, it makes things much much slower since
                        // our size is 65,536.  32,768 is the cut off for this feature.

                        break;
                    }

                    // Get media size
                    case GET_SECTOR_COUNT:
                    {
                        DWORD *d = (DWORD*)buff;

                        // our max sector is the (size of flash in bytes - flash FS offset in flash) / SECTOR_SIZE_IN_BYTES
                        //*d = MAX_FLASH_SECTOR;
                        *d = (DWORD)FlashControllerReadCapacity();

                        break;
                    }

                    // Get sector size
                    case GET_SECTOR_SIZE:
                    {
                        DWORD *d = (DWORD*)buff;

                        *d = FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;

                        break;
                    }

                    // Get erase block size
                    case GET_BLOCK_SIZE:
                    {
                        DWORD *d = (DWORD*)buff;

                        // this tells the FatFs that
                        // we don't know how large our
                        // flash block size is.  32,768 is
                        // the cutoff, and we are 65,536.
                        // So we have to return a 1.
                        *d = FLASH_BUFFER_SIZE_IN_BYTES;

                        break;
                    }
                }

                return RES_OK;
            }
        #endif // end of #if (USING_FLASH_FILE_SYSTEM == 1)
    }

    return RES_PARERR;
}

#if (FF_FS_NORTC == 0)
    /*
    Current local time is returned with packed into a DWORD value. The bit field is as follows:
        bit31:25 Year origin from the 1980 (0..127)
        bit24:21 Month (1..12)
        bit20:16 Day of the month(1..31)
        bit15:11 Hour (0..23)
        bit10:5 Minute (0..59)
        bit4:0 Second / 2 (0..29)
    */
    DWORD get_fattime (void)
    {
        #if (USING_FILE_SYSTEM_RTCC == 1)
            RTCC_TIME CurrentTime;
            RTCC_DATE CurrentDate;
            DWORD BCDConverter;
            DWORD fattime = 0;

            CurrentTime.l = RtccGetTime();
            CurrentDate.l = RtccGetDate();

            // the year is 0 - 99 from 2000
            BCDConverter = (CurrentDate.year >> 4) * 10;
            BCDConverter += CurrentDate.year & 0x0F;

            // lets normalize this from Microchips format
            BCDConverter += 2000;

            // The year for FatFS is how many years since 1980
            // We subtract 1980 to go from 2000 to 1980 for the FatFS
            fattime = (BCDConverter - 1980) << 25;

            // now get the month
            BCDConverter = (CurrentDate.mon >> 4) * 10;
            BCDConverter += (CurrentDate.mon & 0x0F);

            // put the months in the correct spot
            fattime |= BCDConverter << 21;

            BCDConverter = (CurrentDate.mday >> 4) * 10;
            BCDConverter += (CurrentDate.mday & 0x0F);

            // put in the day of the month
            fattime |= BCDConverter << 16;

            BCDConverter = (CurrentTime.hour >> 4) * 10;
            BCDConverter += (CurrentTime.hour & 0x0F);

            // now put in the hours
            fattime |= BCDConverter << 11;

            BCDConverter = (CurrentTime.min >> 4) * 10;
            BCDConverter += (CurrentTime.min & 0x0F);

            // now put in the minutes
            fattime |= BCDConverter << 5;

            BCDConverter = (CurrentTime.sec  >> 4) * 10;
            BCDConverter += (CurrentTime.sec & 0x0F);

            // now put in the seconds
            fattime |= (BCDConverter / 2);

            return fattime;
        #else
            return 0;
        #endif // end of #if (USING_FILE_SYSTEM_RTCC == 1)
    }
#endif // end of #if (FF_FS_NORTC == 0)