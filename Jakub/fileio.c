#include "fileio.h"
#include <stdio.h>

void save_game(GameState *state, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(state, sizeof(GameState), 1, f);
        fclose(f);
    }
}

int load_game(GameState *state, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f) {
        fread(state, sizeof(GameState), 1, f);
        fclose(f);
        return 1;
    }
    return 0;
}

void save_stats(int winner) {
    FILE *f = fopen("scores.txt", "a");
    if (f) {
        fprintf(f, "Player %d won\n", winner);
        fclose(f);
    }
}
