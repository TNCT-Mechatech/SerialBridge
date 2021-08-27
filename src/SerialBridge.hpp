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

    ~SerialBridge();

    int add_frame(frame_id id, sb::_Message *str);
    int rm_frame(frame_id id);

    int read();
    int write(frame_id id);

    void update();

protected:
    enum{
        STRUCT_MAX_NUM = 10,
    };

private:
    int _id_2_order(frame_id id);

    sb::_Message *_str[STRUCT_MAX_NUM];
    frame_id _id_list[STRUCT_MAX_NUM];

    CobsSerial *_dev;
};

#endif //#ifndef _SERIAL_BRIDGE_HPP_
