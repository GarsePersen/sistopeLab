#include <uC++.h>
_Monitor
    BoundedBuffer {
        uCondition full, empty;
        int front, back, count;
        int elements[20];
    
    public:
    
    BoundedBuffer() : front(0), back(0), count(0) {}
    _Nomutex 
        int query() {
            return count; }
        void insert(int elem) {
            if (count == 20) empty.wait();
            elements[back] = elem;
            back = (back+1)% 20;
            count += 1;
            full.signal();
            }
        int remove() {
            if(count == 0) full.wait();
            int elem = elements[front];
            front = (front+1)%20;
            count -= 1;
            empty.signal();
            return elem;
            }
};