/**
* @file MbedHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Mbed environment.
* @author testusuke
* @date 2021/8/28
*/

#ifndef _MBED_HARDWARE_SERIAL_HPP_
#define _MBED_HARDWARE_SERIAL_HPP_

#include "mbed.h"

#include "SerialDev.hpp"
//  for using queue
#include "CobsSerial.hpp"

#define RX_QUEUE_SIZE 64

/**
* @brief This class is an implementation for Mbed devices that inherits the SerialDev class.
*/
class MbedHardwareSerial : public SerialDev
{
public:
    /**
    * @brief MbedHardwareSerial class constructor.
    * @param[in] dev (Serial class pointer)A class object for doing character / binary
    * @par Example
    *   When using USBTX/RX pin, declare as follows.
    *
    *   Serial serial(USBTX, USBRX);
    *   MbedHardwareSerial obj(&serial);
    *
    */
    MbedHardwareSerial(Serial *dev) : _rx_queue(RX_QUEUE_SIZE)
    {
        _dev = dev;
        _dev->attach(callback(this, &MbedHardwareSerial::_serial_rx), Serial::RxIrq);
    }

    virtual int read()
    {
        uint8_t data = _rx_queue.front();
        _rx_queue.pop();
        return data;
    }

    virtual int readable_len()
    {
        return _rx_queue.size();
    }

    virtual int write(unsigned char *data, unsigned int len)
    {
        for(int i = 0; i < len; i++){
            _dev->putc(data[i]);
        }
        return len;
    }

private:
    /**
     * @brief This function is called when raise rx event
     */
    void _serial_rx(void)
    {
        while (_dev->readable()) {
            unsigned char c = _dev->getc();
            //  append character into queue
            _rx_queue.push(c);
        }
    }

    Serial *_dev;
    //  rx character queue
    ring_queue<uint8_t> _rx_queue;
};
#endif