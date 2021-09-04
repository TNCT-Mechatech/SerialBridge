/**
* @file InoHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Arduino environment.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#ifdef ARDUINO

#ifndef _INO_HARDWARE_SERIAL_HPP_
#define _INO_HARDWARE_SERIAL_HPP_

#include <Arduino.h>

#include "SerialDev.hpp"

/**
* @brief This class is an implementation for Arduino devices that inherits the SerialDev class.
*/
class InoHardwareSerial : public SerialDev
{
public:

    InoHardwareSerial(Stream *dev);

    virtual int read();

    virtual int readable_len();

    virtual int write(unsigned char *data, unsigned int len);

private:

    Stream *_dev;

};

#endif //#ifndef _INO_HARDWARE_SERIAL_HPP_

#endif //#ifdef ARDUINO
