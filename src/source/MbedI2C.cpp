/**
 * @file MbedI2C.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#include "../MbedI2C.hpp"

MbedI2C::MbedI2C(I2C *i2c, uint8_t buffer_size)
: _dev(i2c)
{
    _buffer_size = buffer_size;
}

void MbedI2C::set_address(int address)
{
    _address = address;
}

int MbedI2C::update(unsigned char *tx_data, unsigned char *rx_data)
{
    char _tx[_buffer_size], _rx[_buffer_size];
    memcpy(&_tx, tx_data, _buffer_size);

    _dev->write(_address, _tx, _buffer_size);
    _dev->read(_address, _rx, _buffer_size);

    memcpy(rx_data, &_rx, _buffer_size);

    return 0;
}

unsigned int MbedI2C::size() {
  return _buffer_size;
}

#endif