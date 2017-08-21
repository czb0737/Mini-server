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
#include "../../json.hpp"
#include <string>
#include <fstream>
#include "split.cpp"
#include <vector>

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

int main(int argc, char *argv[])
{
    int cfd = 0;
    int recbytes =0;
    int sin_size = 0;
    char *buffer;
    struct sockaddr_in s_add, c_add;

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

    // Open file
    fstream file(argv[1]);

    // string user_name, friend_name;
    // string password;
    // string message;
    string token;
    int t = 0, op = 0;
    // Only register or login
    // cout << "Pleast enter type, operating and user_name: " << endl;
    // file >> t >> op >> user_name;
    string ss;

    while(getline(file, ss, '\n'))
    {
        cout << "Test: " << ss << endl;
        vector<string> v = split(ss, ' ');
        t = stoi(v[0]);
        op = stoi(v[1]);

        Json json = Json();
        json["type"] = t;
        json["operating"] = op;
        json["userName"] = v[2];
        if (t == 1)
        {
            json["password"] = v[3];
        }
        else
        {
            json["token"] = token;
        }
        if (t == 2 && op > 1)
        {
            json["userName2"] = v[3];
        }
        if (t == 3 && op == 2)
        {
            json["userName2"] = v[3];
            json["message"] = v[4];
        }

        string s = json.dump();
        cout << s << endl;
        int size = s.size();
        buffer = new char[size + 1];
        s.copy(buffer, size, 0);
        buffer[size] = '\0';

        if(-1 == write(cfd, buffer, size))
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

        if (t == 1 && op == 2)
        {
            auto j = Json::parse(buffer);
            token = j["token"];
        }

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

        // cout << "Pleast enter type, operating and user_name: " << endl;
        // file >> t >> op >> user_name;

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
