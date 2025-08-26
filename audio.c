#include <SDL2/SDL.h>

typedef struct {

    Uint32 length;
	Uint32 position;
	Uint8 *data; // SDL_FreeWAV(data);

} Audio;

static char dummy[256];
static Audio *current_audio;

void audio_callback(void *userdata, Uint8 *stream, int requested_len) {
	
	Uint8 data[16384 * 4] = {0};

	if (current_audio) {

		int retrieved_len = current_audio->length - current_audio->position;
		
		if (requested_len < retrieved_len)
			retrieved_len = requested_len;

		memcpy(data, current_audio->data + current_audio->position, retrieved_len);
		
		current_audio->position += retrieved_len;

		if (current_audio->position == current_audio->length) {
			current_audio = NULL;
		}
	}

	// copy the requested length of data of the audio buffer into the requesting audio buffer (stream)
	SDL_memcpy(stream, data, requested_len);
}

void init_audio(const char *path) { // needs to take an example file to determine audio format
	
	SDL_AudioSpec wav_spec;

	if(SDL_LoadWAV(path, &wav_spec, (Uint8 **) dummy, (Uint32 *) dummy) == NULL) {
		printf("Couldn't load audio: %s\n", SDL_GetError());
		exit(-1);
	}
	wav_spec.callback = audio_callback;
	wav_spec.userdata = NULL;
	
	if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
		printf("Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	
	SDL_PauseAudio(0);
}

void play_audio(const char *path) {

	Audio *audio = malloc(sizeof(Audio));

	if (SDL_LoadWAV(path, (SDL_AudioSpec *) dummy, &(audio->data), &(audio->length)) == NULL) {
		printf("Couldn't load audio: %s\n", SDL_GetError());
		exit(-1);
	}

	audio->position = 0;

	current_audio = audio;
}

int main(int argc, char* argv[]) {

	// initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	init_audio("rim.wav");
	play_audio("rim.wav");

	SDL_Delay(3000);
	SDL_CloseAudio();
}