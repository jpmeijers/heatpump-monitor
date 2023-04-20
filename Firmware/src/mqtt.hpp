#include <Arduino.h>

void setupMqtt();
void mqttLoop(void *pvParameters);
void mqttLoopOnce();
boolean mqttConnected();
boolean mqttSendBuffer(char* topic, char* buffer, size_t length);