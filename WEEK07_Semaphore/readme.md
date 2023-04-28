
Binary Semaphore:

https://youtu.be/O9ptHEAvwvY

The same program as the mutex blink function but using a semaphore. The Binary Semaphore and Mutexes are very similiar implementation but two different functions. One is to block the other to synchronise/signal functions. Usually mutexs are constrained to its owned thread, and have sense of ownership whereas a binary sempahore there is no ownership and can be taken/given across multiple threads.

Counting Semaphore:

https://youtu.be/M_jsQDzc5x8

Instead of being limited to 0 and 1. Counting Semaphores have no limits. 0 until your defined max val. In this program we create 5 threads. It will display some text and print its length. Only when all the task is completed it signals back to the setup code that the semaphore is zero and can run the code below it.

Semaphore Challenge

https://youtube.com/shorts/mJQlxnqVGxM

In this program, we are using semaphores to signal that some task is ready to be used. And when used available it decreases a counting semaphore. Only when this semaphore is available they can carry out the task. For this challenge, the program prints out the numbers 0-4 3 times each in any order. However, it can only write/read 3 times each as that is the size of the buffer. Very similiar to a queue and can be replicated similiarly.

Conclusion:

Semaphores and Mutexes are both very similiar. They have the same implementations but are used differently. Mutexes used for locking shared resources for a particular thread/task, whilst semaphores is used for signaling. It allows a producer to say that the task is available then a consumer will take that resource to use. There are two types of semaphores, counting and binary. Binary can exist in two states 0,1, whilst counting boundless. Note it is possible to replace a semaphore by a queue. Mutexs are constrained to its owned thread, and have sense of ownership whereas a binary sempahore there is no ownership and can be taken/given across multiple threads.
