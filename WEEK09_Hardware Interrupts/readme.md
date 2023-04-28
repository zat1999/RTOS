
Simple Blink:

https://youtube.com/shorts/qwoW8ZC9SqU?feature=share

This blinks every 1s based on the processors Hz. It may very depending on the cpu.


IR Sensor Interrupt:



Hardware Challenge Interrupt:




Conclusion:
Hardware timers have a higher priority than software timers and any other thread so its good for ISRs. It is also noted that hardware timers have a greater precision compared to a software timer. 
