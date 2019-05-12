#include "coutput.h"

coutput::coutput()
{};

coutput::~coutput()
{};

void coutput::operator<< (const std::string& rhs)
{
   std::cout << "\33[1;31m" + rhs + "\33[0m";
}

void coutput::foo()
{
    std::cout << "foo";
}
