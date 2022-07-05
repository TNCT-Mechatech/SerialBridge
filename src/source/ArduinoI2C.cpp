/**
 * @file ArduinoI2C.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/30
 */

#ifdef ARDUINO

#include "../ArduinoI2C.hpp"

ArduinoI2C::ArduinoI2C(TwoWire *wire, uint8_t buffer_size)
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
    // printf debug
    Serial.println("tx:");
    for (int i = 0; i < size(); i++) {
      Serial.print(tx_data[i]);
      Serial.print(" ");
    }
    Serial.println("\n\rrx:");

    //  start condition
    _dev->beginTransmission(_address);
    //  write
    _dev->write((char*)tx_data, 5);
    // _dev->write((char*)tx_data++);
    /*
    for (int i = 0; i < _buffer_size; i++) {
      _dev->write(_tx[i]);
    }
    */
    //  send and stop condition
    _dev->endTransmission();

    //  read request
    _dev->requestFrom(_address, (int)_buffer_size);

    //  read
    for (int i = 0; _dev->available() > 0 && i < size(); i++) {
      unsigned char c = (unsigned char)_dev->read();
      // rx_data[i] = _dev->read();

      rx_data[i] = c;
      //  DEBUG
      Serial.print(c);
      Serial.print(" ");
    }
    /*
    int i = 0;
    while (_dev->available() > 0) {
      if (i >= _buffer_size) {
        break;
      }
      rx_data[i] = _dev->read();

      //  DEBUG
      Serial.print(rx_data[i]);
      Serial.print(" ");

      //  counter
      i++;
    }
    */
    Serial.println("\n\rfin.");

    return 0;
}

unsigned int ArduinoI2C::size() {
  return _buffer_size;
}

#endif