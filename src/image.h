#include <stdio.h>

typedef struct{
    int r;
    int g;
    int b;
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
    int bitPerPixel;
    int isCompressed;
    Triad **triads;
}Image;


Image *imageHandler(char *file_name);
FILE *openImage(char *file_name);
void readImage(Image *img, FILE *file_pointer);
void printPixelMatrix(Image *img);
void convertToGrayScale(Image *img);
void Binarization(Image *img, int umbral);
void closeImage(FILE *file_pointer);
