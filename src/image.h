#include <stdio.h>

typedef struct{
    int r;
    int g;
    int b;
    int a;
}Triad;

typedef struct {
    int type;
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
    Triad **triads;
}Image;


int imageHandler(char *file_name, int umbral, int umbralNearlyBlack);
FILE *openImage(char *file_name);
void readImage(Image *img, FILE *file_pointer);
void printPixelMatrix(Image *img);
void convertToGrayScale(Image *img);
void closeImage(FILE *file_pointer);
void writeGrayImage(Image *img, FILE *file_pointer);
void cpy_img(char *nameFile, char *nameFileOut);
int* binarization(Image *img, int umbral);
int calculateNearlyBlack(int *binarization,  int umbralForClasify);
