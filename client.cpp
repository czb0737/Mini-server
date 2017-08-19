#ifndef SERVER_CPP_INCLUDED
#define SERVER_CPP_INCLUDED

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "cJSON.c"
/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
*/

using namespace std;

#define server_ip "127.0.0.1"
#define port 8192

#endif // SERVER_CPP_INCLUDE

int main()
{
    int cfd = 0;
    int recbytes =0;
    int sin_size = 0;
    char *buffer;
    struct sockaddr_in s_add, c_add;
    cJSON *json;

    cout << "Hello, welcome to client!" << endl;

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd == -1)
    {
        cout << "Socket fail!" << endl;
        return -1;
    }
    cout << "Socket ok!" << endl;

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr= inet_addr(server_ip);
    s_add.sin_port=htons(port);
    printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);

    if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("connect fail !\r\n");
        return -1;
    }
    printf("connect ok !\r\n");
    char *userName = new char[101];
    char *userName2 = new char[101];
    char *message = new char[101];
    int t = 0;
    cin >> t >> userName >> userName2 >> message;


    while(1)
    {
        buffer = new char[1024];

        json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "type", 3);
        cJSON_AddNumberToObject(json, "operating", t);
        cJSON_AddStringToObject(json, "userName", userName);
        cJSON_AddStringToObject(json, "userName2", userName2);
        cJSON_AddStringToObject(json, "message", message);
        cJSON_AddStringToObject(json, "password", "czb2");
        cJSON_AddStringToObject(json, "token", "9dc32e9084464446afcf3d89209af910");

        buffer = cJSON_Print(json);
        cout << buffer << endl;

        if(-1 == write(cfd, buffer, 1024))
        {
            cout << "Fail to send it !" << endl;
        }

        delete[] buffer;

        buffer = new char[1024];

        if(-1 == (recbytes = read(cfd,buffer,1024)))
        {
            printf("read data fail !\r\n");
            return -1;
        }
        printf("read ok\r\nREC:\r\n");
        buffer[recbytes]='\0';
        printf("%s\r\n",buffer);

        close(cfd);

        cfd = socket(AF_INET, SOCK_STREAM, 0);
        if(cfd == -1)
        {
            cout << "Socket fail!" << endl;
            return -1;
        }
        cout << "Socket ok!" << endl;

        bzero(&s_add,sizeof(struct sockaddr_in));
        s_add.sin_family=AF_INET;
        s_add.sin_addr.s_addr= inet_addr(server_ip);
        s_add.sin_port=htons(port);
        printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);

        delete[] userName;
        delete[] userName2;
        delete[] message;
        userName = new char[101];
        userName2 = new char[101];
        message = new char[101];
        cin >> t >> userName >> userName2 >> message;

        if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
        {
            printf("connect fail !\r\n");
            return -1;
        }
        printf("connect ok !\r\n");
    }

    close(cfd);
    return 0;
}
