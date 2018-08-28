#include "image.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

/*Función que maneja las imagenes. Se encarga de llamar a las funciones que abren, leen y cierran los archivos
para mantener un encapsulamiento de estas.
Entrada: char *file_name (Nombre archivo de entrada), int umbral, int umbralNearlyBlack
Salida: Entero representando si la imagen es NearlyBlack o no.
*/
void* threadsHandler(void* data_o){
	DataInit *data = (DataInit*)data_o;
	
	

	if( searchThread(pthread_self(),data->dataThreads,data->threads) ==0){
		char *fileNameOut = (char*)malloc(sizeof(char)*100); //Se asigna un nombre al archivo de salida.
		strcpy(fileNameOut,"binarizado-"); //Se guarda el archivo original
		strcat(fileNameOut,data->file_name); //Se asigna un identificador al archivo
		cpy_img(data->file_name,fileNameOut); //Se copia el archivo

		data->img->filePointer = openImage(fileNameOut); //Se abre imagen
		readImage(data->img,data->img->filePointer);
	}
	
	pthread_barrier_wait(&data->barrier); //Se espera a que lleguen todas las hebras para continuar

	

	//Se espera a la hebra que está leyendo.
	
	//Se inicia la escala de grises
	//SC para el calculo de la porcion de imagen que se 
	//envia a la escala de grises.
	
	pthread_mutex_lock(&data->mutexCalculus); 
			unsigned int indexthread = searchThread(pthread_self(),data->dataThreads,data->threads);
			if(data->aux == 0){
				int numPixels = data->img->height * data->img->width;
				data->img->numberPixelsXthread = numPixels/data->threads;
				
				//Se setean el inicio y el fin de las particiones
				data->dataThreads[indexthread].start = 0;
				data->img->partition_start = 0;
				data->dataThreads[indexthread].end = data->img->numberPixelsXthread;
				data->img->partition_end = data->img->numberPixelsXthread;

				data->aux = 1;
				
			}else{
				data->dataThreads[indexthread].start = data->img->partition_start + data->img->numberPixelsXthread + 1;
				data->img->partition_start = data->img->partition_start + data->img->numberPixelsXthread + 1;
				data->dataThreads[indexthread].end  = data->img->partition_end + data->img->numberPixelsXthread +1;	
				data->img->partition_end = data->img->partition_end + data->img->numberPixelsXthread +1;	
			}
			//ajustarPorcion.
	pthread_mutex_unlock(&data->mutexCalculus);

	convertToGrayScale(data->img,data->dataThreads[indexthread].start,data->dataThreads[indexthread].end);
	binarization(data->img,data->umbral,data->dataThreads[indexthread].start,data->dataThreads[indexthread].end);


	/*

	
	
	img->numberPixelsXthread = numPixelsXthread;
	for(int i = 0; i < number_threads+1; i++){
		pthread_mutex_init(&mutex_acum, NULL);
	}
	for(int i = 1; i < number_threads +1; i++){
		pthread_create(&threads[i],NULL,convertToGrayScaleHandler,(void *) img);		
	}
	
	
	
	for(int i = 0; i < number_threads+1; i++){
		pthread_join(threads[i],NULL);
	}

	writeImage(img, img->filePointer);

	return 0; */
}

unsigned int searchThread(pthread_t id, DataThread *threads, int numThread){
	//printf("x: %d\n",x);
	for(int x = 0 ; x<numThread; x++){
		if(id == threads[x].id){
			//printf("Lo encontre index: %d\n",x);
			return x;
		}
	}
	return 0;

}


	
/*Función que lee los datos de la imagen desplazandose sobre ella por los bytes. Guarda los datos
en la estructura img.
Entrada: Struct Image, FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
/*Función que lee los datos de la imagen desplazandose sobre ella por los bytes. Guarda los datos
en la estructura img.
Entrada: Struct Image, FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
void *readImage(Image *img, FILE *file_pointer){
	fread(&img->type, 1, 1, file_pointer); //1
	fread(&img->type2, 1, 1, file_pointer); //1
	/*if((img->type != 'B' ) && (img->type != 'M')){ //Se comprueba que el archivo sea del tipo bmp
	    free(img);
	    return -1;
	}*/


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

	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*tam_img);
	fread(data,tam_img,1,file_pointer); //Se extrae la data de la imagen.

	int x;
	
	img->triads = (Triad**)malloc(sizeof(Triad*)*(img->height*img->width)); //Se asigna memoria para la matriz
	
	int count_matrix = 0;
	for(x=0; x<img->height*img->width; x++){ //Se inicia la extracción de datos
			img->triads[x].b = data[count_matrix];//r
			count_matrix++;
			img->triads[x].g = data[count_matrix];//r
			count_matrix++;
			img->triads[x].r = data[count_matrix];//r
			count_matrix++;
			img->triads[x].a = data[count_matrix];//r
			count_matrix++;	
	}
	

	//Se libera memoria de data
	free(data);


}/*Función que convierte la matriz de pixeles de acuerdo a la fórmula dada en el enunciado
Entrada: Struct Image
Salida: Void
*/
void *convertToGrayScale(Image *img, unsigned int start, unsigned int end){
	int x;

	for(x = start; x<end; x++){
		int calculo = img->triads[x].b*0.11+img->triads[x].g*0.59+img->triads[x].r*0.3;
		img->triads[x].b = calculo;
		img->triads[x].g = calculo;
		img->triads[x].r = calculo;
		img->triads[x].a = 255;
	}
		//se retorna la particion
	return NULL;

}

void binarization(Image *img, int umbral, unsigned int start, unsigned int end){
	
	int x;
	for(x = start; x<end; x++){
		if(img->triads[x].r > umbral){
			img->triads[x].r = 255;
            img->triads[x].g = 255;
            img->triads[x].b = 255;
		} else {
			pthread_mutex_lock(&img->mutexNearlyBlack); 
			img->numberBlacks++;
			pthread_mutex_unlock(&img->mutexNearlyBlack); 

			img->triads[x].r = 0;
            img->triads[x].g = 0;
            img->triads[x].b = 0;
		}

	}
	
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

void writeImage(Image *img, FILE *file_pointer){
	
	int count_matrix = 0;
	unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*img->tam_img); 
    int nPixels = img->height * img->width;
	for(int x=0 ; x  < nPixels; x++){ 
        data[count_matrix] = img->triads[x].b;//r
        count_matrix++; 
        data[count_matrix] = img->triads[x].g;//r 
        count_matrix++; 
        data[count_matrix] = img->triads[x].r;//r 
        count_matrix++; 
		data[count_matrix] = 255;//r 
        count_matrix++; 
    }
	fseek(file_pointer,0,SEEK_SET);	
	fseek(file_pointer,img->dataPointer,SEEK_SET); //Se busca el puntero a la data de pixeles
	for(int x=0; x<count_matrix; x++){
		fwrite(&data[x], sizeof(unsigned char), 1, file_pointer);
	}

    free(data); //Se libera memoria de data


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




	
	
