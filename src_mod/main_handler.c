#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "image.h"


int main(int argc, char const *argv[])
{
	
	printf("Se inicia el pipeline.\n");

	//Se inicia el proceso para la lectura de la imagen.
	pid_t pidLecturaImg;
	int pipe_read[2];
	pipe(pipe_read);
	if((pidLecturaImg = fork())==0){
		//Si es el hijo.
		close(pipe_read[0]);
		//Se convierte pipe a char*
		char pipe_to_string[12];
		snprintf(pipe_to_string, 12, "%i", pipe_read[1]);
		int res = execlp("./readImage","readImage","imagen2.bmp", &pipe_to_string,(char*)NULL);
		printf("Resultado execlp = %u\n", res);
	}else{
		//Si soy el padre.
		close(pipe_read[1]);
		
		wait(&pidLecturaImg);
	/*
		Image *img = (Image*) malloc(sizeof(img));
		img->triads = (Triad**)malloc(sizeof(Triad*)*512);
		for (int i = 0; i < 512; i++)
		{
			img->triads[i] = (Triad*)malloc(sizeof(Triad)*512);
		}
		
		
		//read(pipe_read[0], img, sizeof(Image));
		//printf("Respuesta en el padre de Image->type: %i\n", img->type);
		printf("Respuesta en padre image->width: %d\n", img->width);
		printf("Respuesta en padre image->height: %d\n", img->height);
		
		int aux, x, y;
	    unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*512*512*4);
        for(x = 0; x<512*512*4; x++){
            read(pipe_read[0], &data[x], sizeof(unsigned char ));

        }
        for(x = 0; x<512*512*4; x++){
            printf(" %d ", data[x]);
            if(x%4==0){
                printf("\n");
            }
        }
	    FILE *file_pointer = fopen("binarizado-prueba.bmp", "r+");
        fread(&img->type, 1, 1, file_pointer); //1
        fread(&img->type2, 1, 1, file_pointer); //1
        if((img->type != 'B' ) && (img->type != 'M')){ //Se comprueba que el archivo sea del tipo bmp
          free(img);
          return -1;
          }


        fread(&img->fileSize, 4, 1, file_pointer);//5

        fread(&img->reserved1, 2, 1, file_pointer);//7

        fread(&img->reserved2, 2, 1, file_pointer);//9

        fread(&img->dataPointer, 4, 1, file_pointer);//13

        fseek(file_pointer,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.
	    for(x=0; x<512*512*4; x++){
            fwrite(&data[x],sizeof(unsigned char),1,file_pointer); //Se extrae la data de la imagen.

        }

        for(x = 0; x<img->height;x++){
			for(y = 0; y<img->width;y++){
				printf("(%d)",img->triads[x][y].r);
			}
		}
		*/
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
