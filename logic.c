static Texture *note_red;
static Texture *note_blue;
static Texture *drum;

static Audio *rim;

void logic_init() {

    note_red = load_texture("note_red.png");
    note_blue = load_texture("note_blue.png");
    drum = load_texture("drum.png");

    rim = create_audio("rim.wav");
}

void logic_process(unsigned long time) {

    float beat = time * 0.05;
    float beat_dec = beat - (int) beat;

    for (int i=0; i<4; i++) {

        int c = (int) ((beat_dec + i) * 20);

        draw_rect(c, c, c, 0, (int) (beat_dec * 100 + i * 100), 128, 3);
    }

    draw_texture(drum, 0, 400);

    draw_texture(note_blue, 0, (int) (beat * 100));
    draw_texture(note_red, 0, (int) (beat * 100));
}

void logic_resize_window(int w, int h) {

}

void logic_keychange(int scancode, int is_pressed) {

    if (scancode == SDL_SCANCODE_Z && is_pressed) {
        play_audio(rim);
    }
}

void logic_exit() {

    // destroy_audio(rim);
}