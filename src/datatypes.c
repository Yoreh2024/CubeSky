#include "datatypes.h"

uint8_t varint_decode(char* buf, uint8_t len, int32_t* out) {
    int32_t value = 0;
    uint8_t shift = 0;
    char* end = buf + len;
    for(char* ptr = buf; ptr < end; ++ptr){
        if (shift >= 32) {
            return 0;
        }
        unsigned char b = *ptr;
        value |= ((uint32_t)(b & 0x7F)) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
    }
    *out = value;
    return 1;
}

uint8_t varlong_decode(char* buf, uint8_t len, int64_t* out) {
    int64_t value = 0;
    uint8_t shift = 0;
    char* end = buf + len;
    for(char* ptr = buf; ptr < end; ++ptr){
        if (shift >= 64) {
            return 0;
        }
        unsigned char b = *ptr;
        value |= ((uint64_t)(b & 0x7F)) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
    }
    *out = value;
    return 1;
}