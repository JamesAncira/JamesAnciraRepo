#pragma once
#include <stdlib.h>
#include <assert.h>

#pragma warning(disable:4996)

namespace Stack_Char
{
	struct Stack_Str
	{
		char **data;
		int n_data;
		int size;
	};

	void resize(Stack_Str *c)
	{
		c->size += c->size >> 1;
		c->data = (char**)realloc(c->data, sizeof(char*)*c->size); assert(c->data);
	}

	void init(Stack_Str *c, int initial_size)
	{
		assert(initial_size != 0);
		c->n_data = 0;
		c->size = initial_size;
		c->data = (char**)malloc(sizeof(char*)*c->size); assert(c->data);
	}

	void push(Stack_Str *c, const char *val)
	{
		if (c->n_data >= c->size) resize(c);
		c->data[c->n_data++] = (char*) val;
	}

	char *split(Stack_Str *c, int nmers)
	{
		return c->data[c->n_data];
	}

	char *pop(Stack_Str *c)
	{
		return c->data[--c->n_data];
	}

	void clear(Stack_Str *c)
	{
		c->n_data = 0;
	}

	void cleanup(Stack_Str *c)
	{
		free(c->data);
		*c = {};
	}
}