/**
 * @file MbedI2C.hpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#ifndef _MBED_I2C_HPP_
#define _MBED_I2C_HPP_

#include "stdint.h"
#include "mbed.h"

#include "SynchronizedSerialDev.hpp"

class MbedI2C: public SynchronizedSerialDev
{
public:
    MbedI2C(I2C i2c, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE);

    virtual int update(unsigned char *tx_data, unsigned char *rx_data);

    virtual unsigned int size();

    void set_address(int address);

    enum{
    	DEFAULRT_BUFFER_SIZE = 32,
    };

private:
    //  i2c device
    I2C *_i2c;

    uint8_t _buffer_size;

    int _address;

};
#endif // _MBED_I2C_HPP_

#endif