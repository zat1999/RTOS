#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <FirebaseESP32.h>


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

#define MQ3 34

double lati;
double lang;
int sensorValue;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


TinyGPSPlus gps;

static SemaphoreHandle_t binSem_gps;        //handle for semaphore reference
static SemaphoreHandle_t binMutex_alcohol;  //handle for semaphore reference
static TimerHandle_t auto_reload_timer = NULL;

void AlcoholSensor(void *parameter) {

  while (1) {

    //creates mutex to lock this thread.
    xSemaphoreGive(binMutex_alcohol);

    sensorValue = analogRead(MQ3);
    Serial.print("Sensor Val: ");
    Serial.print(sensorValue);


    if (sensorValue > 185) {
      //alcohol concentration high, detected
      Serial.println(" --- Alcohol Detected");
      xSemaphoreGive(binSem_gps);
      xSemaphoreTake(binMutex_alcohol, portMAX_DELAY);
    } else {
      Serial.println("--- No Alcohol Detected");
      //alcohol concentration low/not detected
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    xSemaphoreTake(binMutex_alcohol, portMAX_DELAY);
  }
}

void GPS_Receiver(void *parameter) {
  while (1) {

    //will wait until semaphore is free.
    xSemaphoreTake(binSem_gps, portMAX_DELAY);

    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {



        lati = gps.location.lat();
        lang = gps.location.lng();

        Serial.print(F("Location: "));

        Serial.print(lati);
        Serial.print(F(","));
        Serial.print(lang);
        Serial.println();

        //String path = auth.token.uid.c_str();
        String path1 = "/test/latitude";
        String path2 = "/test/longtitude";

        Firebase.setFloat(fbdo, path1, lati);
        Firebase.setFloat(fbdo, path2, lang);

        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
    }

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
  //Wait 20s for Alcohol Sensor to heat up, wifi to connect, etc.
  Serial.println("Starting Program:");
  vTaskDelay(10000 / portTICK_PERIOD_MS);

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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
    GPS_Receiver,    //func
    "GPS Receiver",  //name
    8000,            //memory
    NULL,
    1,         //priority
    NULL,      //handle null
    app_cpu);  //core

  xTaskCreatePinnedToCore(
    AlcoholSensor,     //func
    "Alcohol Sensor",  //name
    1024,              //memory
    NULL,
    1,         //priority
    NULL,      //handle null
    app_cpu);  //core
}

void loop() {
  // put your main code here, to run repeatedly:
}
