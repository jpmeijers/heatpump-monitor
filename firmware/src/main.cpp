#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>
#include "secrets.hpp"
#include "mqtt.hpp"
#include "ntp.hpp"
#include "queue.hpp"
#include "temperature.hpp"
#include "reason.hpp"
#include "webserver.hpp"
#include "xye.hpp"
#include "onewire.hpp"
#include "led.hpp"
#include "config.hpp"
#include "utils.hpp"

// variabls for blinking an LED with Millis
unsigned long tempLastReport = 0;

struct message myMessage;
StaticJsonDocument<256> doc;
TaskHandle_t taskTwoHandle;
TaskHandle_t taskThreeHandle;

// Task three
void taskThree(void *parameter)
{
  while (1)
  {
    // Report temperature
    if (millis() - tempLastReport > INTERNAL_TEMPERATURE_REPORT_PERIOD)
    {
      tempLastReport = millis();
      getTemperature();
    }
    delay(100);
  }
}

void killTaskThree()
{
  vTaskDelete(taskThreeHandle);
}

void setupTaskThree()
{
  setupTemperature();

  xTaskCreate(
      taskThree,         /* Task function. */
      "taskThree",       /* String with name of task. */
      10000,             /* Stack size in bytes. */
      NULL,              /* Parameter passed as input of the task */
      1,                 /* Priority of the task. */
      &taskThreeHandle); /* Task handle. */
}

// Task two
void taskTwo(void *parameter)
{
  while (1)
  {
    // Listen on RS485
    loopXyeOnce();
    delay(100);
  }
}

void killTaskTwo()
{
  vTaskDelete(taskTwoHandle);
}

void setupTaskTwo()
{
  setupXye();

  xTaskCreate(
      taskTwo,         /* Task function. */
      "taskTwo",       /* String with name of task. */
      10000,           /* Stack size in bytes. */
      NULL,            /* Parameter passed as input of the task */
      1,               /* Priority of the task. */
      &taskTwoHandle); /* Task handle. */
}

void setupNetwork() {
    // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution. ex http://esp32.local */
  if (!MDNS.begin(host))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

void processQueue()
{
  if (WiFi.status() == WL_CONNECTED && mqttConnected())
  {
    // Process queue
    if (xQueuePeek(queue, &myMessage, 0))
    {
      Serial.print("[Main] Sending to mqtt: ");
      Serial.print(myMessage.topic);
      Serial.print(" ");
      Serial.println(myMessage.raw);

      doc.clear();
      doc["time"] = myMessage.time;
      doc["raw"] = myMessage.raw;

      char buffer[256];
      size_t n = serializeJson(doc, buffer);

      if (mqttSendBuffer(myMessage.topic, buffer, n))
      {
        // if send was successful, we remove it from the queue
        xQueueReceive(queue, &myMessage, 0);
      }
      else
      {
        Serial.println("[Main] Send failed, leaving on queue");
      }
    }
  }
}

// Main task, aka taskOne
void loop(void)
{
  ledLoopOnce();
  processQueue();
  loopWebserverOnce();
  mqttLoopOnce();
  ntpLoopOnce();
  loopOneWireOnce();

  delay(100);
}

/* setup function */
void setup(void)
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  delay(3000);

  Serial.println("\n== Startup ==");
  
  char host[17];
  getHostname(host, sizeof(host));
  Serial.print("# Receiver ID: ");
  Serial.println(host);

  // Need to setup the queues before using them
  setupQueue();

  // Send startup reason as event
  String reason0 = get_reset_reason(get_reset_reason_code(0));
  String reason1 = get_reset_reason(get_reset_reason_code(1));
  struct message myMessage;
  myMessage.time = getTime();
  snprintf(myMessage.topic, sizeof(myMessage.topic), "/events");
  snprintf(myMessage.raw, sizeof(myMessage.raw), "system startup, %s, %s", reason0, reason1);
  xQueueSend(queue, &myMessage, portTICK_PERIOD_MS * 1000);

  setupNetwork();
  setupNtp();
  setupMqtt();
  setupWebserver();
  setupOneWire();

  setupTaskTwo();
  setupTaskThree();
}
