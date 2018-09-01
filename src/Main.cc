#include <iostream>
#include "Monitor.cc"
#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>

using namespace std;
void readImage(FILE *file_pointer, string fileName);
void* read(void* x);
void* write(void *arg);
void* bin(void * arg);
void* nearly(void * arg);
void* gray(void * arg);
void cpy_img(string nameFile, string nameFileOut);
BoundedBuffer buffer_gray;
BoundedBuffer buffer_bin;
BoundedBuffer buffer_nearlyB;
BoundedBuffer buffer_write;



int main(int argc, char **argv)
{

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
    if((numImgs == -1) || (umbral == -1) || (umbralNearlyBlack == -1)){
        printf("\nNo ha ingresado todas las opciones, revisar leeme\n\n");
        return -1;
    }
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

    InitialData *initialData = new InitialData();
    initialData->umbral = umbral;
    initialData->umbralNearyBlack = umbralNearlyBlack;
    initialData->flag = flag;
    pthread_t threads_read[numImgs];
    pthread_t threads_bin[numImgs];
    pthread_t threads_gray[numImgs];
    pthread_t threads_nearlyB[numImgs];
    pthread_t threads_write[numImgs];
    
    int rc;
    
    for(int i = 0; i<numImgs; i++){
        pthread_create(&threads_read[i], NULL, &read, (void*) i+1);
    }
    for(int i = 0; i<numImgs; i++){
        pthread_create(&threads_bin[i], NULL, &gray, NULL);
    }
    for(int i = 0; i<numImgs; i++){
        pthread_create(&threads_gray[i], NULL, &bin, (void*)initialData);
    }
    for(int i = 0; i<numImgs; i++){
        pthread_create(&threads_nearlyB[i], NULL, &nearly, (void*)initialData);
    }
    for(int i = 0; i<numImgs; i++){
        pthread_create(&threads_write[i], NULL, &write, (void*)initialData);
    }
    for(int x = 0; x<5; x++){
        for(int i = 0; i<numImgs; i++){
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
    exit(0);
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
    
    //Copiar imagen
    string fileNameOut="binarizado-";
	fileNameOut=fileNameOut+nombreImagen; 
	cpy_img(nombreImagen,fileNameOut); 

	//data.&img->filePointer = openImage(fileNameOut); //Se abre imagen
	//readImage(data.img,data.&img->filePointer);
    FILE *imagePtr = openImage(nombreImagen);
    readImage(imagePtr, fileNameOut);
}


void* write(void *arg){
    InitialData *initialData = (InitialData*)arg;
    Image *img = (Image*)buffer_write.remove();
    int count_matrix = 0;
    FILE *imagePtr = openImage(img->fileName);
    vector<unsigned char> data;
    int nPixels = img->height * img->width;
	for(int x=0 ; x  < nPixels; x++){ 
        data.push_back(img->triadas[x]->b);//r
        data.push_back(img->triadas[x]->g);//r 
        data.push_back(img->triadas[x]->r);//r 
        if(img->bitPerPixel == 32){
		    data.push_back(255);//r 
        }
    }
	fseek(imagePtr,0,SEEK_SET);	
	fseek(imagePtr,img->dataPointer,SEEK_SET); //Se busca el puntero a la data de pixeles
	for(int x=0; x<data.size(); x++){
		fwrite(&data[x], sizeof(unsigned char), 1, imagePtr);
	}
    if(initialData->flag == 1){
        if(img->nearlyBlack){
        cout << "| "<<img->fileName <<" | nb? = yes"<<endl;
        }else{
            cout << "| "<<img->fileName <<" | nb? = no"<<endl;
        }
    }
     //Se libera memoria de data
}

void* bin(void * arg){
    InitialData *initialData = (InitialData*)arg;
    Image *img = (Image*)buffer_bin.remove();
    int x;
    int umbral = 50;
	for(x = 0; x<img->triadas.size(); x++){
		if(img->triadas[x]->r > initialData->umbral){
			img->triadas[x]->r = 255;
            img->triadas[x]->g = 255;
            img->triadas[x]->b = 255;
		} else {
			img->numberBlacks++;
			img->triadas[x]->r = 0;
            img->triadas[x]->g = 0;
            img->triadas[x]->b = 0;
		}
        

	}
    buffer_nearlyB.insert(img);
    //Image r = buffer_bin.remove();
    //cout<<"Estoy removiendo el valor HAGO BIN=> "<< r <<endl;
    //buffer_nearlyB.insert((long)r);
}
void* nearly(void * arg){
    //Image r = buffer_nearlyB.remove();
    InitialData *initialData = (InitialData*)arg;
    int x;
    Image *img = (Image*)buffer_nearlyB.remove();
    int countBlack = 0;
	for(x = 0; x<img->triadas.size(); x++){
		if(img->triadas[x]->b == 0){
            countBlack++;
        }
	}
    if(countBlack > (img->triadas.size()*(initialData->umbralNearyBlack))/100){
        img->nearlyBlack = true;
        
    }else{
        img->nearlyBlack = false;
       
    }
    buffer_write.insert(img);
		//se retorna la particion
	return NULL;




    //buffer_write.insert((long)r);
}
void* gray(void * arg){
    int x;
    Image *img = (Image*)buffer_gray.remove();
	for(x = 0; x<img->triadas.size(); x++){
		int calculo = img->triadas[x]->b*0.11+img->triadas[x]->g*0.59+img->triadas[x]->r*0.3;
		img->triadas[x]->b = calculo;
		img->triadas[x]->g = calculo;
		img->triadas[x]->r = calculo;
		img->triadas[x]->a = 255;
	}
    buffer_bin.insert(img);
		//se retorna la particion
	return NULL;

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
    Image *img = new Image();
    img->umbralNearyBlack = 50;
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
        if(img->bitPerPixel == 32){
            triada->a = data[count_matrix];
            count_matrix++;
        }
        img->triadas.push_back(triada);
    }

    /*for(auto it = img->triadas.begin(); it != img->triadas.end(); it++){ 
        cout << *it << endl;
    } */   
    buffer_gray.insert(img);

	//Se libera memoria de data
	//free(data);
}