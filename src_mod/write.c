
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
    int aux, x, y, flag;
    int width,height;
	read(STDIN_FILENO, &flag, sizeof(int));	
	read(STDIN_FILENO, &height, sizeof(int));
	read(STDIN_FILENO, &width, sizeof(int));
    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
	for(x = 0; x<width*height*4; x++){
        read(STDIN_FILENO, &data[x], sizeof(unsigned char ));
    }
    Image *img = (Image*) malloc(sizeof(img));



	char *file_name = (char *)argv[1];
	char *fileNameOut = (char *)malloc(sizeof(char*)*100); //Se asigna un nombre al archivo de salida.
	strcpy(fileNameOut,"binarizado-"); //Se guarda el archivo original
	strcat(fileNameOut,file_name); //Se asigna un identificador al archivo
		

	FILE *file_pointer = fopen(fileNameOut, "r+");
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


	//Se recibe resultado de la categoria
	int nearlyBlack;
	read(STDIN_FILENO, &nearlyBlack, sizeof(int));

	if(flag == 1){
        if(nearlyBlack == 1){
            printf("|\t%s\t|\t\tyes\t\t|\n",argv[1]);	
        }else if(nearlyBlack == 0){
            printf("|\t%s\t|\t\tno \t\t|\n",argv[1]);
        } else {
            printf("|\t%s\t|\t\tArchivo no es BMP \t\t|\n",argv[1]);

        }

		
	}

    return 0;
}
