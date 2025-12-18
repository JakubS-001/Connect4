#include "../include/UI.hpp"
#include "../include/SaveManager.hpp"
#include "../include/Screen.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <cstdio>
#include <memory>

static constexpr int CELL_SIZE = 90;
static constexpr int TOP_MARGIN = 120;
static constexpr int SCREEN_WIDTH = 640;
static constexpr int SCREEN_HEIGHT = CELL_SIZE * 6 + TOP_MARGIN;
static constexpr int FPS = 60;

UI::UI(Game &game) : game_(game) {
    menuScreen_ = std::make_unique<MenuScreen>(game_);
    playScreen_ = std::make_unique<PlayScreen>(game_);
    currentScreen_ = nullptr;
}

bool UI::playTurn(int col) {
    if (game_.isGameOver() || col < 0 || col >= Board::COLS || fallingPiece_.active) return false;
    for (int r = Board::ROWS - 1; r >= 0; --r) {
        if (game_.board.at(r,col) == Board::EMPTY) {
            fallingPiece_.col = col;
            fallingPiece_.target_row = r;
            fallingPiece_.y = CELL_SIZE / 2;
            fallingPiece_.player = game_.currentPlayer();
            fallingPiece_.active = true;
            return true;
        }
    }
    return false;
}

void UI::drawMenu(ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(15, 27, 39));
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 150, ALLEGRO_ALIGN_CENTER, "Connect 4");
    al_draw_text(font, al_map_rgb(200,200,200), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 10, ALLEGRO_ALIGN_CENTER, "Kliknij ENTER aby zaczac");
    al_draw_text(font, al_map_rgb(200,200,200), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50, ALLEGRO_ALIGN_CENTER, "Nacisnij R aby zresetowac statystyki");
    char stats[64];
    sprintf(stats, "Statystyki: Gracz 1: %d   Gracz 2: %d", game_.score1, game_.score2);
    al_draw_text(font, al_map_rgb(255,255,255), SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 90, ALLEGRO_ALIGN_CENTER, stats);
}

void UI::drawBoard(ALLEGRO_FONT* font, int hover_col) {
    al_clear_to_color(al_map_rgb(15, 27, 39));
    if (hover_col >= 0 && hover_col < Board::COLS) {
        ALLEGRO_COLOR highlight_color = (game_.currentPlayer() == Board::PLAYER1) ? al_map_rgba(255,127,127,120) : al_map_rgba(255,255,150,120);
        // Draw an outline rectangle (under the pieces) so only the border is visible
        float x1 = hover_col * CELL_SIZE + 2.0f;
        float y1 = TOP_MARGIN + 2.0f;
        float x2 = (hover_col + 1) * CELL_SIZE - 2.0f;
        float y2 = SCREEN_HEIGHT - 2.0f;
        al_draw_rectangle(x1, y1, x2, y2, highlight_color, 4.0f);
    }

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

    if (fallingPiece_.active) {
        int cx = fallingPiece_.col * CELL_SIZE + CELL_SIZE / 2;
        ALLEGRO_COLOR color = (fallingPiece_.player == Board::PLAYER1) ? al_map_rgb(227,66,52) : al_map_rgb(255,215,0);
        al_draw_filled_circle(cx, fallingPiece_.y + TOP_MARGIN, CELL_SIZE / 2 - 5, color);
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

void UI::start() {
    if (!al_init()) {
        fprintf(stderr, "Nie mozna zainicjowac Allegro!\n");
        return;
    }

    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) { fprintf(stderr, "Blad: nie udalo sie utworzyc okna\n"); return; }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", 24, 0);
    if (!font) { fprintf(stderr, "Blad: nie udalo sie zaladowac czcionki Arial.ttf\n"); al_destroy_display(display); return; }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool running = true;
    bool redraw = true;
    int selected_col = 0;

    game_.resetBoard();
    SaveManager::loadScores(game_);
    game_.setState(Game::STATE_MENU);
    // set initial screen
    currentScreen_ = menuScreen_.get();
    if (currentScreen_) currentScreen_->onEnter();
    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (game_.getState() == Game::STATE_PLAYING) {
                int col = ev.mouse.x / CELL_SIZE;
                if (col >= 0 && col < Board::COLS) selected_col = col;
            }
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (game_.getState() == Game::STATE_PLAYING && !game_.isGameOver()) {
                int col = ev.mouse.x / CELL_SIZE;
                playTurn(col);
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (game_.getState() == Game::STATE_MENU) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                } else {
                    // delegate menu key handling
                    if (menuScreen_) menuScreen_->handleEvent(ev);
                }
            } else if (game_.getState() == Game::STATE_PLAYING) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_R) { game_.resetBoard(); }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) selected_col = (selected_col - 1 + Board::COLS) % Board::COLS;
                else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) selected_col = (selected_col + 1) % Board::COLS;
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER && !game_.isGameOver()) { playTurn(selected_col); }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) game_.setState(Game::STATE_MENU);
                else {
                    // allow play-screen-specific key handling
                    if (playScreen_) playScreen_->handleEvent(ev);
                }
            }
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (game_.getState() == Game::STATE_PLAYING && fallingPiece_.active) {
                float target_y = (fallingPiece_.target_row) * CELL_SIZE + CELL_SIZE / 2;
                fallingPiece_.y += 20;
                if (fallingPiece_.y >= target_y) {
                    int placed_row = fallingPiece_.target_row;
                    int placed_col = fallingPiece_.col;
                    // commit to board
                    int out_row = -1;
                    game_.board.placePiece(placed_col, fallingPiece_.player, out_row);
                    game_.finalizeDrop(placed_row, placed_col, fallingPiece_.player);
                    fallingPiece_.active = false;
                }
            }
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            // switch screens if needed
            Screen* newScreen = (game_.getState() == Game::STATE_MENU) ? static_cast<Screen*>(menuScreen_.get()) : static_cast<Screen*>(playScreen_.get());
            if (newScreen != currentScreen_) {
                if (currentScreen_) currentScreen_->onExit();
                currentScreen_ = newScreen;
                if (currentScreen_) currentScreen_->onEnter();
            }

            if (currentScreen_) currentScreen_->render(font);

            // draw UI-specific overlays (selection highlight, falling piece)
            if (game_.getState() == Game::STATE_PLAYING) {
                if (selected_col >= 0 && selected_col < Board::COLS) {
                    ALLEGRO_COLOR highlight_color = (game_.currentPlayer() == Board::PLAYER1) ? al_map_rgba(255,127,127,120) : al_map_rgba(255,255,150,120);
                    float x1 = selected_col * CELL_SIZE + 2.0f;
                    float y1 = TOP_MARGIN + 2.0f;
                    float x2 = (selected_col + 1) * CELL_SIZE - 2.0f;
                    float y2 = SCREEN_HEIGHT - 2.0f;
                    al_draw_rectangle(x1, y1, x2, y2, highlight_color, 4.0f);
                }

                if (fallingPiece_.active) {
                    int cx = fallingPiece_.col * CELL_SIZE + CELL_SIZE / 2;
                    ALLEGRO_COLOR color = (fallingPiece_.player == Board::PLAYER1) ? al_map_rgb(227,66,52) : al_map_rgb(255,215,0);
                    al_draw_filled_circle(cx, fallingPiece_.y + TOP_MARGIN, CELL_SIZE / 2 - 5, color);
                }
            }

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_font(font);
}
