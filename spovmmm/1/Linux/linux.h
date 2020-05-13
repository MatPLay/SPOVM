#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void draw_borders(WINDOW* scr);
void countdown(WINDOW* win);
