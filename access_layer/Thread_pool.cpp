#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include  <pthread.h>
#include  <sys/epoll.h>
#include <cstring>
#include <queue>
#include <asm/errno.h>
#include "access_handler.cpp"

pthread_t *pid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
queue<int> task_queue_in; //任务队列
int epfd;    //新建epoll文件描述符
struct epoll_event *events; //epoll事件集

//任务处理线程
void * task_handler(void * para)
{
    struct epoll_event event;   //临时epoll事件变量
    while(1)
    {
        pthread_mutex_lock(&mutex); //获取互斥锁

        while(task_queue_in.empty())
        {
            pthread_cond_wait(&cond, &mutex);   //开放互斥锁，线程挂起
        }

        //For handler
        int tmpfd = task_queue_in.front();
        task_queue_in.pop();

        pthread_mutex_unlock(&mutex);   //放开互斥锁

        try
        {
            char *buffer = new char[1024];
            int recbytes = 0;

            if(-1 == (recbytes = read(tmpfd,buffer,1024)))
            {
                cout << "Fail to read from client!" << endl;
                //continue;
            }
            buffer[recbytes]='\0';

            char *buf = access_handle(buffer);

            if(-1 == write(tmpfd, buf, 1024))
            {
                cout << "Fail to write to client!" << endl;
            }

            delete[] buffer;
            delete[] buf;

            epoll_ctl(epfd, EPOLL_CTL_DEL, tmpfd, NULL);
            close(tmpfd);
        }
        catch(exception e)
        {
            cout << "Access layer down once!" << endl;
        }

    }

}

//初始化线程池
void thread_pool_init(int thread_num = 10)
{
    pid = new pthread_t[thread_num];

    for(int i = 0; i < thread_num; ++i)
    {
        int err = 0;
        err = pthread_create(&pid[i], NULL, task_handler, NULL);    //建立线程池
        if(0 != err)
        {
            cerr << "Fail to create threads because of: " << strerror(err) <<endl;
        }
    }
}



















