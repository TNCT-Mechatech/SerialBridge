/**
* @file CANSerialBridge.cpp
* @brief Communication library for communicating binary packet structures between serial devices.
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#include "../CANSerialBridge.hpp"

/**
* @brief CANSerialBridge class constructor.
* @param[in] dev (SerialDev class pointer) An argument that indicates a serial device class object.
* @param[in] buff_size Receive buffer size.(bytes)
*/
CANSerialBridge::CANSerialBridge(ACAN2517FD *dev)
        : _dev(dev), _id_list() {}

/**
* @brief A function that registers the message frame to be used.
* @param[in] id (Message identification number) A number to identify the message.
* The number can be specified from 0x00 to 0xFF, and up to 256 types of messages can be added.
* However, you cannot add more messages than specified by CANSerialBridge::STRUCT_MAX_NUM.
* @param[in] str (sb::MessageInterface class pointer) A data structure object used to send and receive data.
* @return int Whether the message frame was added successfully.
* @retval  0 : Success.
* @retval -1 : Failure. The argument str specified was NULL.
* @retval -2 : Failure. The number of registered message frames has reached the upper limit.
*/
int CANSerialBridge::add_frame(CANSerialBridge::frame_id id, sb::CANMessageInterface *str) {
    if (str == NULL) {
        return -1;
    }

    //  check if structure has already registered.
    int order = _id_2_order(id);
    if (order >= 0) {
        //  update structure
        _str[order] = str;
        return 0;
    }

    //  find available order and register them.
    for (int i = 0; i < STRUCT_MAX_NUM; i++) {
        if (_str[i] == NULL) {
            _str[i] = str;
            _id_list[i] = id;
            return 0;
        }
    }

    //  failed to add message due to fail to find available order.
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
int CANSerialBridge::rm_frame(frame_id id) {
    //  find order by frame id
    int order = _id_2_order(id);

    //  check if structure not exists
    if (order < 0) {
        return -1;
    }

    //  swap elements which located after removed structure.
    for (int i = order; _str[i] != NULL; i++) {
        _str[i] = _str[i + 1];
        _id_list[i] = _id_list[i + 1];
    }

    return 0;
}

/**
* @brief Sends the message registered with the specified id.
* @param[in] id (Message identification number)ID of the message you want to send.
* @return int Number of written data(byte) or error code.
* @retval -1 : Unregistered message id.
* @retval -2 : Failed to send message.
*/
int CANSerialBridge::write(CANSerialBridge::frame_id id) {
    int order = _id_2_order(id);

    //  check if structure not exists
    if (order < 0) {
        return -1;
    }

    //  pack message with frame_id
    _str[order]->packing();

    acan2517fd::CANFDMessage canfdMessage;
    //  find optimal message length
    canfdMessage.len = find_optimal_size(_str[order]->size());
    //  copy data
    memcpy(&canfdMessage.data, _str[order]->ptr(), _str[order]->size());

    //  write message
    bool result = _dev->tryToSend(canfdMessage);

    //  error handling
    if (!result) {
        return -2;
    }

    return 0;
}

/**
* @brief
* Update the received data from the serial device.
* (Note that this function must be called every time in a processing loop to get the data.)
* @return int Data acquisition status.
* @retval  0 : Updated message.
* @retval -1 : Message not received.
* @retval -2 : Received packet is invalid.
* @retval -3 : The id of the received message is unregistered.
*/
int CANSerialBridge::update() {
    //  update device
    _dev->poll();

    //  update frame
    return _update_frame();
}

/**
* @brief A function that converts id to the number of array elements.(private)
* @param[in] id Message identification number.
* @return int Number of elements or error.
* @retval -1 : Its id is not included in the array.
*/
int CANSerialBridge::_id_2_order(frame_id id) {
    for (int i = 0; _str[i] != NULL; i++) {
        if (id == _id_list[i]) {
            return i;
        }
    }
    return -1;
}

/**
* @brief Update the message from the packet data obtained from the serial device.
* The acquired packet data is checked for consistency from the packet length and checksum,
*  and passed to the ID registration message.
* @return int The number of received message.
*/
int CANSerialBridge::_update_frame() {
    int received_count = 0;

    while (_dev->available()) {
        acan2517fd::CANFDMessage canfdMessage;

        //  receive message
        bool is_received = _dev->receive(canfdMessage);

        //  failed to receive
        if (!is_received) {
            continue;
        }

        //  get order by frame_id
        int order = _id_2_order(canfdMessage.id);

        if (order < 0) {
            //  failed to find message
            continue;
        }

        //  summary of data for check sum
        uint32_t sum = 0;
        //  add all data
        for (int i = 0; i < canfdMessage.len - 1; i++) {
            sum += canfdMessage.data[i];
        }

        //  check summary of data, message data length
        if (canfdMessage.data[canfdMessage.len - 1] == (uint8_t) (sum & 0xFF)) {
            //  insert data to message
            for (int i = 0; i < _str[order]->size(); i++) {
                _str[order]->ptr()[i] = canfdMessage.data[i];
            }

            //  unpack message
            _str[order]->unpacking();

            received_count++;
        }
    }

    return received_count;
}


int CANSerialBridge::find_optimal_size(uint8_t size) {
    uint8_t optimal_size = 8;

    if ((size > 8) && (size < 12)) {
        optimal_size = 12;
    } else if ((size > 12) && (size < 16)) {
        optimal_size = 16;
    } else if ((size > 16) && (size < 20)) {
        optimal_size = 20;
    } else if ((size > 20) && (size < 24)) {
        optimal_size = 24;
    } else if ((size > 24) && (size < 32)) {
        optimal_size = 32;
    } else if ((size > 32) && (size < 48)) {
        optimal_size = 48;
    } else if ((size > 48) && (size < 64)) {
        optimal_size = 64;
    }

    return optimal_size;
}