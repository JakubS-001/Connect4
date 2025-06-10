#include "../include/game.h"

int board[ROWS][COLS];
int current_player = PLAYER1;
int score1 = 0, score2 = 0;
bool game_over = false;
bool draw_game = false;

void reset_board() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = EMPTY;
        }
    }
    current_player = PLAYER1;
    game_over = false;
    draw_game = false;
}

bool place_piece(int col) {
    if (game_over || col < 0 || col >= COLS || board[0][col] != EMPTY) {
        return false;
    }

    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == EMPTY) {
            board[r][col] = current_player;
            return true;
        }
    }
    return false;
}

bool check_winner(int player) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r][c+1] == player && 
                board[r][c+2] == player && board[r][c+3] == player) {
                return true;
            }
        }
    }

    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == player && board[r+1][c] == player && 
                board[r+2][c] == player && board[r+3][c] == player) {
                return true;
            }
        }
    }

    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == player && board[r+1][c+1] == player && 
                board[r+2][c+2] == player && board[r+3][c+3] == player) {
                return true;
            }
        }
    }

    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 3; c < COLS; c++) {
            if (board[r][c] == player && board[r+1][c-1] == player && 
                board[r+2][c-2] == player && board[r+3][c-3] == player) {
                return true;
            }
        }
    }

    return false;
}

bool is_board_full(void) {
    for (int c = 0; c < COLS; c++) {
        if (board[0][c] == EMPTY) {
            return false;
        }
    }
    return true;
}
