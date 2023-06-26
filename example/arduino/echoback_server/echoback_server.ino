/**
* @file echoback_server.ino
* @brief Echoback example: A server-side program that sends back the transmitted data as is.
* @author Taiyou Komazawa
* @date 2021/9/5
* @details In the Arduino IDE, this file is displayed as an example of the program
*  at the location of "SerialBridge > example > arduino > echoback_server" in
*  the "Examples from Custom Libraries" column of "File> Examples".
*
* @note Operation confirmed with Arduino UNO.
*
*/

#include <SerialBridge.hpp>
#include <InoHardwareSerial.hpp>

#include "TestMsg.h"

//Pass the serial derived class entity for Arduino to SerialDev.
SerialDev *dev = new InoHardwareSerial(&Serial);
SerialBridge serial(dev);

//Declare a two-element message array. (Two for receiving and for sending,)
TestMsg msg[2];

void setup()
{
    Serial.begin(9600);

    //Register "msg[0]" message with ID = 0.
    serial.add_frame(0, &msg[0]);
    //Register "msg[1]" message with ID = 1.
    serial.add_frame(1, &msg[1]);
}

void loop()
{
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
