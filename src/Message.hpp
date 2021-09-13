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

/**
* @namespace sb
* @brief Namespace to indicate SerialBridge messages.
*/
namespace sb
{

/**
* @brief sb::MessageInterface interface class.
* @details
* This is for abstracting various sb::Messages so that they can be handled
*  with a unified interface when used within SerialBridge.
*/
class MessageInterface
{
public:
    uint8_t *ptr();

    void packing(uint8_t id);
    void unpacking();

    /**
    * @brief Returns the length of the current packet.
    * @return int Current packet array length.
    */
    virtual int size() = 0;

protected:
    /**
    * @brief A function that returns a data structure.
    * @return *void Pointer to the data structure of the derived class.
    */
    virtual void *_data_ptr() = 0;

    /**
    * @brief The length of the packet used to identify the data.
    */
    enum{
        CTRL_DATA_LEN = 2,
    };

    uint8_t *_all_packet;
};

/**
* @brief A class that stores the data processed by the SerialBridge class.
* @tparam DataStruct Specify the type of data structure you want to handle with sb::Message.
*/
template <class DataStruct>
class Message : public MessageInterface
{
public:
    /**
    * @brief Data is exchanged through this member variable.
    * You can specify any structure type here with template parameters.
    */
    DataStruct data;

    /**
    * @brief Message class constructor.
    */
    Message()
    {
        _all_packet = new uint8_t[sizeof(DataStruct) + CTRL_DATA_LEN];
    }

    /**
    * @brief Message class destructor.
    */
    ~Message()
    {
        delete _all_packet;
    }

    virtual int size()
    {
        return sizeof(DataStruct) + CTRL_DATA_LEN;
    }

private:

    virtual void *_data_ptr()
    {
        return &data;
    }
};

};

#endif //#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
