#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0; //cpu definition
#else
static const BaseType_t app_cpu = 1;
#endif

static const int led_pin1 = 2; //pin definition
static const int led_pin2 = 4;
static const int led_pin3 = 15;
void toggleLED1(void *parameter)
{
  while(1)
  {
    digitalWrite(led_pin1, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS); //not interrupting delay //500 ms = 500 ticks / 1 tick per ms
    digitalWrite(led_pin1, LOW);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void toggleLED2(void *parameter)
{
  while(1)
  {
    digitalWrite(led_pin2, HIGH);
    vTaskDelay(100/portTICK_PERIOD_MS);
    digitalWrite(led_pin2, LOW);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void toggleLED3(void *parameter)
{
  while(1)
  {
    digitalWrite(led_pin3, HIGH);
    vTaskDelay(1500/portTICK_PERIOD_MS);
    digitalWrite(led_pin3, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void setup()
{
  pinMode(led_pin1, OUTPUT); //data direction
  pinMode(led_pin2, OUTPUT); //data direction
  pinMode(led_pin3, OUTPUT); //data direction
  xTaskCreatePinnedToCore(
    toggleLED1,              //func to call
    "Toggle LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    3,                      //task priority (0,lowest - 24 highest)
    NULL,                   //Task Handler
    app_cpu);               //Run on one core for Demo Purpose

    xTaskCreatePinnedToCore(
    toggleLED2,              //func to call
    "Toggle LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    1,                      //task priority (0,lowest - 24 highest)
    NULL,                   //Task Handler
    app_cpu);

    xTaskCreatePinnedToCore(
    toggleLED3,              //func to call
    "Toggle LED",           //name of task
    1024,                   //stack size
    NULL,                   //pointer to memory
    5,                      //task priority (0,lowest - 24 highest), higher priority
    NULL,                   //Task Handler
    app_cpu);

}

void loop()
{

}
