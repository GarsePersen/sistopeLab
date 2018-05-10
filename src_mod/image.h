#include <stdio.h>
//Estructura para almacenar la matriz de pixeles
typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
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
    Triad **triads;
}Image;

//Funciones
int imageHandler(char *file_name, int umbral, int nearlyBlack);
FILE *openImage(char *file_name);
unsigned char *readImage(Image *img, FILE *file_pointer);
void printPixelMatrix(Image *img);
//void convertToGrayScale(Image *img);
void closeImage(FILE *file_pointer);
void writeGrayImage(Image *img, FILE *file_pointer);
void cpy_img(char *nameFile, char *nameFileOut);
//int binarization(Image *img, int umbral);
int nearlyBlack(int numOfBlacks, int numTotal, int umbralNearlyBlack);
