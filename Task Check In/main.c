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
 *      1. Show how to use the TASK Check In feature.
 *      2. Show how to use the TASK Exit Callback feature.
 * 
 *  Notes:  
 *      This demo code is designed to work with the Ethernet Starter Kit II, but can
 *      be run in the simulator.  When using MPLAB X IDE go to Window-->Simulator-->Stimulus
 *      to pull up the menu for triggering the change notification interrupt.  If you do 
 *      not trigger the RD6 input often enough the CheckInTask will be killed off by
 *      the OS.
 */

// this is the callback method which is called once the CheckIn TASK is deleted.
void CheckInTaskExitCallback(void)
{
	#ifdef SIMULATION
        printf("You let the Check In TASK die...!\r\n");
	#endif // end of #if SIMULATION
}

/*
 * This is a TASK that needs to see the button toggle to stay alive!
 */
UINT32 CheckInTaskCode(void *Args)
{
    UINT32 i;
    
    // make RD0 an output
    TRISDCLR = 0x00000001;
    
    // make sure RD6 is an input
    TRISDSET = 0x00000040;
    
    // turn our LED off    
    LATDCLR = 0x00000001;
    
    // first off we need to create and register for the TASK Check In capability
    // If we don't check in every 5 seconds, we will be deleted!
    if(SetupTaskCheckIn((TASK_CHECK_IN*)NULL, MillisecondsToTicks(5000)) != OS_SUCCESS)
        while(1);
    
    while(1)
    {
        for(i = 0; i < 5; i++)
        {
            // if the button is pushed, perform a TASK checkin
            if(PORTDbits.RD6 == 1)
            {
                // we shall check in
                if(TaskCheckIn() != OS_SUCCESS)
                    while(1);
            }
            
            // now delay for 100 milliseconds
            TaskDelayMilliseconds(100);
        }
        
        LATDINV = 0x00000001;
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
    if(CreateTask(CheckInTaskCode, 300, 1, (void*)NULL, FALSE, (TASK_EXIT_CALLBACK)CheckInTaskExitCallback, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
