Operating System runs on a Computer/Microcontroller to accomplish a functions.
	1. Task Scheduling - Background Task and User Application. Background Processes are always running simultaneously.  
	2. Resource Management - Manages files, libraries and folders so can be accessed to read/write when needed. 
	3. Device Drivers - Provide these drivers so external inputs can be used. 
	
	GPOS - General Purpose Operating Systems, e.g. Windows, MAC-OS, Linux. Designed for Human Interaction, so Scheduling is often prioritize. So Timing Deadlines can be pushed back or lag. These are often not noticeable by humans. The Scheduler is non-Deterministic meaning when the task is called or duration is unknown. So Medical and Vehicle where time deadlines are crucial may have catastrophic failure. 
	
	RTOS - Real-Time Operating System 
	Scheduler can guarantee meeting timing deadlines on the task. Usually used for Microcontrollers e.g. Wi-Fi/Bluetooth Stacks, LCD Drivers, Barebones RTS for Schedulers (Have to import/write own file drivers). RTOS also allow concurrent applications.
	
	
	Super Loop:
	
	Similar to Arduino Setup. Easy to implement, little overhead, good for little task. Saves on CPU cycles, memory, easier to debug. Can include to Interrupts Service Routine (ISR) (Ideally for less 1 millisecond use this; < 1 nanosecond, faster CPU or use a FPGA).
	
	However, cannot do task simultaneously/concurrently. Lags may cause errors. 

	
	RTOS:
	
	Multicore Processor handles each task. However, Single Core microcontrollers will divide the CPU time among the task. Give higher priority to certain task to happen sooner, or take more time.
	
	Task: Set of Program Instructions loaded in memory
	Thread: Unit of CPU utilization with its own program counter and stack. Threads within a process share resources e.g. heap memory to handle the task.
	Process: Instance of a computer Program. Usually 1 or more threads are used to accomplish a task.
	
	FreeRTOS: Task = Thread.
	
	RTOS: Only runs 1 process. While GPOS many processes. Interrupts will work in RTOS assuming priority is higher. 
	
Wireless Stack: Requires large amounts of RAM and Processing Power. Also needs to respond quickly. ![image](https://user-images.githubusercontent.com/111195788/235079394-8f674eb5-a019-4cc5-9fd8-e1fe00237892.png)
