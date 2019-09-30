#include "SDL2-2.0.9\include\SDL.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")
/*	Written by James Ancira Jr /w Levent's help  3/11/2019 */

int audio_buffer[2048];
int queue_needs_data = 1;
void _callback(void *user, unsigned char *stream, int len)
{
	queue_needs_data = 1;
	memcpy(stream, audio_buffer, len);
}

unsigned int init()
{
	SDL_AudioSpec spec;
	spec.callback = _callback;
	spec.userdata = NULL;
	spec.channels = 2;
	spec.freq = 44100;
	spec.padding = 0;
	spec.silence = 0;
	spec.size = 0;
	spec.format = AUDIO_S16SYS;
	spec.samples = 2048;

	SDL_AudioSpec resulting_spec = {};
	unsigned int device_id = SDL_OpenAudioDevice(NULL, 0, &spec, &resulting_spec, 0);
	SDL_PauseAudioDevice(device_id,0);

	return device_id;
}

struct Events
{
	int event_type[10];
	int n_events = 0;
};

//function prototypes
void transform(double *dest, unsigned char *src, int n_bytes);
void window_average(double *dest, int *n_windows, double *src, int n_data, int window_size);
void draw_double(int x, int y, int width, int height, const double *data, int n_data, SDL_Renderer *renderer);
void draw_audio(int x, int y, int width, int height, const double *data, int array_size, int index, SDL_Renderer *renderer);
void show_event_marker(int x, int y, int width, int height, int event_position, int array_size, SDL_Renderer *renderer);
void draw_event_markers(int x, int y, int width, int height, const int *data, int event_index, int array_size, SDL_Renderer *renderer);

#pragma warning(disable : 4996)
int main(int argc, char **argv)
{
	//variables
	int window_size = 1024;
	int n_draw = 800;
	int pause = 0;
	int index = 0;
	int mark_index = 0;
	int event_index = 0;
	int load_data = 0;

	//create a window
	int screen_width = 800;
	int screen_height = 600;
	SDL_Window *window = SDL_CreateWindow("Play Audio",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen_width,
		screen_height,
		SDL_WINDOW_SHOWN);

	//create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//read audio event file
	char songname[32];
	FILE *f = fopen("audio_event_data(load).txt", "r");

	//Event struct array
	int array_size_struct_events = 0;
	Events *array_struct_events = (Events*)malloc(array_size_struct_events * sizeof(Events));

	//file data array
	int array_size_file = 0;
	int *array_data_file = 0;

	if (f != NULL)
	{
		fscanf(f, "%s", songname);
		fscanf(f, "%d", &window_size);

		printf("Load event data? Yes: 1, No: 0\n");
		fscanf(stdin, "%d", &load_data);
		if (load_data == 1)
		{
			fscanf(f, "%d", &array_size_file);
			array_data_file = (int*)malloc(array_size_file * sizeof(int));
			array_size_struct_events = array_size_file;

			for (int i = 0; i < array_size_file; i++)
			{
				fscanf(f, "%d", &array_data_file[i]);
			}
		}
		printf("songname: %s\n", songname);
		printf("window_size: %d\n", window_size);
		printf("array_size_file: %d\n", array_size_file);
	}
	else
	{
		printf("File not found.");
	}
	fclose(f);

	//event type look up table
	array_struct_event[index].event_type[array_struct_event[index].n_events++] = 1;

	//init SDL_Audio
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 *wav_buffer;

	//load the WAV
	SDL_LoadWAV(songname, &wav_spec, &wav_buffer, &wav_length);

	int n_samples = wav_length / 4;
	printf("n_samples : %d\n", n_samples);

	//short array of the combined channels from a 16 bit stereo file (L&R)
	int array_size_short = wav_length / 2;
	short *array_data_short = (short*)malloc(array_size_short * sizeof(short));
	
	//the average of the L&R channels normalized
	int array_size_double = array_size_short / 2;
	double *array_data_double = (double*)malloc(array_size_double * sizeof(double));

	//the average of four elements at a time in double_data
	int array_size_window = array_size_double / window_size;
	double *array_data_window = (double*)malloc(array_size_window * sizeof(double));

	//event index array
	int array_size_event = array_size_window;
	int *array_data_event_index = (int*)malloc(array_size_event * sizeof(int));

	//read through wav file two bytes at a time
	array_data_short = (short*)wav_buffer;

	//given short_array, normalize values then take the average of the L&R channels, and give it to the double_data
	transform(array_data_double, (unsigned char*)array_data_short, array_size_short * sizeof(short));

	//average data from double_data & give it to window_data
	int n_windows = 0;
	window_average(array_data_window, &n_windows, array_data_double, array_size_double, window_size);

	printf("n_windows %d\n", n_windows);

	//variables for SDL loop
	int window_width = 800;
	int window_height = 200;
	int draw_position_x = 0;
	int draw_position_y = screen_height / 2;
	int draw_position_audio_x = 0;
	int draw_position_audio_y = 0;
	int counter = 0;
	unsigned int position_in_audio = 0;
	unsigned int device_id = init();
	int index_current_window = 0;

	//SDL loop
	bool quit = false;
	while (quit == false)
	{
		//get mouse position
		int mouse_pos_x = 0;
		int mouse_pos_y = 0;
		SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
		mark_index = 0;

		//get index under mouse_pos_x
		double mark_pos = (double)(mouse_pos_x - draw_position_audio_x) / window_width;
		int event_pos = mark_pos * array_size_window;

		//loop consumes all events coming to  the window & if not looped events will queue up and we require time to draw & update
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case 'p':
					pause = !pause;
					if (pause > 0) printf("Paused audio\n");
					if (pause == 0) printf("Unpaused audio\n");
					SDL_PauseAudioDevice(device_id, pause);
					break;

				case 'r':
					position_in_audio = 0;
					printf("Restart\n");
					break;

				case 'a':
					position_in_audio -= 2048;
					break;

				case 'd':
					position_in_audio += 2048;
					break;

				case 'q':
					draw_position_audio_x += 20;
					break;

				case 'e':
					draw_position_audio_x -= 20;
					break;

				default:
					break;
				}//end of keydown switch
				break;

			case SDL_MOUSEBUTTONDOWN:
				mark_index = 1;
				break;

			default:
				break;
			}//end of event switch
		}//end of event loop

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		//restart song if we reach the end
		if (position_in_audio >= n_samples) position_in_audio = 0;

		//copy from song to audio buffer
		SDL_LockAudioDevice(device_id);
		if (queue_needs_data == 1 && pause == 0)
		{
			queue_needs_data = 0;
			memcpy(audio_buffer, &wav_buffer[position_in_audio * sizeof(int)], 2048*sizeof(int));
			position_in_audio += 2048;
		}
		SDL_UnlockAudioDevice(device_id);

		//index in the array_data_window
		index_current_window = position_in_audio / window_size;
		
		//scrolling, draw a small amount of the array, array[index] and amount of items to draw, draw window data
		if (index_current_window <= array_size_window)
		{
			//draw audio data
			draw_double(draw_position_x, draw_position_y, window_width, window_height, &array_data_window[index_current_window], n_draw, renderer);
			
			//draw audio length & position in audio
			draw_audio(draw_position_audio_x, draw_position_audio_y, window_width, window_height, array_data_window, array_size_window, index_current_window, renderer);
		}
		
		//ignore if mouse is outide of the window area
		if (mouse_pos_x > draw_position_audio_x && mouse_pos_x <= window_width + draw_position_audio_x)
		{
			//show mouse position with marker
			show_event_marker(draw_position_audio_x, draw_position_audio_y, window_width, window_height, event_pos, array_size_window, renderer);

			if (mark_index)
			{
				//mark event position
				array_data_event_index[event_index++] = event_pos;
				printf("%d %d\n", array_data_event_index[event_index - 1], event_index);
			}
		}

		//draw event markers from array_data_file
		if (load_data)
		{
			draw_event_markers(draw_position_audio_x, draw_position_audio_y, window_width, window_height, array_data_file, array_size_file, array_size_event, renderer);
		}

		//draw event markers
		draw_event_markers(draw_position_audio_x, draw_position_audio_y, window_width, window_height, array_data_event_index, event_index, array_size_event, renderer);
		
		SDL_RenderPresent(renderer);
	}//end of SDL loop

	//write data to file
	fopen("audio_event_data(new).txt", "w");
	fprintf(f, "%s\n%d\n%d\n", &songname, window_size, event_index);
	for (int i = 0; i < event_index; i++)
	{
		fprintf(f, "%d\n", array_data_event_index[i]);
	}
	fclose(f);

	//clean up
	SDL_FreeWAV(wav_buffer);
	SDL_Quit();
	return 0;
}

void transform(double *dest, unsigned char *src, int n_bytes)
{
	int n_samples = n_bytes / 4;
	int k = 0;
	short *data = (short*)src;

	for (int i = 0; i < n_samples; i++)
	{
		double data_left = data[k++];
		double data_right = data[k++];

		if (data_left < 0)
		{
			data_left /= SHRT_MIN;
		}
		else
		{
			data_left /= SHRT_MAX;
		}

		if (data_right < 0)
		{
			data_right /= SHRT_MIN;
		}
		else
		{
			data_right /= SHRT_MAX;
		}
		dest[i] = 0.5 * (data_left + data_right);
	}
}

void window_average(double *dest, int *n_windows, double *src, int n_data, int window_size)
{
	//take a number of samples & average them out, reduce to one number
	int k = 0;
	for (int i = 0; i <= n_data - window_size; i+= window_size)
	{
		double sum = 0;
		for (int j = 0; j < window_size; j++)
		{
			//go through number of elements (window_size) in source (array_data_double)
			sum += src[i + j];
		}
		//add them up, then divide them by n_windows
		sum /= window_size;

		//assign data to dest (array_data_window)
		dest[k++] = sum;
	}

	//create windows
	*n_windows = k;
}

void draw_double(int x, int y, int width, int height, const double *data, int n_draw, SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (int i = 0; i < n_draw; i++)
	{
		SDL_Rect r;
		r.x = x+(float)width*i/n_draw;
		r.y = y;
		r.w = 1;
		r.h = data[i] * height;
		SDL_RenderFillRect(renderer, &r);
	}
}

void draw_audio(int x, int y, int width, int height, const double *data, int array_size, int index, SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	for (int i = 0; i < array_size; i++)
	{
		SDL_Rect r;
		r.x = x+(float)width*i/array_size;
		r.y = y;
		r.w = 1;
		r.h = data[i] * height;
		SDL_RenderFillRect(renderer, &r);
	}

	//beginning of audio
	SDL_Rect r1;
	r1.x = x;
	r1.y = y;
	r1.w = 6;
	r1.h = 12;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &r1);

	//end of audio
	SDL_Rect r2;
	r2.x = x + width;
	r2.y = y;
	r2.w = 6;
	r2.h = 12;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &r2);

	//index_current_window, audio currently playing
	SDL_Rect r3;
	r3.x = x + (float)width*index/array_size;
	r3.y = y;
	r3.w = 6;
	r3.h = data[index] * height;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &r3);
}

void show_event_marker(int x, int y, int width, int height, int event_position, int array_size, SDL_Renderer *renderer)
{
	//draw event marker
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect r;
	r.x = x + (float)width*event_position / array_size;
	r.y = y;
	r.w = 3;
	r.h = 12;
	SDL_RenderFillRect(renderer, &r);
}

void draw_event_markers(int x, int y, int width, int height, const int *data, int event_index, int array_size, SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i < event_index; i++)
	{
		SDL_Rect r;
		r.x = x + (float)width*data[i] / array_size;
		r.y = y;
		r.w = 2;
		r.h = 6;
		SDL_RenderFillRect(renderer, &r);
	}
}