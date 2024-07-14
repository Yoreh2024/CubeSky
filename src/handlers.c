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
            handler_LoginAcknowledged(data);
            break;
        case 0x04:
            break;
        }
    case CLIENT_STATUS_CONFIGURATION:

        switch(*(it->pos-1)){
        case 0x00:
            break;
        case 0x01:
            break;
        case 0x02:
            break;
        case 0x03:
            handler_AcknowledgeFinishConfiguration(data);
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:
            break;
        case 0x07:
            handler_RegistryData(data);
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        case 0x0A:
            break;
        case 0x0B:
            break;
        case 0x0C:
            break;
        case 0x0D:
            break;
        case 0x0E:
            break;
        case 0x0F:
            break;
        case 0x10:
            break;
    }

    case CLIENT_STATUS_PLAYING:

        printf("收到游戏数据\n");
        break;

    default:
        break;

    }
    
}
void handler_AcknowledgeFinishConfiguration(struct ClientData* data){
    data->connect.status++;
    reply_LoginPlay(data);
}

void reply_LoginPlay(struct ClientData* data){
    struct evbuffer* buf = data->connect.send_buffer = evbuffer_new();
    bool is_true = true;
    bool is_false = false;
    //包ID 0x2B
    evbuffer_add(buf, "\x2B", 1);
    //玩家的实体ID（EID）
    int32_t eid = 0;
    evbuffer_add(buf, &eid, 4);
    //Is hardcore 暂不清楚
    evbuffer_add(buf, &is_false, 1);
    //注册表数量
    varint_encode(buf, 0);
    //注册表名称
    //最大玩家数
    varint_encode(buf, 20);
    //视距 2-32
    varint_encode(buf, 2);
    //模拟距离
    varint_encode(buf, 10);
    //减少调试信息
    evbuffer_add(buf, &is_false, 1);
    //启用重生屏幕
    evbuffer_add(buf, &is_false, 1);
    //玩家是否只能制作他们已经解锁的食谱
    evbuffer_add(buf, &is_false, 1);
    //尺寸类型
    varint_encode(buf, 0);
    //注册表名称
    //散列种子
    //游戏模式
    uint8_t game_mode = 0;
    evbuffer_add(buf, &game_mode, 1);
    //调试模式
    evbuffer_add(buf, &is_true, 1);
    //平坦世界
    evbuffer_add(buf, &is_false, 1);
    //有死亡地点
    evbuffer_add(buf, &is_false, 1);
    //传送门冷却时间
    varint_encode(buf, 0);
    //强制执行安全聊天
    evbuffer_add(buf, &is_false, 1);
}

void handler_LoginAcknowledged(struct ClientData* data){
    data->connect.status++;
    reply_RegistryData(data);
}

void handler_RegistryData(struct ClientData* data){
    reply_FinishConfiguration(data);
}

void reply_FinishConfiguration(struct ClientData* data){
    data->connect.send_buffer = evbuffer_new();
    //包ID 0x03
    evbuffer_add(data->connect.send_buffer, "\x03", 1);
}

void reply_RegistryData(struct ClientData* data){
    struct evbuffer* buf = data->connect.send_buffer = evbuffer_new();
    //包ID 0x0E
    evbuffer_add(buf, "\x0E", 1);
    //服务器上存在的数据包的数量
    varint_encode(buf, 1);
    //数据包名
    string_encode(buf, "minecraft");
    //数据包ID
    string_encode(buf, "core");
    //数据包版本
    string_encode(buf, "1.21");
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
    evbuffer_add(buf, "\x02", 1);
    //玩家UUID
    evbuffer_add(buf, data->connect.player_uuid, 16);
    //玩家名称
    string_encode(buf, data->connect.player_name);
    //物品数量
    int32_t item_count = 0;
    varint_encode(buf, item_count);
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