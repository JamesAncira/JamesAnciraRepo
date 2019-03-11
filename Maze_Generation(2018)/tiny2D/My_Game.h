#pragma once

//Double check project version if things don't work.

#include "include/Engine_core.h"
#include "include/Light_core.h"
#include "include/Actor_core.h"
#include "include/Particle_core.h"
#include "vec_int_core.h"

namespace My_Game
{
	//create this function
	void init_Maze(Grid::Grid *g);
	//create this function
	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done);

	namespace Command
	{

	}

	namespace Assets
	{
		namespace Animation
		{

		}
	}

	namespace World
	{

		namespace Parameters
		{
			const int maze_n_rows = 80;
			const int maze_n_cols = 128;
			int step_frequency = 5;
		}

		Tileset::Tileset tileset;
		Font::Font text;
		Particle::Emitter emitter;
		Grid::Grid maze;
		Grid::Point current_maze_position = { 1, 1 };
		Grid_Camera::Grid_Camera camera;
		Vector_Int::Vector v;
		unsigned int last_step_time = 0;
	}

	void init(int screen_w, int screen_h)
	{
		//initialize all systems and open game window
		Engine::init("hello", screen_w, screen_h);
		Tileset::init(&World::tileset, "road_tileset.txt", Engine::renderer);
		Particle::init(&World::emitter, "dirt.txt", 512, Engine::renderer);
		Grid::init(&World::maze, World::Parameters::maze_n_rows, World::Parameters::maze_n_cols);
		Grid_Camera::init(&World::camera, Engine::screen_width, Engine::screen_height);
		World::camera.world_coord.x = 0;
		World::camera.world_coord.y = 0;
		World::camera.world_coord.w = World::Parameters::maze_n_cols;
		World::camera.world_coord.h = World::Parameters::maze_n_rows;
		Vector_Int_Core::init(&World::v, World::Parameters::maze_n_cols * World::Parameters::maze_n_rows);
	}

	void begin_Play(unsigned int current_time)
	{
		Font::set_Screen_Size(&World::text, 16, 16);
		init_Maze(&World::maze);
	}

	void update(unsigned int current_time, float dt)
	{
		Engine::event_Loop();
		if (current_time - World::last_step_time >= World::Parameters::step_frequency)
		{
			World::last_step_time = current_time;
			int done = 0;
			int dir = next_Maze_Step(&World::current_maze_position, &World::maze, done);
		}
	}

	void draw(unsigned int current_time)
	{
		SDL_RenderClear(Engine::renderer);
		Grid_Camera::calibrate(&World::camera);
		Tileset::draw_Grid(&World::tileset, &World::camera, &World::maze, Engine::renderer);
		Particle::draw(&World::emitter, &World::camera, current_time, Engine::renderer);
		//flip buffers
		SDL_RenderPresent(Engine::renderer);
	}

	void init_Maze(Grid::Grid *g)
	{
		//data[y*n_cols+x]
		for (int i = 0; i < g->n_rows; i++)
		{
			for (int j = 0; j < g->n_cols; j++)
			{
				g->data[i * g->n_cols + j] = 1;
			}
		}
	}

	int next_Maze_Step(Grid::Point *current_pos, Grid::Grid *g, int &done)
	{
		int _traveled = 2;
		int _backtrack = 3;
		int dir[4];
		int n_data = 0;

		if (current_pos->y > 2 && g->data[(current_pos->y - 2) *
			g->n_cols + current_pos->x] != _traveled)
		{
			dir[n_data++] = 0;
		}

		if (current_pos->x < g->n_cols - 2 && g->data[current_pos->y *
			g->n_cols + (current_pos->x + 2)] != _traveled)
		{
			dir[n_data++] = 1;
		}

		if (current_pos->y < g->n_rows - 2 && g->data[(current_pos->y + 2) *
			g->n_cols + current_pos->x] != _traveled)
		{
			dir[n_data++] = 2;
		}

		if (current_pos->x > 2 && g->data[current_pos->y *
			g->n_cols + (current_pos->x - 2)] != _traveled)
		{
			dir[n_data++] = 3;
		}

		if (n_data == 0) //instead of using a boolean, we check the n_data
		{
			if (World::v.n_data == 0)
			{
				return 2;  //should be finished
			}
			int previous_index = Vector_Int_Core::pop(&World::v);
			g->data[previous_index] = _backtrack; //pop the previous index, make it our current index and draw
			return previous_index;
		}

		int direction = dir[rand() % n_data]; //don't need to n_data - 1

		switch (direction)
		{
		default:
			break;
		case 0:
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled; //draw current index
			g->data[(current_pos->y - 1) * g->n_cols + current_pos->x] = _traveled; //draw the index between current index and the new index
			current_pos->y -= 2; //modify index
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled; //draw new index
			Vector_Int_Core::push(&World::v, current_pos->y * g->n_cols + current_pos->x); //push new index
			break;
		case 1:
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			g->data[current_pos->y * g->n_cols + (current_pos->x + 1)] = _traveled;
			current_pos->x += 2;
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			Vector_Int_Core::push(&World::v, current_pos->y * g->n_cols + current_pos->x);
			break;
		case 2:
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			g->data[(current_pos->y + 1) * g->n_cols + current_pos->x] = _traveled;
			current_pos->y += 2;
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			Vector_Int_Core::push(&World::v, current_pos->y * g->n_cols + current_pos->x);
			break;
		case 3:
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			g->data[current_pos->y * g->n_cols + (current_pos->x - 1)] = _traveled;
			current_pos->x -= 2;
			g->data[current_pos->y * g->n_cols + current_pos->x] = _traveled;
			Vector_Int_Core::push(&World::v, current_pos->y * g->n_cols + current_pos->x);
			break;
		}
		delete[] dir;
		//direction
		return 0;
	}
}