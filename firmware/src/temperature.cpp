#include <Arduino.h>
#include <stdint.h>
#include "queue.hpp"
#include "ntp.hpp"
#ifdef SOC_TEMP_SENSOR_SUPPORTED
#include "driver/temp_sensor.h"
#else

#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#endif

void setupTemperature()
{
}

void getTemperature()
{
    float tempC;
#ifdef SOC_TEMP_SENSOR_SUPPORTED
    temp_sensor_read_celsius(&tempC);
#else
    uint8_t temp;
    uint16_t total;
    int count = 100;
    int i = 0;

    unsigned long start = millis();
    while (millis() - start < 10000)
    {
        temp = temprature_sens_read();
        if (temp != 128)
        { // ignore 128 (53.33 C) measurement
            total += temp;
            i++;
            if (i > count)
            {
                break;
            }
        }
    }

    tempC = total;
    tempC = (tempC / count - 32) / 1.8;
#endif

    Serial.print(" * Core temperature = ");
    Serial.println(tempC);

    struct message myMessage;
    myMessage.time = getTime();

    snprintf(myMessage.topic, sizeof(myMessage.topic), "/temperature");
    snprintf(myMessage.raw, sizeof(myMessage.raw), "%f", tempC);

    xQueueSend(queue, &myMessage, portTICK_PERIOD_MS * 1000);
}