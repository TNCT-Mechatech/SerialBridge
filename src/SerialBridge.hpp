#ifndef _SERIAL_BRIDGE_HPP_
#define _SERIAL_BRIDGE_HPP_

#include "CobsSerial.hpp"
#include "Message.hpp"
#include "SerialDev.hpp"

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

#endif //#ifndef _SERIAL_BRIDGE_HPP_
