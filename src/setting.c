#include "common.h"
#include "setting.h"

struct GameDataStruct GameData = {0};
struct ServerSettingStruct ServerSetting = {0};
struct PlayerDataStruct PlayerData = {0};
struct ServerModuleStruct ServerModule = {0};

int server_setting(void){
    ServerSetting.ipv4_addr.s_addr = inet_addr("0.0.0.0");
    ServerSetting.port = htons(25565);
    ServerSetting.tcpserver_switch = 1;
}