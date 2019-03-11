#include "Fast_Windows_Console.h"
#include <time.h>
#pragma warning(disable:4996)

namespace Grid
{
	struct Grid
	{
		int **grid;
		unsigned int number_of_rows;
		unsigned int number_of_columns;
	};

	void init(Grid *g, unsigned int n_rows, unsigned int n_columns)
	{
		g->number_of_rows = n_rows;
		g->number_of_columns = n_columns;
		g->grid = new int *[g->number_of_rows];

		for (int i = 0; i < g->number_of_rows; i++)
		{
			g->grid[i] = new int[g->number_of_columns];
		}

		FWC::set_Color(0x80);
		FWC::horizontal_Line(0, 0, g->number_of_rows, '0');
		for (int i = 0; i < g->number_of_rows; i++)
		{
			FWC::vertical_Line(i, 0, g->number_of_columns, '0');
		}
	}
}

	namespace Walker
	{
		struct Walker
		{
			unsigned int x;
			unsigned int y;
		};

		void init(Walker *a, int x, int y)
		{
			a->x = x;
			a->y = y;
		}

		void updatePos(Walker *a, Grid::Grid *g)
		{
			int r = rand() % 4;
			FWC::set_Color(0xE0);

			switch (r)
			{
			case 0:
				if (a->x > 0) a->x--;
				FWC::horizontal_Line(a->x, a->y, 1, '1');
				break;
			case 1:
				if (a->x < g->number_of_rows - 1) a->x++;
				FWC::horizontal_Line(a->x, a->y, 1, '1');
				break;
			case 2:
				if (a->y > 0) a->y--;
				FWC::vertical_Line(a->x, a->y, 1, '1');
				break;
			case 3:
				if (a->y < g->number_of_columns - 1) a->y++;
				FWC::vertical_Line(a->x, a->y, 1, '1');
				break;
			}
		}
	}

namespace Vector
{
	struct Vector
	{
		int *data;
		int n_data;
		int array_size;
	};

	void init(Vector *a, int array_size)
	{
		a->n_data = 0;
		a->array_size = array_size;
		a->data = (int*)malloc(array_size * sizeof(int));
		assert(a->data);
	}

	void resize(Vector *a)
	{
		a->array_size += a->array_size / 2 + 1;
		a->data = (int*)realloc(a->data, a->array_size * sizeof(int));
		assert(a->data);
	}

	void add(Vector *a, int value)
	{
		//if (a->n_data >= a->array_size) resize(a);
		//if (a->n_data >= a->array_size) a->array_size++;

		a->data[a->n_data++] = value;
	}

	int pop(Vector *a)
	{
		if (a->n_data == 0) return 0;
		a->n_data--;
		//cout << "Pop " << a->data << endl;
		return a->data[a->n_data];
	}

	void clean(Vector *a)
	{
		if (a->data != NULL) free(a->data);
		*a = {};//zero out everything
	}

}

int main()
{
	
	FWC::init();
	FWC::clear();
	srand(time(0));
	Grid::Grid g;
	Grid::init(&g, 80, 25);
	Walker::Walker w;
	Walker::init(&w, 0, 0);
	
	for (;;)
	{
		Walker::updatePos(&w, &g);
		FWC::present();
	}
	return 0;
}