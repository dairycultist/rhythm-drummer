// gcc main.c -o build -lSDL2_image $(sdl2-config --cflags --libs)
// https://www.beepbox.co/#9n31s0k0l00e08t2-a7g0bj07r1i0o432T7v1u26f21842uaq011d07H_-CSQBKRKRJJJJh0IbE0T1v1u18f0q00d23A0F0B0Q0000Pf600E1617T0v1u59f10r6qgM10de3a0d23w5h0E1c04T2v1u15f10w4qw02d03w0E0b0000000000000000000000008Och4g00p1X0000FEPxvgnM9HFB9EOaaad2yGCyyCCyyCCyyCDgEFFEEFFE5F5F5E5F8M0

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include "logic.c"

#define TRUE 1
#define FALSE 0

typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Texture;

static SDL_Window *window;
static SDL_Renderer *renderer;

Texture *load_texture(const char *path) {

	Texture *tex = malloc(sizeof(Texture));

	tex->sdl_texture = IMG_LoadTexture(renderer, path);

	SDL_QueryTexture(tex->sdl_texture, NULL, NULL, &tex->w, &tex->h);

	return tex;
}

void draw_texture(Texture *tex, int x, int y) {

	SDL_Rect texture_rect = { x, y, tex->w, tex->h };
	SDL_RenderCopy(renderer, tex->sdl_texture, NULL, &texture_rect);
}

void free_texture(Texture *tex) {
	
	SDL_DestroyTexture(tex->sdl_texture);
	free(tex);
}

void draw_rect(int r, int g, int b, int x, int y, int w, int h) {

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderFillRect(renderer, &rect);
}

int main() {

	printf("Starting Rhythm Drummer\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Rhythm Drummer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 200, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	// init game stuff
	Texture *tex = load_texture("reference_also_remember_hit_has_fade_out_pulse_effect.png");

	// process events until window is closed
	SDL_Event event;
	char running = TRUE;
	unsigned long time = 0; // wraps to 0 at around 4.5 years

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = FALSE;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				// event.window.data1
				// event.window.data2

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				// if (event.key.keysym.scancode == SDL_SCANCODE_Z)
			}
		}

		// clear window to black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// logic/rendering
		time++;
		draw_texture(tex, 0, 0);
		draw_rect(255, 0, 0, 40, 40, 20, 10);

		// present rendered content to screen
		SDL_RenderPresent(renderer);

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}