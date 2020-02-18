.PHONY: all
all: first second

first: main.cpp sub.cpp server.cpp
	g++ -c server.cpp
	g++ -c main.cpp -Wall
	g++ -c sub.cpp

second: first main.o sub.o server.o
	g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	g++ sub.o -o game_sub -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	g++ server.o -o server -lzmq -lpthread -Wall

spec: main.cpp sub.cpp server.cpp
	x86_64-w64-mingw32-g++ main.cpp -o game.exe -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	x86_64-w64-mingw32-g++ sub.cpp -o game_sub.exe -lsfml-graphics -lsfml-window -lsfml-system -lzmq -lpthread -Wall
	x86_64-w64-mingw32-g++ server.cpp -o server.exe -lzmq -lpthread -Wall


.PHONY: clean
clean:
	rm -f *.o game
