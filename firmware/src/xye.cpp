#include "xye.hpp"
#include "ntp.hpp"
#include "queue.hpp"

#define XYE_REPORT_INTERVAL 60000

#define SER1_RX 35
#define SER1_TX 32
#define ENABLE 33

#define XYE_FRAME_LENGTH 32

uint8_t xyeBuffer[XYE_FRAME_LENGTH];
size_t xyeBufferLength = 0;
uint8_t xyeByte;
bool inFrame = false;
unsigned long lastReportCommand = 0;
unsigned long lastReportStatus = 0;

void sendFrame()
{
    struct message myMessage;
    myMessage.time = getTime();

    if (xyeBuffer[2] == 0xFF)
    {
        Serial.println("XYE command received");
        // request/command has FF in 3rd byte
        // if (millis() - lastReportCommand < XYE_REPORT_INTERVAL)
        // {
        //     return;
        // }
        lastReportCommand = millis();
        // /rs485/heatpump/xye/command
        snprintf(myMessage.topic, sizeof(myMessage.topic), "/rs485/xye/command");
    }
    if (xyeBuffer[3] == 0xFF)
    {
        Serial.println("XYE status received");
        // response/status has FF in 4th byte
        // if (millis() - lastReportStatus < XYE_REPORT_INTERVAL)
        // {
        //     return;
        // }
        lastReportStatus = millis();
        // /rs485/heatpump/xye/status
        snprintf(myMessage.topic, sizeof(myMessage.topic), "/rs485/xye/status");
    }

    // Raw byte array to hex char array
    int nrOfBytes = min(xyeBufferLength, (sizeof(myMessage.raw) / 2) - 1);

    for (int i = 0; i < nrOfBytes; i++)
    {
        byte nib1 = (xyeBuffer[i] >> 4) & 0x0F;
        byte nib2 = (xyeBuffer[i] >> 0) & 0x0F;
        myMessage.raw[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        myMessage.raw[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
    }
    myMessage.raw[nrOfBytes * 2] = '\0';

    xQueueSend(queue, &myMessage, portTICK_PERIOD_MS * 1000);
}

void loopXyeOnce()
{
    while (Serial1.available())
    {
        xyeByte = Serial1.read();

        if (inFrame)
        {
            // Busy receiving frame
            if (xyeBufferLength < XYE_FRAME_LENGTH - 1)
            {
                xyeBuffer[xyeBufferLength] = xyeByte;
                xyeBufferLength++;
            }
            else if (inFrame && xyeByte == 0x55 && xyeBufferLength == XYE_FRAME_LENGTH - 1)
            {
                xyeBuffer[xyeBufferLength] = xyeByte;
                xyeBufferLength++;

                sendFrame();

                inFrame = false;
                xyeBufferLength = 0;
            }
            else
            {
                // too many byte received
                inFrame = false;
                xyeBufferLength = 0;
            }
        }
        else
        {
            // not in frame
            if (xyeByte == 0xAA)
            {
                inFrame = true;
                xyeBufferLength = 0;
                xyeBuffer[xyeBufferLength] = xyeByte;
                xyeBufferLength++;
            }
            else
            {
                // not start of frame, ignore
                inFrame = false;
                xyeBufferLength = 0;
            }
        }
    }
}

void setupXye()
{
    Serial1.begin(1200, SERIAL_8N1, SER1_RX, SER1_TX);
    pinMode(ENABLE, OUTPUT);
}