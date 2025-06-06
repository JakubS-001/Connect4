#include "game.h"
#include <string.h>

void init_game(GameState *state) {
    memset(state->board, 0, sizeof(state->board));
    state->current_player = 1;
    state->winner = 0;
}

int drop_token(GameState *state, int column) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (state->board[row][column] == 0) {
            state->board[row][column] = state->current_player;
            return row;
        }
    }
    return -1;
}

int check_winner(GameState *state) {
    int (*b)[COLUMNS] = state->board;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            int p = b[r][c];
            if (p == 0) continue;
            if (c + 3 < COLUMNS &&
                p == b[r][c+1] && p == b[r][c+2] && p == b[r][c+3]) return p;
            if (r + 3 < ROWS &&
                p == b[r+1][c] && p == b[r+2][c] && p == b[r+3][c]) return p;
            if (r + 3 < ROWS && c + 3 < COLUMNS &&
                p == b[r+1][c+1] && p == b[r+2][c+2] && p == b[r+3][c+3]) return p;
            if (r - 3 >= 0 && c + 3 < COLUMNS &&
                p == b[r-1][c+1] && p == b[r-2][c+2] && p == b[r-3][c+3]) return p;
        }
    }
    return 0;
}

void switch_player(GameState *state) {
    state->current_player = 3 - state->current_player;
}
