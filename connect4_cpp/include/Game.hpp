#ifndef CONNECT4_GAME_HPP
#define CONNECT4_GAME_HPP

#include "Board.hpp"

class Game {
public:
    enum State { STATE_MENU, STATE_PLAYING };

    Game();
    void resetBoard();
    bool startPlayTurn(int col); // prepares falling piece, returns true if accepted
    void finalizeDrop(int row, int col, int player);
    int currentPlayer() const;
    void switchPlayer();
    bool isGameOver() const;
    bool isDraw() const;
    State getState() const;
    void setState(State s);

    int score1;
    int score2;

    Board board;
private:
    int current_player_;
    bool game_over_;
    bool draw_game_;
    State state_;
};

#endif
