#include <uC++.h>
#include "image.h"
_Monitor
    BoundedBuffer {
        uCondition full, empty;
        int front, back, count;
        Image **elements;
    
    public:
    
    BoundedBuffer() : front(0), back(0), count(0) {
        elements = (Image**)malloc(sizeof(Image*)*10);
    }
    _Nomutex 
        int query() {
            return count; }
        void insert(Image *elem) {
            if (count == 10) empty.wait();
            elements[back] = elem;
            back = (back+1)% 10;
            count += 1;
            full.signal();
            }
        Image *remove() {
            if(count == 0) full.wait();
            Image *elem = elements[front];
            front = (front+1)%10;
            count -= 1;
            empty.signal();
            return elem;
        }       
};