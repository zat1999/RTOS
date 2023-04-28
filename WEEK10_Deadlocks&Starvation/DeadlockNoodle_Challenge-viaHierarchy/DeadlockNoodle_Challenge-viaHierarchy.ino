// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

enum { NUM_TASKS = 5 };           // Number of tasks (philosophers)
enum { TASK_STACK_SIZE = 2048 };  // Bytes in ESP32, words in vanilla FreeRTOS

static SemaphoreHandle_t bin_sem;   // Wait for parameters to be read
static SemaphoreHandle_t done_sem;  // Notifies main task when done
static SemaphoreHandle_t chopstick[NUM_TASKS];


// The only task: eating
void eat(void *parameters) {

  int num;
  char buf[50];
  int prio1;
  int prio2;

  //Copy parameter and increment semaphore count
  num = *(int *)parameters;
  xSemaphoreGive(bin_sem);


  //uses the chopstick based on the philospher number and the chopstick adjacent to them.
  if (num > (num + 1) % NUM_TASKS) {
    prio1 = num;
    prio2 = (num + 1) % NUM_TASKS;
  } else {
    prio1 = (num + 1) % NUM_TASKS;
    prio2 = num;
  }

  //Take left chopstick
  xSemaphoreTake(chopstick[prio1], portMAX_DELAY);
  sprintf(buf, "Philosopher %i took chopsticks %i", num, prio1);
  Serial.println(buf);

  //Add some delay to force deadlock
  vTaskDelay(1 / portTICK_PERIOD_MS);

  //Take right chopstick
  xSemaphoreTake(chopstick[prio2], portMAX_DELAY);
  sprintf(buf, "Philosopher %i took chopstick %i", num, prio2);
  Serial.println(buf);

  //Do some eating
  sprintf(buf, "Philosopher %i is eating", num);
  Serial.println(buf);
  vTaskDelay(1 / portTICK_PERIOD_MS);

  //Put down right chopstick
  xSemaphoreGive(chopstick[prio2]);
  sprintf(buf, "Philosopher %i returned chopstick %i", num, prio2);
  Serial.println(buf);

  //Put down left chopstick
  xSemaphoreGive(chopstick[prio1]);
  sprintf(buf, "Philosopher %i returned chopstick %i", num, prio1);
  Serial.println(buf);

  xSemaphoreGive(done_sem);
  vTaskDelete(NULL);
}

//core 1 prio 1
void setup() {

  char task_name[20];

  Serial.begin(115200);

  //wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Dining Philosophers Challenge");

  //create kernel objects before starting tasks
  bin_sem = xSemaphoreCreateBinary();
  done_sem = xSemaphoreCreateCounting(NUM_TASKS, 0);
  for (int i = 0; i < NUM_TASKS; i++) {
    chopstick[i] = xSemaphoreCreateMutex();
  }


  //have the philosphers start eating
  for (int i = 0; i < NUM_TASKS; i++) {
    sprintf(task_name, "Philosopher %i", i);
    xTaskCreatePinnedToCore(eat,
                            task_name,
                            TASK_STACK_SIZE,
                            (void *)&i,
                            1,
                            NULL,
                            app_cpu);
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }


  //wait until all the philosophers are done
  for (int i = 0; i < NUM_TASKS; i++) {
    xSemaphoreTake(done_sem, portMAX_DELAY);
  }

  //say that we made it through without deadlock
  Serial.println("Done! No deadlock occurred!");
}

void loop() {
  // Do nothing in this task
}