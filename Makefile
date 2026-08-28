CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -g -std=c++17

OBJECTS = main.o terrain.o maps.o pathfinding.o character.o ui.o csvParser.o pokemon.o battle.o buildings.o

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS) -lncurses -lm

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: main
	./main

clean:
	rm -f $(OBJECTS) main