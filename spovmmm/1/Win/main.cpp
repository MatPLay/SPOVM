#include "1.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>



int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	
	TCHAR path[MAX_PATH];
	lstrcpy(path, TEXT("G:?\\child_proc.exe"));
	/*insert path*/
	
	print_time(3);

	if (FALSE == CreateProcess(path, NULL, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		perror("Error creating process.\nPress any key to continue");
		getch();
		return 0;
	}
	
	print_time(100);

	return 0;

}
