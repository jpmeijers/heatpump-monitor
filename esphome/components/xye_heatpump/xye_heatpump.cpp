// Datasheet https://wiki.dfrobot.com/_A02YYUW_Waterproof_Ultrasonic_Sensor_SKU_SEN0311

#include "xye_heatpump.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

#define XYE_FRAME_LENGTH 32

namespace esphome
{
  namespace xye_heatpump
  {

    static const char *const TAG = "xye_heatpump.sensor";

    void XyeHeatpumpComponent::setup()
    {
      if (this->flow_control_pin_ != nullptr)
      {
        this->flow_control_pin_->setup();
      }
    }

    void XyeHeatpumpComponent::loop()
    {
      uint8_t data;
      this->flow_control_pin_->digital_write(false);
      while (this->available() > 0)
      {
        this->read_byte(&data);
        if (this->buffer_.empty() && (data != 0xaa))
          continue;
        buffer_.push_back(data);
        this->check_buffer_();
      }
    }

    void XyeHeatpumpComponent::check_buffer_()
    {
      // If we captured more than expected, clear and start again
      if (this->buffer_.size() > XYE_FRAME_LENGTH)
      {
        ESP_LOGW(TAG, "Captured too many bytes: %d > %d", this->buffer_.size(), XYE_FRAME_LENGTH);
        this->buffer_.clear();
        return;
      }

      // If we saw the end of frame byte, and at the correct length, process
      if (this->buffer_.back() == 0x55 && this->buffer_.size() == XYE_FRAME_LENGTH)
      {
        ESP_LOGD(TAG, "Captured full frame");

        // Crc check
        uint8_t crc_rx = this->buffer_[XYE_FRAME_LENGTH - 2];
        uint8_t crc_calc = 0;
        for (int i = 1; i < XYE_FRAME_LENGTH - 2; i++)
        {
          crc_calc += this->buffer_[i];
        }
        crc_calc = 256 - (crc_calc % 256);
        if (crc_calc != crc_rx)
        {
          ESP_LOGE(TAG, "CRC check failed");
          this->buffer_.clear();
          return;
        }

        // Command from panel to outdoor unit
        if (this->buffer_[2] == 0xFF)
        {
          ESP_LOGD(TAG, "XYE command received");

          uint8_t temp;

          temp = this->buffer_[1];
          if (temp == 0x08)
          {
            // schedule off
            if (this->schedule_binary_sensor_ != nullptr)
            {
              this->schedule_binary_sensor_->publish_state(false);
            }
          }
          if (temp == 0x00)
          {
            // schedule on
            if (this->schedule_binary_sensor_ != nullptr)
            {
              this->schedule_binary_sensor_->publish_state(true);
            }
          }
          // other values unknown

          temp = this->buffer_[5];
          if (this->t_set_sensor_ != nullptr)
          {
            this->t_set_sensor_->publish_state(float(temp));
          }

          temp = this->buffer_[6];
          if (t_delta_sensor_ != nullptr)
          {
            this->t_delta_sensor_->publish_state(float(temp));
          }

          temp = this->buffer_[11];
          if (this->e_heat_binary_sensor_ != nullptr)
          {
            this->e_heat_binary_sensor_->publish_state(temp > 0);
          }
        }

        // Reply from outdoor unit to panel
        if (this->buffer_[3] == 0xFF)
        {
          ESP_LOGD(TAG, "XYE status received");
          uint8_t temp;

          // Use schedule from command
          // uint8_t schedule = this->buffer_[4];

          temp = this->buffer_[8];
          if (t1_sensor_ != nullptr)
          {
            this->t1_sensor_->publish_state(float(temp) / 2 - 0x0F);
          }

          temp = this->buffer_[9];
          if (pump_percentage_sensor_ != nullptr)
          {
            this->pump_percentage_sensor_->publish_state(float(temp));
          }

          temp = this->buffer_[14];
          if (t3_sensor_ != nullptr)
          {
            this->t3_sensor_->publish_state(float(temp) / 2 - 0x0F);
          }

          temp = this->buffer_[16];
          if (pump_current_sensor_ != nullptr)
          {
            this->pump_current_sensor_->publish_state(float(temp));
          }

          temp = this->buffer_[22];
          if (t5_sensor_ != nullptr)
          {
            this->t5_sensor_->publish_state(float(temp) / 2 - 0x0F);
          }

          temp = this->buffer_[23];
          if (t4_sensor_ != nullptr)
          {
            this->t4_sensor_->publish_state(float(temp) / 2 - 0x0F);
          }

          temp = this->buffer_[0x19];
          if (this->pump_on_binary_sensor_ != nullptr)
          {
            this->pump_on_binary_sensor_->publish_state(temp > 0);
          }

          // Use set temperature from command
          // uint8_t t_set = this->buffer_[0x1C];
        }

        this->buffer_.clear();
        return;
      }
    }

    // void XyeHeatpumpComponent::dump_config() { LOG_SENSOR("", "XYE Heatpump Sensor", this); }
    void XyeHeatpumpComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "XYE Heatpump:");
      LOG_PIN("  Flow Control Pin: ", this->flow_control_pin_);
    }

  } // namespace xye_heatpump
} // namespace esphome
