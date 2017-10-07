#ifndef THREAD_POOL_CPP
#define THREAD_POOL_CPP

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <cstring>
#include <queue>
#include <set>
// #include "data_base.cpp"
#include "storageHandler.h"

struct task
{
    int sock_fd;
    //int handle_type;   //0为向下层发送，1为向上层发送
    //char * buf;
};

struct epoll_task
{
    int sock_fd;
    int sock_fd2;   //恒为上层socket
};

extern pthread_t *pid;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern queue<int> task_queue_in;//, task_queue_out; //任务队列
extern int epfd;    //新建epoll文件描述符
extern struct epoll_event *events; //epoll事件集
// extern set<int> unhandle_fds;

//任务处理线程
void * task_handler(void * para);

//初始化线程池
void thread_pool_init(int thread_num);

#endif

















