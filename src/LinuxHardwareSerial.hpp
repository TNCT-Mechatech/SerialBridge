#ifndef _LINUX_HARDWARE_SERIAL_H_
#define _LINUX_HARDWARE_SERIAL_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "SerialDev.hpp"

extern "C"
{
    int fdwrite(int fd, unsigned char *data, unsigned int len)
    {
        return write(fd, data, len);
    }

    int fdread(int fd)
    {
        unsigned char c[1];
        if(read(fd, c, 1) == 1)
            return *c;
        else
            return 0;
    }

    long fdtell(int fd)
    {
        return lseek(fd, 0, SEEK_CUR);
    }
}

class LinuxHardwareSerial : public SerialDev
{
public:
    LinuxHardwareSerial(const char port[], int baud_rate = B9600)
    {
        int fd = open(port, O_RDWR);
        if (fd < 0){
            perror("open port failed.");
            exit(EXIT_FAILURE);
        }

        struct termios tio;
        tio.c_cflag += CREAD;  //rx eable
        tio.c_cflag += CLOCAL; //local
        tio.c_cflag += CS8;    //8bit

        cfsetispeed(&tio, baud_rate);
        cfsetospeed(&tio, baud_rate);

        cfmakeraw(&tio); //raw mode

        tcsetattr(fd, TCSANOW, &tio);
        ioctl(fd, TCSETS, &tio);

        _fd = fd;
    }

    ~LinuxHardwareSerial()
    {
        close(_fd);
    }

    virtual int read()
    {
        return fdread(_fd);
    }

    virtual int readable_len()
    {
        return (int)fdtell(_fd);
    }

    virtual int write(unsigned char *data, unsigned int len)
    {
        return fdwrite(_fd, data, len);
    }

    private:
        int _fd;
};

#endif //#ifndef _LINUX_HARDWARE_SERIAL_H_
