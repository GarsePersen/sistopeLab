#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


Image *loadImage(char *file_name){
  	Image *img = (Image*)malloc(sizeof(Image));
	FILE *fptr;
	fptr = fopen(file_name, "r");
	
	fread(&img->type, 2, 1, fptr); //1
	printf("Tipo: %c\n", img->type);

	fread(&img->fileSize, 4, 1, fptr);//5
	printf("File size: %d\n", img->fileSize);
	
	fread(&img->reserved1, 2, 1, fptr);//7
	printf("Reserved1: %d\n", img->reserved1);

	fread(&img->reserved2, 2, 1, fptr);//9
	printf("Reserved2: %d\n", img->reserved2);
	
	//Puntero a datos
	fread(&img->dataPointer, 4, 1, fptr);//13
	printf("DP: %d\n", img->dataPointer);	//


	//printf("Header size: %d\n", img->headerSize);

	fseek(fptr,4,SEEK_CUR); //4 desplazamientos
	fread(&img->width, 4, 1, fptr);//18 ->ancho
	fread(&img->height, 4, 1, fptr);//21 ->Largo


	
	printf("Image size = %d x %d\n", img->width, img->height);
	fseek(fptr,26,SEEK_SET);
	fread(&img->planes, 2, 1, fptr);//Planos
	printf("Number of colour planes is %d\n", img->planes);

	fseek(fptr,28,SEEK_SET);
	fread(&img->bitPerPixel, 2, 1, fptr);//bits x pixel
	printf("Bits per pixel is %d\n", img->bitPerPixel);
	int tam_img = 0;
	fseek(fptr,34,SEEK_SET);
	fread(&tam_img,4,1,fptr);
	printf("tamaño de la imagen: %d\n",tam_img);

	fseek(fptr,30,SEEK_SET);
	fread(&img->isCompressed,4,1,fptr);
	printf("Compression type is %d\n", img->isCompressed);
	int tablaCol;
	fseek(fptr,46,SEEK_SET);
	fread(&tablaCol,4,1,fptr);
	printf("tam_tabla_col: %d \n",tablaCol);
	//Se obtiene la imágen.
	fseek(fptr,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.
	//Se extrae la data de la imagen.
	unsigned char *data = (unsigned char*)malloc(sizeof(char)*tam_img);
	fread(data,tam_img,1,fptr);
	//Se asigna memoria para la matriz
	int x;
	int y;
	img->triads = (Triad**)malloc(sizeof(Triad*)*img->height);
	for(x = 0; x<img->width; x++){
		img->triads[x] = (Triad*)malloc(sizeof(Triad)*img->width);
	}
	int count_matrix = 0;
	//Se calcula el salto para el padding.
	int padding = -(img->width*3%4 -4) ;
	//Se inicia la extracción de datos
	for(x=img->height-1; x>=0; x--){
		for(y=0; y<img->width;y++){
			img->triads[x][y].b = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].g = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].r = data[count_matrix];//r
			count_matrix++;
			//Si se llega al final de la fila, se saltan tantos bytes como el padding diga.
			if(y == img->width -1){
				printf("padding %d\n",padding);
				count_matrix+=padding;
			}
		}
	}
	//(OTRA FUNCIÓN)
	//Impresión de la matriz.
	for(x = 0; x<img->height;x++){
		for(y = 0; y<img->width;y++){
			printf("(%d %d %d)",img->triads[x][y].b,img->triads[x][y].g,img->triads[x][y].r);
		}
		printf("\n");
	}
	
	fclose(fptr);

	return img;
}

void convertToGrayScale(Image *img){
	int x;
	int y;

	for(x =0 ; x<img->height; x++){
		for(y = 0; y<img->width; y++){
			img->triads[x][y].b = img->triads[x][y].b*0.11;
			img->triads[x][y].g = img->triads[x][y].g*0.59;
			img->triads[x][y].r = img->triads[x][y].r*0.3;
		}
	}

	for(x = 0; x<img->height;x++){
		for(y = 0; y<img->width;y++){
			printf("(%d %d %d)",img->triads[x][y].b,img->triads[x][y].g,img->triads[x][y].r);
		}
		printf("\n");
	}
}

void Binarization(Image *img, int umbral){
	//Se pasa a escala de grises
	convertToGrayScale(img);
	int x;
	int y;
	int **result = (int**)malloc(sizeof(int)*img->height);
	for(x =0 ; x<img->height; x++){
		result[x] = (int*)malloc(sizeof(int)*img->width);
		for(y = 0; y<img->width; y++){
			if((img->triads[x][y].r + img->triads[x][y].g + img->triads[x][y].b) > umbral ){
					
				//Copiar la imágen y binarizarla.
			}
		}
	}
}
    


