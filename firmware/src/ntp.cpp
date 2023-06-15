#include <time.h>
#include <WiFi.h>
#include "config.hpp"

boolean timeSynced = false;

// Function that gets current epoch time
unsigned long getTime()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo, 1)) // 1ms timeout, default is 5 seconds
    {
        Serial.println("[NTP] Time not set, using 0");
        return (0);
    }
    time(&now);
    return now;
}

bool checkIsSynced()
{
    time_t now = time(nullptr);
    if(now < 8 * 3600 * 2) {
        return false;
    }
    return true;
}

void printTime() {
    time_t now;
    time(&now);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("[NTP] ");
    Serial.print(asctime(&timeinfo));
}

void ntpLoopOnce()
{
    static unsigned long previousPrintTime = 0;

    // As soon as we see a sync, print out
    if(!timeSynced){
        if(checkIsSynced()) {
            timeSynced = true;
            printTime();
        }
    }

    else {
        unsigned long now = getTime();
        if(now % NTP_PRINT_PERIOD == 0 && now != previousPrintTime) {
            printTime();
            previousPrintTime = now;
        }
    }
}

void ntpLoop(void *pvParameters)
{
    while (1)
    {
        delay(100);
        ntpLoopOnce();
    }
}

void setupNtp() {
    int tz = NTP_PRINT_TZ_OFFSET;
    int dst = 0;
    configTime(tz * 3600, dst * 3600, NTP_SERVER_1, NTP_SERVER_2);
}