.PHONY: all
all: first second

first: main.cpp
	g++ -c main.cpp

second: first main.o
	g++ main.o -o game -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: clean
clean:
	rm -f *.o game
