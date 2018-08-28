#include <uC++.h>
_Monitor
    BoundedBuffer {
        uCondition full_bin, empty_bin;
        uCondition full_gray, empty_gray;
        uCondition full_nearly, empty_nearly;
        uCondition full_write, empty_write;
        int front_bin, back_bin, count_bin;
        int front_gray, back_gray, count_gray;
        int front_nearly, back_nearly, count_nearly;
        int front_write, back_write, count_write;
        int buffBin[2];
        int buffgray[2];
        int buffNearly[2];
        int buffWrite[2];
        int elements[2];
    
    public:
    
    BoundedBuffer() : front_bin(0), back_bin(0), count_bin(0),front_gray(0), back_gray(0), count_gray(0),
    front_nearly(0), back_nearly(0), count_nearly(0),front_write(0), back_write(0), count_write(0) {}
    _Nomutex 
        void insert(int elem, int type) {
            

            switch(type){
                case 0:
                    if (count_gray == 2) empty_gray.wait();
                    elements[back_gray] = elem;
                    back_gray = (back_gray+1)% 2;
                    count_gray += 1;
                    full_gray.signal();
                    break;
                case 1:
                    if (count_bin == 2) empty_bin.wait();
                    elements[back_bin] = elem;
                    back_bin = (back_bin+1)% 2;
                    count_bin += 1;
                    full_bin.signal();
                    break;
                case 2:
                    if (count_nearly == 2) empty_nearly.wait();
                    elements[back_nearly] = elem;
                    back_nearly = (back_nearly+1)% 2;
                    count_nearly += 1;
                    full_nearly.signal();
                    break;
                case 3:
                    if (count_write == 2) empty_write.wait();
                    elements[back_write] = elem;
                    back_write = (back_write+1)% 2;
                    count_write += 1;
                    full_write.signal();
                    break;
                default:
                    //cout << "Default" << endl;
                    break;
            }
            }
        int remove(int type) {  
            int elem;
            switch(type){
                case 0:
                    {
                        if (count_gray == 0) full_gray.wait();
                        elem = elements[front_gray];
                        front_gray = (front_gray+1)%2;
                        count_gray -= 1;
                        empty_gray.signal();
                        return elem;
                    }
                case 1:
                    {
                    
                        if (count_bin == 0) full_bin.wait();
                        elem = elements[front_bin];
                        front_bin = (front_bin+1)%2;
                        count_bin -= 1;
                        empty_bin.signal();
                        return elem;
                    }
                case 2:
                {
                        if (count_nearly == 0) full_nearly.wait();
                        elem = elements[front_nearly];
                        front_nearly = (front_nearly+1)%2;
                        count_nearly -= 1;
                        empty_nearly.signal();
                        return elem;
                }
                case 3:
                { 
                        if (count_write == 0) full_write.wait();
                        elem = elements[front_write];
                        front_write = (front_write+1)%2;
                        count_write -= 1;
                        empty_write.signal();
                        return elem;
                }
                default:
                    //cout << "Default" << endl;
                    break;
            }
            }
        
};