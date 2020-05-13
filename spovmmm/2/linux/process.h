#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0
#define LIMIT 21

extern int block;
extern int release;

static const char *process_names[] = { " process", "First", "Second",
			"Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth",
			"Ninth", "Tenth", "Eleventh", "Twelfth", "Thirteenth",
			"Fourteenth", "Fifteenth", "Sixteenth", "Seventeenth",
			"Eighteenth", "Nineteenth", "Twentieth", };

void block_print(int32_t signum);
void release_print(int32_t signum);
void print(int32_t process_number);


