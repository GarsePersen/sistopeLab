
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "image.h"



 
int main(int argc, char const *argv[])
{
    printf("Soy el write\n");
    int aux, x, y;
    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*512*512*4);
    int width,height;
	read(STDOUT_FILENO, &height, sizeof(int));
	read(STDOUT_FILENO, &width, sizeof(int));
	for(x = 0; x<width*height*4; x++){
        read(STDOUT_FILENO, &data[x], sizeof(unsigned char ));
    }
    Image *img = (Image*) malloc(sizeof(img));
		FILE *file_pointer = fopen("binarizado-imagen2.bmp", "r+");
		fread(&img->type, 1, 1, file_pointer); //1
		fread(&img->type2, 1, 1, file_pointer); //1
		if((img->type != 'B' ) && (img->type != 'M')){ //Se comprueba que el archivo sea del tipo bmp
			free(img);
			return -1;
			}


		fread(&img->fileSize, 4, 1, file_pointer);//5

		fread(&img->reserved1, 2, 1, file_pointer);//7

		fread(&img->reserved2, 2, 1, file_pointer);//9

		fread(&img->dataPointer, 4, 1, file_pointer);//13

		fseek(file_pointer,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.
		for(x=0; x<width*height*4; x++){
			fwrite(&data[x],sizeof(unsigned char),1,file_pointer); //Se extrae la data de la imagen.
		}

        fclose(file_pointer);

    return 0;
}
