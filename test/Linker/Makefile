CC = g++
FLAG = -Wall -std=c++11

main: main.o Linker.o
	$(CC) main.o Linker.o -o main
main.o: main.cpp
	$(CC) -c main.cpp
Linker.o: Linker.h Linker.cpp
	$(CC) -c Linker.cpp