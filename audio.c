#include <SDL2/SDL.h>

typedef struct {

    Uint32 length;
	Uint32 position;
	Uint8 *data;

} Audio;

static char dummy[256];
static Audio *current_audio;

void audio_callback(void *userdata, Uint8 *stream, int requested_len) {
	
	Uint8 data[16384] = {0};

	if (current_audio) {

		// mix requested data (up to as much as is available) into data buffer
		int retrieved_len = current_audio->length - current_audio->position;
		
		if (requested_len < retrieved_len)
			retrieved_len = requested_len;

		for (int i=0; i<retrieved_len; i++) {
			data[i] += (current_audio->data + current_audio->position)[i];
		}
		
		current_audio->position += retrieved_len;

		// if there is no more data left for this audio, remove it from future callbacks
		if (current_audio->position == current_audio->length) {
			current_audio = NULL;
		}
	}

	// copy the requested length of data of the audio buffer into the requesting audio buffer (stream)
	SDL_memcpy(stream, data, requested_len);
}

Audio *create_audio(const char *path) {

	Audio *audio = malloc(sizeof(Audio));

	if (SDL_LoadWAV(path, (SDL_AudioSpec *) dummy, &(audio->data), &(audio->length)) == NULL) {
		printf("Couldn't load audio: %s\n", SDL_GetError());
		exit(-1);
	}

	return audio;
}

void destroy_audio(Audio *audio) {

	SDL_FreeWAV(audio->data);
	free(audio);
}

void play_audio(Audio *audio) {

	audio->position = 0;
	current_audio = audio;
}