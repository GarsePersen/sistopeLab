FLAGS= -g -Wall

all: lab

lab: main.o image.o
	gcc $(FLAGS) image.o main.o -g -o lab

image.o: image.h image.c
	gcc $(FLAGS) -c image.c -o image.o -pthread

clean:
	rm -f .*swp *.o .*swo .*swn  *.swo *.swn *.swp lab.exe binarizado-*.bmp lab

