#include "image.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argc, char const *argv[]) {
   

	int fd = atoi(argv[2]);
   
	Image *img = (Image*)malloc(sizeof(img));
	char *file_name = (char*)argv[1];
	char *fileNameOut = (char*)malloc(sizeof(char)*100); //Se asigna un nombre al archivo de salida.
	strcpy(fileNameOut,"binarizado-"); //Se guarda el archivo original
	strcat(fileNameOut,file_name); //Se asigna un identificador al archivo
	cpy_img(file_name,fileNameOut); //Se copia el archivo
	FILE *file_pointer = openImage(fileNameOut); //Se abre imagen
	unsigned char *resultado = readImage(img, file_pointer); //Se lee la data
	
	//IMPORTANTE NO OLVIDAR VALIDAR
	/*if(resultado == -1){ //Si la imagen no es bmp
		return -1;
	}*/

	write(fd, img, sizeof(Image));
	
	int x,y;
	for(x = 0; x<img->height; x++){
		for(y = 0; y<img->width; y++){
			write(fd, &img->triads[x][y].r, sizeof(unsigned char));
			write(fd, &img->triads[x][y].g, sizeof(unsigned char));
			write(fd, &img->triads[x][y].b, sizeof(unsigned char));
			write(fd, &img->triads[x][y].a, sizeof(unsigned char));
		}
	}
	// for(int aux = 0; aux<img->tam_img; aux++){
		// write(fd, &resultado[aux], sizeof(unsigned char));
	// }
	
	//Pipe tercer pipe
	/*
	int pipe_read[2];
	pipe(pipe_read);
	if((pidLecturaImg = fork())==0){
		//Si es el hijo.
		close(pipe_read[0]);
		//Se convierte pipe a char*
		char pipe_to_string[12];
		snprintf(pipe_to_string, 12, "%i", pipe_read[1]);
		printf("hola");
		int res = execlp("./readImage","readImage","prueba.bmp", &pipe_to_string,(char*)NULL);
		printf("Resultado execlp = %u\n", res);
	}else{
	*/
	
	
	return 0;
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
unsigned char *readImage(Image *img, FILE *file_pointer){
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

	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char)*tam_img);
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
	//free(data);

	return data;

}
