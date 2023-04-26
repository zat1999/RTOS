
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


static const int led_pin = 2;
static SemaphoreHandle_t mutex;

// Blink LED based on rate passed by parameter
void blinkLED(void *parameters) {




  // Copy the parameter into a local variable
  int num = *(int *)parameters;
  xSemaphoreGive(mutex);
  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  // Configure the LED pin
  pinMode(led_pin, OUTPUT);

  // Blink forever and ever
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}


// Main (runs as its own task with priority 1 on core 1)
void setup() {

  long int delay_arg;

  // Configure Serial
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Enter delay in milliseconds)");


  // Wait for input from Serial
  while (Serial.available() <= 0);

  // Read integer value
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);

    mutex = xSemaphoreCreateMutex();
  xSemaphoreTake(mutex,portMAX_DELAY);
  //alt use portMAX_DELAY runs indefinetly


  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          1024,
                          (void *)&delay_arg,
                          1,
                          NULL,
                          app_cpu);

  //waits for mutex to be turned/increment (max delay)
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("Done!");
}

void loop() {
  
  // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}