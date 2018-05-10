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
		char pipe_readGray_toString[12];
		dup2(pipeGray_bin[0],STDOUT_FILENO);
		//Le paso el canal de lectura al hijo
		snprintf(pipe_readGray_toString,12,"%i",pipeGray_bin[0]);
		int id = execlp("./binarization","binarization",&pipe_readGray_toString,(char*)NULL);

	}else{
		//Se cierra el canal de lectura
		close(pipeGray_bin[0]);
		
		int pipe_read = atoi(argv[1]);

		//Se leen los datos desde readImage
		int height, width;
		int aux, x, y;
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*512*512*4);
		read(STDOUT_FILENO, &height, sizeof(int));
		read(STDOUT_FILENO, &width, sizeof(int));
		printf("height: %d\n",height);
		printf("width: %d\n",width);
		for(x = 0; x<512*512*4; x++){
			read(STDOUT_FILENO, &data[x], sizeof(unsigned char ));
		}
		for(x = 0; x<512*512*4; x++){
			//printf(" %d ", data[x]);
			if(x%4==0){
			//  printf("\n");
			}
		}
		unsigned char *data_gray = convertToGrayScale(data,512,512 );

		//Se escribe en el pipe para pasarlo a binarization
		for(x = 0; x<512*512*4; x++){
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





