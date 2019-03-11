#pragma once
#include "min_heap_int_data.h"
#include <assert.h>
#include "Windows.h"
#include <iostream>
#define TYPE int
using namespace std;

namespace Min_Heap_Int_Core
{
	void init(Min_Heap_Int::Heap *h, int size);
	void add(Min_Heap_Int::Heap *h, int val);
	int remove(Min_Heap_Int::Heap *h);
	void clean(Min_Heap_Int::Heap *h);
}

namespace Min_Heap_Int_Core
{
	namespace Internals
	{
		void resize(Min_Heap_Int::Heap *h)
		{
			h->array_size += h->array_size >> 1;
			h->data = (int*)realloc(h->data, h->array_size * sizeof(int));
			assert(h->data);
		}
	}

	void init(Min_Heap_Int::Heap *h, int size)
	{
		h->n_data = 0;
		h->array_size = size;
		h->data = (int*)malloc(size * sizeof(int));
		assert(h->data);
	}

	void add(Min_Heap_Int::Heap *h, int val)
	{
		if (h->n_data >= h->array_size) Min_Heap_Int_Core::Internals::resize(h);
		int current_index = h->n_data;
		h->data[h->n_data++] = val;

		for (;;)
		{
			int parent = (current_index - 1) / 2;

			if (h->data[parent] > h->data[current_index])
			{
				swap(h->data[current_index], h->data[parent]);
				current_index = parent;
			}
			else
			{
				break;
			}
		}
	}

	int remove(Min_Heap_Int::Heap *h)
	{
		if (h->n_data == 0) return 0;
		int temp = h->data[0];
		cout << "Temp" << temp << endl;
		h->data[0] = h->data[h->n_data - 1];
		int current_index = 0;
		h->n_data--;

		for (;;)
		{
			int left_child_index = 2 * current_index + 1;
			int right_child_index = left_child_index + 1;
			int preferred_child_index;

			if (left_child_index > h->n_data || right_child_index > h->n_data)
			{
				break;
			}

			if (h->data[left_child_index] <= h->data[right_child_index])
			{
				preferred_child_index = left_child_index;
			}
			else
			{
				preferred_child_index = right_child_index;
			}

			if (h->data[current_index] <= h->data[preferred_child_index])
			{
				break;
			}
			swap(h->data[current_index], h->data[preferred_child_index]);
			current_index = preferred_child_index;
		}// end of for(;;)
		return temp;
	}

	void clean(Min_Heap_Int::Heap *h)
	{
		if (h->data != NULL) free(h->data);
		*h = {};
	}
}
