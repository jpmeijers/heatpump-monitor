import esphome.codegen as cg
from esphome.components import sensor
from esphome.components.xye_heatpump import XyeHeatpumpComponent
from esphome.components.xye_heatpump.constants import (
    CONF_XYE_HEATPUMP_ID,
    CONF_PUMP_CURRENT,
    CONF_PUMP_PERCENTAGE,
    CONF_TEMPERATURE_DELTA,
    CONF_TEMPERATURE_SET,
    CONF_TEMPERATURE_T1,
    CONF_TEMPERATURE_T3,
    CONF_TEMPERATURE_T4,
    CONF_TEMPERATURE_T5,
)
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_CURRENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_AMPERE,
)

DEPENDENCIES = ["xye_heatpump"]


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_TEMPERATURE_SET): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_DELTA): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_T1): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_T3): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_T4): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_T5): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PUMP_PERCENTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PUMP_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(
    {
        cv.GenerateID(CONF_XYE_HEATPUMP_ID): cv.use_id(XyeHeatpumpComponent),
    }
)


async def to_code(config):
    platform = await cg.get_variable(config[CONF_XYE_HEATPUMP_ID])

    if temperature_set_config := config.get(CONF_TEMPERATURE_SET):
        sens = await sensor.new_sensor(temperature_set_config)
        cg.add(platform.set_temperature_set_sensor(sens))

    if temperature_delta_config := config.get(CONF_TEMPERATURE_DELTA):
        sens = await sensor.new_sensor(temperature_delta_config)
        cg.add(platform.set_temperature_delta_sensor(sens))

    if temperature_t1_config := config.get(CONF_TEMPERATURE_T1):
        sens = await sensor.new_sensor(temperature_t1_config)
        cg.add(platform.set_temperature_t1_sensor(sens))

    if temperature_t3_config := config.get(CONF_TEMPERATURE_T3):
        sens = await sensor.new_sensor(temperature_t3_config)
        cg.add(platform.set_temperature_t3_sensor(sens))

    if temperature_t4_config := config.get(CONF_TEMPERATURE_T4):
        sens = await sensor.new_sensor(temperature_t4_config)
        cg.add(platform.set_temperature_t4_sensor(sens))

    if temperature_t5_config := config.get(CONF_TEMPERATURE_T5):
        sens = await sensor.new_sensor(temperature_t5_config)
        cg.add(platform.set_temperature_t5_sensor(sens))

    if pump_percentage_config := config.get(CONF_PUMP_PERCENTAGE):
        sens = await sensor.new_sensor(pump_percentage_config)
        cg.add(platform.set_pump_percentage_sensor(sens))

    if pump_current_config := config.get(CONF_PUMP_CURRENT):
        sens = await sensor.new_sensor(pump_current_config)
        cg.add(platform.set_pump_current_sensor(sens))
