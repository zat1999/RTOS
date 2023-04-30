
Prioprity Inversion:

https://youtu.be/O3GnrE-LN_U

![image](https://user-images.githubusercontent.com/111195788/235205903-b02f9002-a64e-4ee3-b00a-dd79480a2e7f.png)

Thread, Tick Graph Cycle.

Priority Inversion is when a higher priority task cannot run, as a lower priority task is in critical section and still running, and hence locks out the higher priority task. In the above graph we see, task L, priority level 1 runs and is placed into the critical section. Task H then interrupts however is locked out. The next priority level then runs. Afterwards L is able to complete, releasing its critical section. Only then the highest critical section is able to run and complete. This causes some delay and can use future problems. In order to solve, this we can increaes the priority level of the lower priority level to match so the scheduluer will cause to release its critical section/prioirty inheritance.


Priority Inversion Mutex Solution:

https://youtu.be/ORIAXrQiBgQ

Note in the inital program, we use binary semaphore to control the critical sections, however if we use mutexes, the rtos library has some inbuilt protection by upgrading the priority level of the lower task priority to match and then decreases it. Note that now the delay is shorter, task l runs first, completes and then task h is able to run and complete, unlike in the first program, l and h runs but does not complete, until task m finishes. 

Priority Inversion Critical Marker Solution:

https://youtu.be/EW-rniZv1yU

In this separate solution we disable the scheduler. So task L is able to run first and release the lock the lock instantly so the higher priority task can run. There is no delay. Note using spinlocks to disable the scheduler may give some undesired effects.
