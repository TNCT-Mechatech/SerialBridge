/**
 * @file ArduinoI2CSlave.cpp
 * @brief
 * @author testusuke
 * @date 2022/06/30
 */

#ifdef ARDUINO

#include "../ArduinoI2CSlave.hpp"

ArduinoI2CSlave::ArduinoI2CSlave(TwoWire *wire, uint8_t buffer_size)
: _dev(wire), _buffer_size(buffer_size),
 _tx_buffer(new unsigned char[buffer_size]), _rx_buffer(new unsigned char[buffer_size])
{
    //  zero fill
    memset(&_tx_buffer, 0, _buffer_size);
    memset(&_rx_buffer, 0, _buffer_size);

    //  set up handler
    _dev.onReceive(receive_handler);
    _dev.onRequest(request_handler);
}

int ArduinoI2CSlave::update(unsigned char *tx_data, unsigned char *rx_data)
{
    memcpy(&_tx_buffer, tx_data, _buffer_size);
    memcpy(rx_data, &_rx_buffer, _buffer_size);
    return 0;
}

unsigned int ArduinoI2CSlave::size() {
    return _buffer_size;
}

void ArduinoI2CSlave::receive_handler(int size)
{
    for (int i = 0; i < size; i++) {
        uint8_t c = _dev.read();
        _rx_buffer[i] = c;
    }
}

void ArduinoI2CSlave::request_handler()
{
    _dev.write(_tx_buffer, _buffer_size);
}

#endif