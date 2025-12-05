#ifndef CONNECT4_BOARD_HPP
#define CONNECT4_BOARD_HPP

#include <array>
#include <utility>

class Board {
public:
    static constexpr int COLS = 7;
    static constexpr int ROWS = 6;
    static constexpr int EMPTY = 0;
    static constexpr int PLAYER1 = 1;
    static constexpr int PLAYER2 = 2;
    using Pos = std::pair<int,int>;

    Board();
    void reset();
    bool placePiece(int col, int player, int &out_row);
    bool isFull() const;
    bool checkWinner(int player);
    std::array<Pos,4> getWinningPositions() const;
    int at(int r, int c) const;

private:
    void resetWinningPositions();
    std::array<std::array<int, COLS>, ROWS> board_;
    std::array<Pos,4> winning_positions_;
};

#endif
