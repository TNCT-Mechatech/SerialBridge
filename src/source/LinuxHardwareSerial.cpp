#ifdef __linux__

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
        int len = 0;
        ioctl(fd, FIONREAD, &len);
        return len;
    }
}

LinuxHardwareSerial::LinuxHardwareSerial(const char port[], int baud_rate)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd < 0){
        fprintf(stderr, "open port failed");
        exit(EXIT_FAILURE);
    }
    fcntl(fd, F_SETFL, FNDELAY);

    tcgetattr(fd, &_oldtio);

    struct termios tio;

    tcgetattr(fd, &tio);
    bzero(&tio, sizeof(tio));

    tio.c_cflag += CREAD;  //rx eable
    tio.c_cflag += CLOCAL; //local
    tio.c_cflag += CS8;    //8bit

    tio.c_cflag += IGNPAR;
    tio.c_cflag += IGNBRK;

    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 1;

    cfsetispeed(&tio, baud_rate);
    cfsetospeed(&tio, baud_rate);

    cfmakeraw(&tio); //raw mode

    tcflush(fd, TCIFLUSH);
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

#endif //#ifdef __linux__
