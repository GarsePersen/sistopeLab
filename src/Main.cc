#include <iostream>
#include "Monitor.cc"
using namespace std;


int main()
{
    UpDownCounter a(0), b(2), c;
    cout << a.up() << endl;
    cout << a.up() << endl;
    cout << a.up() << endl;
    cout << b.down() << endl;
    cout << b.down() << endl;
    cout << b.down() << endl;

    return 0;
}
