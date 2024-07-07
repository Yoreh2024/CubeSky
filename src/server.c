#include "server.h"

int server_listen(){
    struct sockaddr_in server_tcp;
    memset(&server_tcp, 0, sizeof(server_tcp));
    server_tcp.sin_family = AF_INET;
    server_tcp.sin_port = ServerSetting.port;
    server_tcp.sin_addr.s_addr = ServerSetting.ipv4_addr.s_addr;

    struct event_base* base = event_base_new();
    
    event_base_free(base);

    return 0;
}

int server_close(void){

}