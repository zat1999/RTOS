
//Uses 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static int shared_var = 0;
static SemaphoreHandle_t mutex;

void incTask(void *parameters)
{
  int local_var;

  while(1)
  {

    //checks if the thread is available, no wait time, and if successful posted
    if(xSemaphoreTake(mutex,0) == pdTRUE)
    {
    
    //if mutex is taken carries out this thread and also decreases its count to zero. Therefore another thread using this cannot use function/shared resource cannot run it
    local_var = shared_var;
    local_var++;
    vTaskDelay(random(100,500) / portTICK_PERIOD_MS);
    shared_var = local_var;

    //releases lock
    xSemaphoreGive(mutex);

    Serial.println(shared_var);
    }else
    {
      // will do a different task if condition fails, whilst waiting 
    }
  } 
}


void setup() {
  // put your setup code here, to run once:
randomSeed(analogRead(0));
Serial.begin(115200);

vTaskDelay(1000/portTICK_PERIOD_MS);
  
  //creates mutex
mutex = xSemaphoreCreateMutex();
  
  //creates two threads
xTaskCreatePinnedToCore(
  incTask,
  "Task 1",
  1024,
  NULL,
  1,
  NULL, //assigning handles
  app_cpu);

  
xTaskCreatePinnedToCore(
  incTask,
  "Task 2",
  1024,
  NULL,
  1,
  NULL, //assigning handles
  app_cpu);

  //deletes start & loop
vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
