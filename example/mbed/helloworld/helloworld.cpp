//#define ENABLE_HELLO_EXAMPLE_MAIN

#if defined(__arm__) && defined(ENABLE_HELLO_EXAMPLE_MAIN)

#include "mbed.h"
#include "platform/mbed_thread.h"

#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

#include <Message.hpp>

typedef struct
{
    char c[32];
} words_t;

typedef sb::Message<words_t> Words;

Words hello;

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);

DigitalOut myled(LED1);

int main()
{
    serial.add_frame(123, &hello);
    sprintf(hello.data.c, "Hello world from mbed.\n");

    serial.write(123);

    myled = 0;

    while (1)
    {
        bool read_success = serial.read() == 0;
        if (read_success)
        {
            myled = 1;
        }

        serial.update();
    }
}

#endif
