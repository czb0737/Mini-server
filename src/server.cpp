#include "../include/ThreadPool.h"
#include "../include/handler/accessHandler.h"
#include "../include/handler/userHandler.h"
#include "../include/handler/friendHandler.h"
#include "../include/handler/messageHandler.h"
#include "../include/handler/storageHandler.h"

#define MAX_EVENTS 10000    // Max number of events for epoll

ThreadPool *thread_pool = nullptr;

int initialization(string &);

int main(int argc, char **argv)
{
    string module = string(argv[1]);
    int port = initialization(module);
    if (port == -1)
    {
        cerr << "Invalid module name !" << endl;
    }

    int sfp, nfp;   //sfp为 server用于监听的socket，nfp为server在accept到请求后的socket
    struct sockaddr_in s_add, c_add;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    struct epoll_event event;   //临时epoll事件变量

    cout << "Hello,welcome to my server !" << endl;

    //建立socket
    sfp = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfp)
    {
        cerr << "socket fail !" << endl;
        return -1;
    }
    cout << "socket ok !" << endl;

    bzero(&s_add,sizeof(struct sockaddr_in));
    s_add.sin_family=AF_INET;
    s_add.sin_addr.s_addr=htonl(INADDR_ANY);
    s_add.sin_port=htons(port);

    //绑定端口
    if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        cerr << "bind fail !" << endl;
        return -1;
    }
    cout << "bind ok !" << endl;

    //监听socket
    if(-1 == listen(sfp, 10))
    {
        cerr << "listen fail !" << endl;
        return -1;
    }
    cout << "listen ok !" << endl;

    //新建epoll
    int epfd = epoll_create(MAX_EVENTS);
    if(-1 == epfd)
    {
        cerr << "Fail to create epoll !" << endl;
    }

    struct epoll_event *events = NULL;
    events = new epoll_event[MAX_EVENTS];

    // Set up thread pool
    thread_pool = ThreadPool::getInstance();

    event.data.fd = sfp;
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sfp, &event);

    while(1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, 5);

        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == sfp)
            {
                nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);
                // cout << "Storage accept fd IDs: " << nfp << endl;
                if(-1 == nfp)
                {
                    printf("Storage fail to accept!\n");
                }

                event.data.fd = nfp;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, nfp, &event);

            }

            else if(events[i].events & EPOLLIN)
            {
                //通过互斥锁把任务放到任务队列里
                int sockfd = events[i].data.fd;
                thread_pool->addTask(sockfd);
            }

        }

    }
    close(sfp);
    close(epfd);
    delete events;
    return 0;
}

int initialization(string &s)
{
    int port = 0;
    if (s == "Access")
    {
        port = ACCESS_PORT;
        ThreadPool::handler = &accessHandler;
    }
    else if (s == "User")
    {
        port = USER_PORT;
        ThreadPool::handler = &userHandler;
    }
    else if (s == "Friend")
    {
        port = FRIEND_PORT;
        ThreadPool::handler = &friendHandler;
    }
    else if (s == "Message")
    {
        port = MESSAGE_PORT;
        ThreadPool::handler = &messageHandler;
    }
    else if (s == "Storage")
    {
        port = STORAGE_PORT;
        ThreadPool::handler = &storageHandler;
    }
    else    return -1;
    return port;
}