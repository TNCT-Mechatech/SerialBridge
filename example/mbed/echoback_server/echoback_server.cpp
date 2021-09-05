//#define ENABLE_ECHO_EXAMPLE_MAIN
#if defined(__arm__) && defined(ENABLE_ECHO_EXAMPLE_MAIN)

#include "mbed.h"
#include "platform/mbed_thread.h"

#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

#include "TestMsg.h"

SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);

TestMsg msg[2];

int main()
{
    serial.add_frame(0, &msg[0]);
    serial.add_frame(1, &msg[1]);

    while (1)
    {
        serial.update();
        int err = serial.read();

        if (err == 0)
        {
            msg[1].data.prv_prv = msg[0].data.prv_prv;
            msg[1].data.prv = msg[0].data.prv;
            msg[1].data.cur = msg[0].data.cur;

            serial.write(1);
        }
    }

    return 0;
}

#endif
