#include "codec.h"

bool void_decode(Data* it, Data* out, uint16_t len){
    if(len > (it->p + it->len - it->pos)){
        return ;
    }
    memcpy(out, it->pos, len);
    it->pos+=len;
    return true;
}

Data varint_decode(Data* it, int32_t* out) {
    char* start = it->pos;
    const unsigned char* end = it->p + it->len;
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

Data varint_encode(struct evbuffer* buf, int32_t value) {
    char tmp[5];
    char* start = tmp;
    while (value >= 0x80) {
        *start++ = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    *start++ = value;
    evbuffer_add(buf, &tmp, start-tmp);
    return true;
}
Data varint_encode_prepend(struct evbuffer* buf, int32_t value){
    char tmp[5];
    char* start = tmp;
    while (value >= 0x80) {
        *start++ = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    *start++ = value;
    evbuffer_prepend(buf, &tmp, start-tmp);
    return true;
}

Data varlong_decode(Data* it, int64_t* out) {
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

Data string_decode(Data* it, Data* out, int32_t maxlen) {
    int32_t len;
    varint_decode(it, &len);
    if ((len > (it->data + it->length - it->pos)) || len < -1 || ( maxlen != -1 && len > maxlen)){
        return false;
    }
    
    out->data = it->pos;
    out->pos = it->pos;
    out->length = len;
    it->pos += len;

    return true;
}

Data string_encode(struct evbuffer* buf, const char* ptr) {
    size_t len = strlen(ptr);
    varint_encode(buf, len);
    evbuffer_add(buf, ptr, len);
    return true;
}