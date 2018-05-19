#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "image.h"

#define READ 0
#define WRITE 1

int main(int argc, char const *argv[])
{

	int numImgs = -1;
	int umbral = -1;
	int umbralNearlyBlack = -1;
	int flag = 0;
	int aux;
	while ((aux = getopt (argc, argv, "c:u:n:b")) != -1){
		switch (aux){
			case 'c':
				numImgs = atoi(optarg);
				break;
			case 'u':
				umbral = atoi(optarg);
				break;
			case 'n':
				umbralNearlyBlack = atoi(optarg);
				break;
			case 'b':
				flag = 1;
				break;
			default:
				abort();
		}
	}
    //Comprobaciones getopt
    if((umbral < 0 ) || (umbral > 255)){
        printf("\nValor del umbral incorrecto, debe ser entre 0 y 255, saliendo...\n\n");
        return -1;
    }
    if((umbralNearlyBlack < 0) || (umbralNearlyBlack > 100)){
        printf("\nValor del umbral para la categoría inválido, debe ser un porcentaje entre 0 y 100\n\n");
        return -1;
    }
    if(numImgs <= 0){
        printf("\nCantidad de imágenes inválida, debe ser igual o mayor a 1\n\n");
        return -1;
    }
    if((numImgs == -1) || (umbral == -1) || (umbralNearlyBlack == -1)){
        printf("\nNo ha ingresado todas las opciones, revisar leeme\n\n");
        return -1;
    }

	int op = 0;
	char **imageNames = (char**)malloc(sizeof(char*)*numImgs);
	int x;
	printf("Ingrese el nombre del prefijo de sus imagenes y luego presione Enter...\n");
	char *prefijo = malloc(sizeof(char)*100);
	scanf("%s",prefijo);
	for(x = 0; x<numImgs; x++){
		imageNames[x] = (char*)malloc(sizeof(char)*100);
		strcpy(imageNames[x],prefijo);
		char strX[12];
		sprintf(strX, "%d", x+1);
		strcat(imageNames[x],"_");
		strcat(imageNames[x],strX);
		strcat(imageNames[x],".bmp");
		//Comprobación de que el archivo existe
		FILE *archivo = fopen(imageNames[x], "r"); 
        if(archivo == NULL){
            printf("\nError: El archivo %s no existe, favor de revisar.\n\n", imageNames[x])
			;
            return -1;
        } else {
            fclose(archivo);
        }

	}

	printf("\n|\timage\t|\t\tnearly black\t\t|\n" );
    printf("|---------------|---------------------------------------|\n");
        
	for(x=0; x<numImgs; x++){
		//Pipe para enviar la matriz de la imagen
		int pipeMain_read[2];
		pipe(pipeMain_read);
		pid_t pidLecturaImg;
		if((pidLecturaImg = fork())==0){
			close(pipeMain_read[WRITE]);
			//Le paso el canal de lectura al hijo			
			dup2(pipeMain_read[READ],STDIN_FILENO);

			int res = execlp("./readImage","readImage", imageNames[x],(char*)NULL);
		}else{
			//Si soy el padre.
			close(pipeMain_read[READ]);
			//Se escriben los parametros
			write(pipeMain_read[WRITE], &umbral, sizeof(int));
			write(pipeMain_read[WRITE], &umbralNearlyBlack, sizeof(int));
			write(pipeMain_read[WRITE], &flag, sizeof(int));
			wait(&pidLecturaImg);
		}
	}
	

	return 0;
}
