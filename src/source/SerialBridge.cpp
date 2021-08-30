#include "../SerialBridge.hpp"

SerialBridge::SerialBridge(SerialDev *dev)
:   _id_list()
{
    _dev = new CobsSerial(dev);
}

SerialBridge::~SerialBridge()
{
    delete _dev;
}

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

int SerialBridge::write(SerialBridge::frame_id id)
{
    int order = _id_2_order(id);

    if(order < 0)
        return -1; //undefined data structure

    _str[order]->packing(id);
    return _dev->write(_str[order]->ptr(), _str[order]->size());
}

void SerialBridge::update()
{
    _dev->update();
}

int SerialBridge::_id_2_order(frame_id id)
{
    for(int i = 0; _str[i] != NULL; i++){
        if(id == _id_list[i])
            return i;
    }
    return -1;
}
