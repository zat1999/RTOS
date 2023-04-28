#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;


//Task A - higher priority
void doTaskA(void *parameters) {
  while (1) {
    //Take mutex 1 - force deadlock by adding a delay
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task A took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    //Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task A took mutex 2");

    //Critical section protected by 2 mutexes
    Serial.println("Task A working");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //releases mutexes in reverse order
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    //Wait to let the other task execute
    Serial.println("Task A sleeping");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//Task B (low priority)
void doTaskB(void *parameters) {


  while (1) {

    //Take mutex 1 - blocked deadlock due to delay
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.println("Task B took mutex 1");
    vTaskDelay(1 / portTICK_PERIOD_MS);

    //Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task B took mutex 2");

    //Critical section protected by 2 mutexes
    Serial.println("Task B working");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Give back mutexes (in reverse order that we took them)
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    //Wait to let the other task execute
    Serial.println("Task A sleeping");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}



void setup() {
  

  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Deadlock - With a Priority Solution ");

  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  //Start Task A (high priority)
  xTaskCreatePinnedToCore(doTaskA,
                          "Task A",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  //Start Task B (low priority)
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
}