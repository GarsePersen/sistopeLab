#include <iostream>
#include "Monitor.cc"
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define NUM_THREADS 2
void* read(void* x);
void* gray(void *arg);
void* bin(void * arg);
void* nearly(void * arg);
void* write(void * arg);
void cpy_img(string nameFile, string nameFileOut);
BoundedBuffer buffer;



int main()
{


    pthread_t threads_read[NUM_THREADS];
    pthread_t threads_bin[NUM_THREADS];
    pthread_t threads_gray[NUM_THREADS];
    pthread_t threads_nearlyB[NUM_THREADS];
    pthread_t threads_write[NUM_THREADS];
    
    int rc;
    
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_read[i], NULL, &read, (void*) i+1);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_bin[i], NULL, &gray, (void*) i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_gray[i], NULL, &bin, (void *)i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_nearlyB[i], NULL, &nearly, (void *)i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_write[i], NULL, &write, (void *)i);
    }
    for(int x = 0; x<5; x++){
        for(int i = 0; i<NUM_THREADS; i++){
            if(x ==0){
                pthread_join(threads_bin[i],NULL);
            }else if(x==1){
                pthread_join(threads_gray[i],NULL);
            }else if(x==2){
                pthread_join(threads_write[i],NULL);
            }else if(x==3){
                pthread_join(threads_nearlyB[i],NULL);
            }else if(x==4){
                pthread_join(threads_read[i],NULL);
            }else{
                cout << "Error" <<endl;
            }
        }
    }
    
    return 0;
}


void* read(void* x){
    //Crear nombre de imagen
    stringstream strstream;
    stringstream ss;
    string nImagen;
    strstream << (long)x;
    strstream >> nImagen;
    ss<<"image_"<<nImagen;
    string nombreImagen = ss.str();
    nombreImagen=nombreImagen+".bmp";
    cout<<"Estoy leyendo "<<nombreImagen<<endl;
    
    //Copiar imagen
    string fileNameOut="binarizado-";
	fileNameOut=fileNameOut+nombreImagen; 
	cpy_img(nombreImagen,fileNameOut); 

	//data->img->filePointer = openImage(fileNameOut); //Se abre imagen
	//readImage(data->img,data->img->filePointer);
    buffer.insert((long)x,0);
}

void* gray(void *arg){
    int a = buffer.remove((long)0);
    cout<<"Estoy tomando el valor y hago GRAY=> "<< a <<endl;
    buffer.insert((long)a,1);
}

void* bin(void * arg){
    int r = buffer.remove(1);
    cout<<"Estoy removiendo el valor HAGO BIN=> "<< r <<endl;
    buffer.insert((long)r,2);
}
void* nearly(void * arg){
    int r = buffer.remove(2);
    cout<<"Estoy removiendo el valor HAGO NEARLY=> "<< r <<endl;
    buffer.insert((long)r,3);
}
void* write(void * arg){
    int r = buffer.remove(3);
    cout<<"Estoy removiendo el valor HAGO WRITE=> "<< r <<endl;
}

void cpy_img(string nameFile, string nameFileOut){
    string command="cp ";
    string cpy_nameFile = nameFile;
    command=command+cpy_nameFile+" ./"+nameFileOut; // cp nameFile
    system(command.c_str());
}