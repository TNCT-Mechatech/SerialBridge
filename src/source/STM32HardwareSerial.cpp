#if defined(__arm__) && defined(STM32)

#include "../STM32HardwareSerial.hpp"
#include <cstring>

/**
 * @brief コンストラクタ
 */
STM32HardwareSerial::STM32HardwareSerial(STM32BufferedSerial *dev)
{
    _dev = dev;
}

/**
 * @brief 1バイト読み取り
 */
int STM32HardwareSerial::read()
{
    return _dev->read();
}

/**
 * @brief バッファ内のデータ数
 */
int STM32HardwareSerial::readable_len()
{
    return _dev->readable_len();
}

/**
 * @brief 送信
 */
int STM32HardwareSerial::write(unsigned char *data, unsigned int len)
{
    return _dev->write((uint8_t*)data, (uint16_t)len);
}

#endif //ifdef(__arm__)