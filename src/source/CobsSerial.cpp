#include "../CobsSerial.hpp"

#include "../cobs.hpp"

CobsSerial::CobsSerial(SerialDev *dev)
{
    _dev = dev;
    _got_packet = false;
}

int CobsSerial::read(uint8_t *data, const unsigned int max_len)
{
    int cnt = 0;
    uint8_t tmp[max_len] = {};

    if (_got_packet)
    {
        uint8_t val = 0;
        while (_rx_buffer.size())
        {
            val = _rx_buffer.front();
            _rx_buffer.pop();
            tmp[cnt] = val;
            cnt++;
            if (val == 0)
            { //次のデータパケットの終点で終了
                _got_packet = false;
                cnt = cobsDecode(tmp, cnt, data);
                break;
            }
        }
    }

    return cnt;
}

int CobsSerial::write(uint8_t *data, const unsigned int len)
{
    uint8_t tmp[COBS_TX_BUFFER_SIZE] = {};
    int tx_size = cobsEncode(data, len, tmp);
    return _dev->write(data, tx_size);
}

void CobsSerial::update()
{
    if(_dev->readable_len()){
        char tmp = _dev->read();
        if(tmp == 0 && _rx_buffer.size() && !_got_packet){ //データの終端
            _rx_buffer.push(tmp);
            _got_packet = true;
        }else if (_rx_buffer.size()){ //データの追加
            _rx_buffer.push(tmp);
        }
    }
}
