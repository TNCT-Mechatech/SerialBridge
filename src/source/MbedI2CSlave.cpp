/**
 * @file MbedI2CSlave.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#include "../MbedI2CSlave.hpp"

MbedI2CSlave::MbedI2CSlave(I2CSlave *i2c, uint8_t buffer_size)
: _dev(i2c)
{
  _buffer_size = buffer_size;
}

int MbedI2CSlave::update(unsigned char *tx_data, unsigned char *rx_data)
{
  int state = -1;
  int i = _dev.receive();

  if (i == I2CSlave::ReadAddressed) {
    //  copy
    char _tx[_buffer_size];
    memcpy(&_tx, tx_data, _buffer_size);
    //  send
    state = _dev.write(tx_data, _buffer_size);
  }

  if (i == I2CSlave::WriteGeneral || i == I2CSlave::WriteAddressed) {
    char _rx[_buffer_size];
    //  read
    state = _dev.read(rx_data, _buffer_size);
    //  copy
    memcpy(rx_data, &_rx, _buffer_size);
  }

  return state;
}

unsigned int MbedI2CSlave::size() {
  return _buffer_size;
}

#endif