/*
    NexOS Kernel Version v1.01.04
    Copyright (c) 2022 brodie

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include <plib.h>

#include "HardwareProfile.h"

#include "../NexOS/Kernel/Task.h"
#include "../NexOS/IOBuffer/IOBuffer.h"
#include "CriticalSection.h"

#pragma config UPLLEN   = ON        	// USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = ON            // Internal/External Switch-over
#pragma config FSOSCEN  = ON           	// Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF          	// Code Protect
#pragma config BWP      = OFF	        // Boot Flash Write Protect
#pragma config PWP      = OFF       	// Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF            // Background Debugger Enable
#pragma config FVBUSONIO = OFF
#pragma config FCANIO 	= OFF			// OFF Enables Alternate CAN I/O



/*
 *  Example Code Goals:
 *      1. Show how to read and write to/from IO_BUFFERS for the below peripherals:
 *          - UART4
 *          - CAN2
 *          - ADC1 (channel 9)
 * 
 *      2. Show how to declare and use a callback function for the data received 
 *      functions of the below peripherals:
 *          - UART4
 *          - CAN2
 *          - ADC1 (channel 9)
 * 
 *  Notes:  
 *      This demo code is designed to work with a custom PCB that allowed the
 *      use of all shown peripherals.  There are a total of 3 TASKs and 3 callback
 *      methods in use.  The each TASK will read data over either the UART, CAN,
 *      or ADC IO_BUFFERs.  In the case of the UART and CAN TASKs they will then
 *      loopback the data out.
 * 
 *      The 3 callback methods (1 for each peripheral RX callback) show how to
 *      implement the use of the callback and the arguments they take.  This allows
 *      a user to get raw access to the data coming in over the associated
 *      peripheral before it gets written to the IO_BUFFER.
 * 
 *      There are also two pictures in the project folder which show the 
 *      configuration of the RS-232 settings in Hyper Terminal, and the CAN bus
 *      settings in the Microchip CAN Bus Analyzer.
 */

// This is the buffer in RAM that the PIC32 CAN module uses
// So in essence we are double buffering the CAN data in RAM
BYTE gCANBuffer[CAN_TX_RX_MESSAGE_SIZE_BYTES * 32 * 2];





// CAN Task Example
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Change the numbers in the below defines to use a new ADC.
// They both must have the same number for the example to work.
// Also be sure that if you change them, to change USING_ADC_x_IO_BUFFER 
// inside of RTOSConfig.h accordingly.
#define CAN_BUFFER_ID                                       IO_BUFFER_CAN_2_ID
#define CAN_ID                                              CAN2

#define DEFAULT_SEG_1_TQ                                    CAN_BIT_3TQ
#define DEFAULT_SEG_2_TIME_SELECT                           TRUE
#define DEFAULT_SEG_2_TQ                                    CAN_BIT_3TQ
#define DEFAULT_PROPAGATION_SEG_TQ                          CAN_BIT_3TQ
#define DEFAULT_SAMPLE_3_TIMES                              TRUE
#define DEFAULT_SYNC_JUMP_WIDTH                             CAN_BIT_2TQ

// This is the Callback for the CAN RX Interrupt, it is optional
void CAN2InterruptRXCallback(BYTE *Buffer, UINT32 DataSizeInBytes)
{
    /*
        Below is an example of getting a handle on the incoming data, then
        do something meaningful with it.
     
        CANRxMessageBuffer *CANRXDataPacket;

        CANRXDataPacket = (CANRxMessageBuffer*)Buffer;

        // now do something with CANRXDataPacket if need be
    */
}

// This is just a supporting configuration method used to initialize the CAN mode
// and has nothing to do with the actual RTOS.
BOOL InitCAN(CAN_MODULE Module, void *Buffer, UINT32 BufferSizeInBytes, CAN_BIT_CONFIG *CANBitConfig, UINT32 CANBusSpeed)
{
    CANEnableModule(Module, (BOOL)TRUE);

    CANSetOperatingMode(Module, CAN_CONFIGURATION);

    while(CANGetOperatingMode(Module) != CAN_CONFIGURATION);

    CANSetSpeed(Module, CANBitConfig, GetPeripheralClock(), CANBusSpeed);

    CANAssignMemoryBuffer(Module, (void*)Buffer, (UINT32)BufferSizeInBytes);

    return (BOOL)TRUE;
}

// This is just a supporting configuration method used to initialize the CAN mode
// and has nothing to do with the actual RTOS.
BOOL CANDisableFilter(CAN_MODULE CANModule, CAN_FILTER CANFilterToDisable)
{
    CANEnableFilter(CANModule, CANFilterToDisable, FALSE);
    
    while(CANIsFilterDisabled(CANModule, CANFilterToDisable) != TRUE)
        TaskDelayTicks(1);
    
    return TRUE;
}

// This is just a supporting configuration method used to initialize the CAN mode
// and has nothing to do with the actual RTOS.
BOOL CANModuleEnable(CAN_MODULE Module, BOOL Enable)
{
    CAN_OP_MODE CANOpMode;

    CANOpMode = CANGetOperatingMode(Module);
	
	// if we're already in the mode we asked for then just return
	if(CANOpMode == CAN_NORMAL_OPERATION && Enable == TRUE)
		return (BOOL)TRUE;
	
	// if we're already in the mode we asked for then just return
	if(CANOpMode == CAN_DISABLE && Enable == FALSE)
		return (BOOL)TRUE;

    if(Enable == TRUE)
        CANSetOperatingMode(Module, CAN_NORMAL_OPERATION);
    else
        CANSetOperatingMode(Module, CAN_DISABLE);

    while(CANGetOperatingMode(Module) == CANOpMode);

    return TRUE;
}

// This is just a supporting configuration method used to initialize the CAN mode
// and has nothing to do with the actual RTOS.
void InitializeCANHardware(CAN_MODULE Module, BYTE ThisNodeID, CAN_BIT_CONFIG *CANBitConfig, UINT32 CANBusSpeed)
{
    if(InitCAN(Module, (void*)gCANBuffer, sizeof(gCANBuffer), CANBitConfig, CANBusSpeed) == FALSE)
        while(1);

    // Configure TX channel
    CANConfigureChannelForTx(Module, CAN_CHANNEL0, 32, CAN_TX_RTR_DISABLED, CAN_HIGHEST_PRIORITY);
    
    // Configure RX Channel
    CANConfigureChannelForRx(Module, CAN_CHANNEL1, 32, CAN_RX_FULL_RECEIVE);

    // First disable the filters we will be using
    if(CANDisableFilter(Module, CAN_FILTER0) == FALSE)
        while(1);
    
    // This is the Client/Server Filter
    CANConfigureFilter(Module, CAN_FILTER0, 0x080 | ThisNodeID, CAN_SID);

    // Now configure the default masks
    // This one is for the Client/Server
    CANConfigureFilterMask(Module, CAN_FILTER_MASK0, 0x1BF, CAN_SID, CAN_FILTER_MASK_IDE_TYPE);
    
    // Now link our Filters, Masks, and Channels together.
    CANLinkFilterToChannel(Module, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1);
    
    // Enable the Filters
    CANEnableFilter(Module, CAN_FILTER0, TRUE);

    // Now re-enable the CAN module
    if(CANModuleEnable(Module, TRUE) == FALSE)
        while(1);
}

/*
 * This TASK will take a single packet over the CAN bus and send it to SID 0x0FE.
 * The incoming SID should be 0x09B to address this node.
 * 
 * The CAN bus speed is 250,000 bits/sec by default.
 */
UINT32 CANBufferTaskCode(void *Args)
{
    CAN_BIT_CONFIG CANBitConfig;
    CANRxMessageBuffer CANTaskPacket;
    UINT32 BytesWritten = 0, BytesRead = 0;
    
    
    
    // CAN Specific Configuration Code
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    // Initialize the default parameters for the bit segments
    CANBitConfig.phaseSeg1Tq = DEFAULT_SEG_1_TQ;
    CANBitConfig.phaseSeg2TimeSelect = DEFAULT_SEG_2_TIME_SELECT;
    CANBitConfig.phaseSeg2Tq = DEFAULT_SEG_2_TQ;
    CANBitConfig.propagationSegTq = DEFAULT_PROPAGATION_SEG_TQ;
    CANBitConfig.sample3Time = DEFAULT_SAMPLE_3_TIMES;
    CANBitConfig.syncJumpWidth = DEFAULT_SYNC_JUMP_WIDTH;

    // Initialize the default parameters for the bus with a speed of 250,000 bits/sec
    // with our ID being 27, the SID value 155 (0x09B) is used to communicate to us
    InitializeCANHardware(CAN_ID, 27, &CANBitConfig, 250000);
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
        
    // Create the RX buffer with 128 bytes
    // If we passed in the buffers it's more efficient because we wouldn't be
    // using the OS Heap
    if(InitIOBuffer(CAN_BUFFER_ID, (BYTE*)NULL, 128) != OS_SUCCESS)
        while(1);
    
    // Now obtain the CAN_BUFFER_ID so we can use it
    if(GetIOBuffer(CAN_BUFFER_ID, 0) != OS_SUCCESS)
        while(1);
        
        
        
    // Main TASK loop
    //------------------------------------------------------------------------------------------------------------------------------------------------
    while(1)
    {
        // Read 1 packet of data over the CAN bus.
        // regardless of the DLC field, we will always get a packet with 8 bytes in it
        // in the CAN IO_BUFFER with this port and configuration, the DLC is just telling
        // us how many bytes of the 8 are valid.
        if(IOBufferReadBytes(CAN_BUFFER_ID, (BYTE*)&CANTaskPacket, sizeof(CANTaskPacket), sizeof(CANTaskPacket), &BytesRead, WAIT_FOREVER) != OS_SUCCESS)
            while(1);
            
        // Modify the SID to this just to show we got it
        // and resend with the same data otherwise
        CANTaskPacket.msgSID.SID = 0xFE;
        
        // Now write 1 CAN packet out on the bus with the modified SID value
        if(IOBufferWriteBytes(CAN_BUFFER_ID, (BYTE*)&CANTaskPacket, sizeof(CANTaskPacket), &BytesWritten, 1000) != OS_SUCCESS)
            while(1);
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    // We should never get here
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





// UART Task Example
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Change the numbers in the below defines to use a new UART.
// They both must have the same number for the example to work.
// Also be sure that if you change them, to change USING_UART_x_IO_BUFFER 
// inside of RTOSConfig.h accordingly.
#define UART_BUFFER_ID                                  IO_BUFFER_UART_4_ID
#define UART_ID                                         UART4

// This is the Callback for the UART RX Interrupt, it is optional
void UART4InterruptRXCallback(BYTE Data)
{
    /*
     * This function is called each time a single byte is received 
     * over the specified UART.  That does not mean the interrupt was
     * technically triggered for each byte though.
     * 
     * The argument BYTE Data is the byte which the UART received.
     */
}

// This method just configures up the parameters for the UART hardware
void UARTInit(void)
{
	UARTConfigure(UART_ID, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTSetLineControl(UART_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
	UARTSetDataRate(UART_ID, GetPeripheralClock(), 9600);
	UARTEnable(UART_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}

/*
 * This TASK will take a single line over the UART and send it back out.
 * 
 * A line ends with the sequence "\r\n" or in hex 0x0D 0x0A.
 * 
 * The UART is configured for 9600 8N1.
 */
UINT32 UARTBufferTaskCode(void *Args)
{
    BYTE UARTTaskBuffer[128];
    UINT32 BytesWritten = 0, BytesRead = 0;
    
    
    
    // initialize our UART to 9600 8N1
    UARTInit();
    
    
    
    // Create the RX buffer with 128 bytes
    // If we passed in the buffers it's more efficient because we wouldn't be
    // using the OS Heap
    if(InitIOBuffer(UART_BUFFER_ID, (BYTE*)NULL, 128) != OS_SUCCESS)
        while(1);
    
    // Now obtain the UART_BUFFER_ID so we can use it
    if(GetIOBuffer(UART_BUFFER_ID, 0) != OS_SUCCESS)
        while(1);
    
        
        
    // Main TASK loop
    //------------------------------------------------------------------------------------------------------------------------------------------------
    while(1)
    {
        // Now wait forever for a new line to come in over the UART.
        // Since we didn't modify the new line sequence it is the default sequence
        // of "\r\n" or in hex 0x0D 0x0A.
        if(IOBufferReadLine(UART_BUFFER_ID, UARTTaskBuffer, sizeof(UARTTaskBuffer), &BytesRead, WAIT_FOREVER) != OS_SUCCESS)
            while(1);
        
        // Just spit back whatever we got over the UART
        if(IOBufferWriteBytes(UART_BUFFER_ID, UARTTaskBuffer, BytesRead, &BytesWritten, 1000) != OS_SUCCESS)
            while(1);
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    // We should never get here
    return 0;
}





// ADC Task Example
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Change the numbers in the below defines to use a new ADC.
// They both must have the same number for the example to work.
// Also be sure that if you change them, to change USING_ADC_x_IO_BUFFER 
// inside of RTOSConfig.h accordingly.
#define ADC_BUFFER_ID                                   IO_BUFFER_ADC_1_ID
#define ADC_ID                                          ADC1
#define ANALOG_PIN                                      ENABLE_AN9_ANA
#define ANALOG_CHANNEL                                  9

// This is the Callback for the ADC Interrupt, it is optional
void ADC1InterruptCallback(UINT16 ADCValue)
{
    /*
     * This function is called each time a single byte is received 
     * over the specified ADC.  That does not mean the interrupt was
     * technically triggered for each byte though.
     * 
     * The argument UINT16 Data is the data which the ADC received.
     */
}

/*
 * This TASK will read 64 bytes from the the ADC IO_BUFFER and then wait
 * again for the data.
 * 
 * The way this example is configured the ADC sampling will just keep going.
 * 
 */
UINT32 ADCBufferTaskCode(void *Args)
{
    UINT16 ADCTaskBuffer[64];
    UINT32 BytesRead = 0;
    
    
    
    // ADC Specific Configuration Code
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    AD1PCFG = 0xFFFF;
    
    // Make sure module is off
    AD1CON1 = 0;

    AD1CON1bits.FORM = 0b000; // set output format to 16-bit integer
    AD1CON1bits.SSRC = 0b111; // use ADC internal counter for sample time
    AD1CON1bits.ASAM = 1; // auto sampling is on
    //AD1CON1bits.CLRASAM = 1; // clear the ASAM bit once the ADC interrupt is generated

    AD1CON2 = 0; // all default values
    AD1CON2bits.SMPI = 0b1111; // 16 samples per interrupt

    AD1CON3bits.SAMC = 0b11111; // maximum sample time
    AD1CON3bits.ADCS = 0b11111111; // slowest sample clock rate

    AD1CHS = 0; // this is set when a sample is being done, this reg will end
                // up being shared amongst all the other software modules that
                // need to do an ADC conversion

    AD1CSSL = 0; // we're not going to scan

    // Turn the ADC module on
    AD1CON1bits.ADON = 1;
    
    // set the physical pin into analog mode
    AD1PCFGCLR |= ANALOG_PIN;
    
    // now select the pin mux we will be using
    AD1CHSbits.CH0SA = ANALOG_CHANNEL;
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    // Create 1 buffer for RX at 256 bytes large.
    // If we passed in the buffers it's more efficient because we wouldn't be
    // using the OS Heap
    if(InitIOBuffer(ADC_BUFFER_ID, (BYTE*)NULL, 256) != OS_SUCCESS)
        while(1);
    
    // Now obtain the ADC_BUFFER_ID so we can use it
    if(GetIOBuffer(ADC_BUFFER_ID, 0) != OS_SUCCESS)
        while(1);
    
        
        
    // ADC Specific Configuration Code
    //------------------------------------------------------------------------------------------------------------------------------------------------
        
    // Begin the ADC sampling
    AD1CON1bits.ASAM = 1;
    AD1CON1bits.SAMP = 1;
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    // Main TASK loop
    //------------------------------------------------------------------------------------------------------------------------------------------------
    while(1)
    {
        // First clear out the buffer before reading
        memset((void*)ADCTaskBuffer, 0, sizeof(ADCTaskBuffer));
        
        // Now wait forever for sizeof(ADCTaskBuffer) ADC samples to come in.
        // With this port they are generated and read in chunks of 16
        if(IOBufferReadBytes(ADC_BUFFER_ID, (BYTE*)ADCTaskBuffer, sizeof(ADCTaskBuffer), sizeof(ADCTaskBuffer), &BytesRead, WAIT_FOREVER) != OS_SUCCESS)
            while(1);
        
        if(BytesRead != sizeof(ADCTaskBuffer))
        {
            // do some error handling here
            while(1);
        }
            
        // Now do something with the data in ADCTaskBuffer
        
    }
    //------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    // We should never get here
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





int main(int argc, char** argv)
{
    while(OSCCONbits.SLOCK != 1);

	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
    
    SetSystemClocks(GetSystemClock());
    
    // InitOS() must be called BEFORE ANY OTHER OS CALL!!!!
    if(InitOS() != OS_SUCCESS)
        while(1);

    // Now we are going to create our TASKs
    if(CreateTask(CANBufferTaskCode, 500, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    if(CreateTask(UARTBufferTaskCode, 500, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    if(CreateTask(ADCBufferTaskCode, 500, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
