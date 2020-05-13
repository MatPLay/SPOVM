// lb5.h

#pragma once

#ifdef USE_DLL
#define MY_DLL_EXPORT __declspec(dllexport)
#else
#define MY_DLL_EXPORT __declspec(dllimport)
#endif

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#define SIZE 100
 
extern MY_DLL_EXPORT char file_readed[5];
extern MY_DLL_EXPORT char* buffer;
extern MY_DLL_EXPORT char bytes_written;

MY_DLL_EXPORT DWORD WINAPI reader(CONST LPVOID lpParam);
MY_DLL_EXPORT DWORD WINAPI writer(CONST LPVOID lpParam);