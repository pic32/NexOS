#include <string.h>

#include <plib.h>

#include "SST26VF064BSerialFlashDriver.h"
#include "HardwareProfile.h"

BOOL InitSerialFlash(BOOL SPIConfig)
{
    if(SPIConfig)
    {
        SpiChnEnable(SERIAL_FLASH_SPI_CHANNEL, FALSE);
	
        SpiChnConfigure(SERIAL_FLASH_SPI_CHANNEL, SERIAL_FLASH_SPI_CONFIG_BITS);
	
        SpiChnSetBitRate(SERIAL_FLASH_SPI_CHANNEL, GetPeripheralClock(), SERIAL_FLASH_MAX_SPI_FREQUENCY);
	
        SpiChnEnable(SERIAL_FLASH_SPI_CHANNEL, TRUE);
    }
	
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
	SERIAL_FLASH_CS_TRIS = 0;
	
	return TRUE;
}

static BYTE SerialFlashStreamReadByte(UINT32 Address)
{
	BYTE SPIReturnValue;

	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_READ_COMMAND);
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);

    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 16));
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 8));
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)Address);
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);

	// Write a 0 on the SPI line so that the CLK continues for the slave to return data
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)0);
	SPIReturnValue = (BYTE)SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);

	return SPIReturnValue;
}

static SERIAL_FLASH_ERROR SerialFlashReadByteWorker(UINT32 Address, BYTE *Data)
{
	while(SpiChnIsBusy(SERIAL_FLASH_SPI_CHANNEL));
    
	SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;

	*Data = SerialFlashStreamReadByte(Address);

	SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;

	return SERIAL_FLASH_OK;
}

SERIAL_FLASH_ERROR SerialFlashReadByte(UINT32 Address, BYTE *Data)
{
    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Address > MAX_SERIAL_FLASH_ADDRESS)
        {
            return SERIAL_FLASH_ADDRESS_OUT_OF_RANGE;
        }
        
        if(Data == (BYTE*)NULL)
        {
            return SERIAL_FLASH_INVALID_BUFFER;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)

    return SerialFlashReadByteWorker(Address, Data);
}

SERIAL_FLASH_ERROR SerialFlashReadBytes(UINT32 Address, BYTE *Data, UINT32 NumberOfBytes)
{
    UINT32 i;
    BYTE StatusRegister;
    
    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Address > MAX_SERIAL_FLASH_ADDRESS)
        {
            return SERIAL_FLASH_ADDRESS_OUT_OF_RANGE;
        }
        
        if(Data == (BYTE*)NULL)
        {
            return SERIAL_FLASH_INVALID_BUFFER;
        }
    
        if(NumberOfBytes == 0)
        {
            return SERIAL_FLASH_OK;
        }
    
        if(NumberOfBytes > MAX_SERIAL_FLASH_ADDRESS || (Address + NumberOfBytes) > MAX_SERIAL_FLASH_ADDRESS)
        {
            return SERIAL_FLASH_ADDRESS_OUT_OF_RANGE;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
    
    while(SpiChnIsBusy(SERIAL_FLASH_SPI_CHANNEL));
    
    // wait for any other pending writes to clear
    do
    {
        StatusRegister = SerialFlashReadStatusRegister();
    }
    while(StatusRegister & STATUS_REGISTER_BUSY_BIT);
    
	SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;

	*Data++ = SerialFlashStreamReadByte(Address);
    
    for(i = 1; i < NumberOfBytes; i++)
    {
        SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)0);
        *Data++ = (BYTE)SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    }

	SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;

	return SERIAL_FLASH_OK;
}

SERIAL_FLASH_ERROR SerialFlashEraseSector(UINT32 Address)
{
    BYTE StatusRegister;
    
    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Address > MAX_SERIAL_FLASH_ADDRESS)
        {
            return SERIAL_FLASH_ADDRESS_OUT_OF_RANGE;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)

    do
    {
        StatusRegister = SerialFlashReadStatusRegister();
    }
    while(StatusRegister & STATUS_REGISTER_BUSY_BIT);
    
    SerialFlashEnableWrites();
    
    // now erase the actual data
    
    // start up the chip select
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    // issue the page program command
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_SECTOR_ERASE_COMMAND);
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    // now output the address
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 16));
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 8));
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address & (UINT32)0xFF));
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return SERIAL_FLASH_OK;
}

// this writes 256 bytes
SERIAL_FLASH_ERROR SerialFlashWriteSector(UINT32 Address, BYTE *Data)
{
    UINT32 i;
    BYTE StatusRegister;

    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Address > MAX_SERIAL_FLASH_ADDRESS)
        {
            return SERIAL_FLASH_ADDRESS_OUT_OF_RANGE;
        }
        
        if(Data == (BYTE*)NULL)
        {
            return SERIAL_FLASH_INVALID_BUFFER;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)

    // wait for any other pending writes to clear
    do
    {
        StatusRegister = SerialFlashReadStatusRegister();
    }
    while(StatusRegister & STATUS_REGISTER_BUSY_BIT);
        
    // make it write enable
    SerialFlashEnableWrites();
    
    // start up the chip select
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    // issue the page program command
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_PAGE_PROGRAM_COMMAND);
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    // now output the address
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 16));
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address >> 8));
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
	SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)(Address & (UINT32)0xFF));
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    // now output the data
    for(i = 0; i < SERIAL_FLASH_PAGE_PROGRAM_SIZE_IN_BYTES; i++)
    {
        SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)*Data);
        SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
        
        Data++;
    }
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return SERIAL_FLASH_OK;
}

BYTE SerialFlashReadStatusRegister(void)
{
    BYTE StatusRegister;
    
    while(SpiChnIsBusy(SERIAL_FLASH_SPI_CHANNEL));
    
    // now issue the read status register command
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_READ_STATUS_REGISTER_COMMAND);
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)0);
    StatusRegister = SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return StatusRegister;
}

SERIAL_FLASH_ERROR SerialFlashReadBlockProtectionRegister(BYTE *BlockProtectionRegister)
{
    UINT32 i;
    
    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Data == (BYTE*)NULL)
        {
            return SERIAL_FLASH_INVALID_BUFFER;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
    
    while(SpiChnIsBusy(SERIAL_FLASH_SPI_CHANNEL));
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_READ_BLOCK_PROTECTION_REGSITER_COMMAND);
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    for(i = 0; i < 18; i++)
    {
        SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)0);
        
        *BlockProtectionRegister = (BYTE)SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
        
        BlockProtectionRegister++;
    }
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return SERIAL_FLASH_OK;
}

SERIAL_FLASH_ERROR SerialFlashWriteBlockProtectionRegister(BYTE *BlockProtectionRegister)
{
    UINT32 i;
    BYTE StatusRegister;
    
    #if (USING_SERIAL_FLASH_SAFE_MODE == 1)
        if(Data == (BYTE*)NULL)
        {
            return SERIAL_FLASH_INVALID_BUFFER;
        }
    #endif // end of #if (USING_SERIAL_FLASH_SAFE_MODE == 1)

    // wait for any other pending writes to clear
    do
    {
        StatusRegister = SerialFlashReadStatusRegister();
    }
    while(StatusRegister & STATUS_REGISTER_BUSY_BIT);
    
    SerialFlashEnableWrites();
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_WRITE_BLOCK_PROTECTION_REGSITER_COMMAND);
    SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    for(i = 0; i < 18; i++)
    {
        SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)*BlockProtectionRegister);
        SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
        
        BlockProtectionRegister++;
    }
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return SERIAL_FLASH_OK;
}

SERIAL_FLASH_ERROR SerialFlashEnableWrites(void)
{
    while(SpiChnIsBusy(SERIAL_FLASH_SPI_CHANNEL));
    
    // make it write enable
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_ACTIVE;
    
    SpiChnPutC(SERIAL_FLASH_SPI_CHANNEL, (unsigned int)SST26_WRITE_ENABLE_COMMAND);
	SpiChnGetC(SERIAL_FLASH_SPI_CHANNEL);
    
    SERIAL_FLASH_CS_PIN = SERIAL_FLASH_CS_INACTIVE;
    
    return SERIAL_FLASH_OK;
}

SERIAL_FLASH_ERROR SerialFlashUnblockAllSectors(void)
{
    BYTE BlockRegisters[18];
    
    memset((void*)BlockRegisters, 0, sizeof(BlockRegisters));
    
    return SerialFlashWriteBlockProtectionRegister(BlockRegisters);
}
