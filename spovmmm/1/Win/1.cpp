#include "1.h"

void print_time(int sec)
{
	while (0 != sec)
	{
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		printf("%s", asctime(timeinfo));
		Sleep(1000);
		system("CLS");
		sec--;
	}
}