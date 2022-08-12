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

#include "HardwareProfile.h"

#include "../NexOS/Kernel/Task.h"

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
 *      1. Show how to create and run some simple TASKS.
 * 
 *  Notes:  
 *      This demo code is designed to work with the Ethernet Starter Kit II.  This
 *      project shows the bare minimum to get the NexOS up and running.
 */

/*
 * This is the TASK code which will toggle RD0 at a specific rate.
 */
UINT32 Task1Code(void *Args)
{
    // make RD0 an output
    TRISDCLR = 0x0001;
    
    while(1)
    {
        LATDINV = 0x0001;
        
        #ifdef SIMULATION
            printf("TASK 1 RD0 TOGGLE\r\n");
        #endif // end of #if SIMULATION
        
        TaskDelayMilliseconds(100);
    }
}

/*
 * This is the TASK code which will toggle RD1 at a specific rate.
 */
UINT32 Task2Code(void *Args)
{
    // make RD1 an output
    TRISDCLR = 0x0002;
    
    while(1)
    {
        LATDINV = 0x0002;
        
        #ifdef SIMULATION
            printf("TASK 2 RD1 TOGGLE\r\n");
        #endif // end of #if SIMULATION
        
        TaskDelayMilliseconds(200);
    }
}

/*
 * This is the TASK code which will toggle RD2 at a specific rate.
 */
UINT32 Task3Code(void *Args)
{
    // make RD2 an output
    TRISDCLR = 0x0004;
    
    while(1)
    {
        LATDINV = 0x0004;
        
        #ifdef SIMULATION
            printf("TASK 3 RD2 TOGGLE\r\n");
        #endif // end of #if SIMULATION
        
        TaskDelayMilliseconds(300);
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
    if(CreateTask(Task1Code, 300, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
            
    if(CreateTask(Task2Code, 300, 2, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    if(CreateTask(Task3Code, 300, 3, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
