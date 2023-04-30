
Prioprity Inversion:

https://youtu.be/O3GnrE-LN_U

![image](https://user-images.githubusercontent.com/111195788/235205903-b02f9002-a64e-4ee3-b00a-dd79480a2e7f.png)

Thread, Tick Graph Cycle.

Priority Inversion is when a higher priority task cannot run, as a lower priority task is in critical section and still running, and hence locks out the higher priority task. In the above graph we see, task L, priority level 1 runs and is placed into the critical section. Task H then interrupts however is locked out. The next priority level then runs. Afterwards L is able to complete, releasing its critical section. Only then the highest critical section is able to run and complete. This causes some delay and can use future problems. In order to solve, this we can increaes the priority level of the lower priority level to match so the scheduluer will cause to release its critical section/prioirty inheritance.
