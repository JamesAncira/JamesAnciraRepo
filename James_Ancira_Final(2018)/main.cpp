#include "simulated_annealing.h"
#include <time.h>

/*TO DO LIST
array_size
v.n_node
v.array_size
print these out
debug it
*/

#pragma warning(disable:4996)
int main(int argc, char **argv)
{
	//seed random
	srand(time(0));

	//variables
	int screen_width = 800;
	int screen_height = 600;
	double temperature = 1.0;
	double temperature_decay = 0.998;
	const int array_size = 13;
	int index_begin = rand() % array_size;
	int index_end = rand() % array_size;
	FILE *f = fopen("log.txt", "w+");

	//namespace::struct
	Vector_Int::Vector path_array;
	Vector_Int::Vector tmp_path_array;
	Vector_Int_Core::init(&path_array, array_size);
	Vector_Int_Core::init(&tmp_path_array, array_size);
	Nodes::Node node_array[array_size];

	//init SA
	SA::retrieve_Nodes(node_array, array_size, screen_width, screen_height);
	SA::generate_Path_From_Nodes(node_array, &path_array, &tmp_path_array, array_size, index_begin, index_end);

	//initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	//create a window
	SDL_Window *window = SDL_CreateWindow("James Ancira Final(2018)",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen_width,
		screen_height,
		SDL_WINDOW_SHOWN);

	//create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window,
		-1,
		SDL_RENDERER_ACCELERATED);

	//SDL loop
	bool state_SA = false;//update & draw
	bool state_Fancy = false;//fancy stuff & draw
	bool quit = false;

	while (quit == false)
	{
		SDL_Event event;

		//loop consumes all events coming to the window
		//if not looped, events will queue up and we require time to draw & update
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) quit = true;
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		
		//run simulated annealing
		if (temperature >= 0.000001)
		{
			double cost = 0.0;
			
			cost = SA::simulated_Annealing(node_array, &path_array, &tmp_path_array, array_size, temperature, f);
			temperature *= temperature_decay;
			printf("temperature = %f current_cost = %lf\n", temperature, cost);
		}
		else
		{
			//exit(0);
			//reset temperature, retrieve nodes with random pos, genertate new path
			temperature = 1.0;

			SA::retrieve_Nodes(node_array, array_size, screen_width, screen_height);

			//reset n_data to zero for both vectors
			path_array.n_data = 0;
			tmp_path_array.n_data = 0;

			SA::generate_Path_From_Nodes(node_array, &path_array, &tmp_path_array,array_size, index_begin, index_end);
		}

		//draw nodes
		for (int i = 0; i < array_size; i++)
		{
			SA::draw_Node(renderer, &node_array[i], &node_array[index_begin], &node_array[index_end]);
		}

		//draw path
		SA::draw_Path(renderer, node_array, &path_array, array_size);

		SDL_RenderPresent(renderer);
	}
	return 0;
}