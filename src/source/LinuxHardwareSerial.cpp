/**
* @file LinuxHardwareSerial.cpp
* @brief Define the class required for serial communication using SerialBridge in Linux environment.
* @author Taiyou Komazawa
* @date 2021/8/18
*/

#if defined(__linux__) || defined(__APPLE__)

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

/**
* @brief LinuxHardwareSerial class constructor.
* @param[in] port Serial device virtual file path.(ex : "/dev/ttyUSB0")
* @param[in] baud_rate (speed_t) Macro related to baud rate by c_cflag.
*  (ex : B9600(9600 baudrate), B115200(115200 baudrate))
*/
LinuxHardwareSerial::LinuxHardwareSerial(const char port[], speed_t baud_rate)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if(fd < 0){
        perror("Open port failed");
        exit(EXIT_FAILURE);
    }
    fcntl(fd, F_SETFL, FNDELAY);

    tcgetattr(fd, &_oldtio);

    struct termios tio;

    tcgetattr(fd, &tio);
    bzero(&tio, sizeof(tio));

    #ifdef __linux__
    tio.c_cflag += CREAD;  //rx eable
    tio.c_cflag += CLOCAL; //local
    tio.c_cflag += CS8;    //8bit

    tio.c_cflag += IGNPAR;
    tio.c_cflag += IGNBRK;

    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 0;
    #elif __APPLE__
    tio.c_cflag |= ( CLOCAL | CREAD |  CS8);    // Configure the device : 8 bits, no parity, no control
    tio.c_iflag |= ( IGNPAR | IGNBRK );
    tio.c_cc[VTIME]=0;                          // Timer unused
    tio.c_cc[VMIN]=0;

    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 0;
    #endif

    cfsetispeed(&tio, baud_rate);
    cfsetospeed(&tio, baud_rate);

    cfmakeraw(&tio); //raw mode

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);
    #ifdef __linux__
    ioctl(fd, TCSETS, &tio);
    #elif __APPLE__
    ioctl(fd, TIOCSETA, &tio);
    #endif

    _fd = fd;
}

/**
* @brief LinuxHardwareSerial class destructor.
*/
LinuxHardwareSerial::~LinuxHardwareSerial()
{
    tcsetattr(_fd, TCSANOW, &_oldtio);
    close(_fd);
}

/**
* @brief Return a received character
* @return unsigned char
*/
int LinuxHardwareSerial::read()
{
    return fdread(_fd);
}

/**
* @brief Return number of available characters
* @return int of available chracter.
*/
int LinuxHardwareSerial::readable_len()
{
    return (int)fdtell(_fd);
}

/**
* @brief A function of write array of charaters
* @param[in] data (characters pointer) characters
* @param[in] len (int) length of characters
* @return int length of characters
*/
int LinuxHardwareSerial::write(unsigned char *data, unsigned int len)
{
    return fdwrite(_fd, data, len);
}

#endif //#ifdef __linux__
