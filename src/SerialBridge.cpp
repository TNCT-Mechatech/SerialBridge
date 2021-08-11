#include "SerialBridge.hpp"

SerialBridge::SerialBridge(SerialDev *dev)
{
    _dev = new CobsSerial(dev);
    _max_data_size = 0;
}

void SerialBridge::add_frame(SerialBridge::frame_id id, sb::_Message *str)
{
    if (str != NULL && _str[id] == NULL && id <= STRUCT_MAX_NUM)
    {
        _str[id] = str;
        if (_max_data_size < str->size())
            _max_data_size = str->size();
    }
}

int SerialBridge::read()
{
    uint8_t tmp[32] = {};
    int len = _dev->read(tmp, 32);
    if(len > 0){
        uint8_t id = tmp[0];
        int sum = 0;
        for (int i = 0; i < len - 1; i++)
            sum += tmp[i];

        if(tmp[len - 1] == (uint8_t)(sum & 0xFF) && len == _str[id]->size()){
            for (int i = 0; i < len; i++)
                _str[id]->ptr()[i] = tmp[i];
            return 0;
        }else
            return -2; //data error
    }

    return -1; //cannot received
}

void SerialBridge::write(SerialBridge::frame_id id, sb::_Message *str)
{
    str->packing(id);
    _dev->write(str->ptr(), str->size());
}

void SerialBridge::update()
{
    _dev->update();
}
