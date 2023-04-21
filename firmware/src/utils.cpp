#include "utils.hpp"

void hexDump(const uint8_t *data, uint32_t dataSize) // debug function
{
    for (int i = 0; i < dataSize; i++)
    {
        Serial.printf("%02X ", data[i]);
    }
    Serial.println("");
}