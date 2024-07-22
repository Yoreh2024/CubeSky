#include "handlers.h"
void cs_handler(struct ClientData* data){
    Data* it = &data->connect.original_message;

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

        switch(*(it->pos-1)){
        case 0x00:
            //handler_StatusRequest(data);
            reply_StatusResponse(data);
            break;
        case 0x01:
            //handler_PingRequest(data);
            reply_PingResponse(data);
            break;
        }
        break;

    case CLIENT_STATUS_LOGIN:

        switch(*(it->pos-1)){
        case 0x00:
            handler_EncryptionResponse(data);
            if(ServerSetting.encryption_switch){
                //reply_EncryptionRequest(data);
            }
            reply_LoginSuccess(data);
            break;
        case 0x01:
            break;
        case 0x02:
            break;
        case 0x03:
            handler_LoginAcknowledged(data);
            //reply_RegistryData(data);
            break;
        case 0x04:
            break;
        }
        break;

    case CLIENT_STATUS_CONFIGURATION:

        switch(*(it->pos-1)){
        case 0x00:
            handler_ClientInformation(data);
            reply_FinishConfiguration(data);
            break;
        case 0x01:
            break;
        case 0x02:
            handler_ServerboundPluginMessage(data);
            reply_ClientboundPluginMessage(data);
            break;
        case 0x03:
            handler_AcknowledgeFinishConfiguration(data);
            reply_LoginPlay(data);
            //reply_SynchronizePlayerPosition(data);
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:
            break;
        case 0x07:
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
        break;

    case CLIENT_STATUS_PLAYING:

        switch (*(it->pos-1)){
        case 0x00:
            handler_ConfirmTeleportation(data);
            reply_GameEvent(data);
            break;
        }
        break;
    }
}

void cs_reply(struct ClientData* data, struct evbuffer* buf){
    int32_t send_datalen = evbuffer_get_length(buf);
    varint_encode_prepend(buf, send_datalen);
    bufferevent_write_buffer(data->connect.bev, buf);
    evbuffer_free(buf);
}

//握手状态 CLIENT_STATUS_HANDSHAKING

void handler_Handshake(struct ClientData* data){
    Data* it = &data->connect.original_message;
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

//Ping状态 CLIENT_STATUS_STATUS

void handler_StatusRequest(struct ClientData* data){
    
}

void reply_StatusResponse(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x00
    unsignedbyte_encode(buf, "\x00");
    //状态响应内容
    char tmp[]="{\"version\": {\"name\": \"CubeSky 1.21\",\"protocol\": 767},\"description\":\"A Minecraft Server\",\"players\": {\"max\": 100,\"online\": 5}}";
    string_encode(buf, tmp);

    cs_reply(data, buf);
}

void handler_PingRequest(struct ClientData* data){
    
}

void reply_PingResponse(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();

    //包ID 0x01
    unsignedbyte_encode(buf, "\x01");
    //Ping响应
    long_encode(buf, data->connect.original_message.p+1);

    cs_reply(data, buf);
}

//登录状态 CLIENT_STATUS_LOGIN

void handler_EncryptionResponse(struct ClientData* data){
    Data* it = &data->connect.original_message;
    //获取用户名称
    Data name;
    string_decode(it, &name, 16);
    strncpy(data->connect.player_name, name.p, name.len);
    data->connect.player_name[name.len]='\0';

    //获取UUID
    memcpy(data->connect.player_uuid, it->pos, 16);
}

void reply_EncryptionRequest(struct ClientData* data){


}

void reply_LoginSuccess(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x02
    unsignedbyte_encode(buf, "\x02");
    //玩家UUID
    uuid_encode(buf, data->connect.player_uuid);
    //玩家名称
    string_encode(buf, data->connect.player_name);
    //物品数量
    varint_encode(buf, 0);
    //如果客户端认为数据包不正常是否立即断开
    boolean_encode(buf, "\x00");

    cs_reply(data, buf);
}

void handler_LoginAcknowledged(struct ClientData* data){
    data->connect.status++;
}
void reply_RegistryData(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x07
    evbuffer_add(buf, "\x07", 1);
    //

    cs_reply(data, buf);
}

void reply_ClientboundKnownPacks(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
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

    cs_reply(data, buf);
}


//配置状态 CLIENT_STATUS_CONFIGURATION

void handler_ClientInformation(struct ClientData* data){
    ClientInfo* info = &data->connect.client_info;
    Data* it = &data->connect.original_message;
    //客户端语言
    Data tmp;
    string_decode(it, &tmp, 16);
    strncpy(info->locale, tmp.p, tmp.len);
    info->locale[tmp.len] = '\0';
    //客户端渲染距离，以块为单位
    byte_decode(it, &info->view_distance);
    //聊天模式
    int32_t chatmode;
    varint_decode(it, &chatmode);
    info->chat_mode = (int8_t)chatmode;
    //聊天颜色
    boolean_decode(it, &info->chat_colors);
    //显示的皮肤部件
    unsignedbyte_decode(it, &info->skin_parts);
    //主手
    byte_decode(it, &info->main_hand);
    //允许过滤标志和书名上的文本。
    boolean_decode(it, &info->text_filtering);
    //允许被显示在服务器列表中
    boolean_decode(it, &info->allow_serverlistings);
}

void reply_FinishConfiguration(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x03
    unsignedbyte_encode(buf, "\x03");

    cs_reply(data, buf);
}

void handler_ServerboundPluginMessage(struct ClientData* data){
    Data* it = &data->connect.original_message;
    Data channel;
    string_decode(it, &channel, 32767);
    Data channel_data;
    string_decode(it, &channel_data, 32767);
}

void reply_ClientboundPluginMessage(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x0E
    unsignedbyte_encode(buf, "\x01");
    string_encode(buf, "minecraft:brand");
    string_encode(buf, "vanilla");

    cs_reply(data, buf);
}

void handler_AcknowledgeFinishConfiguration(struct ClientData* data){
    data->connect.status++;
}

//游玩状态 CLIENT_STATUS_PLAYING

void handler_ConfirmTeleportation(struct ClientData* data){
    Data* it = &data->connect.original_message;
    //varint_decode(it, &data->connect.teleport_id);
}

void reply_GameEvent(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x22
    unsignedbyte_encode(buf, "\x22");
    //事件ID
    unsignedbyte_encode(buf, "\x13");

    cs_reply(data, buf);
}

void reply_LoginPlay(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID 0x2B
    unsignedbyte_encode(buf, "\x2B");
    //玩家的实体ID（EID）
    int_encode(buf, "\x00\x00\x00\xF6");
    //是否是硬编码的数据
    boolean_encode(buf, "\x00");
    //维度数量
    varint_encode(buf, 1);
    //维度数组
    string_encode(buf, "minecraft:overworld"); //主世界
    //string_encode(buf, "minecraft:the_nether"); //下界
    //string_encode(buf, "minecraft:the_end"); //末地
    //最大玩家数
    varint_encode(buf, 20);
    //视距 2-32
    varint_encode(buf, 10);
    //模拟距离
    varint_encode(buf, 10);
    //减少调试信息
    boolean_encode(buf, "\x00");
    //启用重生屏幕
    boolean_encode(buf, "\x01");
    //玩家是否只能制作他们已经解锁的食谱
    boolean_encode(buf, "\x00");
    //当前所处的维度类型
    varint_encode(buf, 0);
    //维度名称
    string_encode(buf, "minecraft:overworld");
    //散列种子
    evbuffer_add(buf, "\x33\xD2\xA3\x4F\x5E\x5F\x13\x44", 8);
    //游戏模式
    unsignedbyte_encode(buf, "\x00");
    //上一个游戏模式
    unsignedbyte_encode(buf, "\xFF");
    //调试模式
    boolean_encode(buf, "\x00");
    //平坦世界
    boolean_encode(buf, "\x00");
    //有死亡地点
    boolean_encode(buf, "\x00");
    if(0==1){
        //死亡维度名称
        string_encode(buf, "minecraft:overworld");
        //死亡地址坐标
        position_encode(buf, "\xFF\xFF\xF2\x3F\xFF\xFD\x70\x45");
    }
    //传送门冷却时间
    varint_encode(buf, 0);
    //强制执行安全聊天
    boolean_encode(buf, "\x00");

    cs_reply(data, buf);
}

void reply_SynchronizePlayerPosition(struct ClientData* data){
    struct evbuffer* buf = evbuffer_new();
    //包ID
    unsignedbyte_encode(buf, "\x40");
    //X
    double x = 0;
    double_encode(buf, &x);
    //Y
    double y = 0;
    double_encode(buf, &y);
    //Z
    double z = 0;
    double_encode(buf, &z);
    //X轴上的绝对或相对旋转，以度为单位。
    float Yaw = 0;
    float_encode(buf, &Yaw);
    //Y轴上的绝对或相对旋转，以度为单位。
    float Pitch = 0;
    float_encode(buf, &Pitch);
    //标志
    unsignedbyte_encode(buf, "\x00");
    //传送ID
    varint_encode(buf, 1);

    cs_reply(data, buf);
}