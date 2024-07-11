/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: test.proto */

#ifndef PROTOBUF_C_test_2eproto__INCLUDED
#define PROTOBUF_C_test_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1005000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct NetworkMessage NetworkMessage;


/* --- enums --- */


/* --- messages --- */

struct  NetworkMessage
{
  ProtobufCMessage base;
  uint32_t packet_id;
  uint32_t protocol_version;
  char *server_ip;
  uint32_t server_port;
  uint32_t next_state_id;
};
#define NETWORK_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&network_message__descriptor) \
, 0, 0, (char *)protobuf_c_empty_string, 0, 0 }


/* NetworkMessage methods */
void   network_message__init
                     (NetworkMessage         *message);
size_t network_message__get_packed_size
                     (const NetworkMessage   *message);
size_t network_message__pack
                     (const NetworkMessage   *message,
                      uint8_t             *out);
size_t network_message__pack_to_buffer
                     (const NetworkMessage   *message,
                      ProtobufCBuffer     *buffer);
NetworkMessage *
       network_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   network_message__free_unpacked
                     (NetworkMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*NetworkMessage_Closure)
                 (const NetworkMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor network_message__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_test_2eproto__INCLUDED */
