#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
static TaskHandle_t task_1 = NULL;

void testTask(void *parameter) {


  while (1) {
    int a = 1;
    int b[100];  //4 bytes, stores numbers in an array (int = 4 bytes)

    for (int i = 0; i < 100; i++) {
      b[i] = a + 1;
    }
    Serial.println(b[0]);

    Serial.print("High Water Mark (words): ");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));  //how many bytes left in stack

    Serial.print("Heap Before Malloc(bytes): ");
    Serial.println(xPortGetFreeHeapSize());  //how much free heap memory bytes

    int *ptr = (int *)pvPortMalloc(1024 * sizeof(int));

    //Check if there is any heap memory in stack
    if (ptr == NULL) {
      Serial.println("Not Enough Heap.");
    } else {
      //Do something with memory so not optimized
      for (int i = 0; i < 1024; i++) {
        ptr[i] = 3;
      }
    }

    Serial.print("Heap After Malloc(bytes): ");
    Serial.println(xPortGetFreeHeapSize());  //how much free heap memory bytes

    // Free up Allocated Memory
    //vPortFree(ptr);

    vTaskDelay(100 / portTICK_PERIOD_MS);
    //vTaskSuspend(task_1);
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memmory---");

  //Starting the other Task
  xTaskCreatePinnedToCore(testTask,
                          "Test Task",
                          1500,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  //deletes setup and loop task, so 1 loop is running
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
