#ifndef SST26VF064B_SERIAL_FLASH_DRIVER_H
    #define SST26VF064B_SERIAL_FLASH_DRIVER_H

#include "GenericTypeDefs.h"

#define SERIAL_FLASH_MAX_SPI_FREQUENCY                                  20000000
#define SERIAL_FLASH_PAGE_PROGRAM_SIZE_IN_BYTES                         256
#define STATUS_REGISTER_BUSY_BIT                                        0x80
#define SST26_ERASE_BLOCK_SIZE_IN_BYTES                                 4096
#define SST26_MAX_ADDRESS                                               0x7FFFFF

#define SST26_READ_COMMAND                                              0x03
#define SST26_WRITE_ENABLE_COMMAND                                      0x06
#define SST26_PAGE_PROGRAM_COMMAND                                      0x02
#define SST26_SECTOR_ERASE_COMMAND                                      0x20
#define SST26_READ_STATUS_REGISTER_COMMAND                              0x05
#define SST26_READ_BLOCK_PROTECTION_REGSITER_COMMAND                    0x72
#define SST26_WRITE_BLOCK_PROTECTION_REGSITER_COMMAND                   0x42

typedef enum
{
    SERIAL_FLASH_OK = 0,
    SERIAL_FLASH_ADDRESS_OUT_OF_RANGE,
    SERIAL_FLASH_INVALID_BUFFER,
            
    NUMBER_OF_SERIAL_FLASH_ERRORS
}SERIAL_FLASH_ERROR;

BOOL InitSerialFlash(BOOL SPIConfig);

SERIAL_FLASH_ERROR SerialFlashReadByte(UINT32 Address, BYTE *Data);

BYTE SerialFlashReadStatusRegister(void);

SERIAL_FLASH_ERROR SerialFlashReadBytes(UINT32 Address, BYTE *Data, UINT32 NumberOfBytes);

SERIAL_FLASH_ERROR SerialFlashWriteSector(UINT32 Address, BYTE *Data);

SERIAL_FLASH_ERROR SerialFlashReadBlockProtectionRegister(BYTE *BlockProtectionRegister);

SERIAL_FLASH_ERROR SerialFlashEnableWrites(void);

SERIAL_FLASH_ERROR SerialFlashUnblockAllSectors(void);

SERIAL_FLASH_ERROR SerialFlashEraseSector(UINT32 Address);

#endif // end of SST26VF064B_SERIAL_FLASH_DRIVER_H
