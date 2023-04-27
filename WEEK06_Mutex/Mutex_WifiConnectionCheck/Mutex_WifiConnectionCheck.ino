
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#include <WiFi.h>

#define WIFI_SSID "network"
#define WIFI_PASSWORD "password"

static SemaphoreHandle_t mutex1;  //creates mutex & task handles
static SemaphoreHandle_t mutex2;
static TaskHandle_t ConnectWifiTask = NULL;
static TaskHandle_t StatusTask = NULL;

//task will attempt to reconnect to wifi
void ConnectToWIFI(void *parameters) {
  while (1) {
    while (WiFi.status() != WL_CONNECTED) {
      //display
      vTaskDelay(500 / portTICK_PERIOD_MS);
      Serial.print(".");
    }

    //releases mutex2 once connection established
    xSemaphoreGive(mutex2);

    //waits for it to be released
    xSemaphoreTake(mutex1, portMAX_DELAY);

    //Reconnects to wifi
    Serial.print("Re-Connecting with Network: ");
    WiFi.reconnect();
  }
}

//Task will run any function that requires internet in this case checks for IP of connected wifi
void StatusWIFI(void *parameters) {
  while (1) {

    //waits for it to be release
    xSemaphoreTake(mutex2, portMAX_DELAY);

    while (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected with IP: ");
      Serial.println(WiFi.localIP()); 
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      //Any program that relies on internet access goes here
    }

    //releases mutex1
    xSemaphoreGive(mutex1);
  }
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  //wifi stuff
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  //creates mutex
  mutex1 = xSemaphoreCreateMutex();
  mutex2 = xSemaphoreCreateMutex();

  xSemaphoreTake(mutex1, portMAX_DELAY);
  xSemaphoreTake(mutex2, portMAX_DELAY);


//creates task
  xTaskCreatePinnedToCore(ConnectToWIFI,
                          "ConnectToWIFI",
                          1024,
                          NULL,
                          1,
                          &ConnectWifiTask,
                          app_cpu);

  xTaskCreatePinnedToCore(StatusWIFI,
                          "StatusConnection",
                          1024,
                          NULL,
                          1,
                          &StatusTask,
                          app_cpu);
}

void loop() {
  // put your main code here, to run repeatedly:
}
