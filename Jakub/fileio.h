#ifndef FILEIO_H
#define FILEIO_H

#include "game.h"

void save_game(GameState *state, const char *filename);
int load_game(GameState *state, const char *filename);
void save_stats(int winner);

#endif
