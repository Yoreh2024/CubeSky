#include "main.h"

int main(){
    server_init();
    server_listen();
    server_close();
    printf("Server closed\n");
}

int server_init(){
    server_setting();
}

