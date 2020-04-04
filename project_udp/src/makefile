
TAGS = -std=c++11 -lpthread -ljson -lncurses
CLIENT = ./client
SERVER = ./chatServer
WIN = ./win

.PHONY:all
	
all:$(CLIENT) $(SERVER) $(WIN)


$(SERVER):chatServer.cpp
	g++ $^ -o $@ $(TAGS)

$(CLIENT):client.cpp
	g++ $^ -o $@ $(TAGS)

$(WIN):chatWindow.cpp 
	g++ $^ -o $@ $(TAGS)

.PHONY:clean

clean:
	rm -f ./client ./chatServer
