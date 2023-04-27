#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//refers to in-built LED
static const int led_pin = 2;
static const int buttonPin = 4;
int var;
int loopN = 0; //will blink the LED 5 times before stopping. Will have to press the button to reset it
void toggleLED(void *parameter)
{
  while(1)
  {
      while(loopN < 5) //will blink 5 times
      {
              digitalWrite(led_pin, HIGH);
              vTaskDelay(500/portTICK_PERIOD_MS);
              digitalWrite(led_pin, LOW);
              vTaskDelay(500/portTICK_PERIOD_MS);
              Serial.println(loopN);
              loopN++; //increment counter
      } 

    vTaskSuspend(task_1); //stops the task
  }
}

void resumeLED(void *parameter)
{
  while(1)
  {
    var = digitalRead(buttonPin);
    if(!var)
    {
        loopN = 0;            //resets the loop
        vTaskResume(task_1);  //resume the task_1 if it was suspended    
    }
    //Serial.println(var);
  }
}


void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT); //data direction
  pinMode(buttonPin, INPUT_PULLUP);

  xTaskCreatePinnedToCore(
    toggleLED,              //func to call
    "Toggle LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    1,                      //task priority (0,lowest - 24 highest)
    &task_1,                //Task Handler
    app_cpu);               //Run on one core for Demo Purpose

    xTaskCreatePinnedToCore(
    resumeLED,              //func to call
    "Resume LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    1,                      //task priority (0,lowest - 24 highest)
    &task_2,                //Task Handler
    app_cpu);               //Run on one core for Demo Purpose



}

void loop()
{
}
