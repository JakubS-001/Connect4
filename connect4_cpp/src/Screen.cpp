#include "../include/Screen.hpp"
#include "../include/SaveManager.hpp"
#include "../include/Board.hpp"
#include <allegro5/allegro_primitives.h>
#include <cstdio>

static constexpr int CELL_SIZE = 90;
static constexpr int TOP_MARGIN = 120;
static constexpr int SCREEN_WIDTH = 640;
static constexpr int SCREEN_HEIGHT = CELL_SIZE * 6 + TOP_MARGIN;

MenuScreen::MenuScreen(Game &game) : Screen(game) {}

void MenuScreen::render(ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(15, 27, 39));
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, ALLEGRO_ALIGN_CENTER, "Connect 4");
    al_draw_text(font, al_map_rgb(200,200,200), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10, ALLEGRO_ALIGN_CENTER, "Kliknij ENTER aby zaczac");
    al_draw_text(font, al_map_rgb(200,200,200), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zresetowac statystyki");
    char stats[64];
    sprintf(stats, "Statystyki: Gracz 1: %d   Gracz 2: %d", game_.score1, game_.score2);
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 90, ALLEGRO_ALIGN_CENTER, stats);
}

void MenuScreen::handleEvent(const ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            game_.setState(Game::STATE_PLAYING);
            game_.resetBoard();
        } else if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
            SaveManager::resetScores(game_);
        }
    }
}

PlayScreen::PlayScreen(Game &game) : Screen(game) {}

void PlayScreen::render(ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(15, 27, 39));

    for (int r = 0; r < Board::ROWS; ++r) {
        for (int c = 0; c < Board::COLS; ++c) {
            int cx = c * CELL_SIZE + CELL_SIZE / 2;
            int cy = r * CELL_SIZE + TOP_MARGIN + CELL_SIZE / 2;
            ALLEGRO_COLOR color = al_map_rgb(44, 62, 80);
            if (game_.board.at(r,c) == Board::PLAYER1) color = al_map_rgb(227, 66, 52);
            else if (game_.board.at(r,c) == Board::PLAYER2) color = al_map_rgb(255, 215, 0);
            al_draw_filled_circle(cx, cy, CELL_SIZE / 2 - 5, color);
        }
    }

    char score_text[64];
    sprintf(score_text, "Gracz 1 (Czerwony): %d     Gracz 2 (Zolty): %d", game_.score1, game_.score2);
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, 10, ALLEGRO_ALIGN_CENTER, score_text);

    if (game_.isGameOver()) {
        const char* end_text = game_.isDraw() ? "Remis!" : (game_.currentPlayer() == Board::PLAYER1 ? "Gracz 1 wygrywa!" : "Gracz 2 wygrywa!");
        al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, 50, ALLEGRO_ALIGN_CENTER, end_text);
        al_draw_text(font, al_map_rgb(200,200,200), SCREEN_WIDTH/2, 80, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zrestartowac");
    }
}

void PlayScreen::handleEvent(const ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev.keyboard.keycode == ALLEGRO_KEY_R) {
            game_.resetBoard();
        } else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            game_.setState(Game::STATE_MENU);
        }
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        // simple click -> try to start a play turn (UI still handles falling animation)
        int col = ev.mouse.x / CELL_SIZE;
        if (game_.startPlayTurn(col)) {
            // no-op here: this class doesn't implement animation; integration point for UI
        }
    }
}
