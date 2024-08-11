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
      SUB_BINARY_SENSOR(schedule) // CONF_SCHEDULE
      SUB_BINARY_SENSOR(e_heat)   // CONF_E_HEAT
      SUB_BINARY_SENSOR(pump_on)  // CONF_PUMP_ON

      SUB_SENSOR(temperature_set)   // CONF_TEMPERATURE_SET
      SUB_SENSOR(temperature_delta) // CONF_TEMPERATURE_DELTA
      SUB_SENSOR(temperature_t1)    // CONF_TEMPERATURE_T1
      SUB_SENSOR(pump_percentage)   // CONF_PUMP_PERCENTAGE
      SUB_SENSOR(temperature_t3)    // CONF_TEMPERATURE_T3
      SUB_SENSOR(pump_current)      // CONF_PUMP_CURRENT
      SUB_SENSOR(temperature_t5)    // CONF_TEMPERATURE_T5
      SUB_SENSOR(temperature_t4)    // CONF_TEMPERATURE_T4

      // ========== INTERNAL METHODS ==========
      void setup() override;
      void loop() override;
      void dump_config() override;
      void set_flow_control_pin(GPIOPin *flow_control_pin) { this->flow_control_pin_ = flow_control_pin; }

    protected:
      GPIOPin *flow_control_pin_{nullptr};
      void check_buffer_();

      std::vector<uint8_t> buffer_;
    };

  } // namespace xye_heatpump
} // namespace esphome
