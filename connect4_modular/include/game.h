#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define COLS 7
#define ROWS 6
#define CELL_SIZE 90
#define SCREEN_WIDTH 640
// #define SCREEN_HEIGHT 640
#define SCREEN_HEIGHT (CELL_SIZE * ROWS + TOP_MARGIN)
#define FPS 60
#define TOP_MARGIN 120


enum { EMPTY, PLAYER1, PLAYER2 };

extern int board[ROWS][COLS];
extern int current_player;
extern int score1, score2;
extern bool game_over;
extern bool draw_game;
extern int winning_positions[4][2];

void reset_board();
bool place_piece(int col);
bool check_draw();
bool check_winner(int player);
bool is_board_full(void);

typedef enum {
    STATE_MENU,
    STATE_PLAYING
} GameState;

extern GameState game_state;

#endif