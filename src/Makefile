PACKAGE = /usr/local/u++-7.0.0
include ${PACKAGE}/CONFIG

CXX = ${INSTALLBINDIR}/${UPP}
FLAGS= -g -Wall


all: clear clean lab

lab:
	u++ $(FLAGS) Main.cc -o lab


clear:
	clear
clean:
	rm -f *.o *.swp lab.exe lab

