#include "server.h"
void event_read(struct bufferevent* bev, void* client_data) {
    //struct ClientData* data = client_data;
    printf("#%p:%d\n",client_data,*((int*)client_data));
    //struct evbuffer *input = bufferevent_get_input(bev);

    /*printf("#：%d\n",data->connect.message_length);

    if(data->connect.message_length == 0){
        printf("***\n");
        char tmpbuf_datalen[5];
        ssize_t tmp = evbuffer_copyout(input, tmpbuf_datalen, 5);
        uint32_t datalen = 0;
        uint8_t tmp1 = varint_decode(tmpbuf_datalen, tmp, &datalen);
        printf("数据长度：%d\n", datalen);
        evbuffer_drain(input, tmp1);
        
        if(evbuffer_get_length(input) < datalen){
            if(!(1 < datalen < MAX_DATALEN)){
                printf("数据长度不合法\n");
                bufferevent_flush(bev, EV_READ, BEV_FLUSH);
                data->connect.message_length = 0;
                bufferevent_setwatermark(bev, EV_READ, 0, 0);
                return;
            }

            bufferevent_setwatermark(bev, EV_READ, datalen, 0);
            data->connect.message_length = datalen;
            return;
        }
    }

    printf("总数据长度：%d",data->connect.message_length);

    char* buf = (char*)mi_malloc(data->connect.message_length);
    char tmp[2048];
    hex_decode(buf, data->connect.message_length, tmp);
    mi_free(buf);

    printf("收到客户端发来的数据：%s\n", tmp);
    
    bufferevent_setwatermark(bev, EV_READ, 0, 0);*/
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
    
    bufferevent_free(bev);    
    printf("buffevent 资源已经被释放\n"); 
}

void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr){
    struct event_base* base=(struct event_base*)ptr;
    struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);

    struct ClientData client_data;
    memset(&client_data, 0, sizeof(client_data));

    int* a = mi_malloc(sizeof(int));
    *a = 13;
    
    printf("$%p:%d\n",a,*a);
    bufferevent_setcb(bev, event_read, event_write, event_other, a);

    bufferevent_enable(bev, EV_READ);

    mi_free(a);
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