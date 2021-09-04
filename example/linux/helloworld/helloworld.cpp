//g++ -Wall -Wextra  helloworld.cpp ../../../bin/libSerialBridge.a -I ../../../src

#ifdef __linux__

#include <stdio.h>

#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

#include <Message.hpp>

#define SERIAL_PATH "/dev/ttyUSB0"

typedef struct{
    char c[32];
} words_t;

typedef sb::Message<words_t> Words;

Words hello;

SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);

int main()
{
    serial.add_frame(123, &hello);
    sprintf(hello.data.c, "Hello world from linux.\n");

    serial.write(123);

    while(1){
        bool read_success = serial.read() == 0;
        if (read_success){
            printf("%s", hello.data.c);
        }

        serial.update();
    }
}

#endif
