#ifndef THREAD_POOL_CPP
#define THREAD_POOL_CPP

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class ThreadPool
{
private:
    static pthread_mutex_t mutex;
    static pthread_cond_t cond;
    static queue<int> task_queue_in;//, task_queue_out; //任务队列
    vector<pthread_t> pids;
    
    //任务处理线程
    static void * task_handler(void * para);

    //初始化线程池
    ThreadPool(int thread_num);

public:
    static ThreadPool *instance;
    static ThreadPool *getInstance();
    static string (*handler)(string &);

    void addTask(int fd);
};

#endif

















