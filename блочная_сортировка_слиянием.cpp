#include "pch.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

//БЛОЧНАЯ СОРТИРОВКА СЛИЯНИЕМ
using namespace std;
int *A;
int n, s;
int q;

struct Str
{
	int x;
	int y;
};

//Сортировка каждого блока
DWORD WINAPI MyThreadFunction(LPVOID lpParam);
//Для четно-нечетной сортировки
DWORD  WINAPI MyThreadFunction_2(LPVOID lpP);

int main()
{
	int p; //количество потоков
	cin >> n;
	cin >> p;
	A = new int[n];
	srand(GetTickCount());
	for (int i = 0; i < n; i++)
	{
		A[i] = rand() % 10;
		cout << A[i] << ' ';
	}
	cout << "\n";

	s = 2*p; //количество блоков
	q = n / (2 * p); //размер блока

	if (n % (2 * p) != 0)
	{
	    s = n / (q + 1); 
		if (n % s*(q+1)!=0)
			s++;
		q++;
	}
	cout << "q = " << q<<"\n";

	cout << "\n";
	HANDLE *hThreads = new HANDLE[p];
	Str *elemArr = new Str[p];
	
	for (int i = 0; i < p; i++)
	{
		elemArr[i].x = 2 * q*i;
		elemArr[i].x = 2 * q*i + q;
		hThreads[i] = CreateThread(0, 0, MyThreadFunction, (LPVOID)(elemArr+i), 0, 0);
	}

	WaitForMultipleObjects(p, hThreads, TRUE, INFINITE);

	//фаза
	for (int i = 1; i < s; i *= 2)
	{
		//длина сравнивающих устройств на одном шаге
		for (int l = i; l > 0; l /= 2)
		{
			int k = 0; //индекс в массиве структур

			//начальный отступ от первого узла
			for (int j = l % i; j + l < n; j += 2 * l)
			{
				int f = 0;
				while (f < l)
				{
					if (((j + f) / (2 * i) != (j + f + l) / (2 * i)) || j + l + f >= n) break;

					elemArr[k].x = (j + f)*q;
					elemArr[k].y = (j + f + l)*q;
					//cout << '[' << j + f << ',' << j + f + l << "] ";
					hThreads[k] = CreateThread(NULL, 0, MyThreadFunction_2, (LPVOID)(elemArr + k), 0, 0);
					f++; k++;

				}
			}
			//cout << endl;
			WaitForMultipleObjects(k, hThreads, TRUE, INFINITE);
			for (int i = 0; i < k; i++)
			{
				CloseHandle(hThreads[i]);
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		cout << A[i] << ' ';
	}

	delete[] hThreads, A;
	return 0;
}

DWORD  WINAPI MyThreadFunction(LPVOID lpParam)
{
	Str *elemArr = (Str*)lpParam;

	int pos_1 = elemArr->x;
	int pos_2 = elemArr->y;

	if (n>pos_1+q) {
		sort(A + pos_1, A + pos_1 + q);
		//if(pos_2+q<n)
		sort(A + pos_2, A + pos_2 + q - q*s + n); 
		//else sort(A + pos_2, A + n);
	}
	else sort(A + pos_1, A + n);
	
	return 0;
}

DWORD  WINAPI MyThreadFunction_2(LPVOID lpParam)
{
	Str *S = (Str*)lpParam;

	int pos_1 = S->x;
	int pos_2 = S->y;

	int len;
	if (pos_2 + q > n) len = n - pos_2; else len = 2 * q;

	int *sortArr = new int[q + len];
	int j = pos_2, i = pos_1, k = 0;

	while (i < pos_1 + q && j < pos_2+len)
	{
		if (A[i] < A[j]) sortArr[k++] = A[i++];
		else sortArr[k++] = A[j++];
	}

			while (i < pos_1 + q)
		    {
				sortArr[k++] = A[i++];
		    }
	
			while (j < pos_2 + len)
			{
				sortArr[k++] = A[j++];
			}
		
			for (int I = 0; I < q; I++)
				A[pos_1 + I] = sortArr[I];

			for (int J = 0; J < q - len; J++)
				A[pos_2 + J] = sortArr[q + J];

			delete[]sortArr;
			return 0;
}

