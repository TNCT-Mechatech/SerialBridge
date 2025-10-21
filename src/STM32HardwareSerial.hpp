/**
 * @file STM32HardwareSerial.hpp
 * @brief STM32 HAL + DMA(IDLE割り込み)対応 シリアル通信クラス
 * @date 2025/10/10
 */

#ifndef _STM32_HARDWARE_SERIAL_HPP_
#define _STM32_HARDWARE_SERIAL_HPP_

#include "SerialDev.hpp"
#include "../STM32BufferedSerial/src/STM32BufferedSerial.hpp"
#include <cstdint>

//#define RX_BUFFER_SIZE 256

class STM32HardwareSerial : public SerialDev
{
public:
    STM32HardwareSerial(STM32BufferedSerial *dev);

    virtual int read();                              ///< 1バイト読み取り
    virtual int readable_len();                         ///< バッファ内データ数
    virtual int write(unsigned char *data, unsigned int len); ///< 送信

private:
    STM32BufferedSerial *_dev;

};

#endif //_STM32_HARDWARE_SERIAL_HPP_
