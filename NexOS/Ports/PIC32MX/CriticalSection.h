/*
    NexOS Kernel Version v1.02.00
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

#ifndef CRITICAL_SECTIONS_H
	#define CRITICAL_SECTIONS_H

#include "Port.h"

/*
	void EnterCritical(void)

	Description: This method shuts off interupts with a priority level of OS_PRIORITY
	or lower.  It also increments the critical count.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: Nothing

	Notes:
		- None

	See Also:
		- ExitCritical()
*/
void EnterCritical(void);

/*
	void ExitCritical(void)

	Description: This method first decrements the critical count.  If the critical count
	is zero this method will then re-enable interrupts at or below OS_PRIORITY level.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: Nothing

	Notes:
		- None

	See Also:
		- EnterCritical()
*/
void ExitCritical(void);

/*
	OS_WORD GetCurrentCriticalCount(void)

	Description: This method will return the current critical count.  A critical
    count of > 0 will result in interrupts being disabled.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: 
        OS_WORD - The current critical count.

	Notes:
		- None

	See Also:
		- EnterCritical(),ExitCritical()
        - USING_GET_CURRNET_CRITICAL_COUNT_METHOD must be defined as a 1 in RTOSConfig.h
*/
OS_WORD GetCurrentCriticalCount(void);

/*
	void ExitCriticalUnconditional(void)

	Description: This method will set the critical count back to zero and enable
    interrupts regardless of what the critical count was.

	Blocking: No

	User Callable: Yes

	Arguments:
		None

	Returns: 
        None

	Notes:
		- None

	See Also:
		- EnterCritical(),ExitCritical()
        - USING_EXIT_CRITICAL_UNCONDITIONAL_METHOD must be defined as a 1 in RTOSConfig.h
*/
void ExitCriticalUnconditional(void);

#endif // end of #ifndef CRITICAL_SECTIONS_H
