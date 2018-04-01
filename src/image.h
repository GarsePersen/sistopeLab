


typedef struct{
    int r;
    int g;
    int b;
}Triad;

typedef struct {

    int height;
    int width;
    int dataPointer;
    int isCompressed;
    Triad **triads;
}Image;


Image *loadImage(char *file_name);