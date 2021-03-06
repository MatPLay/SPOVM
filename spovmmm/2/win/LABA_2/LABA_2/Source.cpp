#define _CRT_SECURE_NO_WARNINGS

#define STRING_SIZE 8
#define MAX_PROCESS_LEN 10
#define CREATE_NEW_PROCESS '+'
#define DESTROY_LAST_PROCESS '-' 
#define QUIT 'q'

#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

const char* numbers[] = { 
	"1", "2", "3", "4", "5", "6", "7", "8"
};

char* stringToChars(string str) {
	size_t strSize = str.length();
	char* chars = (char*)malloc((strSize) * sizeof(char));
	for (size_t i = 0; i < strSize; i++) {
		chars[i] = str[i];
	}
	return chars;
}

string getUniqString() {
	string uniqString = "";

	srand(time(nullptr));
	for (size_t i = 0; i < STRING_SIZE; i++) {
		uniqString += numbers[rand() % STRING_SIZE];
	}

	return uniqString;
}

PROCESS_INFORMATION createNewProcess(string appName, string commandLine) {
	STARTUPINFO startupinfo;
	ZeroMemory(&startupinfo, sizeof(startupinfo));
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(processInformation));
	string str = appName + " " + commandLine;
	CreateProcess(nullptr, stringToChars(str), nullptr, nullptr, TRUE,
		NULL, nullptr, nullptr, &startupinfo, &processInformation);
	return processInformation;
}

void printUniqString(char* uniqString) {
	HANDLE canWriteEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "canWrite");
	HANDLE cannotWriteEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, uniqString);
	while (true) {
		if (WaitForSingleObject(canWriteEvent, INFINITE) == WAIT_OBJECT_0) {
			if (WaitForSingleObject(cannotWriteEvent, 50) == WAIT_OBJECT_0) {
				SetEvent(canWriteEvent);
				return;
			}

			for (size_t i = 0; i < STRING_SIZE; i++) {
				cout << uniqString[i];
				Sleep(100);
			}

			cout << endl;

			SetEvent(canWriteEvent);
		}
	}
	return;
}

void processManager(string appName) {
	int activeProcessLen = 0;
	char userChoice = NULL;

	HANDLE canWriteEvent = CreateEvent(nullptr, FALSE, TRUE, "canWrite");

	PROCESS_INFORMATION processInformationArray[MAX_PROCESS_LEN];
	HANDLE cannotWriteEventArray[MAX_PROCESS_LEN];

	while (userChoice = _getch()) {
		if (tolower(userChoice) == CREATE_NEW_PROCESS && activeProcessLen <= MAX_PROCESS_LEN) {
			char* uniqString = stringToChars(getUniqString());
			cannotWriteEventArray[activeProcessLen] = CreateEvent(nullptr, FALSE, FALSE, uniqString);
			processInformationArray[activeProcessLen] = createNewProcess(appName, uniqString);
			activeProcessLen++;
		}
		else if (userChoice == DESTROY_LAST_PROCESS) {
			if (--activeProcessLen >= 0) {
				WaitForSingleObject(canWriteEvent, INFINITE);
				SetEvent(cannotWriteEventArray[activeProcessLen]);
				TerminateProcess((processInformationArray[activeProcessLen]).hProcess, 1);
				SetEvent(canWriteEvent);
			}
		}
		else if (tolower(userChoice) == QUIT) {
			break;
		}
	}

	while (activeProcessLen >= 0) {
		WaitForSingleObject(canWriteEvent, INFINITE);
		SetEvent(cannotWriteEventArray[activeProcessLen]);
		TerminateProcess((processInformationArray[activeProcessLen]).hProcess, 9);
		activeProcessLen--;
		SetEvent(canWriteEvent);
	}

	system("pause");
}

int main(int argc, char* argv[]) {
	if (argc == 2) {
		printUniqString(argv[1]);
	} else {
		processManager(argv[0]);
	}
	return 1;
}