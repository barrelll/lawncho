#include "benchmark.h"

namespace bmark
{
    /***            ***/
    template<typename _T, typename _class> 
    bmark<_T, _class>::bmark()
        :
            _start(0),
            _end(0)
    {};

    template<typename _T, typename _class> 
    bmark<_T, _class>::~bmark()
    {
        if (_start)
            _start = 0;
        if ( _end )
            _end   = 0;
    };
    /***            ***/

    template<typename _T, typename _class> 
    double  bmark<_T, _class>::getCPUtime() // Returns time calculation in seconds
    {
        return _start = omp_get_wtime();
    }

    template<typename _T, typename _class>
    _T      bmark<_T, _class>::getbmarkSecs()
    {
        return _end - _start;
    }

    template<typename _T, typename _class>
    _T      bmark<_T, _class>::getbmarkMilliSecs()
    {
        return (_end - _start) * 1000;
    }

    template<typename _T, typename _class>
    _T      bmark<_T, _class>::GetFuncBmark(void (_class::*function)(), int nTimes)
    {
        /***    Something similar to this, but needs better functions calling   ***/
        _class c;
        _start  = omp_get_wtime();

        for (int i = 0; i < nTimes; i++)
        {
            (c.*function)();   
        }

        _end    = omp_get_wtime();
        return getbmarkMilliSecs();
    }

    template<typename _T, typename _class>
    _T      bmark<_T, _class>::GetFuncBmark(void (_class::*function)(const std::string&), int nTimes, std::string rhs)
    {
        /***    Something similar to this, but needs better functions calling   ***/
        _class c;
        _start  = omp_get_wtime();

        for (int i = 0; i < nTimes; i++)
        {
            (c.*function)(rhs);   
        }

        _end    = omp_get_wtime();
        return getbmarkMilliSecs();
    }

    template<typename _T, typename _class>
    _T      bmark<_T, _class>::CompareFunc(void (*function)(), void (*function2)(), int nTimes) 
    {}

    /***>>>>>>>><<<<<<<<<***/
     #include "bm_prototype"
    /***>>>>>>>><<<<<<<<<***/
}
