#include "../include/Board.hpp"
#include <algorithm>

Board::Board() {
    reset();
}

void Board::resetWinningPositions() {
    for (auto &p : winning_positions_) p = {-1, -1};
}

void Board::reset() {
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            board_[r][c] = EMPTY;
    resetWinningPositions();
}

bool Board::placePiece(int col, int player, int &out_row) {
    if (col < 0 || col >= COLS) return false;
    if (board_[0][col] != EMPTY) return false;
    for (int r = ROWS - 1; r >= 0; --r) {
        if (board_[r][col] == EMPTY) {
            board_[r][col] = player;
            out_row = r;
            return true;
        }
    }
    return false;
}

bool Board::isFull() const {
    for (int c = 0; c < COLS; ++c) if (board_[0][c] == EMPTY) return false;
    return true;
}

int Board::at(int r, int c) const { return board_[r][c]; }

std::array<Board::Pos,4> Board::getWinningPositions() const { return winning_positions_; }

bool Board::checkWinner(int player) {
    // horizontal
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS - 3; ++c) {
            if (board_[r][c] == player && board_[r][c+1] == player && board_[r][c+2] == player && board_[r][c+3] == player) {
                winning_positions_[0] = {r, c};
                winning_positions_[1] = {r, c+1};
                winning_positions_[2] = {r, c+2};
                winning_positions_[3] = {r, c+3};
                return true;
            }
        }
    }
    // vertical
    for (int r = 0; r < ROWS - 3; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (board_[r][c] == player && board_[r+1][c] == player && board_[r+2][c] == player && board_[r+3][c] == player) {
                winning_positions_[0] = {r, c};
                winning_positions_[1] = {r+1, c};
                winning_positions_[2] = {r+2, c};
                winning_positions_[3] = {r+3, c};
                return true;
            }
        }
    }
    // diag down-right
    for (int r = 0; r < ROWS - 3; ++r) {
        for (int c = 0; c < COLS - 3; ++c) {
            if (board_[r][c] == player && board_[r+1][c+1] == player && board_[r+2][c+2] == player && board_[r+3][c+3] == player) {
                winning_positions_[0] = {r, c};
                winning_positions_[1] = {r+1, c+1};
                winning_positions_[2] = {r+2, c+2};
                winning_positions_[3] = {r+3, c+3};
                return true;
            }
        }
    }
    // diag down-left
    for (int r = 0; r < ROWS - 3; ++r) {
        for (int c = 3; c < COLS; ++c) {
            if (board_[r][c] == player && board_[r+1][c-1] == player && board_[r+2][c-2] == player && board_[r+3][c-3] == player) {
                winning_positions_[0] = {r, c};
                winning_positions_[1] = {r+1, c-1};
                winning_positions_[2] = {r+2, c-2};
                winning_positions_[3] = {r+3, c-3};
                return true;
            }
        }
    }
    return false;
}
