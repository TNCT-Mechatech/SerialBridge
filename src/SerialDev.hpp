/**
* @file SerialDev.hpp
* @brief The serial device used by SerialBridge complies with this interface class.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#ifndef _SERIAL_DEV_HPP_
#define _SERIAL_DEV_HPP_

/**
* @brief Serial device class used by SerialBridge.
* @note This class is used to solve the difference in serial communication API
*  depending on the execution environment.
*/
class SerialDev
{
public:

    /**
    * @brief Reads 1 byte from the received data.
    * @return int Read data.(1 byte)
    */
    virtual int read() = 0;

    /**
    * @brief Returns the number of data that can be read.(bytes)
    * @return int Number of data that can be read.(bytes)
    */
    virtual int readable_len() = 0;

    /**
    * @brief Writes data to the specified length (bytes) serial bus.
    * @param[in] data An array of data to send.
    * @param[in] len Length of data array to send.
    * @return int Number of written data.(bytes)
    */
    virtual int write(unsigned char *data, unsigned int len) = 0;
};

#endif //#ifndef _SERIAL_DEV_HPP_
