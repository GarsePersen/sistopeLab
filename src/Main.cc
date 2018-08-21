#include <iostream>
#include "Fibonacci.cc"
using namespace std;


int main()
{
    fibonacci f1, f2;
    for (int i = 1; i <= 10; i++)
    {
        cout << f1.next() << " " << f2.next() << endl;
    }
    return 0;
}
