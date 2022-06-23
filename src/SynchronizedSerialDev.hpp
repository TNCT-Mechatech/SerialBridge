/**
 * @file SynchronizedSerialDev.hpp
 * @brief The synchronized serial device interface
 * @author testusuke
 * @date 2022/6/22
 */

#ifndef _SYNCHRONIZED_SERIAL_DEV_HPP_
#define _SYNCHRONIZED_SERIAL_DEV_HPP_

/**
 * @brief Synchronized serial device class
 * @note this class is used to run different platform.
 * you need to create new serial class depending on this class.
 */
class SynchronizedSerialDev
{
public:

    /**
     * @brief transfer TX/RX data
     * @param tx_data sending data
     * @param rx_data received data
     * @return int return 0 if succeed to transfer
     */
    virtual int update(unsigned char *tx_data, unsigned char *rx_data) = 0;

    /**
     * @brief get TX/RX buffer size
     * @return unsigned int
     */
    virtual unsigned int size() = 0;

};
#endif // _SYNCHRONIZED_SERIAL_DEV_HPP_
