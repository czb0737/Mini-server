#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>

#include "ConnectionPool.h"

using namespace std;

int main(int argc, char *argv[])
{
    ConnectionPool *cp = ConnectionPool::getInstance();
    cp->createConnection();
    // sleep(10);
    cout << string(argv[1]) << endl;
    cout << string(argv[2]) << endl;
    // cp->getData(string(argv[1]));
    auto v = cp->getData(string(argv[2]));
    for (auto s : v)
        cout << s << endl;
}