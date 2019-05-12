#include "../benchmarking/benchmark.h"
#include "../testOutput/coutput.h"

int main(int argc, char *argv[])
{
    bmark::bmark<double, coutput> b;
    auto a = b.GetFuncBmark(&coutput::foo, 10 );

    std::cout << ' ' << a << std::endl;
    return 0;
}
