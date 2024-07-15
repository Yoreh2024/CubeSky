#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <event2/buffer.h>
#include "typedefine.h"
#include "common.h"

bool varint_decode(Iterator* it, int32_t* out);
bool varint_encode(struct evbuffer* buf, int32_t value);
bool varint_encode_prepend(struct evbuffer* buf, int32_t value);
bool varlong_decode(Iterator* it, int64_t* out);
bool unsignedshort_decode(Iterator* it, uint16_t* out);
bool string_decode(Iterator* it, Iterator* out, int32_t maxlen);
bool string_encode(struct evbuffer* buf, const char* ptr);
bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str);
bool byte_decode(Iterator* it, int8_t* out);
bool buf_decode(Iterator* it, void* out, uint16_t len);