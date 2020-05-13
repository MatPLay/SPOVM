CC = gcc
CFLAGS = $(OPTIMIZATION) $(WARNINGS) -D_POSIX_SOURCE -lpthread 
OPTIMIZATION = 
WARNINGS = -Wshadow -Wall -Wextra -Wpedantic -Wbad-function-cast -Wcast-qual
DEPS = Makefile
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

lab3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f lab3 main.o
