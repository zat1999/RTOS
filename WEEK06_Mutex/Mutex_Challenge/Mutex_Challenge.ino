
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


static const int led_pin = 2;
static SemaphoreHandle_t mutex;

//blink func
void blinkLED(void *parameters) {

  //local var from parameters passed when pinned by setup.
  int num = *(int *)parameters;
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  //increments the mutex +1
  xSemaphoreGive(mutex);
  
  Serial.print("Received: ");
  Serial.println(num);

  //data directions
  pinMode(led_pin, OUTPUT);

  //blink
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

void setup() {

  long int delay_arg;
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Enter delay in milliseconds)");


  //serial input.
  while (Serial.available() <= 0);
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);
  
  //creates the mutex
  mutex = xSemaphoreCreateMutex();
  
  //set mutex to zero(in critical section) so blocks the other code from accessing this thread
  xSemaphoreTake(mutex,portMAX_DELAY);


  //creates the task
  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          1024,
                          (void *)&delay_arg, //passing the parameters from its local var
                          1,
                          NULL,
                          app_cpu);

  //waits for mutex to be returned/increment (max delay)
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("Done!");
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}