// The_Barbershop_problem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <queue>
using namespace std;

int customers, s, barbers, seats;//клиенты, циклы, парикмахеры, места на диване
queue <int> TableForCustomers;
queue <int> TableForBarbers;
HANDLE FreeSeats_Semaphore;
HANDLE IsReadyToHaircut_Semaphore;
HANDLE FinishedHaircut_Semaphore;
HANDLE *CustomersCount_Semaphore;
HANDLE customMutex;
HANDLE barberMutex;
DWORD WINAPI ThreadCustomers(LPVOID lpParam);
DWORD WINAPI ThreadBarbers(LPVOID lpParam);
int main()
{
	setlocale(LC_ALL, "Russian");
	cin >> customers >> s >> barbers >> seats;

	FreeSeats_Semaphore = CreateSemaphore(
			NULL, barbers,  barbers, NULL);

	IsReadyToHaircut_Semaphore = CreateSemaphore(
		NULL, 0, barbers, NULL);

	FinishedHaircut_Semaphore = CreateSemaphore(
		NULL, 0, barbers,NULL);
	CustomersCount_Semaphore = new HANDLE[barbers + seats];
	for (int i = 0; i < barbers + seats; i++) {
		CustomersCount_Semaphore[i] = CreateSemaphore(
			NULL,0,	1,NULL);
		TableForCustomers.push(i);
	}
	customMutex = CreateMutex(NULL, FALSE, NULL);
	barberMutex = CreateMutex(NULL, FALSE, NULL);

		HANDLE *bThreads = new HANDLE[barbers];
		HANDLE *cThreads = new HANDLE[customers];

		for (int i = 0; i < barbers; i++)
		{
			bThreads[i] = CreateThread(NULL, 0, ThreadBarbers, (LPVOID)i, 0, 0);
		}
		for (int i = 0; i < customers; i++)
		{
			cThreads[i] = CreateThread(NULL, 0, ThreadCustomers, (LPVOID)i, 0, 0);
		}
		
		WaitForMultipleObjects(customers, cThreads, TRUE, INFINITE);
		WaitForMultipleObjects(customers, bThreads, TRUE, INFINITE);
		

		for (int i = 0; i < customers; i++)
			CloseHandle(cThreads[i]);
		for (int i = 0; i < barbers; i++)
			CloseHandle(bThreads[i]);
		CloseHandle(barberMutex);
		CloseHandle(customMutex);
		CloseHandle(FreeSeats_Semaphore);
		CloseHandle(IsReadyToHaircut_Semaphore);
		CloseHandle(FinishedHaircut_Semaphore);
		for(int i=0; i<seats+customers; i++)
		CloseHandle(CustomersCount_Semaphore[i]);
		delete[] cThreads, bThreads;
		return 0;
}

DWORD WINAPI ThreadBarbers(LPVOID lpParam)
{
	int c;
	int I = (int)lpParam;
	while (true) {
		WaitForSingleObject(IsReadyToHaircut_Semaphore, INFINITE);
		WaitForSingleObject(barberMutex, INFINITE);
		c = TableForBarbers.front();
		ReleaseMutex(barberMutex);
		printf("Парикмахер %d стрижет клиента с номерком %d \n", I, count);
		Sleep((rand() % 3 + 1)*1000);
		ReleaseSemaphore(CustomersCount_Semaphore[count], 1, NULL);
		WaitForSingleObject(FinishedHaircut_Semaphore, INFINITE);
		WaitForSingleObject(customMutex, INFINITE);
		TableForCustomers.push();
		ReleaseMutex(customMutex);

		ReleaseSemaphore(FreeSeats_Semaphore, 1, NULL);
		TableForBarbers.pop();
		printf("Парикмахер %d готов принять следующего клиента \n", I);
		return 0;
	}
}

DWORD WINAPI ThreadCustomers(LPVOID lpParam)
{
	int I = (int)lpParam;
	int i = 0, count;
	while (i < s)
	{
		WaitForSingleObject(customMutex, INFINITE);
		if (TableForCustomers.empty()) {
			printf("Свободных мест нет. Клиент %d уходит из парикмахерской \n", I);
			ReleaseMutex(customMutex);
			Sleep((rand() % 5 + 1) * 1000);
		}
		else {
			i++;
			count = TableForCustomers.front();
			TableForCustomers.pop();
			printf("Клиент %d зашел в парикмахерскую и взял номерок %d \n", I, count);
			ReleaseMutex(customMutex);
		}

		WaitForSingleObject(FreeSeats_Semaphore, INFINITE);
		WaitForSingleObject(barberMutex, INFINITE);
		TableForBarbers.push(count);
		ReleaseMutex(barberMutex);
		printf("Клиент %d садится в кресло парикмахера \n", I);
		ReleaseSemaphore(IsReadyToHaircut_Semaphore, 1, NULL);
		WaitForSingleObject(CustomersCount_Semaphore[count], INFINITE);
		ReleaseSemaphore(FinishedHaircut_Semaphore, 1, NULL);
		printf("Клиент %d постригся и уходит из парикмахерской \n", I);
		Sleep((rand() % 5 + 1) * 1000);
	}
	return 0;
}






