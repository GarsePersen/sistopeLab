
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


int main(int argc, char const *argv[])
{

    pid_t pidCat_write;
    int pipeCat_write[2];
    pipe(pipeCat_write);

    if((pidCat_write = fork()) == 0){
        //Soy el hijo
        //Se cierra el canal de escritura
		close(pipeCat_write[WRITE]);
		dup2(pipeCat_write[READ], STDIN_FILENO);
		int id = execlp("./write","write",argv[1],(char*)NULL);


    }else{

        //Soy el padre.
        //Se cierra el canal de lectura
		close(pipeCat_write[READ]);
		int umbralNearlyBlack, flag;
		read(STDIN_FILENO, &umbralNearlyBlack, sizeof(int));
		read(STDIN_FILENO, &flag, sizeof(int));
        write(pipeCat_write[WRITE], &flag, sizeof(int));
		//Se leen los datos desde readImage
		int aux, x, y;
		int width, height;
		read(STDIN_FILENO, &height, sizeof(int));
		read(STDIN_FILENO, &width, sizeof(int));
		unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*width*height*4);
		for(x = 0; x<width*height*4; x++){
			read(STDIN_FILENO, &data[x], sizeof(unsigned char ));
		}
        int numBlacks;
        read(STDIN_FILENO, &numBlacks, sizeof(int));
		write(pipeCat_write[WRITE], &height, sizeof(int));
		write(pipeCat_write[WRITE], &width, sizeof(int));
		//Se escribe en el pipe para pasarlo a binarization
		for(x = 0; x<width*height*4; x++){
			write(pipeCat_write[WRITE], &data[x], sizeof(unsigned char ));
		}
        int resultado = nearlyBlack(numBlacks, height*width, umbralNearlyBlack);
        write(pipeCat_write[WRITE], &resultado, sizeof(resultado));


        wait(&pidCat_write);
    }
    

    return 0;
}

/*Función que calcula el porcentaje de pixeles negros que contiene la imagen. Si este porcentaje es mayor al
umbral dado por el usuario, se considera como NearlyBlack,
Entrada; Int numOfBlacks (Cantidad de pixeles negros), int numTotal (Cantidad de pixeles totales), int umbralNearlyBlack (umbral dado por el usuario)
Salida: Int representando si es NearlyBlack o no.
*/
int nearlyBlack(int numOfBlacks, int numTotal, int umbralNearlyBlack){

	float percent = ((float)numOfBlacks/(float)numTotal )*100;
	if(percent >= umbralNearlyBlack){
		return 1;
	}else{
		return 0;
	}
}