#include "server.h"
void event_read(struct bufferevent *bev, void *arg) {  
    char buf[1024];
    uint8_t tmplen = bufferevent_read(bev, buf, 5);
    buf[tmplen] = '\0';

    int32_t len;
    varint_decode(buf, tmplen, &len);
    char* buf_ptr = buf + tmplen - len;

    if(len > 5-(tmplen-len)){
        bufferevent_read(bev, buf, (1024<len)? 1024: len);
    }

    char hex_buf[2048];
    hex_decode(buf_ptr, len, hex_buf);

    printf("%s\n", hex_buf);
}

void event_write(struct bufferevent *bev, void *data){
    printf("成功将数据写给客户端\n");
}

void event_other(struct bufferevent *bev, short events, void *data){
    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");  
    }
    else if(events & BEV_EVENT_ERROR)   
    {
        printf("some other error\n");
    }
    
    bufferevent_free(bev);    
    printf("buffevent 资源已经被释放\n"); 
}

void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr){
    struct event_base* base=(struct event_base*)ptr;
    struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    
    bufferevent_setcb(bev,event_read,event_write,event_other,NULL);
    bufferevent_enable(bev, EV_READ);
    return ;
}

int server_listen(){
    struct event_base* base = event_base_new();

    struct sockaddr_in server_tcp;
    memset(&server_tcp, 0, sizeof(server_tcp));
    server_tcp.sin_family = AF_INET;
    server_tcp.sin_port = ServerSetting.port;
    server_tcp.sin_addr.s_addr = ServerSetting.ipv4_addr.s_addr;
    struct evconnlistener* listener_tcp = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 20, (struct sockaddr *)&server_tcp, sizeof(server_tcp)); 
    
    event_base_dispatch(base);

    evconnlistener_free(listener_tcp);
    event_base_free(base);

    return 0;
}

int server_close(void){

}