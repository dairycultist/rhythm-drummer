void logic_init() {

    // Texture *tex = load_texture("reference_also_remember_hit_has_fade_out_pulse_effect.png");
}

void logic_process(unsigned long time) {

    // draw_texture(tex, 0, 0);
    draw_rect(255, 0, 0, 40, 40 + (time % 100), 20, 10);
}

void logic_resize_window(int w, int h) {

}

void logic_keychange(int scancode, int is_pressed) {

}

void logic_exit() {
    
}