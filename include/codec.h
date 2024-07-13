#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "typedefine.h"

bool varint_decode(Iterator* it, int32_t* out);
bool varint_encode(int32_t value, Iterator* it);
bool varlong_decode(Iterator* it, int64_t* out);
bool unsignedshort_decode(Iterator* it, uint16_t* out);
bool string_decode(Iterator* it, Iterator* out, int32_t maxlen);
bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str);