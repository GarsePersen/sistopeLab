

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


Image *loadImage(char *file_name);
