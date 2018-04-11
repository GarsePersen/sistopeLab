#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "image.h"


int main(int argc, char **argv){
	



	int op = 0;
	printf("Ingrese el número de imágenes: \n");
	int numImgs = 0;
	scanf("%d",&numImgs);
	char **imageNames = (char**)malloc(sizeof(char*)*numImgs);
	int x;
	for(x = 0; x<numImgs; x++){
		imageNames[x] = (char*)malloc(sizeof(char)*100);
		printf("Ingrese el nombre de la imágen %d sin su extensión y luego presione Enter...\n",x);
		scanf("%s",imageNames[x]);
		strcat(imageNames[x],".bmp");
		fflush( stdin );
		printf("Imagen: %s\n",imageNames[x]);
	}


	int imageNumber = -1;
	int thresholdB = -1;
	int thresholdC = -1;
	int flag = 0;
	int aux;

	while ((aux = getopt (argc, argv, "c:u:n:b")) != -1){
		switch (aux){
			case 'c':
				imageNumber = atoi(optarg);
				break;
			case 'u':
				thresholdB = atoi(optarg);
				break;
			case 'n':
				thresholdC = atoi(optarg);
				break;
			case 'b':
				flag = 1;
				break;
			default:
				abort();
		}
		

	}
	//Después se activa. xd
	//Comprobaciones
	/*if ((imageNumber == -1) || (thresholdB == -1) || (thresholdC == -1)){
		printf("Compruebe que ingreso todas las opciones (ver manual)\n");
		return -1;
	}

	printf("cantImagenes: %i \numbralB: %i \numbralC: %i \nbandera: %i\n", imageNumber, thresholdB, thresholdC, flag);
	*/
	
	
	
	//Prueba de extracción de imágen.
	for(x = 0; x<numImgs; x++){
		printf("NearlyBlack %d = %d \n",x,imageHandler(imageNames[x],50,50));
	}
	
	printf("\n=========== GrayScale ===========\n");

	//Copia archivos
	

	return 0;
}
