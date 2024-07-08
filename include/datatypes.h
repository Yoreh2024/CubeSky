#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

bool varint_decode(char* buf, uint8_t len, int32_t* out);
bool varlong_decode(char* buf, uint8_t len, int64_t* out);
bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str);