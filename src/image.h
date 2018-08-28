#include <vector>

class Image{
    public:
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
        vector<Triads> triads;
        FILE* filePointer;
        int nThreads;
        int partition_end;
        int partition_start;
        int numberPixelsXthread;
        int numberBlacks;
};


class Triad{
    public:
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
        unsigned int partition_size;
};