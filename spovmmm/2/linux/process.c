#include "process.h"

void block_print(int32_t signum) 
{
  block = 1;
}

void release_print(int32_t signum) 
{
  release = 1;
}

void print(int32_t process_number) 
{ 
  for(int32_t i = 0; i < strlen(process_names[process_number]); i++) 
    {
      printf("%c", process_names[process_number][i]);
      fflush(stdout);
      usleep(100000);
    }
  for(int32_t i = 0; i < strlen(process_names[0]); i++) 
    {
      printf("%c", process_names[0][i]);
      fflush(stdout);
      usleep(100000);
    }
  printf("\n");
  fflush(stdout);
}
