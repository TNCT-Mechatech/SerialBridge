#ifndef TEST_MSG_H_
#define TEST_MSG_H_

#include <Message.hpp>

typedef struct TestMsgType
{
    uint32_t cur;
    uint32_t prv;
    uint32_t prv_prv;
} test_msg_t;

typedef sb::Message<test_msg_t> TestMsg;

#endif
