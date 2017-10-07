#include "ThreadPool.h"

pthread_t *pid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
queue<int> task_queue_in;//, task_queue_out; //任务队列
int epfd;    //新建epoll文件描述符
struct epoll_event *events; //epoll事件集
// set<int> unhandle_fds;

//任务处理线程
void * task_handler(void * para)
{
    pthread_detach(pthread_self());
    while (true)
    {
        pthread_mutex_lock(&mutex); //获取互斥锁

        while (task_queue_in.empty())
        {
            pthread_cond_wait(&cond, &mutex);   //开放互斥锁，线程挂起
        }

        //cout << "!" << endl;
        //For handler
        int tmpfd = task_queue_in.front();
        task_queue_in.pop();
        // unhandle_fds.erase(tmpfd);

        pthread_mutex_unlock(&mutex);   //放开互斥锁
        cout << "Thread: #" << *((int *)para) << endl;
        cout << "Socket fd: #" << tmpfd << endl;
        string s;
        try
        {
            char buffer[1024];
            int recbytes;

            if(-1 == (recbytes = read(tmpfd,buffer,1024)))
            {
                printf("read data fail2!\r\n");
                //continue;
            }
            buffer[recbytes]='\0';
            cout << "Storage read: " << buffer << endl;
            // cout << buffer << endl;

            // char *buf = get_data_from_db(buffer);
            // cout << buf << endl;
            s = string(buffer);
            s = storageHandler(s);
            char *buf = new char[1024];
            copy(s.begin(), s.end(), buf);
            cout << s << endl;

            int tt = 0;
            if(-1 == (tt = write(tmpfd, buf, 1024)))
            {
                cerr << "Fail to write!" << endl;
            }

            delete[] buf;

            // epoll_ctl(epfd, EPOLL_CTL_DEL, tmpfd, NULL);
            // close(tmpfd);
        }
        catch(exception e)
        {
            cerr << "Storage layer down once!" << endl;
        }
        cout << "Thread: #" << *((int *)para) << " finish! @@ " << s << endl;
    }

}

//初始化线程池
void thread_pool_init(int thread_num = 10)
{
    pid = new pthread_t[thread_num];

    for(int i = 0; i < thread_num; ++i)
    {
        int err = 0;
        err = pthread_create(&pid[i], NULL, task_handler, (void *)(new int(i)));    //建立线程池
        if(0 != err)
        {
            cerr << "Fail to create threads because of: " << strerror(err) <<endl;
        }
    }
}