#include <iostream>

using namespace std;

int kkk = 5;

class Linker
{
private:
    int size;
public:
    Linker();
    void inc();
    int getSize();
    static void getInstance();
};