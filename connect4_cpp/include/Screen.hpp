#ifndef CONNECT4_SCREEN_HPP
#define CONNECT4_SCREEN_HPP

#include "Game.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Screen {
public:
    explicit Screen(Game &game) : game_(game) {}
    virtual ~Screen() = default;

    // Render screen contents using provided font
    virtual void render(ALLEGRO_FONT* font) = 0;

    // Handle Allegro events relevant for the screen
    virtual void handleEvent(const ALLEGRO_EVENT &ev) = 0;

    // Called when screen becomes active/inactive
    virtual void onEnter() {}
    virtual void onExit() {}

protected:
    Game &game_;
};

// Forward-declare derived screens in this header for convenience
class MenuScreen : public Screen {
public:
    explicit MenuScreen(Game &game);
    void render(ALLEGRO_FONT* font) override;
    void handleEvent(const ALLEGRO_EVENT &ev) override;
};

class PlayScreen : public Screen {
public:
    explicit PlayScreen(Game &game);
    void render(ALLEGRO_FONT* font) override;
    void handleEvent(const ALLEGRO_EVENT &ev) override;
};

#endif
