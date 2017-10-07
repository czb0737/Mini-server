CC = g++
FLAG = -Wall -std=c++11
PT = -lpthread
LD = -I/usr/include/cppconn -L/usr/lib
DB = -lmysqlcppconn
TARGET = server
OBJS = server.o ConnectionPool.o ThreadPool.o storageHandler.o

$(TARGET): $(OBJS)
	$(CC) $(FLAG) -o $(TARGET) $(OBJS) $(DB) $(PT)
server.o: server.cpp
	$(CC) $(FLAG) -c server.cpp
ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(CC) $(FLAG) -c ThreadPool.cpp $(PT)
storageHandler.o: storageHandler.h storageHandler.cpp
	$(CC) $(FLAG) -c storageHandler.cpp
ConnectionPool.o: ConnectionPool.h ConnectionPool.cpp
	$(CC) $(FLAG) $(LD) -c ConnectionPool.cpp $(DB)


# g++ -Wall -std=c++11 -I/usr/include/cppconn -L/usr/lib server.cpp -pthread -o server -lmysqlcppconn