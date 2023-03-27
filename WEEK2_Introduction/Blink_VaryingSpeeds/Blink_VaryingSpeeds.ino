#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//refers to in-built LED
static const int led_pin = 2;

void toggleLED(void *parameter)
{
  while(1)
  {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void setup()
{
  pinMode(led_pin, OUTPUT); //data direction
  xTaskCreatePinnedToCore(
    toggleLED,              //func to call
    "Toggle LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    1,                      //task priority (0,lowest - 24 highest)
    NULL,                   //Task Handler
    app_cpu);               //Run on one core for Demo Purpose


}

void loop()
{

}