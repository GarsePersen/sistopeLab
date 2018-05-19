
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "image.h"

#define READ 0
#define WRITE 1

int binarization(unsigned char *data,int width, int height, int umbral);

int main(int argc, char const *argv[])
{

    pid_t pidBin_category;
    int pipeBin_category[2];
    pipe(pipeBin_category);

    if((pidBin_category = fork()) == 0){
        //Soy el hijo
        //Se cierra el canal de escritura
		close(pipeBin_category[WRITE]);
		dup2(pipeBin_category[READ], STDIN_FILENO);
		int id = execlp("./category","category",argv[1],(char*)NULL);


    }else{

        //Soy el padre.
        //Se cierra el canal de lectura
		close(pipeBin_category[READ]);
		int umbral, umbralNearlyBlack, flag;
		read(STDIN_FILENO, &umbral, sizeof(int));
		read(STDIN_FILENO, &umbralNearlyBlack, sizeof(int));
		read(STDIN_FILENO, &flag, sizeof(int));
		write(pipeBin_category[WRITE], &umbralNearlyBlack, sizeof(int));
		write(pipeBin_category[WRITE], &flag, sizeof(int));
		//Se leen los datos desde readImage
		int aux, x, y;
		int width, height;
		read(STDIN_FILENO, &height, sizeof(int));
		read(STDIN_FILENO, &width, sizeof(int));
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
		for(x = 0; x<width*height*4; x++){
			read(STDIN_FILENO, &data[x], sizeof(unsigned char ));

		}
		int numBlacks=binarization(data,width,height, umbral);
		write(pipeBin_category[WRITE], &height, sizeof(int));
		write(pipeBin_category[WRITE], &width, sizeof(int));
		//Se escribe en el pipe para pasarlo a binarization
		for(x = 0; x<width*height*4; x++){
			write(pipeBin_category[WRITE], &data[x], sizeof(unsigned char ));
		}
		//Se escribe cantidad de pixeles negros
		write(pipeBin_category[WRITE], &numBlacks, sizeof(int));

        wait(&pidBin_category);
    }
    

    return 0;
}


int binarization(unsigned char *data,int width, int height, int umbral){

	int x;
	int y;
	int numBlacks = 0;
    int count = 0;
	for(x =0 ; x< height; x++){
		for(y = 0; y< width; y++){
			if( data[count] > umbral ){
                data[count] = 255;
                data[count + 1] = 255;
                data[count + 2] = 255;
                count +=4;
			}else{
                data[count] = 0;
                data[count + 1 ] = 0;
                data[count + 2 ] = 0;
                count +=4;
                numBlacks++;
            }
		}
	}
	return numBlacks;
}
