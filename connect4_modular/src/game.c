#include "../include/game.h"
#include <stdbool.h>

int board[ROWS][COLS];
int current_player = PLAYER1;
int score1 = 0, score2 = 0;
bool game_over = false;
bool draw_game = false;

void reset_board() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            board[i][j] = EMPTY;
    current_player = PLAYER1;
    game_over = false;
    draw_game = false;
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

bool is_board_full() {
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == EMPTY) {
            return false;
        }
    }
    return true;
}

bool play_turn(int col) {
    if (!place_piece(col)) {
        return false;
    }

    if (check_winner(current_player)) {
        game_over = true;
        draw_game = false;
        if (current_player == PLAYER1)
            score1++;
        else
            score2++;
    } else if (is_board_full()) {
        game_over = true;
        draw_game = true;
    } else {
        current_player = (current_player == PLAYER1) ? PLAYER2 : PLAYER1;
    }
    return true;
}
