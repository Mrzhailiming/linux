
CLIENT = ./client
SERVER = ./chatServer

.PHONY:all
	
all:$(CLIENT) $(SERVER)


$(SERVER):chatServer.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -ljson

$(CLIENT):client.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -ljson

.PHONY:clean

clean:
	rm -f ./client ./chatServer
