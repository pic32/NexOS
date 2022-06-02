#ifndef LUN_FLASH_FILE_SYSTEM_INTERFACE_H
    #define LUN_FLASH_FILE_SYSTEM_INTERFACE_H

#include "GenericTypeDefs.h"
#include "FSDefs.h"

UINT32 FlashControllerReadCapacity(void);
WORD FlashControllerReadSectorSize(void);
BYTE FlashControllerMediaDetect(void);
BYTE FlashControllerFileSystemWriteSector(DWORD SectorAddress, BYTE* Buffer, BYTE AllowWriteToZero);
BYTE FlashControllerWriteProtectState(void);
BYTE FlashControllerFileSystemReadSector(DWORD SectorAddress, BYTE* Buffer);
MEDIA_INFORMATION *FlashControllerMediaInitialize(void);
BOOL FlashControllerFlushFlashBuffer(void);

#endif // end of #ifndef LUN_FLASH_FILE_SYSTEM_INTERFACE_H
