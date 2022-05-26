#include "../NexOS/Kernel/OS_Callback.h"

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

#endif // end of #if (USING_OS_GENERAL_EXCEPTION_HANDLER == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------


// Memory.c Callbacks
//----------------------------------------------------------------------------------------------------------------------------------------------------------
#if (USING_MEMORY_WARNING_USER_CALLBACK == 1)
    #include <stdio.h>
    
    void MemoryWarningUserCallback(UINT32 HeapUsageInBytes)
    {
        printf("\r\nMemory warning callback triggered!  %i bytes of the heap used.\r\n", (unsigned int)HeapUsageInBytes);
    }
#endif // end of #if (USING_MEMORY_WARNING_USER_CALLBACK == 1)

#if (USING_MEMORY_WARNING_CLEAR_USER_CALLBACK == 1)
    void MemoryWarningClearUserCallback(void)
    {
        printf("\r\nMemory warning clear callback triggered!  Heap usage is below %i bytes which is defined as MEMORY_WARNING_LEVEL_IN_BYTES in RTOSConfig.h.\r\n", MEMORY_WARNING_LEVEL_IN_BYTES);
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
    void MaintenanceTaskUserCallback(void)
    {

    }
#endif // end of #if (USING_MAINTENANCE_TASK_USER_CALLBACK == 1)
//----------------------------------------------------------------------------------------------------------------------------------------------------------