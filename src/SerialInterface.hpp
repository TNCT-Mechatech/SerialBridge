/**
 * @file SerialInterface.hpp
 */

#ifndef _SERIAL_INTERFACE_HPP_
#define _SERIAL_INTERFACE_HPP_

#include "stdint.h"

class SerialInterface
{
public:
    virtual ~SerialInterface() {};

    virtual int read(uint8_t *data) = 0;

    virtual int write(uint8_t *data, const unsigned int len) = 0;

    virtual int update() = 0;
};
#endif // _SERIAL_INTERFACE_HPP_
