.PHONY: all
all: first second

first: main.cpp sub.cpp
	g++ -c main.cpp
	g++ -c sub.cpp

second: first main.o sub.o
	g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread
	g++ sub.o -o game_sub -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread

.PHONY: clean
clean:
	rm -f *.o game
