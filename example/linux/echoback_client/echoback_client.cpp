//g++ -Wall -Wextra  echoback_client.cpp ../../../bin/libSerialBridge.a -I ../../../src

#ifdef __linux__

#include <iostream>

#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

#include "TestMsg.h"

#define SERIAL_PATH "/dev/ttyUSB0"

SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);

TestMsg msg[2];

int main()
{
    serial.add_frame(0, &msg[0]);
    serial.add_frame(1, &msg[1]);

    uint32_t count = 0;

    while(1){
        msg[0].data.prv_prv = msg[0].data.prv;
        msg[0].data.prv = msg[0].data.cur;
        msg[0].data.cur = count++;

        serial.write(0);

        serial.update();
        int err = serial.read();

        if(err == 0){

            printf("%d\t%d\t%d\n", msg[1].data.cur, msg[1].data.prv, msg[1].data.prv_prv);
        }
        sleep(0.05);
    }

    return 0;
}

#endif
