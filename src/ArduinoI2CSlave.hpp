/**
 * @file ArduinoI2CSlave.hpp
 * @brief
 * @author testusuke
 * @date 2022/06/30
 */

#ifdef ARDUINO

#ifndef _ARDUINO_I2C_SLAVE_HPP_
#define _ARDUINO_I2C_SLAVE_HPP_

#include <Arduino.h>
#include <Wire.h>

#include "SynchronizedSerialDev.hpp"

class ArduinoI2CSlave: public SynchronizedSerialDev
{
public:
    ArduinoI2CSlave(TwoWire *wire, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE);

    virtual int update(unsigned char *tx_data, unsigned char *rx_data);

    virtual unsigned int size();

    enum{
    	DEFAULRT_BUFFER_SIZE = 32,
    };

private:
    //  i2c device
    TwoWire *_dev;

    uint8_t _buffer_size;

    //  internal buffer
    unsigned char *_tx_buffer;
    unsigned char *_rx_buffer;

    //  handler
    void receive_handler(int size);
    void request_handler();
};

#endif // _ARDUINO_I2C_SLAVE_HPP_

#endif // ARDUINO