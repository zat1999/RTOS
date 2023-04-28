
1. Mutex Example:

https://www.youtube.com/watch?v=kgWrPxjKYZI

In this program we have two threads runnings the same function, that shares a common resource. In the function it increments a global variable(the shared resource). Without the mutex, both function will simultaneoutly and take the current value of the global variable and increment it. Hence causing issues of that the variable is being increment and displayed multiple times for the same value. 

However, with a mutex, the first thread will check if the resource is available if so it will decrease it to zero(locking it) so another thread cannot access it and then carry out with the function method, incrementing it and displaying. Afterwards it will release the mutex. The scheduler then tries run the second thread and sees that the since the mutex is now available(1) it allows it to take the global variable and carry out the function. 


2. Mutex Wifi Connection Check

https://www.youtube.com/watch?v=PZ-zRzXGD44

This program has two threads. One to connect/reconnect to wifi. And other to carry out a function when there is a connection in this case displaying the ip. In setup, we decrease the 2 mutexes to zero so its locked. Thread 1, tries to connect to wifi. Thread 2 is block by mutex2 so cannot run, also meaning that there is no wifi and won't run the thread at all. Once thread 1 connects to wifi it releases mutex2 and then gets block indefinetly by mutex1. Since mutex 2 is released, thread 2 can continue can print the ipconfig. If there is a disconnect/error for the wifi, it locks mutex2 (blocks thread2) and releases mutex1 so thread 1 can reconnect.

3. Mutex Challenge

https://www.youtube.com/watch?v=zWn-xiQqUQE

This program has 1 single thread responsible for blinking. In setup it wait for the user to input a number for a delay to blink a LED. When passing passing parameters and pinning a task to the core, the value in the serial may not be fully initialized, so we a mutex is necessary for the parameter to be properly passed to the function.


Conclusion: Mutexes are used as locking mechanism to ensure that a shared resource is only used by 1 thread if multiple threads are using the same share resource in a critical section.  Semaphores are similiar to mutexes, however the function is different, and used more towards signaling. 
