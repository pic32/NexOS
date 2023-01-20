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

#ifndef RTOS_CONFIG_H
	#define RTOS_CONFIG_H

/*
 * This file configures the NexOS options and system operation.
 */

// OS Configurations
//----------------------------------------------------------------------------------------------------

// This is version v1.02.01 release of the RTOSConfig.h file.
#define RTOS_CONFIG_H_VERSION                                   0x00000009

// OS_PRIORITY is the priority of the OS compared to interrupts.  When an OS call is made the
// interrupt priority will be set to OS_PRIORITY so that interrupts of OS_PRIORITY level or lower
// do not interrupt the OS in a critical section.
#define OS_PRIORITY												1

// HIGHEST_USER_TASK_PRIORITY is the highest TASK priority the user will use.  This directly 
// dictates the size of the gCPUScheduler[].  The higher the TASK priority value the higher
// the priority.  A TASKs priority cannot exceed HIGHEST_USER_TASK_PRIORITY.
#define HIGHEST_USER_TASK_PRIORITY								3

// OS_TICK_RATE_IN_HZ is the speed of the OS Tick timer.  This value should be how many times a second 
// the main system timer interrupt will fire.
#define OS_TICK_RATE_IN_HZ										10000

// USING_OS_GENERAL_EXCEPTION_HANDLER will enable the use of the general exception handler.
// This is useful if you want to be able to delete or restart a TASK which causes a CPU
// exception.  A 1 enables this feature, and a 0 disables it.
#define USING_OS_GENERAL_EXCEPTION_HANDLER						0

// USING_OS_GENERAL_EXCEPTION_TO_STRING gives the ability for to stringing a CPU exception.
// A 1 enables this feature, and a 0 disables it.
#define USING_OS_GENERAL_EXCEPTION_TO_STRING                    0

// USER_PROGRAM_STARTING_ADDRESS is the starting address where valid instructions
// can exist for the CPU.  This is used for error checking data.
#define USER_PROGRAM_STARTING_ADDRESS							0x9D000000

// USER_RAM_STARTING_ADDRESS is the starting address where valid data
// can exist for the CPU.  This is used for error checking data.
#define USER_RAM_STARTING_ADDRESS								0xA0000000

// SYSTEM_STACK_SIZE_IN_WORDS is the size in words of the system stack.
// The system stack is used to process interrupts so that the stack size
// burden can be taken off of each TASK.
#define SYSTEM_STACK_SIZE_IN_WORDS                              256

// MINIMUM_STACK_SIZE_IN_BYTES is the number of bytes large a TASK
// stack must be when created.
#define MINIMUM_STACK_SIZE_IN_BYTES							    200

// USING_TICKS_TO_MILLISECONDS_METHOD must be defined as a 1 to enable the ticks
// to milliseconds conversion method.  A 1 enables this feature, and a 0 disables it.
#define USING_TICKS_TO_MILLISECONDS_METHOD                      0

// USING_MILLISECONDS_TO_TICKS_METHOD must be defined as a 1 to enable the milliseconds
// to ticks conversion method.  A 1 enables this feature, and a 0 disables it.
#define USING_MILLISECONDS_TO_TICKS_METHOD                      0

// USING_TICKS_TO_MICROSECONDS_METHOD must be defined as a 1 to enable the ticks
// to microseconds conversion method.  A 1 enables this feature, and a 0 disables it.
#define USING_TICKS_TO_MICROSECONDS_METHOD                      0

// USING_MICROSECONDS_TO_TICKS_METHOD must be defined as a 1 to enable the microseconds
// to ticks conversion method.  A 1 enables this feature, and a 0 disables it.
#define USING_MICROSECONDS_TO_TICKS_METHOD                      0

// USING_GET_OS_TICK_COUNT_METHOD must be defined as a 1 to enable the OS Tick count method
// This method returns how many OS ticks have elapsed since the system started.  This is a
// 32-bit wide number that will rollover from 0xFFFFFFFF to 0x00000000.
#define USING_GET_OS_TICK_COUNT_METHOD                          0

// USING_GET_OS_TICK_COUNT_FROM_ISR_METHOD must be defined as a 1 to get the OS
// Tick count from an ISR method.
#define USING_GET_OS_TICK_COUNT_FROM_ISR_METHOD                 0

// USING_ENTER_DEVICE_SLEEP_MODE_METHOD must be defined as a 1 to use the sleep function.
#define USING_ENTER_DEVICE_SLEEP_MODE_METHOD                    0

// USING_GET_CURRENT_CRITICAL_COUNT_METHOD must be defined as a 1 to enable getting the 
// critical count.  The critical count is incremented by 1 each time the method EnterCritical()
// is called.  The critical count will decrement by 1 each time ExitCritical() is called.
#define USING_GET_CURRENT_CRITICAL_COUNT_METHOD                 0

// USING_EXIT_CRITICAL_UNCONDITIONAL_METHOD must be defined as a 1 to enable the 
// ExitCriticalUnconditional() method.  Regardless of the critical count, this method
// will set it back to zero and enable interrupts.
#define USING_EXIT_CRITICAL_UNCONDITIONAL_METHOD                0

// USING_KERNEL_VERSION_TO_STRING must be defined as a 1 to use the KernelVersionToString() method.
#define USING_KERNEL_VERSION_TO_STRING                          0

// USING_ANALYZE_SYSTEM_STACK_METHOD will allow the user to see how many bytes
// have been unused in the system stack
#define USING_ANALYZE_SYSTEM_STACK_METHOD                       0

// USING_TASK_RUNTIME_EXECUTION_COUNTER will keep track of how much time
// each TASK spends executing.
#define USING_TASK_RUNTIME_EXECUTION_COUNTER                    0

// USING_TASK_RUNTIME_EXECUTION_TO_STRING_METHOD will enable the method
// which to strings all the TASKs that have run, and their total execution time.
#define USING_TASK_RUNTIME_EXECUTION_TO_STRING_METHOD           0

// USING_TASK_RUNTIME_HISTORY will enable keeping track of the last
// TASK_RUNTIME_HISTORY_SIZE_IN_TASKS TASKs that have executed
#define USING_TASK_RUNTIME_HISTORY                              0

// TASK_RUNTIME_HISTORY_SIZE_IN_TASKS is the number of TASKs that the
// USING_TASK_RUNTIME_HISTORY will keep track of
#define TASK_RUNTIME_HISTORY_SIZE_IN_TASKS                      10
//----------------------------------------------------------------------------------------------------


// Memory and Heap Configurations
//----------------------------------------------------------------------------------------------------

// USING_CALLOC_MEMORY_METHOD must be defined as a 1 to use the CallocMemory() method.
#define USING_CALLOC_MEMORY_METHOD								0

// USING_REALLOC_MEMORY_METHOD must be defined as a 1 to use the ReallocMemory() method.
#define USING_REALLOC_MEMORY_METHOD								0

// USING_GET_HEAP_USED_IN_BYTES_METHOD must be defined as a 1 to use the GetHeapUsedInBytes() method.
#define USING_GET_HEAP_USED_IN_BYTES_METHOD                     0

// USING_GET_HEAP_REMAINING_IN_BYTES_METHOD must be defined as a 1 to use the GetHeapRemainingInBytes() method.
#define USING_GET_HEAP_REMAINING_IN_BYTES_METHOD                0

// MEMORY_WARNING_LEVEL_IN_BYTES when MEMORY_WARNING_LEVEL_IN_BYTES are used the
// USING_MEMORY_WARNING_EVENT is raised if enabled and the USING_MEMORY_WARNING_USER_CALLBACK
// is called if enabled.
#define MEMORY_WARNING_LEVEL_IN_BYTES							45000

// OS_HEAP_SIZE_IN_BYTES is the size of the OS heap in bytes.  Calls to AllocateMemory() or
// CallocMemory() use the memory from the OS heap.
#define OS_HEAP_SIZE_IN_BYTES                                   50000

// OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES is the size of the memory header in the heap.
// This allows the system to know how big of a jump to take to get to the data of an
// allocated section in the heap.  The memory header always has the first bit to denote
// if it is free or not, and the remaining bits denote how large the section is.  Until
// further notice this should always be 4.
#define OS_MEMORY_BLOCK_HEADER_SIZE_IN_BYTES   					4

// CLEAR_HEAP_AT_START_UP if set to 1 will initialize the OS heap with the value located
// inside of HEAP_INITIALIZATION_VALUE.
#define CLEAR_HEAP_AT_START_UP									1

// HEAP_INITIALIZATION_VALUE is the value the OS will put in the OS heap at startup.
// CLEAR_HEAP_AT_START_UP must be set to a 1 for this to have any meaning.  At startup
// the OS will call the following function to set the values in the heap:
// memset(&gKernelManagedMemory[1], HEAP_INITIALIZATION_VALUE, (OS_HEAP_SIZE_IN_BYTES / OS_WORD_SIZE_IN_BYTES));
#define HEAP_INITIALIZATION_VALUE								(OS_WORD)0
//----------------------------------------------------------------------------------------------------


// Idle Task Configurations
//----------------------------------------------------------------------------------------------------

// This is the stack size in words that the Idle Task will be assigned.
// Tests have shown that 59 words are used by the TASK.
#define IDLE_TASK_STACK_SIZE_IN_BYTES                           384

// This is the Idle Task priority and until further notice should always be set to zero.
// The OS scheduler is designed around the fact that at least 1 task is in the READY
// state.  That 1 guaranteed TASK is the Idle Task.
#define IDLE_TASK_PRIORITY										0

// This is the name used by the Idle Task should USING_TASK_NAMES be defined as 1
#define IDLE_TASK_TASK_NAME										(BYTE*)"IdleTask"

// These are the arguments passed to the Idle Task at startup.
#define IDLE_TASK_ARGS											(void*)NULL

// IDLE_TASK_PERFORM_DELETE_TASK if set to 1 will perform the clean up required
// when a TASK is deleted.  This is less deterministic than if the Maintenance Task
// performs the clean up.
#define IDLE_TASK_PERFORM_DELETE_TASK                           0
//----------------------------------------------------------------------------------------------------


// Maintenance Task Configurations
//----------------------------------------------------------------------------------------------------

// This is the stack size in words that the Maintenance Task will be assigned.
// Test have shown 48 words are used.  This seems too low and will be evaluated
// further in more depth.
#define MAINTENANCE_TASK_STACK_SIZE_IN_BYTES                    256

// This is the priority of the Maintenance Task.  It can be any valid priority.
#define MAINTENANCE_TASK_PRIORITY 								HIGHEST_USER_TASK_PRIORITY

// This is the name used by the Maintenance Task should USING_TASK_NAMES be defined as 1
#define MAINTENANCE_TASK_TASK_NAME								(BYTE*)"MaintTask"

// These are the arguments passed to the Maintenance Task at startup.
#define MAINTENANCE_TASK_ARGS									(void*)NULL

// This is how many OS Ticks the Maintenance Task will delay for, before it 
// runs again to see if any TASKS need deleting or restarting.
#define MAINTENANCE_TASK_DELAY_TICKS                            200
//----------------------------------------------------------------------------------------------------


// IO Buffer Task Configurations
//----------------------------------------------------------------------------------------------------

// This is the stack size in words that the Buffer Task will be assigned.
// Test have shown that 61 words at least are needed.
#define IO_BUFFER_TASK_STACK_SIZE_IN_BYTES                      384

// This is the priority of the Buffer Task.  It can be any valid priority.
#define IO_BUFFER_TASK_PRIORITY                                 HIGHEST_USER_TASK_PRIORITY

// This is the name used by the Buffer Task should USING_TASK_NAMES be defined as 1
#define IO_BUFFER_TASK_TASK_NAME                                (BYTE*)"IOBuffTask"

// These are the arguments passed to the IO Buffer Task at startup.
#define IO_BUFFER_TASK_ARGS                                     (void*)NULL

// This is how many OS Ticks the Buffer Task will delay for, before it 
// runs again to see if any data can be read or written in from or out to a peripheral port.
#define IO_BUFFER_TASK_PERIODICITY_IN_OS_TICKS                  100
//----------------------------------------------------------------------------------------------------


// Task Configurations
//----------------------------------------------------------------------------------------------------

// USING_CHECK_TASK_PARAMETERS if set to 1 will enable the code which
// checks the parameters of data coming into any method in Task.c.  This
// is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_TASK_PARAMETERS                             0

// USING_TASK_CHECK_IN if set to 1 will allow a TASK to register for
// task check in.  WHen a TASK registers for check in it lets the OS
// know how often it needs to check in.  If the TASK does not check in
// at the alloted time the OS will either delete or restart the TASK.
#define USING_TASK_CHECK_IN										0

// USING_DELETE_TASK if set to 1 allows the user to delete a TASK.
#define USING_DELETE_TASK										0

// USING_SUSPEND_TASK_METHOD if set to 1 allows the user to suspend a TASK.
// If a TASK is suspended it is removed from the READY queue and will not execute.
// To get the TASK into the READY queue ResumeTask() must be called.  Only a READY
// TASK can be suspended.
#define USING_SUSPEND_TASK_METHOD								0

// USING_SUSPEND_ALL_TASKS_METHOD if set to 1 allows the user to suspend all TASKs
// in the READY queue.
#define USING_SUSPEND_ALL_TASKS_METHOD                          0

// USING_RESUME_TASK_METHOD if set to 1 allows the user to resume a TASK which
// was suspended.
#define USING_RESUME_TASK_METHOD								0

// USING_RESUME_ALL_TASKS_METHOD if set to 1 allows the user to resume all TASKs
// which were suspended.
#define USING_RESUME_ALL_TASKS_METHOD                           0

// USING_RESTART_TASK if set to a 1 allows the user to restart a TASK.
#define USING_RESTART_TASK										0

// USING_GET_TASK_PRIORITY_METHOD if set to a 1 allows the user to get
// the priority of a TASK.
#define USING_GET_TASK_PRIORITY_METHOD							0

// USING_CHANGE_TASK_PRIORITY_METHOD if set to a 1 allows the user to
// set a new priority for a TASK.  This is rather dicey, and may be 
// depreciated down the road.
#define USING_CHANGE_TASK_PRIORITY_METHOD                       0

// USING_TASK_DELAY_TICKS_METHOD if set to a 1 allows the user to
// exercise the ability to wait for specified periods of time.  This
// includes waiting on a resource or communication aside from just 
// a strict delay.
#define USING_TASK_DELAY_TICKS_METHOD                           1

// USING_TASK_HIBERNATION if set to a 1 allows the user to put TASKs
// into hibernation.  This is just another term for suspend.
#define USING_TASK_HIBERNATION									0

// USING_TASK_SIGNAL if set to a 1 will allow the TASK signal feature.
#define USING_TASK_SIGNAL										0

// USING_TASK_GET_STATE_METHOD if set to 1 will allow the user to 
// find out the state of a given TASK.
#define USING_TASK_GET_STATE_METHOD								0

// USING_TASK_EXIT_METHOD_CALLBACK if set to 1 allows the user to
// specify a method that a TASK will execute if deleted.
#define USING_TASK_EXIT_METHOD_CALLBACK                         0

// USING_TASK_NAMES if set to 1 allows the user to specify a TASK
// name at TASK creation time.  This is useful for debugging.
#define USING_TASK_NAMES										0

// This is the buffer size of the TASK name buffer.  Another byte is
// always added to this value for the NULL character.  This value has no
// meaning if USING_TASK_NAMES is set to zero.
#define TASK_NAME_LENGTH_IN_BYTES								16

// USING_TASK_UNIQUE_ID if set to a 1 assigns each TASK a unique
// ID at creation time.
#define USING_TASK_UNIQUE_ID                                    0

// USING_TASK_LOCAL_STORAGE_ACCESS if set to a 1 allows TASKs the ability
// for other TASKs to interact with its own local variables which are
// registered.
#define USING_TASK_LOCAL_STORAGE_ACCESS                         0

// NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS determines how many
// items can be used for the local thread storage capability.  This
// has no meaning if USING_TASK_LOCAL_STORAGE_ACCESS is set to zero.
#define NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS                 0

// USING_GET_NUMBER_OF_TASKS_METHOD if set to a 1 allows the user 
// to find out how many TASKs are present in the OS.  This includes
// TASKs of all states.
#define USING_GET_NUMBER_OF_TASKS_METHOD                        0

// This allows the OS to check if a TASK stack was in an overflow condition upon 
// performing a context switch.
#define USING_CHECK_TASK_STACK_FOR_OVERFLOW                     0

// This allows the OS to analyze the stack usage of a task.  At
// TASK creation time TASK_STACK_FILL_VALUE will be put into the
// entire stack area of the TASK.  Then the user can at any time
// get the number of bytes free in the TASK stack area of a TASK.
#define ANALYZE_TASK_STACK_USAGE                                0

// This is the value to fill the TASK stack with if ANALYZE_TASK_STACK_USAGE
// is set to a 1.  This is done at TASK creation time.
#define TASK_STACK_FILL_VALUE                                   0xDEADBEEF
//----------------------------------------------------------------------------------------------------


// Binary Semaphore Configurations
//----------------------------------------------------------------------------------------------------
// NOTE: OS Binary Semaphores must be included to use a Binary Semaphore.

// USING_BINARY_SEMAPHORES must be set to 1 in order to use any BINARY_SEMAPHORE
// related functionality.
#define USING_BINARY_SEMAPHORES									1

// USING_CHECK_BINARY_SEMAPHORE_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in BinarySemaphore.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_BINARY_SEMAPHORE_PARAMETERS                 0

// USING_BINARY_SEMAPHORE_STARVATION_PROTECTION if set to a 1 will prevent a lower
// priority TASK from never getting a BINARY_SEMAPHORE because higher priority TASKs
// are in the blocked list with it.  Each time a TASK is passed over while in the 
// blocked list, its priority gets incremented by 1.  Once a TASK obtains the 
// BINARY_SEMAPHORE its priority will return to what it was when it first entered
// the blocked list of the BINARY_SEMAPHORE.
#define USING_BINARY_SEMAPHORE_STARVATION_PROTECTION            0

// USING_DELETE_BINARY_SEMAPHORE_METHOD if set to a 1 will allow the user to 
// delete a BINARY_SEMAPHORE which was created in the OS heap.
#define USING_DELETE_BINARY_SEMAPHORE_METHOD					1

// USING_GET_BINARY_SEMAPHORE_OWNER_METHOD if set to a 1 will allow the user
// to call a method which returns a TASK pointer to the current owner of
// a BINARY_SEMAPHORE.
#define USING_GET_BINARY_SEMAPHORE_OWNER_METHOD					0

// USING_GET_BINARY_SEMAPHORE_TAKEN_METHOD if set to a 1 will allow the user
// to call a method to find out if a BINARY_SEMAPHORE is current owned by
// another TASK.
#define USING_GET_BINARY_SEMAPHORE_TAKEN_METHOD					0
//----------------------------------------------------------------------------------------------------


// Mutex Configruations
//----------------------------------------------------------------------------------------------------
// NOTE: OS Binary Semaphores must be included to use a Mutex.

// USING_MUTEXES must be set to 1 in order to use any MUTEX
// related functionality.
#define USING_MUTEXES											1

// USING_CHECK_MUTEX_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in Mutex.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_MUTEX_PARAMETERS                            0

// USING_DELETE_MUTEX_METHOD if set to a 1 will allow the user to delete a MUTEX
// which was created in the OS heap.
#define USING_DELETE_MUTEX_METHOD								1

// USING_GET_MUTEX_OWNER_METHOD if set to a 1 will allow the user to get the owner
// of a MUTEX.  The owner comes in the form of a pointer to a TASK.
#define USING_GET_MUTEX_OWNER_METHOD							1

// USING_GET_MUTEX_TAKEN_METHOD if set to a 1 will allow the user
// to call a method to find out if a MUTEX is current owned by another TASK.
#define USING_GET_MUTEX_TAKEN_METHOD							0
//----------------------------------------------------------------------------------------------------


// Counting Semaphore Configurations
//----------------------------------------------------------------------------------------------------
// NOTE: Binary Semaphores must be included to use a Counting Semaphore.

// USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in CountingSemaphore.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_COUNTING_SEMAPHORE_PARAMETERS               0

// USING_RELEASE_UNCONDITIONALLY_COUNTING_SEMAPHORE_METHOD if set to a 1 will allow
// the user to call a method which will give up the COUNTING_SEMAPHORE regardless of
// the count in the COUNTING_SEMAPHORE.
#define USING_RELEASE_UNCONDITIONALLY_COUNTING_SEMAPHORE_METHOD	0

// USING_DELETE_COUNTING_SEMAPHORE_METHOD if set to a 1 will allow the user to delete a 
// COUNTING_SEMAPHORE which was created in the OS heap.
#define USING_DELETE_COUNTING_SEMAPHORE_METHOD					0

// USING_GET_COUNTING_SEMAPHORE_OWNER_METHOD if set to a 1 will allow the user to get the owner
// of a COUNTING_SEMAPHORE.  The owner comes in the form of a pointer to a TASK.
#define USING_GET_COUNTING_SEMAPHORE_OWNER_METHOD				0

// USING_GET_COUNTING_SEMAPHORE_TAKEN_METHOD if set to a 1 will allow the user
// to call a method to find out if a COUNTING_SEMAPHORE is current owned by another TASK. 
#define USING_GET_COUNTING_SEMAPHORE_TAKEN_METHOD				0

// USING_GET_COUNTING_SEMAPHORE_COUNT_METHOD if set to a 1 will allow the user to 
// get the current count in the COUNTING_SEMAPHORE.
#define USING_GET_COUNTING_SEMAPHORE_COUNT_METHOD				0
//----------------------------------------------------------------------------------------------------


// Software Timer Configurations
//----------------------------------------------------------------------------------------------------

// USING_SOFTWARE_TIMERS must be set to 1 in order to use any
// SOFTWARE_TIMER related functionality.
#define USING_SOFTWARE_TIMERS									0

// USING_CHECK_SOFTWARE_TIMER_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in SoftwareTimer.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_SOFTWARE_TIMER_PARAMETERS                   0

// USING_SOFTWARE_TIMER_ENABLE_FROM_ISR_METHOD if set to a 0 will allow the user to
// start and stop a SOFTWARE_TIMER from an ISR.
#define USING_SOFTWARE_TIMER_ENABLE_FROM_ISR_METHOD             0

// USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD if set to a 1 will allow the user to
// reset a SOFTWARE_TIMER from an ISR.
#define USING_SOFTWARE_TIMER_RESET_FROM_ISR_METHOD              0

// USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD if set to a 1 will allow the user
// to restart a SOFTWARE_TIMER from an ISR.  This clears the OS tick count and keeps
// the SOFTWARE_TIMER running.
#define USING_SOFTWARE_TIMER_RESTART_FROM_ISR_METHOD            0

// USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD if set to a 1 will allow the user to
// clear the current count of OS ticks in the SOFTWARE_TIMER from an ISR.
#define USING_SOFTWARE_TIMER_CLEAR_FROM_ISR_METHOD              0

// USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD if set to a 1 will allow the user to
// get the current number of OS Ticks the SOFTWARE_TIMER has accrued from an ISR.
#define USING_SOFTWARE_TIMER_GET_TICKS_FROM_ISR_METHOD          0

// USING_SOFTWARE_TIMER_GET_MILLISECONDS_FROM_ISR_METHOD if set to a 1 will allow the user
// to get the current time in milliseconds the SOFTWARE_TIMER has been on from an ISR.
#define USING_SOFTWARE_TIMER_GET_MILLISECONDS_FROM_ISR_METHOD   0

// USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD if set to a 1 will allow the user to see
// if a SOFTWARE_TIMER is currently running.  This method can be called from an ISR.
#define USING_SOFTWARE_TIMER_IS_RUNNING_FROM_ISR_METHOD         0

// USING_SOFTWARE_TIMER_DELETE_METHOD if set to a 1 will allow the user to delete a 
// SOFTWARE_TIMER which was created in the OS heap.
#define USING_SOFTWARE_TIMER_DELETE_METHOD						0

// USING_SOFTWARE_TIMER_IS_RUNNING_METHOD if set to a 1 will allow the user to see
// if a SOFTWARE_TIMER is currently running.
#define USING_SOFTWARE_TIMER_IS_RUNNING_METHOD					0

// USING_SOFTWARE_TIMER_GET_MILLISECONDS_METHOD if set to a 1 will allow the user
// to get the current time in milliseconds the SOFTWARE_TIMER has been on.
#define USING_SOFTWARE_TIMER_GET_MILLISECONDS_METHOD			0

// USING_SOFTWARE_TIMER_GET_TICKS_METHOD if set to a 1 will allow the user to
// get the current number of OS Ticks the SOFTWARE_TIMER has accrued.
#define USING_SOFTWARE_TIMER_GET_TICKS_METHOD					0

// USING_SOFTWARE_TIMER_CLEAR_METHOD if set to a 1 will allow the user to
// clear the current count of OS ticks in the SOFTWARE_TIMER.
#define USING_SOFTWARE_TIMER_CLEAR_METHOD						0

// USING_SOFTWARE_TIMER_RESTART_METHOD if set to a 1 will allow the user
// to restart a SOFTWARE_TIMER.  This clears the OS tick count and keeps
// the SOFTWARE_TIMER running.
#define USING_SOFTWARE_TIMER_RESTART_METHOD						0

// USING_SOFTWARE_TIMER_RESET_METHOD if set to a 1 will allow the user to
// stop a SOFTWARE_TIMER and clear its current count of OS ticks.
#define USING_SOFTWARE_TIMER_RESET_METHOD						0

// USING_SOFTWARE_TIMER_ENABLE_METHOD if set to a 1 will allow the user to
// start and stop a SOFTWARE_TIMER.
#define USING_SOFTWARE_TIMER_ENABLE_METHOD						0
//----------------------------------------------------------------------------------------------------


// Callback Timer Configurations
//----------------------------------------------------------------------------------------------------
// NOTE: Software Timers must be included to use a Callback Timer

// USING_CALLBACK_TIMERS must be set to 1 in order to use any
// CALLBACK_TIMER related functionality.
#define USING_CALLBACK_TIMERS											0

// USING_CHECK_CALLBACK_TIMER_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in CallbackTimer.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_CALLBACK_TIMER_PARAMETERS                   		0

// USING_CALLBACK_TIMER_DELETE_METHOD if set to a 1 will allow the user to delete a 
// CALLBACK_TIMER which was created in the OS heap.
#define USING_CALLBACK_TIMER_DELETE_METHOD								0

// USING_CALLBACK_TIMER_SET_PERIODICITY_METHOD if set to a 1 will allow the user to
// change the periodicity of the callback method in the CALLBACK_TIMER.
#define USING_CALLBACK_TIMER_SET_PERIODICITY_METHOD						0

// USING_CALLBACK_TIMER_SET_CALLBACK_METHOD if set to a 1 will allow the user to
// change the callback method of a CALLBACK_TIMER.
#define USING_CALLBACK_TIMER_SET_CALLBACK_METHOD                		0

// USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_METHOD if set to a 1 will allow the user
// to get the current periodicity of the callback method in the CALLBACK_TIMER.
#define USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_METHOD			0

// USING_CALLBACK_TIMER_GET_TICKS_REMAINING_METHOD if set to a 1 will allow the user
// to get how many OS ticks remain until the callback method in the CALLBACK_TIMER
// is executed.
#define USING_CALLBACK_TIMER_GET_TICKS_REMAINING_METHOD					0

// USING_CALLBACK_TIMER_RESTART_METHOD if set to a 1 will allow the user to 
// reset the current OS tick count in CALLBACK_TIMER to a zero.  The CALLBACK_TIMER
// will also be enabled if disabled.
#define USING_CALLBACK_TIMER_RESTART_METHOD								0

// USING_CALLBACK_TIMER_RESET_METHOD if set to a 1 will allow the user to 
// reset the current OS tick count in CALLBACK_TIMER to a zero and disables
// the CALLBACK_TIMER.
#define USING_CALLBACK_TIMER_RESET_METHOD								0

// USING_CALLBACK_TIMER_ENABLE_METHOD if set to a 1 will allow the user to 
// enable and disable a CALLBACK_TIMER.
#define USING_CALLBACK_TIMER_ENABLE_METHOD								0

// USING_CALLBACK_TIMER_IS_RUNNING_METHOD if set to a 1 will allow the user to
// know if a CALLBACK_TIMER is currently running (enabled).
#define USING_CALLBACK_TIMER_IS_RUNNING_METHOD							0

// USING_CALLBACK_TIMER_RESET_FROM_ISR_METHOD if set to a 1 will allow the user
// to reset a CALLBACK_TIMER from an ISR or critical section.
#define USING_CALLBACK_TIMER_RESET_FROM_ISR_METHOD              		0

// USING_CALLBACK_TIMER_ENABLE_FROM_ISR_METHOD if set to a 1 will allow the user
// to enable or disable a CALLBACK_TIMER from an ISR or critical section.
#define USING_CALLBACK_TIMER_ENABLE_FROM_ISR_METHOD                     0

// USING_CALLBACK_TIMER_RESTART_FROM_ISR_METHOD if set to a 1 will allow the user
// to restart a CALLBACK_TIMER from an ISR or critical section.
#define USING_CALLBACK_TIMER_RESTART_FROM_ISR_METHOD                    0

// USING_CALLBACK_TIMER_GET_TICKS_REMAINING_FROM_ISR_METHOD if set to a 1 will 
// allow the user to get the number of ticks remaining in a CALLBACK_TIMER 
// until the callback method is called from an ISR or critical section.
#define USING_CALLBACK_TIMER_GET_TICKS_REMAINING_FROM_ISR_METHOD        0

// USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_FROM_ISR_METHOD if set to a 1
// will allow the user to get the periodicity in ticks of the CALLBACK_TIMER
// from an ISR or critical section
#define USING_CALLBACK_TIMER_GET_PERIODICITY_IN_TICKS_FROM_ISR_METHOD   0

// USING_CALLBACK_TIMER_SET_PERIODICITY_FROM_ISR_METHOD if set to a 1 will allow
// the user to set the periodicity of the CALLBACK_TIMER from an ISR or critical
// section.
#define USING_CALLBACK_TIMER_SET_PERIODICITY_FROM_ISR_METHOD            0

// USING_CALLBACK_TIMER_SET_CALLBACK_FROM_ISR_METHOD if set to a 1 will allow
// the user to set the callback to a CALLBACK_TIMER from an ISR or critical section.
#define USING_CALLBACK_TIMER_SET_CALLBACK_FROM_ISR_METHOD               0

// USING_CALLBACK_TIMER_IS_RUNNING_FROM_ISR_METHOD if set to a 1 will allow the
// user to see if a CALLBACK_TIMER is running from an ISR or critical section.
#define USING_CALLBACK_TIMER_IS_RUNNING_FROM_ISR_METHOD                 0
//----------------------------------------------------------------------------------------------------


// Event Timer Configurations
//----------------------------------------------------------------------------------------------------
// NOTE: Software Timers must be included to use EVENT_TIMERs
// NOTE: USING_EVENTS must be defined as a 1 to use EVENT_TIMERs

// USING_EVENT_TIMERS must be set to 1 in order to use any
// EVENT_TIMER related functionality.
#define USING_EVENT_TIMERS										0

// USING_CHECK_EVENT_TIMER_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in EventTimer.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_EVENT_TIMER_PARAMETERS                      0

// USING_EVENT_TIMER_SET_ENABLE_METHOD if set to a 1 will allow the user to
// enable/disable an EVENT_TIMER after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_ENABLE_METHOD						0

// USING_EVENT_TIMER_SET_START_EVENT_METHOD if set to a 1 will allow the user
// to change the start EVENT after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_START_EVENT_METHOD				0

// USING_EVENT_TIMER_SET_STOP_EVENT_METHOD if set to a 1 will allow the user
// to change the stop EVENT after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_STOP_EVENT_METHOD					0

// USING_EVENT_TIMER_SET_START_EVENT_CALLBACK_METHOD if set to a 1 will allow the user
// to change the start EVENT after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_START_EVENT_CALLBACK_METHOD		0

// USING_EVENT_TIMER_SET_STOP_EVENT_CALLBACK_METHOD if set to a 1 will allow the user
// change the stop EVENT after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_STOP_EVENT_CALLBACK_METHOD		0

// USING_EVENT_TIMER_SET_EVENT_TIMER_START_POLICY_METHOD if set to a 1 will allow the user
// to change the EVENT_TIMER_START_POLICY after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_EVENT_TIMER_START_POLICY_METHOD	0

// USING_EVENT_TIMER_SET_EVENT_TIMER_STOP_POLICY_METHOD if set to a 1 will allow the user
// to change the EVENT_TIMER_STOP_POLICY after EVENT_TIMER creation.
#define USING_EVENT_TIMER_SET_EVENT_TIMER_STOP_POLICY_METHOD	0

// USING_EVENT_TIMER_CLEAR_TIMER_METHOD if set to a 1 will allow the user to
// clear the number of ticks that have accumulated in the EVENT_TIMER.
#define USING_EVENT_TIMER_CLEAR_TIMER_METHOD					0

// USING_EVENT_TIMER_GET_ENABLE_METHOD if set to a 1 will allow the user to
// find out if an EVENT_TIMER is enabled or not.
#define USING_EVENT_TIMER_GET_ENABLE_METHOD						0

// USING_EVENT_TIMER_GET_START_EVENT_METHOD if set to a 1 will allow the user
// to get the start EVENT of an EVENT_TIMER.
#define USING_EVENT_TIMER_GET_START_EVENT_METHOD				0

// USING_EVENT_TIMER_GET_STOP_EVENT_METHOD if set to a 1 will allow the user
// to get the stop EVENT of an EVENT_TIMER.
#define USING_EVENT_TIMER_GET_STOP_EVENT_METHOD					0

// USING_EVENT_TIMER_GET_START_EVENT_CALLBACK_METHOD if set to a 1 will allow the user
// to get the callback associated with the start EVENT.
#define USING_EVENT_TIMER_GET_START_EVENT_CALLBACK_METHOD		0

// USING_EVENT_TIMER_GET_STOP_EVENT_CALLBACK_METHOD if set to a 1 will allow the user
// to get the callback associated with the stop EVENT.
#define USING_EVENT_TIMER_GET_STOP_EVENT_CALLBACK_METHOD		0

// USING_EVENT_TIMER_GET_TIMER_START_POLICY_METHOD if set to a 1 will allow the user
// to get the EVENT_TIMER EVENT_TIMER_START_POLICY.
#define USING_EVENT_TIMER_GET_TIMER_START_POLICY_METHOD			0

// USING_EVENT_TIMER_GET_TIMER_STOP_POLICY_METHOD if set to a 1 will allow the user
// to get the EVENT_TIMER EVENT_TIMER_STOP_POLICY.
#define USING_EVENT_TIMER_GET_TIMER_STOP_POLICY_METHOD			0

// USING_EVENT_TIMER_GET_TIMER_TICKS_METHOD if set to a 1 will allow the user to
// get how many ticks have accumulated in the EVENT_TIMER.
#define USING_EVENT_TIMER_GET_TIMER_TICKS_METHOD				0

// USING_EVENT_TIMER_GET_TIMER_MILLISECONDS_METHOD if set to a 1 will allow the user
// get how many milliseconds the EVENT_TIMER has been running for.
#define USING_EVENT_TIMER_GET_TIMER_MILLISECONDS_METHOD			0

// USING_EVENT_TIMER_GET_TIMER_IS_RUNNING_METHOD if set to a 1 will allow the user
// to find out if the EVENT_TIMER is running or not.
#define USING_EVENT_TIMER_GET_TIMER_IS_RUNNING_METHOD			0

// USING_EVENT_TIMER_DELETE_METHOD if set to a 1 will allow the user to delete an 
// EVENT_TIMER which was created in the OS heap.
#define USING_EVENT_TIMER_DELETE_METHOD							0

// USING_EVENT_TIMER_START_CALLBACK_ACTION_METHOD if set to a 1 will allow the user
// to determine when to start an EVENT_TIMER if multiple start EVENTs can occur.
#define USING_EVENT_TIMER_START_CALLBACK_ACTION_METHOD          0

// USING_EVENT_TIMER_STOP_CALLBACK_ACTION_METHOD if set to a 1 will allow the user
// to set the state of an EVENT_TIMER once it has encountered a valid stop EVENT.
#define USING_EVENT_TIMER_STOP_CALLBACK_ACTION_METHOD			0

// USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER if set to a 1 will allow the user
// to utilize the start EVENT callback feature.
#define USING_EVENT_TIMER_START_EVENT_CALLBACK_MEMBER           0

// USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER if set to a 1 will allow the user
// to utilize the stop EVENT callback feature.
#define USING_EVENT_TIMER_STOP_EVENT_CALLBACK_MEMBER            0
//----------------------------------------------------------------------------------------------------


// Message Queue Configurations
//----------------------------------------------------------------------------------------------------

// USING_CHECK_MESSAGE_QUEUE_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in MessageQueue.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_MESSAGE_QUEUE_PARAMETERS                    0

// USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA if set to a 1 will allow the user to
// pass in a simple UINT32 to each message added to the queue.
#define USING_MESSAGE_QUEUE_MESSAGE_CODE_DATA                   0

// USING_MESSAGE_QUEUE_ADDITIONAL_DATA if set to a 1 will allow the user to assign
// a void pointer a value when adding new data to a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_ADDITIONAL_DATA						0

// USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD if set to a 1 will allow the user
// to free data from a MESSAGE_QUEUE while in an ISR.
#define USING_MESSAGE_QUEUE_FREE_DATA_FROM_ISR_METHOD			0

// USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD if set to a 1 will allow the user
// to get the size of a MESSAGE_QUEUE from an ISR.
#define USING_MESSAGE_QUEUE_GET_SIZE_FROM_ISR_METHOD			0

// USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD if set to a 1 will allow the user to
// clear a MESSAGE_QUEUE from an ISR.
#define USING_MESSAGE_QUEUE_CLEAR_FROM_ISR_METHOD				0

// USING_MESSAGE_QUEUE_IS_EMPTY_FROM_ISR_METHOD if set to a 1 will allow the user
// to check if a MESSAGE_QUEUE is empty from an ISR.
#define USING_MESSAGE_QUEUE_IS_EMPTY_FROM_ISR_METHOD			0

// USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD if set to a 1 will allow the user to
// remove the next piece of data from a MESSAGE_QUEUE while in an ISR.
#define USING_MESSAGE_QUEUE_REMOVE_FROM_ISR_METHOD              0

// USING_MESSAGE_QUEUE_ADD_FROM_ISR_METHOD if set to a 1 will allow the user to
// add data to a MESSAGE_QUEUE while in an ISR.
#define USING_MESSAGE_QUEUE_ADD_FROM_ISR_METHOD					0

// USING_MESSAGE_QUEUE_PEEK_FROM_ISR_METHOD if set to a 1 will allow the user
// to peek at the top piece of data in a MESSAGE_QUEUE from an ISR.
#define USING_MESSAGE_QUEUE_PEEK_FROM_ISR_METHOD                0

// USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD if set to a 1 will allow the user to delete a 
// MESSAGE_QUEUE which was created in the OS heap.  This can be done from an ISR.
#define USING_MESSAGE_QUEUE_DELETE_FROM_ISR_METHOD				0

// USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD if set to a 1 will allow the user to
// block a TASK pending data being added to a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_WAIT_FOR_DATA_METHOD				0

// USING_MESSAGE_QUEUE_ADD_METHOD if set to a 1 will allow the user to add
// data to a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_ADD_METHOD							0

// USING_MESSAGE_QUEUE_REMOVE_METHOD if set to a 1 will allow the user to remove
// data from a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_REMOVE_METHOD						0

// USING_MESSAGE_QUEUE_PEEK_METHOD if set to a 1 will allow the user to peek at
// the top most data of a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_PEEK_METHOD                         0

// USING_MESSAGE_QUEUE_IS_EMPTY_METHOD if set to a 1 will allow the user to check
// and see if a MESSAGE_QUEUE is empty (no data).
#define USING_MESSAGE_QUEUE_IS_EMPTY_METHOD						0

// USING_MESSAGE_QUEUE_CLEAR_METHOD if set to a 1 will allow the user to clear
// a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_CLEAR_METHOD						0

// USING_MESSAGE_QUEUE_DELETE_METHOD if set to a 1 will allow the user to delete
// a MESSAGE_QUEUE which was created in the OS heap.
#define USING_MESSAGE_QUEUE_DELETE_METHOD						0

// USING_MESSAGE_QUEUE_GET_SIZE_METHOD if set to a 1 will allow the user to get
// the size of a MESSAGE_QUEUE.
#define USING_MESSAGE_QUEUE_GET_SIZE_METHOD						0

// USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD if set to a 1 will allow the user
// to specify a method for freeing data which a MESSAGE_QUEUE additional data pointer
// points to.
#define USING_MESSAGE_QUEUE_ADDITIONAL_DATA_FREE_METHOD			0
//----------------------------------------------------------------------------------------------------


// Pipes Configurations
//----------------------------------------------------------------------------------------------------

// USING_CHECK_PIPE_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in Pipe.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_PIPE_PARAMETERS                             0

// USING_PIPE_PEEK_FROM_ISR_METHOD if set to a 1 will allow the user to peek at
// data in a PIPE from an ISR.
#define USING_PIPE_PEEK_FROM_ISR_METHOD							0

// USING_PIPE_FLUSH_FROM_ISR_METHOD if set to a 1 will allow the user to flush
// all data in a PIPE from an ISR.
#define USING_PIPE_FLUSH_FROM_ISR_METHOD						0

// USING_PIPE_CONTAINS_FROM_ISR_METHOD if set to a 1 will allow the user to check
// if a PIPE contains a byte value from an ISR.
#define USING_PIPE_CONTAINS_FROM_ISR_METHOD						0

// USING_PIPE_CONTAINS_SEQUENCE_FROM_ISR_METHOD if set to a 1 will allow the user
// to check a PIPE for a sequence of bytes from an ISR.
#define USING_PIPE_CONTAINS_SEQUENCE_FROM_ISR_METHOD			0

// USING_PIPE_SEEK_FROM_ISR_METHOD if set to a 1 will allow the user to find how
// deep in the PIPE a particular value is from an ISR.
#define USING_PIPE_SEEK_FROM_ISR_METHOD							0

// USING_PIPE_GET_SIZE_FROM_ISR_METHOD if set to a 1 will allow the user to get the
// size of a PIPE from an ISR.
#define USING_PIPE_GET_SIZE_FROM_ISR_METHOD						0

// USING_PIPE_GET_CAPACITY_FROM_ISR_METHOD if set to a 1 will allow the user to 
// get the capacity of a PIPE from an ISR.
#define USING_PIPE_GET_CAPACITY_FROM_ISR_METHOD					0

// USING_PIPE_GET_REMAINING_BYTES_FROM_ISR_METHOD if set to a 1 will allow the user
// to get how many bytes left in the PIPE there are for writing until full from an ISR.
#define USING_PIPE_GET_REMAINING_BYTES_FROM_ISR_METHOD			0

// USING_PIPE_DELETE_FROM_ISR_METHOD if set to a 1 will allow the user to delete a
// PIPE that was previously allocated in the OS heap from an ISR.
#define USING_PIPE_DELETE_FROM_ISR_METHOD						0

// USING_PIPE_STARVATION_PROTECTION if set to a 1 will allow the user to have
// starvation protection enabled on all PIPEs.
#define USING_PIPE_STARVATION_PROTECTION                        0

// USING_PIPE_PEEK_METHOD if set to a 1 will allow the user to peek at data
// in the PIPE.
#define USING_PIPE_PEEK_METHOD                                  0

// USING_PIPE_FLUSH_METHOD if set to a 1 will allow the user to flush all
// data in the PIPE.
#define USING_PIPE_FLUSH_METHOD									0

// USING_PIPE_CONTAINS_METHOD if set to a 1 will allow the user to check if a PIPE
// contains a particular byte value.
#define USING_PIPE_CONTAINS_METHOD								0

// USING_PIPE_CONTAINS_SEQUENCE_METHOD if set to a 1 will allow the user to check
// if a PIPE has a sequence of bytes in it.
#define USING_PIPE_CONTAINS_SEQUENCE_METHOD						0

// USING_PIPE_SEEK_METHOD if set to a 1 will allow the user to see how many bytes
// deep a particular value is in the PIPE.
#define USING_PIPE_SEEK_METHOD									0

// USING_PIPE_GET_SIZE_METHOD if set to a 1 will allow the user to get the size
// of the PIPE.
#define USING_PIPE_GET_SIZE_METHOD								0

// USING_PIPE_GET_CAPACITY_METHOD if set to a 1 will allow the user to check
// the capacity of the PIPE.
#define USING_PIPE_GET_CAPACITY_METHOD							0

// USING_PIPE_GET_REMAINING_BYTES_METHOD if set to a 1 will allow the user to
// get how many remaining bytes are left in the PIPE until it is full.
#define USING_PIPE_GET_REMAINING_BYTES_METHOD					0

// USING_PIPE_DELETE_METHOD if set to a 1 will allow the user to delete a 
// PIPE which was created in the OS heap.
#define USING_PIPE_DELETE_METHOD								0
//----------------------------------------------------------------------------------------------------

// IO Buffer Configuration
//----------------------------------------------------------------------------------------------------

// USING_IO_BUFFERS must be set to 1 in order to use any IO_BUFFER
// related functionality.
#define USING_IO_BUFFERS                                        0

// USING_CHECK_IO_BUFFER_PARAMETERS if set to 1 will enable the code 
// which checks the parameters of data coming into any method in IOBuffer.c.  
// This is useful for debugging code to make sure junk isn't being put into
// the system.  It is up to you if you want to use it during runtime.
#define USING_CHECK_IO_BUFFER_PARAMETERS                        0

// USING_IO_BUFFER_ASYNC_WRITE if set to a 1 will allow the user to write
// data to an IO_BUFFER and not block while waiting for the data to TX.
#define USING_IO_BUFFER_ASYNC_WRITE                             0

// USING_IO_BUFFER_READ_BYTES_METHOD if set to a 1 will allow the user to read
// a specified number of bytes from an IO_BUFFER.
#define USING_IO_BUFFER_READ_BYTES_METHOD                       0

// USING_IO_BUFFER_READ_UNTIL_METHOD if set to a 1 will allow the user to read
// a variable length of bytes from an IO_BUFFER until a certain byte is read.
#define USING_IO_BUFFER_READ_UNTIL_METHOD                       0

// USING_IO_BUFFER_READ_LINE_METHOD if set to a 1 will allow the user to read
// a variable length of bytes from an IO_BUFFER until a certain byte sequence is read.
#define USING_IO_BUFFER_READ_LINE_METHOD                        0

// USING_IO_BUFFER_WRITE_BYTES_METHOD if set to a 1 will allow the user to write
// a specified number of bytes to an IO_BUFFER which will be transmitted.
#define USING_IO_BUFFER_WRITE_BYTES_METHOD                      0

// USING_IO_BUFFER_WRITE_UNTIL_METHOD if set to a 1 will allow the user to write
// a variable number of bytes to an IO_BUFFER until a certain byte is written.
// All data will be transmitted.
#define USING_IO_BUFFER_WRITE_UNTIL_METHOD                      0

// USING_IO_BUFFER_WRITE_LINE_METHOD if set to a 1 will allow the user to write
// a specified number of bytes to an IO_BUFFER.  Then the end of line sequence
// is written to the IO_BUFFER.  All data will be transmitted.
#define USING_IO_BUFFER_WRITE_LINE_METHOD                       0

// USING_IO_BUFFER_RELEASE_METHOD if set to a 1 will allow the user to release
// an IO_BUFFER so that multiple TASKs can use it.
#define USING_IO_BUFFER_RELEASE_METHOD                          0

// USING_IO_BUFFER_CONTAINS_METHOD if set to a 1 will allow the user to check
// if an IO_BUFFER has a certain byte in it.
#define USING_IO_BUFFER_CONTAINS_METHOD                         0

// USING_IO_BUFFER_CONTAINS_SEQUENCE_METHOD if set to a 1 will allow the user to check
// if an IO_BUFFER has a certain byte sequence in it.
#define USING_IO_BUFFER_CONTAINS_SEQUENCE_METHOD                0

// USING_IO_BUFFER_PEEK_METHOD if set to a 1 will allow the user to peek at
// data within the IO_BUFFER.
#define USING_IO_BUFFER_PEEK_METHOD                             0

// USING_IO_BUFFER_FLUSH_METHOD if set to a 1 will allow the user to flush
// the data within the IO_BUFFER.
#define USING_IO_BUFFER_FLUSH_METHOD                            0

// USING_IO_BUFFER_GET_SIZE_METHOD if set to a 1 will allow the user to get
// the number of bytes currently in an IO_BUFFER.
#define USING_IO_BUFFER_GET_SIZE_METHOD                         0

// USING_IO_BUFFER_GET_REMAINING_BYTES_METHOD if set to a 1 will allow 
// the user to get the empty space left in bytes of an IO_BUFFER.
#define USING_IO_BUFFER_GET_REMAINING_BYTES_METHOD              0

// USING_IO_BUFFER_GET_CAPACITY_METHOD if set to a 1 will allow 
// the user to get the number of bytes an IO_BUFFER can store overall.
#define USING_IO_BUFFER_GET_CAPACITY_METHOD                     0

// USING_IO_BUFFER_GET_NEW_LINE_METHOD if set to a 1 will allow the user
// to get the new line sequence associated with the IO_BUFFER.
#define USING_IO_BUFFER_GET_NEW_LINE_METHOD                     0

// USING_IO_BUFFER_SET_NEW_LINE_METHOD if set to a 1 will allow the user
// to set the new line sequence associated with the IO_BUFFER.
#define USING_IO_BUFFER_SET_NEW_LINE_METHOD                     0

// USING_IO_BUFFER_GET_STATE if set to a 1 will allow the user
// to get the RX or TX state of the IO_BUFFER
#define USING_IO_BUFFER_GET_STATE                               0

// Set USING_X_X_IO_BUFFER to a 1 to use that associated IO_BUFFER.
#define USING_UART_1_IO_BUFFER                                  0
#define USING_UART_2_IO_BUFFER                                  0
#define USING_UART_3_IO_BUFFER                                  0
#define USING_UART_4_IO_BUFFER                                  0
#define USING_UART_5_IO_BUFFER                                  0
#define USING_UART_6_IO_BUFFER                                  0
#define USING_CAN_1_IO_BUFFER                                   0
#define USING_CAN_2_IO_BUFFER                                   0
#define USING_ADC_1_IO_BUFFER                                   0
//----------------------------------------------------------------------------------------------------


// Event Configurations
//----------------------------------------------------------------------------------------------------
// NOTE: USING_EVENTS must be defined as a 1 to use EVENTs

// USING_EVENTS if set to a 1 will allow the user to make use of EVENTs.
// EVENTs are port specific and not all may be supported on each microcontroller.
#define USING_EVENTS											0

// USING_RAISE_EVENT_METHOD if set to a 1 will allow the user to 
// raise user EVENTs.
#define USING_RAISE_EVENT_METHOD								0

// USING_EXT_INT_x_EVENT if set to a 1 will allow the user to make use
// of the external interrupt events.  These are usually triggered by
// an input pin changing to a specific state.
#define USING_EXT_INT_0_EVENT                                   0
#define USING_EXT_INT_1_EVENT                                   0
#define USING_EXT_INT_2_EVENT                                   0
#define USING_EXT_INT_3_EVENT                                   0
#define USING_EXT_INT_4_EVENT                                   0

// USING_CN_INT_EVENT if set to a 1 will allow the user to make use
// of the change notification interrupt events.  These are usually triggered by
// an input pin changing states.
#define USING_CN_INT_EVENT                                      0

// USING_TIMER_x_EVENT if set to a 1 will allow the user to make use
// of the timer interrupt events.  These are triggered when a hardware
// timer interrupt occurs.
#define USING_TIMER_1_EVENT                                     0
#define USING_TIMER_2_EVENT                                     0
#define USING_TIMER_3_EVENT                                     0
#define USING_TIMER_4_EVENT                                     0
#define USING_TIMER_5_EVENT                                     0

// USING_ADC_x_EVENT if set to a 1 will allow the user to make use
// of the ADC interrupt events.  These are triggered when an ADC finishes
// a conversion.
#define USING_ADC_1_EVENT                                       0

// USING_RTCC_ALARM_EVENT if set to a 1 will allow the user to make use
// of the RTCC interrupt events.  These are triggered when an interrupt
// occurs with the RTCC module.
#define USING_RTCC_ALARM_EVENT                                  0

// USING_EXT_OSC_FAILED_EVENT if set to a 1 will generate an EVENT
// when the external oscillator interrupt fires.
#define USING_EXT_OSC_FAILED_EVENT                              0

// USING_UART_x_x_DONE_EVENT if set to a 1 will allow the user to make use
// of the UART TX, RX, and error interrupt events.
#define USING_UART_1_TX_DONE_EVENT                              0
#define USING_UART_2_TX_DONE_EVENT                              0
#define USING_UART_3_TX_DONE_EVENT                              0
#define USING_UART_4_TX_DONE_EVENT                              0
#define USING_UART_5_TX_DONE_EVENT                              0
#define USING_UART_6_TX_DONE_EVENT                              0
#define USING_UART_1_RX_DONE_EVENT                              0
#define USING_UART_2_RX_DONE_EVENT                              0
#define USING_UART_3_RX_DONE_EVENT                              0
#define USING_UART_4_RX_DONE_EVENT                              0
#define USING_UART_5_RX_DONE_EVENT                              0
#define USING_UART_6_RX_DONE_EVENT                           	0
#define USING_UART_1_ERROR_EVENT                                0
#define USING_UART_2_ERROR_EVENT                                0
#define USING_UART_3_ERROR_EVENT                                0
#define USING_UART_4_ERROR_EVENT                                0
#define USING_UART_5_ERROR_EVENT                                0
#define USING_UART_6_ERROR_EVENT                                0

// USING_CAN_x_x_EVENT if set to a 1 will allow the user to make use
// of the associated CAN TX, RX or error interrupts.
#define USING_CAN_1_TX_EVENT                                    0
#define USING_CAN_2_TX_EVENT                                    0
#define USING_CAN_1_RX_EVENT                                    0
#define USING_CAN_2_RX_EVENT                                    0
#define USING_CAN_1_ERROR_EVENT                                 0
#define USING_CAN_2_ERROR_EVENT                                 0

// USING_MEMORY_WARNING_EVENT if set to a 1 will allow the generation
// of an EVENT if the memory usage exceeds a user defined value.
#define USING_MEMORY_WARNING_EVENT                              0

// USING_MEMORY_WARNING_CLEAR_EVENT if set to a 1 will allow the generation
// of an EVENT if the memory usage exceeds a user defined value and then 
// decreases below the user defined value.
#define USING_MEMORY_WARNING_CLEAR_EVENT                        0

// USING_CPU_EXCEPTION_RAISED_EVENT will generate an EVENT when a
// CPU exception is raised.
#define USING_CPU_EXCEPTION_RAISED_EVENT                        0

// USING_IO_BUFFER_OVERFLOW_EVENT will generate an EVENT when a
// IO_BUFFER enters into an RX overflow condition
#define USING_IO_BUFFER_OVERFLOW_EVENT                          0

// USING_USER_x_EVENT will allow the user to raise custom EVENTs.
// These EVENTs must not be generated in a critical section.
#define USING_USER_1_EVENT                                      0
#define USING_USER_2_EVENT                                      0
#define USING_USER_3_EVENT                                      0
#define USING_USER_4_EVENT                                      0
#define USING_USER_5_EVENT                                      0
#define USING_USER_6_EVENT                                      0
#define USING_USER_7_EVENT                                      0
#define USING_USER_8_EVENT                                      0
#define USING_USER_9_EVENT                                      0
#define USING_USER_10_EVENT                                     0
//----------------------------------------------------------------------------------------------------


// Callback Configurations
//----------------------------------------------------------------------------------------------------
// USING_EXT_INT_x_CALLBACK if set to a 1 will allow the user to make use
// of the external interrupt callback.  These are usually triggered by
// an input pin changing to a specific state.
#define USING_EXT_INT_0_CALLBACK                                0
#define USING_EXT_INT_1_CALLBACK                                0
#define USING_EXT_INT_2_CALLBACK                                0
#define USING_EXT_INT_3_CALLBACK                                0
#define USING_EXT_INT_4_CALLBACK                                0

// USING_CN_INT_EVENT if set to a 1 will allow the user to make use
// of the change notification interrupt callback.  These are usually triggered by
// an input pin changing states.
#define USING_CN_INT_CALLBACK                                   0

// USING_TIMER_x_EVENT if set to a 1 will allow the user to make use
// of the timer interrupt callback.  These are triggered when a hardware
// timer interrupt occurs.
#define USING_TIMER_1_CALLBACK                                  0
#define USING_TIMER_2_CALLBACK                                  0
#define USING_TIMER_3_CALLBACK                                  0
#define USING_TIMER_4_CALLBACK                                  0
#define USING_TIMER_5_CALLBACK                                  0

// USING_ADC_x_CALLBACK if set to a 1 will allow the user to make use
// of the ADC interrupt callback.  These are triggered when an ADC finishes
// a conversion.
#define USING_ADC_1_CALLBACK                                    0

// USING_RTCC_ALARM_EVENT if set to a 1 will allow the user to make use
// of the RTCC interrupt callback.  These are triggered when an interrupt
// occurs with the RTCC module.
#define USING_RTCC_ALARM_CALLBACK                               0

// USING_EXT_OSC_FAILED_EVENT if set to a 1 will call the ext osc failed
// callback when the external oscillator interrupt fires.
#define USING_EXT_OSC_FAILED_CALLBACK                           0

// USING_UART_x_x_CALLBACK if set to a 1 will allow the user to make
// use of the specified UART TX, RX or error callback.
#define USING_UART_1_TX_CALLBACK                                0
#define USING_UART_2_TX_CALLBACK                                0
#define USING_UART_3_TX_CALLBACK                                0
#define USING_UART_4_TX_CALLBACK                                0
#define USING_UART_5_TX_CALLBACK                                0
#define USING_UART_6_TX_CALLBACK                                0
#define USING_UART_1_RX_CALLBACK                                0
#define USING_UART_2_RX_CALLBACK                                0
#define USING_UART_3_RX_CALLBACK                                0
#define USING_UART_4_RX_CALLBACK                                0
#define USING_UART_5_RX_CALLBACK                                0
#define USING_UART_6_RX_CALLBACK                                0
#define USING_UART_1_ERROR_CALLBACK                             0
#define USING_UART_2_ERROR_CALLBACK                             0
#define USING_UART_3_ERROR_CALLBACK                             0
#define USING_UART_4_ERROR_CALLBACK                             0
#define USING_UART_5_ERROR_CALLBACK                             0
#define USING_UART_6_ERROR_CALLBACK                             0

// USING_CAN_x_x_CALLBACK if set to a 1 will allow the user to make
// use of the specified CAN TX, RX or error callback.0
#define USING_CAN_1_TX_CALLBACK                                 0
#define USING_CAN_2_TX_CALLBACK                                 0
#define USING_CAN_1_RX_CALLBACK                                 0
#define USING_CAN_2_RX_CALLBACK                                 0
#define USING_CAN_1_ERROR_CALLBACK                              0
#define USING_CAN_2_ERROR_CALLBACK                              0

// USING_MEMORY_WARNING_EVENT if set to a 1 will allow the calling
// of a callback if the memory usage exceeds a user defined value.
#define USING_MEMORY_WARNING_USER_CALLBACK                      0

// USING_MEMORY_WARNING_EVENT if set to a 1 will allow the calling
// of a callback if the memory usage exceeds a user defined value.
#define USING_MEMORY_WARNING_CLEAR_USER_CALLBACK                0

// USING_CPU_EXCEPTION_RAISED_USER_CALLBACK if set to a 1 will call a callback
// if a CPU exception occurs.
#define USING_CPU_EXCEPTION_RAISED_USER_CALLBACK                0

// USING_IO_BUFFER_OVERFLOW_CALLBACK if set to a 1 will call a callback
// whenever an IO_BUFFER enters an RX overflow condition
#define USING_IO_BUFFER_OVERFLOW_CALLBACK                       0

// USING_IDLE_TASK_USER_CALLBACK if set to a 1 will call a callback
// each time the Idle TASK is run.
#define USING_IDLE_TASK_USER_CALLBACK                           0

// USING_MAINTENANCE_TASK_USER_CALLBACK if set to a 1 will call a callback
// each time the Maintenance TASK is run.
#define USING_MAINTENANCE_TASK_USER_CALLBACK                    0

// USING_IO_BUFFER_TASK_USER_CALLBACK if set to a 1 will call a callback
// each time the IO Buffer TASK is run.
#define USING_IO_BUFFER_TASK_USER_CALLBACK                      0

// USING_TASK_CHECK_IN_USER_CALLBACK if set to a 1 will call a callback
// if a TASK exceeds its check in time.
#define USING_TASK_CHECK_IN_USER_CALLBACK                       0

// USING_OS_TICK_UPDATE_USER_CALLBACK if set to a 1 will call a callback
// each time the OS timer produces another tick.
#define USING_OS_TICK_UPDATE_USER_CALLBACK                      0

// USING_CONTEXT_SWITCH_USER_CALLBACK if set to a 1 will call a callback
// each time the current TASK is swapped out for another TASK.
#define USING_CONTEXT_SWITCH_USER_CALLBACK                      0

// USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK if set to a 1 will call a callback
// right before the device enters sleep mode.
#define USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK             0

// USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK if set to a 1 will call a callback
// right after the device wakes up from sleep mode.
#define USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK				0
//----------------------------------------------------------------------------------------------------

/*
	This area checks for configurations mismatches and puts out errors if any are found.
*/

// These have to do with TASK configurations
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#if(USING_RESTART_TASK == 1 && USING_TASK_DELAY_TICKS_METHOD != 1)
	#error "USING_TASK_DELAY_TICKS_METHOD must be defined as 1 if USING_RESTART_TASK == 1"
#endif // end of #if(USING_RESTART_TASK == 1 && USING_TASK_DELAY_TICKS_METHOD != 1)

#if(USING_DELETE_TASK == 1 && USING_TASK_DELAY_TICKS_METHOD != 1)
	#error "USING_TASK_DELAY_TICKS_METHOD must be defined as 1 if USING_DELETE_TASK == 1"
#endif // end of #if(USING_DELETE_TASK == 1 && USING_TASK_DELAY_TICKS_METHOD != 1)

#if((USING_RESUME_ALL_TASKS_METHOD == 1 || USING_RESUME_TASK_METHOD == 1) && USING_SUSPEND_ALL_TASKS_METHOD != 1 && USING_SUSPEND_TASK_METHOD != 1)
	#error "If USING_RESUME_ALL_TASKS_METHOD == 1 or USING_RESUME_TASK_METHOD == 1, at least USING_SUSPEND_TASK_METHOD or USING_SUSPEND_ALL_TASKS_METHOD must be defined as 1"
#endif // end of #if((USING_RESUME_ALL_TASKS_METHOD == 1 || USING_RESUME_TASK_METHOD == 1) && USING_SUSPEND_ALL_TASKS_METHOD != 1 && USING_SUSPEND_TASK_METHOD != 1)

#if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)
    #if (NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS == 0)
        #error "NUMBER_OF_LOCAL_THREAD_STORAGE_POINTERS cannot be 0 if USING_TASK_LOCAL_STORAGE_ACCESS == 1!"
    #endif // end of #if (USING_TASK_LOCAL_STORAGE_ACCESS == 0)
#endif // end of #if (USING_TASK_LOCAL_STORAGE_ACCESS == 1)

#if (USING_TASK_RUNTIME_HISTORY == 1)
    #if (USING_TASK_RUNTIME_EXECUTION_COUNTER != 1)
        #error "If USING_TASK_RUNTIME_HISTORY == 1, USING_TASK_RUNTIME_EXECUTION_COUNTER must be 1!"
    #endif // end of #if (USING_TASK_RUNTIME_EXECUTION_COUNTER != 1)

    #if (TASK_RUNTIME_HISTORY_SIZE_IN_TASKS < 3)
        #error "If USING_TASK_RUNTIME_HISTORY == 1, TASK_RUNTIME_HISTORY_SIZE_IN_TASKS must be greater than 2!"
    #endif // end of #if (TASK_RUNTIME_HISTORY_SIZE_IN_TASKS < 3)
#endif // end of #if (USING_TASK_RUNTIME_HISTORY == 1)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // end of #ifndef RTOS_CONFIG_H
