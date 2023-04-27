//config to use 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[] = "Test String. Test String. Test String. Test String. ";

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//counts the number of characters in the string and prints each character one by one.
void startTask1(void *parameter)
{
	int msg_len = strlen(msg);

	while(1)
  {
	  Serial.println();
    for(int i = 0; i < msg_len; i++)
    {
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000/portTICK_PERIOD_MS); //then enters the block state for 1 second.
  }
}

//second task to print * every 100ms. also this task has higher priority so we can see the task being called priot the previous task
void startTask2(void *parameter)
{
  while(1)
  {
    Serial.print("*");
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(300); //slower so easier to see. 
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Demo");

  Serial.print("Setup and Loop Task Running on Core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with Priority");
  Serial.println(uxTaskPriorityGet(NULL));

  xTaskCreatePinnedToCore(startTask1,
  "Task 1",
  1024,
  NULL,
  2,
  &task_1, //assigning handles
  app_cpu);

  xTaskCreatePinnedToCore(startTask2,
  "Task 2",
  1024,
  NULL,
  3, //Higher Priority
  &task_2, //assigning handles
  app_cpu);
}

void loop()
{
  //will suspend and resume the task2 every two seconds
  for(int i = 0; i < 3; i++)
  {
    vTaskSuspend(task_2); //task2 enters the block state so cannot enter the ready state
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(task_2);  //task2 can now enter the running state
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }

  if (task_1 != NULL)
  {
    vTaskDelete(task_1);  //will remove the first task. 
    task_1 = NULL;        //to avoid any errors
  }
}
