/*
    NexOS Kernel Version v1.01.00
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

#include "HardwareProfile.h"

#include "../NexOS/Kernel/Task.h"
#include "../NexOS/Pipe/Pipe.h"

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
 *      1. Show how to use a PIPE to communicate between 2 tasks.
 * 
 *  Notes:  
 *      This demo code is designed to work with the the MPLAB Simulator.
 */

// this is the PIPE that the two tasks will use to communicate through
PIPE gPipe;

/*
 * This TASK produces data to be placed in the PIPE.
 */
UINT32 DataProducerTask(void *Args)
{
    BYTE TempBuffer[64];
    UINT32 BytesWritten;
    UINT32 MessageCount = 1;
    
    // create a buffer with 256 bytes of space in it
    if(CreatePipe(&gPipe, (BYTE*)NULL, 256) == (PIPE*)NULL)
        while(1);
    
    while(1)
    {
        // now begin to write data to the buffer
        sprintf((char*)TempBuffer, "This is the %li message written!\n", MessageCount++);
        
        if(PipeWrite(&gPipe, TempBuffer, strlen((char*)TempBuffer), &BytesWritten) != OS_SUCCESS)
            while(1);
    }
}

/*
 * This TASK consumes data that was placed in the PIPE.
 */
UINT32 DataConsumerTask(void *Args)
{
    BYTE TempBuffer[64];
    UINT32 BytesRead;
    
    while(1)
    {
        // clear out our buffer
        memset((void*)TempBuffer, 0, sizeof(TempBuffer));
        
        // read the data in 63 byte chunks
        if(PipeRead(&gPipe, TempBuffer, sizeof(TempBuffer), 63, &BytesRead) != OS_SUCCESS)
            while(1);
            
        #ifdef SIMULATION
            // now print out the data we read from the buffer
            printf("%s", TempBuffer);
        #endif // end of #if SIMULATION
    }
}

int main(int argc, char** argv)
{
	#ifdef SIMULATION
        __XC_UART = 1;
	#endif // end of #if SIMULATION

    #ifndef SIMULATION		
        while(OSCCONbits.SLOCK != 1);
    #endif // end of #ifndef SIMULATION		

	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
    
    SetSystemClocks(GetSystemClock());

    // InitOS() must be called BEFORE ANY OTHER OS CALL!!!!
    if(InitOS() != OS_SUCCESS)
        while(1);
        
    #ifdef SIMULATION
    {
        BYTE KernelVersionString[16];
            
        printf("NexOS v%s\n\n", KernelVersionToString(KernelVersionString));
    }
	#endif // end of #if SIMULATION

    // Now we are going to create our TASKs
        
    // this is the producer task
    if(CreateTask(DataProducerTask, 300, 2, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    // this is the consumer task
    if(CreateTask(DataConsumerTask, 300, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
