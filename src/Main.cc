#include <iostream>
#include "Monitor.cc"
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>

using namespace std;

#define NUM_THREADS 10
void readImage(FILE *file_pointer, string fileName);
void* read(void* x);
void* gray(void *arg);
void* bin(void * arg);
void* nearly(void * arg);
void* write(void * arg);
void cpy_img(string nameFile, string nameFileOut);
BoundedBuffer buffer_gray;
BoundedBuffer buffer_bin;
BoundedBuffer buffer_nearlyB;
BoundedBuffer buffer_write;



int main()
{


    pthread_t threads_read[NUM_THREADS];
    pthread_t threads_bin[NUM_THREADS];
    pthread_t threads_gray[NUM_THREADS];
    pthread_t threads_nearlyB[NUM_THREADS];
    pthread_t threads_write[NUM_THREADS];
    
    int rc;
    
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_read[i], NULL, &read, (void*) i+1);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_bin[i], NULL, &gray, (void*) i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_gray[i], NULL, &bin, (void *)i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_nearlyB[i], NULL, &nearly, (void *)i);
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_create(&threads_write[i], NULL, &write, (void *)i);
    }
    for(int x = 0; x<5; x++){
        for(int i = 0; i<NUM_THREADS; i++){
            if(x ==0){
                pthread_join(threads_bin[i],NULL);
            }else if(x==1){
                pthread_join(threads_gray[i],NULL);
            }else if(x==2){
                pthread_join(threads_write[i],NULL);
            }else if(x==3){
                pthread_join(threads_nearlyB[i],NULL);
            }else if(x==4){
                pthread_join(threads_read[i],NULL);
            }else{
                cout << "Error" <<endl;
            }
        }
    }
    
    return 0;
}


FILE *openImage(string file_name){
	FILE *file_pointer = fopen(file_name.c_str(), "r+");
    //Se comprueba que la imagen exista
    if ((file_pointer == NULL)){
        return NULL;
    }
    return file_pointer;
	

}
void* read(void* x){
    //Crear nombre de imagen
    stringstream strstream;
    stringstream ss;
    string nImagen;
    strstream << (long)x;
    strstream >> nImagen;
    ss<<"image_"<<nImagen;
    string nombreImagen = ss.str();
    nombreImagen=nombreImagen+".bmp";
    cout<<"Estoy leyendo "<<nombreImagen<<endl;
    
    //Copiar imagen
    string fileNameOut="binarizado-";
	fileNameOut=fileNameOut+nombreImagen; 
	cpy_img(nombreImagen,fileNameOut); 

	//data.&img->filePointer = openImage(fileNameOut); //Se abre imagen
	//readImage(data.img,data.&img->filePointer);
    FILE *imagePtr = openImage(nombreImagen);
    readImage(imagePtr, fileNameOut);
}


void* gray(void *arg){
    Image img = buffer_gray.remove();
    int count_matrix = 0;
    FILE *imagePtr = openImage(img.fileName);
    unsigned char *data = (unsigned char*)malloc(sizeof(unsigned char)*img.tam_img); 
    int nPixels = img.height * img.width;
	for(int x=0 ; x  < nPixels; x++){ 
        data[count_matrix] = img.triadas[x]->b;//r
        count_matrix++; 
        data[count_matrix] = img.triadas[x]->g;//r 
        count_matrix++; 
        data[count_matrix] = img.triadas[x]->r;//r 
        count_matrix++; 
		data[count_matrix] = 255;//r 
        count_matrix++; 
    }
	fseek(imagePtr,0,SEEK_SET);	
	fseek(imagePtr,img.dataPointer,SEEK_SET); //Se busca el puntero a la data de pixeles
	for(int x=0; x<count_matrix; x++){
		fwrite(&data[x], sizeof(unsigned char), 1, imagePtr);
	}
    cout << "Escribi" << endl;
    free(data); //Se libera memoria de data
}

void* bin(void * arg){
    //Image r = buffer_bin.remove();
    //cout<<"Estoy removiendo el valor HAGO BIN=> "<< r <<endl;
    //buffer_nearlyB.insert((long)r);
}
void* nearly(void * arg){
    //Image r = buffer_nearlyB.remove();
    //cout<<"Estoy removiendo el valor HAGO NEARLY=> "<< r <<endl;
    //buffer_write.insert((long)r);
}
void* write(void * arg){
    //Image r = buffer_write.remove();
    //cout<<"Estoy removiendo el valor HAGO WRITE=> "<< r <<endl;
}

void cpy_img(string nameFile, string nameFileOut){
    string command="cp ";
    string cpy_nameFile = nameFile;
    command=command+cpy_nameFile+" ./"+nameFileOut; // cp nameFile
    system(command.c_str());
}



/*Función que lee los datos de la imagen desplazandose sobre ella por los bytes. Guarda los datos
en la estructura &img->
Entrada: Struct Image, FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
/*Función que lee los datos de la imagen desplazandose sobre ella por los bytes. Guarda los datos
en la estructura &img->
Entrada: Struct Image, FILE *file_pointer (puntero a la imagen con la que se está trabajando)
Salida: Void
*/
void readImage(FILE *file_pointer, string fileName){
    cout <<"Lei " <<endl;
    Image *img = new Image();
    img->fileName = fileName;
	fread(&(img->type), 1, 1, file_pointer); //1
	fread(&(img->type2), 1, 1, file_pointer); //1
	/*if((&img->type != 'B' ) && (&img->type != 'M')){ //Se comprueba que el archivo sea del tipo bmp
	    free(img);
	    return -1;
	}*/


	fread(&(img->fileSize), 4, 1, file_pointer);//5

	fread(&(img->reserved1), 2, 1, file_pointer);//7

	fread(&(img->reserved2), 2, 1, file_pointer);//9

	fread(&(img->dataPointer), 4, 1, file_pointer);//13



	fseek(file_pointer,4,SEEK_CUR); //4 desplazamientos
	fread(&(img->width), 4, 1, file_pointer);//18 .ancho
	fread(&(img->height), 4, 1, file_pointer);//21 .Largo



	fseek(file_pointer,26,SEEK_SET);
	fread(&(img->planes), 2, 1, file_pointer);//Planos

	fseek(file_pointer,28,SEEK_SET);
	fread(&(img->bitPerPixel), 2, 1, file_pointer);//bits x pixel
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
    img->filePointer = file_pointer;
	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char *)*tam_img);
	fread(data,tam_img,1,file_pointer); //Se extrae la data de la imagen.
	int x;
	int count_matrix = 0;
	for(x=0; x<img->height*img->width; x++){ //Se inicia la extracción de datos
        Triad *triada = new Triad();
        triada->b = data[count_matrix];
		count_matrix++;
        triada->g = data[count_matrix];
		count_matrix++;
        triada->r = data[count_matrix];
		count_matrix++;
        triada->a = data[count_matrix];
		count_matrix++;
        img->triadas.push_back(triada);
    }

    /*for(auto it = img->triadas.begin(); it != img->triadas.end(); it++){ 
        cout << *it << endl;
    } */   
    buffer_gray.insert(*img);

    cout << "SALI " <<endl;
	//Se libera memoria de data
	//free(data);
}