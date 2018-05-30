#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "image.h"

	

int main(int argc, char **argv){
	
    /*
    SECCION SIN HEBRAS
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
	
	
	
	int *resultsNearlyBlack = (int*)malloc(sizeof(int)*numImgs); 
	for(x = 0; x<numImgs; x++){
		resultsNearlyBlack[x] = imageHandler(imageNames[x],umbral,umbralNearlyBlack); //image handler retorna 0 -> no, 1 -> si.
        if(resultsNearlyBlack[x] == -1){ //Si se detecta que el archivo no es bmp
            printf("\n\nError, archivo %s no es bmp, saltando...\n\n", imageNames[x]);
        }
	}

    if(flag == 1){
        printf("\n|\timage\t|\t\tnearly black\t\t|\n" );
        for(x = 0; x<numImgs; x++){

            printf("|---------------|---------------------------------------|\n");
            if(resultsNearlyBlack[x] == 1){
                printf("|\t%s\t|\t\tyes\t\t|\n",imageNames[x]);	
            }else if(resultsNearlyBlack[x] == 0){
                printf("|\t%s\t|\t\tno \t\t|\n",imageNames[x]);
            } else {
                printf("|\t%s\t|\t\tArchivo no es BMP \t\t|\n",imageNames[x]);

            }


        }	

    }
    */

    /*
    **** SECCION PRUEBA DE HEBRAS ****
    */
    int umbral = 100;
    int threads;
    printf("Ingrese el numero de hebras: ");
    scanf("%i",&threads);
    int umbralNearlyBlack = 100;
    int resultsNearlyBlack = imageHandler("prueba.bmp",umbral,umbralNearlyBlack, threads); //image handler retorna 0 -> no, 1 -> si.
    printf("Se termino imagen, resultNearlyBlack: %i\n", resultsNearlyBlack);
    printf("\n");
    
	return 0;
}
	


