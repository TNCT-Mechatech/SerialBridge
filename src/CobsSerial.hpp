/**
* @file CobsSerial.hpp
* @brief Serial communication library using "Consistent Overhead Byte Stuffing".
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#ifndef _COBS_SERIAL_HPP_
#define _COBS_SERIAL_HPP_

#include <stdint.h>
#include <string.h>

#include "SerialDev.hpp"

/**
* @brief Fixed length ring queue buffer class.
* @tparam Type_t Type name of the data stored in the queue buffer.
*/
template <typename Type_t>
class ring_queue
{
public:

    /**
    * @brief ring_queue class constructor.
    * @param[in] max_len Maximum number of elements in the ring queue.
    */
    ring_queue(int max_len)
        : _max_len(max_len), _queue(new Type_t[max_len])
    {
        _begin = _num = 0;
    }

    /**
    * @brief A function that adds an element to the end of a queue.
    * @param[in] value Element to add.
    * @return int Whether the addition was successful.
    * @retval  0 : Success.
    * @retval -1 : The buffer is full.
    */
    int push(Type_t value)
    {
        if (_num >= _max_len)
            return -1;

        _queue[(_begin + _num) % _max_len] = value;
        _num++;
        return 0;
    }

    /**
    * @brief Returns a pointer to the buffer.
    * Use this when you want to monitor the entire ring queue for debugging purposes.
    * @return Type_t* Pointer to the beginning of the queue.
    */
    Type_t *all()
    {
        return _queue;
    }

    /**
    * @brief A function that returns the first inserted element.
    * This function call does not change the queue.
    * @return Type_t First data.
    */
    Type_t front()
    {
        return _queue[_begin];
    }

    /**
    * @brief Returns the number of elements of data stored in the current ring queue.
    * @return int Current number of elements.
    */
    int size()
    {
        return _num;
    }

    /**
    * @brief Discards the first element inserted in the queue.
    * The data read by this process by the function ring_queue<Type_t>::front()
    *  will be the element inserted after the discarded element.
    * @return int Whether the destruction was successful.
    * @retval  0 : Success.
    * @retval -1 : The buffer is empty.
    */
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

/**
* @brief Serial communication class with "Consistent Overhead Byte Stuffing".
*/
class CobsSerial
{
public:

    CobsSerial(SerialDev *dev);

    int read(uint8_t *data);

    int write(uint8_t *data, const unsigned int len);

    void update();

    enum{
        RX_BUFFER_SIZE = 256,
    };

private:
    SerialDev *_dev;

    ring_queue<uint8_t> _rx_buffer;
    bool _got_packet;
    bool _data_begin;
};

#endif //#ifndef _COBS_SERIAL_HPP_
