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

#include "RTOSConfig.h"
#include "PortInterruptHandler.h"
#include "../../Kernel/Kernel.h"
#include "../../Event/OS_EventCallback.h"

#if (USING_EVENTS == 1)
    #include "../../Event/Event.h"
#endif // end of #if (USING_EVENTS == 1)

#if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
    OS_WORD *ExternalInterrupt0Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt0Flag();
        
        #if (USING_EXT_INT_0_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_0_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_0_EVENT == 1)

        #if (USING_EXT_INT_0_CALLBACK == 1)
            ExternalInterrupt0Callback();
        #endif // end of #if (USING_EXT_INT_0_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
    
#if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
    OS_WORD *ExternalInterrupt1Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt1Flag();

        #if (USING_EXT_INT_1_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_1_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_1_EVENT == 1)

        #if (USING_EXT_INT_1_CALLBACK == 1)
            ExternalInterrupt1Callback();
        #endif // end of #if (USING_EXT_INT_1_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)

#if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    OS_WORD *ExternalInterrupt2Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt2Flag();

        #if (USING_EXT_INT_2_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_2_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_2_EVENT == 1)

        #if (USING_EXT_INT_2_CALLBACK == 1)
            ExternalInterrupt2Callback();
        #endif // end of #if (USING_EXT_INT_2_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    
#if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    OS_WORD *ExternalInterrupt3Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt3Flag();

        #if (USING_EXT_INT_3_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_3_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_3_EVENT == 1)

        #if (USING_EXT_INT_3_CALLBACK == 1)
            ExternalInterrupt3Callback();
        #endif // end of #if (USING_EXT_INT_3_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    
#if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
    OS_WORD *ExternalInterrupt4Handler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalInterrupt4Flag();

        #if (USING_EXT_INT_4_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_INTERRUPT_4_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_4_EVENT == 1)

        #if (USING_EXT_INT_4_CALLBACK == 1)
            ExternalInterrupt4Callback();
        #endif // end of #if (USING_EXT_INT_4_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)

#if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
    OS_WORD *ChangeNotificationInterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearChangeNotificationInterruptFlag();

        #if (USING_CN_INT_EVENT == 1)
            if(OS_RaiseEvent(CHANGE_NOTIFICATION_INTERRUPT_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_INT_4_EVENT == 1)

        #if (USING_CN_INT_CALLBACK == 1)
            ChangeNotificationInterruptCallback();
        #endif // end of #if (USING_CN_INT_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
    
#if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)
    OS_WORD *Timer1InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer1InterruptFlag();

        #if (USING_TIMER_1_EVENT == 1)
            if(OS_RaiseEvent(TIMER_1_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_1_EVENT == 1)

        #if (USING_TIMER_1_CALLBACK == 1)
            Timer1InterruptCallback();
        #endif // end of #if (USING_TIMER_1_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)  
    
#if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)
    OS_WORD *Timer2InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer2InterruptFlag();

        #if (USING_TIMER_2_EVENT == 1)
            if(OS_RaiseEvent(TIMER_2_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_2_EVENT == 1)

        #if (USING_TIMER_2_CALLBACK == 1)
            Timer2InterruptCallback();
        #endif // end of #if (USING_TIMER_2_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)  
    
#if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)
    OS_WORD *Timer3InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer3InterruptFlag();

        #if (USING_TIMER_3_EVENT == 1)
            if(OS_RaiseEvent(TIMER_3_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_3_EVENT == 1)

        #if (USING_TIMER_3_CALLBACK == 1)
            Timer3InterruptCallback();
        #endif // end of #if (USING_TIMER_3_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)  
    
#if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)
    OS_WORD *Timer4InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer4InterruptFlag();

        #if (USING_TIMER_4_EVENT == 1)
            if(OS_RaiseEvent(TIMER_4_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_4_EVENT == 1)

        #if (USING_TIMER_4_CALLBACK == 1)
            Timer4InterruptCallback();
        #endif // end of #if (USING_TIMER_4_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)  
 
#if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)
    OS_WORD *Timer5InterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearTimer5InterruptFlag();

        #if (USING_TIMER_5_EVENT == 1)
            if(OS_RaiseEvent(TIMER_5_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_TIMER_5_EVENT == 1)

        #if (USING_TIMER_5_CALLBACK == 1)
            Timer5InterruptCallback();
        #endif // end of #if (USING_TIMER_5_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)  
    
#if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
    OS_WORD *ExternalOscillatorFailedInterruptHandler(OS_WORD *CurrentTaskStackPointer)
    {
        // first clear out the interrupt flag
        PortClearExternalOscillatorFailedInterruptFlag();

        #if (USING_EXT_OSC_FAILED_EVENT == 1)
            if(OS_RaiseEvent(EXTERNAL_OSCILLATOR_FAILED_EVENT) == TRUE)
                CurrentTaskStackPointer = OS_NextTask(CurrentTaskStackPointer);
        #endif // end of #if(USING_EXT_OSC_FAILED_EVENT == 1)

        #if (USING_EXT_OSC_FAILED_CALLBACK == 1)
            ExternalOscillatorFailedInterruptCallback();
        #endif // end of #if (USING_EXT_OSC_FAILED_CALLBACK == 1)

        return CurrentTaskStackPointer;
    }
#endif // end of #if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
