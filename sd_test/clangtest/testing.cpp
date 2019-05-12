#include <vector>

namespace foot {
class testing 
{
    public:
        int foo();
    private:
        float foo2();
        int a;
};

float foo2()
{ return 1.2; }

int testing::foo()
{ int a = 1; return a; }

float newFunction(int i, int a)
{
    return float( i + a );
}

std::vector< float > poop;

int main(int argc, char** argv)
{
    testing a;
    int i = a.foo();
    i = 0;
    return 0;
}
}
