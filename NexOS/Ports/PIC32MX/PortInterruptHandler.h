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

#ifndef PORT_INTERRUPT_HANDLERS_H
    #define PORT_INTERRUPT_HANDLERS_H

#include <plib.h>

#include "RTOSConfig.h"

#if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_EXTERNAL_0_VECTOR))) ExternalInterrupt0Interrupt(void);
#endif // end of #if (USING_EXT_INT_0_EVENT == 1 || USING_EXT_INT_0_CALLBACK == 1)

#if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_EXTERNAL_1_VECTOR))) ExternalInterrupt1Interrupt(void);
#endif // end of #if (USING_EXT_INT_1_EVENT == 1 || USING_EXT_INT_1_CALLBACK == 1)
    
#if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_EXTERNAL_2_VECTOR))) ExternalInterrupt2Interrupt(void);
#endif // end of #if (USING_EXT_INT_2_EVENT == 1 || USING_EXT_INT_2_CALLBACK == 1)
    
#if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_EXTERNAL_3_VECTOR))) ExternalInterrupt3Interrupt(void);
#endif // end of #if (USING_EXT_INT_3_EVENT == 1 || USING_EXT_INT_3_CALLBACK == 1)
    
#if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_EXTERNAL_4_VECTOR))) ExternalInterrupt4Interrupt(void);
#endif // end of #if (USING_EXT_INT_4_EVENT == 1 || USING_EXT_INT_4_CALLBACK == 1)
    
#if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_TIMER_1_VECTOR))) Timer1Interrupt(void);
#endif // end of #if (USING_TIMER_1_EVENT == 1 || USING_TIMER_1_CALLBACK == 1)
    
#if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_TIMER_2_VECTOR))) Timer2Interrupt(void);
#endif // end of #if (USING_TIMER_2_EVENT == 1 || USING_TIMER_2_CALLBACK == 1)
    
#if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_TIMER_3_VECTOR))) Timer3Interrupt(void);
#endif // end of #if (USING_TIMER_3_EVENT == 1 || USING_TIMER_3_CALLBACK == 1)
    
#if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_TIMER_4_VECTOR))) Timer4Interrupt(void);
#endif // end of #if (USING_TIMER_4_EVENT == 1 || USING_TIMER_4_CALLBACK == 1)
    
#if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_TIMER_5_VECTOR))) Timer5Interrupt(void);
#endif // end of #if (USING_TIMER_5_EVENT == 1 || USING_TIMER_5_CALLBACK == 1)

#if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_CHANGE_NOTICE_VECTOR))) ChangeNotificationInterrupt(void);
#endif // end of #if (USING_CN_INT_EVENT == 1 || USING_CN_INT_CALLBACK == 1)

#if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
    void __attribute__((interrupt(IPL1SAVEALL), vector(_FAIL_SAFE_MONITOR_VECTOR))) ExternalOscillatorFailedInterrupt(void);
#endif // end of #if (USING_EXT_OSC_FAILED_EVENT == 1 || USING_EXT_OSC_FAILED_CALLBACK == 1)
    
#define PortClearExternalInterrupt0Flag()                                   INTClearFlag(INT_INT0)
#define PortClearExternalInterrupt1Flag()                                   INTClearFlag(INT_INT1)
#define PortClearExternalInterrupt2Flag()                                   INTClearFlag(INT_INT2)
#define PortClearExternalInterrupt3Flag()                                   INTClearFlag(INT_INT3)
#define PortClearExternalInterrupt4Flag()                                   INTClearFlag(INT_INT4)
#define PortClearChangeNotificationInterruptFlag()                          INTClearFlag(INT_CN)
#define PortClearTimer1InterruptFlag()                                      INTClearFlag(INT_T1)
#define PortClearTimer2InterruptFlag()                                      INTClearFlag(INT_T2)
#define PortClearTimer3InterruptFlag()                                      INTClearFlag(INT_T3)
#define PortClearTimer4InterruptFlag()                                      INTClearFlag(INT_T4)
#define PortClearTimer5InterruptFlag()                                      INTClearFlag(INT_T5)
#define PortClearExternalOscillatorFailedInterruptFlag()                    INTClearFlag(INT_FSCM)

#endif // end of #ifndef PORT_INTERRUPT_HANDLERS_H
