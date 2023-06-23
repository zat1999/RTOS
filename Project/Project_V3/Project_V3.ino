#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <FirebaseESP32.h>


//Multicore, Deadlocks/Starvation/SoftwareTimer, Semaphores, Mutex, Task Scheduling,

#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "Nahkoda Jaring"
#define WIFI_PASSWORD "6365am94969905nias"
#define API_KEY "AIzaSyD9GHrA92344vGWI0Mlqowj7C6ISw0VxdI"
#define DATABASE_URL "https://esp32-demo-2bd1c-default-rtdb.asia-southeast1.firebasedatabase.app/"


//config to use 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define MQ3 35
double lati;
double lang;
char buf[30];

int sensorValue;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
TinyGPSPlus gps;

static SemaphoreHandle_t binSem_gps;        //handle for semaphore reference
static SemaphoreHandle_t binMutex_alcohol;  //handle for semaphore reference
static SemaphoreHandle_t binSem_timer;      //handle for semaphore reference
static TimerHandle_t one_shot_timer = NULL;

//Timer
void myTimerCallback(TimerHandle_t xTimer) {
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
    Serial.println("OST-Timer: Heating Element & WIFI/Firebase Connection");
    xSemaphoreGive(binSem_timer); //signals to allow the other threads to run;
  }
}

void AlcoholSensor(void *parameter) {
  while (1) {
    //unlocks the thread.
    xSemaphoreGive(binMutex_alcohol);

    //gets and reads the value
    sensorValue = analogRead(MQ3);
    //Debugging
    //Serial.print("Sensor Val: ");
    //Serial.print(sensorValue);
    if (sensorValue > 700) {
      //alcohol concentration high, detected
      Serial.println(" --- Alcohol Detected");
      xSemaphoreGive(binSem_gps);                       //via semaphore unlocks the task thread below it.
      xSemaphoreTake(binMutex_alcohol, portMAX_DELAY);  //locks the threads
    } else {
      //Serial.println("--- No Alcohol Detected");
      //alcohol concentration low/not detected
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    xSemaphoreTake(binMutex_alcohol, portMAX_DELAY);    //resets the signal/semaphore
  }
}

void GPS_Receiver(void *parameter) {
  while (1) {
    Serial.println("GPS Thread Waiting");
    //will wait until semaphore is free.
    xSemaphoreTake(binSem_gps, portMAX_DELAY);
    Serial.println("Starting");
    //Reads from the RT Pins
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {
        lati = gps.location.lat();
        lang = gps.location.lng();        
        Serial.print(sprintf(buf, "Location: %d,%d",lati,lang));
        //Paths for the 
        String path1 = "/test/latitude";
        String path2 = "/test/longtitude";
        String timePath = "/test/time";
        String datePath = "/test/date";
        //Writes to Firebase
        Firebase.setFloat(fbdo, path1, lati);
        Firebase.setFloat(fbdo, path2, lang);
        sprintf(buf, "%d:%d",gps.time.hour(),gps.time.minute());
        Firebase.setString(fbdo, timePath, buf);
        sprintf(buf, "%d:%d",gps.date.day(),gps.date.month());
        Firebase.setString(fbdo, datePath, buf);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }}
    //Timeout
    if (millis() > 5000 && gps.charsProcessed() < 10) {
      Serial.println(F("No GPS detected: check wiring."));
      while (true)
        ;
    }
    xSemaphoreGive(binSem_gps);
  }
}

void setup() {
  Serial.begin(115200);  // sets the serial port to 9600
  Serial2.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  

  binSem_timer = xSemaphoreCreateBinary();  //Creates the Semaphore, default is zero.

  //Wait 30s for Alcohol Sensor to heat up, wifi to connect, etc.
  one_shot_timer = xTimerCreate(
    "OneShotTimer",              //name
    30000 / portTICK_PERIOD_MS,  //period ticks or length
    pdFALSE,                     //auto-reupload, meaning expire and call it back
    (void *)0,                   //timer ID, pointer to something for unique timers
    myTimerCallback);            //callback function

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("---------------------------------");
  Serial.println("Starting Program:");

  xTimerStart(one_shot_timer, portMAX_DELAY); //calls the timer

  //vTaskDelay(10000 / portTICK_PERIOD_MS);

  xSemaphoreTake(binSem_timer, portMAX_DELAY);

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);

  Serial.print("Sign up new user... ");

  //anonymouse sign in
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Signed In");
  } else {
    //displays error msg
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  binSem_gps = xSemaphoreCreateBinary();       //Creates the Semaphore, default is zero.
  binMutex_alcohol = xSemaphoreCreateMutex();  //Creates the Semaphore, default is zero.
  pinMode(MQ3, INPUT);                         //Data direction

  xTaskCreatePinnedToCore(
    GPS_Receiver,    //Func
    "GPS Receiver",  //Name
    8192,            //Stack Depth
    NULL,
    1,         //Priority
    NULL,      //Handle
    app_cpu);  //Core

  xTaskCreatePinnedToCore(
    AlcoholSensor,     //func
    "Alcohol Sensor",  //name
    1024,              //stack
    NULL,
    1,         //priority
    NULL,      //handle null
    app_cpu);  //core

}

void loop() {
  // put your main code here, to run repeatedly:
}
