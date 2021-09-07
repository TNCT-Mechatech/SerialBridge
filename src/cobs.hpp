/**
* @file cobs.hpp
* @brief Array encoder/decoder using "Consistent Overhead Byte Stuffing".
* @author Taiyou Komazawa
* @date 2021/8/11
*
* @details An example of data byte processing.
*
*  [OHB]                                    : Overhead byte (Start of frame)
*      3+ -------------->|                  : Points to relative location of first zero symbol
*                        2+-------->|       : Is a zero data byte, pointing to next zero symbol
*                                   [EOP]   : Location of end-of-packet zero symbol.
*      0     1     2     3     4    5       : Byte Position
*      03    11    22    02    33   00      : COBS Data Frame
*            11    22    00    33           : Extracted Data
*
* OHB = Overhead Byte (Points to next zero symbol)
* EOP = End Of Packet
*
* See below for more information.
* Wiki: https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
* @note
* The implementation of this algorithm uses the one
* in the Wikipedia article (as of 31 May 2021 update).
*/

#ifndef _COBS_HPP_
#define _COBS_HPP_

#include <stdlib.h>
#include <stdint.h>

extern size_t cobsEncode(const void *data, size_t length, uint8_t *buffer);
extern size_t cobsDecode(const uint8_t *buffer, size_t length, void *data);

#endif //#ifndef _COBS_HPP_