#include <stdint.h>
#include "common.h"

#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

typedef enum {
    CLIENT_STATUS_HANDSHAKING,
    CLIENT_STATUS_STATUS,
    CLIENT_STATUS_LOGIN,
    CLIENT_STATUS_CONFIGURATION,
    CLIENT_STATUS_PLAYING
} ClientStatus;

typedef struct {
    char locale[16];
    int8_t view_distance;
    enum {
        CHAT_MODE_ENABLED,
        CHAT_MODE_COMMANDS_ONLY,
        CHAT_MODE_HIDDEN
    } chat_mode;
    bool chat_colors;
    uint8_t skin_parts;
    enum {
        MAIN_HAND_LEFT,
        MAIN_HAND_RIGHT
    } main_hand;
    bool text_filtering;
    bool allow_serverlistings;
} ClientInfo;

struct connect{
    Data original_message;
    struct evbuffer *send_buffer;
    ClientStatus status;
    int32_t protocol_ver;
    unsigned short server_port;
    char player_name[16];
    uint64_t player_uuid[2];
    ClientInfo client_info;
};

struct player{

};

struct ClientData{
    struct connect connect;
    struct player player;
};
#endif