#include <string.h>

#include "LUNFlashFileSystemInterface.h"
#include "FlashController.h"

UINT32 FlashControllerReadCapacity(void)
{
    return FLASH_CONTROLLER_NUMBER_OF_USABLE_MEDIA_SECTORS; // this is how many FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES sectors
}

WORD FlashControllerReadSectorSize(void)
{
    return FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;
}

BYTE FlashControllerMediaDetect(void)
{
    return 1;
}

BYTE FlashControllerFileSystemWriteSector(DWORD SectorAddress, BYTE* Buffer, BYTE AllowWriteToZero)
{
    UINT32 Address;
    FLASH_CONTROLLER_ERROR Outcome;
    
    if(AllowWriteToZero == FALSE)
    {
        if(SectorAddress == 0x00000000)
        {
            return FALSE;
        }    
    }
    
    Address = SectorAddress * FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS)
        return 0;
        
    if(Buffer == (BYTE*)NULL)
        return 0;
    
    Address += FLASH_CONTROLLER_FILE_SYSTEM_OFFSET;
    
    // flush the current buffer to flash if applicable
    Outcome = UpdateFlashBufferToApplicableAddress(Address);

    // was there an error?
    if(Outcome != FLASH_CONTROLLER_OK)
        return 0;
    
    // now just copy in the data
    if(FlashControllerWriteToFlashBuffer(Address & (FLASH_BUFFER_SIZE_IN_BYTES - 1), Buffer, FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES) != FLASH_CONTROLLER_OK)
        return 0;
    
    return 1;
}

BYTE FlashControllerWriteProtectState(void)
{
    return (BYTE)FALSE;
}

BYTE FlashControllerFileSystemReadSector(DWORD SectorAddress, BYTE* Buffer)
{
    UINT32 Address;
    FLASH_CONTROLLER_ERROR Outcome;
    
    Address = SectorAddress * FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS + FLASH_CONTROLLER_FILE_SYSTEM_OFFSET)
        return 0;
        
    if(Buffer == (BYTE*)NULL)
        return 0;
    
    Address += FLASH_CONTROLLER_FILE_SYSTEM_OFFSET;
    
    // flush the current buffer to flash if applicable
    Outcome = UpdateFlashBufferToApplicableAddress(Address);

    // was there an error?
    if(Outcome != FLASH_CONTROLLER_OK)
        return 0;
    
    // now just copy in the data
    if(FlashControllerReadFromFlashBuffer(Address & (FLASH_BUFFER_SIZE_IN_BYTES - 1), Buffer, FLASH_CONTROLLER_MEDIA_SECTOR_SIZE_IN_BYTES) != FLASH_CONTROLLER_OK)
        return 0;

    return 1;
}

MEDIA_INFORMATION *FlashControllerMediaInitialize(void)
{
    if(InitFlashController(FALSE) != FLASH_CONTROLLER_OK)
        return (MEDIA_INFORMATION*)NULL;
    
    // it just uses the value as a true or false return value even
    // though it is just a pointer
    return (MEDIA_INFORMATION*)1;
}

BOOL FlashControllerFlushFlashBuffer(void)
{
    return (BOOL)(FlashControllerFlushBufferToMedia() == FLASH_CONTROLLER_OK);
}
