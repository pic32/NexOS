12/20/2022

Below are a list of the example projects which can be opened in MPLAB X IDE:
	- Advanced Task Creation
	- Callback Timer
	- Event Timer
	- External Event
	- File System
	- Handling Exceptions
	- Heap Event and Monitoring
	- IO Buffer
	- Kernel Project
	- Pipe
	- Simple Semaphore
	- Simple Task Creation
	- Task Check In

These example projects either run in MPLAB X Simulator, the Ethernet Starter Kit II,
or in the case of IO Buffer a custom PCB.  Each project has a section at the top
which explains what the project is and why it was made.

The project Kernel Project is just there to have everything in the configuration file
RTOSConfig.h enabled.  This is just to make sure there are no compilation errors.

The Folder NexOS holds the actual source code of the RTOS.  All projects reference
this folder and its contents.  A similar relationship exists between the projects and
the contents of the folder Generic Libraries.

The files DoubleLinkedListConfig.h and GenericBufferConfig.h are just for configuring
their respective libraries.  You always need DoubleLinkedListConfig.h, but only need
GenericBufferConfig.h if using IO_BUFFER or PIPE.

There are two guides as of this time.  The NexOS Quick Start Guide and the NexOS User
Manual.  These currently hold any external documenation on the NexOS.  For further
documentation look at the respective .h files and the RTOSConfig.h.  Alternatively, you
can always email me at brodiedog32@outlook.com with any questions.