#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <event2/buffer.h>
#include "typedefine.h"
#include "common.h"
#include <yyjson.h>

typedef bool Boolean;
typedef int8_t Byte;
typedef uint8_t UnsignedByte;
typedef int16_t Short;
typedef uint16_t UnsignedShort;
typedef int32_t Int;
typedef int64_t Long;
typedef float Float;
typedef double Double;
typedef Data TextComponent;
typedef yyjson_mut_doc* JSONTextComponent;
typedef Data Identifier;
typedef int32_t VarInt;
typedef int64_t VarLong;
typedef Data EntityMetadata;
typedef Data Slot;
typedef Data NBT;
typedef struct{
    int64_t x : 26;
    int64_t z : 26;
    int64_t y : 12;
} Position;
typedef uint8_t Angle;
typedef __uint128_t UUID;
typedef Data BitSet;
typedef Data FixedBitSet;
typedef Data Optional;
typedef Data Array;
typedef Data Enum;
typedef Data ByteArray;

#define boolean_decode(it, out) void_decode(it, out, sizeof(Boolean))
#define byte_decode(it, out) void_decode(it, out, sizeof(Byte))
#define unsignedbyte_decode(it, out) void_decode(it, out, sizeof(UnsignedByte))
#define short_decode(it, out) void_decode(it, out, sizeof(Short))
#define unsignedshort_decode(it, out) void_decode(it, out, sizeof(UnsignedShort))
#define int_decode(it, out) void_decode(it, out, sizeof(Int))
#define long_decode(it, out) void_decode(it, out, sizeof(Long))
#define float_decode(it, out) void_decode(it, out, sizeof(Float))
#define double_decode(it, out) void_decode(it, out, sizeof(Double))
#define position_decode(it, out) void_decode(it, out, sizeof(Position))
#define angle_decode(it, out) void_decode(it, out, sizeof(Angle))
#define uuid_decode(it, out) void_decode(it, out, sizeof(UUID))

bool hex_decode(const uint8_t *data, uint8_t length, char* hex_str);

bool void_decode(Iterator* it, Data* out, uint16_t len);
bool string_decode(Iterator* it, Data* out, int32_t maxlen);
bool varint_decode(Iterator* it, VarInt* out);
bool varlong_decode(Iterator* it, VarLong* out); 

bool void_encode(Iterator* it, void* out, uint16_t len);
bool string_encode(struct evbuffer* buf, const char* ptr);
bool varint_encode(struct evbuffer* buf, int32_t value);
bool varint_encode_prepend(struct evbuffer* buf, int32_t value);


