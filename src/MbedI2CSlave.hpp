/**
 * @file MbedI2CSlave.hpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#if defined(__arm__) && defined(MBED_H)

#ifndef MBED_I2C_SLAVE_HPP
#define MBED_I2C_SLAVE_HPP

#include "stdint.h"
#include "mbed.h"

#include "SynchronizedSerialDev.hpp"


class MbedI2CSlave: public SynchronizedSerialDev
{
public:
    MbedI2CSlave(I2CSlave *i2c, uint8_t buffer_size = DEFAULRT_BUFFER_SIZE);

    virtual int update(unsigned char *tx_data, unsigned char *rx_data);

    virtual unsigned int size();

    enum{
    	DEFAULRT_BUFFER_SIZE = 32,
    };

private:

    //  i2c device
    I2CSlave *_i2c;

    uint8_t _buffer_size;
};

#endif // MBED_I2C_SLAVE_HPP

#endif