#include "handlers.h"
void clientdata_handler(struct ClientData* data){
    Iterator* it = &data->connect.original_message;
    it->pos++;
    switch (data->connect.status){
    case CLIENT_STATUS_HANDSHAKING:

        switch((uint8_t)*(it->pos-1)){
        case 0x00:
            handler_handshake(data);
            break;
        case 0xFE:
            break;
        }
        break;

    case CLIENT_STATUS_STATUS:

        printf("收到状态数据\n");
        break;

    case CLIENT_STATUS_LOGIN:

        printf("收到登录数据\n");
        break;

    case CLIENT_STATUS_PLAYING:

        printf("收到游戏数据\n");
        break;

    default:
        break;

    }
}
void handler_handshake(struct ClientData* data){
    //获取客户端通信协议版本
    Iterator* it = &data->connect.original_message;
    varint_decode(it, &data->connect.protocol_ver);

    //跳过接收服务器ip
    int32_t tmp;
    varint_decode(it, &tmp);
    it->pos += tmp;

    //服务器端口号
    unsignedshort_decode(it, &data->connect.server_port);
    
    switch (*it->pos){
    case 0x01:
        break;
    case 0x02:
        data->connect.status++;
        break;
    case 0x03:
        break;
    default:
        break;
    }
    
}