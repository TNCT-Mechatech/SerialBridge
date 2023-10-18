/**
* @file MbedHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Mbed environment.
* @author testusuke
* @date 2021/8/28
*/

#if defined(__arm__) && defined(MBED)

#ifndef _MBED_HARDWARE_SERIAL_HPP_
#define _MBED_HARDWARE_SERIAL_HPP_

#include "mbed.h"

#include "SerialDev.hpp"
//  for using queue
#include "CobsSerial.hpp"

/**
* @brief This class is an implementation for Mbed devices that inherits the SerialDev class.
*/
class MbedHardwareSerial : public SerialDev
{
public:
    virtual int read();

    virtual int readable_len();

    virtual int write(unsigned char *data, unsigned int len);

#if MBED_MAJOR_VERSION >= 6

    MbedHardwareSerial(BufferedSerial *dev);

private:
    BufferedSerial *_dev;

#else //mbed-os 6 and earlier versions.

    MbedHardwareSerial(Serial *dev);

protected:
    enum{
        RX_QUEUE_SIZE = 64,
    };

private:
    void _serial_rx(void);

    Serial *_dev;
    //  rx character queue
    ring_queue<uint8_t> _rx_queue;

#endif //MBED_MAJOR_VERSION >= 6
};
#endif //   #ifdef _MBED_HARDWARE_SERIAL_HPP_

#endif //   #ifdef __arm__
