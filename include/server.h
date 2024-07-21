#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "typedefine.h"
#include "handlers.h"
#include "codec.h"
// 当从客户端读取到数据时调用的回调函数
void event_read(struct bufferevent *bev, void* client_data);
// 成功向客户端发送数据时调用的回调函数
void event_write(struct bufferevent *bev, void* client_data);
// 当发生错误或其他事件时调用的回调函数
void event_other(struct bufferevent *bev, short events, void* client_data);
//监听客户端发给服务器的消息
int server_listen(void);
//关闭服务器
int server_close(void);