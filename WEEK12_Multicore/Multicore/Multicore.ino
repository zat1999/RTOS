
//assign 2 cores
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

static const TickType_t time_hog = 200;

//no operation, waste a cycle to create a delay.
static void hog_delay(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    for (uint32_t j = 0; j < 40000; j++) {
      asm("nop");
    }
  }
}

//task l
void taskL(void *parameters)
{
  TickType_t timestamp;
  char str[20];

    while (1) {

    sprintf(str, "Task L, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    //note notVTaskDelay will run this task not giving another task to run.
    hog_delay(time_hog);
  }
}

void taskH(void *parameters)
{
  TickType_t timestamp;
  char str[20];

    while (1) {

    sprintf(str, "Task H, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    //note notVTaskDelay will run this task not giving another task to run.
    hog_delay(time_hog);
  }

}


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Multicore");

  xTaskCreatePinnedToCore(taskL,
                          "Task L",
                          2048,
                          NULL,
                          1,
                          NULL,
                          tskNO_AFFINITY);

  xTaskCreatePinnedToCore(taskH,
                          "Task H",
                          2048,
                          NULL,
                          2,
                          NULL,
                          tskNO_AFFINITY);

//taskNO_AFFINITY, uses any core that is available.

  //delete "setup and loop" task
  vTaskDelete(NULL);



}

void loop() {
  // put your main code here, to run repeatedly:

}
