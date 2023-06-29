#ifndef _CAN_SERIAL_BRIDGE_HPP_
#define _CAN_SERIAL_BRIDGE_HPP_

#include "Message.hpp"
//  acan2517fd
#include "../acan2517FD/src/ACAN2517FD.h"
#include "../acan2517FD/src/CANFDMessage.h"

using namespace acan2517fd;

/**
* @brief Communication class for communicating binary packet structures between CAN FD devices.
* @details
* This class provides a protocol for freely communicating extremely lightweight data structures
*  in one-to-many local serial communication.
*/
class CANSerialBridge
{
public:
    typedef uint32_t frame_id;

    CANSerialBridge(ACAN2517FD *dev);

    ~CANSerialBridge();

    int add_frame(frame_id id, sb::CANMessageInterface *str);
    int rm_frame(frame_id id);

    int write(frame_id id);

    int update();

protected:
    enum{
        STRUCT_MAX_NUM = 10,
    };

private:
    int _id_2_order(frame_id id);
    int _update_frame();
    int find_optimal_size(uint8_t size);

    sb::CANMessageInterface *_str[STRUCT_MAX_NUM];
    frame_id _id_list[STRUCT_MAX_NUM];

    ACAN2517FD *_dev;
};

#endif //#ifndef _CAN_SERIAL_BRIDGE_HPP_
