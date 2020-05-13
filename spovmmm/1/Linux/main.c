#include "linux.h"
int main()
{
  int lines = 0, cols = 0;
  
  initscr();

  start_color(); 
  
  curs_set(FALSE);

  getmaxyx(stdscr, lines, cols);

  WINDOW* win = newwin(lines, cols, 0, 0); 

  assume_default_colors(COLOR_WHITE, COLOR_BLUE);
  
  draw_borders(win);

  countdown(win);

  pid_t p = fork();

  switch (p)
    {
    case -1:
      {
	perror("Fork error");
	exit(1);
      }
    case 0:
      {
	char* const cmd[] = {"xfce4-terminal&",
			     "-x", "./child.sh"};
	execvp(cmd[0], cmd);
	break;
      }
    default:
      {
	sleep(228);
	waitpid(p, NULL, 0);
	break;
      }
    }
  
  delwin(win);
  
  endwin();
 
  return 0;
}

