#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "gui.h"
#include "fileio.h"

ALLEGRO_DISPLAY *display;
ALLEGRO_FONT *font;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

int init_gui() {
    if (!al_init()) return 0;
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    display = al_create_display(640, 480);
    font = al_load_ttf_font("arial.ttf", 24, 0);
    queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 30);

    if (!display || !font || !queue || !timer) return 0;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    return 1;
}

void shutdown_gui() {
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
}

void draw_board(GameState *state, int selector) {
    al_clear_to_color(al_map_rgb(0, 0, 50));
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            ALLEGRO_COLOR color = al_map_rgb(200, 200, 200);
            if (state->board[r][c] == 1) color = al_map_rgb(255, 0, 0);
            else if (state->board[r][c] == 2) color = al_map_rgb(255, 255, 0);
            al_draw_filled_circle(60 + c*60, 60 + r*60, 25, color);
        }
    }
    al_draw_textf(font, al_map_rgb(255,255,255), 320, 10, ALLEGRO_ALIGN_CENTER, "Player %d's turn", state->current_player);
    al_draw_rectangle(35 + selector*60, 35, 85 + selector*60, 445, al_map_rgb(255,255,255), 3);
    al_flip_display();
}

void game_loop() {
    GameState state;
    init_game(&state);
    int selector = 0;
    int running = 1;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = 0;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT && selector > 0) selector--;
            if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT && selector < COLUMNS - 1) selector++;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                int row = drop_token(&state, selector);
                if (row != -1) {
                    int win = check_winner(&state);
                    if (win) {
                        state.winner = win;
                        draw_board(&state, selector);
                        save_stats(win);
                        al_rest(2);
                        init_game(&state);
                    } else {
                        switch_player(&state);
                    }
                }
            }
        }

        draw_board(&state, selector);
    }

    save_game(&state, "savegame.dat");
}
