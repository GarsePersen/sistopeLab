#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "image.h"

#define READ 0
#define WRITE 1

unsigned char *convertToGrayScale(unsigned char *data, int width, int height);

int main(int argc, char const *argv[])
{


   	pid_t pidGray_bin;
	int pipeGray_bin[2];
	pipe(pipeGray_bin);

	if((pidGray_bin = fork()) == 0){

		//Se cierra el canal de escritura
		close(pipeGray_bin[WRITE]);
		dup2(pipeGray_bin[READ],STDIN_FILENO);
		//Le paso el canal de lectura al hijo
		int id = execlp("./binarization","binarization", argv[1], (char*)NULL);

	}else{
		//Se cierra el canal de lectura
		close(pipeGray_bin[READ]);
		//Se leen parametros y se escriben en el hijo
		int umbral, umbralNearlyBlack, flag;
		read(STDIN_FILENO, &umbral, sizeof(int));
		read(STDIN_FILENO, &umbralNearlyBlack, sizeof(int));
		read(STDIN_FILENO, &flag, sizeof(int));
		write(pipeGray_bin[WRITE], &umbral, sizeof(int));
		write(pipeGray_bin[WRITE], &umbralNearlyBlack, sizeof(int));
		write(pipeGray_bin[WRITE], &flag, sizeof(int));



		//Se leen los datos desde readImage
		int height, width;
		int aux, x, y;
		read(STDIN_FILENO, &height, sizeof(int));
		read(STDIN_FILENO, &width, sizeof(int));
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
		for(x = 0; x<height*width*4; x++){
			read(STDIN_FILENO, &data[x], sizeof(unsigned char ));
		}

		unsigned char *data_gray = convertToGrayScale(data,width,height);

		//Se escribe en el pipe para pasarlo a binarization
		//Se escribe el tamaño de la imágen.
		write(pipeGray_bin[1], &height, sizeof(int));
		write(pipeGray_bin[1], &width, sizeof(int));
		for(x = 0; x<height*width*4; x++){
			write(pipeGray_bin[WRITE], &data_gray[x], sizeof(unsigned char ));
		}
		
	
		wait(&pidGray_bin);
	
		

	}




    
	
	
	return 0;
}


unsigned char *convertToGrayScale(unsigned char *data, int width, int height){
	int x;
	int y;
    int count = 0;
	for(x =0 ; x< height; x++){
		for(y = 0; y< width; y++){
			int calculo = data[count]*0.11+data[count+1]*0.59+data[count+2]*0.3;
			data[count++] = calculo;
			data[count++] = calculo;
			data[count++] = calculo;
			data[count++] = 255;
		}
	}
	return data;
}





