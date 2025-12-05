#include "../include/Game.hpp"
#include "../include/SaveManager.hpp"

Game::Game()
    : score1(0), score2(0), current_player_(Board::PLAYER1), game_over_(false), draw_game_(false), state_(STATE_MENU)
{
}

void Game::resetBoard() {
    board.reset();
    current_player_ = Board::PLAYER1;
    game_over_ = false;
    draw_game_ = false;
}

bool Game::startPlayTurn(int col) {
    if (game_over_ || col < 0 || col >= Board::COLS) return false;
    // just check top cell
    if (board.at(0,col) != Board::EMPTY) return false;
    return true;
}

void Game::finalizeDrop(int row, int col, int player) {
    // place is already done by Board::placePiece in this design (UI will call placePiece to commit)
    if (board.checkWinner(player)) {
        game_over_ = true;
        if (player == Board::PLAYER1) score1++; else score2++;
        SaveManager::saveScores(*this);
    } else if (board.isFull()) {
        game_over_ = true;
        draw_game_ = true;
    } else {
        current_player_ = (current_player_ == Board::PLAYER1) ? Board::PLAYER2 : Board::PLAYER1;
    }
}

int Game::currentPlayer() const { return current_player_; }
void Game::switchPlayer() { current_player_ = (current_player_ == Board::PLAYER1) ? Board::PLAYER2 : Board::PLAYER1; }
bool Game::isGameOver() const { return game_over_; }
bool Game::isDraw() const { return draw_game_; }
Game::State Game::getState() const { return state_; }
void Game::setState(State s) { state_ = s; }
