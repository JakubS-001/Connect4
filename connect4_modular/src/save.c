#include "../include/save.h"
#include "../include/game.h"
#include <stdio.h>

#define SCORE_FILE "scores.txt"

void load_scores() {
    FILE* f = fopen(SCORE_FILE, "r");
    if (f) {
        fscanf(f, "Gracz1: %d\n", &score1);
        fscanf(f, "Gracz2: %d\n", &score2);
        fclose(f);
    } else {
        score1 = 0;
        score2 = 0;
    }
}

void save_scores() {
    FILE* f = fopen(SCORE_FILE, "w");
    if (f) {
        fprintf(f, "Gracz1: %d\n", score1);
        fprintf(f, "Gracz2: %d\n", score2);
        fclose(f);
    }
}

void reset_scores() {
    score1 = 0;
    score2 = 0;
    save_scores();
}