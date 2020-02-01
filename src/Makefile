.PHONY: all
all: first second
%two: first second

first: main.cpp sub.cpp server.cpp
	g++ -c main.cpp -Wall
	g++ -c sub.cpp
	g++ -c server.cpp

second: first main.o sub.o
	g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	g++ sub.o -o game_sub -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	g++ server.o -o server -lzmq -lpthread -Wall

.PHONY: clean
clean:
	rm -f *.o game
