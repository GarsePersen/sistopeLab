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
	
	fread(&img->type, 2, 1, fptr); //1
	printf("Tipo: %c\n", img->type);

	fread(&img->fileSize, 4, 1, fptr);//5
	printf("File size: %d\n", img->fileSize);
	
	fread(&img->reserved1, 2, 1, fptr);//7
	printf("Reserved1: %d\n", img->reserved1);

	fread(&img->reserved2, 2, 1, fptr);//9
	printf("Reserved2: %d\n", img->reserved2);
	
	//Puntero a datos
	fread(&img->dataPointer, 4, 1, fptr);//13
	printf("DP: %d\n", img->dataPointer);	//


	//printf("Header size: %d\n", img->headerSize);

	fseek(fptr,4,SEEK_CUR); //4 desplazamientos
	fread(&img->width, 4, 1, fptr);//18 ->ancho
	fread(&img->height, 4, 1, fptr);//21 ->Largo


	
	printf("Image size = %d x %d\n", img->width, img->height);
	fseek(fptr,26,SEEK_SET);
	fread(&img->planes, 2, 1, fptr);//Planos
	printf("Number of colour planes is %d\n", img->planes);

	fseek(fptr,28,SEEK_SET);
	fread(&img->bitPerPixel, 2, 1, fptr);//bits x pixel
	printf("Bits per pixel is %d\n", img->bitPerPixel);
	int tam_img = 0;
	fseek(fptr,34,SEEK_SET);
	fread(&tam_img,4,1,fptr);
	printf("tamaño de la imagen: %d\n",tam_img);

	fseek(fptr,30,SEEK_SET);
	fread(&img->isCompressed,4,1,fptr);
	printf("Compression type is %d\n", img->isCompressed);
	int tablaCol;
	fseek(fptr,46,SEEK_SET);
	fread(&tablaCol,4,1,fptr);
	printf("tam_tabla_col: %d \n",tablaCol);
	//Se obtiene la imágen.
	fseek(fptr,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.
	int x;
	int y;
	int var = 0;
	unsigned char *data = (unsigned char*)malloc(sizeof(char)*tam_img);
	fread(data,tam_img,1,fptr);

	for (x=0;x<tam_img;x+=3){
		printf("(%d,%d,%d)\n",data[x],data[x+1],data[x+2]);
	}
	
	fclose(fptr);

	return img;
}
    


