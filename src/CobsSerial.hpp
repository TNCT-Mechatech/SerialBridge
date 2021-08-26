#ifndef _COBS_SERIAL_HPP_
#define _COBS_SERIAL_HPP_

#include <stdint.h>

#include "SerialDev.hpp"

#define COBS_TX_BUFFER_SIZE 64
#define COBS_RX_BUFFER_SIZE 256

template <typename Type_t>
class ring_queue
{
public:
    ring_queue(int max_len)
        : _max_len(max_len), _queue(new Type_t[max_len])
    {
        _begin = _num = 0;
    }

    int push(Type_t value)
    {
        if (_num >= _max_len)
            return -1;

        _queue[(_begin + _num) % _max_len] = value;
        _num++;
        return 0;
    }

    Type_t *all(){
        return (_queue + _begin);
    }

    Type_t front()
    {
        return _queue[_begin];
    }

    int size()
    {
        return _num;
    }

    int pop()
    {
        if (_num <= 0)
            return -1;

        _begin = (_begin + 1) % _max_len;
        _num--;
        return 0;
    }

private:
    const int _max_len;
    int _begin, _num;
    Type_t *_queue;
};

class CobsSerial
{
public:
    CobsSerial(SerialDev *dev);

    int read(uint8_t *data, const unsigned int max_len = 32);

    int write(uint8_t *data, const unsigned int len);

    void update();

private:
    SerialDev *_dev;

    ring_queue<uint8_t> _rx_buffer;
    bool _got_packet;
    bool _data_begin;
};

#endif //#ifndef _COBS_SERIAL_HPP_

//Backtrace: 0x400d5bee:0x3ffb1e60 0x400d114c:0x3ffb1f90 0x400d8615:0x3ffb1fb0 0x40086a7d:0x3ffb1fd0
