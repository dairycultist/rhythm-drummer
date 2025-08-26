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

void destroy_texture(Texture *tex) {
	
	SDL_DestroyTexture(tex->sdl_texture);
	free(tex);
}

void draw_rect(int r, int g, int b, int x, int y, int w, int h) {

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderFillRect(renderer, &rect);
}

void window_init() {

	window = SDL_CreateWindow("Rhythm Drummer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 660, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		exit(-1);
    }

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		exit(-1);
	}
}

void window_exit() {
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}