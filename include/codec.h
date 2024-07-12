#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "typedefine.h"

bool varint_decode(Iterator* it, int32_t* out);
bool varlong_decode(Iterator* it, int64_t* out);
bool unsignedshort_decode(Iterator* it, uint16_t* out);
bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str);