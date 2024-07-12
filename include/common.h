#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <mimalloc.h>

#define MAX_DATALEN 1048576

#ifndef COMMON_H
#define COMMON_H

struct ServerSettings{
    int player_max;                 // 最大玩家数量
    uint16_t port;                  // 服务器端口号
    bool tcpserver_switch;          // 允许Java版玩家加入
    bool udpserver_switch;          // 允许基岩版玩家加入
    uint8_t cpu_quantity;           // 服务器CPU数量
    bool gpuhasten_switch;          // 是否使用GPU
    bool ipv4_switch;               // 是否启用IPv4
    bool ipv6_switch;               // 是否启用IPv6
    struct in_addr ipv4_addr;       // 服务器IPv4地址
    struct in6_addr ipv6_addr;      // 服务器IPv6地址
};

struct ServerData{
    int player_count;               // 当前玩家数量
};

struct ServerModules{
    bool anti_cheat;                // 反作弊
};

extern struct ServerSettings ServerSetting;
extern struct ServerData ServerData;
extern struct ServerModules ServerModules;

#endif