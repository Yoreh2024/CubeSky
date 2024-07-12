#include <stdint.h>

#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

typedef struct {
    const char* data;
    char* pos;
    uint32_t length;
} Iterator;

typedef enum {
    CLIENT_STATUS_HANDSHAKING,
    CLIENT_STATUS_STATUS,
    CLIENT_STATUS_LOGIN,
    CLIENT_STATUS_PLAYING
} ClientStatus;

struct connect{
    Iterator original_message;
    ClientStatus status;
    int32_t protocol_ver;
    unsigned short server_port;
};

struct player{

};

struct ClientData{
    struct connect connect;
    struct player player;
};

#endif