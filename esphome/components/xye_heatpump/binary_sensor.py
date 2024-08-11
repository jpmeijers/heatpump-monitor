import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.components.xye_heatpump import XyeHeatpumpComponent
from esphome.components.xye_heatpump.constants import (
    CONF_E_HEAT,
    CONF_PUMP_ON,
    CONF_SCHEDULE,
    CONF_XYE_HEATPUMP_ID,
)
import esphome.config_validation as cv

DEPENDENCIES = ["xye_heatpump"]


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_SCHEDULE): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_E_HEAT): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_PUMP_ON): binary_sensor.binary_sensor_schema(),
    }
).extend(
    {
        cv.GenerateID(CONF_XYE_HEATPUMP_ID): cv.use_id(XyeHeatpumpComponent),
    }
)


async def to_code(config):
    platform = await cg.get_variable(config[CONF_XYE_HEATPUMP_ID])

    if schedule_config := config.get(CONF_SCHEDULE):
        sens = await binary_sensor.new_binary_sensor(schedule_config)
        cg.add(platform.set_schedule_binary_sensor(sens))

    if e_heat_config := config.get(CONF_E_HEAT):
        sens = await binary_sensor.new_binary_sensor(e_heat_config)
        cg.add(platform.set_e_heat_binary_sensor(sens))

    if pump_on_config := config.get(CONF_PUMP_ON):
        sens = await binary_sensor.new_binary_sensor(pump_on_config)
        cg.add(platform.set_pump_on_binary_sensor(sens))
