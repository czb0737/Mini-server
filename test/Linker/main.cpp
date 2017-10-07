#include <iostream>
#include "Linker.h"

using namespace std;

int main()
{
    Linker::getInstance();
    Linker link;
    link.inc();
    cout << link.getSize() << endl;
}