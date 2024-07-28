CODEOWNERS = ["@jpmeijers"]
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart', 'sensor', 'binary_sensor']

xye_heatpump_component_ns = cg.esphome_ns.namespace('xye_heatpump')
XyeHeatpumpComponent = xye_heatpump_component_ns.class_('XyeHeatpumpComponent', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(XyeHeatpumpComponent)
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)