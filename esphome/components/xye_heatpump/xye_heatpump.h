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

      SUB_BINARY_SENSOR(schedule)
      SUB_BINARY_SENSOR(e_heat)
      SUB_BINARY_SENSOR(pump_on)
      
      SUB_SENSOR(t_set)
      SUB_SENSOR(t_delta)
      SUB_SENSOR(t1)
      SUB_SENSOR(pump_percentage)
      SUB_SENSOR(t3)
      SUB_SENSOR(pump_current)
      SUB_SENSOR(t5)
      SUB_SENSOR(t4)

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
