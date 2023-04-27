
//Note default queue size is 10. stack depth is 2kilobytes


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//handle to reference
static TimerHandle_t one_shot_timer = NULL;

static TimerHandle_t auto_reload_timer = NULL;
//calls a function after some time (calls it once)

//callback function
//timer handle to identify which timer called the function if multiple timers use the same callback func
void myTimerCallback(TimerHandle_t xTimer) {
  
  if((uint32_t)pvTimerGetTimerID(xTimer)==0)
  {
    Serial.println("One-Shot Timer Expired");
  }
    if((uint32_t)pvTimerGetTimerID(xTimer)==1)
  {
    Serial.println("Auto-Reload Timer Expired");
  }

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Timer Demo:");

    one_shot_timer = xTimerCreate(
    "OneShotTimer",             //name
    2000 / portTICK_PERIOD_MS,  //period ticks or length 
    pdFALSE,                    //auto-reupload, meaning expire and call it back
    (void *)0,                  //timer ID, pointer to something for unique timers
    myTimerCallback);           //callback function


  auto_reload_timer = xTimerCreate(
    "Auto Reload Timer",             //name
    3000 / portTICK_PERIOD_MS,  //period ticks or length 
    pdTRUE,                    //auto-reupload, meaning expire and call it back
    (void *)1,                  //timer ID, pointer to something for unique timers
    myTimerCallback);           //callback function

if(one_shot_timer == NULL || auto_reload_timer == NULL)
{
  Serial.println("Timer not created");
}else
{
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("Starting Timer");

  //start the timer, max block timer if command queue is full.
  xTimerStart(one_shot_timer, portMAX_DELAY);
  xTimerStart(auto_reload_timer, portMAX_DELAY);

}

  //delete self task to show that timer will work with no user task
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:
}
