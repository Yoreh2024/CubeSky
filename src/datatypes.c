#include "datatypes.h"

uint8_t varint_decode(char* buf, uint8_t len, int32_t* out) {
    int32_t value = 0;
    uint8_t shift = 0;
    char* ptr = buf;
    char* end = buf + len;
    for( ; ptr < end; ++ptr){
        if (shift >= 32) {
            return false;
        }
        unsigned char b = *ptr;
        value |= ((uint32_t)(b & 0x7F)) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
    }
    *out = value;
    return (ptr-buf+1);
}

uint8_t varlong_decode(char* buf, uint8_t len, int64_t* out) {
    int64_t value = 0;
    uint8_t shift = 0;
    char* ptr = buf;
    char* end = buf + len;
    for( ; ptr < end; ++ptr){
        if (shift >= 64) {
            return false;
        }
        unsigned char b = *ptr;
        value |= ((uint64_t)(b & 0x7F)) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
    }
    *out = value;
    return (ptr-buf+1);
}

bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str) {
    for (uint8_t i = 0; i < length; i++) {
        sprintf(&hex_str[i * 2], "%02X", data[i]);
    }
    hex_str[length * 2] = '\0';
    return true;
}