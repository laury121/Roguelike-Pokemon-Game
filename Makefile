CC = gcc
CFLAGS = -O3 -Wall -Wextra -g -std=c99

OBJECTS = main.o terrain.o maps.o pathFinding.o character.o ui.o

main.exe: $(OBJECTS)
	$(CC) $(CFLAGS) -o main.exe $(OBJECTS) -lncurses -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: main.exe
	./main.exe

clean:
	rm -f $(OBJECTS) main.exe