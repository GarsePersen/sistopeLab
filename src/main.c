#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char **argv){
	
	int imageNumber = -1;
	int thresholdB = -1;
	int thresholdC = -1;
	int flag = 0;
	int aux;

	while ((aux = getopt (argc, argv, "c:u:n:b")) != -1){
		switch (aux){
			case 'c':
				imageNumber = atoi(optarg);
				break;
			case 'u':
				thresholdB = atoi(optarg);
				break;
			case 'n':
				thresholdC = atoi(optarg);
				break;
			case 'b':
				flag = 1;
				break;
			default:
				abort();
		}
		

	}
	
	//Comprobaciones
	if ((imageNumber == -1) || (thresholdB == -1) || (thresholdC == -1)){
		printf("Compruebe que ingreso todas las opciones (ver manual)\n");
		return -1;
	}

	printf("cantImagenes: %i \numbralB: %i \numbralC: %i \nbandera: %i\n", imageNumber, thresholdB, thresholdC, flag);
	


	return 0;
}
