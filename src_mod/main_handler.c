#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char const *argv[])
{
	
	printf("Se inicia el pipeline.\n");

	//Se inicia el proceso para la lectura de la imagen.
	pid_t pidLecturaImg;
	int pipe_read[2];
	if((pidLecturaImg = fork())==0){
		//Si es el hijo.
		pipe(pipe_read);
		int res = execlp("./readImg","readImg","prueba.bmp",(char*)NULL);
		printf("Resultado execlp = %u\n", res);
	}else{
		//Si soy el padre.
		wait(&pidLecturaImg);
	}























	/*

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
	for(x = 0; x<numImgs; x++){
		imageNames[x] = (char*)malloc(sizeof(char)*100);
		printf("Ingrese el nombre de la imágen %d sin su extensión y luego presione Enter...\n",x+1);
		scanf("%s",imageNames[x]);
		strcat(imageNames[x],".bmp");
		fflush( stdin );
		printf("Imagen: %s\n",imageNames[x]);

        FILE *archivo = openImage(imageNames[x]); //Comprobación de que el archivo existe
        if(archivo == NULL){
            printf("\nError: Archivo no existe, ingreselo nuevamente\n\n");
            x--;
        } else {
            closeImage(archivo);
        }

	}
	*/



	return 0;
}