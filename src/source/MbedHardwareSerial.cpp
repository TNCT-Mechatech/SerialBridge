/**
* @file MbedHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Mbed environment.
* @author testusuke
* @date 2021/9/3
*/

#ifdef __arm__

#include "../MbedHardwareSerial.hpp"

/**
* @brief MbedHardwareSerial class constructor.
* @param[in] dev (Serial class pointer)A class object for doing character / binary
* @par Example
*   When using USBTX/RX pin, declare as follows.
*
*   Serial serial(USBTX, USBRX);
*   MbedHardwareSerial obj(&serial);
*/
MbedHardwareSerial::MbedHardwareSerial(Serial *dev) 
: _rx_queue(RX_QUEUE_SIZE)
{
    _dev = dev;
    _dev->attach(callback(this, &MbedHardwareSerial::_serial_rx), Serial::RxIrq);
}

/**
* @brief Return a received character
* @return uint8_t 
*/
int MbedHardwareSerial::read()
{
    uint8_t data = _rx_queue.front();
    _rx_queue.pop();
    return data;
}

/**
* @brief Return number of available characters
* @return int of available chracter.
*/
int MbedHardwareSerial::readable_len()
{
    return _rx_queue.size();    
}

/**
* @brief A function of write array of charaters
* @param[in] data (characters pointer) characters
* @param[in] len (int) length of characters 
* @return int length of characters
*/ 
int MbedHardwareSerial::write(unsigned char *data, unsigned int len)
{
    for(int i = 0; i < len; i++){
        _dev->putc(data[i]);
    }
    return len;
}

/**
* @brief A function is called when raise rx event
*/ 
void MbedHardwareSerial::_serial_rx(void)
{
    while (_dev->readable()) {
        unsigned char c = _dev->getc();
        //  append character into queue
        _rx_queue.push(c);
    }
}

#endif