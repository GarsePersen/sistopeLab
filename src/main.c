#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char **argv){
	
	int cantImagenes = -1;
	int umbralB = -1;
	int umbralC = -1;
	int bandera = 0;
	int aux;

	while ((aux = getopt (argc, argv, "c:u:n:b")) != -1){
		switch (aux){
			case 'c':
				cantImagenes = atoi(optarg);
				break;
			case 'u':
				umbralB = atoi(optarg);
				break;
			case 'n':
				umbralC = atoi(optarg);
				break;
			case 'b':
				bandera = 1;
				break;
			default:
				abort();
		}
		

	}
	
	//Comprobaciones
	if ((cantImagenes == -1) || (umbralB == -1) || (umbralC == -1)){
		printf("Compruebe que ingreso todas las opciones (ver manual)\n");
		return -1;
	}

	printf("cantImagenes: %i \numbralB: %i \numbralC: %i \nbandera: %i\n", cantImagenes, umbralB, umbralC, bandera);
	


	return 0;
}
