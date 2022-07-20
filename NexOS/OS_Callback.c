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

#include "../NexOS/Kernel/OS_Callback.h"

/*
 * This intent of this file is to show the user the different callbacks, their
 * names, and the arguments they take depending on the configuration of the 
 * RTOS.  This file does not need to be included in the build if the user decides
 * to implement the callbacks in their own files.  Otherwise, if callbacks are
 * used, include this file to avoid linking errors and implement your callback
 * code in each desired function.
 */

// Kernel.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
#if (USING_TASK_CHECK_IN_USER_CALLBACK == 1)
    BOOL TaskCheckInUserCallback(TASK *Task)
    {
        return TRUE;
    }
#endif // end of #if(USING_TASK_CHECK_IN_USER_CALLBACK == 1)

#if (USING_CONTEXT_SWITCH_USER_CALLBACK == 1)
    void ContextSwitchUserCallback(void)
    {

    }
#endif // end of #if(USING_CONTEXT_SWITCH_USER_CALLBACK == 1)

#if (USING_OS_TICK_UPDATE_USER_CALLBACK == 1)
    void OSTickUpdateUserCallback(UINT32 CurrentTickCount)
    {

    }
#endif // end of #if (USING_OS_TICK_UPDATE_USER_CALLBACK == 1)
    
#if (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)
    void TaskStackOverflowUserCallback(TASK *Task)
    {
        
    }
#endif // end of #if (USING_CHECK_TASK_STACK_FOR_OVERFLOW == 1)
    
#if (USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
    void EnterDeviceSleepModeUserCallback(void)
    {
        
    }
#endif // end of #if (USING_ENTER_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)

#if (USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)
    void ExitDeviceSleepModeUserCallback(void)
    {
        
    }
#endif // end of #if (USING_EXIT_DEVICE_SLEEP_MODE_USER_CALLBACK == 1)

#if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
    BOOL GeneralExceptionHandlerUserCallback(TASK *CurrentTask, OS_EXCEPTION_CODE ExceptionCode, UINT32 ExceptionAddress, BOOL UserCodeCurrentlyExecuting)
    {
        return TRUE;
    }
#endif // end of #if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------


// Memory.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
#if (USING_MEMORY_WARNING_USER_CALLBACK == 1)
    void MemoryWarningUserCallback(UINT32 HeapUsageInBytes)
    {
        
    }
#endif // end of #if (USING_MEMORY_WARNING_USER_CALLBACK == 1)

#if (USING_MEMORY_WARNING_CLEAR_USER_CALLBACK == 1)
    void MemoryWarningClearUserCallback(void)
    {
        
    }
#endif // end of #if (USING_MEMORY_WARNING_CLEAR_USER_CALLBACK == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------


// KernelTasks.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// this method does not have to return, but it still cannot block!
#if (USING_IDLE_TASK_USER_CALLBACK == 1)
    void IdleTaskUserCallback(void *Args)
    {
        //DeviceEnterSleepMode();
    }
#endif // end of #if (USING_IDLE_TASK_USER_CALLBACK == 1)

#if (USING_MAINTENANCE_TASK_USER_CALLBACK == 1)
    void MaintenanceTaskUserCallback(void *Args)
    {

    }
#endif // end of #if (USING_MAINTENANCE_TASK_USER_CALLBACK == 1)
    
#if (USING_IO_BUFFER_TASK_USER_CALLBACK == 1)
    void IOBufferTaskUserCallback(void *Args)
    {

    }
#endif // end of #if (USING_IO_BUFFER_TASK_USER_CALLBACK == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
// IOBuffer.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
#if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)
    #include "../NexOS/IOBuffer/IOBuffer.h"

    void IOBufferOverflowCallback(IO_BUFFER_ID IOBufferID)
    {
        
    }
#endif // end of #if (USING_IO_BUFFER_OVERFLOW_CALLBACK == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------
