#include "handlers.h"
void clientdata_handler(struct ClientData* data){
    Iterator* it = &data->connect.original_message;

    it->pos++;
    switch (data->connect.status){
    case CLIENT_STATUS_HANDSHAKING:

        switch((uint8_t)*(it->pos-1)){
        case 0x00:
            handler_Handshake(data);
            break;
        case 0xFE:
            break;
        }
        break;

    case CLIENT_STATUS_STATUS:

        printf("ping逻辑\n");
        break;

    case CLIENT_STATUS_LOGIN:

        switch(*(it->pos-1)){
        case 0x00:
            handler_EncryptionResponse(data);
            break;
        case 0x01:
            break;
        case 0x02:
            break;
        case 0x03:
            break;
        case 0x04:
            break;
        }

    case CLIENT_STATUS_PLAYING:

        printf("收到游戏数据\n");
        break;

    default:
        break;

    }
    
}

void clientdata_reply(struct ClientData* data){

}

void handler_EncryptionResponse(struct ClientData* data){
    Iterator* it = &data->connect.original_message;
    //获取用户名称
    Iterator name;
    string_decode(it, &name, 16);
    strncpy(data->connect.player_name, name.data, name.length);
    data->connect.player_name[name.length]='\0';

    //获取UUID
    memcpy(data->connect.player_uuid, it->pos, 16);

    //回复客户端消息
    if(ServerSetting.encryption_switch){
        reply_EncryptionRequest(data);
    }
    reply_LoginSuccess(data);
}
void reply_EncryptionRequest(struct ClientData* data){


}

void reply_LoginSuccess(struct ClientData* data){
    struct evbuffer* buf = data->connect.send_buffer = evbuffer_new();
    //包ID 0x02
    uint8_t packid = 0x02;
    evbuffer_add(buf, &packid, 1);
    //玩家UUID
    evbuffer_add(buf, data->connect.player_uuid, 16);
    //玩家名称
    evbuffer_add_printf(buf, "%s", data->connect.player_name);
    //物品数量
    int32_t item_count = 0;
    Iterator item_count_encoded;
    varint_encode(item_count, &item_count_encoded);
    evbuffer_add(buf, item_count_encoded.data, item_count_encoded.length);
    //如果客户端认为数据包不正常是否立即断开
    bool is_disconnect = true;
    evbuffer_add(buf, &is_disconnect, 1);
}

void handler_Handshake(struct ClientData* data){
    Iterator* it = &data->connect.original_message;
    //获取客户端通信协议版本
    varint_decode(it, &data->connect.protocol_ver);

    //跳过接收服务器ip
    int32_t tmp;
    varint_decode(it, &tmp);
    it->pos += tmp;

    //服务器端口号
    unsignedshort_decode(it, &data->connect.server_port);
    
    switch (*it->pos){
    case 0x01:
        data->connect.status++;
        break;
    case 0x02:
        data->connect.status+=2;
        break;
    case 0x03:
        break;
    default:
        break;
    }
    
}