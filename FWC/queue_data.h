#pragma once
#define TYPE int

namespace Queue
{
	struct Queue
	{
		TYPE *data;
		int n_data;
		int array_size;
		int offset;
	};
}
