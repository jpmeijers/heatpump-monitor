# Heat Pump Monitor

I have an Alliance Heat Pump with a wired control panel. Alliance seems to be rebranding Midea units. 

After sniffing the communication lines with a logic analyzer, I saw that it's a differential pair, like RS485. The request and answer frames start with 0xAA and end with 0x55. Searching for this I stumbled upon: https://codeberg.org/xye/xye

My heat pump's format is however slightly different, but the basics helped to find the correct decoding formula for temperatures. A 24h dump of raw data with my attempt at decoding the frames can be found in [this LibreOffice spreadsheet](FullDayDataDecoded.ods).

I programmed an ESP32 to listen to the RS485 bus and log all frames to MQTT. The firmware is available in the firmware directly in the repository. I write the data to Influxdb and plot the values using Grafana.

## Wired Remote Control
Model KJR-26B or SUNI-W/H-002

![WiredRemoteControl](images/WiredRemoteControl.jpg)

## Wiring

|Code |Colour|Purpose        |
|-----|------|---------------|
|AC10V|White |10Vac supply   |
|AC10V|Red   |10Vac supply   |
|P    |Brown |RS485 B-       |
|Q    |Black |RS485 A+       |
|E    |Yellow|Earth/Ground/0V|

## Request/Command

|Byte Nr|Purpose                                                                             |
|-------|------------------------------------------------------------------------------------|
|0      |Header. Always 0xAA.                                                                |
|1      |8 = schedule off, 0 = schedule on, 0x82 = ?? becomes this when status byte 4 is 0x88|
|2      |0xFF                                                                                |
|3      |0x00                                                                                |
|4      |0x00                                                                                |
|5      |Tank Temp Setting                                                                   |
|6      |Temp Diff Setting                                                                   |
|7      |0x00                                                                                |
|8      |0=??, 1=?? - seen as 1 at start of schedule on                                      |
|9      |0x00                                                                                |
|10     |0x00                                                                                |
|11     |0x00                                                                                |
|12     |0x00                                                                                |
|13     |0x00                                                                                |
|14     |0x00                                                                                |
|15     |0x00                                                                                |
|16     |0x00                                                                                |
|17     |0x00                                                                                |
|18     |0x00                                                                                |
|19     |0x28                                                                                |
|20     |0x00                                                                                |
|21     |0x00                                                                                |
|22     |0x00                                                                                |
|23     |0x00                                                                                |
|24     |0x00                                                                                |
|25     |0x00                                                                                |
|26     |0x00                                                                                |
|27     |0x00                                                                                |
|28     |0x00                                                                                |
|29     |0x00                                                                                |
|30     |Checksum = 256 - (sum(b1:b29)%256)                                                  |
|31     |0x55                                                                                |


## Response/Status

|Byte Nr|Purpose                                                                                  |
|-------|-----------------------------------------------------------------------------------------|
|0      |Header. Always 0xAA.                                                                     |
|1      |0x01                                                                                     |
|2      |0x00                                                                                     |
|3      |0xFF                                                                                     |
|4      |0x08 = schedule off, 0x82 = schedule on, 0x88 = ?? after this command byte 1 becomes 0x82|
|5      |0x08                                                                                     |
|6      |0x00                                                                                     |
|7      |0x00                                                                                     |
|8      |T1 = byte/2 - 0x0F                                                                       |
|9      |0x00 = pump off, 0x64 = pump on (100%?)                                                  |
|10     |0x00                                                                                     |
|11     |0x00                                                                                     |
|12     |0x00 except 0x20 when byte 4 is 0x82                                                     |
|13     |0x00                                                                                     |
|14     |T3 = byte/2 - 0x0F                                                                       |
|15     |0x00                                                                                     |
|16     |Ia - pump amps?                                                                          |
|17     |0x00                                                                                     |
|18     |0x3C                                                                                     |
|19     |0x28                                                                                     |
|20     |0x00 = pump off, 0x20,0x22,0x24 = pump on - related to current temp diff                 |
|21     |0x00                                                                                     |
|22     |T5 = byte/2 - 0x0F                                                                       |
|23     |T4 = byte/2 - 0x0F                                                                       |
|24     |0x01                                                                                     |
|25     |0x00 = pump off, 0x01 = pump on                                                          |
|26     |0x00                                                                                     |
|27     |0x00                                                                                     |
|28     |Tank Temp Setting - follows command byte 5                                               |
|29     |Command check = 0xFF - byte 1                                                            |
|30     |Checksum = 256 - (sum(b1:b29)%256)                                                       |
|31     |0x55                                                                                     |

## Grafana

![Graphs](images/GrafanaGraphs.png "Graphing values")

## Hardware

![ESP32](images/HeatpumpESP32.jpg "Hardware")


