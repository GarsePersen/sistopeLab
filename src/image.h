#include <stdio.h>
#include <pthread.h>

//Estructura para almacenar la matriz de pixeles
typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    unsigned int partition_size;
}Triad;
//Estructura para la imagen
typedef struct {
    int type;
    int type2;
    int fileSize;
    int reserved1;
    int reserved2;
    int dataPointer;
    int headerSize;
    int width;
    int height;
    int planes;
    int tam_img;
    int bitPerPixel;
    int isCompressed;
    Triad *triads;
    FILE* filePointer;
    int nThreads;
    int partition_end;
    int partition_start;
    int numberPixelsXthread;
}Image;



typedef struct{
    pthread_t id;
    unsigned int start;
    unsigned int end;
}DataThread;

typedef struct{
    char *file_name;
    int umbral;
    int nearlyBlack;
    int threads;
	int nearlyBlackResult;
    pthread_mutex_t mutexInit;
	pthread_mutex_t mutexNearlyBlack;
    pthread_mutex_t mutexCalculus;
    pthread_barrier_t barrier;
    int aux;
    Image* img;
    DataThread* dataThreads;
}DataInit;

//Funciones
void* threadsHandler(void* data_o);
FILE *openImage(char *file_name);
void *convertToGrayScale(Image *img, unsigned int start, unsigned int end);
int readImage(Image *img, FILE *file_pointer);
void cpy_img(char *nameFile, char *nameFileOut);
void writeImage(Image *img, FILE *file_pointer);
unsigned int searchThread(pthread_t id, DataThread *threads, int numThread);
int binarization(Image *img, unsigned int start, unsigned int end, int umbral);
/*
void printPixelMatrix(Image *img);
void closeImage(FILE *file_pointer);
int nearlyBlack(int numOfBlacks, int numTotal, int umbralNearlyBlack);
*/