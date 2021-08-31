/**
* @file SerialBridge.cpp
* @brief Communication library for communicating binary packet structures between serial devices.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#include "../SerialBridge.hpp"

/**
* @brief SerialBridge class constructor.
* @param[in] dev (SerialDev class pointer) An argument that indicates a serial device class object.
*/
SerialBridge::SerialBridge(SerialDev *dev)
:   _id_list()
{
    _dev = new CobsSerial(dev);
}

/**
* @brief SerialBridge class destructor.
*/
SerialBridge::~SerialBridge()
{
    delete _dev;
}

/**
* @brief A function that registers the message frame to be used.
* @param[in] id (Message identification number) A number to identify the message.
* The number can be specified from 0x00 to 0xFF, and up to 256 types of messages can be added.
* However, you cannot add more messages than specified by SerialBridge::STRUCT_MAX_NUM.
* @param[in] str (sb::_Message class pointer) A data structure object used to send and receive data.
* @return int Whether the message frame was added successfully.
* @retval  0 : Success.
* @retval -1 : Failure. The argument str specified was NULL.
* @retval -2 : Failure. The number of registered message frames has reached the upper limit.
*/
int SerialBridge::add_frame(SerialBridge::frame_id id, sb::_Message *str)
{
    if(str == NULL)
        return -1;

    for(int i = 0; i < STRUCT_MAX_NUM; i++){
        if(_str[i] == NULL){
            _str[i] = str;
            _id_list[i] = id;
            return 0;
        }
    }

    return -2;
}

/**
* @brief A function to remove registered message frames.
* @param[in] id (Message identification number)
* The identifier of the message frame you want to remove.
* @return int Whether the remove was successful.
* @retval  0 : Success.
* @retval -1 : Unregistered message id.
*/
int SerialBridge::rm_frame(frame_id id)
{
    int order = _id_2_order(id);

    if(order < 0)
        return -1; //undefined data structure

    for(int i = order; _str[i] != NULL; i++){
        _str[i] = _str[i+1];
        _id_list[i] = _id_list[i+1];
    }

    return 0;
}

/**
* @brief Update the message from the packet data obtained from the serial device.
* The acquired packet data is checked for consistency from the packet length and checksum,
*  and passed to the ID registration message.
* @return int Data acquisition status.
* @retval  0 : Updated message.
* @retval -1 : Message not received.
* @retval -2 : Received packet is invalid.
* @retval -3 : The id of the received message is unregistered.
*/
int SerialBridge::read()
{
    uint8_t tmp[CobsSerial::RX_BUFFER_SIZE] = {};
    int len = _dev->read(tmp) - 1;
    if(len > 0){
        int order = _id_2_order(tmp[0]);

        if(order < 0)
            return -3; //undefined data structure

        uint32_t sum = 0;
        for(int i = 0; i < len-1; i++)
            sum += tmp[i];

        if(tmp[len - 1] == (uint8_t)(sum & 0xFF) && len == _str[order]->size()){
            for (int i = 0; i < len; i++)
                _str[order]->ptr()[i] = tmp[i];
            _str[order]->unpacking();
            return 0;
        }else
            return -2; //data error
    }

    return -1; //cannot received
}

/**
* @brief Sends the message registered with the specified id.
* @param[in] id (Message identification number)ID of the message you want to send.
* @return int Number of written data(byte) or error code.
* @retval -1 : Unregistered message id.
*/
int SerialBridge::write(SerialBridge::frame_id id)
{
    int order = _id_2_order(id);

    if(order < 0)
        return -1; //undefined data structure

    _str[order]->packing(id);
    return _dev->write(_str[order]->ptr(), _str[order]->size());
}

/**
* @brief
* Update the received data from the serial device.
* (Note that this function must be called every time in a processing loop to get the data.)
* @return None
*/
void SerialBridge::update()
{
    _dev->update();
}

/**
* @brief A function that converts id to the number of array elements.(private)
* @param[in] id Message identification number.
* @return int Number of elements or error.
* @retval -1 : Its id is not included in the array.
*/
int SerialBridge::_id_2_order(frame_id id)
{
    for(int i = 0; _str[i] != NULL; i++){
        if(id == _id_list[i])
            return i;
    }
    return -1;
}
