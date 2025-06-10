#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define COLS 7
#define ROWS 6
#define CELL_SIZE 200
#define SCREEN_WIDTH (COLS * CELL_SIZE)
#define SCREEN_HEIGHT ((ROWS + 1) * CELL_SIZE)
#define FPS 60

enum { EMPTY, PLAYER1, PLAYER2 };

extern int board[ROWS][COLS];
extern int current_player;
extern int score1, score2;
extern bool game_over;
extern bool draw_game;

void reset_board();
bool place_piece(int col);
bool check_winner(int player);
bool is_board_full(void);

#endif