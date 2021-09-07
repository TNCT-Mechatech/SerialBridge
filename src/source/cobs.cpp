/**
* @file cobs.cpp
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

#include "../cobs.hpp"

/** COBS encode data to buffer
	@param data Pointer to input data to encode
	@param length Number of bytes to encode
	@param buffer Pointer to encoded output buffer
	@return Encoded buffer length in bytes
	@note Does not output delimiter byte
*/
size_t cobsEncode(const void *data, size_t length, uint8_t *buffer)
{
    //assert(data && buffer);

    uint8_t *encode = buffer;  // Encoded byte pointer
    uint8_t *codep = encode++; // Output code pointer
    uint8_t code = 1;          // Code value

    for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte)
    {
        if (*byte) // Byte not zero, write it
            *encode++ = *byte, ++code;

        if (!*byte || code == 0xff) // Input is zero or block completed, restart
        {
            *codep = code, code = 1, codep = encode;
            if (!*byte || length)
                ++encode;
        }
    }
    *codep = code; // Write final code value

    return encode - buffer;
}

/** COBS decode data from buffer
	@param buffer Pointer to encoded input bytes
	@param length Number of bytes to decode
	@param data Pointer to decoded output data
	@return Number of bytes successfully decoded
	@note Stops decoding if delimiter byte is found
*/
size_t cobsDecode(const uint8_t *buffer, size_t length, void *data)
{
    //assert(buffer && data);

    const uint8_t *byte = buffer;      // Encoded input byte pointer
    uint8_t *decode = (uint8_t *)data; // Decoded output byte pointer

    for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block)
    {
        if (block) // Decode block byte
            *decode++ = *byte++;
        else
        {
            if (code != 0xff) // Encoded zero, write it
                *decode++ = 0;
            block = code = *byte++; // Next block length
            if (code == 0x00)       // Delimiter code found
                break;
        }
    }

    return decode - (uint8_t *)data;
}
