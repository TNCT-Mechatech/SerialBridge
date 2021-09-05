#include <SerialBridge.hpp>
#include <InoHardwareSerial.hpp>

#include "TestMsg.h"

SerialDev *dev = new InoHardwareSerial(&Serial);
SerialBridge serial(dev);

TestMsg msg[2];

void setup()
{
    Serial.begin(9600);

    serial.add_frame(0, &msg[0]);
    serial.add_frame(1, &msg[1]);
}

void loop()
{
    serial.update();
    int err = serial.read();

    if(err == 0){
        msg[1].data.prv_prv = msg[0].data.prv_prv;
        msg[1].data.prv = msg[0].data.prv;
        msg[1].data.cur = msg[0].data.cur;

        serial.write(1);
    }
}
