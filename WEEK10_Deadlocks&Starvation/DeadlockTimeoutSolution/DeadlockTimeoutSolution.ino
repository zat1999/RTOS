#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

//settings
TickType_t mutex_timeout = 1000 / portTICK_PERIOD_MS;

//globals
static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;


//task A (high priority)
void doTaskA(void *parameters) {

  while (1) {

    //take mutex 1, if passed successful runs the code below, else wait 1second before running else cond
    if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {

      //say we took mutex 1 and wait (to force deadlock)
      Serial.println("Task A took mutex 1");
      vTaskDelay(1 / portTICK_PERIOD_MS);
  
      //take mutex 2
      if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {

        //say we took mutex 2
        Serial.println("Task A took mutex 2");
  
        //critical section protected by 2 mutexes
        Serial.println("Task A running");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task A timed out waiting for mutex 2");
      }
    } else {
      Serial.println("Task A timed out waiting for mutex 1");
    }

    // Give back mutexes
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task A sleeping");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//task B (low priority)
void doTaskB(void *parameters) {

  // Loop forever
  while (1) {

    //take mutex 2
    if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {

      //took mutex 2 and wait (to force deadlock)
      Serial.println("Task B took mutex 2");
      vTaskDelay(1 / portTICK_PERIOD_MS);
  
      //take mutex 1
      if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {

        //say we took mutex 1
        Serial.println("Task B took mutex 1");
  
        //critical section protected by 2 mutexes
        Serial.println("Task B running");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task B timed out waiting for mutex 1");
      }
    } else {
      Serial.println("Task B timed out waiting for mutex 2");
    }

    //give back mutexes
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);

    //wait to let the other task execute
    Serial.println("Task B sleeping");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  
  // Configure Serial
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Deadlock Solution with a Timeout");

  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(doTaskA,
                          "Task A",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  // Start Task B (low priority)
  xTaskCreatePinnedToCore(doTaskB,
                          "Task B",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);


  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}