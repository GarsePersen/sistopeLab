#include <uC++.h>
_Monitor UpDownCounter{
    int
        counter;

  public
      :
    UpDownCounter() : counter(0) {
        
    }
    UpDownCounter(int init) : counter(init) {

    }
    int
    up()
    {
        counter += 1;
        return counter;
    }
    int
    down()
    {
        counter -= 1;
        return counter;
    }
};