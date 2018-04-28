#include "image.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*Función que maneja las imagenes. Se encarga de llamar a las funciones que abren, leen y cierran los archivos
para mantener un encapsulamiento de estas.
Entrada: char *file_name (Nombre archivo de entrada), int umbral, int umbralNearlyBlack
Salida: Entero representando si la imagen es NearlyBlack o no.
*/
int imageHandler(char *file_name, int umbral, int umbralNearlyBlack){
  	Image *img = (Image*)malloc(sizeof(Image));


    convertToGrayScale(img); //Se convierte a escala de grises

    int numBlacks = binarization(img,umbral); //Se crea la matriz con la binarización

    writeGrayImage(img, file_pointer); //Finalmente se escribe la imágen resultante.

    closeImage(file_pointer); //Se cierra la imagen
   	int totalPixels = img->width * img->height; //Se calcula el total de pixeles para luego calcular el porcentaje

	for(int x = 0; x<img->width; x++){ //Se libera memoria de img
        free(img->triads[x]);
    }
    free(img->triads);
    free(img);

   	return nearlyBlack(numBlacks,totalPixels,umbralNearlyBlack);
}
/*Función que copia la imagen original para no modificarla y trabajar en una imagen separada.
Entrada: char *nameFile (nombre del archivo de entrada), char *nameFileOut (Nombre del archivo de salida)
Salida: Void
*/
void cpy_img(char *nameFile, char *nameFileOut){
	char *command = (char*)malloc(sizeof(char)*100);
	strcpy(command,"cp ");
	char *cpy_nameFile = (char*)malloc(sizeof(char)*100);
	strcpy(cpy_nameFile,nameFile); //nombre del archivo
	strcat(command,cpy_nameFile); // cp nameFile
	strcat(command," ./"); //cp nameFile ./
	strcat(command,nameFileOut); //cp nameFile ./nameFileOut
	system(command);
}

/*Función que lee los datos de la imagen desplazandose sobre ella por los bytes. Guarda los datos
en la estructura img.
Entrada: Struct Image, FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
int readImage(Image *img, FILE *file_pointer){
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
	img->tam_img = tam_img;
	fseek(file_pointer,30,SEEK_SET);
	fread(&img->isCompressed,4,1,file_pointer);
	int tablaCol;
	fseek(file_pointer,46,SEEK_SET);
	fread(&tablaCol,4,1,file_pointer);

	fseek(file_pointer,img->dataPointer,SEEK_SET); //Se avanza tantos como el data pointer desde el inicio.

	unsigned char *data = (unsigned char*)malloc(sizeof(char)*tam_img);
	fread(data,tam_img,1,file_pointer); //Se extrae la data de la imagen.

	int x;
	int y;
	img->triads = (Triad**)malloc(sizeof(Triad*)*img->height); //Se asigna memoria para la matriz
	for(x = 0; x<img->width; x++){
		img->triads[x] = (Triad*)malloc(sizeof(Triad)*img->width);
	}
	int count_matrix = 0;
	for(x=img->height-1; x>=0; x--){ //Se inicia la extracción de datos
		for(y=0; y<img->width;y++){
			img->triads[x][y].b = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].g = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].r = data[count_matrix];//r
			count_matrix++;
			img->triads[x][y].a = data[count_matrix];//r
			count_matrix++;
		}
	}

	//Se libera memoria de data
	free(data);

	return 0;

}




/* Función que escribe en la imagen los datos de la matriz de pixeles en escala de grises.
Entrada: Struct Image, FILE *file_pointer (con puntero a la imagen con la que se está trabajando)
Salida: Void
*/
void writeGrayImage(Image *img, FILE *file_pointer){

	int count_matrix = 0;
	unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*(img->tam_img+img->width));
    for(int x=img->height-1; x>=0; x--){
        for(int y=0; y<img->width;y++){
            data[count_matrix] = img->triads[x][y].b;//r
            count_matrix++;
            data[count_matrix] = img->triads[x][y].g;//r
            count_matrix++;
            data[count_matrix] = img->triads[x][y].r;//r
            count_matrix++; data[count_matrix] = 255;//r
            count_matrix++;
            }
    }
	fseek(file_pointer,0,SEEK_SET);
	fseek(file_pointer,img->dataPointer,SEEK_SET); //Se busca el puntero a la data de pixeles
	for(int x=0; x<count_matrix; x++){
		fwrite(&data[x], sizeof(unsigned char), 1, file_pointer);
	}

    free(data); //Se libera memoria de data


}

/*Función que abre un archivo, y comprueba de que exista.
Entrada: char *file_name (Nombre del archivo)
Salida: FILE * con el puntero al archivo o NULL.
*/
FILE *openImage(char *file_name){
	FILE *file_pointer = fopen(file_name, "r+");
    //Se comprueba que la imagen exista
    if ((file_pointer == NULL)){
        return NULL;
    }
    return file_pointer;


}

/*Función que cierra un archivo
Entrada: FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
void closeImage(FILE *file_pointer){
	fclose(file_pointer);
}



/*Función que imprime la matriz de pixeles por pantalla. Para uso propio para comprobar
datos.
Entrada: Struct Image
Salida: Void
*/
void printPixelMatrix(Image *img){
	for(int x = 0; x<img->height;x++){
		for(int y = 0; y<img->width;y++){
			printf("(%d %d %d)",img->triads[x][y].b,img->triads[x][y].g,img->triads[x][y].r);
		}
		printf("\n");
	}
}

/*Función que convierte la matriz de pixeles de acuerdo a la fórmula dada en el enunciado
Entrada: Struct Image
Salida: Void
*/
void convertToGrayScale(Image *img){
	int x;
	int y;

	for(x =0 ; x<img->height; x++){
		for(y = 0; y<img->width; y++){
			int calculo = img->triads[x][y].b*0.11+img->triads[x][y].g*0.59+img->triads[x][y].r*0.3;
			img->triads[x][y].b = calculo;
			img->triads[x][y].g = calculo;
			img->triads[x][y].r = calculo;
			img->triads[x][y].a = 255;
		}
	}


}

/*Función que transforma la matriz de pixeles en escala de grises, a una matriz binarizada, en donde
si el valor del pixel es mayor al umbral, se transforma en un pixel blanco, y si es menor al umbral, se transforma
en un pixel negro. Además, cuenta cuantos pixeles negros tendrá la imagen.
Entrada: Struct Image, Int umbral
Salida: Cantidad de pixeles negros
*/
int binarization(Image *img, int umbral){

	int x;
	int y;
	int numBlacks = 0;
	for(x =0 ; x<img->height; x++){
		for(y = 0; y<img->width; y++){
			if(img->triads[x][y].r > umbral ){
                img->triads[x][y].r = 255;
                img->triads[x][y].g = 255;
                img->triads[x][y].b = 255;
			}else{
                img->triads[x][y].r = 0;
                img->triads[x][y].g = 0;
                img->triads[x][y].b = 0;
                numBlacks++;
            }
		}
	}
	return numBlacks;
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
