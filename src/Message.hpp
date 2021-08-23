/*
    Message.h

    SerialBridgeで扱われるデータ形式の原型をここで定義しています。

    author: Taiyou Komazawa

    データ形式宣言の例:
        //データの構造を定義
        typedef struct{
            int x;
            int y;
            int z;
        } Vector3_t;
        //データの分類番号を定義(他のメッセージ型と被らなければOK)
        #define VECTOR3_ID 0
        //データ形式をメッセージ型に入れる。
        //(名称が長いのでtypedefで改名すればよい)
        typedef sb::Message<Vector3_t,VECTOR3_ID> Vector3;

    使い方(受送信データはdata.data.以下に読み書きされる):
        Vector3 msg;
        msg.data.data.x = 0;
        msg.data.data.y = 0;
        msg.data.data.z = 0;
*/

#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
#define _SERIAL_BRIDGE_MESSAGE_HPP_

#include <stdint.h>
#include <stdlib.h>

namespace sb    //SerialBridge's namespace
{

class _Message
{
public:
    typedef uint8_t msgid_t;
    virtual uint8_t *ptr() = 0;
    virtual void packing(uint8_t id) = 0;
    virtual int size() = 0;
};

template <class DataStruct, uint8_t MESSAGE_ID>
class Message : public _Message
{
public:
    typedef union{
        uint8_t all[sizeof(DataStruct) + 3];
        struct{
            uint8_t id;
            const msgid_t msg_id = (msgid_t)MESSAGE_ID;
            DataStruct data;
            uint8_t check_sum;
        };
    } Packet_t;

    Packet_t data;

    virtual uint8_t *ptr()
    {
        return data.all;
    }

    virtual inline void packing(uint8_t id)
    {
        data.id = id;
        int sum = 0;
        data.check_sum = 0;
        for(int i = 0; i < size(); i++)
            sum += data.all[i];

        data.check_sum = (uint8_t)(sum & 0xFF);
    }

    virtual int size()
    {
        return sizeof(DataStruct) + 3;
    }

};

};

#endif //#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
