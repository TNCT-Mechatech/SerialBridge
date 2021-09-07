/**
* @file InoHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Arduino environment.
* @author Taiyou Komazawa
* @date 2021/9/3
*/

#ifdef ARDUINO

#include "../InoHardwareSerial.hpp"

/**
* @brief InoHardwareSerial class constructor.
* @param[in] dev (Stream class pointer)A class object for doing character / binary
*  based streams in an Arduino environment.
* @par Example
*   When using "Serial1", declare as follows.
*
*   InoHardwareSerial obj(&Serial1);
*
*/
InoHardwareSerial::InoHardwareSerial(Stream *dev)
{
    _dev = dev;
}

/**
* @brief Return a received character
* @return int
*/
int InoHardwareSerial::read()
{
    return _dev->read();
}

/**
* @brief Return number of available characters
* @return int of available chracter.
*/
int InoHardwareSerial::readable_len()
{
    return _dev->available();
}

/**
* @brief A function of write array of charaters
* @param[in] data (characters pointer) characters
* @param[in] len (int) length of characters
* @return int length of characters
*/
int InoHardwareSerial::write(unsigned char *data, unsigned int len)
{
    return _dev->write(data, len);
}

#endif //#ifdef ARDUINO
