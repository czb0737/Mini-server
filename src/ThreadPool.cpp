#include "../include/ThreadPool.h"

// Static member initialization
pthread_mutex_t ThreadPool::mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::cond = PTHREAD_COND_INITIALIZER;
queue<int> ThreadPool::task_queue_in;
ThreadPool *ThreadPool::instance = nullptr;
string (*ThreadPool::handler)(string &) = nullptr;

//任务处理线程
void * ThreadPool::task_handler(void * para)
{
    pthread_detach(pthread_self());
    while (true)
    {
        pthread_mutex_lock(&mutex); //获取互斥锁

        while (task_queue_in.empty())
        {
            pthread_cond_wait(&cond, &mutex);   //开放互斥锁，线程挂起
        }

        //For handler
        int tmpfd = task_queue_in.front();
        task_queue_in.pop();

        pthread_mutex_unlock(&mutex);   //放开互斥锁
        string s;
        try
        {
            char buffer[1024];
            int recbytes;

            if(-1 == (recbytes = read(tmpfd,buffer,1024)))
            {
                printf("read data fail2!\r\n");
            }
            buffer[recbytes]='\0';
            s = string(buffer);
            // s = storageHandler(s);
            s = (*handler)(s);
            char *buf = new char[1024];
            copy(s.begin(), s.end(), buf);

            int tt = 0;
            if(-1 == (tt = write(tmpfd, buf, 1024)))
            {
                cerr << "Fail to write!" << endl;
            }

            delete[] buf;
        }
        catch(exception e)
        {
            cerr << "Storage layer down once!" << endl;
        }
    }

}

//初始化线程池
ThreadPool::ThreadPool(int thread_num) : pids(thread_num)
{
    cout << "Initial thread pool!" << endl;
    for(int i = 0; i < thread_num; ++i)
    {
        int err = 0;
        err = pthread_create(&pids[i], NULL, task_handler, nullptr);    //建立线程池
        if(0 != err)
        {
            cerr << "Fail to create threads because of: " << strerror(err) <<endl;
        }
    }
}

void ThreadPool::addTask(int fd)
{
    pthread_mutex_lock(&mutex);
    task_queue_in.push(fd);  //把任务放到任务队列里面
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

ThreadPool * ThreadPool::getInstance()
{
    if (instance == nullptr)
    {
        pthread_mutex_lock(&mutex);
        if (instance == nullptr)
            instance = new ThreadPool(10);
        pthread_mutex_unlock(&mutex);
    }
    return instance;
}