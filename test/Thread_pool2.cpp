#include <iostream>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
//#include <string.h>
#include <cstring>

using namespace std;

class Thread_pool
{
private:

    int thread_num;
    pthread_t *pid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

public:

    static int id;

    Thread_pool(){}

    void thread_pool_init(int num = 10)
    {
        mutex = PTHREAD_MUTEX_INITIALIZER;
        cond = PTHREAD_COND_INITIALIZER;
        thread_num = num;
        pid = new pthread_t(thread_num);

        for(int i = 0; i < thread_num; ++i)
        {
            int err = 0;
            err = pthread_create(&pid[i], NULL, task_handler, NULL);
            if(0 != err)
            {
                cerr << "Fail to create threads because of: " << strerror(err) <<endl;
            }
        }
    }

    void * task_handler(void * para)
    {
        //for function
        int sid = id++;

        while(1)
        {
            pthread_mutex_lock(&mutex);
            cout << sid <<endl;
            sleep(2);
        }

    }

};

int Thread_pool::id = 0;

int main()
{
    Thread_pool tp = Thread_pool();
    tp.thread_pool_init();

    while(1){}
}




















