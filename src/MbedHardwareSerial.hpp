#ifndef _MBED_HARDWARE_SERIAL
#define _MBED_HARDWARE_SERIAL

#include "mbed.h"

#include "SerialDev.hpp"
#include "CobsSerial.hpp"

#define RX_QUEUE_SIZE 64

class MbedHardwareSerial : public SerialDev
{
public:
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
    void _serial_rx(void)
    {
        while (_dev->readable()) {
            unsigned char c = _dev->getc();
            _rx_queue.push(c);
        }
    }

    Serial *_dev;
    ring_queue<uint8_t> _rx_queue;
};
#endif