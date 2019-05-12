#pragma once
#include "bm_includes"

namespace bmark
{
    template<typename _T, typename _class> 
    class bmark
    {
    public:
        bmark();
        ~bmark();

        /***    Call any function with this argument, and return its benchmark  ***/
        _T      GetFuncBmark(void (_class::*function)(), int nTimes);
        _T      GetFuncBmark(void (_class::*function)(const std::string&), int nTimes, std::string rhs);
        _T      CompareFunc(void (*function)(), void (*function2)(), int nTimes);

    private:
        /***    Not going to use these outside the timing class   ***/
        double  getCPUtime();
        _T      getbmarkSecs();
        _T      getbmarkMilliSecs();
        bmark(const bmark& rhs);
        bmark& operator= (const bmark& rhs);
        /***                                                    ***/

        _T _start, _end;
    };
}
