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
#include "json.hpp"
#include <string>
#include "logic_layer/message/connect.cpp"
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

using Json = nlohmann::json;
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

    cout << "Hello, welcome to client!" << endl;

    // cfd = socket(AF_INET, SOCK_STREAM, 0);
    // if(cfd == -1)
    // {
    //     cout << "Socket fail!" << endl;
    //     return -1;
    // }
    // cout << "Socket ok!" << endl;

    // bzero(&s_add,sizeof(struct sockaddr_in));
    // s_add.sin_family=AF_INET;
    // s_add.sin_addr.s_addr= inet_addr(server_ip);
    // s_add.sin_port=htons(port);
    // printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);

    // if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    // {
    //     printf("connect fail !\r\n");
    //     return -1;
    // }
    // printf("connect ok !\r\n");
    string user_name, friend_name;
    string password;
    string message;
    string token;
    int t = 0, op = 0;
    // // Only register or login
    // cout << "Pleast enter type, operating and user_name: " << endl;
    // cin >> t >> op >> user_name;
    // cfd = connect_to_server(8192);
    while(true)
    {
        printf("s_addr = %#x ,port : %#x\r\n",s_add.sin_addr.s_addr,s_add.sin_port);

        cout << "Pleast enter type, operating and user_name: " << endl;
        cin >> t >> op >> user_name;
        if (t == 4) break;
        Json json;
        json["type"] = t;
        json["operating"] = op;
        json["userName"] = user_name;
        if (t == 1)
        {
            cout << "Pleast enter password: " << endl;
            cin >> password;
            json["password"] = password;
        }
        else
        {
            json["token"] = token;
        }
        if (t == 2 && op > 1)
        {
            cout << "Pleast enter your friend's name: " << endl;
            cin >> friend_name;
            json["userName2"] = friend_name;
        }
        if (t == 3 && op == 2)
        {
            cout << "Pleast enter your friend's name and message: " << endl;
            cin >> friend_name >> message;
            json["userName2"] = friend_name;
            json["message"] = message;
        }

        string s = json.dump();
        cout << s << endl;
        int size = s.size();
        buffer = new char[size + 1];
        s.copy(buffer, size, 0);
        buffer[size] = '\0';

        cfd = connect_to_server(8192);
        // cfd = socket(AF_INET, SOCK_STREAM, 0);
        // if(cfd == -1)
        // {
        //     cout << "Socket fail!" << endl;
        //     return -1;
        // }
        // cout << "Socket ok!" << endl;

        // bzero(&s_add,sizeof(struct sockaddr_in));
        // s_add.sin_family=AF_INET;
        // s_add.sin_addr.s_addr= inet_addr(server_ip);
        // s_add.sin_port=htons(port);

        // if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
        // {
        //     printf("connect fail !\r\n");
        //     return -1;
        // }
        // printf("connect ok !\r\n");
        cout << "!" << endl;
        if(-1 == write(cfd, buffer, size))
        {
            cout << "Fail to send it !" << endl;
        }

        delete[] buffer;

        buffer = new char[1024];
        cout << "@" << endl;
        if(-1 == (recbytes = read(cfd,buffer,1024)))
        {
            printf("read data fail !\r\n");
            return -1;
        }
        printf("read ok\r\nREC:\r\n");
        buffer[recbytes]='\0';
        cout << "$: " << recbytes << endl;
        cout << buffer + 100 << endl;
        printf("%s\r\n",buffer);
        cout << "%" << endl;

        if (t == 1 && op == 2)
        {
            auto j = Json::parse(buffer);
            token = j["token"];
        }
        delete buffer;
        // close(cfd);
    }

    close(cfd);
    return 0;
}
