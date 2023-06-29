/**
* @file Message.cpp
* @brief The data format handled by SerialBridge is defined here.
* @author Taiyou Komazawa
* @date 2021/8/18
*/

#include "../Message.hpp"

/**
* @brief Returns a pointer to the current packet.
* @return uint8_t* Pointer indicating the packet array.
*/
uint8_t *sb::MessageInterface::ptr() {
    return _all_packet;
}

/**
* @brief Stores the data structure in a packet.
* This function is used internally by SerialBridge when sending.
* @param[in] id Message ID assigned when sending.
* @return None
*/
void sb::MessageInterface::packing(uint8_t id) {
    //  set frame id
    _all_packet[0] = id;

    //  copy message data
    memcpy(_all_packet + 1, _data_ptr(), size() - CTRL_DATA_LEN);

    //  summary of data for checksum
    uint32_t sum = 0;
    //  sum all data
    for (int i = 0; i < size() - 1; i++) {
        sum += _all_packet[i];
    }
    //  set checksum data
    _all_packet[size() - 1] = (uint8_t) (sum & 0xFF);
}

/**
* @brief Extract the data structure from the packet.
* This function is used internally by SerialBridge on reception.
* @return None
*/
void sb::MessageInterface::unpacking() {
    memcpy(_data_ptr(), _all_packet + 1, size() - CTRL_DATA_LEN);
    _unpacked = true;
}



/**
* @brief Returns a pointer to the current packet.
* @return uint8_t* Pointer indicating the packet array.
*/
uint8_t *sb::CANMessageInterface::ptr() {
    return _all_packet;
}

/**
* @brief Stores the data structure in a packet.
* This function is used internally by SerialBridge when sending.
* @param[in] id Message ID assigned when sending.
* @return None
*/
void sb::CANMessageInterface::packing() {
    //  copy message data
    memcpy(_all_packet, _data_ptr(), size() - CTRL_DATA_LEN);

    //  summary of data for checksum
    uint32_t sum = 0;
    //  sum all data
    for (int i = 0; i < size() - 1; i++) {
        sum += _all_packet[i];
    }
    //  set checksum data
    _all_packet[size() - 1] = (uint8_t) (sum & 0xFF);
}

/**
* @brief Extract the data structure from the packet.
* This function is used internally by SerialBridge on reception.
* @return None
*/
void sb::CANMessageInterface::unpacking() {
    memcpy(_data_ptr(), _all_packet + 1, size() - CTRL_DATA_LEN);
    _unpacked = true;
}
