/**
* @file InoHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Arduino environment.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

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
    /**
    * @brief InoHardwareSerial class constructor.
    * @param[in] dev (Stream class pointer)A class object for doing character / binary
    *  based streams in an Arduino environment.
    * @par Example
    *   When using "Serial1", declare as follows.
    *
    *   InoHardwareSerial obj(&Serial1);
    *
    */
    InoHardwareSerial(Stream *dev)
    {
        _dev = dev;
    }

    virtual int read()
    {
        return _dev->read();
    }

    virtual int readable_len()
    {
        return _dev->available();
    }

    virtual int write(unsigned char *data, unsigned int len)
    {
        for(int i = 0; i < len; i++){
            _dev->write(data[i]);
            if(_dev->availableForWrite() <= 0)
                return i;
        }
        return len;
    }

private:

    Stream *_dev;

};

#endif //#ifndef _INO_HARDWARE_SERIAL_HPP_
