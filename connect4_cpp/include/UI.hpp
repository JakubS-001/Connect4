#ifndef CONNECT4_UI_HPP
#define CONNECT4_UI_HPP

#include "Game.hpp"
#include "Screen.hpp"
#include <allegro5/allegro_font.h>
#include <memory>

class UI {
public:
    explicit UI(Game &game);
    void start();

private:
    Game &game_;
    struct FallingPiece {
        int col = 0;
        int target_row = 0;
        float y = 0;
        int player = 0;
        bool active = false;
    };
    FallingPiece fallingPiece_;

    bool playTurn(int col);

    // Drawing helpers used by UI
    void drawMenu(ALLEGRO_FONT* font);
    void drawBoard(ALLEGRO_FONT* font, int hover_col);

    std::unique_ptr<MenuScreen> menuScreen_;
    std::unique_ptr<PlayScreen> playScreen_;
    Screen* currentScreen_ = nullptr;
};

#endif
