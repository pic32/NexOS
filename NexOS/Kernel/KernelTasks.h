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

#ifndef KERNEL_TASKS_H
	#define KERNEL_TASKS_H

/*
	UINT32 IdleTaskCode(void *Args)

	Description: This is the code that the Idle TASK executes.  It never returns
    from this method.

	Blocking: No

	User Callable: No

	Arguments:
		void *Args - The arguments passed with the macro IDLE_TASK_ARGS.

	Returns:
        UINT32 - This method should never return.

	Notes:
        - None

	See Also:
		- IDLE_TASK_ARGS, USING_IDLE_TASK_USER_CALLBACK inside of RTOSConfig.h
*/
UINT32 IdleTaskCode(void *Args);

/*
	UINT32 MaintenanceTaskCode(void *Args)

	Description: This is the code that the Maintenance TASK executes.  It never returns
    from this method.

	Blocking: No

	User Callable: No

	Arguments:
		void *Args - The arguments passed with the macro MAINTENANCE_TASK_ARGS.

	Returns:
        UINT32 - This method should never return.

	Notes:
        - None

	See Also:
		- MAINTENANCE_TASK_ARGS, USING_MAINTENANCE_TASK_USER_CALLBACK inside of RTOSConfig.h
*/
UINT32 MaintenanceTaskCode(void *Args);

/*
	UINT32 IOBufferTaskCode(void *Args)

	Description: This is the code that the IO Buffer TASK executes.  It never returns
    from this method.

	Blocking: No

	User Callable: No

	Arguments:
		void *Args - The arguments passed with the macro MAINTENANCE_TASK_ARGS.

	Returns:
        UINT32 - This method should never return.

	Notes:
        - None

	See Also:
		- MAINTENANCE_TASK_ARGS, USING_MAINTENANCE_TASK_USER_CALLBACK inside of RTOSConfig.h
*/
UINT32 IOBufferTaskCode(void *Args);

#endif // end of KERNEL_TASKS_H
