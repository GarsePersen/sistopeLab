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
        int count = 0;
        read(id_open,&b,sizeof(char));
        read(id_open,&m,sizeof(char));
        count += 2;
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
            count += 8;
            read(id_open,&(img->dataPointer),4);
            count += 4;
            //Proceso de lectura de dimensiones (bit 18)
            read(id_open,&temp,4);
            count += 4;
            read(id_open,&img->width,4); //ancho.
            count += 4;
            read(id_open,&img->height,4); //alto.
            count += 4;
            //Proceso de extracción de variable de compresión (bit 30);
            read(id_open,&temp,4);
            count += 4;
            read(id_open,&img->isCompressed,4); //está comprimido?
            count += 4;

            if(img->isCompressed != 0){
                printf("Está comprimido.\n");
            }else{
                printf("No está comprimido.\n");

            }

            printf("Dp: %i\n",img->dataPointer);
            printf("ancho: %i\n",img->width);
            printf("Altp: %i\n",img->height);
            printf("está comprimido?: %i\n",img->isCompressed);

            //Proceso de extracción de tuplas.
            //Han pasado 26 ciclos de lectura por lo tanto 
            int i = 0;
            printf("dp - 26: %i\n",img->dataPointer - 26);
            printf("count: %i\n",count);
            for(x=0; x<(img->dataPointer - count); x++){
                i = read(id_open,&temp,1);
            }

            
            printf("i: %i\n",i);
            int y;
            //Denuevo
            img->triads = (Triad**)malloc(sizeof(Triad*)*img->height);        
            for(y = 0; y<(img->height); y++){
                img->triads[y] = (Triad*)malloc(sizeof(Triad)*img->width);
            }
            
            for(x= img->height - 1; x>=0; x--){
                for(y = 0; y<img->width; y++){
                    read(id_open,&img->triads[x][y].r,1);
                    //printf("r: %i ",img->triads[x][y].r);
                    read(id_open,&img->triads[x][y].g,1);
                    //printf("g: %i ",img->triads[x][y].g);
                    read(id_open,&img->triads[x][y].b,1);
                    //printf("b: %i \n",img->triads[x][y].b);
                    if(img->width -1 == y){
                        read(id_open,&temp,4);

                    }
                }
            }
            printf("pixel[0,0]: (%i,%i,%i)\n",img->triads[0][0].r,img->triads[0][0].g,img->triads[0][0].b);
            printf("pixel[F,0]: (%i,%i,%i)\n",img->triads[img->height-1][0].r,img->triads[img->height-1][0].g,img->triads[img->height-1][0].b);
            printf("pixel[0,F]: (%i,%i,%i)\n",img->triads[0][img->width-1].r,img->triads[0][img->width-1].g,img->triads[0][img->width-1].b);
            printf("pixel[F,F]: (%i,%i,%i)\n",img->triads[img->height-1][img->width-1].r,img->triads[img->height-1][img->width-1].g,img->triads[img->height-1][img->width-1].b);
            
           int id = 0;
            read(id_open,&id,1);
            printf("r: %i\n",id);
            read(id_open,&id,1);
            printf("g: %i\n",id);
            read(id_open,&id,1);
            printf("b: %i\n",id);

            return img;
        }
    }

}