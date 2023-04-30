#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

TickType_t cs_wait = 250;   //time spent in critical section (ms)
TickType_t med_wait = 5000; //time medium task spends working (ms)

static SemaphoreHandle_t lock;

//task L (low priority)
void doTaskL(void *parameters) {

  TickType_t timestamp;

  while (1) {

    //take lock
    Serial.println("Task L trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    //say how long we spend waiting for a lock
    Serial.print("Task L got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    //hog the processor for a while doing nothing
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ( (xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait);

    //release lock
    Serial.println("Task L releasing lock.");
    xSemaphoreGive(lock);

    //go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//task M (medium priority)
void doTaskM(void *parameters) {

  TickType_t timestamp;

  while (1) {

    //hog the processor for a while doing nothing
    Serial.println("Task M doing some work...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ( (xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < med_wait);

    //go to sleep
    Serial.println("Task M done!");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//task H (high priority)
void doTaskH(void *parameters) {

  TickType_t timestamp;
  while (1) {

    //take lock
    Serial.println("Task H trying to take lock...");
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    xSemaphoreTake(lock, portMAX_DELAY);

    //say how long we spend waiting for a lock
    Serial.print("Task H got lock. Spent ");
    Serial.print((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp);
    Serial.println(" ms waiting for lock. Doing some work...");

    //hog the processor for a while doing nothing
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ( (xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < cs_wait);

    //release lock
    Serial.println("Task H releasing lock.");
    xSemaphoreGive(lock);
    
    //go to sleep
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {

  Serial.begin(115200);

  //wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Priority Inversion Simulation");

  //create semaphores and mutexes before starting tasks, since it starts at zero.
  lock = xSemaphoreCreateMutex();
  //xSemaphoreGive(lock); // Make sure binary semaphore starts at 1 so its available

  //the order of starting the tasks matters to force priority inversion

  //start Task L (low priority)
  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  
  //introduce a delay to force priority inversion
  vTaskDelay(1 / portTICK_PERIOD_MS);

  //start Task H (high priority)
  xTaskCreatePinnedToCore(doTaskH,
                          "Task H",
                          1024,
                          NULL,
                          3,
                          NULL,
                          app_cpu);

  //start Task M (medium priority)
  xTaskCreatePinnedToCore(doTaskM,
                          "Task M",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}

void loop() {

}