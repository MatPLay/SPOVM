CC = gcc
CFLAGS = -lncurses  -std=c11 -Wall -pedantic
DEPS = linux.h
OBJ = main.o linux.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lab1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
