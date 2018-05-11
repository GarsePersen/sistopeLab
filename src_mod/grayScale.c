#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "image.h"


unsigned char *convertToGrayScale(unsigned char *data, int width, int height);

int main(int argc, char const *argv[])
{




   	printf("Soy el grayScale\n");
   	pid_t pidGray_bin;
	int pipeGray_bin[2];
	pipe(pipeGray_bin);

	if((pidGray_bin = fork()) == 0){

		//Se cierra el canal de escritura
		close(pipeGray_bin[1]);
		dup2(pipeGray_bin[0],STDOUT_FILENO);
		//Le paso el canal de lectura al hijo
		int id = execlp("./binarization","binarization",(char*)NULL);

	}else{
		//Se cierra el canal de lectura
		close(pipeGray_bin[0]);

		//Se leen los datos desde readImage
		int height, width;
		int aux, x, y;
		read(STDOUT_FILENO, &height, sizeof(int));
		read(STDOUT_FILENO, &width, sizeof(int));
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
		printf("height: %d\n",height);
		printf("width: %d\n",width);
		for(x = 0; x<height*width*4; x++){
			read(STDOUT_FILENO, &data[x], sizeof(unsigned char ));
		}

		unsigned char *data_gray = convertToGrayScale(data,width,height);

		//Se escribe en el pipe para pasarlo a binarization
		//Se escribe el tamaño de la imágen.
		write(pipeGray_bin[1], &height, sizeof(int));
		write(pipeGray_bin[1], &width, sizeof(int));
		for(x = 0; x<height*width*4; x++){
			write(pipeGray_bin[1], &data_gray[x], sizeof(unsigned char ));
		}
	
		wait(&pidGray_bin);
	
		//Data extraída desde el pipe
		

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





