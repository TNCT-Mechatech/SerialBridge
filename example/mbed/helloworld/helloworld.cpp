/**
* @file helloworld.cpp
* @brief A simple program example for communicating strings.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details This is an example of using SerialBridge in the mbed environment.
*  Please enable "#define ENABLE_HELLO_EXAMPLE_MAIN" which is commented out to prevent
*  overloading of "main()" before compiling.
* @note Operation confirmed with mbed-os v6.14.0.
*/

//#define ENABLE_HELLO_EXAMPLE_MAIN

//ARM-GCC compiler identification macro.
#if defined(__arm__) && defined(ENABLE_HELLO_EXAMPLE_MAIN)

#include "mbed.h"
#include "platform/mbed_thread.h"

#include <SerialBridge.hpp>
#include <MbedHardwareSerial.hpp>

#include <Message.hpp>

//Define a structure with a 32 bytes character string.
typedef struct{
    char c[32];
} words_t;

//Name the sb::Message type,which has a data structure of the words_t type,the Words type.
typedef sb::Message<words_t> Words;

Words hello;

//Pass the serial derived class entity for Arduino to SerialDev.
SerialDev *dev = new MbedHardwareSerial(new BufferedSerial(USBTX, USBRX, 9600));
SerialBridge serial(dev);

//Enable built-in LED lamp for result display.
DigitalOut myled(LED1);

int main()
{
    //Register "hello" message with ID = 123.
    serial.add_frame(123, &hello);
    //Stores a string inside the structure of the "hello" message.(I used sprintf.)
    sprintf(hello.data.c, "Hello world from mbed.\n");

    //Send a message with ID = 123("hello" message) once.
    serial.write(123);

    //LED lamp is off.
    myled = 0;

    while(1){   //Data reception loop.
        //Get the message. (If there is an update, the return value is 0)
        bool read_success = serial.read() == 0;

        if(read_success){  //If read () succeeds.
            //If the other party writes the same data structure to ID = 123
            //"hello.data.c" will be updated with the string sent by the other party.
            //However,since there is only one serial bus,the built-in LED lamp is lit to display the result.
            //(If you can monitor the LCD, other Serial ports, etc., try changing the process here.)
            myled = 1;
            break;
        }

        //Get data from the serial bus.
        serial.update();
    }

    //Processing at the end.
    //Unregister the message with ID = 123.
    serial.rm_frame(123);
    delete dev;

    return 0;
}

#endif
