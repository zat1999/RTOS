#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

enum {BUF_SIZE = 5};                  //size of buffer array
static const int num_prod_tasks = 5;  //num of producer tasks
static const int num_cons_tasks = 2;  //num of consumer tasks
static const int num_writes = 3;      //num times each producer writes to buf

//global var
static int buf[BUF_SIZE];             //shared buffer
static int head = 0;                  //writing index to buffer
static int tail = 0;                  //reading index to buffer
static SemaphoreHandle_t bin_sem;     //waits for parameter to be read

static SemaphoreHandle_t mutex;       //lock access to buffer and Serial
static SemaphoreHandle_t empty_semphore;   //counts number of empty slots in buf
static SemaphoreHandle_t filling_semaphore;  //counts number of filled slots in buf

//writes to buffers
void producer(void *parameters) {

  int num = *(int *)parameters;

  //releases binary sem
  xSemaphoreGive(bin_sem);

  //writes to buffers
  for (int i = 0; i < num_writes; i++) {

    //decrease by 1
    xSemaphoreTake(empty_semphore
  
, portMAX_DELAY);
    //locks the mutex
    xSemaphoreTake(mutex, portMAX_DELAY);

    //critical section (accessing shared buffer)
    buf[head] = num;
    head = (head + 1) % BUF_SIZE;

    //releases mutex
    xSemaphoreGive(mutex);

    //increses by 1
    xSemaphoreGive(filling_semaphore);
  }

  //delete self task
  vTaskDelete(NULL);
}


//consumer- reads from buffers
void consumer(void *parameters) {

  int val;

  //read from buffer
  while (1) {

    xSemaphoreTake(filling_semaphore, portMAX_DELAY);
    xSemaphoreTake(mutex, portMAX_DELAY);

    //critical section (accessing shared buffer and Serial)
    val = buf[tail];
    tail = (tail + 1) % BUF_SIZE;
    Serial.println(val);

    xSemaphoreGive(mutex);
    xSemaphoreGive(empty_semphore
  
);
  }
}

void setup() {

  char task_name[14];
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Semaphore Challenge");

  //create mutexes and semaphores before starting tasks


  bin_sem = xSemaphoreCreateBinary();
  mutex = xSemaphoreCreateMutex();
  
  //semaphore size 5, initial at 5
  empty_semphore
 = xSemaphoreCreateCounting(BUF_SIZE, BUF_SIZE);
  //semaphore size 5, initial at 0
  filling_semaphore = xSemaphoreCreateCounting(BUF_SIZE, 0);

  //creates all producer task + wait for each to read argument
  for (int i = 0; i < num_prod_tasks; i++) {
    sprintf(task_name, "Producer %i", i);
    xTaskCreatePinnedToCore(producer,
                            task_name,
                            1024,
                            (void *)&i,
                            1,
                            NULL,
                            app_cpu);
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }

  //creates all consumer task
  for (int i = 0; i < num_cons_tasks; i++) {
    sprintf(task_name, "Consumer %i", i);
    xTaskCreatePinnedToCore(consumer,
                            task_name,
                            1024,
                            NULL,
                            1,
                            NULL,
                            app_cpu);
  }
  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("All tasks created");
  xSemaphoreGive(mutex);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}


