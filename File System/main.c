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

#include "HardwareProfile.h"

#include "../NexOS/Kernel/Task.h"
#include "ff.h"

#ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
    #include "FlashController.h"
#endif // end of #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT

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

#ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
    #pragma config FCANIO = ON // this is default CAN I/O, OFF = USE ALTERNATE CAN PINS
#else
    #pragma config FCANIO 	= OFF			// OFF Enables Alternate CAN I/O
#endif // end of #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT

/*
 *  Example Code Goals:
 *      1. Show the integration of the FatFs with the NexOS.
 *      2. Show 2 TASKs writing to their own files in different locations.
 * 
 *  Notes:  
 *      This demo code is designed to work with a custom PCB to show off
 *      using the FatFs with the NexOS.  Two TASKs write to a file of their
 *      own on the default media.  Once 100 writes are performed by the higher
 *      priority TASK the test concludes.
 * 
 *      There are 2 project configurations for this demo.  One interacts with
 *      the SD card for the default file system.  The other interacts with
 *      a SST26VF064B serial flash.  This project also is an example
 *      of how to interact with flash in general, and how to abstract the 
 *      flash page from the file system by using a RAM buffer.
 * 
 *      The flash driver was designed in such a way that another flash could
 *      be supported should it implement the same list of functions being used
 *      by the file FlashController.h.  This allows a little more portability.
 */

// this is a data structure for the FatFs
// it is used to access the file system
// on the SD card.
FATFS gDefaultFat;

// this is a flag used to denote the test has completed
volatile BOOL gComplete = FALSE;

/*
 * This TASK will just continuously write a message a file as long as the
 * test continues.  It is interrupted by the higher priority TASK every 2
 * OS ticks.
 */
UINT32 LowPriorityFileTaskCode(void *Args)
{
    FIL File;
    UINT BytesWritten;
    BYTE Message[] = {"THIS IS A TEST OF THE FATFS WITH THE NEXOS!\r\n"};
    
    // open up our test file
    if(f_open(&File, "\\user\\LowPriorityFile.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
        while(1);
        
    while(1)
    {
        // if we've written enough, close the file and just keep delaying
        if(gComplete == TRUE)
        {
            // close our file
            f_close(&File);

            // shutdown the file system
            f_mount(0, "", 0);
            
            #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT
                if(FlashControllerFlushBufferToMedia() != FLASH_CONTROLLER_OK)
                {
                    while(1)
                        TaskDelayTicks(10);
                }
            #endif // end of #ifdef FLASH_FILE_SYSTEM_EXAMPLE_PROJECT

            // delay forever
            while(1)
                TaskDelayTicks(10);
        }
        
        // write our message to the file
        if(f_write(&File, Message, (UINT)strlen((const char*)Message), &BytesWritten) != FR_OK)
        {
            while(1)
                TaskDelayTicks(10);
        }
        
        if(BytesWritten != (UINT)strlen((const char*)Message))
        {
            while(1)
                TaskDelayTicks(10);
        }
    }
    
    // we should never get out here
    return 0;
}

/*
 * This TASK will interrupt the lower priority TASK every 2 OS ticks.
 * It will then write another message to its file.  After this is done
 * 100 times, a flag will get set and the test is over.
 */
UINT32 HighPriorityFileTaskCode(void *Args)
{
    UINT BytesWritten;
    FIL File;
    BYTE Message[] = {"THIS IS A HIGH PRIORITY TASK INTERRUPTING A LOWER PRIORITY TASK!\r\n"};
    UINT32 Count = 0;
    
    // open up our test file
    if(f_open(&File, "\\HighPriorityFile.txt", FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
        while(1);
    
    while(1)
    {
        // are we done yet?
        if(gComplete == TRUE)
        {
            // we're done, close the file and just delay
            f_close(&File);
            
            while(1)
                TaskDelayTicks(10);
        }
        
        // write our message to the file
        if(f_write(&File, Message, (UINT)strlen((const char*)Message), &BytesWritten) != FR_OK)
        {
            while(1)
                TaskDelayTicks(10);
        }
        
        // just do a small delay before interrupting the
        // lower priority TASK
        TaskDelayTicks(2);
        
        // if we got to 100 writes, set this flag so both
        // TASKs close their files.
        if(++Count == 100)
            gComplete = TRUE;
    }
    
    // we should never get out here
    return 0;
}

// This is just a support method for initialization of a directory
BOOL DirectoryExists(BYTE *DirectoryPath)
{
    DIR Directory;
    FRESULT FResult;
    
    // just open the file
    FResult = f_opendir(&Directory, (const TCHAR*)DirectoryPath);
    
    // now close it
    if(FResult == FR_OK)
        f_closedir(&Directory);
    
    // return the result
    return (BOOL)(FResult == FR_OK);
}

int main(int argc, char** argv)
{
    while(OSCCONbits.SLOCK != 1);

	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);
    
    SetSystemClocks(GetSystemClock());
    
    // InitOS() must be called BEFORE ANY OTHER OS CALL!!!!
    if(InitOS() != OS_SUCCESS)
        while(1);

    // mount the sd card file system
    if(f_mount(&gDefaultFat, "\\", 1) != FR_OK)
    {
        while(1);
    }
    
    // now make the user directory
    // that one of the file is stored in
    if(DirectoryExists("\\user") == FALSE)
    {
        if(f_mkdir("\\user") != FR_OK)
        {
            while(1);
        }
    }
        
    // Now we are going to create our TASKs
    if(CreateTask(LowPriorityFileTaskCode, 500, 1, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
        
    if(CreateTask(HighPriorityFileTaskCode, 500, 2, (void*)NULL, (TASK*)NULL) == (TASK*)NULL)
        while(1);
    
    // This starts the OS Scheduler and will begin executing the TASK with the highest priority.
    StartOSScheduler();
    
    // We should never get here!
	return (EXIT_SUCCESS);
}
