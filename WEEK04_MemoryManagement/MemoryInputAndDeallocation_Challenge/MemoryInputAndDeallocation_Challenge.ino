#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
//use of cores

//declare handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//msg pointer
static char *msg_ptr = NULL;

//Thread to receive
void TaskA(void *parameter) {

  char c;                  //stores single char
  char buffer[255];        //char - 1 byte
  int b_index = 0;         //index for looping purposes
  memset(buffer, 0, 255);  //resets buffer to zero
  while (1) {


    if (Serial.available() > 0) {  //when input receive
      c = Serial.read();           //read the first byte (1st character next)

      if (b_index < 255 - 1) {  //prevent it from reading over the buffer limit
        buffer[b_index] = c;    //saves the next character to the buffer
        b_index++;              //increments counters
      }

      Serial.print(c);  //debugging purposes
      Serial.print('-');

      if (c == '\n') {               //if a new line character detected
        buffer[b_index - 1] = '\0';  //replace the last character with a null terminator to indicate thats where the string ends


        msg_ptr = (char *)pvPortMalloc(b_index * sizeof(char));  //allocates the memory
        configASSERT(msg_ptr);                                   //if malloc(above) returns zero it means out of memory and program resets
        memcpy(msg_ptr, buffer, b_index);                        //copies the buffer msg to the pointer with that correct size



        vTaskResume(task_2);  //resumes the print thread


        memset(buffer, 0, 255);  //resets the buffer and counter to zero
        b_index = 0;
      }
    }
  }
}


void TaskB(void *parameter) {
  while (1) {
    vTaskSuspend(task_1);  //suspends the terminal input
    Serial.println("Task B - Printing Message");
    Serial.println(msg_ptr);  //prints the message

    Serial.print("Free heap (bytes, BEFORE): ");
    Serial.println(xPortGetFreeHeapSize());  //gets the free memory before clearing

    //Free up Allocated Memory
    vPortFree(msg_ptr);
    msg_ptr = NULL;

    Serial.print("Free heap (bytes, AFTER): ");  //gets the free memory after clearing, always constant
    Serial.println(xPortGetFreeHeapSize());



    vTaskResume(task_1);  //suspends this thread and continues the initial thread
    vTaskSuspend(task_2);
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memmory---");

  //Starting the other Task
  xTaskCreatePinnedToCore(TaskA,
                          "ReadTerminal",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  //Prints the Message
  xTaskCreatePinnedToCore(TaskB,
                          "PrintMsg",
                          1024,
                          NULL,
                          2,  //printing has higher priority over reading
                          &task_2,
                          app_cpu);

  //deletes setup and loop task, so 1 loop is running and suspends the PrintMsg Task
  vTaskSuspend(task_2);
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
