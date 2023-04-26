
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//handle to reference
static TimerHandle_t timerLED = NULL;
static TaskHandle_t console_listener = NULL;
const static int ledPin = 2;



void LED_Backlight(TimerHandle_t xTimer) {

    Serial.println("One-Shot Timer Expired");
    digitalWrite(ledPin, LOW);
  
}

void ConsoleListener(void *parameter) {
  while (1) {
    if (Serial.available() > 0) {

      char cmd = Serial.read();
      Serial.print(cmd);

      digitalWrite(ledPin, HIGH);
      
      xTimerStart(timerLED, portMAX_DELAY);
    }
  }
}


void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Timer Demo:");

  timerLED = xTimerCreate(
    "Timer_LED",                //name
    5000 / portTICK_PERIOD_MS,  //period ticks or length
    pdFALSE,                    //auto-reupload, meaning expire and call it back
    (void *)0,                  //timer ID, pointer to something for unique timers
    LED_Backlight);             //callback function

  xTaskCreatePinnedToCore(
    ConsoleListener,
    "ConsoleListener",
    1024,
    NULL,
    1,
    &console_listener,
    app_cpu);


    if(timerLED == NULL )
{
  Serial.println("Timer not created");
}
}

void loop() {
}
