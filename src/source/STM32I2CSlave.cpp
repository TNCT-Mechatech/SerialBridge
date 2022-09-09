#if defined(__arm__) && defined(SB_HAL)

#include "../STM32I2CSlave.hpp"

STM32I2CSlave::STM32I2CSlave(I2C_HandleTypeDef *hi2c, uint8_t buffer_size)
: hi2c_(hi2c), _buffer_size(buffer_size),
  _tx_buffer(new unsigned char[buffer_size]), _rx_buffer(new unsigned char[buffer_size])
{
    //  zero fill
    memset(_tx_buffer, 0, _buffer_size);
    memset(_rx_buffer, 0, _buffer_size);
}

int STM32I2CSlave::update(unsigned char *tx_data, unsigned char *rx_data)
{
    memcpy(_tx_buffer, tx_data, _buffer_size);
    memcpy(rx_data, _rx_buffer, _buffer_size);
    return 0;
}

unsigned int STM32I2CSlave::size()
{
    return _buffer_size;
}


#endif  //  ARM
