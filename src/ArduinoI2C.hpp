/**
 * @file ArduinoI2C.hpp
 * @brief
 * @author testusuke
 * @date 2022/06/30
 */

#ifdef ARDUINO

#ifndef _ARDUINO_I2C_HPP_
#define _ARDUINO_I2C_HPP_

#include "SynchronizedSerialDev.hpp"

class ArduinoI2C: public SynchronizedSerialDev
{
public:
    ArduinoI2C(Wire *wire, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE);

    virtual int update(unsigned char *tx_data, unsigned char *rx_data);

    virtual unsigned int size();

    void set_address(int address);

    enum{
    	DEFAULRT_BUFFER_SIZE = 32,
    };

private:
    //  i2c device
    Wire *_dev;

    uint8_t _buffer_size;

    int _address;

};

#endif // _ARDUINO_I2C_HPP_

#endif // ARDUINO