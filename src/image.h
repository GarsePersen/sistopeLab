#include <stdio.h>
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

//Funciones
int threadsHandler(char *file_name, int umbral, int nearlyBlack, int threads);
FILE *openImage(char *file_name);
void *convertToGrayScale(void* triads);
void *readImage(void *img);
void cpy_img(char *nameFile, char *nameFileOut);
void *convertToGrayScaleHandler(void *img_o);
void writeImage(Image *img, FILE *file_pointer);

/*
void printPixelMatrix(Image *img);
void closeImage(FILE *file_pointer);
int binarization(Image *img, int umbral);
int nearlyBlack(int numOfBlacks, int numTotal, int umbralNearlyBlack);
*/