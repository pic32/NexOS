#ifndef FLASH_CONTROLLER_H
    #define FLASH_CONTROLLER_H

#include "GenericTypeDefs.h"
#include "FlashControllerFlash.h"

#define FLASH_BUFFER_SIZE_IN_BYTES                              SST26_ERASE_BLOCK_SIZE_IN_BYTES
#define FLASH_CONTROLLER_MAX_ADDRESS                            SST26_MAX_ADDRESS
#define FLASH_CONTROLLER_FILE_SYSTEM_OFFSET                     4096
#define FLASH_CONTROLLER_NUMBER_OF_MEDIA_SECTORS                (UINT32)0x00003FFF
#define FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES             512
#define FLASH_CONTROLLER_NUMBER_OF_USABLE_MEDIA_SECTORS         (FLASH_CONTROLLER_NUMBER_OF_MEDIA_SECTORS - 8)

typedef enum
{
    FLASH_CONTROLLER_OK = 0,
    FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_INIT_ERROR,
    FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_READ_ERROR,
    FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_WRITE_ERROR,
    FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_ERASE_ERROR,
    FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_UNLOCK_ERROR,
    FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE,
    FLASH_CONTROLLER_INVALID_BUFFER,
    NUMBER_OF_FLASH_CONTROLLER_ERRORS
}FLASH_CONTROLLER_ERROR;

FLASH_CONTROLLER_ERROR InitFlashController(BOOL InitSPI);

FLASH_CONTROLLER_ERROR FlashControllerReadByte(UINT32 Address, BYTE *DataBuffer);

FLASH_CONTROLLER_ERROR FlashControllerReadUINT16(UINT32 Address, UINT16 *DataBuffer);

FLASH_CONTROLLER_ERROR FlashControllerReadUINT32(UINT32 Address, UINT32 *DataBuffer);

FLASH_CONTROLLER_ERROR FlashControllerReadBytes(UINT32 Address, BYTE *DataBuffer, UINT32 NumberOfBytesToRead, UINT32 *NumberOfBytesRead);

FLASH_CONTROLLER_ERROR FlashControllerWriteByte(UINT32 Address, BYTE Data);

FLASH_CONTROLLER_ERROR FlashControllerWriteUINT16(UINT32 Address, UINT16 Data);

FLASH_CONTROLLER_ERROR FlashControllerWriteUINT32(UINT32 Address, UINT32 Data);

FLASH_CONTROLLER_ERROR FlashControllerWriteBytes(UINT32 Address, BYTE *DataBuffer, UINT32 NumberOfBytesToWrite, UINT32 *NumberOfBytesWritten);

FLASH_CONTROLLER_ERROR FlashControllerFlushBufferToMedia(void);

FLASH_CONTROLLER_ERROR FlashControllerEraseFlash(void);

FLASH_CONTROLLER_ERROR UpdateFlashBufferToApplicableAddress(UINT32 Address);

FLASH_CONTROLLER_ERROR FlashControllerWriteToFlashBuffer(UINT32 Index, BYTE *Buffer, UINT32 SizeInBytes);

FLASH_CONTROLLER_ERROR FlashControllerReadFromFlashBuffer(UINT32 Index, BYTE *Buffer, UINT32 SizeInBytes);

BOOL FlashControllerGetFlashBufferModified(void);

#endif // end of #ifndef FLASH_CONTROLLER_H
