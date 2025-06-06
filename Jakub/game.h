#ifndef GAME_H
#define GAME_H

#define ROWS 6
#define COLUMNS 7

typedef struct {
    int board[ROWS][COLUMNS];
    int current_player;
    int winner;
} GameState;

void init_game(GameState *state);
int drop_token(GameState *state, int column);
int check_winner(GameState *state);
void switch_player(GameState *state);

#endif
