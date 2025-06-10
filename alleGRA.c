// gcc alleGRA.c -o connect4 \
//   $(pkg-config --cflags --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_main-5 allegro_image-5)

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define COLS 7
// #define ROWS 6
#define ROWS 6
#define CELL_SIZE 200
#define SCREEN_WIDTH (COLS * CELL_SIZE)
#define SCREEN_HEIGHT ((ROWS + 1) * CELL_SIZE)
#define FPS 60

enum { EMPTY, PLAYER1, PLAYER2 };

int board[ROWS][COLS];
int current_player = PLAYER1;
int score1 = 0, score2 = 0;
bool game_over = false;

void reset_board() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = EMPTY;
    current_player = PLAYER1;
    game_over = false;
}

bool place_piece(int col) {
    if (col < 0 || col >= COLS || game_over)
        return false;
    for (int row = ROWS - 1; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = current_player;
            return true;
        }
    }
    return false;
}

bool check_winner(int player) {
    // Sprawdź poziomo, pionowo i na skos
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (c <= COLS - 4 &&
                board[r][c] == player && board[r][c+1] == player &&
                board[r][c+2] == player && board[r][c+3] == player)
                return true;
            if (r <= ROWS - 4 &&
                board[r][c] == player && board[r+1][c] == player &&
                board[r+2][c] == player && board[r+3][c] == player)
                return true;
            if (c <= COLS - 4 && r <= ROWS - 4 &&
                board[r][c] == player && board[r+1][c+1] == player &&
                board[r+2][c+2] == player && board[r+3][c+3] == player)
                return true;
            if (c >= 3 && r <= ROWS - 4 &&
                board[r][c] == player && board[r+1][c-1] == player &&
                board[r+2][c-2] == player && board[r+3][c-3] == player)
                return true;
        }
    }
    return false;
}

void draw_board(ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(15, 27, 39));

    // Plansza
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

    // Interfejs
    char score_text[64];
    sprintf(score_text, "Gracz 1 (Czerwony): %d     Gracz 2 (Zolty): %d", score1, score2);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 10, ALLEGRO_ALIGN_CENTER, score_text);

    if (game_over) {
        const char* win_text = current_player == PLAYER1 ? "Gracz 1 wygrywa!" : "Gracz 2 wygrywa!";
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, 70, ALLEGRO_ALIGN_CENTER, win_text);
        al_draw_text(font, al_map_rgb(200, 200, 200), SCREEN_WIDTH / 2, 130, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zrestartowac");
    }
}

int main() {
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_FONT* font = al_load_ttf_font("/System/Library/Fonts/Supplemental/Arial.ttf", 48, 0);
    if (!font) {
        printf("Nie można załadować fontu!\n");
        return 1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

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
            }
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            draw_board(font);
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(font);
    al_destroy_timer(timer);
    return 0;
}
