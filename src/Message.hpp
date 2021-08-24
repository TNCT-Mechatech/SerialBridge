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
    virtual uint8_t *ptr() = 0;
    virtual void packing(uint8_t id) = 0;
    virtual void unpacking() = 0;
    virtual int size() = 0;
};

template <class DataStruct, uint8_t MESSAGE_ID>
class Message : public _Message
{
public:
    Message()
    {
        _all_packet[1] = MESSAGE_ID;
    }
    typedef union{
        uint8_t all[sizeof(DataStruct)];
        DataStruct data;
    } Packet_t;

    Packet_t data;

    virtual uint8_t *ptr()
    {
        return _all_packet;
    }

    virtual void packing(uint8_t id)
    {
        _all_packet[0] = id;
        uint32_t sum = 0;
        for(int i = 0; i < size()-1; i++){
            if(i >= 2)
                _all_packet[i] = data.all[i-2];
            sum += _all_packet[i];
        }
        _all_packet[size()-1] = (uint8_t)(sum & 0xFF);
    }

    virtual void unpacking()
    {
        for(int i = 2; i < (int)sizeof(DataStruct); i++){
            data.all[i - 2] = _all_packet[i];
        }
    }

    virtual int size()
    {
        return sizeof(DataStruct) + 3;
    }
private:
    uint8_t _all_packet[sizeof(DataStruct)+3];
};

};

#endif //#ifndef _SERIAL_BRIDGE_MESSAGE_HPP_
