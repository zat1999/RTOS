
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




  //copies parameters into local var
  int num = *(int *)parameters;
  
  //increments the mutex
  xSemaphoreGive(mutex);
  
  
  
  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  //data directions
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
  
  // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
