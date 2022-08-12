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
#include <stdlib.h>
#include <plib.h>

#include "GenericTypes.h"
#include "HardwareProfile.h"
#include "../NexOS/Kernel/Kernel.h"
#include "../NexOS/Kernel/Memory.h"
#include "../NexOS/Kernel/Task.h"
#include "../NexOS/Semaphore/Binary Semaphore/BinarySemaphore.h"
#include "../NexOS/Timer/SoftwareTimer.h"
#include "../NexOS/Event/Event.h"
#include "../NexOS/Event/Event Timer/EventTimer.h"
#include "../NexOS/Message Queue/MessageQueue.h"
#include "../NexOS/Pipe/Pipe.h"
#include "../NexOS/IOBuffer/IOBuffer.h"

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
#pragma config FCANIO 	= ON			// OFF Enables Alternate CAN I/O

/*
 *  Example Code Goals:
 *      1. None
 * 
 *  Notes:  
 *      This project is just to enable every single option in RTOSConfig.h
 *      to make sure that the NexOS compiles with no errors.
 */

UINT32 TestTaskCode(void *Args)
{
    while(1)
    {
        
    }
}

int main(int argc, char** argv)
{
	#ifdef RELEASE
		// initialize the Asynchronous Serial Port
	#endif // end of #ifdef RELEASE

	#ifdef SIMULATION
        __XC_UART = 1;
	#endif // end of #if SIMULATION

    #ifndef SIMULATION		
        while(OSCCONbits.SLOCK != 1);
    #endif // end of #ifndef SIMULATION		

	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
    
    SetSystemClocks(GetSystemClock());
    
    if(InitOS() != OS_SUCCESS)
        while(1);
        
    #ifdef SIMULATION
    {
        BYTE KernelVersionString[16];
            
        printf("NexOS v%s\n\n", KernelVersionToString(KernelVersionString));
    }
	#endif // end of #if SIMULATION

    // Now we are going to create our TASKs
    if(CreateTask(TestTaskCode, 500, 1, (void*)NULL, FALSE, READY, FALSE, (BYTE*)NULL, (TASK_EXIT_CALLBACK)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    StartOSScheduler();
    
	return (EXIT_SUCCESS);
}
