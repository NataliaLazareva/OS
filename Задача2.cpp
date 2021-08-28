// Задача2.cpp: определяет точку входа для консольного приложения.
#include "stdafx.h"
#include <windows.h>
#include <algorithm>
#include<iostream>
#include <ctime> 
using namespace std;
int n, m;
int *A;
int *B;
int *C;
int *T;
DWORD WINAPI MyThreadFunction(LPVOID lpParam);
int Binary_Search(int key, int left, int right, int f);
int main()
{
	cin >> n >> m;
	A = new int[n];
	B = new int[m];
	C = new int[n + m];
	T = new int[n + m];
	for (int i = 0; i < n+m; i++)
	{
		T[i] = i+1;
	}
	random_shuffle(T, T + n + m);
	A = T;
	B = T + n;
	sort(A, A + n);
	sort(B, B + m);
	for (int i = 0; i < n; i++)
	{
		cout << A[i]<<' ';
	}
	cout << endl;
	for (int i = 0; i < m; i++)
	{
		cout << B[i]<<' ';
	}
	cout << endl;

	HANDLE *hThreads = new HANDLE[n + m];
	for (int i = 0; i < n+m; i++)
	{
		hThreads[i] = CreateThread(NULL, 0, MyThreadFunction, (LPVOID)i, 0, 0);

		if (hThreads[i] == NULL)
			ExitProcess(3);
	}
	WaitForMultipleObjects(n + m, hThreads, TRUE, INFINITE);

	for (int i = 0; i < n + m; i++) cout << C[i] << ' ';

	for (int i = 0; i < n + m; i++)
	{
		CloseHandle(hThreads[i]);
	}
	delete[] T, C, hThreads;
	system("pause");
	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	int I, k;
	I = (int)lpParam;
	if (I < n)
	{
			k = Binary_Search(A[I], 0, m, 0);
			C[I + k] = A[I];
	}
	else
	{
		I -= n;
		k = Binary_Search(B[I], 0, n, 1);
		C[I+k] = B[I];
	}
	return 0;
}

int Binary_Search(int key, int left, int right, int f)
{
	int midd = 0;
	int *S;
	if (f == 0)
	{
		S = new int[m];
		S = B;
	}
	else 
	{
		S = new int[n];
		S = A;
	}
	while (left!=right)
	{
		midd = (left + right) / 2;
		if (key < S[midd])
		{
			right = midd;
		}
		else
			{
				left = (left + right + 1)/2;
			}
	}
	return left;
	delete[] S;
}