CFLAGS= -g -Wall

all: lab readImage

lab: main_handler.o image.h
	gcc $(FLAGS) image.h main_handler.o -g -o lab

readImage: readImage.o
	gcc $(FLAGS) readImage.o -g -o readImage
	
readImage.o: readImage.c image.h
	gcc $(FLAGS) -c readImage.c -o readImage.o
	
clean:
	rm -f .*swp *.o .*swo .*swn  *.swo *.swn *.swp *.exe lab

