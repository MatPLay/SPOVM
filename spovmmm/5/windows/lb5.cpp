// ������� DLL-����.

#include "stdafx.h"

#include "lb5.h"

char file_readed[5] = {0,0,0,0,0};
char bytes_written = 0;
char* buffer;

DWORD WINAPI reader(CONST LPVOID lpParam)
{
    buffer = (char*) malloc(SIZE);

    printf("Reader thread\n");
    for(int i = 1; i <= 5; i++)
	{
        char name[3];

        itoa(i, name, 10);
 
        HANDLE file_handler = CreateFileA(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

        if (INVALID_HANDLE_VALUE == file_handler)
        {
            perror("CreateFileA");
            exit(EXIT_FAILURE);
        }
 
        int read_size = 10;
        char* read_data = (char*) malloc(read_size);
        
		memset(read_data, 0, read_size);
 
        OVERLAPPED overlapped;

        memset(&overlapped, 0, sizeof(overlapped));
 
        BOOL result = ReadFile(file_handler, read_data, read_size, NULL, &overlapped);
        printf("%s\n", read_data);
 
        while(FALSE == HasOverlappedIoCompleted(&overlapped))
		{
            Sleep(10);
        }

        strcpy(buffer, read_data);

        file_readed[i - 1]++;

        bytes_written += strlen(read_data);
 
        DWORD bytes_readed;

        result = GetOverlappedResult(file_handler, &overlapped, &bytes_readed, TRUE); 

        Sleep(1000);

        CloseHandle(file_handler);
    }
    ExitThread(0);
}
 
DWORD WINAPI writer(CONST LPVOID lpParam)
{
    printf("Writer thread\n");

    HANDLE writer_file = CreateFileA("concatenated", FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

    for(int i = 1; i <=5; i++){
       
        if (INVALID_HANDLE_VALUE == writer_file)
        {
            perror("CreateFileA");
            exit(EXIT_FAILURE);
        }

        DWORD file_pointer = SetFilePointer(writer_file, 0, NULL, FILE_END);
 
        int read_size = 10;

        char* write_data = (char*) calloc (1, read_size);

		while(0 == file_readed[i - 1])
		{
				Sleep(10);
		}	

        strcpy(write_data, buffer);
       
 
        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(overlapped));
        DWORD length;
        GetFileSize(writer_file, &length);
        overlapped.Offset = file_pointer;
       
 
        BOOL result = WriteFile(writer_file, write_data, strlen(write_data), NULL, &overlapped);
        while(FALSE == HasOverlappedIoCompleted(&overlapped))
		{
            printf("Working");
        }

		file_readed[i - 1]++;

        DWORD bytes_readed;

        result = GetOverlappedResult(writer_file, &overlapped, &bytes_readed, TRUE); 
 
       
    }

    CloseHandle(writer_file);

    ExitThread(0);
}