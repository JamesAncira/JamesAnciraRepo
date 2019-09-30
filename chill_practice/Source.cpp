#include <stdio.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <assert.h>
using namespace std;

#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib, "SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")

//#define SPAWN_CONSOLE
#ifdef SPAWN_CONSOLE
#pragma comment(linker, "/subsystem:console")
#else
#pragma comment(linker, "/subsystem:windows")
#endif

struct Vec2D
{
	float x, y;
};

struct Pixel
{
	char r, g, b, a;
};

struct Rect
{
	int x, y, w, h;
	Pixel color;
};

struct Slider
{
	Rect range;
	Rect marker;
	float position;
};

//The button
struct Button
{
	SDL_Rect rect;
	int x;
	int y;
	int w;
	int h;
};

void HandleEvents(Button *rect, SDL_Event event)
{
	//get mouse position
	int mouse_pos_x = 0;
	int mouse_pos_y = 0;
	SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
	
	//If a mouse button was pressed
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		//If the left mouse button was pressed
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			//If the mouse is over the button
			if ((mouse_pos_x > rect->x) && (mouse_pos_x < rect->x + rect->w) && (mouse_pos_y > rect->y) && (mouse_pos_y < rect->y + rect->h))
			{
				printf("HandleEvents called Press");
			}
		}
	}

	//If a mouse button was released
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		//If the left mouse button was released
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			//If the mouse is over the button
			if ((mouse_pos_x > rect->x) && (mouse_pos_x < rect->x + rect->w) && (mouse_pos_y > rect->y) && (mouse_pos_y < rect->y + rect->h))
			{
				printf("HandleEvents called Press");
			}
		}
	}
}

void draw_Rect(Pixel* screen, int screen_width, int screen_height, const Rect* rect)
{
	for (int y = 0; y < rect->h; y++)
	{
		for (int x = 0; x < rect->w; x++)
		{
			int rect_index = y * rect->w + x;
			int screen_index = (y + rect->y) * screen_width + x + rect->x;

			screen[screen_index] = rect->color;
		}
	}
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	int window_width = 920;
	int window_height = 720;
	int window_area = window_width * window_height;

	SDL_Window *window = SDL_CreateWindow("Chillanium", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Button button;
	button.w = 10;
	button.h = 10;
	button.x = 30;
	button.y = 30;

	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_l = 0;
	int mouse_prev_l = 0;
	int selected_slider = -3;
	char button_pressed = 0;
	char save_button_pressed = -1;

	for (;;)
	{
		//get mouse position
		int mouse_pos_x = 0;
		int mouse_pos_y = 0;
		SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

		SDL_Event event;
		while (SDL_PollEvent(&event) == 1)
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
			if (event.type == SDL_QUIT) exit(0);
			else if (event.type == SDL_KEYDOWN)
			{
				char key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE)
				{
					exit(0);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				mouse_l = 1;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				mouse_l = 0;
			}
		}//end of while loop

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		HandleEvents(&button, event);

		SDL_RenderPresent(renderer);
	}

	return 0;
}