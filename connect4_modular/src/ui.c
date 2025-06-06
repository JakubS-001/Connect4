
#include "../include/ui.h"
#include "../include/game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

static int selected_col = 0;

void draw_board(ALLEGRO_FONT* font, int hover_col) {
    al_clear_to_color(al_map_rgb(15, 27, 39));
    
    if (hover_col >= 0 && hover_col < COLS) {
        al_draw_filled_rectangle(hover_col * CELL_SIZE, CELL_SIZE, (hover_col + 1) * CELL_SIZE, SCREEN_HEIGHT, al_map_rgba(255, 255, 255, 30));
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int cx = c * CELL_SIZE + CELL_SIZE / 2;
            int cy = (r + 1) * CELL_SIZE + CELL_SIZE / 2;
            ALLEGRO_COLOR color = al_map_rgb(39, 56, 72);
            if (board[r][c] == PLAYER1)
                color = al_map_rgb(227, 66, 52);
            else if (board[r][c] == PLAYER2)
                color = al_map_rgb(255, 215, 0);
            al_draw_filled_circle(cx, cy, CELL_SIZE / 2 - 5, color);
        }
    }

    char score_text[64];
    sprintf(score_text, "Gracz 1 (Czerwony): %d     Gracz 2 (Zolty): %d", score1, score2);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, score_text);

    if (game_over) {
        const char* win_text = current_player == PLAYER1 ? "Gracz 1 wygrywa!" : "Gracz 2 wygrywa!";
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 70, ALLEGRO_ALIGN_CENTER, win_text);
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, 130, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zrestartowac");
    }
}

void start_game() {
    if (!al_init()) {
        fprintf(stderr, "Nie mozna zainicjowac Allegro!\n");
        return;
    }

    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Blad: nie udalo sie utworzyc okna (display)\n");
        return;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Blad: nie udalo sie utworzyc kolejki zdarzen\n");
        al_destroy_display(display);
        return;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Blad: nie udalo sie utworzyc timera\n");
        al_destroy_display(display);
        al_destroy_event_queue(queue);
        return;
    }

    ALLEGRO_FONT* font = al_load_ttf_font("/System/Library/Fonts/Supplemental/Arial.ttf", 48, 0);
    if (!font) {
        fprintf(stderr, "Blad: nie udalo sie zaladowac czcionki Arial.ttf\n");
        al_destroy_display(display);
        al_destroy_event_queue(queue);
        al_destroy_timer(timer);
        return;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    printf("START GAME\n");

    bool running = true;
    bool redraw = true;
    reset_board();
    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !game_over) {
            int col = ev.mouse.x / CELL_SIZE;
            if (place_piece(col)) {
                if (check_winner(current_player)) {
                    game_over = true;
                    if (current_player == PLAYER1) score1++;
                    else score2++;
                } else {
                    current_player = (current_player == PLAYER1) ? PLAYER2 : PLAYER1;
                }
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
                reset_board();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                selected_col = (selected_col - 1 + COLS) % COLS;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                selected_col = (selected_col + 1) % COLS;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                if (place_piece(selected_col)) {
                    if (check_winner(current_player)) {
                        game_over = true;
                        if (current_player == PLAYER1) score1++;
                        else score2++;
                    } else {
                        current_player = (current_player == PLAYER1) ? PLAYER2 : PLAYER1;
                    }
                }
            }
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            draw_board(font, selected_col);
            al_flip_display();
            redraw = false;
        }
    }

    printf("GAME LOOP EXITED\n");

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(font);
    al_destroy_timer(timer);
}
