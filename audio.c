typedef struct {

    Uint32 length;
	Uint32 position;
	Uint8 *data;

} Audio;

static char dummy[256];
static Audio *active_audio[8] = {0};

static void audio_callback(void *userdata, Uint8 *stream, int requested_len) {
	
	Uint8 data[2048] = {0};

	for (int i=0; i<8; i++) {

		if (active_audio[i]) {

			// mix requested data (up to as much as is available) into data buffer
			int retrieved_len = active_audio[i]->length - active_audio[i]->position;
			
			if (requested_len < retrieved_len)
				retrieved_len = requested_len;

			for (int j=0; j<retrieved_len; j++) {
				data[j] += (active_audio[i]->data + active_audio[i]->position)[j];
			}
			
			active_audio[i]->position += retrieved_len;

			// if there is no more data left for this audio, remove it from future callbacks
			if (active_audio[i]->position == active_audio[i]->length) {
				active_audio[i] = NULL;
			}
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
	
	for (int i=0; i<8; i++) {

		if (!active_audio[i]) {

			active_audio[i] = audio;
			return;
		}
	}
}

void audio_init() {

	SDL_AudioSpec wav_spec;

	if(SDL_LoadWAV("rim.wav", &wav_spec, (Uint8 **) dummy, (Uint32 *) dummy) == NULL) { // needs to take an example file to determine audio format
		printf("Couldn't load audio: %s\n", SDL_GetError());
		exit(-1);
	}

	wav_spec.samples = 512; // lower samples = lower latency, more frequent audio callbacks with lesser requested length

	wav_spec.callback = audio_callback;
	wav_spec.userdata = NULL;
	
	if (SDL_OpenAudio(&wav_spec, NULL) < 0) {
		printf("Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	
	SDL_PauseAudio(0); // begin audio playback
}

void audio_exit() {

	SDL_CloseAudio();
}