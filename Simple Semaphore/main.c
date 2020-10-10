/*
    NexOS Kernel Version v1.00.00
    Copyright (c) 2020 brodie

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
 *      1. Show how to create a BINARY_SEMAPHORE.
 *      2. Show a TASK obtaining a BINARY_SEMAPHORE.
 *      3. Show a TASK releasing a BINARY_SEMAPHORE.
 * 
 *  Notes:  
 *      This demo code is designed to work with the Ethernet Starter Kit II, but can
 *      be run in the simulator.  When using MPLAB X IDE go to Window-->Simulator-->Stimulus
 *      to pull up the menu for triggering the inputs.  There is a common semaphore that
 *      each task will try and get.  Each task is looking for a certain button to be pushed.  
 *      Once pushed the task will try and get the semaphore.  Once a task owns the semaphore
 *      it will turn on an LED.  At this time the task will look at the same button to
 *      see if it is pushed, and if so it will release the semaphore and turn off the LED.
 */

BINARY_SEMAPHORE *BinarySemaphore;

/*
 * This is the TASK code which will toggle RD0 at a specific rate.
 */
UINT32 Task1Code(void *Args)
{
    // make RD0 an output
    TRISDCLR = 0x00000001;
    
    // make sure RD6 is an input
    TRISDSET = 0x00000040;
    
    // turn our LED off    
    LATDCLR = 0x00000001;
    
    while(1)
    {
        // wait for it to go low
        while(PORTDbits.RD6 == 1)
            TaskDelayMilliseconds(5);
        
        // now wait for a debounce
        TaskDelayMilliseconds(30);
        
        // wait for it to go high
        while(PORTDbits.RD6 == 0)
            TaskDelayMilliseconds(5);
        
        // now get the Semaphore and wait forever for it
        if(GetBinarySemaphore(BinarySemaphore, WAIT_FOREVER) == OS_SUCCESS)
        {
            // we got it, turn our light on and wait for the button push again
            LATDSET = 0x00000001;
            
            // now wait for a debounce
            TaskDelayMilliseconds(30);
            
            // wait for it to go low
            while(PORTDbits.RD6 == 1)
                TaskDelayMilliseconds(5);

            // now wait for a debounce
            TaskDelayMilliseconds(30);

            // wait for it to go high
            while(PORTDbits.RD6 == 0)
                TaskDelayMilliseconds(5);
            
            // now release the BINARY_SEMAPHORE
            if(ReleaseBinarySemaphore(BinarySemaphore) != OS_SUCCESS)
                while(1);
                
            // turn our LED off    
            LATDCLR = 0x00000001;
                
            // now wait for a debounce
            TaskDelayMilliseconds(30);
        }
    }
}

/*
 * This is the TASK code which will toggle RD1 at a specific rate.
 */
UINT32 Task2Code(void *Args)
{
    // make RD1 an output
    TRISDCLR = 0x00000002;
    
    // make sure RD7 is an input
    TRISDSET = 0x00000080;
    
    // turn our LED off    
    LATDCLR = 0x00000002;
    
    while(1)
    {
        while(1)
        {
            // wait for it to go low
            while(PORTDbits.RD7 == 1)
                TaskDelayMilliseconds(5);

            // now wait for a debounce
            TaskDelayMilliseconds(30);

            // wait for it to go high
            while(PORTDbits.RD7 == 0)
                TaskDelayMilliseconds(5);

            // now get the Semaphore
            if(GetBinarySemaphore(BinarySemaphore, WAIT_FOREVER) == OS_SUCCESS)
            {
                // we got it, turn our light on and wait for the button push again
                LATDSET = 0x00000002;

                // now wait for a debounce
                TaskDelayMilliseconds(30);

                // wait for it to go low
                while(PORTDbits.RD7 == 1)
                    TaskDelayMilliseconds(5);

                // now wait for a debounce
                TaskDelayMilliseconds(30);

                // wait for it to go high
                while(PORTDbits.RD7 == 0)
                    TaskDelayMilliseconds(5);

                // now release the BINARY_SEMAPHORE
                if(ReleaseBinarySemaphore(BinarySemaphore) != OS_SUCCESS)
                    while(1);

                // turn our LED off    
                LATDCLR = 0x00000002;

                // now wait for a debounce
                TaskDelayMilliseconds(30);
            }
        }
    }
}

/*
 * This is the TASK code which will toggle RD2 at a specific rate.
 */
UINT32 Task3Code(void *Args)
{
    // make RD2 an output
    TRISDCLR = 0x00000004;
    
    // make sure RD13 is an input
    TRISDSET = 0x00002000;
    
    // turn our LED off    
    LATDCLR = 0x00000004;
    
    while(1)
    {
        while(1)
        {
            // wait for it to go low
            while(PORTDbits.RD13 == 1)
                TaskDelayMilliseconds(5);

            // now wait for a debounce
            TaskDelayMilliseconds(30);

            // wait for it to go high
            while(PORTDbits.RD13 == 0)
                TaskDelayMilliseconds(5);

            // now get the Semaphore
            if(GetBinarySemaphore(BinarySemaphore, WAIT_FOREVER) == OS_SUCCESS)
            {
                // we got it, turn our light on and wait for the button push again
                LATDSET = 0x00000004;

                // now wait for a debounce
                TaskDelayMilliseconds(30);

                // wait for it to go low
                while(PORTDbits.RD13 == 1)
                    TaskDelayMilliseconds(5);
                    
                // now wait for a debounce
                TaskDelayMilliseconds(30);

                // wait for it to go high
                while(PORTDbits.RD13 == 0)
                    TaskDelayMilliseconds(5);

                // now release the BINARY_SEMAPHORE
                if(ReleaseBinarySemaphore(BinarySemaphore) != OS_SUCCESS)
                    while(1);

                // turn our LED off    
                LATDCLR = 0x00000004;

                // now wait for a debounce
                TaskDelayMilliseconds(30);
            } 
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
    if(CreateTask(Task1Code, 300, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
            
    if(CreateTask(Task2Code, 300, 2, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    if(CreateTask(Task3Code, 300, 3, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    // Create the BINARY_SEMAPHORE in the OS heap, and do not get ownership of it!
    // If we wanted to get ownership of it we need to create it in a TASK.
    BinarySemaphore = CreateBinarySemaphore((BINARY_SEMAPHORE*)NULL, FALSE);
    
    // make sure it was allocated and created ok.
    if(BinarySemaphore == (BINARY_SEMAPHORE*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
