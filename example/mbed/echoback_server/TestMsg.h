/**
* @file TestMsg.h
* @brief Example of self-made message type definition.
* @author Taiyou Komazawa
* @date 2021/9/5
*/

#ifndef TEST_MSG_H_
#define TEST_MSG_H_

#include <Message.hpp>

//Define a structure with three unsigned integer 32-bit types.
typedef struct TestMsgType
{
    uint32_t cur;
    uint32_t prv;
    uint32_t prv_prv;
} test_msg_t;

//Name the sb::Message type,which has a data structure of the test_msg_t type,the TestMsg type.
typedef sb::Message<test_msg_t> TestMsg;

#endif
