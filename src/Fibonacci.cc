#include <uC++.h>
_Coroutine fibonacci
{
    int fn;
    void
    main()
    {
        int fn1, fn2;
        fn = 0;
        fn1 = fn;
        suspend();
        //return to last resume
        fn = 1;
        fn2 = fn1;
        fn1 = fn;
        suspend();
        //return to last resume
        for (;;)
        {
            fn = fn1 + fn2;
            fn2 = fn1;
            fn1 = fn;
            suspend();
            //return to last resume
        }
    }

  public:
    int next()
    {
        resume();
        //transfer to last suspend
        return fn;
    }
};