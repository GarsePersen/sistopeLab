
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "image.h"



int binarization(unsigned char *data,int width, int height, int umbral);

int main(int argc, char const *argv[])
{

    pid_t pidBinWrite;
    int pipeBinWrite[2];
    pipe(pipeBinWrite);

    if((pidBinWrite = fork()) == 0){
        //Soy el hijo
        //Se cierra el canal de escritura
		close(pipeBinWrite[1]);
		dup2(pipeBinWrite[0], STDOUT_FILENO);
		char pipe_readGray_toString[12];
		//Le paso el canal de lectura al hijo
		snprintf(pipe_readGray_toString,12,"%i",pipeBinWrite[0]);
		int id = execlp("./write","write",&pipe_readGray_toString,(char*)NULL);


    }else{

        //Soy el padre.
        //Se cierra el canal de lectura
		close(pipeBinWrite[0]);

		//Se leen los datos desde readImage
		int aux, x, y;
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*512*512*4);
		int width, height;
		read(STDOUT_FILENO, &height, sizeof(int));
		read(STDOUT_FILENO, &width, sizeof(int));
		for(x = 0; x<width*height*4; x++){
			read(STDOUT_FILENO, &data[x], sizeof(unsigned char ));

		}
		binarization(data,width,height,50 );
		write(pipeBinWrite[1], &height, sizeof(int));
		write(pipeBinWrite[1], &width, sizeof(int));
		//Se escribe en el pipe para pasarlo a binarization
		for(x = 0; x<512*512*4; x++){
			write(pipeBinWrite[1], &data[x], sizeof(unsigned char ));
		}
        wait(&pidBinWrite);
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
