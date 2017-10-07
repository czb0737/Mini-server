#include "Linker.h"

void Linker::getInstance()
{
    cout << "!!" << endl;
}

Linker::Linker()
{
    size = 0;
}

void Linker::inc()
{
    ++size;
}

int Linker::getSize()
{
    return size;
}