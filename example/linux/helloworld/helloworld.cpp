/**
* @file helloworld.cpp
* @brief A simple program example for communicating strings.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details This is an example of using "Serial Bridge" in a linux environment.
* Change "SERIAL_PATH" according to the path of the virtual file to which the microcomputer is connected.
* When compiling, execute "make all" in the "SerialBridge" directory,
*  and then execute the following command in the directory where this file is located.
*
* g++ helloworld.cpp ../../../bin/libSerialBridge.a -I ../../../src
*
* It works by executing the generated file "a.out". (Command:"./a.out")
*
*/

//linux os identification macro.
#ifdef __linux__

#include <stdio.h>

#include <SerialBridge.hpp>
#include <LinuxHardwareSerial.hpp>

#include <Message.hpp>

#define SERIAL_PATH "/dev/ttyUSB0"

//Define a structure with a 32 bytes character string.
typedef struct{
    char c[32];
} words_t;

//Name the sb::Message type,which has a data structure of the words_t type,the Words type.
typedef sb::Message<words_t> Words;

Words hello;

//Pass the serial derived class entity for Linux to SerialDev.
SerialDev *dev = new LinuxHardwareSerial(SERIAL_PATH, B9600);
SerialBridge serial(dev);

int main()
{
    //Register "hello" message with ID = 123.
    serial.add_frame(123, &hello);
    //Stores a string inside the structure of the "hello" message.(I used sprintf.)
    sprintf(hello.data.c, "Hello world from linux.\n");

    //Send a message with ID = 123 ("hello" message) once.
    serial.write(123);

    while(1){   //Data reception loop.
        //Get data from the serial bus.
        serial.update();
        
        if (hello.was_updated()){  //When hello is updated.
            //If the other party writes the same data structure to ID = 123
            //"hello.data.c" will be updated with the string sent by the other party.
            printf("%s", hello.data.c);
            break;
        }
    }

    //Processing at the end.
    //Unregister the message with ID = 123.
    serial.rm_frame(123);
    delete dev;

    return 0;
}

#endif
