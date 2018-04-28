#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "image.h"


int main(int argc, char const *argv[])
{
	
	return 0;
}



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