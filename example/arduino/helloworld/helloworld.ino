/**
* @file helloworld.ino
* @brief A simple program example for communicating strings.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details In the Arduino IDE, this file is displayed as an example of the program
*  at the location of "SerialBridge > example > arduino > hello world" in
*  the "Examples from Custom Libraries" column of "File> Examples".
*
* @note Operation confirmed with Arduino UNO.
*/

#include <SerialBridge.hpp>
#include <InoHardwareSerial.hpp>

#include <Message.hpp>

//Define a structure with a 32 bytes character string.
typedef struct{
    char c[32];
} words_t;

//Name the sb::Message type,which has a data structure of the words_t type,the Words type.
typedef sb::Message<words_t> Words;

Words hello;

//Pass the serial derived class entity for Arduino to SerialDev.
SerialDev *dev = new InoHardwareSerial(&Serial);
SerialBridge serial(dev);

void setup()
{
    Serial.begin(9600);

    //Register "hello" message with ID=123.
    serial.add_frame(123, &hello);
    //Stores a string inside the structure of the "hello" message.(I used sprintf.)
    sprintf(hello.data.c, "Hello world from ino.\n");

    //Send a message with ID = 123("hello" message) once.
    serial.write(123);

    //Enable built-in LED lamp for result display. (lamp is off)
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    //Get the message. (If there is an update, the return value is 0)
    static bool read_success = serial.read() == 0;

    if(read_success){  //If read () succeeds.
        //If the other party writes the same data structure to ID = 123
        //"hello.data.c" will be updated with the string sent by the other party.
        //However,since there is only one serial bus,the built-in LED lamp is lit to display the result.
        //(If LCD, Serial2, Serial3, etc. can be monitored, try changing the processing here.)
        digitalWrite(LED_BUILTIN, HIGH);
    }

    //Get data from the serial bus.
    serial.update();
}
