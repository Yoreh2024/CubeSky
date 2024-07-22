#include "server.h"
void event_read(struct bufferevent* bev, void* client_data) {
    ((struct ClientData*)client_data)->connect.bev = bev;
    struct evbuffer *input = bufferevent_get_input(bev);

    while(evbuffer_get_length(input) != 0){
        struct ClientData* data = client_data;

        uint32_t datalen = 0;
        //上轮是否处理了完整消息（是否有消息残余）
        if(data->connect.original_message.len == 0){
            char tmpbuf_datalen[5];

            ssize_t tmp = evbuffer_copyout(input, tmpbuf_datalen, 5);

            Data tmpbuf_tmp={
                .p=tmpbuf_datalen,
                .pos=tmpbuf_datalen,
                .len=tmp
            };
            varint_decode(&tmpbuf_tmp, &datalen);
            printf("数据长度:%d\n", datalen);

            evbuffer_drain(input, tmpbuf_tmp.pos-tmpbuf_tmp.p);
            

            if(evbuffer_get_length(input) < datalen){
                if(datalen == 0 || datalen > MAX_DATALEN){
                    printf("数据长度不合法\n");
                    bufferevent_flush(bev, EV_READ, BEV_FLUSH);
                    data->connect.original_message.len = 0;
                    bufferevent_setwatermark(bev, EV_READ, 0, 0);
                    return;
                }

                printf("缓冲区数据长度:%d\n", evbuffer_get_length(input));
                data->connect.original_message.len = datalen;
                bufferevent_setwatermark(bev, EV_READ, datalen, 0);
                return;
            }
        }else{
            //上一轮解析出的数据长度
            datalen = data->connect.original_message.len;
        }
        
        char* buf = (char*)mi_malloc(datalen);
        bufferevent_read(bev, buf, datalen);

        char tmp[2048];
        hex_decode(buf, datalen, tmp);
        printf("收到客户端发来的数据：%s\n", tmp);

        data->connect.original_message.p = buf;
        data->connect.original_message.pos = buf;
        data->connect.original_message.len = datalen;
        //接收消息并处理
        cs_handler(data);
        
        mi_free(buf);
        bufferevent_setwatermark(bev, EV_READ, 0, 0);
        data->connect.original_message.len = 0;
    }
    
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