/**
* @file CobsSerial.cpp
* @brief Serial communication library using "Consistent Overhead Byte Stuffing".
* @author Taiyou Komazawa
* @date 2021/8/11
*/

#include "../CobsSerial.hpp"

#include "../cobs.hpp"

/**
* @brief CobsSerial class constructor.
* @param[in] dev (SerialDev class pointer) An argument that indicates a serial device class object.
* @param[in] buff_size Receive buffer size.(bytes)
*/
CobsSerial::CobsSerial(SerialDev *dev, const unsigned int buff_size)
    : _buff_size(buff_size), _rx_buffer(buff_size)
{
    _dev = dev;
    _got_packet = false;
}

/**
* @brief Reads the data in the queue buffer after decoding.
* @param[out] data Decoded data array.
* @return int Number of acquired data. (bytes)
* @details
* This function gets and decodes the packets stored in the queue buffer
* by the CobsSerial::update function.
* The obtained data is output to the array specified by the argument data.
*/
int CobsSerial::read(uint8_t *data)
{
    int cnt = 0;
    uint8_t tmp[_buff_size];

    if(!_data_begin && _got_packet){
        uint8_t val = 1;
        while(_rx_buffer.size()){
            val = _rx_buffer.front();
            _rx_buffer.pop();
            tmp[cnt] = val;
            cnt++;
        }

        if(val == 0){ //データパケットの終端で終了
            cnt = cobsDecode(tmp, cnt, data);
        }else{
            cnt = 0;
        }
        _got_packet = false;
    }
    return cnt;
}

/**
* @brief Write the encoded data to the serial device.
* @param[in] data Raw data array you want to send.
* @param[in] len Length of data array to send.
* @return int Number of written data.(bytes)
* @details
* This function encodes the passed data in COBS format
* and writes it to the serial device.
* This process is expected to process less than 256 bytes of input data.
*
* The breakdown of the converted data is as follows.
*
*   |----raw data----|      : Input (len[bytes])
*   0|--encoded data---|    : After the cobsEncode function (tx_size = len+2[bytes])
*   0|--encoded data---|0   : Data when sending (total size = len+3[bytes])
*
* 0 ('\0') is the data that represents the start and end of the packet.
* Whenever data less than 256 bytes in length is COBS encoded, it will be one byte longer.
* If you add data that represents the beginning and end of the packet,the final data length
*  will be the original data length plus 3 bytes.
*/
int CobsSerial::write(uint8_t *data, const unsigned int len)
{
    uint8_t tmp[len+3];
    memset(tmp, 0, len+3);
    int tx_size = cobsEncode(data, len, tmp+1); //先頭の次にcobsコードを代入
    return _dev->write(tmp, tx_size+2);
}

/**
* @brief
* Update the received data from the serial device.
* (Note that this function must be called every time in a processing loop to get the data.)
* @return None
*/
int CobsSerial::update()
{
    while(_dev->readable_len()){
        char tmp = _dev->read();

        if(_data_begin && _got_packet){
            if(tmp){                    //データの始端(始端の次のデータが始端以外であるとき)
                _rx_buffer.push(tmp);
                _got_packet = false;
                continue;
            }
        }

        if(!tmp){                       //データが0であったときの処理
            if(!_rx_buffer.size()){     //データが入っていないとき
                _data_begin = true;
                continue;               //フラグがtrue,trueで継続
            }else{                      //データの終端(データが入っているとき)
                _rx_buffer.push(tmp);

                _data_begin = false;
                _got_packet = true;
                return 0;
            }
        }

        if(_data_begin && !_got_packet){ //データの追加
            _rx_buffer.push(tmp);
        }
    }
    return -1;
}
