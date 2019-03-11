#pragma once
#include <assert.h>
#include "Windows.h"
#include <stdio.h>
#include "queue_data.h"

namespace Queue_Core
{
	namespace Internals
	{
		void resize(Queue::Queue *a)
		{
			int new_size = a->array_size + (a->array_size >> 1);
			int *tmp = (int*)malloc(sizeof(int*) * new_size);
			assert(tmp);
			int n_elements_before_offset = (a->offset + a->n_data) % a->array_size;
			int n_elements_after_offet = a->n_data - n_elements_before_offset;
		}
	}

	void init(Queue::Queue *a, int array_size)
	{
		a->n_data = 0;
		a->offset = 0;
		a->array_size = array_size;
		a->data = (int*)malloc(array_size * sizeof(int));
		assert(a->data);
	}

	void enqueue(Queue::Queue *a, int value)
	{
		if (a->n_data >= a->array_size) Queue_Core::Internals::resize(a);
		a->data[((a->offset + a->n_data) % a->array_size)] = value;
		a->n_data++;
	}

	int dequeue(Queue::Queue *a)
	{
		if (a->n_data == 0) return 0;
		int old_offset = a->offset;
		a->offset = (a->offset + 1) % a->array_size;
		a->n_data--;
		return a->data[old_offset];
	}

	void clean(Queue::Queue *a)
	{
		if (a->data != NULL) free(a->data);
		*a = {};//zero out everything
	}
}
