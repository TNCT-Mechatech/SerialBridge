/**
 * @file MbedI2C.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#include "../MbedI2C.hpp"

MbedI2C::MbedI2C(I2C *i2c, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE)
: _i2c(i2c)
{
  _buffer_size = buffer_size;
}

void MbedI2C::set_address(int address)
{
    _address = address;
}

int MbedI2C::update(unsigned char *tx_data, unsigned char *rx_data)
{
  if(_i2c.write(_address, tx_data, _buffer_size) == 0 &&
          _i2c.read(_address, rx_data, _buffer_size) == 0) return 0;
  return -1;
}

unsigned int MbedI2C::size() {
  return _buffer_size;
}

#endif