#include "Fast_Windows_Console.h"
#include "min_heap_int_core.h"
#include <time.h>
#include <iostream>
using namespace std;
#pragma warning(disable:4996)

int main()
{
	srand(time(0));
	int array_size = 20;
	Min_Heap_Int::Heap h;
	Min_Heap_Int_Core::init(&h, array_size);
	
	for (int i = 0; i < array_size; i++)
	{
		Min_Heap_Int_Core::add(&h, rand() % 100);
	}
	
	for (int i = 0; i < array_size; i++)
	{
		cout << h.data[i] << endl;
	}

	cout << endl;

	for (int i = 0; i < array_size; i++)
	{
		int temp = Min_Heap_Int_Core::remove(&h);
		cout << temp << endl;
	}

	Min_Heap_Int_Core::clean(&h);
	getchar();
}