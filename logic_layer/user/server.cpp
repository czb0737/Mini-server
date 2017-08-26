#include "Thread_pool.cpp"

#define port 8193   //服务器接入层端口
#define MAX_EVENTS 10000    //最大事件数

int main()
{
    int sfp, nfp;   //sfp为 server用于监听的socket，nfp为server在accept到请求后的socket
    struct sockaddr_in s_add,c_add;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    struct epoll_event event;   //临时epoll事件变量

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

    events = NULL;
    events = new epoll_event[MAX_EVENTS];

    thread_pool_init(20);

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
                // cout << "User module accept!" << endl;
                if(-1 == nfp)
                {
                    printf("Fail to accept!\n");
                }

                event.data.fd = nfp;
                event.events = EPOLLIN | EPOLLONESHOT;
                epoll_ctl(epfd, EPOLL_CTL_ADD, nfp, &event);

            }

            else if(events[i].events & EPOLLIN)
            {
                int sockfd = events[i].data.fd;

                //通过互斥锁把任务放到任务队列里
                pthread_mutex_lock(&mutex);
                task_queue_in.push(sockfd);  //把任务放到任务队列里面
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

   /*             //删除掉对应的epoll事件
                event.data.fd = tmpfd;
                event.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_DEL, tmpfd, &event);*/
            }

        }

    }
    close(sfp);
    close(epfd);
    free(events);
    return 0;
}

