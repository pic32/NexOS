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
#include "../NexOS/Semaphore/Binary Semaphore/BinarySemaphore.h"
#include "../NexOS/Timer/SoftwareTimer.h"

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
 *      1. Show how to create multiple TASKs that will execute the same code (found in main() method).
 *      2. Show passing starting arguments to a TASK (found in main() method).
 *          2a. The argument specified in CreateTask() is void *Args in the method UINT32 AdvancedTaskCreationDemonstrationCode(void *Args).
 *          2b. Use the void *Args argument to determine how long to block for.
 *      3. Execute the same code by multiple TASKs (found in AdvancedTaskCreationDemonstrationCode() method).
 *          3a. Show using local variables in the TASK code, yet each are unique to the TASK.
 *          3b. Show a TASK referencing whatever its name is.
 *          3c. Show how a TASK with no explicit reference to itself can delete itself. 
 * 
 *  Notes:  
 *      This demo code is designed to work with the MPLAB X IDE Simulator.  Each task will
 *      toggle an output based off of the Arg it got.  It will then delete itself after some time.
 */

// This BINARY_SEMAPHORE is used to control access to the printf() function
BINARY_SEMAPHORE *gPrintfSemaphore;

/*
 * This is the TASK code which will toggle an output at a specific rate.
 * The TASK will then delete itself.
 */
UINT32 AdvancedTaskCreationDemonstrationCode(void *Args)
{
    UINT32 Count = 0;
    SOFTWARE_TIMER *SoftwareTimer = CreateSoftwareTimer((SOFTWARE_TIMER*)NULL, FALSE);
    
    // Since we are a multi tasking environment we should not read modify write.
    // Clear the bit in TRISA which we got passed in as an Argument.  This will be an output.
    TRISACLR = 1 << (UINT32)Args;
    
    while(1)
    {
        // Clear and enable the local SOFTWARE_TIMER
        if(SoftwareTimerRestart(SoftwareTimer) != OS_SUCCESS)
            while(1);

        // now delay for the specified milliseconds
        if(TaskDelayMilliseconds(1 << (UINT32)Args) != OS_SUCCESS)
            while(1);
        
        // get the printf semaphore before calling printf.  This is because it is a shared resource amongst the tasks.
        if(GetBinarySemaphore(gPrintfSemaphore, WAIT_FOREVER) != OS_SUCCESS)
            while(1);
        
        // print out the status messages
        printf("%s: Waited for %u milliseconds\n", GetTaskName((TASK*)NULL), (unsigned int)SoftwareTimerGetMilliseconds(SoftwareTimer));
        
        printf("%s: Toggling LATA bit %i\n", GetTaskName((TASK*)NULL), (int)Args);
        
        // release the printf semaphore so someone else can use printf()
        if(ReleaseBinarySemaphore(gPrintfSemaphore) != OS_SUCCESS)
            while(1);
        
        // now toggle our output pin
        LATAINV = 1 << (UINT32)Args;
        
        // if we have done our task, delete ourselves
        if(Count == (UINT32)(1 << (UINT32)Args))
        {
            // get the printf semaphore before calling printf.  This is because it is a shared resource amongst the tasks.
            if(GetBinarySemaphore(gPrintfSemaphore, WAIT_FOREVER) != OS_SUCCESS)
                while(1);
        
            // print out the status messages
            printf("%s: Deleting itself.  Goodbye cruel world...\n", GetTaskName((TASK*)NULL));

            // release the printf semaphore so someone else can use printf())
            if(ReleaseBinarySemaphore(gPrintfSemaphore) != OS_SUCCESS)
                while(1);
                
            // now delete our local SOFTWARE_TIMER before we delete ourselves as we don't want any memory leaks!
            if(SoftwareTimerDelete(SoftwareTimer) != OS_SUCCESS)
                while(1);
        
            // now delete ourselves.  We pass in a (TASK*)NULL to specify that we want to delete the currently executing task.
            if(DeleteTask((TASK*)NULL) != OS_SUCCESS)
                while(1);
        }
        else
        {
            Count++;
            
            // get the printf semaphore before calling printf.  This is because it is a shared resource amongst the tasks.
            if(GetBinarySemaphore(gPrintfSemaphore, WAIT_FOREVER) != OS_SUCCESS)
                while(1);
        
            // print out the status messages
            printf("%s: %i of %i loops done!\n", GetTaskName((TASK*)NULL), (int)Count, (int)(1 << (UINT32)Args));

            // release the printf semaphore so someone else can use printf())
            if(ReleaseBinarySemaphore(gPrintfSemaphore) != OS_SUCCESS)
                while(1);
        }
    }
}

int main(int argc, char** argv)
{
    UINT32 i;
    BYTE TaskName[10];

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

    // Now we are going to create 6 TASKs
    for(i = 0; i < 6; i++)
    {
        sprintf((char*)TaskName, "TASK %i", (int)i + 1);
        
        // Now we are creating 6 TASKs that will all execute the same program.
        // The only difference is the void *Args passed to each one and the task name.
        if(CreateTask(AdvancedTaskCreationDemonstrationCode, 1200, 1 + i, (void*)i, FALSE, TaskName, (TASK*)NULL) == (TASK*)NULL)
            while(1);
    }
    
    // Create a BINARY_SEMAPHORE which is used to control access to the printf() method.
    // Note, we CANNOT set BOOL GetBinarySemaphore to TRUE since we are not executing out of a TASK.
    gPrintfSemaphore = CreateBinarySemaphore((BINARY_SEMAPHORE*)NULL, FALSE);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
