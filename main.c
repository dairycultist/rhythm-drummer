// gcc main.c -o build -lSDL2_image $(sdl2-config --cflags --libs)
// https://www.beepbox.co/#9n31s0k0l00e08t2-a7g0bj07r1i0o432T7v1u26f21842uaq011d07H_-CSQBKRKRJJJJh0IbE0T1v1u18f0q00d23A0F0B0Q0000Pf600E1617T0v1u59f10r6qgM10de3a0d23w5h0E1c04T2v1u15f10w4qw02d03w0E0b0000000000000000000000008Och4g00p1X0000FEPxvgnM9HFB9EOaaad2yGCyyCCyyCCyyCDgEFFEEFFE5F5F5E5F8M0
// base the falling of the notes on how far along the audio stream is

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.c"
#include "audio.c"
#include "logic.c"

int main() {

	printf("Starting Rhythm Drummer\n");

	// init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0 || SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	// init everything else
	window_init();
	audio_init();
	logic_init();
	logic_resize_window(400, 600);

	// process events until window is closed
	SDL_Event event;
	int running = 1;
	unsigned long time = 0; // wraps to 0 at around 4.5 years

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = 0;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				logic_resize_window(event.window.data1, event.window.data2);

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				logic_keychange(event.key.keysym.scancode, event.key.state == SDL_PRESSED);
			}
		}

		// clear window to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// logic/rendering
		logic_process(time++);

		// present rendered content to screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / 60);
	}

	logic_exit();
	audio_exit();
	window_exit();
	SDL_Quit();

	return 0;
}