CC = g++
FLAG = -Wall -std=c++11
PT = -lpthread
LD = -I/usr/include/cppconn -L/usr/lib
DB = -lmysqlcppconn
INCLUDE = include
SRC = src
INCLUDE_HANDLER = include/handler
SRC_HANDLER = src/handler
ACCESS_TARGET = bin/accessServer
ACCESS_OBJS = server.o ThreadPool.o accessHandler.o
STORAGE_TARGET = bin/storageServer
STORAGE_OBJS = server.o ConnectionPool.o ThreadPool.o storageHandler.o

all: access storage

# Access layer
access: $(ACCESS_TARGET)
$(ACCESS_TARGET): $(ACCESS_OBJS)
	$(CC) $(FLAG) -o $(ACCESS_TARGET) $(ACCESS_OBJS) $(PT)
accessHandler.o: $(INCLUDE_HANDLER)/accessHandler.h $(SRC_HANDLER)/accessHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/accessHandler.cpp

# Storage layer
storage: $(STORAGE_TARGET)
$(STORAGE_TARGET): $(STORAGE_OBJS)
	$(CC) $(FLAG) -o $(STORAGE_TARGET) $(STORAGE_OBJS) $(DB) $(PT)
storageHandler.o: $(INCLUDE_HANDLER)/storageHandler.h $(SRC_HANDLER)/storageHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/storageHandler.cpp
ConnectionPool.o: $(INCLUDE)/ConnectionPool.h $(SRC)/ConnectionPool.cpp
	$(CC) $(FLAG) $(LD) -c $(SRC)/ConnectionPool.cpp $(DB)

server.o: $(SRC)/server.cpp
	$(CC) $(FLAG) -c $(SRC)/server.cpp
ThreadPool.o: $(SRC)/ThreadPool.cpp $(INCLUDE)/ThreadPool.h
	$(CC) $(FLAG) -c $(SRC)/ThreadPool.cpp $(PT)

run:
	./access_layer/server &
	./logic_layer/user/server &
	./logic_layer/friend/server &
	./logic_layer/message/server &
	./storage_layer/server &
	
.PHONY: clean
clean:
	rm *.o
