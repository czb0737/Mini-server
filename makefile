CC = g++ -o
PT = -pthread
DB = -L./lib -lmysqlclient

install:
	$(CC) ./access_layer/server $(PT) ./access_layer/server.cpp
	$(CC) ./logic_layer/user/server $(PT) ./logic_layer/user/server.cpp
	$(CC) ./logic_layer/friend/server $(PT) ./logic_layer/friend/server.cpp
	$(CC) ./logic_layer/message/server $(PT) ./logic_layer/message/server.cpp
	$(CC) ./storage_layer/server $(PT) ./storage_layer/server.cpp $(DB)
#	rm ./access_layer/server.o
#	rm ./logic_layer/user/server.o
#	rm ./logic_layer/friend/server.o
#	rm ./logic_layer/message/server.o
#	rm ./storage_layer/server.o
run:
	./access_layer/server &
	./logic_layer/user/server &
	./logic_layer/friend/server &
	./logic_layer/message/server &
	./storage_layer/server &
	
#.PHONY: clean
#clean:
#	rm -rf ./
