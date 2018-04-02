#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


Image *loadImage(char *file_name){
  	Image *img = (Image*)malloc(sizeof(Image));
	FILE *fptr;
	fptr = fopen(file_name, "r");
	
	fread(&img->type, 2, 1, fptr);
	printf("Tipo: %c\n", img->type);

	fread(&img->fileSize, 4, 1, fptr);
	printf("File size: %d\n", img->fileSize);
	
	printf("Reserved1: %d\n", img->reserved1);
	printf("Reserved2: %d\n", img->reserved2);
	printf("Header size: %d\n", img->headerSize);
	printf("DP: %d\n", img->dataPointer);	
	printf("Image size = %d x %d\n", img->width, img->height);
	printf("Number of colour planes is %d\n", img->planes);
	printf("Bits per pixel is %d\n", img->bitPerPixel);
	printf("Compression type is %d\n", img->isCompressed);
	
	return img;
}
    


