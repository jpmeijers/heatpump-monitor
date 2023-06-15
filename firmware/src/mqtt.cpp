#include <stdint.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "secrets.hpp"
#include "mqtt.hpp"
#include "queue.hpp"
#include "ntp.hpp"
#include "utils.hpp"
#include "wifiFix.h"

WiFiClient *wifiClient = new WiFiClientFixed();
PubSubClient mqttClient(*wifiClient);

// placeholder topics to get lengths correct
char topicEvents[] = "/mac-112233445566/events";
char topicLogs[] = "/mac-112233445566/logs";
char topicConfig[] = "/mac-112233445566/config/#";
char topicMacPrefix[] = "/mac-112233445566";

void macToTopics()
{
  char host[17];
  getHostname(host, sizeof(host));

  snprintf(topicMacPrefix, sizeof(topicMacPrefix), "/%s", host);
  snprintf(topicEvents, sizeof(topicEvents), "%s/events", topicMacPrefix);
  snprintf(topicLogs, sizeof(topicLogs), "%s/logs", topicMacPrefix);
  snprintf(topicConfig, sizeof(topicConfig), "%s/config/#", topicMacPrefix);
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("MQTT: Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // TODO do something with the received message
}

void mqttReconnect()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!mqttClient.connected())
    {
      Serial.println("[MQTT] Reconnecting");

      // Attempt to connect
      if (mqttClient.connect(WiFi.macAddress().c_str()))
      {
        Serial.println("[MQTT] connected");
        // Once connected, publish an announcement...
        boolean result = mqttClient.publish(topicEvents, "mqtt connected");
        if (!result)
        {
          Serial.println("[MQTT] Failed to publish");
          Serial.print("failed, rc=");
          Serial.print(mqttClient.state());
        }

        // (Re)subscribe
        result = mqttClient.subscribe(topicConfig);
        if (!result)
        {
          Serial.println("[MQTT] Failed to subscribe");
          Serial.print("failed, rc=");
          Serial.print(mqttClient.state());
        }
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
      }
    }
  }
}

void setupMqtt()
{
  macToTopics();
  mqttClient.setServer(mqtt_host, mqtt_port);
  mqttClient.setCallback(mqttCallback);
}

void mqttCheckState()
{
  static int previousState = MQTT_DISCONNECTED;
  int state = mqttClient.state();
  // if (state != 0 && state != -1) // ignore connected and disconnected
  if (state != previousState)
  {
    struct message myMessage;
    myMessage.time = getTime();
    snprintf(myMessage.topic, sizeof(myMessage.topic), "/events");

    switch (mqttClient.state())
    {
    case -4:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECTION_TIMEOUT");
      break; // - the server didn't respond within the keepalive time
    case -3:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECTION_LOST");
      break; // - the network connection was broken
    case -2:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_FAILED");
      break; // - the network connection failed
    case -1:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_DISCONNECTED");
      break; // - the client is disconnected cleanly
    case 0:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECTED");
      break; // - the client is connected
    case 1:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_BAD_PROTOCOL");
      break; // - the server doesn't support the requested version of MQTT
    case 2:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_BAD_CLIENT_ID");
      break; // - the server rejected the client identifier
    case 3:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_UNAVAILABLE");
      break; // - the server was unable to accept the connection
    case 4:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_BAD_CREDENTIALS");
      break; // - the username/password were rejected
    case 5:
      snprintf(myMessage.raw, sizeof(myMessage.raw), "MQTT_CONNECT_UNAUTHORIZED");
      break; // - the client was not authorized to connect
    }

    Serial.print("[MQTT] State: ");
    Serial.println(myMessage.raw);

    xQueueSend(queue, &myMessage, portTICK_PERIOD_MS * 1000);
  }
  previousState = state;
}

void mqttLoopOnce()
{
  mqttClient.loop();
  mqttCheckState();
  mqttReconnect();
}

void mqttLoop(void *pvParameters)
{
  while (1)
  {
    delay(100); // https://github.com/knolleary/pubsubclient/issues/795#issuecomment-733740481
    mqttLoopOnce();
  }
}

boolean mqttSendBuffer(char *topicSuffix, char *buffer, size_t length)
{
  char topic[256];
  snprintf(topic, sizeof(topic), "%s%s", topicMacPrefix, topicSuffix);

  boolean result = mqttClient.publish(topic, (byte *)buffer, length, false);
  if (!result)
  {
    Serial.println("MQTT: Failed to publish");
    Serial.print("failed, rc=");
    Serial.println(mqttClient.state());
  }
  return result;
}

boolean mqttConnected()
{
  return mqttClient.connected();
}