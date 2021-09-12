/**
* @file Message.hpp
* @brief The data format handled by SerialBridge is defined here.
* @author Taiyou Komazawa
* @date 2021/8/11
*
* @details
* @par Example of data format declaration:
*       //Define the structure of the data.
*       typedef struct{
*           int32_t x;
*           int32_t y;
*           int32_t z;
*       } Vector3_t;
*       //Define data classification number.
*       #define VECTOR3_ID 0
*       //Put the data format in the message type.
*       //(Since the name is long, you can rename it with typedef as follows.)
*       typedef sb::Message<Vector3_t,VECTOR3_ID> Vector3;
*
* @par How to use:
*       Vector3 msg; //Declare a message.
*
*       //Register this message frame in the SerialBridge instance
*       // (assuming you declared it with the name obj this time).
*       //The msg_id passed to the argument together has the role of
*       // associating the data communicated with the msg declared earlier.
*       obj.add_frame(0, &msg); //msg_id = 0
*
*       //The data to be communicated conforms
*       // to the contents of the object member's data structure.
*       msg.data.x;
*       msg.data.y;
*       msg.data.z;
*/

#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
#define _SERIAL_BRIDGE_MESSAGE_HPP_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/** SerialBridge's namespace */
namespace sb
{

/**
* @brief sb::Message interface class.
* @details
* This is for abstracting various sb::Messages so that they can be handled
*  with a unified interface when used within SerialBridge.
*/
class _Message
{
public:
    /**
    * @brief Returns a pointer to the current packet.
    * @return uint8_t* Pointer indicating the packet array.
    */
    virtual uint8_t *ptr() = 0;

    /**
    * @brief Stores the data structure in a packet.
    * This function is used internally by SerialBridge when sending.
    * @param[in] id Message ID assigned when sending.
    * @return None
    */
    virtual void packing(uint8_t id) = 0;

    /**
    * @brief Extract the data structure from the packet.
    * This function is used internally by SerialBridge on reception.
    * @return None
    */
    virtual void unpacking() = 0;

    /**
    * @brief Returns the length of the current packet.
    * @return int Current packet array length.
    */
    virtual int size() = 0;
};

/**
* @brief A class that stores the data processed by the SerialBridge class.
* @tparam DataStruct Specify the type of data structure you want to handle with sb::Message.
*/
template <class DataStruct>
class Message : public _Message
{
public:

    /** Data is passed using this member structure. */
    DataStruct data;

    /**
    * @deprecated The following functions are used for internal processing
    *  and do not support direct user calls.
    */
    virtual uint8_t *ptr()
    {
        return _all_packet;
    }

    /**
    * @deprecated The following functions are used for internal processing
    *  and do not support direct user calls.
    */
    virtual void packing(uint8_t id)
    {
        _all_packet[0] = id;

        uint32_t sum = 0;
        memcpy(_all_packet+1, &data, sizeof(DataStruct));
        for(int i = 0; i < size()-1; i++){
            sum += _all_packet[i];
        }
        _all_packet[size()-1] = (uint8_t)(sum & 0xFF);
    }

    /**
    * @deprecated The following functions are used for internal processing
    *  and do not support direct user calls.
    */
    virtual void unpacking()
    {
        memcpy(&data, _all_packet+1, sizeof(DataStruct));
    }

    virtual int size()
    {
        return sizeof(DataStruct) + 2;
    }
private:

    uint8_t _all_packet[sizeof(DataStruct)+2];
};

};

#endif //#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
