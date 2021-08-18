#include "../LinuxHardwareSerial.hpp"

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

LinuxHardwareSerial::LinuxHardwareSerial(const char port[], int baud_rate)
{
    int fd = open(port, O_RDWR);
    if (fd < 0)
    {
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

int LinuxHardwareSerial::read()
{
    return fdread(_fd);
}

int LinuxHardwareSerial::readable_len()
{
    return (int)fdtell(_fd);
}

int LinuxHardwareSerial::write(unsigned char *data, unsigned int len)
{
    return fdwrite(_fd, data, len);
}
