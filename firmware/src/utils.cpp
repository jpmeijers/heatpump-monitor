#include "utils.hpp"
#include <WiFi.h>

void getHostname(char *buffer, size_t bufferSize)
{
    uint8_t mac[6];
    WiFi.macAddress(mac);

    snprintf(buffer, bufferSize, "mac-%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void hexDump(const uint8_t *data, uint32_t dataSize) // debug function
{
    for (int i = 0; i < dataSize; i++)
    {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println("");
}