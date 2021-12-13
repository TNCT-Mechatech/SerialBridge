/**
* @file echoback_server.cpp
* @brief Echoback example: A server-side program that sends back the transmitted data as is.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details This is an example of using SerialBridge in the mbed environment.
*  Please enable "#define ENABLE_ECHO_EXAMPLE_MAIN" which is commented out to prevent
*  overloading of "main()" before compiling.
* @note Operation confirmed with mbed-os v6.14.0.
*/

//#define ENABLE_ECHO_EXAMPLE_MAIN

//ARM-GCC compiler identification macro.
#if defined(__arm__) && defined(ENABLE_ECHO_EXAMPLE_MAIN)

#include "mbed.h"
#include "platform/mbed_thread.h"

#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

#include "TestMsg.h"

//Pass the serial derived class entity for Mbed to SerialDev.
SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);

//Declare a two-element message array. (Two for receiving and for sending,)
TestMsg msg[2];

int main()
{
    //Register "msg[0]" message with ID = 0.
    serial.add_frame(0, &msg[0]);
    //Register "msg[1]" message with ID = 1.
    serial.add_frame(1, &msg[1]);

    while (1){
        //Get data from the serial bus.
        serial.update();

        if(msg[0].was_updated()){   //When msg[0] is updated.
            //Assign the received message to the message for sending.
            //Since it is a structure, you can write it in the following way.
            //  msg[1].data = msg[0].data;

            msg[1].data.prv_prv = msg[0].data.prv_prv;
            msg[1].data.prv = msg[0].data.prv;
            msg[1].data.cur = msg[0].data.cur;

            //Send a message with ID = 1 ("msg[1]" message).
            serial.write(1);
        }
    }

    return 0;
}

#endif
