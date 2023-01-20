/*
    NexOS Kernel Version v1.02.01
    Copyright (c) 2023 brodie

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

#include "HardwareProfile.h"

#include "../NexOS/Kernel/Task.h"
#include "../NexOS/Kernel/Memory.h"

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
 *      1. Show how to allocate and release memory fromt he OS heap.
 *      2. Show how to get the heap usage.
 *      3. Show how to use the heap usage events.
 * 
 *  Notes:  
 *      This demo code is designed to work with the MPLAB X IDE Simulator.
 */

/*
 * This is the TASK code which will allocate and deallocate space on the OS heap.
 */
UINT32 OSHeapDemoCode(void *Args)
{
    void *MemoryArray[100];
    UINT32 i;
    UINT32 StartingHeapUsage = GetHeapUsedInBytes();

    // initialize the memory array
    for(i = 0; i < sizeof(MemoryArray) / sizeof(void *); i++)
        MemoryArray[i] = (void*)NULL;
    
    printf("\r\n%li bytes used in heap.\r\n%li bytes remain in heap.\r\n", GetHeapUsedInBytes(), GetHeapRemainingInBytes());
    
    while(1)
    {
        for(i = 0; i < sizeof(MemoryArray) / sizeof(void *); i++)
        {
            // allocate 1000 bytes
            MemoryArray[i] = AllocateMemory(1000);
            
            // print out all relevant memory usage
            printf("\r\n%li bytes used in heap.\r\n%li bytes remain in heap.\r\n", GetHeapUsedInBytes(), GetHeapRemainingInBytes());
            
            // we didn't get our allocation
            if(MemoryArray[i] == (void*)NULL)
                break;
        }
        
        for(i = 0; i < sizeof(MemoryArray) / sizeof(void *); i++)
        {
            // allocate 1000 bytes
            if(ReleaseMemory(MemoryArray[i]) == FALSE)
                break;
            
            // print out all relevant memory usage
            printf("\r\n%li bytes used in heap.\r\n%li bytes remain in heap.\r\n", GetHeapUsedInBytes(), GetHeapRemainingInBytes());
            
            // we didn't get our allocation
            if(MemoryArray[i] == (void*)NULL)
                break;
        }
        
        // initialize the memory array
        for(i = 0; i < sizeof(MemoryArray) / sizeof(void *); i++)
            MemoryArray[i] = (void*)NULL;
        
        // if the starting heap usage does not equal the current at this point, there is an error in Memory.c, or the OS heap is
        // somehow getting clobbered and Memory.c is fine, or both.
        if(StartingHeapUsage != GetHeapUsedInBytes())
        {
            // we have a problem here
            printf("\r\nStarting bytes used = %li and current bytes used = %li.\r\nThese values should be the same!\r\n", StartingHeapUsage, GetHeapUsedInBytes());
            
            while(1);
        }
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
    
    // clear out the LATD
    LATDCLR = 0xFFFF;
    
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
    if(CreateTask(OSHeapDemoCode, 5000, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
