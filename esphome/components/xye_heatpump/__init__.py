import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.cpp_helpers import gpio_pin_expression
from esphome.components import uart
from esphome.const import (
    CONF_FLOW_CONTROL_PIN,
)
from esphome.const import CONF_ID
from esphome import pins

CODEOWNERS = ["@jpmeijers"]
DEPENDENCIES = ["uart", "sensor", "binary_sensor"]

xye_heatpump_component_ns = cg.esphome_ns.namespace("xye_heatpump")
XyeHeatpumpComponent = xye_heatpump_component_ns.class_(
    "XyeHeatpumpComponent", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(XyeHeatpumpComponent),
            cv.Optional(CONF_FLOW_CONTROL_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    # if CONF_FLOW_CONTROL_PIN in config:
    pin = await gpio_pin_expression(config[CONF_FLOW_CONTROL_PIN])
    cg.add(var.set_flow_control_pin(pin))
