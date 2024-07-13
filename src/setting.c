#include "common.h"
#include "setting.h"

struct ServerSettings ServerSetting={0};
struct ServerData ServerData={0};
struct ServerModules ServerModules={0};

int server_setting(void){
    ServerSetting.ipv4_addr.s_addr = inet_addr("0.0.0.0");
    ServerSetting.port = htons(25565);
    ServerSetting.tcpserver_switch = 1;
    ServerSetting.encryption_switch = 0;
}

int server_close(void){

}