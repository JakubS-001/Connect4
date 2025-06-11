#include "../include/ui.h"
#include "../include/game.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

static int selected_col = 0;

typedef struct {
    int col;
    int target_row;
    float y;
    int player;
    bool active;
} FallingPiece;

static FallingPiece falling_piece = {0};

void draw_board(ALLEGRO_FONT* font, int hover_col) {
    al_clear_to_color(al_map_rgb(15, 27, 39)); //Background color
    
    if (hover_col >= 0 && hover_col < COLS) {
        al_draw_filled_rectangle(hover_col * CELL_SIZE, CELL_SIZE, (hover_col + 1) * CELL_SIZE, SCREEN_HEIGHT, al_map_rgba(76, 91, 106, 90));

    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int cx = c * CELL_SIZE + CELL_SIZE / 2;
            int cy = (r + 1) * CELL_SIZE + CELL_SIZE / 2;
            ALLEGRO_COLOR color = al_map_rgb(44, 62, 80); // Default cell color
            if (board[r][c] == PLAYER1)
                color = al_map_rgb(227, 66, 52);
            else if (board[r][c] == PLAYER2)
                color = al_map_rgb(255, 215, 0);
            al_draw_filled_circle(cx, cy, CELL_SIZE / 2 - 5, color);
            for (int i = 0; i < 4; i++) {
            if (winning_positions[i][0] == r && winning_positions[i][1] == c) {
                    al_draw_circle(cx, cy, CELL_SIZE / 2 - 3, al_map_rgb(64, 224, 208), 4); // Highlight winning positions
                    break;
                }
            }
        }
    }

    if (falling_piece.active) {
        int cx = falling_piece.col * CELL_SIZE + CELL_SIZE / 2;
        ALLEGRO_COLOR color = (falling_piece.player == PLAYER1)
            ? al_map_rgb(227, 66, 52)
            : al_map_rgb(255, 215, 0);
        al_draw_filled_circle(cx, falling_piece.y, CELL_SIZE / 2 - 5, color);
    }

    char score_text[64];
    sprintf(score_text, "Gracz 1 (Czerwony): %d     Gracz 2 (Zolty): %d", score1, score2);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, score_text);

    if (game_over) {const char* end_text = draw_game ? "Remis!" :
        (current_player == PLAYER1 ? "Gracz 1 wygrywa!" : "Gracz 2 wygrywa!");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 70, ALLEGRO_ALIGN_CENTER, end_text);
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, 130, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zrestartowac");
    }
}

bool play_turn(int col) {
    if (game_over || col < 0 || col >= COLS || board[0][col] != EMPTY || falling_piece.active) {
        return false;
    }

    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == EMPTY) {
            falling_piece.col = col;
            falling_piece.target_row = r;
            falling_piece.y = CELL_SIZE / 2;  // start animacji (góra)
            falling_piece.player = current_player;
            falling_piece.active = true;
            return true;
        }
    }

    return false;
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

//     ALLEGRO_FONT* font = al_load_ttf_font("../../assets/fonts/DejaVuSans.ttf", 48, 0);
    ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", 24, 0);
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

        } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            int col = ev.mouse.x / CELL_SIZE;
            if (col >= 0 && col < COLS) {
                selected_col = col;
            }

        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !game_over) {
            int col = ev.mouse.x / CELL_SIZE;
            play_turn(col);

        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
                reset_board();
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                selected_col = (selected_col - 1 + COLS) % COLS;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                selected_col = (selected_col + 1) % COLS;
            } else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER && !game_over) {
                play_turn(selected_col);
            }

        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (falling_piece.active) {
                float target_y = (falling_piece.target_row + 1) * CELL_SIZE + CELL_SIZE / 2;
                falling_piece.y += 20; // szybkość spadania

                if (falling_piece.y >= target_y) {
                    board[falling_piece.target_row][falling_piece.col] = falling_piece.player;

                    if (check_winner(falling_piece.player)) {
                        game_over = true;
                        if (falling_piece.player == PLAYER1) score1++;
                        else score2++;
                    } else if (is_board_full()) {
                        game_over = true;
                        draw_game = true;
                    } else {
                        current_player = (current_player == PLAYER1) ? PLAYER2 : PLAYER1;
                    }

                    falling_piece.active = false;
                }
            }

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