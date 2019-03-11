#pragma once
#include "SDL2-2.0.4\include\SDL.h"
#include "node.h"
#include "vec_int_core.h"
#include <time.h>
#include <math.h>
#pragma comment(lib,"SDL2-2.0.4\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.4\\lib\\x86\\SDL2main.lib")

namespace SA
{
	void create_Random_Nodes(Nodes::Node *n, int array_size, int screen_w, int screen_h)
	{
		for (int i = 0; i < array_size; i++)
		{
			n[i].x = rand() % screen_w;
			n[i].y = rand() % screen_h;
		}
	}

	void retrieve_Nodes(Nodes::Node *n, int array_size, int screen_w, int screen_h)
	{
		SA::create_Random_Nodes(n, array_size, screen_w, screen_h);
	}

	void generate_Path_From_Nodes(Nodes::Node *n, Vector_Int::Vector *v, Vector_Int::Vector *tmp_v, int array_size, int index_begin, int index_end)
	{
		v->n_data = 0;
		tmp_v->n_data = 0;
		for (int i = 0; i < array_size; i++)
		{
			Vector_Int_Core::push(v, i);
			Vector_Int_Core::push(tmp_v, i);
		}
		
		int shuffle_range = array_size - 1;
		for (int i = 0; i < shuffle_range; i++)
		{
			//between [i] && [8]
			int index = i + rand() % (shuffle_range - i + 1);
			int index_temp = v->data[index];
			v->data[index] = v->data[i];
			v->data[i] = index_temp;
		}

		//check for begin & end index
		for (int i = 0; i < array_size; i++)
		{
			if (v->data[i] == index_begin)
			{
				int tmp = v->data[0];
				v->data[0] = v->data[i];
				v->data[i] = tmp;
			}

			if (v->data[i] == index_end)
			{
				int tmp = v->data[array_size - 1];
				v->data[array_size - 1] = v->data[i];
				v->data[i] = tmp;
			}
		}
	}

	void permute_Solution(Vector_Int::Vector *v, int array_size)
	{
		//between index v->data[0] & v->data[array_size - 2]
		int index_a = 1 + rand() % (array_size - 2);
		int index_b = 1 + rand() % (array_size - 2);

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

	double simulated_Annealing(Nodes::Node *n, Vector_Int::Vector *v, Vector_Int::Vector *tmp_v, int array_size, double temperature, FILE *f)
	{
		//LA: save the original solution before permuting it
		//When using memcpy(), pass the data array address instead of the struct address
		memcpy(tmp_v->data, v->data, sizeof(int)*array_size);
		permute_Solution(tmp_v, array_size);
		double solution_current = SA::distance(n, v, array_size);
		double solution_new = SA::distance(n, tmp_v, array_size);

		fprintf(f,"%f\t%f\n", solution_current, solution_new);

		if (solution_new <= solution_current)
		{
			memcpy(v->data, tmp_v->data, sizeof(int)*array_size);
			return solution_new;
		}

		//if the permuted solution does not work out, you dont do do anything
		double pow_e = exp((solution_current - solution_new) / temperature);
		double prob = (double)rand() / RAND_MAX;

		//if it works out, you copy permuted solution to original
		if (prob < pow_e)
		{
			memcpy(v->data, tmp_v->data, sizeof(int)*array_size);
			return solution_new;
		}

		return solution_current;
	}

	void draw_Node(SDL_Renderer *renderer, Nodes::Node *n, Nodes::Node *n_begin, Nodes::Node *n_end)
	{
		SDL_Rect r;
		r.x = n->x;
		r.y = n->y;
		r.w = 10;
		r.h = 10;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &r);

		SDL_Rect r_index_begin;
		r_index_begin.x = n_begin->x;
		r_index_begin.y = n_begin->y;
		r_index_begin.w = 10;
		r_index_begin.h = 10;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &r_index_begin);

		SDL_Rect r_index_end;
		r_index_end.x = n_end->x;
		r_index_end.y = n_end->y;
		r_index_end.w = 10;
		r_index_end.h = 10;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &r_index_end);
	}

	void draw_Path(SDL_Renderer *renderer, Nodes::Node *n, Vector_Int::Vector *v, int array_size)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		for (int i = 0; i < array_size - 1; i++)
		{
			SDL_RenderDrawLine(renderer, n[v->data[i]].x, n[v->data[i]].y, n[v->data[i + 1]].x, n[v->data[i + 1]].y);
		}
	}
}
