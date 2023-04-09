#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Settings
static const uint8_t msg_queue_len = 5;  // Max Number of items queue can hold

//Globals - so all task can access it
static QueueHandle_t msg_queue;

void printMessages(void *parameters) {
  int item;

  while (1) {
    //Parameters: HandleOfQueue, AddressOfLocalVar Where Queue is copied to, timeout of x ticks(0 = immedietely)
    //returns pDTRUE if item is received, and pDFalse else.
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE)
    {
      Serial.println(item);
    }
    //Serial.println(item); //This shows local var does not change when no new items are added
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memmory---");

  //creates the queue with desired length, size of each item 
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  xTaskCreatePinnedToCore(printMessages,
                          "printMessages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
}

void loop() {
  static int num = 0;

  //Tries to add items to queue for 10 ticks, fails if queue is full
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue is Full");
  }
  num++; //increments
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
