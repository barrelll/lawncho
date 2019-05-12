#pragma once
#include <iostream>

class coutput
{
    public:
    coutput();
    ~coutput();
    void operator<< (const std::string& rhs);

    void foo();

    private:
    coutput(const coutput& rhs);
    coutput operator= (const coutput& rhs);
};
