/**
* @file MbedHardwareSerial.hpp
* @brief Define the class required for serial communication using SerialBridge in Mbed environment.
* @author testusuke
* @date 2021/9/3
*/

#if defined(__arm__) || defined(MBED_SB)

#include "../MbedHardwareSerial.hpp"

#if MBED_MAJOR_VERSION >= 6

/**
* @brief MbedHardwareSerial class constructor.
* @param[in] dev (BufferedSerial class pointer)A class object for doing character / binary
* @par Example
*   When using USBTX/RX pin, declare as follows.
*
*   BufferedSerial serial(USBTX, USBRX);
*   MbedHardwareSerial obj(&serial);
*/
MbedHardwareSerial::MbedHardwareSerial(BufferedSerial *dev)
{
    _dev = dev;
}

#else //mbed-os 6 and earlier versions.

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

#endif //#if MBED_MAJOR_VERSION >= 6

/**
* @brief Return a received character
* @return uint8_t
*/
int MbedHardwareSerial::read()
{

#if MBED_MAJOR_VERSION >= 6

    uint8_t data[1];
    if(_dev->read(data, 1) == 1)
        return data[0];
    else
        return 0;

#else //mbed-os 6 and earlier versions.

    uint8_t data = _rx_queue.front();
    _rx_queue.pop();
    return data;

#endif //#if MBED_MAJOR_VERSION >= 6
}

/**
* @brief Return number of available characters
* @return int of available chracter.
*/
int MbedHardwareSerial::readable_len()
{

#if MBED_MAJOR_VERSION >= 6

    return _dev->readable();

#else //mbed-os 6 and earlier versions.

    return _rx_queue.size();

#endif //#if MBED_MAJOR_VERSION >= 6
}

/**
* @brief A function of write array of charaters
* @param[in] data (characters pointer) characters
* @param[in] len (int) length of characters
* @return int length of characters
*/
int MbedHardwareSerial::write(unsigned char *data, unsigned int len)
{

#if MBED_MAJOR_VERSION >= 6

    return _dev->write(data, len);

#else //mbed-os 6 and earlier versions.

    for(int i = 0; i < len; i++){
        _dev->putc(data[i]);
    }
    return len;

#endif //#if MBED_MAJOR_VERSION >= 6
}

//mbed-os 6 and earlier versions.
#if MBED_MAJOR_VERSION < 6

/**
* @brief A function is called when raise rx event
*/
void MbedHardwareSerial::_serial_rx(void)
{
    while(_dev->readable()){
        unsigned char c = _dev->getc();
        //  append character into queue
        _rx_queue.push(c);
    }
}

#endif //#if MBED_MAJOR_VERSION < 6

#endif //#ifdef __arm__
