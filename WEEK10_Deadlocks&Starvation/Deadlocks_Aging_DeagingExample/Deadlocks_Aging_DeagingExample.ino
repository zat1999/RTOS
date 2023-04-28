
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//task1 has a constant priority that is higher than 2
static TaskHandle_t task1;
static TaskHandle_t task2;

static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

void Shout1(void *parameters) {

  int to_age_val = 5;
  int to_deage_val = 8;
  int i = 0;
  while (1)
  {
      xSemaphoreTake(mutex_1, portMAX_DELAY);
      xSemaphoreTake(mutex_2, portMAX_DELAY);


      Serial.println("Task 1 is Running");
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      if(i == to_age_val)
      {
        vTaskPrioritySet(task2, 3);
      }else if(i == to_deage_val)
      {
        vTaskPrioritySet(task2, 2);
        i = 0;
      }


      xSemaphoreGive(mutex_1);
      xSemaphoreGive(mutex_2);

      i++;
  }
}

void Shout2(void *parameters) {
  while (1)
  {
      
      xSemaphoreTake(mutex_2, portMAX_DELAY);

      Serial.println("Task 2 is Running");
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      xSemaphoreGive(mutex_2);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(Shout1,
                          "Shout1", 
                          1024, 
                          NULL, 
                          3, 
                          &task2, 
                          app_cpu);

  xTaskCreatePinnedToCore(Shout2,
                          "Shout2", 
                          1024, 
                          NULL, 
                          2, 
                          &task2, 
                          app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:
}
