#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

// 定义一个结构体来保存每个连接的信息
struct connection_info {
    struct bufferevent *bev; // bufferevent用于读写数据
};

// 当有新连接时调用的回调函数
void accept_connection(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);
// 当从客户端读取到数据时调用的回调函数
void read_callback(struct bufferevent *bev, void *ctx);
// 当发生错误或其他事件时调用的回调函数
void event_callback(struct bufferevent *bev, short events, void *ctx);
//监听客户端发给服务器的消息
int server_listen(void);
//关闭服务器
int server_close(void);