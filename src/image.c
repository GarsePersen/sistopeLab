#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


Image *loadImage(char *file_name){
    Image *img = (Image*)malloc(sizeof(Image));
    int id_open = open(file_name, O_RDONLY);

    if(id_open < 0){
        printf("Error en la ectura del archivo.");
        return NULL;
    }else{
        printf("El archivo ha sido accedido.\n");
        //Se valida que el archivo sea una imagen en BMP
        char b;
        char m;
        read(id_open,&b,sizeof(char));
        read(id_open,&m,sizeof(char));
        if(b != 'B' || m != 'M'){
            printf("No es un archivo BMP\n");
            return NULL;
        }else{
            printf("Es un archivo BMP\n");
            //Si los 2 primeros bits son BM efectivamente es
            //un archivo bmp
            int x;
            int temp;
            
            //Proceso de extracción del puntero de los datos (bit 10)
            read(id_open,&temp,8);
            read(id_open,&(img->dataPointer),4);
            //Proceso de lectura de dimensiones (bit 18)
            read(id_open,&temp,4);
            read(id_open,&img->width,4); //ancho.
            read(id_open,&img->height,4); //alto.
            //Proceso de extracción de variable de compresión (bit 30);
            read(id_open,&temp,4);
            read(id_open,&img->isCompressed,4); //está comprimido?

            if(img->isCompressed != 0){
                printf("Está comprimido.\n");
            }else{
                printf("No está comprimido.\n");

            }

            printf("Dp: %i\n",img->dataPointer);
            printf("ancho: %i\n",img->width);
            printf("Largo: %i\n",img->height);
            printf("está comprimido?: %i\n",img->isCompressed);

            //Proceso de extracción de tuplas.
            //Han pasado 26 ciclos de lectura por lo tanto 
            int i = 0;
            for(x=0; x<(img->dataPointer - 26); x++){
                i = read(id_open,&temp,1);
            }
            
            printf("i: %i\n",i);
            int y;
            img->triads = (Triad**)malloc(sizeof(Triad*)*img->height);
            for(y = 0; y<(img->height); y++){
                img->triads[y] = (Triad*)malloc(sizeof(Triad)*img->width);
            }
            for(x=0; x<(img->height); x++){
                for(y = 0; y<(img->width); y++){
                    read(id_open,&img->triads[x][y].r,1);
                    //printf("r: %i ",img->triads[x][y].r);
                    read(id_open,&img->triads[x][y].g,1);
                    //printf("g: %i ",img->triads[x][y].g);
                    read(id_open,&img->triads[x][y].b,1);
                    //printf("b: %i \n",img->triads[x][y].b);
                }
            }

            return img;
        }
    }

}