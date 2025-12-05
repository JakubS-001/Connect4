#include "../include/SaveManager.hpp"
#include "../include/Game.hpp"
#include <cstdio>

void SaveManager::loadScores(Game &game, const std::string &filename) {
    FILE* f = fopen(filename.c_str(), "r");
    if (f) {
        fscanf(f, "Gracz1: %d\n", &game.score1);
        fscanf(f, "Gracz2: %d\n", &game.score2);
        fclose(f);
    } else {
        game.score1 = 0;
        game.score2 = 0;
    }
}

void SaveManager::saveScores(const Game &game, const std::string &filename) {
    FILE* f = fopen(filename.c_str(), "w");
    if (f) {
        fprintf(f, "Gracz1: %d\n", game.score1);
        fprintf(f, "Gracz2: %d\n", game.score2);
        fclose(f);
    }
}

void SaveManager::resetScores(Game &game, const std::string &filename) {
    game.score1 = 0;
    game.score2 = 0;
    saveScores(game, filename);
}
