#include <OneWire.h>
#include <DallasTemperature.h>
#include "config.hpp"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

bool readNextSensor() {
  byte i;
  byte addr[8];

  char sensorId[17];
  
  // iterate addresses using onewire library
  if ( !oneWire.search(addr)) {
    // Serial.println("No more addresses.");
    // Serial.println();
    oneWire.reset_search();
    return false;
  }
  
  sprintf(sensorId, "%02X%02X%02X%02X%02X%02X%02X%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]);

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return true;
  }
 
    Serial.print("Sensor ID: ");
    Serial.print(sensorId);
    Serial.print(" ");

   DallasTemperature::request_t result = sensors.requestTemperaturesByAddress(addr);
    if(!result.result) {
         Serial.println("Request failed");
         return true;
    }


  // use dallas temperature to read temeprature of address
  float temperature = sensors.getTempC(addr);
    Serial.println(temperature);

  return true;
}

void setupOneWire() {
    
  // Start up the library
//   sensors.begin();
}

unsigned long lastRead = 0;
bool moreToRead = true;
void loopOneWireOnce() {
    if(millis() - lastRead < ONEWIRE_READ_INTERVAL) {
        return;
    }
    
    moreToRead = readNextSensor();
    if(!moreToRead) {
        lastRead = millis();
    }
}