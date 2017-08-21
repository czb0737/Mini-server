CC = g++
FLAG = -Wall -std=c++11
LD = -I/usr/include/cppconn -L/usr/lib
DB = -lmysqlcppconn

main: main.o ConnectionPool.o
	$(CC) $(FLAG) main.o ConnectionPool.o -o main $(DB)
main.o: main.cpp
	$(CC) $(FLAG) -c main.cpp
ConnectionPool.o: ConnectionPool.h ConnectionPool.cpp
	$(CC) $(FLAG) $(LD) -c ConnectionPool.cpp $(DB)

gg: main.cpp ConnectionPool.h ConnectionPool.cpp
	$(CC) $(FLAG) main.cpp -o main