#ifdef __linux__

#ifndef _LINUX_HARDWARE_SERIAL_HPP_
#define _LINUX_HARDWARE_SERIAL_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "SerialDev.hpp"


class LinuxHardwareSerial : public SerialDev
{
public:
    LinuxHardwareSerial(const char port[], int baud_rate = B9600);

    ~LinuxHardwareSerial()
    {
        tcsetattr(_fd, TCSANOW, &_oldtio);
        close(_fd);
    }

    virtual int read();

    virtual int readable_len();

    virtual int write(unsigned char *data, unsigned int len);

private:
    int _fd;
    struct termios _oldtio;
};

#endif //#ifndef _LINUX_HARDWARE_SERIAL_HPP_

#endif //#ifdef __linux__
