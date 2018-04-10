#include "image.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int imageHandler(char *file_name, int umbral, int umbralNearlyBlack){
  	Image *img = (Image*)malloc(sizeof(Image));

    
    //Se asigna un nombre al archivo de salida.
    char *fileNameOut = (char*)malloc(sizeof(char)*100);
    //Se guarda el archivo original
    strcpy(fileNameOut,"original-");
    //Se asigna un identificador al archivo
    strcat(fileNameOut,file_name);
    //Se copia el archivo
    cpy_img(file_name,fileNameOut);
    //Se abre imagen
    FILE *file_pointer = openImage(file_name);
    //Se lee la data
    readImage(img, file_pointer);
    //Se convierte a escala de grises
    
    convertToGrayScale(img);
	
    int *result = binarization(img,umbral);

    int resultNearlyBlack  = calculateNearlyBlack(result,umbralNearlyBlack);
    //Finalmente se escribe la imágen resultante.
    writeGrayImage(img, file_pointer);
	printf("\nb\n");
    
    //Se cierra la imagen
    closeImage(file_pointer);

    return resultNearlyBlack;
}

void cpy_img(char *nameFile, char *nameFileOut){
    printf("Copia de archivos\n");
    int data1 =0;
    
    FILE *fileIn = fopen(nameFile,"r");
    FILE *file2 = fopen ( nameFileOut , "w" );
    
    while ( (data1 = fgetc ( fileIn )) != EOF ) {
        fputc ( data1, file2 );
    }
    fclose(fileIn);
    
}

void writeGrayImage(Image *img, FILE *file_pointer){
	
	//Se crea estructura para los datos.


	int count_matrix = 0;
	//Se calcula el salto para el padding.
	int padding = 0;
	unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*(img->tam_img+img->width*padding));
	printf("\nCM: %i\n", padding);
	for(int x=img->height-1; x>=0; x--){
		for(int y=0; y<img->width;y++){
			data[count_matrix] = img->triads[x][y].b;//r
			count_matrix++;
			data[count_matrix] = img->triads[x][y].g;//r
			count_matrix++;
			data[count_matrix] = img->triads[x][y].r;//r
			count_matrix++;
			data[count_matrix] = img->triads[x][y].a;//r
			count_matrix++;
			//Si se llega al final de la fila, se saltan tantos bytes como el padding diga.
			if(y == img->width -1){
				int i = padding;
				while (i>0){
					count_matrix++;
					data[count_matrix] = 0;
					i--;
				}
			}
		}
	}

	//Se resetea el puntero al archivo
	fseek(file_pointer,0,SEEK_SET);
	//Se busca el puntero a la data de pixeles
	fseek(file_pointer,img->dataPointer,SEEK_SET);
	for(int x=0; x<count_matrix; x++){
		fwrite(&data[x], sizeof(unsigned char), 1, file_pointer);
	}


	printf("\nSe crea imagen en escalas grises\n");
}

FILE *openImage(char *file_name){
	FILE *file_pointer = fopen(file_name, "r+");
    return file_pointer;
	

}

void closeImage(FILE *file_pointer){
	fclose(file_pointer);
}

void readImage(Image *img, FILE *file_pointer){
	fread(&img->type, 2, 1, file_pointer); //1
	printf("Tipo: %c\n", img->type);

	fread(&img->fileSize, 4, 1, file_pointer);//5
	printf("File size: %d\n", img->fileSize);
	
	fread(&img->reserved1, 2, 1, file_pointer);//7
	printf("Reserved1: %d\n", img->reserved1);

	fread(&img->reserved2, 2, 1, file_pointer);//9
	printf("Reserved2: %d\n", img->reserved2);
	
	//Puntero a datos
	fread(&img->dataPointer, 4, 1, file_pointer);//13
	printf("DP: %d\n", img->dataPointer);	//



	fseek(file_pointer,4,SEEK_CUR); //4 desplazamientos
	fread(&img->width, 4, 1, file_pointer);//18 ->ancho
	fread(&img->height, 4, 1, file_pointer);//21 ->Largo


	
	fseek(file_pointer,26,SEEK_SET);
	fread(&img->planes, 2, 1, file_pointer);//Planos

	fseek(file_pointer,28,SEEK_SET);    
	fread(&img->bitPerPixel, 2, 1, file_pointer);//bits x pixel
	int tam_img = 0;
	fseek(file_pointer,34,SEEK_SET);
	fread(&tam_img,4,1,file_pointer);
	printf("Img_size: %d\n",tam_img);
    img->tam_img = tam_img;
	fseek(file_pointer,30,SEEK_SET);
	fread(&img->isCompressed,4,1,file_pointer);
	int tablaCol;
	fseek(file_pointer,46,SEEK_SET);
	fread(&tablaCol,4,1,file_pointer);
	
    //Se obtiene la imágen.
	fseek(file_pointer,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.
	
    //Se extrae la data de la imagen.
	unsigned char *data = (unsigned char*)malloc(sizeof(char)*tam_img);
	fread(data,tam_img,1,file_pointer);
    
    //Se asigna memoria para la matriz
	int x;
	int y;
	img->triads = (Triad**)malloc(sizeof(Triad*)*img->height);
	for(x = 0; x<img->width; x++){
		img->triads[x] = (Triad*)malloc(sizeof(Triad)*img->width);
	}
	int count_matrix = 0;
	//Se calcula el salto para el padding.
	int padding = -(img->width*4%8 -4) ;
	//Se inicia la extracción de datos
	for(x=img->height-1; x>=0; x--){
		for(y=0; y<img->width;y++){
			img->triads[x][y].b = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].g = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].r = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].a = data[count_matrix];//r
			count_matrix++;
			//Si se llega al final de la fila, se saltan tantos bytes como el padding diga.
			if(y == img->width ){
				count_matrix+=padding;
			}
		}
	}


    //Print's
	/*printf("Image size = %d x %d\n", img->width, img->height);
	printf("Number of colour planes is %d\n", img->planes);
	printf("Bits per pixel is %d\n", img->bitPerPixel);
	printf("tamaño de la imagen: %d\n",tam_img);
	printf("Compression type is %d\n", img->isCompressed);
	printf("tam_tabla_col: %d \n",tablaCol);*/

}

void printPixelMatrix(Image *img){
	for(int x = 0; x<img->height;x++){
		for(int y = 0; y<img->width;y++){
			printf("(%d %d %d)",img->triads[x][y].b,img->triads[x][y].g,img->triads[x][y].r);
		}
		printf("\n");
	}
}

void convertToGrayScale(Image *img){
	int x;
	int y;

	for(x =0 ; x<img->height; x++){
		for(y = 0; y<img->width; y++){
			img->triads[x][y].b = img->triads[x][y].b*0.11+img->triads[x][y].g*0.59+img->triads[x][y].r*0.3;
			img->triads[x][y].g = img->triads[x][y].b*0.11+img->triads[x][y].g*0.59+img->triads[x][y].r*0.3;
			img->triads[x][y].r = img->triads[x][y].b*0.11+img->triads[x][y].g*0.59+img->triads[x][y].r*0.3;
		}
	}


}

int* binarization(Image *img, int umbral){
	
	int x;
	int y;
	int black;
	int total;
	int *result =(int*)malloc(sizeof(int)*2);

	int **resultBin = (int**)malloc(sizeof(int)*img->height);
	for(x =0 ; x<img->height; x++){
		resultBin[x] = (int*)malloc(sizeof(int)*img->width);
		for(y = 0; y<img->width; y++){
			if(img->triads[x][y].r > umbral ){
                img->triads[x][y].r = 255;
                img->triads[x][y].g = 255;
                img->triads[x][y].b = 255;
                img->triads[x][y].a = 255;
			}else{
				black++;
                img->triads[x][y].r = 0;
                img->triads[x][y].g = 0;
                img->triads[x][y].b = 0;
                img->triads[x][y].a = 255;
            }
            total ++;
		}
	}
	result[0] = black;
	result[1] = total;
	return result;
}


int calculateNearlyBlack(int *binarization,  int umbralForClasify){
	int numOfBlacks = binarization[0];
	int numTotalPixels = binarization[1];
	int percentOfBlack = (numOfBlacks*100)/numTotalPixels;
	if(percentOfBlack > umbralForClasify){
		return 1;
	}else{
		return 0;
	}
}
    


