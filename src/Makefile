FLAGS= -g -Wall

all: lab

lab: main.o image.o
	gcc $(FLAGS) image.o main.o -g -o lab

image.o: image.h image.c
	gcc $(FLAGS) -c image.c -o image.o

clean:
	rm -f *.o lab.exe lab

