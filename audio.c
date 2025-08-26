#include <SDL2/SDL.h>

typedef struct {

    Uint32 length;
	Uint32 position;
	Uint8 *data; // SDL_FreeWAV(data);

} Audio;

char dummy[256];
Audio *current_audio;

void audio_callback(void *userdata, Uint8 *stream, int requested) {
	
	Uint8 data[2048];

	if (current_audio->position == current_audio->length) {
		return;
	}
	
	requested = requested > current_audio->length - current_audio->position ? current_audio->length - current_audio->position : requested;
	SDL_memcpy(stream, current_audio->data + current_audio->position, requested); // copy the requested length of data of the audio buffer into the requesting audio buffer (stream)
	
	current_audio->position += requested;
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

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	init_audio("rim.wav");
	play_audio("rim.wav");

	SDL_Delay(3000);
	SDL_CloseAudio();
}