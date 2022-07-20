/*
    NexOS Kernel Version v1.01.03
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

#ifndef PORT_H
	#define PORT_H

#include <plib.h>

#include "GenericTypeDefs.h"
#include "HardwareProfile.h"
#include "CPUInfo.h"

#ifndef TASK_ENTRY_POINT_DATA_TYPE
	#define TASK_ENTRY_POINT_DATA_TYPE

	typedef UINT32 (*TASK_ENTRY_POINT)(void *);
#endif // end of #ifndef TASK_ENTRY_POINT_DATA_TYPE

#ifndef TASK_EXIT_POINT_DATA_TYPE
	#define TASK_EXIT_POINT_DATA_TYPE

	typedef void (*TASK_EXIT_CALLBACK)(void);
#endif // end of #ifndef TASK_EXIT_POINT_DATA_TYPE

#define OS_WORD														UINT32
#define OS_WORD_SIZE_IN_BYTES										4
    
// These are used in Memory.h for knowing if a variable is in a valid memory range.
#define USER_PROGRAM_SIZE_IN_BYTES                                  GetProgramMemorySizeInBytes()
#define USER_RAM_SIZE_IN_BYTES                                      GetRAMSizeInBytes()
    
void ClearSoftwareInterrupt(void);

/*
	void PortClearCoreInterruptFlag(void)

	Description: This method must clear the core interrupt flag.  This interrupt flag
    should only be set with the core interrupt which is used to return control
    back over to the scheduler from within a critical section.  This method is called
    every time the OS scheduler runs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- None

	See Also:
		- None
*/
#define PortClearCoreInterruptFlag()                                ClearSoftwareInterrupt()
 
/*
	void PortClearOSTickTimerInterruptFlag(void)

	Description: This method must clear the timer interrupt flag which is used to generate an
    OS tick.  This is only called when the interrupt is triggered.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- None

	See Also:
		- None
*/
#define PortClearOSTickTimerInterruptFlag()                         INTClearFlag(INT_CT)
    
/*
	BOOL PortGetOSTickTimerInterruptFlag(void)

	Description: This method returns the status of the timer interrupt flag used
    to generate an OS tick.  This method is called each time the OS scheduler runs.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        BOOL - TRUE if the timer flag is set, FALSE otherwise.

	Notes:
		- None

	See Also:
		- None
*/
#define PortGetOSTickTimerInterruptFlag()                           INTGetFlag(INT_CT)
    
/*
	void PortUpdateOSTimer(void)

	Description: This method should configure the timer used for the OS tick to
    fire another interrupt in 1 OS tick time from the time this method is called.
    This method is called each time after PortGetOSTickTimerInterruptFlag() returns 
    TRUE.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- None

	See Also:
		- None
*/
#define PortUpdateOSTimer()                             OpenCoreTimer(GetInstructionClock() / 2 / OS_TICK_RATE_IN_HZ)

/*
	void SurrenderCPU(void)

	Description: This method releases control of the CPU and returns it
    to the OS while also changing the current interrupt priority to
    OS_PRIORITY.  This allows control to be given to the OS even if in
    a critical section (which is a must).

	Blocking: Yes

	User Callable: Yes

	Arguments:
		None

	Returns: 
        None

	Notes:
		- This method must transfer control to the OS even in a critical section.

	See Also:
		- None
*/
void SurrenderCPU(void);

/*
	void PortStartOSScheduler(void)

	Description: This method configures and enables any interrupts which are needed
    for the OS scheduler to run.  It is called within StartOSScheduler() right
    before OS_StartFirstTask() is called.

	Blocking: No

	User Callable: No

	Arguments:
		None

	Returns: 
        None

	Notes:
		- None

	See Also:
		- StartOSScheduler()
*/
void PortStartOSScheduler(void);

/*
	OS_WORD *PortInitializeTaskStack(OS_WORD *Stack, UINT32 StackSizeInWords, TASK_ENTRY_POINT StartingAddress, void *Args)

	Description: This method will initialize the TASK stack passed in.  The initialization of the stack makes
    it so that the TASK can start execution at the address StartingAddress.  This is architecture specific and
    must be implemented for the type of CPU being used.

	Blocking: No

	User Callable: No

	Arguments:
		OS_WORD *Stack - A pointer to a location in RAM where the TASK's stack starts from.  This is the
        low end of the stack.  Meaning that Stack + StackSizeInWords = direction of growing stack in positive direction.
 
        UINT32 StackSizeInWords - The size in OS_WORD of the stack.
 
        TASK_ENTRY_POINT StartingAddress - The starting execution address of the TASK.  This is a 4 byte wide pointer.
        This must be embedded somewhere in the TASK stack.

        void *Args - Optional arguments which can be passed to the TASK.  These must be embedded somewhere in
        the TASK stack.

	Returns: 
        OS_WORD * - A valid pointer to the start of the TASK's stack.  If (OS_WORD*)NULL is returned the
        method failed to initialized the TASK stack.

	Notes:
		- This method must be implemented by the user depending upon which CPU architecture is used.

	See Also:
		- None
*/
OS_WORD *PortInitializeTaskStack(OS_WORD *Stack, UINT32 StackSizeInWords, TASK_ENTRY_POINT StartingAddress, void *Args);

/*
	OS_WORD *PortInitializeSystemStack(OS_WORD *Stack, UINT32 StackSizeInWords)

	Description: This method will initialize the system stack.

	Blocking: No

	User Callable: No

	Arguments:
		OS_WORD *Stack - A pointer to a location in RAM where the system stack starts from.  This is the
        low end of the stack.  Meaning that Stack + StackSizeInWords = direction of growing stack in positive direction.
 
        UINT32 StackSizeInWords - The size in OS_WORD of the stack.

	Returns: 
        OS_WORD * - A valid pointer to the start of the systems stack.  If (OS_WORD*)NULL is returned the
        method failed to initialized the stack.

	Notes:
		- This method must be implemented by the user depending upon which CPU architecture is used.
        - The system stack is used anytime an interrupt is processed.

	See Also:
		- None
*/
OS_WORD *PortInitializeSystemStack(OS_WORD *Stack, UINT32 StackSizeInWords);

/*
	UINT32 PortAnaylzeTaskStackUsage(OS_WORD *StartOfStack, UINT32 StackSizeInWords)

	Description: This method will analyze the stack passed in for usage.  At 
    creation a TASKs stack is filled with the value specified by 
    TASK_STACK_FILL_VALUE.  This method starts at the end of the stack and starts
    iterating to the beginning of the stack while looking for a value other than
    TASK_STACK_FILL_VALUE.  Once it finds a value other than TASK_STACK_FILL_VALUE
    it will compute in words how much of the stack is assumingly unused.

	Blocking: No

	User Callable: No

	Arguments:
		OS_WORD *StartOfStack - A pointer to a location in RAM where the stack starts from.  This is the
        low end of the stack.  Meaning that Stack + StackSizeInWords = direction of growing stack in positive direction.
 
        UINT32 StackSizeInWords - The size in OS_WORD of the stack.

	Returns: 
        UINT32 - The number of times in a row the value TASK_STACK_FILL_VALUE was found from the end of the
        stack going towards the beginning until another value is found.

	Notes:
		- This method must be implemented if ANALYZE_TASK_STACK_USAGE inside of RTOSConfig.h is a 1.
        - A unique value for TASK_STACK_FILL_VALUE inside of RTOSConfig.h should be chosen.

	See Also:
		- PortIsStackOverflowed()
*/
UINT32 PortAnaylzeTaskStackUsage(OS_WORD *StartOfStack, UINT32 StackSizeInWords);

/*
	BOOL PortIsStackOverflowed(OS_WORD *CurrentStackPointer, OS_WORD *StartOfStack, UINT32 StackSizeInWords)

	Description: This method will analyze the stack passed in to see if it has grown beyond its bounds.

	Blocking: No

	User Callable: No

	Arguments:
        OS_WORD * CurrentStackPointer - The location the stack is currently pointing to.

		OS_WORD *StartOfStack - A pointer to a location in RAM where the stack starts from.  This is the
        low end of the stack.  Meaning that Stack + StackSizeInWords = direction of growing stack in positive direction.
 
        UINT32 StackSizeInWords - The size in OS_WORD of the stack.

	Returns: 
        BOOL - TRUE if the CurrentStackPointer is beyond the bounds of the stack, FALSE otherwise.

	Notes:
		- This method must be implemented if USING_CHECK_TASK_STACK_FOR_OVERFLOW inside of RTOSConfig.h is a 1.
        - This method is called each time a TASK is swapped out for another TASK by the OS.
        - TaskStackOverflowUserCallback() in OS_Callback.c is called if PortIsStackOverflowed() returns TRUE.

	See Also:
		- PortAnaylzeTaskStackUsage()
*/
BOOL PortIsStackOverflowed(OS_WORD *CurrentStackPointer, OS_WORD *StartOfStack, UINT32 StackSizeInWords);

/*
	void PortSetInterruptPriority(BYTE NewInterruptPriority)

	Description: This method sets the current interrupt level to NewInterruptPriority.
    Any interrupt below level NewInterruptPriority should not interrupt the CPU.

	Blocking: No

	User Callable: No

	Arguments:
		BYTE NewInterruptPriority - The new CPU interrupt level which should be active.

	Returns: 
        None

	Notes:
		- None

	See Also:
		- None
*/
void PortSetInterruptPriority(BYTE NewInterruptPriority);

/*
	void PortStopOSTickTimer(void)

	Description: This method stops the hardware timer used by the OS for the 
    OS tick timer.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns: 
        None

	Notes:
		- This only needs to be defined if USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1
        - This method gets called before the device enters sleep if there are no 
          TASKs in the delayed queue.

	See Also:
		- PortStartOSTickTimer()
*/
#define PortStopOSTickTimer()                                       INTEnable(INT_CT, INT_DISABLED);

/*
	void PortStartOSTickTimer(void)

	Description: This method starts the hardware timer used by the OS for the 
    OS tick timer.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns: 
        None

	Notes:
		- This only needs to be defined if USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1
        - This method will get called after the device wakes up regardless of if PortStopOSTickTimer()
          was called or not.

	See Also:
		- PortStopOSTickTimer()
*/
void PortStartOSTickTimer(void);

/*
	void PortEnterSleepMode(void)

	Description: This method should put the device to sleep.  In sleep mode only
    an interrupt will wake the CPU up.

	Blocking: No

	User Callable: No

	Arguments:
        None

	Returns: 
        None

	Notes:
		- This only needs to be defined if USING_ENTER_DEVICE_SLEEP_MODE_METHOD == 1

	See Also:
		- None
*/
#define PortEnterSleepMode()                                    PowerSaveSleep()

#endif // end of #ifndef PORT_H
