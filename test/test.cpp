#ifndef TEST_CPP_INCLUDED
#define TEST_CPP_INCLUDED

#include <iostream>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
//#include <string.h>
#include <cstring>

using namespace std;

void * multi(void * para)
{
    int *tmp = (int*) para;

    while(1)
    {
        cout << *tmp << endl;
        sleep(2);
    }

    return 0;
}

int main()
{
    pthread_t pt[10];

    for(int i = 0; i < 10; ++i)
    {
        pthread_create(&pt[i], NULL, multi, (void *) &i);
    }

    sleep(20);

}


#endif // TEST_CPP_INCLUDED
