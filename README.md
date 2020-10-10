# NexOS
The NexOS is a RTOS written and tested on the PIC32MX series and its hardware features. The NexOS CPU scheduler is totally priority driven and will execute the highest priority task that is eligible run. If there are multiple highest priority tasks to run, they are done so in a round robin fashion.

The NexOS RTOS offers a few sets of unique features which are either not common or found in other available RTOS:

 - The ability to restart a task
 - Software watchdog timers for tasks
 - Built in hardware events for task synchronization and execution
 - Event timers for measuring the time between events (tasks don't need to be running to measure this time)
 - Real time heap monitoring (how much of the heap have I used?)
 - Extensive user callbacks for various conditions and events
 - Starvation protection for certain resources
 - Exception handling and recovery (system at least wouldn't be totally halted if it occurs)
 - Buffers for the TX or RX of data (in progress)

The main design goals of the NexOS are as follows:
 - Runtime Safety
 - Code Modularity and Clarity
 - Execution Speed
 - Functionality (A main goal of a RTOS should be to afford the user enough functionality to get the Idle task to run as much as possible!)
 - Deterministic Runtime Behavior (this is only a wish and has not been fully evaluated...)
 
The repository has the main source code for the NexOS and a bunch of demonstation projects.  As of now the demonstration projects are setup to work with just MPLAB Simulator, or the Ethernet Starter Kit from Microchip.  Below is a quick description of the folders included in this repository.
 - Simple Task Creation: The smallest possible project to get the NexOS up and running.  This is a good starting point.
 - Advanced Task Creation: A demonstration project showing advanced features of task creation and execution.
 - Callback Timer: A demonstration project showing how to use a callback timer.
 - Event Timer: A demonstration project showing how to use an event timer.
 - External Event: A demonstration project showing how a task can block on an external event.
 - Generic Libraries: A folder containing data structures and libraries not specific to the OS and could be used in other projects.
 - Handling Exceptions: A demonstration project showing how to handle and recover from CPU exceptions.
 - NexOS: A folder containing all the source code for the NexOS.
 - Pipe: A demonstration project showing two tasks communicating through a pipe.
 - Simple Semaphore: A demonstration project showing how to use a semaphore.
 - Task Check In: A demonstration project showing the software watch dog timer feature.

The main goal of offering the source for this project is for other people to use it and provide feedback. I cannot possibly test all the scenarios the RTOS should be put through as this is essentially an infinite list. That is the goal I hope people playing with it would fill. I will try to fix any and every bug that is reported.  This project also serves as a good learning tool for anyone else interested in learning about Real Time Operating Systems.  There is also a list of items in the Git Projects tab.  The list contains all the items that will be added to the RTOS and any items that are currently being worked on.

Please be sure to refer to the NexOS Quick Start Guide located in the project tree to see the quickest way to progress using this project.
