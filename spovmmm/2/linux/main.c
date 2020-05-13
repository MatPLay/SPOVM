#include "process.h"

struct sigaction block_handler;
struct sigaction release_handler;

int block = 0;
int release = 1;

int main() 
{
  int8_t symbol;
  int number_of_process = 0;
  int32_t flag = FALSE;

  int current_process = 0;
  pid_t process_array[LIMIT];

  block_handler.sa_handler = block_print;
  sigaction(SIGUSR1, &block_handler, NULL);
  
  release_handler.sa_handler = release_print;
  sigaction(SIGUSR2, &release_handler, NULL);

  while(TRUE) 
    { 
      symbol = getchar();

      switch (symbol) 
	{
	case '+': 
	  {
	    if (number_of_process < LIMIT - 1) 
	      {
		process_array[number_of_process] = fork();
		number_of_process++;

		switch (process_array[number_of_process - 1])
		  {
		  case 0: 
		    {
		     release = 0;
		      while (0 == release) 
			{
			  usleep(1000);
			  if (1 == block) 
			    {
			      print(number_of_process);
			      block = 0;
			      kill(getppid(), SIGUSR2);
			    }
			}
		      return 0;
		      break;
		    }
		  case -1: 
		    {
		      perror("fork");
		      break;
		    }
		  default: 
		    {
		      break;
		    }
		  }

	      }
	    break;
	  }
	case '-': 
	  {
	    if(number_of_process > 0) 
	      {
		kill(process_array[number_of_process - 1], SIGUSR2);
		number_of_process--;
	      }
	    break;
	  }
	case 'q': 
	  {
	    if(number_of_process > 0) 
	      {
		for (number_of_process; number_of_process >= 0; number_of_process--) 
		  {
		    kill(process_array[number_of_process - 1], SIGUSR2);
		    waitpid(process_array[number_of_process - 1], NULL, 0);
		  }
	      }
	    return 0;
	  }
	}

      if(0 != release && number_of_process > 0)
      	{
      	  release = 0;
      	  if(current_process >= number_of_process - 1)
      	    {
      	      current_process = 0;
      	    }
      	  else if(0 == flag)
      	    {
      	      current_process++;
      	    }
      	  flag = 0;
      	  kill(process_array[current_process], SIGUSR1);
      	}
    }

  return 0;
}
