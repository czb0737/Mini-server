#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include  <pthread.h>
#include  <sys/epoll.h>
#include <cstring>
#include <queue>
#include <asm/errno.h>

using namespace std;

#define server_ip "127.0.0.1"   //服务器IP地址
#define port 8197   //服务器接入层端口
#define MAX_EVENTS 10000    //最大事件数

int main()
{
    int epfd;    //新建epoll文件描述符
    int sfp, nfp;   //sfp为 server用于监听的socket，nfp为server在accept到请求后的socket
    struct sockaddr_in s_add,c_add;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    struct epoll_event event;   //临时epoll事件变量
    struct epoll_event *events; //epoll事件集
    char buffer[1024];
    int recbytes;

    printf("Hello,welcome to my server !\r\n");

    //建立socket
    sfp = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfp)
    {
        printf("socket fail ! \r\n");
        return -1;
    }
    printf("socket ok !\r\n");

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);
    s_add.sin_port=htons(port);

    //绑定端口
    if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("bind fail !\r\n");
        return -1;
    }
    printf("bind ok !\r\n");

    //监听socket
    if(-1 == listen(sfp, 10))
    {
        printf("listen fail !\r\n");
        return -1;
    }
    printf("listen ok\r\n");

    //新建epoll
    epfd = epoll_create(MAX_EVENTS);
    if(-1 == epfd)
    {
        cout << "Succeed to create epoll !" <<endl;
    }

    event.data.fd = sfp;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfp, &event);
    events = new epoll_event[MAX_EVENTS];

    while(1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, 500);

        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == sfp)
            {
                nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);

                if(-1 != nfp)
                {
                    cout << "Success!" << endl;
                }

/*                if(-1 == write(nfp,"hello,welcome to my server \r\n",32))
                {
                    printf("write fail!\r\n");
                    return -1;
                }
                printf("write ok!\r\n");*/

                event.data.fd = nfp;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, nfp, &event);

            }

            else if(events[i].events & EPOLLIN)
            {
                int tmpfd = events[i].data.fd;
                if(-1 == (recbytes = read(tmpfd,buffer,1024)))
                {
                    printf("read data fail !\r\n");
                }
                buffer[recbytes]='\0';
                cout << buffer << endl;

                //删除掉对应的epoll事件
                event.data.fd = tmpfd;
                event.events = EPOLLOUT | EPOLLONESHOT;
                epoll_ctl(epfd, EPOLL_CTL_MOD, tmpfd, &event);
            }

            else if(events[i].events&EPOLLOUT) //有数据待发送，写socket
            {
                int tmpfd = events[i].data.fd;
                if(-1 == write(tmpfd, buffer, 1024))
                {
                    cout << "Fail to send it !" << endl;
                }

  /*             event.data.fd=tmpfd;
               event.events=EPOLLIN;
               epoll_ctl(epfd,EPOLL_CTL_MOD,tmpfd,&event); //修改标识符，等待下一个循环时接收数据*/
            }

        }

    }
    close(sfp);
    close(epfd);
    return 0;
}
