/**
* @file SerialBridge.hpp
* @brief Communication library for communicating binary packet structures between serial devices.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#ifndef _SERIAL_BRIDGE_HPP_
#define _SERIAL_BRIDGE_HPP_

#include "CobsSerial.hpp"
#include "Message.hpp"
#include "SerialDev.hpp"

/**
* @brief Communication class for communicating binary packet structures between serial devices.
* @details
* This class provides a protocol for freely communicating extremely lightweight data structures
*  in one-to-one local serial communication.
* For example, when data such as sensor data and control instruction values are used
*  between the microcomputer and the PC, communication is possible simply
*  by registering the structure that stores the data in this class object.
*/
class SerialBridge
{
public:
    typedef uint8_t frame_id;

    SerialBridge(SerialDev *dev);

    ~SerialBridge();

    int add_frame(frame_id id, sb::MessageInterface *str);
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

    sb::MessageInterface *_str[STRUCT_MAX_NUM];
    frame_id _id_list[STRUCT_MAX_NUM];

    CobsSerial *_dev;
};

#endif //#ifndef _SERIAL_BRIDGE_HPP_
