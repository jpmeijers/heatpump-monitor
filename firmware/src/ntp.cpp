#include <time.h>
#include <WiFi.h>

boolean timeSynced = false;

// Function that gets current epoch time
unsigned long getTime()
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        // Serial.println("Failed to obtain time");
        return (0);
    }
    time(&now);
    return now;
}

void updateTime()
{
    int tz = +2;
    int dst = 0;
    time_t now = time(nullptr);
    unsigned timeout = 5000; // try for timeout
    unsigned start = millis();
    configTime(tz * 3600, dst * 3600, "pool.ntp.org", "time.nist.gov");
    Serial.println("[NTP] Waiting for NTP sync");
    while (now < 8 * 3600 * 2)
    { // what is this ? - close to 0 is 1970
        delay(100);
        now = time(nullptr);
        if ((millis() - start) > timeout)
        {
            Serial.println("[NTP] Failed to get NTP time.");
            return;
        }
    }
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("[NTP] Current time: ");
    Serial.print(asctime(&timeinfo));
    timeSynced = true;
}

void ntpLoopOnce()
{
    static unsigned long mtime;
    if (millis() - mtime > 60000 || !timeSynced)
    {
        mtime = millis();
        if (WiFi.status() == WL_CONNECTED)
        {
            updateTime();
        }
        // else
        //     Serial.println("[NTP] No Wifi");
    }
}

void ntpLoop(void *pvParameters)
{
    while (1)
    {
        delay(1000);
        ntpLoopOnce();
    }
}