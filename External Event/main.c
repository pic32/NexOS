/*
    NexOS Kernel Version v1.01.05
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
#include <plib.h>

#include "HardwareProfile.h"

#include "../NexOS/Event/Event.h"
#include "../NexOS/Kernel/Task.h"
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
 *      1. Show how to run a TASK when an EVENT occurs on pin RD6.
 * 
 *  Notes:  
 *      This demo code is designed to work with the Ethernet Starter Kit II, but can
 *      be run in the simulator.  When using MPLAB X IDE go to Window-->Simulator-->Stimulus
 *      to pull up the menu for triggering the change notification interrupt on RD6.  The task
 *      is waiting on the CHANGE_NOTIFICATION_INTERRUPT_EVENT to fire with the WaitOnEvent()
 *      method.  This is a blocking method and the task will stop executing once called.
 *      Once the CHANGE_NOTIFICATION_INTERRUPT_EVENT fires this TASK will toggle an LED.
 */

/*
 * This is the TASK code which will toggle RD0 at a specific rate.
 */
UINT32 EventTaskCode(void *Args)
{
    // make RD0 an output
    TRISDCLR = 0x00000001;
    
    // make sure RD6 is an input
    TRISDSET = 0x00000040;
    
    // now configure up the change notification interrupt
    EnterCritical();
    {
        mCNOpen(CN_ON, CN15_ENABLE, 0);
        
        ConfigIntCN(OS_PRIORITY | CHANGE_INT_ON);
    }
    ExitCritical();
    
    while(1)
    {
        // Now we will wait on the CHANGE_NOTIFICATION_INTERRUPT_EVENT to fire.
        if(WaitOnEvent(CHANGE_NOTIFICATION_INTERRUPT_EVENT, WAIT_FOREVER) == OS_SUCCESS)
        {
            BOOL PortStatus;
            
            // if we got into here the CHANGE_NOTIFICATION_INTERRUPT_EVENT event fired
            
            // toggle the LED
            LATDINV = 0x0001;
            
            #ifdef SIMULATION		
                // now print out a notification that we got triggered
                printf("CHANGE NOTIFICATION EVENT TRIGGERED!\r\n");
            #endif // end of #ifndef SIMULATION

            TaskDelayMilliseconds(30);
            
            PortStatus = PORTDbits.RD6;
                
            while(PORTDbits.RD6 == PortStatus)
                TaskDelayMilliseconds(5); // check back every 5 milliseconds

            TaskDelayMilliseconds(30);
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
    if(CreateTask(EventTaskCode, 1200, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
