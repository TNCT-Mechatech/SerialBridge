#ifndef _COBS_H_
#define _COBS_H_

#include <stdlib.h>
#include <stdint.h>

extern size_t cobsEncode(const void *data, size_t length, uint8_t *buffer);
extern size_t cobsDecode(const uint8_t *buffer, size_t length, void *data);

#endif