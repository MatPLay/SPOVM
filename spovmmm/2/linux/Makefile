CC = gcc
CFLAGS = -D_POSIX_SOURCE -D_XOPEN_SOURCE
DEPS = Makefile process.h 
OBJ = main.o process.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lab2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

