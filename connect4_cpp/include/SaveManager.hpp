#ifndef CONNECT4_SAVE_MANAGER_HPP
#define CONNECT4_SAVE_MANAGER_HPP

#include <string>

class Game;

class SaveManager {
public:
    static void loadScores(Game &game, const std::string &filename="scores.txt");
    static void saveScores(const Game &game, const std::string &filename="scores.txt");
    static void resetScores(Game &game, const std::string &filename="scores.txt");
};

#endif
