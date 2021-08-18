#ifndef _COBS_SERIAL_H_
#define _COBS_SERIAL_H_

#include <stdint.h>

#include <queue>

#include "SerialDev.hpp"


#define COBS_TX_BUFFER_SIZE 32

class CobsSerial
{
public:
    CobsSerial(SerialDev *dev);

    int read(uint8_t *data, const unsigned int max_len = 32);

    int write(uint8_t *data, const unsigned int len);

    void update();

private:
    SerialDev *_dev;

    std::queue<uint8_t> _rx_buffer;
    bool _got_packet;

};

#endif
