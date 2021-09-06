/**
* @file echoback_client.cpp
* @brief echoback example: A client-side program that sends data and displays the returned data.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details This is an example of using "Serial Bridge" in a linux environment.
* Change "SERIAL_PATH" according to the path of the virtual file to which the microcomputer is connected.
* When compiling, execute "make all" in the "SerialBridge" directory,
*  and then execute the following command in the directory where this file is located.
*
* g++ -Wall -Wextra  echoback_client.cpp ../../../bin/libSerialBridge.a -I ../../../src
*
* It works by executing the generated file "a.out". (Command:"./a.out")
*
*/

//linux os identification macro.
#ifdef __linux__

#include <stdio.h>

#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

//Include the message type (TestMsg) created in advance.
#include "TestMsg.h"

#define SERIAL_PATH "/dev/ttyUSB0"

//Pass the serial derived class entity for Linux to SerialDev.
SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);

//Declare a two-element message array. (Two for sending and for receiving,)
TestMsg msg[2];

int main()
{
    //Register "msg[0]" message with ID = 0.
    serial.add_frame(0, &msg[0]);
    //Register "msg[1]" message with ID = 1.
    serial.add_frame(1, &msg[1]);

    uint32_t count = 0;

    while(1){
        //The data to be sent can be anything, but this time I will try to send
        // the current number of loops, the number of previous loops, and the number of previous previous loops.
        msg[0].data.prv_prv = msg[0].data.prv;
        msg[0].data.prv = msg[0].data.cur;
        msg[0].data.cur = count++;

        //Send a message with ID = 0 ("msg[0]" message).
        serial.write(0);

        //Get data from the serial bus.
        serial.update();
        //Get the message. (If there is an update, the return value is 0)
        int err = serial.read();

        if(err == 0){   //If read () succeeds.
            //Display the returned data (data is stored in ID = 1 (msg [1])).
            printf("%d\t%d\t%d\n", msg[1].data.cur, msg[1].data.prv, msg[1].data.prv_prv);
        }
        sleep(0.05); //Wait 50 milliseconds. (May not need)
    }

    return 0;
}

#endif
