#pragma once
#include "vec_int_data.h"
#include "vec_int_core.h"

namespace Vector_Int_Ext
{
	void Bubble_Sort(Vector_Int::Vector *a)
	{
		for (int i = 0; i < a->n_data; i++)
		{
			for (int k = i + 1; k < a->n_data; k++)
			{
				int temp;
				if (a->data[i] > a->data[k])
				{
					temp = a->data[i];
					a->data[i] = a->data[k];
					a->data[k] = temp;
				}
			}
		}
	}
}
