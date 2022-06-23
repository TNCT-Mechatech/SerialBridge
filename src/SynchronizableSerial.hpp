/**
 * @file SynchronizableSerial.hpp
 * @brief
 * @author testusuke
 * @date 2022/06/23
 */

#ifndef _SYNCHRONIZABLE_SERIAL_HPP_
#define _SYNCHRONIZABLE_SERIAL_HPP_

#include "stdint.h"
#include "string.h"

#include "SerialInterface.hpp"
#include "SynchronizedSerialDev.hpp"

/**
 * @brief Synchronizable Serial class
 * @note Data format
 * | data_length | data |
 * | uint8_t | uint8_t[] |
 */
class SynchronizableSerial : public SerialInterface {
public:
    SynchronizableSerial(SynchronizedSerialDev *dev);

    virtual ~SynchronizableSerial();

    virtual int read(uint8_t *data);

    virtual int write(uint8_t *data, const unsigned int len);

    virtual int update();

private:
    SynchronizedSerialDev *_dev;
    uint8_t *_tx_buffer_, *_rx_buffer_;
};

#endif // _SYNCHRONIZABLE_SERIAL_HPP_
