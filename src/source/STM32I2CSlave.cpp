

#if defined(__arm__) && defined(SB_HAL)

#include "../STM32I2CSlave.hpp"

STM32I2CSlave::STM32I2CSlave(I2C_HandleTypeDef *hi2c, uint8_t buffer_size)
: hi2c_(hi2c), _buffer_size(buffer_size),
  _tx_buffer(new unsigned char[buffer_size]), _rx_buffer(new unsigned char[buffer_size])
{
    //  zero fill
    memset(&_tx_buffer, 0, _buffer_size);
    memset(&_rx_buffer, 0, _buffer_size);
}

int STM32I2CSlave::update(unsigned char *tx_data, unsigned char *rx_data)
{
    memcpy(&_tx_buffer, tx_data, _buffer_size);
    memcpy(rx_data, &_rx_buffer, _buffer_size);
    return 0;
}

unsigned int STM32I2CSlave::size()
{
    return _buffer_size;
}

void STM32I2CSlave::i2c_callback(uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  //  check address
  if (AddrMatchCode == hi2c_->Init.OwnAddress1)
  {
    switch (TransferDirection) {
      /*  Master --> Slave(self)  */
      case I2C_DIRECTION_TRANSMIT: {
        if(HAL_I2C_Slave_Seq_Receive_IT(hi2c_, (uint8_t*)_rx_buffer, _buffer_size,I2C_FIRST_FRAME) != HAL_OK)
        {
          Error_Handler();
        }
        break;
      }

      /*  Master <-- Slave(self)  */
      case I2C_DIRECTION_RECEIVE: {
        if (HAL_I2C_Slave_Seq_Transmit_IT(hi2c_, (uint8_t*)_tx_buffer, _buffer_size, I2C_FIRST_FRAME) != HAL_OK)
        {
          Error_Handler();
        }
        break;
      }
    }
  }
}

#endif  //  ARM