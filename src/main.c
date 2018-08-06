#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "image.h"
#include <pthread.h>

	

int main(int argc, char **argv){
	
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
    int threads;
    printf("Ingrese el numero de hebras: ");
    scanf("%i",&threads);
	
	int op = 0;
	char **imageNames = (char**)malloc(sizeof(char*)*numImgs);
	int x;
	for(x = 0; x<numImgs; x++){
        char str[8];
		imageNames[x] = (char*)malloc(sizeof(char)*100);
        char image[8] = "imagen_";
        sprintf(str,"%d", x+1);
        strcat(image, str);
        
        strcat(image, ".bmp");
		strcpy(imageNames[x],image);
		memset(&image[0], 0, sizeof(image));
		memset(&str[0], 0, sizeof(str));
        FILE *archivo = openImage(imageNames[x]); //Comprobación de que el archivo existe
        if(archivo == NULL){
            printf("\nError: Archivo %s no existe\n\n", imageNames[x]);
            return -1;
        }
        fclose(archivo);
        

	}
	
	
	int *resultsNearlyBlack = (int*)malloc(sizeof(int)*numImgs); 
	for(int y = 0; y<numImgs; y++){
		DataInit *data = (DataInit*)malloc(sizeof(DataInit));
        data->file_name = imageNames[y];
        data->umbral = umbral;
        data->nearlyBlack = umbralNearlyBlack;
        data->threads = threads;
        pthread_mutex_init(&data->mutexInit,NULL);
        pthread_mutex_init(&data->mutexCalculus,NULL);
        pthread_barrier_init(&data->barrier,NULL,data->threads);
        data->aux =0;
        Image *img = (Image*)malloc(sizeof(Image));
        data->img = img;
        img->numberBlacks = 0;
        pthread_mutex_init(&img->mutexNearlyBlack,NULL);    
        int x;
        DataThread* dataThreads = (DataThread*)malloc(sizeof(DataThread)*threads);
        data->dataThreads = dataThreads;
        for(x=0;x<threads;x++){
            pthread_create(&dataThreads[x].id,NULL,threadsHandler,(void*)data);
        }
        for(x=0;x<threads;x++){
            pthread_join(data->dataThreads[x].id,NULL);
        }
        writeImage(data->img,data->img->filePointer);

        //Identificacion de nearly black
        resultsNearlyBlack[y] = nearlyBlack(img->numberBlacks, img->width*img->height, umbralNearlyBlack);
        
	}

    if(flag == 1){
        printf("\n|\timage\t|\t\tnearly black\t\t|\n" );
        for(x = 0; x<numImgs; x++){

            printf("|---------------|---------------------------------------|\n");
            if(resultsNearlyBlack[x] == 1){
                printf("|\t%s\t|\t\tyes\t\t|\n",imageNames[x]);	
            }else{
                printf("|\t%s\t|\t\tno \t\t|\n",imageNames[x]);
            }


        }	

    }

    


    

    /*
    **** SECCION PRUEBA DE HEBRAS ****
    */
}
	


