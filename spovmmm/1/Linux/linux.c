#include "linux.h"
void draw_borders(WINDOW* scr)
{
  int cols = 0, lines = 0;

  getmaxyx(scr, lines, cols);

  mvwprintw(scr, 0, 0, "+");
  mvwprintw(scr, lines - 1, 0, "+");
  mvwprintw(scr, 0, cols - 1, "+");
  mvwprintw(scr, lines - 1, cols - 1, "+");

  for (int i = 1; i < lines - 1; i++)
    {
      mvwprintw(scr, i, 0, "|");
      mvwprintw(scr, i, cols - 1, "|");
    }

   for (int i = 1; i < cols - 1; i++)
    {
      mvwprintw(scr, 0, i, "-");
      mvwprintw(scr, lines - 1, i, "-");
    }
}

void countdown(WINDOW* win)
{
  int cols = 0, lines = 0;

  getmaxyx(win, lines, cols);

  const char* header = "Fork countdown:";
  
  mvwprintw(win, lines/2 - 1, (cols - strlen(header))/2 , header);
  mvwprintw(win, lines/2, (cols - strlen(header))/2 - 3 ,"[");
  mvwprintw(win, lines/2, (cols + strlen(header))/2 + 1 ,"]");

  int i = 0;
  while(i < (strlen(header) + 4)/3)
    {
      mvwprintw(win, lines/2, (cols - strlen(header))/2 - 2 + 3*i,"###");
      i++;
      wrefresh(win);
      sleep(1);
    }
}
