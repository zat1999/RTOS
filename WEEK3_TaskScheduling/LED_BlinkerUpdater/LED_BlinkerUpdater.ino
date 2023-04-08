//config to use 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


static const int led_pin = 2;
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//default values
int delayVal = 1000;

//responsible for blinking
void blink(void *parameter)
{
  while(1)
  {
    //Serial.println(delay);
    digitalWrite(led_pin, HIGH);
    vTaskDelay(delayVal/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(delayVal/portTICK_PERIOD_MS);
    Serial.println(delayVal);
     
  }
    
}

void getDelay(void *parameter)
{
  while(1)
  {
      if(Serial.available() > 0)
      {
          delayVal = Serial.parseInt();
      }

  }
    
}
void setup()
{
  Serial.begin(300);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("To Update Blinking Rate, Enter the New Rate in Milliseconds into the Terminal");
  pinMode(led_pin, OUTPUT);

  xTaskCreatePinnedToCore(
  blink,
  "Task 1",
  1024,
  NULL,
  1,        //same priority levels so sceduler will cycle between these two task and prevent lock up.
  &task_1,  //assigning handles
  app_cpu);

  xTaskCreatePinnedToCore(
  getDelay,
  "Task 2",
  1024,
  NULL,
  1,
  &task_2, //assigning handles
  app_cpu);
}


void loop()
{



}