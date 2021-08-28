// Задача_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
//#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <stdlib.h>
#include<iostream>
using namespace std;

int n, m;
int*A;
int*B;
int*C;
DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int _tmain()
{
	cin >> n >> m;
	A = new int[n+1];
	B = new int[m+1];
	C = new int[n + m + 1];
	for (int i = 0; i <= n; i++)
	{
		A[i] = rand() % 10+1;
		cout << A[i] << "x^" << i;
		if (i == n)
			cout << "." << endl;
		else cout <<' '<< "+"<<' ';
	}
	
	for (int i = 0; i <= m; i++)
	{
		B[i] = rand() % 10+1;
		cout << B[i] << "x^" << i;
		if (i == m)
			cout << "."<<endl;
		else cout << ' ' << "+" << ' ';
	}
	
	HANDLE *hTreads = new HANDLE[n + m + 1];

	for (int i = 0; i < n + m + 1; i++)
	{
		hTreads[i] = CreateThread(NULL, 0, MyThreadFunction, (LPVOID)i, 0, 0);

	
	}

	WaitForMultipleObjects(n + m + 1, hTreads, TRUE, INFINITE);

	for (int i = 0; i < n + m + 1; i++)
	{
		CloseHandle(hTreads[i]);
	}
	for (int i = 0; i < n + m + 1; i++)
	{
		cout << C[i] << "x^" << i;
		if (i == n+m)
			cout << ".";
		else cout << ' ' << "+" << ' ';
	}
	delete[] A, B, C, hTreads;
	system("pause");
	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int index;
	index = (int)lpParam;
	C[index] = 0;
	for (int i = max(0, index - m); i <= min(n, index); i++)
	{
		C[index] += A[i] * B[index - i];
	}
	return 0;
}