#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

using namespace std;

#define server_ip "127.0.0.1"
#define user_port 8193
#define friend_port 8194
#define message_port 8195

int connect_to_server(int port2)
{
    int cfd = 0;
    struct sockaddr_in s_add, c_add;

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1)
    {
        cout << "Socket fail!" << endl;
        return -1;
    }

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr= inet_addr(server_ip);
    s_add.sin_port=htons(port2);

    if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        cout << "connect fail !\r\n" << endl;
        return -1;
    }

    return cfd;
}


