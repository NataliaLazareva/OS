// The_dining_philosophers_problem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

HANDLE phSemaphore;
HANDLE *phMutex;
DWORD WINAPI ThreadProc(LPVOID);

int N, s;//философы, количество циклов 

int main()
{
	setlocale(LC_ALL, "Russian");
	cin >> N >> s;

	phSemaphore = CreateSemaphore(
		NULL,           // default security attributes
		N-1,  // initial count
		N-1,  // maximum count
		NULL);          // unnamed semaphore

	phMutex = new HANDLE[N];
	for (int i = 0; i < N; i++)
	{
		phMutex[i] = CreateMutex(NULL, FALSE, NULL);
	}

	HANDLE *hThreads = new HANDLE[N];

		for (int i = 0; i < N; i++)
		{
			hThreads[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)i, 0, 0);
		}
		WaitForMultipleObjects(N, hThreads, TRUE, INFINITE);

		for (int i = 0; i < N; i++)
			CloseHandle(hThreads[i]);
		
		for (int i = 0; i < N; i++)
		CloseHandle(phMutex[i]);

	CloseHandle(phSemaphore);
	delete[]phMutex, hThreads;
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	int I = (int)lpParam;
	for (int j = 0; j < s; j++) {
		printf("Философ %d %s\n", I, "размышляет");
		Sleep((rand() % 5 + 1)*1000);
		WaitForSingleObject(
			phSemaphore,   // handle to semaphore
			INFINITE); 

		WaitForSingleObject(phMutex[I], INFINITE);
		printf("Философ %d взял левую палочку\n", I);

		WaitForSingleObject(phMutex[(I+1)%N], INFINITE);
		printf("Философ %d %s\n", I, "взял правую палочку и обедает");
		Sleep((rand() % 3 + 1)*1000);
		ReleaseMutex(phMutex[I]);
		ReleaseMutex(phMutex[(I + 1) % N]);

		ReleaseSemaphore(
			phSemaphore,  // handle to semaphore
			1,            // increase count by one
			NULL);       // not interested in previous count
	}
	return 0;
}