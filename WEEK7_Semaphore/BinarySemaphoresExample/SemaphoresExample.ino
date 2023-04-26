//config to use 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Semaphores -
//Better used to control access to a shared device between task/threads. (Single it)
//It allows to put to different task into the critical section. Similiar to a mutex.
//Semaphores are commonly used to synchronise threads instead. Similiar to a Queue


//Mutex
/*
Implies ownership during a critical section ( 1 key)
priority inheritance - prevents higher priority task from being being blocked for a long time
*/

//Semaphore
/*
No ownership, but rather signals to task some shared resourse is ready to consume/read.
1 task increases semaphore val, 1 task decreases.
Good for ISR
*/

static const int led_pin = 2;      //LED pin
static SemaphoreHandle_t bin_sem;  //handle for semaphore reference

void blinkLED(void *parameters) {

  int num = *(int *)parameters;
  Serial.print("Received: ");
  Serial.println(num);

  xSemaphoreGive(bin_sem);  //Adds 1 to the semaphore obj

  pinMode(led_pin, OUTPUT);  //Data direction

  while (1) {  //Blink
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

void setup() {
  // put your setup code here, to run once:
  long int delay_arg;

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Mutex Solutions");
  Serial.println("Enter a number for a delay (milliseconds");

  while (Serial.available() <= 0) {  //Waits for an input
  };

  delay_arg = Serial.parseInt();  //Converts to an Int
  Serial.print("Sending: ");
  Serial.println(delay_arg);

  //will be initilize to zero, therefore don't have to take before creating the task
  bin_sem = xSemaphoreCreateBinary();  //Creates the Semaphore

  xTaskCreatePinnedToCore(
    blinkLED,  //func
    "Task 1",  //name
    1024,      //memory
    (void *)&delay_arg,
    1,         //priority
    NULL,      //handle null
    app_cpu);  //core

  //Do nothing until binary semaphore has been returned/parameter has been read.
  xSemaphoreTake(bin_sem, portMAX_DELAY);
  Serial.println("Done Pinning");
}

void loop() {
  // put your main code here, to run repeatedly:
}
