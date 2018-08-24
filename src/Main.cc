#include <iostream>
#include "Monitor.cc"
#include <pthread.h>
using namespace std;

#define NUM_THREADS 15

void* insert(void* x);
void* queryy(void * arg);
void* remove(void * arg);
BoundedBuffer buffer;
int main()
{
    
    pthread_t threads_insert[NUM_THREADS];
    pthread_t threads_query[NUM_THREADS];
    pthread_t threads_remove[NUM_THREADS];
    buffer.insert(2);
    cout<<buffer.query() <<endl;
    //cout << a.query() << endl;
    //cout << b.insert(2)<<endl;
    int rc;
    
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_query[i], NULL, &queryy, NULL);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_remove[i], NULL, &remove, NULL);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_insert[i], NULL, &insert, (void *)i);
    }
    for(int x = 0; x<3; x++){
        for(int i = 0; i<NUM_THREADS; i++){
            if(x ==0){
                pthread_join(threads_insert[i],NULL);
            }else if(x==1){
                pthread_join(threads_query[i],NULL);
            }else if(x==2){
                pthread_join(threads_remove[i],NULL);
            }
        }
    }
    
    return 0;
}


void* insert(void* x){
    buffer.insert((long)    x);
    cout<<"Estoy insertando"<<endl;
}

void* queryy(void *arg){
    int a = buffer.query();
    cout<<"Estoy tomando el valor=> "<< a <<endl;
    
}

void* remove(void * arg){
    int r = buffer.remove();
    cout<<"Estoy removiendo el valor=> "<< r <<endl;
}
