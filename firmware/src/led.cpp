#include "led.hpp"
#include "config.hpp"
#include "mqtt.hpp"
#include "xye.hpp"
#include <WiFi.h>

unsigned long ledLastToggle = 0;

void setupLed()
{
    pinMode(LED, OUTPUT);
}

void ledLoopOnce()
{
    if (WiFi.status() == WL_CONNECTED && mqttConnected() &&
        lastReportStatus != 0 && millis() - lastReportStatus < XYE_TIMEOUT)
    {
        // Network fine and reading a bms or balancer: constant on
        digitalWrite(LED, HIGH);
    }
    else if (WiFi.status() == WL_CONNECTED && mqttConnected())
    {
        // Network fine, blink slow
        if (millis() - ledLastToggle > LED_FLASH_PERIOD * 2)
        {
            ledLastToggle = millis();
            digitalWrite(LED, !digitalRead(LED));
        }
    }
    else
    {
        // Network not connected
        if (millis() - ledLastToggle > LED_FLASH_PERIOD / 2)
        {
            ledLastToggle = millis();
            digitalWrite(LED, !digitalRead(LED));
        }
    }
}