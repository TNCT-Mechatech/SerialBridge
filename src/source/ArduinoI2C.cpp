/**
 * @file ArduinoI2C.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/30
 */

#ifdef ARDUINO

#include "../ArduinoI2C.hpp"

ArduinoI2C::ArduinoI2C(Wire *wire, uint8_t buffer_size)
: _dev(wire)
{
    _buffer_size = buffer_size;
}

void ArduinoI2C::set_address(int address)
{
    _address = address;
}

int ArduinoI2C::update(unsigned char *tx_data, unsigned char *rx_data)
{
    //  start condition
    _dev.beginTransmission(_address);
    //  write
    _dev.write((char*)tx_data);
    //  send and stop condition
    _dev.endTransmission();

    //  read request
    _dev.request(_address, _buffer_size);
    //  read
    int i = 0;
    while (_dev.available()) {
      if (i >= _buffer_size) {
        break;
      }
      rx_data[i] = _dev.read();
      //  counter
      i++;
    }

    return 0;
}

unsigned int ArduinoI2C::size() {
  return _buffer_size;
}

#endif