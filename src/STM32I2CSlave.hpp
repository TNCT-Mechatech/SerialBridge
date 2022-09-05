/**
 * @file STM32I2CSlave.hpp
 */

#if defined(__arm__) && defined(SB_HAL)

#ifndef _STM32_I2C_SLAVE_HPP_
#define _STM32_I2C_SLAVE_HPP_

//  STM32 Hal module
#include "main.h"
//  required module
#include "stdint.h"
#include <string.h>
//  parent module
#include "SynchronizedSerialDev.hpp"

class STM32I2CSlave: public SynchronizedSerialDev
{
public:
    STM32I2CSlave(I2C_HandleTypeDef *hi2c, uint8_t buffer_size = DEFAULT_BUFFER_SIZE);

    virtual int update(unsigned char *tx_data, unsigned char *rx_data);

    virtual unsigned int size();

    virtual void i2c_callback(uint8_t TransferDirection, uint16_t AddrMatchCode);

private:
    enum {
    	DEFAULT_BUFFER_SIZE = 32,
    };

    //  I2C driver
    I2C_HandleTypeDef *hi2c_;

    //  buffer size
    uint8_t _buffer_size;

    //  internal buffer
    unsigned char *_tx_buffer;
    unsigned char *_rx_buffer;
};
#endif  //  _STM32_I2C_SLAVE_HPP_
#endif  //  ARM