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
USER_TARGET = bin/userServer
USER_OBJS = server.o ThreadPool.o userHandler.o
FRIEND_TARGET = bin/friendServer
FRIEND_OBJS = server.o ThreadPool.o friendHandler.o
MESSAGE_TARGET = bin/messageServer
MESSAGE_OBJS = server.o ThreadPool.o messageHandler.o
STORAGE_TARGET = bin/storageServer
STORAGE_OBJS = server.o ConnectionPool.o ThreadPool.o storageHandler.o

all: access storage user friend message

# Access layer
access: $(ACCESS_TARGET)
$(ACCESS_TARGET): $(ACCESS_OBJS)
	$(CC) $(FLAG) -o $(ACCESS_TARGET) $(ACCESS_OBJS) $(PT)
accessHandler.o: $(INCLUDE_HANDLER)/accessHandler.h $(SRC_HANDLER)/accessHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/accessHandler.cpp

# User module
user: $(USER_TARGET)
$(USER_TARGET): $(USER_OBJS)
	$(CC) $(FLAG) -o $(USER_TARGET) $(USER_OBJS) $(PT)
userHandler.o: $(INCLUDE_HANDLER)/userHandler.h $(SRC_HANDLER)/userHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/userHandler.cpp

# Friend module
friend: $(FRIEND_TARGET)
$(FRIEND_TARGET): $(FRIEND_OBJS)
	$(CC) $(FLAG) -o $(FRIEND_TARGET) $(FRIEND_OBJS) $(PT)
friendHandler.o: $(INCLUDE_HANDLER)/friendHandler.h $(SRC_HANDLER)/friendHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/friendHandler.cpp

# Message module
message: $(MESSAGE_TARGET)
$(MESSAGE_TARGET): $(MESSAGE_OBJS)
	$(CC) $(FLAG) -o $(MESSAGE_TARGET) $(MESSAGE_OBJS) $(PT)
messageHandler.o: $(INCLUDE_HANDLER)/messageHandler.h $(SRC_HANDLER)/messageHandler.cpp
	$(CC) $(FLAG) -c $(SRC_HANDLER)/messageHandler.cpp

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
	./bin/accessServer &
	./bin/userServer &
	./bin/friendServer &
	./bin/messageServer &
	./bin/storageServer &
	
.PHONY: clean
clean:
	rm *.o
