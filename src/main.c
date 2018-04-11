#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "image.h"

	

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


	int op = 0;
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
	
	
	
	//Prueba de extracción de imágen.
	int *resultsNearlyBlack = (int*)malloc(sizeof(int)*numImgs);
	for(x = 0; x<numImgs; x++){
		//image handler retorna 0 -> no, 1 -> si.
		resultsNearlyBlack[x] = imageHandler(imageNames[x],umbral,umbralNearlyBlack);
		printf("NearlyBlack?: %d\n",resultsNearlyBlack[x] );
	}

	if(flag == 1){
		printf("|\timage\t|\t\tnearly black\t\t|\n" );
	for(x = 0; x<numImgs; x++){
		
		printf("|---------------|---------------------------------------|\n");
		if(resultsNearlyBlack[x] == 1){
			printf("|\t%s\t|\t\tyes\t\t|\n",imageNames[x]);	
		}else{
			printf("|\t%s\t|\t\tno \t\t|\n",imageNames[x]);
		}
		

		}	

	}


	printf("\n=========== GrayScale ===========\n");
	return 0;
}
	


