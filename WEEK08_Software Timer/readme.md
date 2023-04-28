
One-Shot Timer:

https://youtu.be/JEjhnUGzBDI

A timer doesn't relay on using a single task thread/hardware to time but rather uses the software timers on the esp32. The one-shot timer will only run time and won't repeat.

Auto-Reload:

https://youtu.be/F2EVw-ylZYg

Opposite of a one-shot timer, but repeats/loops back some time. Similiar implementation just change the bool when creating the timer.

Software-Timer Challenge:

https://youtu.be/-JHJej-oLhA

When there is no active inputs, the led switches off. However once there is an input it will stay on. Whenever an input is called, the thread will call start the software timer(one shot) and switch it off again.


CONCLUSION:

Software timers are good, if you don't want to create a separate thread/task just to control time. It makes it easy if there are many task that rely on a single timer function you can use since it can save stack space.
