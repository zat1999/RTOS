
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

static const uint32_t task_0_delay = 500; //delay for task 0
static const int ledPIN = 2; 
static SemaphoreHandle_t bin_sem;

void doTask0(void *parameters) {

  pinMode(ledPIN, OUTPUT);

  while (1) {

    //release
    xSemaphoreGive(bin_sem);
    
    vTaskDelay(task_0_delay / portTICK_PERIOD_MS);
  }
}

void doTask1(void *parameters) {

  while (1) {

    //wait for semaphore
    xSemaphoreTake(bin_sem, portMAX_DELAY);

    digitalWrite(ledPIN, !digitalRead(ledPIN));
  }
}


void setup() {
    bin_sem = xSemaphoreCreateBinary();
  
  //task 0 in core 0
  xTaskCreatePinnedToCore(doTask0,
                          "Task 0",
                          1024,
                          NULL,
                          1,
                          NULL,
                          pro_cpu);

  //task 1 in core 1
  xTaskCreatePinnedToCore(doTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
