/**
* @file LinuxHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Linux environment.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#if defined(__linux__) || defined(__APPLE__)

#ifndef _LINUX_HARDWARE_SERIAL_HPP_
#define _LINUX_HARDWARE_SERIAL_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "SerialDev.hpp"

/**
* @brief This class is an implementation for Linux devices that inherits the SerialDev class.
*/
class LinuxHardwareSerial : public SerialDev
{
public:
    LinuxHardwareSerial(const char port[], speed_t baud_rate = B9600);

    ~LinuxHardwareSerial();

    virtual int read();

    virtual int readable_len();

    virtual int write(unsigned char *data, unsigned int len);

private:
    int _fd;
    struct termios _oldtio;
};

#endif //#ifndef _LINUX_HARDWARE_SERIAL_HPP_

#endif //#ifdef __linux__
