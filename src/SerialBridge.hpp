#ifndef SERIAL_BRIDGE_H_
#define SERIAL_BRIDGE_H_

#include "CobsSerial.hpp"
#include "SerialDev.hpp"
#include "Message.hpp"

class SerialBridge
{
public:
    typedef uint8_t frame_id;

    SerialBridge(SerialDev *dev);

    ~SerialBridge(){
        delete _dev;
    }

    void add_frame(frame_id id, sb::_Message *str);
    int read();
    void write(frame_id id, sb::_Message *str);

    void update();

protected:
    enum
    {
        STRUCT_MAX_NUM = 10,
    };

private:
    sb::_Message *_str[STRUCT_MAX_NUM];
    uint8_t _max_data_size;

    CobsSerial *_dev;
};

#endif
