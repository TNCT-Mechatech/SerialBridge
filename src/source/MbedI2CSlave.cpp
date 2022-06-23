/**
 * @file MbedI2CSlave.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#include "../MbedI2CSlave.hpp"

MbedI2CSlave::MbedI2CSlave(I2CSlave *i2c, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE)
: _i2c(i2c)
{
  _buffer_size = buffer_size;
}

int MbedI2C::update(unsigned char *tx_data, unsigned char *rx_data)
{
  int state = -1;
  int i = _i2c.receive();

  if (i == I2CSlave::ReadAddressed) {
    state = _i2c.write(tx_data, _buffer_size);
  }

  if (i == I2CSlave::WriteGeneral || i == I2CSlave::WriteAddressed) {
    state = _i2c.read(rx_data, _buffer_size);
  }

  return state;
}

unsigned int MbedI2C::size() {
  return _buffer_size;
}

#endif