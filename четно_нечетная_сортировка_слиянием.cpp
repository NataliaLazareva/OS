#include "pch.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

//ЧЕТНО-НЕЧЕТНАЯ СОРТИРОВКА СЛИЯНИЕМ
using namespace std;

struct Str
{
	int x;
	int y;
};

int n;
int *A;

DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int main()
{
	cin >> n;
	A = new int[n];
	srand(GetTickCount());
	for (int i = 0; i < n; i++)
	{
		A[i] = rand() % 10;
		cout << A[i] << ' ';
	}
	cout << "\n";

	Str *num = new Str[n / 2];
	HANDLE  *hThreads = new HANDLE[n / 2];

	//фаза
	for (int i = 1; i < n; i *= 2)
	{
		//длина сравнивающих устройств на одном шаге
		for (int l = i; l > 0; l /= 2)
		{
			int k = 0; //индекс в массиве структур

			//начальный отступ от первого узла
			for (int j = l % i; j + l < n; j += 2 * l)
			{
				int s = 0;
				while (s < l)
				{
					if (((j + s) / (2 * i) != (j + s + l) / (2 * i)) || j + l + s >= n) break;
					
					num[k].x = j + s;
					num[k].y = j + s + l;
					cout << '[' << j + s << ',' << j + s + l << "] ";
					hThreads[k] = CreateThread(0, 0, MyThreadFunction, (LPVOID)(num + k), 0, 0);
					s++; k++;

				}
			}
			cout << endl;
			WaitForMultipleObjects(k, hThreads, TRUE, INFINITE);
			for (int i = 0; i < k; i++)
			{
				CloseHandle(hThreads[i]);
			}
		}
	}
	for (int i = 0; i < n; i++)
		cout << A[i] << " ";
	cout << "\n";

	delete[] hThreads, A;

	return 0;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	Str* num = (Str*)lpParam;
	int i = num->x;
	int j = num->y;

	if (A[i] > A[j]) swap(A[i], A[j]);

	return 0;
}