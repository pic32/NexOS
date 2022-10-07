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

#include "CriticalSection.h"
#include "../NexOS/Event/Event Timer/EventTimer.h"
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
 *      1. Show how to synchronize an EVENT_TIMER and a TASK between EVENTS.
 * 
 *  Notes:  
 *      This demo code is designed to work with the Ethernet Starter Kit II, but can
 *      be run in the simulator.  When using MPLAB X IDE go to Window-->Simulator-->Stimulus
 *      to pull up the menu for triggering the change notification interrupt on RD6.  The EVENT_TIMER 
 *      is configured to measure the time between the CHANGE_NOTIFICATION_INTERRUPT_EVENT and 
 *      the USER_1_EVENT.  You need to use the stimulus (or RD6 button) to trigger the
 *      CHANGE_NOTIFICATION_INTERRUPT_EVENT.  At which point the EVENT_TIMER will start
 *      counting.  Then once the UserEventTaskCode() triggers the USER_1_EVENT the EVENT_TIMER
 *      will stop counting and the EventTaskCode() will begin execution as it was waiting
 *      on the USER_1_EVENT to fire.  Then the EventTaskCode() will print out the time between
 *      the two events.
 */

// this the EVENT_TIMER which will trigger on the CHANGE_NOTIFICATION_INTERRUPT_EVENT
// and the USER_1_EVENT.
EVENT_TIMER gEventTimer;

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
        
        ConfigIntCN(CHANGE_INT_PRI_1 | CHANGE_INT_ON);
    }
    ExitCritical();
    
    // now create the EVENT_TIMER.
    // It will start on the CHANGE_NOTIFICATION_INTERRUPT_EVENT and stop on the USER_1_EVENT.
    // It will only start if it is not running (it still needs to be enabled too), and when it 
    // stops it will just disable itself.
    if(CreateEventTimer(&gEventTimer, CHANGE_NOTIFICATION_INTERRUPT_EVENT, USER_1_EVENT, ONLY_START_WHEN_NOT_RUNNING, STOP_DISABLE_TIMER, TRUE) == (EVENT_TIMER*)NULL)
        while(1);
    
    while(1)
    {
        // Now we will wait on the USER_1_EVENT to fire.
        // this way we sync ourselves up with the 
        if(WaitOnEvent(USER_1_EVENT, WAIT_FOREVER) == OS_SUCCESS)
        {
            // if we got into here the USER_1_EVENT event fired
            LATDINV = 0x0001;
            
            // if the EVENT_TIMER is running, get the time elapsed
            if(EventTimerGetTicks(&gEventTimer) != 0)
            {
                #ifdef SIMULATION		
                    // now print out the data, this is the time (in OS Ticks) between the lastest CHANGE_NOTIFICATION_INTERRUPT_EVENT and USER_1_EVENT
                    printf("TIME BEWTEEN EVENTS WAS %i OS TICKS\r\n", (unsigned int)EventTimerGetTicks(&gEventTimer));
                #endif // end of #ifndef SIMULATION

                // since we don't want this to be a cumulative EVENT_TIMER we will clear it then enable it.
                if(EventTimerClearTimer(&gEventTimer) != OS_SUCCESS)
                    while(1);

                // now enable the EVENT_TIMER
                if(EventTimerSetEnable(&gEventTimer, TRUE) != OS_SUCCESS)
                    while(1);
            }
        }
    }
}

UINT32 UserEventTaskCode(void *Args)
{
    // seed it with the core timer
    srand(ReadCoreTimer());
    
    while(1)
    {
        // wait for a random amount of time (sort of))
        TaskDelayMilliseconds(50 + ((rand() + 1) / 1000000));
        
        // now we will raise the user event
        if(RaiseEvent(USER_1_EVENT) != OS_SUCCESS)
            while(1);
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
    if(CreateTask(EventTaskCode, 1200, 2, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    // Now we are going to create our TASKs
    if(CreateTask(UserEventTaskCode, 1200, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
