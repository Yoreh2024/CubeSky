#include "server.h"
void event_read(struct bufferevent* bev, void* client_data) {
    struct ClientData* data = client_data;
    struct evbuffer *input = bufferevent_get_input(bev);

    uint32_t datalen = 0;
    if(data->connect.original_message.length == 0){
        char tmpbuf_datalen[5];

        ssize_t tmp = evbuffer_copyout(input, tmpbuf_datalen, 5);

        Iterator tmpbuf_tmp={
            .data=tmpbuf_datalen,
            .pos=tmpbuf_datalen,
            .length=tmp
        };
        varint_decode(&tmpbuf_tmp, &datalen);
        printf("数据长度：%d\n", datalen);

        evbuffer_drain(input, tmpbuf_tmp.pos-tmpbuf_tmp.data);
        
        if(evbuffer_get_length(input) < datalen){
            if(datalen == 0 || datalen > MAX_DATALEN){
                printf("数据长度不合法\n");
                bufferevent_flush(bev, EV_READ, BEV_FLUSH);
                data->connect.original_message.length = 0;
                bufferevent_setwatermark(bev, EV_READ, 0, 0);
                return;
            }

            bufferevent_setwatermark(bev, EV_READ, datalen, 0);
            return;
        }
    }
    
    char* buf = (char*)mi_malloc(datalen);
    bufferevent_read(bev, buf, datalen);

    char tmp[2048];
    hex_decode(buf, datalen, tmp);
    printf("收到客户端发来的数据：%s\n", tmp);

    data->connect.original_message.data = buf;
    data->connect.original_message.pos = buf;
    data->connect.original_message.length = datalen;
    //接收消息并处理
    clientdata_handler(data);
    //如果有数据需要发送
    if(data->connect.send_buffer != NULL){
        int32_t send_datalen = evbuffer_get_length(data->connect.send_buffer);
        varint_encode_prepend(data->connect.send_buffer, send_datalen);
        bufferevent_write_buffer(bev, data->connect.send_buffer);
        evbuffer_free(data->connect.send_buffer);
        data->connect.send_buffer=NULL;
    }
    
    mi_free(buf);
    bufferevent_setwatermark(bev, EV_READ, 0, 0);
    data->connect.original_message.length = 0;
}

void event_write(struct bufferevent* bev, void* client_data){
    printf("成功将数据写给客户端\n");
}

void event_other(struct bufferevent* bev, short events, void* client_data){
    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");  
    }
    else if(events & BEV_EVENT_ERROR)   
    {
        printf("some other error\n");
    }

    mi_free(client_data);
    bufferevent_free(bev);    
    printf("buffevent 资源已经被释放\n"); 
}

void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr){
    struct event_base* base=(struct event_base*)ptr;
    struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);

    //-> 在event_other中被释放
    struct ClientData* client_data = mi_malloc(sizeof(struct ClientData));
    memset(client_data, 0, sizeof(client_data));

    bufferevent_setcb(bev, event_read, event_write, event_other, client_data);

    bufferevent_enable(bev, EV_READ | EV_PERSIST);

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