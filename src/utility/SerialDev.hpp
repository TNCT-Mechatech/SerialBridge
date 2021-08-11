#ifndef _SERIAL_DEV_H_
#define _SERIAL_DEV_H_

class SerialDev
{
public:
    virtual int read() = 0;
    virtual int readable_len() = 0;
    virtual int write(unsigned char *data, unsigned int len) = 0;
};

#endif
