#include "SDL2-2.0.4\include\SDL.h"
#include "vec_int_core.h"
#include <time.h>
#pragma comment(lib,"SDL2-2.0.4\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.4\\lib\\x86\\SDL2main.lib")

namespace Nodes
{
	struct Node
	{
		int n_nodes;
		int x;
		int y;
	};
}

namespace Simulation
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;

	void init()
	{
		SDL_Init(SDL_INIT_VIDEO);

		SDL_Window *window = SDL_CreateWindow("James Ancira Final(2018)",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			screen_width,
			screen_height,
			SDL_WINDOW_SHOWN);

		renderer = SDL_CreateRenderer(window,
			-1, SDL_RENDERER_ACCELERATED);

		/*

		YOUR INIT CODE

		*/
	}

	void create_Random_Nodes(Nodes::Node *n, int array_size, int screen_w, int screen_h)
	{
		for (int i = 0; i < array_size; i++)
		{
			n[i].x = rand() % screen_w;
			n[i].y = rand() % screen_h;
		}
		n->n_nodes = array_size;
	}

	void retrieve_Nodes(Nodes::Node *n, int array_size, int screen_w, int screen_h)
	{
		Simulation::create_Random_Nodes(n, array_size, screen_w, screen_h);
	}

	void current_Solution(Nodes::Node *n, Vector_Int::Vector *v, int array_size)
	{
		int n_nodes = array_size - 1;

		for (int i = 0; i < array_size; i++)
		{
			Vector_Int_Core::push(v, i);
		}

		for (int i = 0; i < n_nodes; i++)
		{
			//between [i] && [8]
			int index = i + rand() % (n_nodes - i + 1);
			int index_temp = v->data[index];
			v->data[index] = v->data[i];
			v->data[i] = index_temp;
		}

		for (int i = 0; i < array_size; i++)
		{
			if (v->data[0] == 0)
			{
				int tmp = v->data[0];
				v->data[0] = v->data[i];
				v->data[i] = tmp;
				break;
			}
		}
	}

	void permute_Solution(Vector_Int::Vector *v, int array_size)
	{
		//between index v->data[0] & v->data[array_size - 1]
		int index_a = 1 + rand() % (array_size - 1);
		int index_b = 1 + rand() % (array_size - 1);

		int temp = v->data[index_a];
		v->data[index_a] = v->data[index_b];
		v->data[index_b] = temp;
	}

	double distance(Nodes::Node *n, Vector_Int::Vector *v, int array_size)
	{
		double sum = 0.0;
		//refer to the node index with n[v->data[i]]; 
		for (int i = 1; i < array_size; i++)
		{
			double dx = n[v->data[i]].x - n[v->data[i - 1]].x;
			double dy = n[v->data[i]].y - n[v->data[i - 1]].y;
			double distance = dx * dx + dy * dy;
			sum += distance;
		}
		return sum;
	}

	double simulated_Annealing(Nodes::Node *n, Vector_Int::Vector *v, int array_size, double temperature)
	{
		double current_cost = Simulation::distance(n, v, array_size);
		return current_cost;
	}

	void update()
	{
		//consume all window events, key state gets updated here automatically
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit(0);
			}
		}

		/*

		YOUR UPDATE CODE

		*/

	}

	void draw(Nodes::Node *n)
	{
		//set color to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(renderer);

		/*
		YOUR DRAW CODE
		*/

		SDL_Rect r;
		r.x = n->x;
		r.y = n->y;
		r.w = 10;
		r.h = 10;
		SDL_RenderFillRect(renderer, &r);

		for (int i = 0; i < n->n_nodes - 1; i++)
		{
			SDL_RenderDrawLine(renderer, n[i].x, n[i].y, n[i + 1].x, n[i + 1].y);
		}
		//flip buffers
		SDL_RenderPresent(renderer);
	}

}
int main(int argc, char **argv)
{


	Simulation::init();


	for (;;)
	{
		Simulation::update();

		Simulation::draw();
	}


	return 0;
}