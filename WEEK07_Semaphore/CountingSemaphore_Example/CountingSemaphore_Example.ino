//config to use 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Semaphores -
//Better used to control access to a shared device between task/threads. (Single it)
//It allows to put to different task into the critical section. Similiar to a mutex.
//Semaphores are commonly used to synchronise threads instead. Similiar to a Queue


//Mutex
/*
Implies ownership during a critical section ( 1 key)
priority inheritance - prevents higher priority task from being being blocked for a long time
*/

//Semaphore
/*
No ownership, but rather signals to task some shared resourse is ready to consume/read.
1 task increases semaphore val, 1 task decreases.
Good for ISR
*/

static const int num_task = 5;  //num of task to create

typedef struct Message {
  char body[20];
  uint8_t len;
} Message;

static SemaphoreHandle_t sem_params;  //handle for semaphore reference


void myTask(void *parameters) {
  Message msg = *(Message *)parameters;  //copy the message struct from the parater to local

 
  
  Serial.print("Received: ");
  Serial.print(msg.body);
  Serial.print(" ---- Length: ");
  Serial.println(msg.len);

  //decreases by 1
   xSemaphoreGive(sem_params);
  
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}


void setup() {

  char task_name[12];
  Message msg;
  char text[20] = "Eggs";

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Setup Code");

  //creates semaphore and initialize at zero, max count is num_task (5)
  sem_params = xSemaphoreCreateCounting(num_task, 0);

  strcpy(msg.body, text);
  msg.len = strlen(text);

  //creating 5 threads
  for (int i = 0; i < num_task; i++) {
    sprintf(task_name, "Task %i", i);
    xTaskCreatePinnedToCore(myTask,
                            task_name,
                            1024,
                            (void *)&msg,
                            1,
                            NULL,
                            app_cpu);
  }

  //checks if all task are done
  for (int i = 0; i < num_task; i++) {
    xSemaphoreTake(sem_params, portMAX_DELAY);
  }

  Serial.println("All Task Created");
}

void loop() {
  //allows for lower-priority task to run once the task created
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
