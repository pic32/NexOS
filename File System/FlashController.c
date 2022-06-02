#include <string.h>

#include "FlashController.h"

#define IsFlashAddressInCurrentBuffer(Address)                  (BOOL)((Address / FLASH_BUFFER_SIZE_IN_BYTES) == gCurrentFlashPage)

UINT32 gCurrentFlashPage;
BYTE gCurrentFlashDataBuffer[FLASH_BUFFER_SIZE_IN_BYTES];
BYTE gFlashBufferModified = FALSE;

FLASH_CONTROLLER_ERROR UpdateFlashBufferToApplicableAddress(UINT32 Address)
{
    if(IsFlashAddressInCurrentBuffer(Address) == FALSE)
    {
        FLASH_CONTROLLER_ERROR Outcome;
        
        // flush the current buffer to flash if applicable
        Outcome = FlashControllerFlushBufferToMedia();
        
        // was there an error?
        if(Outcome != FLASH_CONTROLLER_OK)
            return Outcome;
        
        // now read in the new page
        if(SerialFlashReadBytes((Address / FLASH_BUFFER_SIZE_IN_BYTES) * FLASH_BUFFER_SIZE_IN_BYTES, gCurrentFlashDataBuffer, FLASH_BUFFER_SIZE_IN_BYTES) != SERIAL_FLASH_OK)
            return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_READ_ERROR;
        
        gCurrentFlashPage = Address / FLASH_BUFFER_SIZE_IN_BYTES;
    }
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR InitFlashController(BOOL InitSPI)
{
    // if the init is called again, we must flush everything that is active
    if(gFlashBufferModified == TRUE)
    {
        FLASH_CONTROLLER_ERROR Outcome;
        
        Outcome = FlashControllerFlushBufferToMedia();
        
        if(Outcome != FLASH_CONTROLLER_OK)
            return Outcome;
    }
    
    gCurrentFlashPage = 0;
    gFlashBufferModified = FALSE;
    
    if(InitSerialFlash(InitSPI) == FALSE)
    {
        memset((void*)gCurrentFlashDataBuffer, 0xFF, sizeof(gCurrentFlashDataBuffer));
        
        return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_INIT_ERROR;
    }
    
    // now read in the first page
    if(SerialFlashReadBytes(FLASH_CONTROLLER_FILE_SYSTEM_OFFSET, gCurrentFlashDataBuffer, FLASH_BUFFER_SIZE_IN_BYTES) != SERIAL_FLASH_OK)
        return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_READ_ERROR;
    
    gCurrentFlashPage = FLASH_CONTROLLER_FILE_SYSTEM_OFFSET / FLASH_BUFFER_SIZE_IN_BYTES;
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerReadByte(UINT32 Address, BYTE *DataBuffer)
{
    FLASH_CONTROLLER_ERROR Outcome;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
        
    if(DataBuffer == (BYTE*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    // flush the current buffer to flash if applicable
    Outcome = UpdateFlashBufferToApplicableAddress(Address);

    // was there an error?
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    // get the data from the flash buffer
    *DataBuffer = gCurrentFlashDataBuffer[Address & (FLASH_BUFFER_SIZE_IN_BYTES - 1)];
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerReadUINT16(UINT32 Address, UINT16 *DataBuffer)
{
    FLASH_CONTROLLER_ERROR Outcome;
    BYTE TempDataBuffer;
        
    if(DataBuffer == (UINT16*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    // get the data from the flash buffer
    Outcome = FlashControllerReadByte(Address++, &TempDataBuffer);
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    *DataBuffer = (UINT16)((UINT16)TempDataBuffer << 8);
    
    Outcome = FlashControllerReadByte(Address, &TempDataBuffer);
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    *DataBuffer |= (UINT16)TempDataBuffer;
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerReadUINT32(UINT32 Address, UINT32 *DataBuffer)
{
    FLASH_CONTROLLER_ERROR Outcome;
    UINT16 TempDataBuffer;
        
    if(DataBuffer == (UINT32*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    // get the data from the flash buffer
    Outcome = FlashControllerReadUINT16(Address, &TempDataBuffer);
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    *DataBuffer = (UINT32)((UINT32)TempDataBuffer << 16);
    
    Outcome = FlashControllerReadUINT16(Address + 2, &TempDataBuffer);
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    *DataBuffer |= (UINT32)TempDataBuffer;
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerReadBytes(UINT32 Address, BYTE *DataBuffer, UINT32 NumberOfBytesToRead, UINT32 *NumberOfBytesRead)
{
    UINT32 i;
    FLASH_CONTROLLER_ERROR Outcome;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
        
    if(DataBuffer == (BYTE*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    if(NumberOfBytesRead != (UINT32*)NULL)
        *NumberOfBytesRead = 0;
    
    for(i = 0; i < NumberOfBytesToRead; i++)
    {
        Outcome = FlashControllerReadByte(Address + i, DataBuffer++);
        
        if(Outcome != FLASH_CONTROLLER_OK)
            return Outcome;
        
        if(NumberOfBytesRead != (UINT32*)NULL)
            *NumberOfBytesRead = *NumberOfBytesRead + 1;
    }
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerWriteByte(UINT32 Address, BYTE Data)
{
    FLASH_CONTROLLER_ERROR Outcome;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
    
    // flush the current buffer to flash if applicable
    Outcome = UpdateFlashBufferToApplicableAddress(Address);

    // was there an error?
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    // get the data from the flash buffer
    gCurrentFlashDataBuffer[Address & (FLASH_BUFFER_SIZE_IN_BYTES - 1)] = Data;
    
    gFlashBufferModified = TRUE;
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerWriteUINT16(UINT32 Address, UINT16 Data)
{
    FLASH_CONTROLLER_ERROR Outcome;
    
    // get the data from the flash buffer
    Outcome = FlashControllerWriteByte(Address++, Data >> 8);
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    return FlashControllerWriteByte(Address, (BYTE)Data);
}

FLASH_CONTROLLER_ERROR FlashControllerWriteUINT32(UINT32 Address, UINT32 Data)
{
    FLASH_CONTROLLER_ERROR Outcome;
    
    // get the data from the flash buffer
    Outcome = FlashControllerWriteUINT16(Address, (UINT16)(Data >> 16));
    
    if(Outcome != FLASH_CONTROLLER_OK)
        return Outcome;
    
    return FlashControllerWriteUINT16(Address + 2, (UINT16)Data);
}

FLASH_CONTROLLER_ERROR FlashControllerWriteBytes(UINT32 Address, BYTE *DataBuffer, UINT32 NumberOfBytesToWrite, UINT32 *NumberOfBytesWritten)
{
    UINT32 i;
    FLASH_CONTROLLER_ERROR Outcome;
    
    if(Address > FLASH_CONTROLLER_MAX_ADDRESS)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
        
    if(DataBuffer == (BYTE*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    if(NumberOfBytesWritten != (UINT32*)NULL)
        *NumberOfBytesWritten = 0;
    
    for(i = 0; i < NumberOfBytesToWrite; i++)
    {
        Outcome = FlashControllerWriteByte(Address + i, *DataBuffer++);
        
        if(Outcome != FLASH_CONTROLLER_OK)
            return Outcome;
        
        if(NumberOfBytesWritten != (UINT32*)NULL)
            *NumberOfBytesWritten = *NumberOfBytesWritten + 1;
    }
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerFlushBufferToMedia(void)
{
    if(gFlashBufferModified == TRUE)
    {
        UINT32 i;
        
        if(SerialFlashUnblockAllSectors() != SERIAL_FLASH_OK)
            return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_UNLOCK_ERROR;
        
        // now save the data to flash
        if(SerialFlashEraseSector((gCurrentFlashPage * FLASH_BUFFER_SIZE_IN_BYTES)) != SERIAL_FLASH_OK)
            return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_ERASE_ERROR;
        
        for(i = 0; i < (FLASH_BUFFER_SIZE_IN_BYTES / SERIAL_FLASH_PAGE_PROGRAM_SIZE_IN_BYTES); i++)
        {
            if(SerialFlashWriteSector((gCurrentFlashPage * FLASH_BUFFER_SIZE_IN_BYTES) + (SERIAL_FLASH_PAGE_PROGRAM_SIZE_IN_BYTES * i), &gCurrentFlashDataBuffer[i * SERIAL_FLASH_PAGE_PROGRAM_SIZE_IN_BYTES]) != SERIAL_FLASH_OK)
                return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_WRITE_ERROR;
        }
 
        gFlashBufferModified = FALSE;
    }
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerEraseFlash(void)
{
    UINT32 i;
    
    for(i = 0; i < (FLASH_CONTROLLER_MAX_ADDRESS + 1) / FLASH_BUFFER_SIZE_IN_BYTES; i++)
    {
        if(SerialFlashUnblockAllSectors() != SERIAL_FLASH_OK)
            return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_UNLOCK_ERROR;

        // now save the data to flash
        if(SerialFlashEraseSector((i * FLASH_BUFFER_SIZE_IN_BYTES)) != SERIAL_FLASH_OK)
            return FLASH_CONTROLLER_LOWER_LEVEL_FLASH_DRIVER_ERASE_ERROR;   
    }
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerWriteToFlashBuffer(UINT32 Index, BYTE *Buffer, UINT32 SizeInBytes)
{
    if(Index + SizeInBytes > FLASH_BUFFER_SIZE_IN_BYTES)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
    
    if(Buffer == (BYTE*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    // now just copy in the data
    memcpy((void*)&gCurrentFlashDataBuffer[Index], (void*)Buffer, SizeInBytes);
    
    gFlashBufferModified = TRUE;
    
    return FLASH_CONTROLLER_OK;
}

FLASH_CONTROLLER_ERROR FlashControllerReadFromFlashBuffer(UINT32 Index, BYTE *Buffer, UINT32 SizeInBytes)
{
    if(Index + SizeInBytes > FLASH_BUFFER_SIZE_IN_BYTES)
        return FLASH_CONTROLLER_FLASH_ADDRESS_OUT_OF_RANGE;
    
    if(Buffer == (BYTE*)NULL)
        return FLASH_CONTROLLER_INVALID_BUFFER;
    
    // now just copy in the data
    memcpy((void*)Buffer, (void*)&gCurrentFlashDataBuffer[Index], SizeInBytes);
    
    return FLASH_CONTROLLER_OK;
}

BOOL FlashControllerGetFlashBufferModified(void)
{
    return gFlashBufferModified;
}
