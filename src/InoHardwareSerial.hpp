#ifndef _INO_HARDWARE_SERIAL_HPP_
#define _INO_HARDWARE_SERIAL_HPP_

#include <Arduino.h>

#include "SerialDev.hpp"

class InoHardwareSerial : public SerialDev
{
public:
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
