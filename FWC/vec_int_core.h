#pragma once
#include "vec_int_data.h"
#include "Windows.h"
#include <assert.h>
#include <stdio.h>
#define TYPE int

namespace Vector_Int_Core
{
	void init(Vector_Int::Vector *a, TYPE array_size);
	void push(Vector_Int::Vector *a, TYPE value);
	TYPE pop(Vector_Int::Vector *a);
	void clean(Vector_Int::Vector *a);
}

namespace Vector_Int_Core
{
	namespace Internals
	{
		void resize(Vector_Int::Vector *a)
		{
			a->array_size += a->array_size >> 1;
			a->data = (TYPE*)realloc(a->data, a->array_size * sizeof(TYPE));
			assert(a->data);
		}
	}

	void init(Vector_Int::Vector *a, TYPE array_size)
	{
		a->n_data = 0;
		a->array_size = array_size;
		a->data = (TYPE*)malloc(array_size * sizeof(TYPE));
		assert(a->data);
	}

	void push(Vector_Int::Vector *a, TYPE value)
	{
		if (a->n_data >= a->array_size) Vector_Int_Core::Internals::resize(a);
		a->data[a->n_data++] = value;
	}

	TYPE pop(Vector_Int::Vector *a)
	{
		if (a->n_data == 0) return 0;
		a->n_data--;
		return a->data[a->n_data];
	}

	void clean(Vector_Int::Vector *a)
	{
		if (a->data != NULL) free(a->data);
		*a = {};//zero out everything
	}
}