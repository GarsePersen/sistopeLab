CFLAGS= -g -Wall

all: pipeline readImage grayScale binarization category write

pipeline: main_handler.o image.h
	gcc $(FLAGS) image.h main_handler.o -g -o pipeline

readImage: readImage.o
	gcc $(FLAGS) readImage.o -g -o readImage
	
readImage.o: readImage.c image.h
	gcc $(FLAGS) -c readImage.c -o readImage.o

grayScale: grayScale.o
	gcc $(FLAGS) grayScale.o -g -o grayScale

grayScale.o: grayScale.c image.h
	gcc $(FLAGS) -c grayScale.c -o grayScale.o

binarization: binarization.o
	gcc $(FLAGS) binarization.o -g -o binarization

binarization.o: binarization.c image.h
	gcc $(FLAGS) -c binarization.c -o binarization.o

category: category.o
	gcc $(FLAGS) category.o -g -o category

category.o: category.c image.h
	gcc $(FLAGS) -c category.c -o category.o

write: write.o
	gcc $(FLAGS) write.o -g -o write

write.o: write.c image.h
	gcc $(FLAGS) -c write.c -o write.o

clean:
	rm -f .*swp *.o .*swo .*swn *.stackdump binarizado-* *.swo *.swn *.swp *.exe  write pipeline category binarization readImage grayScale

