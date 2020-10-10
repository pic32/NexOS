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
 - Buffers for the TX or RX of data (in progress, and not really designed around being real time or efficient)


The main design goals of the NexOS are as follows:
 - Runtime Safety
 - Code Modularity and Clarity
 - Execution Speed
 - Functionality (A main goal of a RTOS should be to afford the user enough functionality to get the Idle task to run as much as possible!)
 - Deterministic Runtime Behavior (this is only a wish and has not been fully evaluated...)

The main goal of offering the source for this project is for other people to use it and provide feedback. I cannot possibly test all the scenarios the RTOS should be put through as this is essentially an infinite list. That is the goal I hope people playing with it would fill. I will try to fix any and every bug that is reported.

Please be sure to refer to the Getting Started Guide file located in the project tree to see the best way to progress using this project.
