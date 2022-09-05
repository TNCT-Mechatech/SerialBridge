/**
 * @file MbedI2C.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(SB_MBED)

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
    _dev->write(_address, (char*)tx_data, _buffer_size, true);
    _dev->read(_address, (char*)rx_data, _buffer_size);
    return 0;
}

unsigned int MbedI2C::size() {
  return _buffer_size;
}

#endif