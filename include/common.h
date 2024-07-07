#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#ifndef COMMON_H
#define COMMON_H
struct GameDataStruct{
    int player_max;
    int player_count;
};
extern struct GameDataStruct GameData;

struct ServerSettingStruct{
    uint64_t port:16;               //服务器端口号
    uint64_t tcpserver_switch:1;    //允许java版玩家加入
    uint64_t udpserver_switch:1;    //允许基岩版玩家加入
    uint64_t cpu_quantity:4;        //服务器cpu数量
    uint64_t gpuhasten_switch:1;    //是否使用GPU
    uint64_t ipv4_switch:1;         //是否启用ipv4
    uint64_t ipv6_switch:1;         //是否启用ipv6
    struct in_addr ipv4_addr;       //服务器ipv4地址
    struct in6_addr ipv6_addr;      //服务器ipv6地址
};
extern struct ServerSettingStruct ServerSetting;

struct PlayerDataStruct{
    uint8_t status;
} ;
extern struct PlayerDataStruct PlayerData;

struct ServerModuleStruct{
    uint8_t tcpserver_switch:1;    //反作弊

};
extern struct ServerModuleStruct ServerModule;
#endif