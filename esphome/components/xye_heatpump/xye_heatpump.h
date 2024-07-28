#pragma once

#include <vector>

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome
{
  namespace xye_heatpump
  {

    class XyeHeatpumpComponent : public Component, public uart::UARTDevice
    {
    public:
      binary_sensor::BinarySensor *schedule;
      sensor::Sensor *t_set;
      sensor::Sensor *t_delta;
      binary_sensor::BinarySensor *e_heat;
      sensor::Sensor *t1;
      sensor::Sensor *pump_percentage;
      sensor::Sensor *t3;
      sensor::Sensor *pump_current;
      sensor::Sensor *t5;
      sensor::Sensor *t4;
      binary_sensor::BinarySensor *pump_on;

      // ========== INTERNAL METHODS ==========
      void loop() override;
      void dump_config() override;

    protected:
      void check_buffer_();

      std::vector<uint8_t> buffer_;
    };

  } // namespace xye_heatpump
} // namespace esphome
