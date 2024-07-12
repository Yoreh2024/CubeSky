#include "codec.h"

bool varint_decode(Iterator* it, int32_t* out) {
    char* start = it->pos;
    const unsigned char* end = it->data + it->length;
    int32_t value = 0;
    uint8_t shift = 0;

    while (start < (char*)end) {
        unsigned char b = *start++;
        value |= (b & 0x7F) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
        if (shift >= 32) {
            return false;
        }
    }

    *out = value;
    it->pos = start;
    return true;
}

bool varlong_decode(Iterator* it, int64_t* out) {
    char* start = it->pos;
    const unsigned char* end = it->data + it->length;
    int64_t value = 0;
    uint8_t shift = 0;

    while (start < (char*)end) {
        unsigned char b = *start++;
        value |= (b & 0x7F) << shift;
        if (!(b & 0x80)) break;
        shift += 7;
        if (shift >= 64) {
            return false;
        }
    }

    *out = value;
    it->pos = start;
    return true;
}

bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str) {
    for (uint8_t i = 0; i < length; i++) {
        sprintf(&hex_str[i * 2], "%02X", data[i]);
    }
    hex_str[length * 2] = '\0';
    return true;
}

bool unsignedshort_decode(Iterator* it, uint16_t* out){
    if(it->pos - it->data < 2){
        return false;
    }
    *out = htons(*((uint16_t*)it->pos));
    it->pos += 2;
    return true;
}