/**
* @file SynchronizableSerial.cpp
* @brief
* @author testusuke
* @date 2022/6/23
*/

#include "../SynchronizableSerial.hpp"

SynchronizableSerial::SynchronizableSerial(SynchronizedSerialDev *dev)
: _dev(dev)
{
  unsigned int buff_size = dev->size();
  //  initialize tx,rx buffer
  _tx_buffer_ = new uint8_t[buff_size];
  _rx_buffer_ = new uint8_t[buff_size];
  //  zero fill
  memset(_tx_buffer_, 0, buff_size);
  memset(_rx_buffer_, 0, buff_size);
}

SynchronizableSerial::~SynchronizableSerial() {
  delete [] _tx_buffer_;
  delete [] _rx_buffer_;
}
int SynchronizableSerial::read(uint8_t *data)
{
  unsigned int len = _rx_buffer_[0];
  if (len > 0 && len <= _dev->size()) {
    memcpy(data, (_rx_buffer_ + 1), len);
    return len + 1;
  }
  return 0;
}

int SynchronizableSerial::write(uint8_t *data, const unsigned int len)
{
  if (len < _dev->size()) {
    //  length
    _tx_buffer_[0] = (uint8_t) len;
    //  data
    memcpy((_tx_buffer_ + 1), data, len);
    return len + 1;
  }
  return 0;
}

int SynchronizableSerial::update()
{
  int status = -1;
  status = _dev->update(_tx_buffer_, _rx_buffer_);
  return status;
}
